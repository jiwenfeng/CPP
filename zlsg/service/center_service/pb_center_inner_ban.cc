#include "player_role.h"
#include "center_interface.h"

void fill_tail(inner::inner_tail* tail);

void* on_center_pb_getPlayerRestictList_req(inner::getPlayerRestictList_req* req, void* ud, size_t* rsp_len)
{
    center_service_t* w = (center_service_t*)ud;
    inner::getPlayerRestictList_rsp rsp;
    if (req->type() == 0 || (uint32)req->type() == player_role::ROLE_BAN_CHAT) {
        std::map<uint32, uint32>& chat = w->bu_mgr.get_chat();
        std::map<uint32, uint32>::iterator i = chat.begin();
        for (; i != chat.end(); ++i) {
            const mysql::sp_load_all_user_smy* smy = w->aus_mgr.get_user_smy(i->first);
            if (smy == NULL) {
                continue;
            }
        
            inner::ban_info* bi = rsp.add_bi(); 
            bi->mutable_user()->set_id(i->first);
            bi->mutable_user()->set_nickname(smy->role_name());
            bi->set_type(player_role::ROLE_BAN_CHAT);
            bi->set_time(i->second);
        }
    }

    if (req->type() == 0 || (uint32)req->type() == player_role::ROLE_BAN_LOGIN) {
        std::map<uint32, uint32>& login = w->bu_mgr.get_login();
        std::map<uint32, uint32>::iterator i = login.begin();
        for (; i != login.end(); ++i) {
            const mysql::sp_load_all_user_smy* smy = w->aus_mgr.get_user_smy(i->first);
            if (smy == NULL) {
                continue;
            }
        
            inner::ban_info* bi = rsp.add_bi(); 
            bi->mutable_user()->set_id(i->first);
            bi->mutable_user()->set_nickname(smy->role_name());
            bi->set_type(player_role::ROLE_BAN_LOGIN);
            bi->set_time(i->second);
        }
    }

    fill_tail(rsp.mutable_tail());
    center_send_inner(w, req->head(), rsp);
    
    return NULL;
}

bool set_ban(inner::addPlayerRestict_req* req, const inner::ban_info& bi, center_service_t* w)
{
    uint32 role_id = bi.user().id();
    inner::addPlayerRestict_req via;
    via.mutable_head()->CopyFrom(req->head());
    inner::ban_info* add_bi = via.add_bi();
    add_bi->CopyFrom(bi);
    player_role* pr = center_get_player_role(w, via, role_id);
    if (pr == NULL) {
        return false;
    }

    pr->set_ban(bi.type(), bi.time());
    pr->write_all(w->ctx);

    return true;
}

void* on_center_pb_addPlayerRestict_req(inner::addPlayerRestict_req* req, void* ud, size_t* rsp_len)
{
    center_service_t* w = (center_service_t*)ud;
    int32 size = req->bi_size();
    uint32 role_id = 0;
    for (int32 i = 0; i < size; ++i) {
        if (req->bi(i).user().has_id()) {
            role_id = req->bi(i).user().id();
        } else if (req->bi(i).user().has_nickname()) {
            const mysql::sp_load_all_user_smy* smy = w->aus_mgr.get_user_smy(req->bi(i).user().nickname());
            if (smy == NULL) {
                continue;
            }
            
            req->mutable_bi(i)->mutable_user()->set_id(smy->role_id());
            role_id = smy->role_id();
        } else {
            continue;
        }

        set_ban(req, req->bi(i), w);
        if (req->bi(i).type() == 0 || req->bi(i).type() == player_role::ROLE_BAN_CHAT) {
            w->bu_mgr.add_chat(role_id, req->bi(i).time());
        } 

        if (req->bi(i).type() == 0 || req->bi(i).type() == player_role::ROLE_BAN_LOGIN) {
            w->bu_mgr.add_login(role_id, req->bi(i).time());
        }
    }
    
    inner::addPlayerRestict_rsp rsp;
    fill_tail(rsp.mutable_tail());
    center_send_inner(w, req->head(), rsp);
    
    return NULL;
}

bool del_ban(inner::delPlayerRestict_req* req, const inner::ban_info& bi, center_service_t* w)
{

    uint32 role_id  = bi.user().id();
    inner::delPlayerRestict_req via;
    via.mutable_head()->CopyFrom(req->head());
    inner::ban_info* add_bi = via.add_bi();
    add_bi->CopyFrom(bi);
    player_role* pr = center_get_player_role(w, via, role_id);
    if (pr == NULL) {
        return false;
    }

    pr->set_ban(0, 0);
    pr->write_all(w->ctx);

    return true;
}

void* on_center_pb_delPlayerRestict_req(inner::delPlayerRestict_req* req, void* ud, size_t* rsp_len)
{
    center_service_t* w = (center_service_t*)ud;
    int32 size = req->bi_size();
    uint32 role_id = 0;
    for (int32 i = 0; i < size; ++i) {
        if (req->bi(i).user().has_id()) {
            role_id = req->bi(i).user().id();
        } else if (req->bi(i).user().has_nickname()) {
            const mysql::sp_load_all_user_smy* smy = w->aus_mgr.get_user_smy(req->bi(i).user().nickname());
            if (smy == NULL) {
                continue;
            }
            
            req->mutable_bi(i)->mutable_user()->set_id(smy->role_id());
            role_id = smy->role_id();
        } else {
            continue;
        }

        del_ban(req, req->bi(i), w);
        if (req->bi(i).type() == 0 || req->bi(i).type() == player_role::ROLE_BAN_CHAT) {
            w->bu_mgr.del_chat(role_id);
        }

        if (req->bi(i).type() == 0 || req->bi(i).type() == player_role::ROLE_BAN_LOGIN) {
            w->bu_mgr.del_login(role_id);
        }
    }
    
    inner::delPlayerRestict_rsp rsp;
    fill_tail(rsp.mutable_tail());
    center_send_inner(w, req->head(), rsp);

    return NULL;
}
