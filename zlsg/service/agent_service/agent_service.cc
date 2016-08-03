#include "client.auth.pb.h"
#include "client.role_login.pb.h"
#include "client.script.pb.h"
#include "client.node.pb.h"
#include "mysql.t_login.pb.h"
#include "server.node.pb.h"
#include "pb_coder.h"
#include "pb_mgr.h"
#include "msg_handler_pool.h"
#include "md5_encrypt.h"
#include "base_time.h"
#include "agent_interface.h"
#include "agent_service.h"

//#define CAPTURE_CLIENT_PACKAGE

#ifdef CAPTURE_CLIENT_PACKAGE

#include <iostream>
#include <fstream>
#include "base_time.h"

static void capture_client_package(const char* msg, size_t sz)
{
    char* buf = (char*)malloc(sz + 8);

    *(uint32*)buf = (uint32)sz;
    uint32 offset = sizeof(uint32);

    *(uint32*)(buf + offset) = time_utils::tenms();
    offset += sizeof(uint32);

    memcpy(buf + offset, msg, sz);
    offset += sz;

    ofstream outfile ("./client_package.data", ofstream::binary|ofstream::app);
    outfile.write(buf, offset);
    outfile.close();
    free(buf);
}

#endif

#define OPEN_ID_LEN 32
#define OPEN_KEY_LEN 32
#define MD5_SIGN_LEN 32
#define C_MD5_SIGN_LEN 16
#define MD5_SVR_KEY     "lhsg$%#*bi03g34ns0j3r0ckpi1e!@*)" // 用于客户端登录PHP的相关校验
#define MD5_GMS_KEY     "aBEsf998fuds908DA8fdkllsajdsafsF"
#define MD5_CUSGMS_KEY  "3y789fyf8hf34fu0suivjcxv89la98s2"
#define MD5_SVR_KEY_LEN 32
#define AUTH_FIX_TIME   600

static string PFS[3] = {
    "pengyou",
    "qzone",
    "website"
};

static int agent_dispatch_msg(struct amtsvr_context* ctx, agent_service_t* a, const char* dst_name, google::protobuf::Message* pb);

static int auth_by_md5(uint32 now, client::auth_req* req)
{
//#ifdef __INNER_DEBUG__PACKAGE_
//    return 0;
//#endif

    if (/*req->openid().length() != OPEN_ID_LEN 
        || req->openkey().length() != OPEN_KEY_LEN
        ||*/ req->sign().length() != MD5_SIGN_LEN) {
            return -1;
    }

    char str_buf[512]= {0};
    if (!req->has_effect_time()) {
        return -2;
    } 

    if ((now - AUTH_FIX_TIME) > req->effect_time()) {
        return -2;
    }

    snprintf(str_buf, sizeof(str_buf), "%s%s%u%s%u", 
        req->openid().c_str(), req->openkey().c_str(), req->yel_vip(), MD5_SVR_KEY, req->effect_time());

    unsigned char digest[C_MD5_SIGN_LEN+1] = {0};
    MD5_CTX md5;  
    MD5Init(&md5);
    MD5Update(&md5, (unsigned char*)str_buf, strlen(str_buf));  
    MD5Final(&md5, digest);

    char req_sign[64] = {0};
    snprintf(req_sign, sizeof(req_sign), "%s", req->sign().c_str());

    char ret[64] = {0};
    int result = 0;
    for (int i = 0, j = 0; i < MD5_SIGN_LEN; i += 2) {
        sprintf(ret + i, "%02x", digest[j++]);
    }

    //printf("md5 [%s]->[%s] req_sign:%s\n", str_buf, ret, req_sign);
    if (memcmp(ret, req->sign().c_str(), MD5_SIGN_LEN) != 0) {
        result = -1;
    }

    if (result == 0) {
        uint32 k = 0;
        if (req->has_pf()) {
            for (; k < 3; ++k) {
                if (PFS[k].compare(req->pf()) == 0) {
                    break;
                }
            }
        }
        req->set_platform_id(k);
    }

    return result;
}

static bool do_auth_req(struct amtsvr_context* ctx, agent_service_t* a, client::auth_req* req)
{
    if (auth_by_md5(a->last_rcv_time, req) != 0) {
        delete req;
        return false;
    }

    a->login_auth_pass = true;
    a->svr_id = req->svr_id();
    snprintf(a->user_name, sizeof(a->user_name), "%s", req->openid().c_str());
    //if (req->has_pf()) {
    //    amtsvr_billlog(a->ctx, "%d|%s|%s|%s|%d|%s", a->role_id, _req->GetTypeName().c_str(), _req->openid().c_str(), _req->pf().c_str(), a->svr_id, a->ip);
    //} else {
    //    amtsvr_billlog(a->ctx, "%d|%s|%s|unknow|%d|%s", a->role_id, _req->GetTypeName().c_str(), _req->openid().c_str(), a->svr_id, a->ip);
    //}

    // 记录登录日志
    mysql::t_login* log = new mysql::t_login;
    log->set_id          (0);
    log->set_sid         (req->sid());
    log->set_did         (req->did());
    log->set_accountname (req->openid());
    log->set_platform    (req->pf());
    log->set_channel_id  (req->channel_id());
    log->set_appversion  (req->app_version());
    log->set_apptype     (req->app_type());
    log->set_device      (req->device());
    log->set_nm          (req->nm());
    log->set_mno         (req->mno());
    log->set_screen      (req->screen());
    log->set_os          (req->os());
    log->set_mac         (req->mac());
    log->set_ip          (a->ip);
    log->set_createtime  ((uint32)time(NULL));
    agent_send_log(a, log);

    const char* dst_name = CLIENT_PB_MGR->get_dst_name(req->GetTypeName().c_str());
    if (dst_name == NULL) {
        amtsvr_log("protobuff name:%s not reg\n", req->GetTypeName().c_str());
        delete req;
        return false;
    }

    set_svr_head(pb_msg_svrhead(req), a);

    if (agent_dispatch_msg(ctx, a, dst_name, req) != 0) {
        delete req;
        return false;
    }

    return true;
}

static void do_register_node_req(agent_service_t* a, server::register_node_req* req)
{
    // 标识此agent为node
    a->is_node = true;
    delete req;
}

static void do_auto_login_req(agent_service_t* a, client::auto_login_req* req)
{
    msg_handler* mh = MSG_HANDLER_POOL->pop();
    if (mh == NULL) {
        amtsvr_log("%s\n", req->DebugString().c_str());
        delete req;
        return ;
    }

    //client::auto_login_req* msg = new client::auto_login_req(*req);
    //mh->msg = msg;
    mh->msg = req;
    amtsvr_sendname(a->ctx, CENTER_NAME, PTYPE_CLIENT | PTYPE_TAG_DONTCOPY, 0, (void*)mh, sizeof(msg_handler));
}

static int do_role_login_req(struct amtsvr_context* ctx, agent_service_t* a, int session, client::role_login_req* req)
{
    client::role_login_rsp rsp;
    rsp.set_ret(client::RLRE_SUCCESS);
    if (req->role_id() == 0) {
        rsp.set_ret(client::RLRE_NOT_EXIST);
    } else {
        int i = 0;
        bool bfind = false;
        for (; i < MAX_ROLE_PER_USER && a->rir[i] != NULL; ++i) {
            if (a->rir[i]->role_id() == req->role_id()) {
                bfind = true;
                a->role_id = req->role_id();
                break;
            }
        }

        if (!bfind) {
            rsp.set_ret(client::RLRE_NOT_BELONG);
        } else {
            char str[32] = {0};
            snprintf(str, 32, "%c%d%c%c%c", (rand() % 25 + 65), rand(), (rand() % 25 + 65), (rand() % 25 + 65), (rand() % 25 + 65));
            rsp.set_key(str);
            msg_handler* mh = MSG_HANDLER_POOL->pop();
            if (mh != NULL) {
                client::update_auto_login_key* via = new client::update_auto_login_key;
                via->set_role_id(req->role_id());
                via->set_key(str);
                mh->msg = via;
                amtsvr_sendname(a->ctx, CENTER_NAME, PTYPE_CLIENT | PTYPE_TAG_DONTCOPY, 0, (void*)mh, sizeof(msg_handler));
            }

            mh = MSG_HANDLER_POOL->pop();
            if (mh != NULL) {
                mysql::sp_login_auth* sla = new mysql::sp_login_auth();
                sla->set_v_user_id  (a->user_id);
                sla->set_v_role_id  (req->role_id());
                sla->set_v_sex      (0);
                sla->set_v_prof     (0);
                sla->set_v_name     (a->user_name);
                sla->set_p_openid   ("");
                sla->set_p_yel_vip  (0);
                sla->set_p_server_id(0);
                sla->set_p_agent_id (a->self);
                sla->set_p_client_id(a->client);
                mh->msg = sla;
                amtsvr_sendname(a->ctx, CENTER_NAME, PTYPE_CLIENT | PTYPE_TAG_DONTCOPY, 0, (void*)mh, sizeof(msg_handler));
            }
        }
    }

    size_t rsp_len = 0;
    void* rl_rsp_buf = encode(rsp, rsp_len);
    amtsvr_send(ctx, 0, a->client, PTYPE_CLIENT | PTYPE_TAG_DONTCOPY, session, rl_rsp_buf, rsp_len);

    delete req;

    return 0;
}

//static google::protobuf::Message* agent_auth_client_msg(agent_service_t* a, const void* msg, size_t sz, char* type_name)
//{
//    capture_client_package((const char*)msg, sz);
//
//    if (++(a->package_index) == 0) {
//        a->package_index = 1;
//    }
//
//    google::protobuf::Message* pb = decode((char*)msg, sz, type_name, 0);//a->package_index);
//    if (pb == NULL) {
//        return NULL;
//    }
//
//    if (strcmp(type_name, "client.auth_req") == 0) {
//    } else if (strcmp(type_name, "client.auto_login_req") == 0 && !a->login_auth_pass) {
//        set_svr_head(pb_msg_svrhead(pb), a);
//        do_auto_login_req(a, static_cast<client::auto_login_req*>(pb));
//        return NULL;
//    } else if (!a->login_auth_pass) {
//        delete pb;
//        return NULL;
//    }
//
//    return pb;
//}

static int agent_dispatch_script(struct amtsvr_context* ctx, agent_service_t* a, const char* dst_name, const void* msg, size_t sz, const char* type_name)
{
    client::script_pb_msg* spm = new client::script_pb_msg;
    spm->set_type_name(type_name);
    msg = decode_pb_bin((char*)msg, sz, type_name, 0);
    string bin((char*)msg, sz);
    spm->set_bin(bin);
    set_svr_head(spm->mutable_svr_head(), a);

    if (strcmp(dst_name, GAME_NAME) == 0) {
        if (a->scene_server_id == 0) {
            return -1;
        }

        // 跨服消息转发
        //////

        msg_handler* mh = MSG_HANDLER_POOL->pop();
        if (mh == NULL) {
            return -2;
        }

        mh->msg = spm;

        amtsvr_send(ctx, 0, a->scene_server_id, PTYPE_SCRIPT | PTYPE_TAG_DONTCOPY, 0, (void*)mh, sizeof(msg_handler));
    } else {
        msg_handler* mh = MSG_HANDLER_POOL->pop();
        if (mh == NULL) {
            return -3;
        }

        mh->msg = spm;

        amtsvr_sendname(ctx, dst_name, PTYPE_SCRIPT | PTYPE_TAG_DONTCOPY, 0, (void*)mh, sizeof(msg_handler));
    }

    return 0;
}

static int agent_dispatch_msg(struct amtsvr_context* ctx, agent_service_t* a, const char* dst_name, google::protobuf::Message* pb)
{
    if (strcmp(dst_name, GAME_NAME) == 0) {
        if (a->scene_server_id == 0) {
            return -1;
        }

        msg_handler* mh = MSG_HANDLER_POOL->pop();
        if (mh == NULL) {
            return -2;
        }

        mh->msg = pb;

        amtsvr_send(ctx, 0, a->scene_server_id, PTYPE_CLIENT | PTYPE_TAG_DONTCOPY, 0, (void*)mh, sizeof(msg_handler));
    } else {
        msg_handler* mh = MSG_HANDLER_POOL->pop();
        if (mh == NULL) {
            return -3;
        }

        mh->msg = pb;

        amtsvr_sendname(ctx, dst_name, PTYPE_CLIENT | PTYPE_TAG_DONTCOPY, 0, (void*)mh, sizeof(msg_handler));
    }

    return 0;
}

static void agent_check_time(struct amtsvr_context* ctx, agent_service_t* a, int session, client::time_check* tc)
{
    tc->set_timestamp(time_utils::tenms());
    size_t rsp_len = 0;
    void* rsp_buf = encode(*tc, rsp_len);
    if (rsp_buf != NULL) {
        amtsvr_send(ctx, 0, a->client, PTYPE_CLIENT | PTYPE_TAG_DONTCOPY, session, rsp_buf, rsp_len);
    }

    delete tc;
}

static int agent_proc_client_msg(struct amtsvr_context* ctx, agent_service_t* a, int type, int session, uint32_t source, const void* msg, size_t sz)
{
    if (a->shutdown) {
        return 0;
    }

    uint32 now = (uint32)time(NULL);
    a->last_rcv_time = now;

    //google::protobuf::Message* pb = agent_auth_client_msg(a, msg, sz, type_name);
    //if (pb == NULL) {
    //    return 0;
    //}

    char type_name[MAX_PB_NAME_LEN] = {0};
    if (!decode_type_name((char*)msg, sz, type_name, 0)) {
        return 0;
    }

    google::protobuf::Message* pb = NULL;
    if (strcmp(type_name, "client.auth_req") == 0) {
        pb = decode_pb((char*)msg, sz, type_name, 0);
        if (pb != NULL && !do_auth_req(ctx, a, static_cast<client::auth_req*>(pb))) {
        }
        return 0;
    } else if (strcmp(type_name, "client.auto_login_req") == 0 && !a->login_auth_pass) {
        pb = decode_pb((char*)msg, sz, type_name, 0);
        if (pb != NULL) {
            set_svr_head(pb_msg_svrhead(pb), a);
            do_auto_login_req(a, static_cast<client::auto_login_req*>(pb));
        }

        return 0;
    } else if (strcmp(type_name, "server.register_node_req") == 0 && !a->login_auth_pass) {
        pb = decode_pb((char*)msg, sz, type_name, 0);
        if (pb != NULL) {
            do_register_node_req(a, static_cast<server::register_node_req*>(pb));
        }

        return 0;
    } else if (!a->login_auth_pass) {
        return 0;
    } else if (a->is_node) {
        return 0;
    }

    if (memcmp(type_name, "client.role_login_req", sizeof("client.role_login_req")) == 0) {
        pb = decode_pb((char*)msg, sz, type_name, 0);
        if (pb != NULL) {
            do_role_login_req(ctx, a, session, static_cast<client::role_login_req*>(pb));
        }
        return 0;
    } else if (memcmp(type_name, "client.time_check", sizeof("client.time_check")) == 0) {
        pb = decode_pb((char*)msg, sz, type_name, 0);
        if (pb != NULL) {
            agent_check_time(ctx, a, session, static_cast<client::time_check*>(pb));
        }
        return 0;
    } else if (memcmp(type_name, "client.heart", sizeof("client.heart")) == 0) {
        return 0;
    }

    const char* dst_name = CLIENT_PB_MGR->get_dst_name(type_name);
    if (dst_name == NULL) {
        amtsvr_log("protobuff name:%s not reg\n", type_name);
        return 0;
    }

    if (CLIENT_PB_MGR->is_script(type_name)) {
        return agent_dispatch_script(ctx, a, dst_name, msg, sz, type_name);
    }

    pb = decode_pb((char*)msg, sz, type_name, 0);
    if (pb == NULL) {
        amtsvr_log("protobuff name:%s create failed\n", type_name);
        return 0;
    }

    set_svr_head(pb_msg_svrhead(pb), a);

    if (agent_dispatch_msg(ctx, a, dst_name, pb) != 0) {
        delete pb;
    }

    return 0;
}

static int agent_proc_svr_msg(struct amtsvr_context* ctx, agent_service_t* a, int type, int session, uint32_t source, const void* msg, size_t sz)
{
    google::protobuf::Message* pb = svr_decode((const char*)msg, (int)sz);
    if (pb == NULL) {
        return 0;
    }

    size_t len = 0;
    void* rsp = a->pd.on_pb_msg(pb, a, &len);
    delete pb;
    if (rsp != NULL) {
        amtsvr_send(ctx, 0, a->client, PTYPE_CLIENT | PTYPE_TAG_DONTCOPY, session, rsp, len);
    }

    return 0;
}

static int agent_proc_inner_msg(struct amtsvr_context* ctx, agent_service_t* a, int type, int session, uint32_t source, const void* msg, size_t sz)
{
    if (a->shutdown) {
        return 0;
    }

    char type_name[MAX_PB_NAME_LEN] = {0};
    google::protobuf::Message* pb = decode_inner((char*)msg, sz, type_name);
    if (pb == NULL) {
        return 0;
    }

    set_inner_head(pb_msg_inhead(pb), a);

    msg_handler* mh = MSG_HANDLER_POOL->pop();
    if (mh == NULL) {
        return 0;
    }

    mh->msg = pb;

    amtsvr_sendname(ctx, CENTER_NAME, PTYPE_CLIENT | PTYPE_TAG_DONTCOPY, 0, (void*)mh, sizeof(msg_handler));

    return 0;
}

static int start_node_agent(agent_service_t* service, server::node_role_req* req)
{
    int id = req->role_id();
    //if (service->link_agent.empty()) {
    //    id = 1;
    //} else {
    //    id = service->link_agent.rbegin()->first + 1;
    //}

    service->link_agent[id] = 0;

    char cmd[128] = {0};
    snprintf(cmd, sizeof(cmd), "open_node %d, %d", id, service->index);
    agent_send_watchdog(service, cmd);

    return 0;
}

static int agent_proc_node_msg(struct amtsvr_context* ctx, agent_service_t* service, int type, int session, uint32_t source, const void* msg, size_t sz)
{
    char type_name[MAX_PB_NAME_LEN] = {0};
    if (!decode_type_name((char*)msg, sz, type_name, 0)) {
        return 0;
    }

    // if 登陆协议 启动新的agent sender
    google::protobuf::Message* pb = NULL;
    if (strcmp(type_name, "server.forward_msg") == 0) {    // else 交互协议
        pb = decode_pb((char*)msg, sz, type_name, 0);
        if (pb != NULL) {
            server::forward_msg* msg = static_cast<server::forward_msg*>(pb);
            // 获取协议中的ID        // 获取ID对应的agent
            uint32 agent_id = service->link_agent[msg->id()];
            // 转发消息至agent
            amtsvr_send(service->ctx, 0, agent_id, PTYPE_CLIENT, 0, (void*)msg->data().c_str(), (size_t)msg->data().length());
            delete pb;
        }
    } else if (strcmp(type_name, "server.node_role_req") == 0) {
        pb = decode_pb((char*)msg, sz, type_name, 0);
        if (pb != NULL) {
            start_node_agent(service, static_cast<server::node_role_req*>(pb));
            delete pb;
        }
    } else if (strcmp(type_name, "server.node_agent_rsp") == 0) {
        pb = decode_pb((char*)msg, sz, type_name, 0);
        if (pb != NULL) {
            server::node_agent_rsp* rsp = static_cast<server::node_agent_rsp*>(pb);
            service->link_agent[rsp->role_id()] = rsp->agent_id();

            // 通知节点agent准备好 可以接收数据
            server::forward_msg fm;
            fm.set_id(rsp->role_id());
            fm.set_type(server::forward_msg_target_server);
            client::agent_ready ar;
            ar.set_unixtime(0);
            fm.set_data(ar.SerializeAsString());
            agent_send_client(service, 0, fm);

            delete pb;
        }
    }
//      else if (strcmp(type_name, "server.user_data") == 0) {
//         pb = decode_pb((char*)msg, sz, type_name, 0);
//         if (pb != NULL) {
//             server::user_data* msg = static_cast<server::user_data*>(pb);
//             uint32 agent_id = service->link_agent[pb->role_id()];
//             amtsvr_send(service->ctx, 0, agent_id, PTYPE_CLIENT, 0, (void*)msg->data().c_str(), (size_t)msg->data().length());
//             delete pb;
//         }
//     }

    return 0;
}

static int agent_proc_msg_cb(struct amtsvr_context* ctx, void* ud, int type, int session, uint32_t source, const void* msg, size_t sz)
{
    agent_service_t* a = (agent_service_t*)ud;
    if (source == a->client || source == a->self) {
        if (!a->inner && !a->is_node) {
            agent_proc_client_msg(ctx, a, type, session, source, msg, sz);
        } else if (!a->inner && a->is_node) {
            agent_proc_node_msg(ctx, a, type, session, source, msg, sz);
        } else if (a->inner && !a->is_node) {
            agent_proc_inner_msg(ctx, a, type, session, source, msg, sz);
        }

    } else if (type == PTYPE_TIMER) { 
        /*uint32 now = (uint32)time(NULL);
        if (!a->login_auth_pass) {
        amtsvr_syslog(ctx, "agent service|close client, becasue not login pass|%s|%s|", a->ip, a->user_name);
        kick_agent(a); 
        } else if (now > a->last_rcv_time + AGENT_HEART_INTERVAL) {
        amtsvr_syslog(ctx, "agent service|close client, becasue loss connection|%s|%s|", a->ip, a->user_name);
        lose_agent(a); 
        } else {
        amtsvr_command(a->ctx, "TIMEOUT", AGENT_CHECK_INTERVAL);
        }*/
    } else {
        agent_proc_svr_msg(ctx, a, type, session, source, msg, sz);
        if (a->exit) {
            amtsvr_command(ctx, "EXIT", NULL);
            amtsvr_syslog(ctx, "agent service|EXIT client, because svrmsg cmd is exit|%s|%s|", a->ip, a->user_name);
        }
    }

    // node节点转发的msg 需要free()

    return 0;
}

struct agent_service_t* agent_create(void)
{
    struct agent_service_t* a = new agent_service_t;

    return a;
}

int agent_init(struct agent_service_t* a, struct amtsvr_context* ctx, char* args)
{
    sscanf(args, "%u %u %u %s %d %d", &a->gate, &a->watch_dog, &a->client, a->ip, &a->index, &a->inner);
    if (a->gate == 0 || a->watch_dog == 0 || a->client == 0) {
        return 1;
    }

    if (agesvr_dispatcher_init(a) != 0) {
        return 1;
    }

    uint32 now = (uint32)time(NULL);
    a->connect_time  = now;
    a->last_rcv_time = now;
    a->recv_count    = 0;
    a->send_count    = 0;
    a->ctx           = ctx;

    a->self = strtoul(amtsvr_command(ctx, "REG", NULL)+1 , NULL, 16);

    amtsvr_callback(ctx, (void*)a, agent_proc_msg_cb); 

    if (a->inner == 0) {  // 内部接口不再增加定时器机制
        amtsvr_command(a->ctx, "TIMEOUT", AGENT_CHECK_INTERVAL);
    }

    return 0;
}

void agent_release(struct agent_service_t* a)
{
    printf("%s:%d agent service|agent_release|%s|%s|\n", __FILE__, __LINE__, a->ip, a->user_name);
    for (int i = 0; i < MAX_ROLE_PER_USER; ++i) {
        if (a->rir[i] != NULL) {
            delete a->rir[i];
        }
    }

    delete a;
}
