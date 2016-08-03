#include "client.auth.pb.h"
#include "client.create_role.pb.h"
#include "role_interface.h"

void* on_pb_auth_req(client::auth_req* req, void* ud, size_t* rsp_len);
void* on_pb_create_role_req(client::create_role_req* req, void* ud, size_t* rsp_len);

void* on_pb_sp_login_auth(mysql::sp_login_auth* sla, void* ud, size_t* rsp_len);
void* on_pb_create_role_check_msg(client::create_role_check_msg* msg, void* ud, size_t* rsp_len);

int role_dispatcher_init(struct role_service_t* a)
{
    // 注册client过来的请求数据
    a->pd.reg<client::auth_req>("client.auth_req", on_pb_auth_req);
    a->pd.reg<client::create_role_req>("client.create_role_req", on_pb_create_role_req);

    a->pd.reg<mysql::sp_login_auth>("mysql.sp_login_auth", on_pb_sp_login_auth);
    a->pd.reg<client::create_role_check_msg>("client.create_role_check_msg", on_pb_create_role_check_msg);

    return 0;
}

void* on_pb_sp_login_auth(mysql::sp_login_auth* sla, void* ud, size_t* rsp_len)
{
    role_service_t* a = (role_service_t*)ud;
    
    a->alm.put(sla);

    client::auth_req req;
    req.set_openid(sla->p_openid());
    req.set_openkey("");
    req.set_yel_vip(sla->p_yel_vip());
    req.set_sign("");
    req.set_svr_id(sla->p_server_id());
    client::server_head* svr_head = req.mutable_svr_head();
    svr_head->set_agent_id(sla->p_agent_id());
    svr_head->set_client_id(sla->p_client_id());

    on_pb_auth_req(&req, ud, rsp_len);

    return NULL;
}

int role_send_agent(role_service_t* a, uint32 agent_id, google::protobuf::Message& msg)
{
    int len = 0;
    void* buf = svr_encode(msg, len);
    amtsvr_send(a->ctx, 0, agent_id, PTYPE_CLIENT|PTYPE_TAG_DONTCOPY, 0, buf, (size_t)len);
    return 0;
}

int role_send_center(role_service_t* a, google::protobuf::Message& msg)
{
    google::protobuf::Message* new_msg = create_msg(msg.GetTypeName());
    if (new_msg == NULL){
        return -1;
    }
    new_msg->CopyFrom(msg);

    msg_handler* mh = MSG_HANDLER_POOL->pop();
    if (mh == NULL){
        delete new_msg;
        return -2;
    }
    mh->msg = new_msg;

    return amtsvr_sendname(a->ctx, CENTER_NAME, PTYPE_CLIENT|PTYPE_TAG_DONTCOPY, 0, mh, sizeof(msg_handler));
}

int call_sp_login_auth(role_service_t* a, client::auth_req* req)
{
    char sql[1024] = {0};
    snprintf(sql, sizeof(sql), "calls mysql.sp_login_auths call sp_login_auths('%s', %u, %u, %u, %u, %u);", 
        req->openid().c_str(), req->yel_vip(), req->svr_id(), req->svr_head().agent_id(), req->svr_head().client_id(), req->platform_id());
    amtsvr_sendname(a->ctx, SQLMGR_NAME, PTYPE_CLIENT|PTYPE_TAG_ALLOCSESSION, 0, sql, strlen(sql)+1);
    return 0;
}

bool role_service_t::generate_role_id(uint32 svr_id, uint32& role_id)
{
    std::map<uint32, server_role_t>::iterator it = _svr_roles.find(svr_id);
    if (it == _svr_roles.end()) {
        _svr_roles[svr_id].set_svrid(svr_id);
        role_id = _svr_roles[svr_id].get();
        return true;
    }

    role_id = it->second.get();
    if (role_id == 0) {
        return false;
    }

    return true;
}

void role_service_t::push(uint32 svr_role_id)
{
    uint32 svr_id = GET_SVR_ID(svr_role_id);
    uint32 max_role_id = GET_ROLE_ID(svr_role_id);
    std::map<uint32, server_role_t>::iterator it = _svr_roles.find(svr_id);
    if (it == _svr_roles.end()) {
        _svr_roles[svr_id].set_svrid(svr_id);
        _svr_roles[svr_id].set_max_roleid(max_role_id);
        return;
    }

    it->second.set_max_roleid(max_role_id);
}

void role_service_t::push_svr(uint32 svr_id)
{
    _svr_roles[svr_id].set_svrid(svr_id);
}
