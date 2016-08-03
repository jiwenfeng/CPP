#include "center_interface.h"

void* on_center_pb_send_flower(client::send_flower* req, void* ud, size_t* rsp_len)
{
    center_service_t* w = (center_service_t*)ud;
    if (w == NULL) {
        return NULL;
    }

    client::send_flower_rsp rsp;
    const mysql::sp_load_all_user_smy* smy = w->aus_mgr.get_user_smy(req->name());
    if (smy == NULL){
        rsp.set_ret(client::SFRE_NONAME);
        center_send_client(w, req->svr_head(), rsp);
        return NULL;
    }

    uint32 dst_role_id = smy->role_id();
    if (dst_role_id == 0){
        rsp.set_ret(client::SFRE_NONAME);
        center_send_client(w, req->svr_head(), rsp);
        return NULL;
    }

    const online_role* oi = w->oi_mgr.get_online_info(dst_role_id);
    if (oi == NULL){
        rsp.set_ret(client::SFRE_OFFLINE);
        center_send_client(w, req->svr_head(), rsp);
        return NULL;
    }

    req->set_dst_role_id(dst_role_id);
    const online_role* src_oi = w->oi_mgr.get_online_info(req->svr_head().role_id());
    if (src_oi != NULL){
        center_send_scene(w, *src_oi, *req);
    }
   
    return NULL;
}

void center_add_friends_degree(center_service_t* w, uint32 src_role_id, uint32 dst_role_id, uint32 degree_val)
{
    uint32 src_ret = 0;
    uint32 dst_ret = 0;
    w->fm_mgr.add_degree(src_role_id, dst_role_id, degree_val, src_ret, dst_ret);
    
    if (src_ret == 0){
        client::friend_degree_msg msg;
        msg.set_friend_role_id(dst_role_id);
        msg.set_degree_val(degree_val);
        center_send_client_by_role_id(w, src_role_id, msg);
    }
    
    if (dst_ret == 0){
        client::friend_degree_msg msg;
        msg.set_friend_role_id(src_role_id);
        msg.set_degree_val(degree_val);
        center_send_client_by_role_id(w, dst_role_id, msg);
    }
}

static const uint32 DEGREE_VAL[client::FNTE_MAX] = {1, 9, 99, 999}; // 增加魅力值

void* on_center_pb_recive_flower(client::recive_flower* rf, void* ud, size_t* rsp_len)
{
    center_service_t* w = (center_service_t*)ud;
    if (w == NULL) {
        return NULL;
    }

    if (rf->number() < client::FNTE_ONE || rf->number() >= client::FNTE_MAX){
        return NULL;
    }

    uint32 reciver_role_id = rf->reciver_role_id();
    const online_role* oi = w->oi_mgr.get_online_info(reciver_role_id);
    switch (rf->number())
    {
    case client::FNTE_ONE: // 1朵9朵
    case client::FNTE_N:
        {
            if (oi != NULL){
                center_send_client(w, *oi, *rf);
            }
        }
        break;
    case client::FNTE_NN:
    case client::FNTE_NNN:
        {
            w->oi_mgr.brodcast(w->ctx, *rf);
        }
        break;
    default:
        return NULL;
    }

    if (oi != NULL){
        center_send_scene(w, *oi, *rf);
    }

    center_add_friends_degree(w, rf->sender_role_id(), reciver_role_id, DEGREE_VAL[rf->number()]);
    
    return NULL;
}
