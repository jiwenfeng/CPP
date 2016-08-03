#include "client.role_login.pb.h"
#include "client.create_role.pb.h"
#include "client.trans_user.pb.h"
#include "role_interface.h"
#include "player_role.h"
#include "msg_handler_pool.h"

//extern map<string, char> reward_openids;

static void create_role_rsp_by_had(role_service_t* a, client::create_role_req* req, mysql::sp_login_auth& sla)
{
    client::role_info_rsp rsp;

    rsp.set_role_id(sla.v_role_id());
    rsp.set_sex(sla.v_sex());
    rsp.set_prof(sla.v_prof());
    rsp.set_name(sla.v_name());
    rsp.set_level(sla.v_level());
    rsp.set_fighting_power(sla.v_fighting_power());
    role_send_agent(a, req->svr_head().agent_id(), rsp);

    /*msg_handler* mh = MSG_HANDLER_POOL->pop();
    if (mh == NULL){
        return;
    }
    mh->msg = new mysql::sp_login_auth(sla);

    amtsvr_sendname(a->ctx, CENTER_NAME, PTYPE_CLIENT, 0, (void*)mh, sizeof(msg_handler));*/
}

static void create_role_rsp_by_new(role_service_t* a, client::create_role_req* req, mysql::sp_login_auth& sla)
{
    client::create_role_check_msg  crcm;
    crcm.mutable_crr()->CopyFrom(*req);
    role_send_center(a, crcm);
    //printf("create_role_rsp_by_new role name = %s---------------------------------------\n", req->name().c_str());
}

void* on_pb_create_role_check_msg(client::create_role_check_msg* msg, void* ud, size_t* rsp_len)
{
    role_service_t* a = (role_service_t*)ud;
    if (!msg->pass()) {
        client::role_info_rsp rsp;
        rsp.set_role_id(0);
        rsp.set_sex(0);
        rsp.set_prof(0);
        rsp.set_name("");
        rsp.set_level(0);
        rsp.set_fighting_power(0);
        rsp.set_ret(client::CREE_NICK_NAME_ERR);
        role_send_agent(a, msg->crr().svr_head().agent_id(), rsp);
        //printf("on_pb_create_role_check_msg failed ,.......................................................................\n");
        return NULL;
    }

    client::create_role_req* req = msg->mutable_crr();

    mysql::sp_login_auth* sla = a->alm.mut(req->svr_head().user_name(), req->svr_head().svr_id());
    if (sla == NULL) {
        return NULL;
    }

    //const mysql::tb_role_init* ri = CONFMGR->get_role_init(req->prof(), req->sex());
    //if (ri == NULL) {
    //    return NULL;
    //}
    const config_birth* cb = CONFMGR->get_birth(req->prof());
    if (cb == NULL) {
        return NULL;
    }

    uint32 role_id = 0;
    if (!a->generate_role_id(req->svr_head().svr_id(), role_id) || role_id == 0) {
        return NULL;
    }

    player_role* pr = new player_role;
    uint32 test_reward = 0;
    /*map<string, char>::iterator it = reward_openids.find(req->svr_head().user_name());
    if (it != reward_openids.end()){
        test_reward = 1;
        reward_openids.erase(it);
    }*/

    if (pr->create_new_player_role(req, cb, role_id, test_reward) != 0) {
        delete pr;
        return NULL;
    }

    pr->account(req->svr_head().user_name());

    pr->write(a->ctx);

    sla->set_v_role_id(pr->get_role_id());
    sla->set_v_sex(req->sex());
    sla->set_v_prof(req->prof());
    sla->set_v_name(req->name());

    client::role_info_rsp rsp;
    rsp.set_role_id(pr->get_role_id());
    rsp.set_sex(pr->get_db_player_role()->sex());
    rsp.set_prof(pr->get_db_player_role()->profession());
    rsp.set_name(pr->get_db_player_role()->role_name());
    rsp.set_level(pr->get_db_player_role()->level());
    rsp.set_fighting_power(pr->get_db_player_role()->fighting_power());
    role_send_agent(a, req->svr_head().agent_id(), rsp);

    msg_handler* mh = MSG_HANDLER_POOL->pop();
    if (mh == NULL) {
        delete pr;
        return NULL;
    }

    // 记录KEY
    client::update_auto_login_key* via = new client::update_auto_login_key;
    via->set_role_id(pr->get_role_id());
    via->set_key("test");
    mh->msg = via;
    amtsvr_sendname(a->ctx, CENTER_NAME, PTYPE_CLIENT, 0, (void*)mh, sizeof(msg_handler));

    // 返回客户端KEY
    client::role_login_rsp rl_rsp;
    rl_rsp.set_ret(client::RLRE_SUCCESS);
    rl_rsp.set_key("test");
    size_t rl_rsp_len = 0;
    void* rl_rsp_buf = encode(rl_rsp, rl_rsp_len);
    amtsvr_send(a->ctx, 0, req->svr_head().client_id(), PTYPE_CLIENT | PTYPE_TAG_DONTCOPY, 0, rl_rsp_buf, rl_rsp_len);

    // 登陆场景
    uintptr_t ptr = (uintptr_t)pr;
    client::trans_user* tr = new client::trans_user;
    tr->set_ptr((uint64)ptr);
    tr->mutable_svr_head()->CopyFrom(req->svr_head());
    mh->msg = tr;
    amtsvr_sendname(a->ctx, CENTER_NAME, PTYPE_CLIENT, 0, (void*)mh, sizeof(msg_handler));

    a->alm.erase(req->svr_head().user_name(), req->svr_head().svr_id());

    //static int create_role_count = 0;
    //printf("create role count [%d] role_id=%d.....................................\n", ++create_role_count, pr->get_role_id());

    return NULL;
}

void* on_pb_create_role_req(client::create_role_req* req, void* ud, size_t* rsp_len)
{
    role_service_t* a = (role_service_t*)ud;
    if (req->name().length() < MIN_NAME_LEN || req->name().length() > MAX_NAME_LEN) {
        return NULL;
    }

    mysql::sp_login_auth* sla = a->alm.mut(req->svr_head().user_name(), req->svr_head().svr_id());
    if (sla == NULL) {
        return NULL;
    }

    client::role_info_rsp rsp;
    if (sla->v_role_id() > 0) {
        create_role_rsp_by_had(a, req, *sla);
        a->alm.erase(req->svr_head().user_name(), req->svr_head().svr_id());
    } else {
        create_role_rsp_by_new(a, req, *sla);
    }

    return NULL;
}

