#include <vector>
#include "random.h"
#include "client.auth.pb.h"
#include "role_interface.h"
#include "msg_handler_pool.h"

void proc_auths_rsp(role_service_t* a, struct cmq_svr_msg_t* msg, int session, uint32_t source)
{
    if (msg->pb_count == 0) {
        return ;
    }

    mysql::sp_login_auths* last = static_cast<mysql::sp_login_auths*>(msg->pb_ctx[msg->pb_count - 1].pb_msg);
    std::string open_id = last->role_id();
    uint32 svr_id       = atoi(last->sex().c_str());
    uint32 user_id      = atoi(last->fighting_power().c_str());
	delete last;
    mysql::sp_login_auth* sla = a->alm.mut(open_id, svr_id);
    if (sla == NULL || user_id == 0) {
        return ;
    }

    uint32 now = (uint32)time(NULL);
    client::auth_rsp rsp;
    rsp.set_user_id(user_id);
    rsp.set_ret(client::ARE_SUCCESS);
    rsp.set_svrtime(now);

    for (int i = 0; i < msg->pb_count - 1; ++i) {
        mysql::sp_login_auths* la = static_cast<mysql::sp_login_auths*>(msg->pb_ctx[i].pb_msg);
        role_info_rsp* ri = rsp.add_role_info();
        ri->set_role_id       (atoi(la->role_id()       .c_str()));
        ri->set_prof          (atoi(la->profession()    .c_str()));
        ri->set_sex           (atoi(la->sex()           .c_str()));
        ri->set_name          (la->role_name());
        ri->set_level         (atoi(la->level()         .c_str()));
        ri->set_fighting_power(atoi(la->fighting_power().c_str()));
        amtsvr_log("\nopen_id: \"%s\"\n%s\n", open_id.c_str(), msg->pb_ctx[i].pb_msg->DebugString().c_str());
		delete la;
    }

    role_send_agent(a, sla->p_agent_id(), rsp); // 这里要保证先通知的是Agent，这样才能保证game的场景数据会在登录返回包之后

    if (sla->v_role_id() > 0) {
        a->alm.erase(open_id, svr_id);
    }
}

void* on_pb_auth_req(client::auth_req* req, void* ud, size_t* rsp_len)
{
    role_service_t* a = (role_service_t*)ud;
    mysql::sp_login_auth* sla = a->alm.mut(req->openid(), req->svr_id());
    if (sla == NULL || sla->p_agent_id() != req->svr_head().agent_id()) {
        call_sp_login_auth(a, req);

        mysql::sp_login_auth la;
        la.set_p_openid   (req->openid  ()            );
        la.set_p_yel_vip  (req->yel_vip ()            );
        la.set_p_server_id(req->svr_id  ()            );
        la.set_p_agent_id (req->svr_head().agent_id ());
        la.set_p_client_id(req->svr_head().client_id());
        a->alm.put(&la);

        return NULL;
    }

    // TODO: 以下代码不会被执行到 后面需要优化登陆做好缓存不要每次都从数据库加载
    sla->set_p_agent_id(req->svr_head().agent_id());
    sla->set_p_client_id(req->svr_head().client_id());

    uint32 now = (uint32)time(NULL);
    client::auth_rsp rsp;
    rsp.set_user_id(sla->v_user_id());
    rsp.set_ret(client::ARE_SUCCESS);
    rsp.set_svrtime(now);

    if (sla->v_role_id() > 0) {
        role_info_rsp* ri = rsp.add_role_info();
        ri->set_role_id(sla->v_role_id());
        ri->set_sex(sla->v_sex());
        ri->set_prof(sla->v_prof());
        ri->set_name(sla->v_name());
        ri->set_level(sla->v_level());
        ri->set_fighting_power(sla->v_fighting_power());
    }

    role_send_agent(a, req->svr_head().agent_id(), rsp); // 这里要保证先通知的是Agent，这样才能保证game的场景数据会在登录返回包之后

    if (sla->v_role_id() > 0) {
        a->alm.erase(req->openid(), req->svr_id());
    }

    return NULL;
}

