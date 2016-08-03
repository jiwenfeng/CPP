#include "inner.pay.pb.h"
#include "mysql.t_recharge.pb.h"
#include "game_interface.h"
#include "player_role.h"
#include "scene_user.h"
#include "config_recharge.h"

void fill_tail(inner::inner_tail* tail);

void* on_game_pb_delivery_ingot_req(inner::delivery_ingot_req* req, void* ud, size_t* rsp_len)
{
    PB_FUNC_GUARD_VIA(req);

    int ret = psu->add_money(INGOT, req->ingot());
    if (ret == 0) {
        psu->send_client_change_attr();
    }

    inner::delivery_ingot_rsp rsp;
    rsp.set_role_id(req->role_id());
    rsp.set_ingot(req->ingot());
    rsp.set_ret(ret);
    fill_tail(rsp.mutable_tail());
    game_send_inner(ss, req->head(), rsp);

    return NULL;
}

void pay_log(scene_user* psu, inner::pay_req* req, uint32 state)
{
    mysql::t_recharge log;
    log.set_id(0);
    log.set_sid("");
    log.set_did("");
    log.set_accountname(psu->get_player_role().account());
    log.set_playername(psu->get_role_name());
    log.set_playerlevel(psu->get_level());
    log.set_transactionid(req->pay_id());
    log.set_currency(req->currency());
    log.set_inpourtype(req->inpour_type());
    log.set_addcoin(req->addcoin());
    log.set_money(req->amt() / 10);
    log.set_gold(req->amt());
    log.set_exchangetype(req->ex_type());
    log.set_depay(req->depay());
    log.set_gdepay(req->g_depay());
    log.set_state(state);
    log.set_createdate((uint32)time(NULL));
    psu->send_log(log);
}

void* on_game_pb_pay_req(inner::pay_req* req, void* ud, size_t* rsp_len)
{
    struct game_service_t* ss = NULL;
    ss = (struct game_service_t*)ud;
    scene_user* psu = ss->sim->find_user(req->user().id());
    if (psu == NULL) {
        // 记录日志充值失败
        inner::pay_rsp rsp;
        rsp.set_amt(req->amt());
        rsp.mutable_user()->CopyFrom(req->user());
        inner::inner_tail* tail = rsp.mutable_tail();
        tail->set_status(0);
        tail->set_info("查找用户失败");
        tail->set_data("");
        game_send_inner(ss, req->head(), rsp);

        return NULL;
    }

	int ret = psu->recharge((int)req->amt());
    if (ret == 0) {
        psu->save_user();
        psu->send_client_change_attr();
    }

    pay_log(psu, req, ret);

    inner::pay_rsp rsp;
    rsp.set_amt(req->amt());
    rsp.mutable_user()->CopyFrom(req->user());
    fill_tail(rsp.mutable_tail());
    game_send_inner(ss, req->head(), rsp);

    return NULL;
}
