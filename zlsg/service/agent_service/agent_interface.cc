#include "pb_mgr.h"
#include "client.auth.pb.h"
#include "client.role_login.pb.h"
#include "client.role_pos_msg.pb.h"
#include "client.offline.pb.h"
#include "pb_coder.h"
#include "inner_interface.h"
#include "msg_handler_pool.h"
#include "pb_agent.h"
#include "agent_interface.h"

static void* on_default_proc_msg(google::protobuf::Message* msg, void* ud, size_t* rsp_len)
{
    return encode(*msg, *rsp_len);
}

static void* on_client_auth_rsp(client::auth_rsp* rsp, void* ud, size_t* rsp_len)
{
    agent_service_t* a = static_cast<agent_service_t*>(ud);
    if (rsp->ret() == client::ARE_SUCCESS) {
        a->user_id = rsp->user_id();
        a->login_auth_pass = true;
        for (int i = 0; i < rsp->role_info_size() && i < MAX_ROLE_PER_USER; ++i) {
            if (a->rir[i] != NULL) {
                delete a->rir[i];
            }

            a->rir[i] = new client::role_info_rsp(rsp->role_info(i));
        }
    }

    return on_default_proc_msg(rsp, ud, rsp_len);
}


static void* on_client_role_info_rsp(client::role_info_rsp* rsp, void* ud, size_t* rsp_len)
{
    agent_service_t* a = static_cast<agent_service_t*>(ud);

    bool success = false;
    if (rsp->role_id() > 0) {
        for (int i = 0; i < MAX_ROLE_PER_USER; ++i) {
            if (a->rir[i] == NULL) {
                a->rir[i] = new client::role_info_rsp(*rsp);
                success = true;
                a->role_id = rsp->role_id();
                break;
            }
        }
    }

    if (!success) {
        // TODO: 断开连接或者增加新协议通知客户端创建失败
    }

    return on_default_proc_msg(rsp, ud, rsp_len);
}

static void* on_client_update_agent_role(client::update_agent_role* via, void* ud, size_t* rsp_len)
{
    agent_service_t* a = static_cast<agent_service_t*>(ud);
    a->role_id         = via->role_id();
    a->svr_id          = GET_SVR_ID(via->role_id());
    a->login_auth_pass = true;
    return on_default_proc_msg(via, ud, rsp_len);
}

static void* on_role_pos_msg(client::role_pos_msg* rsp,  void* ud, size_t* rsp_len)
{
    agent_service_t* a   = static_cast<agent_service_t*>(ud);
    a->scene_server_id   = rsp->svr_head().scenesvr_id();
    a->scene_server_type = rsp->type();

    if (rsp->type() == 1) { // 跨服准备发送玩家数据去中心服
        msg_handler* mh = MSG_HANDLER_POOL->pop();
        if (mh == NULL) {
            return NULL;
        }

        server::node_role_req* req = new server::node_role_req;
        req->set_role_id(a->role_id);
        req->set_agent_id(a->index);
        req->set_client_id(a->client);
        mh->msg = req;
        if (NODE_NAMES.size() > 0) {
            amtsvr_sendname(a->ctx, NODE_NAMES[0].c_str(), PTYPE_CLIENT | PTYPE_TAG_DONTCOPY, 0, (void*)mh, sizeof(msg_handler));
        } else {
            mh->msg = NULL;
            MSG_HANDLER_POOL->push(mh);
            delete req;
        }
    } else if (rsp->type() == 2) {
        return NULL;
    }

    return on_default_proc_msg(rsp, ud, rsp_len);
}

void kick_agent(struct agent_service_t* a)
{
    char cmd[128] = {0};
    snprintf(cmd, sizeof(cmd), "shutdown %d", a->index);
    amtsvr_send(a->ctx, 0, a->watch_dog, PTYPE_TEXT, 0, cmd, strlen(cmd)+1);
}

void lose_agent(struct agent_service_t* a)
{
    char cmd[128] = {0};
    snprintf(cmd, sizeof(cmd), "lose_conn %d", a->index);
    amtsvr_send(a->ctx, 0, a->watch_dog, PTYPE_TEXT, 0, cmd, strlen(cmd)+1);
}

static void* on_offline_msg(client::offline* o, void* ud, size_t* rsp_len)
{
    agent_service_t* a = static_cast<agent_service_t*>(ud);
    if (a->scene_server_id == 0) {
        a->exit = true;
        return NULL;
    }

    msg_handler* mh = MSG_HANDLER_POOL->pop();
    if (mh == NULL) {
        a->exit = true;
        return NULL;
    }

    client::offline* new_o = new client::offline(*o);
    set_svr_head(new_o->mutable_svr_head(), a);
    mh->msg = new_o;

    if (-1 == amtsvr_send(a->ctx, 0, a->scene_server_id, PTYPE_CLIENT | PTYPE_TAG_DONTCOPY, 0, mh, sizeof(msg_handler))) {
        delete new_o;
    }

    a->exit = true;

    return NULL;
}

static void* on_svr_close_agent(server::close_agent* ca, void* ud, size_t* rsp_len)
{
    agent_service_t* a = static_cast<agent_service_t*>(ud);
    if (ca->agent_id() != a->self) {
        return NULL;
    }

    a->shutdown = true;

    char cmd[128] = {0};
    snprintf(cmd, sizeof(cmd), "shutdown %d", a->index);
    amtsvr_send(a->ctx, 0, a->watch_dog, PTYPE_TEXT, 0, cmd, strlen(cmd) + 1);

    return NULL;
}

void set_svr_head(client::server_head* svr_head, struct agent_service_t* a)
{
    if (svr_head != NULL) {
        svr_head->set_svr_id(a->svr_id);
        svr_head->set_user_id(a->user_id);
        svr_head->set_role_id(a->role_id);
        svr_head->set_agent_id(a->self);
        svr_head->set_client_id(a->client);
        svr_head->set_scenesvr_id(a->scene_server_id);
        svr_head->set_client_ip(a->ip);
        svr_head->set_user_name(a->user_name);
    }
}

void set_inner_head(inner::inner_head* in_head, struct agent_service_t* a)
{
    if (in_head != NULL) {
        in_head->set_svr_id(a->svr_id);
        in_head->set_agent_id(a->self);
        in_head->set_client_id(a->client);
        in_head->set_client_ip(a->ip);
    }
}

int agesvr_dispatcher_init(struct agent_service_t* a)
{
    if (a->inner) { // 内部接口处理的PB消息注册，在这里已经是默认了PHP接入的安全性了
        a->inner_pd.reg<inner::addNotice_req>("inner.addNotice_req", on_pb_addNotice_req);
        a->inner_pd.reg<inner::getNoticeList_req>("inner.getNoticeList_req", on_pb_getNoticeList_req);
        a->inner_pd.reg<inner::updateNotice_req>("inner.updateNotice_req", on_pb_updateNotice_req);
        a->inner_pd.reg<inner::delNotice_req>("inner.delNotice_req", on_pb_delNotice_req);
    }

    a->pd.reg_default(on_default_proc_msg);

    a->pd.reg<client::auth_rsp>("client.auth_rsp", on_client_auth_rsp);
    a->pd.reg<client::role_info_rsp>("client.role_info_rsp", on_client_role_info_rsp);
    a->pd.reg<client::update_agent_role>("client.update_agent_role", on_client_update_agent_role);
    a->pd.reg<client::role_pos_msg>("client.role_pos_msg", on_role_pos_msg);
    a->pd.reg<client::offline>("client.offline", on_offline_msg);

    a->pd.reg<server::close_agent>("server.close_agent", on_svr_close_agent);

    // node相关
    a->pd.reg<server::register_node_req>("server.register_node_req", on_agent_register_node_req);
    //

    return 0;
}

void agent_send_client(struct agent_service_t* a, int session, const google::protobuf::Message& msg)
{
    size_t rsp_len = 0;
    void* rsp_buf = encode(msg, rsp_len);
    if (rsp_buf != NULL) {
        amtsvr_send(a->ctx, 0, a->client, PTYPE_CLIENT | PTYPE_TAG_DONTCOPY, session, rsp_buf, rsp_len);
    }
}

void agent_send_watchdog(struct agent_service_t* a, const char* cmd)
{
    amtsvr_send(a->ctx, 0, a->watch_dog, PTYPE_TEXT, 0, (void*)cmd, strlen(cmd) + 1);
}

void agent_send_log(struct agent_service_t* a, google::protobuf::Message* msg)
{
    msg_handler* mh = MSG_HANDLER_POOL->pop();
    if (mh == NULL) {
        return ;
    }

    mh->msg = msg;

    amtsvr_sendname(a->ctx, CENTER_NAME, PTYPE_CLIENT | PTYPE_TAG_DONTCOPY, 0, (void*)mh, sizeof(msg_handler));
}
