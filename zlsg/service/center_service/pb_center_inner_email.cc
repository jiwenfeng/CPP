#include "inner.email.pb.h"
#include "center_interface.h"

void fill_tail(inner::inner_tail* tail);
void* on_pb_delivery_mail(client::delivery_mail_info* dmi, void* ud, size_t* rsp_len);

void* on_center_pb_sendEmail_req(inner::sendEmail_req* req, void* ud, size_t* rsp_len)
{
    center_service_t* w = (center_service_t*)ud;

    client::delivery_mail_info dmi;
    client::mail_info* mi = dmi.mutable_mi();
    client::mail_head* mh = mi->mutable_mh();
    mh->set_mail_id(0);
    mh->set_send_role_id(0);
    mh->set_send_role_name("system");
	mh->set_mail_type(MAIL_TYPE_SYSTEM);
    mh->set_title(req->title());
    mh->set_send_time((uint32)time(NULL));
	mh->set_past_time(get_mail_past_time(MAIL_TYPE_SYSTEM, true));
    mh->set_opened(false);
    client::mail_body* mb = mi->mutable_mb();
    mb->set_content(req->content());

    if (req->type() == 2) {
        const std::map<uint32, mysql::sp_load_all_user_smy*>& users = w->aus_mgr.get_all();
        for (std::map<uint32, mysql::sp_load_all_user_smy*>::const_iterator i = users.begin(); i != users.end(); ++i) {
            dmi.set_recv_name(i->second->role_name());
            dmi.mutable_mi()->mutable_mh()->set_recv_role_id(i->first);
            on_pb_delivery_mail(&dmi, ud, rsp_len);
        }
    } else {
        bool send = false;
        for (int32 i = 0; i < req->user_size(); ++i) {
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
    //char buf[32] = {0};
    //size = req->gi_size();
    //for (int32 i = 0; i < size && i < max_size; ++i) {
    //    memset(buf, 0, sizeof(buf));
    //    snprintf(buf, sizeof(buf), "|%d|%d", req->gi(i).goods_id(), req->gi(i).goods_num());
    //    attachment += buf;
    //}
    
    amtsvr_billlog(w->ctx, "0|%s|%s|%s|%d|%s", req->GetTypeName().c_str(), mh->title().c_str(), mb->content().c_str(), mh->attachment_num(), attachment.c_str());
    
    return NULL;
}
