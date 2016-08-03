#include "inner.online.pb.h"
#include "offline_role_mgr.h"
#include "player_role.h"
#include "center_interface.h"

void fill_tail(inner::inner_tail* tail);

void* on_center_pb_getPlayerList_req(inner::getPlayerList_req* req, void* ud, size_t* rsp_len)
{
    return NULL;
}

void* on_center_pb_t_online_req(inner::t_online_req* req, void* ud, size_t* rsp_len)
{
    center_service_t* w = (center_service_t*)ud;
    const std::map<uint32, online_role>& ors = w->oi_mgr.get_all_online_info();
    static uint32 t_online_id = 0;
    ++t_online_id;
    inner::t_online_rsp rsp;
    rsp.set_id(t_online_id);
    rsp.set_sid(0);
    rsp.set_allcount(ors.size());
    rsp.set_cashcount(0);
    rsp.set_createdate((uint32)time(NULL));
    fill_tail(rsp.mutable_tail());
    center_send_inner(w, req->head(), rsp);
    
    return NULL;
}

void* on_pb_center_online_player_req(inner::online_player_req* req, void* ud, size_t* rsp_len)
{
    center_service_t* w = (center_service_t*)ud;
    inner::online_player_rsp rsp;
    const std::map<uint32, online_role>& ors = w->oi_mgr.get_all_online_info();
    for (std::map<uint32, online_role>::const_iterator i = ors.begin(); i != ors.end(); ++i) {
        rsp.add_role_id(i->second.role_id);
    }

    fill_tail(rsp.mutable_tail());
    center_send_inner(w, req->head(), rsp);

    return NULL;
}

void* on_pb_center_set_attr_req(inner::set_attr_req* req, void* ud, size_t* rsp_len)
{
    center_service_t* w = (center_service_t*)ud;

    client::test t;
    t.set_attr_id(req->index());
    t.set_attr_num(req->value());
    for (int32 i = 0; i < req->user_size(); ++i) {
        const inner::inner_user& iu = req->user(i);
        
        const online_role* oi = w->oi_mgr.get_online_info(iu.id());
        if (oi == NULL) {
            continue;
        }

        t.mutable_svr_head()->set_role_id(iu.id());
        center_send_scene(w, *oi, t);
    }

    inner::set_attr_rsp rsp;
    fill_tail(rsp.mutable_tail());
    rsp.set_index(req->index());
    rsp.set_value(req->value());
    rsp.mutable_user()->CopyFrom(req->user());
    center_send_inner(w, req->head(), rsp);

    return NULL;
}

void* on_pb_center_set_vip_req(inner::set_vip_req* req, void* ud, size_t* rsp_len)
{
    center_service_t* w = (center_service_t*)ud;

    client::test t;
    t.set_vip_lv(req->lv());
    t.set_month(req->month());
    for (int32 i = 0; i < req->user_size(); ++i) {
        const inner::inner_user& iu = req->user(i);

        const online_role* oi = w->oi_mgr.get_online_info(iu.id());
        if (oi != NULL) {
            t.mutable_svr_head()->set_role_id(iu.id());
            center_send_scene(w, *oi, t);
            continue;
        }

        const offline_role* ou = w->ou_mgr->get(iu.id());
        mysql::sp_load_all_user_smy* smy = w->aus_mgr.mutable_user_smy(iu.id());
        if (ou == NULL && smy != NULL) {
            inner::set_vip_req msg;
            msg.add_user()->set_id(iu.id());
            msg.set_lv(req->lv());
            msg.set_month(req->month());
            center_load_role_req(w, iu.id(), msg);
            continue;
        } else if (ou != NULL && smy != NULL) {
            player_role* pr = ou->get();
            if (pr == NULL) {
                continue;
            }

            pr->set_vip_lv(req->lv(), req->month() > 0);
            smy->set_vip_level(req->lv());
        }
    }

    inner::set_vip_rsp rsp;
    fill_tail(rsp.mutable_tail());
    rsp.set_lv(req->lv());
    rsp.set_month(req->month());
    rsp.mutable_user()->CopyFrom(req->user());
    center_send_inner(w, req->head(), rsp);

    return NULL;
}