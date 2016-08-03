#include "player_role.h"
#include "config_props.h"
#include "center_interface.h"

void fill_tail(inner::inner_tail* tail);
void* on_pb_delivery_mail(client::delivery_mail_info* dmi, void* ud, size_t* rsp_len);

bool getBackpackGoodsList(inner::getBackpackGoodsList_req* req, center_service_t* w, inner::getBackpackGoodsList_rsp& rsp)
{
    if (req->user_size() < 1) {
        return true;
    }

    uint32 role_id = req->user(0).id();
    const player_role* pr = center_get_player_role(w, *req, role_id);
    if (pr == NULL) {
        return false;
    }

    pr->fill_role_goods(rsp.add_data());

    return true;
}

void* on_center_pb_getBackpackGoodsList_req(inner::getBackpackGoodsList_req* req, void* ud, size_t* rsp_len)
{
    center_service_t* w = (center_service_t*)ud;
    inner::getBackpackGoodsList_rsp rsp;
    if (getBackpackGoodsList(req, w, rsp)) {
        fill_tail(rsp.mutable_tail());
        center_send_inner(w, req->head(), rsp);
    }

    return NULL;
}

bool updateBackpackGoods(inner::updateBackpackGoods_req* req, center_service_t* w, inner::updateBackpackGoods_rsp& rsp)
{
    if (req->update_size() < 1) {
        return true;
    }

    uint32 role_id = req->update(0).user().id();
    player_role* pr = center_get_player_role(w, *req, role_id);
    if (pr == NULL) {
        return false;
    }

    pr->update_role_goods(req->mutable_update(0));

    return true;
}

void* on_center_pb_updateBackpackGoods_req(inner::updateBackpackGoods_req* req, void* ud, size_t* rsp_len)
{
    center_service_t* w = (center_service_t*)ud;
    inner::updateBackpackGoods_rsp rsp;
    if (updateBackpackGoods(req, w, rsp)) {
        fill_tail(rsp.mutable_tail());
        center_send_inner(w, req->head(), rsp);
    }

    if (req->update_size() < 1) {
        return NULL;
    }

    string attachment = "";
    char   buf[32] = {0};
    int32  size = req->update(0).goods_size();
    for (int32 i = 0; i < size; ++i) {
        memset(buf, 0, sizeof(buf));
        const inner::inner_goods& gi = req->update(0).goods(i);
        if (gi.has_index() && gi.has_goods_id() && gi.has_goods_num()) {
            snprintf(buf, sizeof(buf), "|%d|%d|%d", gi.index(), gi.goods_id(), gi.goods_num());
            attachment += buf;
        }
    }

    const inner::inner_user& user = req->update(0).user();
    amtsvr_billlog(w->ctx, "0|%s|%u|%s|%s|%s", req->GetTypeName().c_str(), user.id(), user.account_name().c_str(), user.nickname().c_str(), attachment.c_str());

    return NULL;
}

void* on_center_pb_sendGoods_req(inner::sendGoods_req* req, void* ud, size_t* rsp_len)
{
    center_service_t* w = (center_service_t*)ud;

    client::delivery_mail_info dmi;
    int32 size = req->gi_size();
    static const int32 max_size = 5;
    client::mail_info* mi = dmi.mutable_mi();
    client::mail_head* mh = mi->mutable_mh();
    mh->set_mail_id(0);
    mh->set_send_role_id(0);
    mh->set_send_role_name("system");
    mh->set_mail_type(MAIL_TYPE_SYSTEM);
    mh->set_title(req->title());
    mh->set_send_time((uint32)time(NULL));
    mh->set_past_time(get_mail_past_time(MAIL_TYPE_SYSTEM,true));
    mh->set_attachment_num(size < max_size ? size : max_size);
    mh->set_opened(false);
	mh->set_is_take(false);
    client::mail_body* mb = mi->mutable_mb();
    mb->set_content(req->content());

    const config_props_mgr* mgr = GET_CONFIG_MGR(config_props_mgr);
    if (mgr != NULL) {
        for (int32 i = 0; i < size && i < max_size; ++i) {
            if (!req->gi(i).has_goods_type() || !req->gi(i).has_goods_id() || !req->gi(i).has_goods_num()) {
                continue;
            }

            const config_props* cp = mgr->get_config_props(req->gi(i).goods_id());
            if (cp == NULL) {
                continue;
            }

            client::props_info* pi = mi->add_ma();
            pi->set_props_id(req->gi(i).goods_id());
            pi->set_count(req->gi(i).goods_num());
            pi->set_bind(req->gi(i).bind());
            pi->set_pos(0);
            pi->set_grid_index(0);
            pi->set_quality(cp->get_quality());
        }
    }

    if (req->type() == 7) {
        const std::map<uint32, mysql::sp_load_all_user_smy*>& users = w->aus_mgr.get_all();
        for (std::map<uint32, mysql::sp_load_all_user_smy*>::const_iterator i = users.begin(); i != users.end(); ++i) {
            dmi.set_recv_name(i->second->role_name());
            dmi.mutable_mi()->mutable_mh()->set_recv_role_id(i->first);
            on_pb_delivery_mail(&dmi, ud, rsp_len);
        }
    } else {
        size = req->user_size();
        bool send = false;
        for (int32 i = 0; i < size; ++i) {
            if (req->user(i).has_id()) {
                const mysql::sp_load_all_user_smy* smy = w->aus_mgr.get_user_smy(req->user(i).id());
                if (smy == NULL) {
                    continue;
                } 

                dmi.set_recv_name(smy->role_name());
                dmi.mutable_mi()->mutable_mh()->set_recv_role_id(req->user(i).id());
                send = true;
            }

            if (!send && req->user(i).has_nickname()) {
                const mysql::sp_load_all_user_smy* smy = w->aus_mgr.get_user_smy(req->user(i).nickname());
                if (smy == NULL) {
                    continue;
                }

                dmi.set_recv_name(req->user(i).nickname());
                dmi.mutable_mi()->mutable_mh()->set_recv_role_id(smy->role_id());
                send = true;
            }

            if (!send && req->user(i).has_account_name()) {
                continue;
            }

            if (send) {
                on_pb_delivery_mail(&dmi, ud, rsp_len);
                send = false;
            }
        }
    }

    inner::sendGoods_rsp rsp;
    fill_tail(rsp.mutable_tail());
    center_send_inner(w, req->head(), rsp);

    string attachment = "";
    char buf[32] = {0};
    size = req->gi_size();
    for (int32 i = 0; i < size && i < max_size; ++i) {
        memset(buf, 0, sizeof(buf));
        snprintf(buf, sizeof(buf), "|%d|%d", req->gi(i).goods_id(), req->gi(i).goods_num());
        attachment += buf;
    }

    amtsvr_billlog(w->ctx, "0|%s|%s|%s|%d|%s", req->GetTypeName().c_str(), mh->title().c_str(), mb->content().c_str(), mh->attachment_num(), attachment.c_str());

    return NULL;
}

