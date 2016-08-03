#include "inner.pay.pb.h"
#include "mysql.t_recharge.pb.h"
#include "player_role.h"
#include "offline_role_mgr.h"
#include "center_interface.h"

void fill_tail(inner::inner_tail* tail);

int pay_get_player_role(center_service_t* w, google::protobuf::Message& msg, uint32 role_id, player_role** pr)
{
    const online_role* oi = w->oi_mgr.get_online_info(role_id);
    if (oi != NULL) {
        center_send_scene(w, *oi, msg);
        return 0;
    }

    const offline_role* ou = w->ou_mgr->get(role_id);
    if (ou == NULL && w->aus_mgr.get_user_smy(role_id) != NULL) {
        center_load_role_req(w, role_id, msg);
        return 0;
    }

    if (ou == NULL) {
        return -1;
    }

    *pr = ou->get();
    if (*pr == NULL) {
        return -1;
    }

    return 1;
}

void* on_center_pb_delivery_ingot_req(inner::delivery_ingot_req* req, void* ud, size_t* rsp_len)
{
    return NULL;

//     center_service_t* w = (center_service_t*)ud;
//     player_role* pr = NULL;
//     int ret = pay_get_player_role(w, *req, req->role_id(), &pr);
//     if (ret == 0) { // 转发处理
//         return NULL;
//     } else if (ret < 0) { // 错误
//         inner::delivery_ingot_rsp rsp;
//         rsp.set_role_id(req->role_id());
//         rsp.set_ingot(req->ingot());
//         rsp.set_ret(ret);
//         inner::inner_tail* tail = rsp.mutable_tail();
//         tail->set_status(0);
//         tail->set_info("查找用户失败");
//         tail->set_data("");
//         center_send_inner(w, req->head(), rsp);
//         return NULL;
//     }
// 
//     ret = pr->add_ingot(req->ingot());
//     pr->write_all(w->ctx);
// 
//     inner::delivery_ingot_rsp rsp;
//     rsp.set_role_id(req->role_id());
//     rsp.set_ingot(req->ingot());
//     rsp.set_ret(ret);
//     fill_tail(rsp.mutable_tail());
//     center_send_inner(w, req->head(), rsp);

//    return NULL;
}

void* on_pb_center_t_recharge(mysql::t_recharge* req, void* ud, size_t* rsp_len);

void center_pay_log(void* ud, size_t* rsp_len, player_role* psu, inner::pay_req* req, uint32 state)
{
    mysql::t_recharge log;
    log.set_id(0);
    log.set_sid("");
    log.set_did("");
    log.set_accountname(psu->account());
    log.set_playername(psu->get_db_player_role()->role_name());
    log.set_playerlevel(psu->get_db_player_role()->level());
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
    on_pb_center_t_recharge(&log, ud, rsp_len);
}

void* on_center_pb_pay_req(inner::pay_req* req, void* ud, size_t* rsp_len)
{
    center_service_t* w = (center_service_t*)ud;

    uint32 role_id = req->user().id();
    player_role* pr = NULL;
    int ret = pay_get_player_role(w, *req, role_id, &pr);
    if (ret == 0) { // 转发处理
        return NULL;
    } else if (ret < 0 || pr == NULL) { // 错误
        inner::pay_rsp rsp;
        rsp.set_amt(req->amt());
        rsp.mutable_user()->CopyFrom(req->user());
        inner::inner_tail* tail = rsp.mutable_tail();
        tail->set_status(0);
        tail->set_info("查找用户失败");
        tail->set_data("");
        center_send_inner(w, req->head(), rsp);

        return NULL;
    }

    pr->add_ingot(req->amt());
    pr->write(w->ctx);

    center_pay_log(ud, rsp_len, pr, req, 0);

    inner::pay_rsp rsp;
    rsp.set_amt(req->amt());
    rsp.mutable_user()->CopyFrom(req->user());
    fill_tail(rsp.mutable_tail());
    center_send_inner(w, req->head(), rsp);

    return NULL;
}