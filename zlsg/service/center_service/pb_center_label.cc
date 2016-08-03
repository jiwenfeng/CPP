#include "center_interface.h"

void fill_honor_label_list(center_service_t* w, honor_label_list_rsp& rsp)
{
#if 0
    map<uint32, uint32>& label = w->rm_mgr.get_label();
    for (map<uint32, uint32>::iterator i = label.begin(); i != label.end(); ++i) {
        client::honor_label_info* hli = rsp.add_li();
        uint32 label_id = rank_mgr::type_to_label(i->first);
        uint32 role_id = i->second;
        if (label_id == 0) {
            continue ;
        }
        
        hli->set_label_code(label_id);
        hli->set_role_id(role_id);
        // printf("type = %d, label = %d, role_id = %d\n", i->first, label, role_id);
    }
#endif
}

void* on_pb_honor_label_list_req(client::honor_label_list_req* req, void* ud, size_t* rsp_len)
{
#if 0
    center_service_t* w = (center_service_t*)ud;
    honor_label_list_rsp rsp;
    fill_honor_label_list(w, rsp);
    center_send_client(w, req->svr_head(), rsp);
#endif
    return NULL;
}

void* on_pb_center_use_honor_label(client::use_honor_label* req, void* ud, size_t* rsp_len)
{
#if 0
    center_service_t* w = (center_service_t*)ud;
    client::use_honor_label_ret ret;
    ret.set_role_id(req->role_id());
    map<uint32, uint32>& label = w->rm_mgr.get_label();
    map<uint32, uint32>::iterator i = label.find(rank_mgr::label_to_type(req->label_code()));
    if (i != label.end()) {
        ret.set_ret(1);
    } else {
        ret.set_ret(0);
    }

    center_send_scene(w, req->role_id(), ret);
#endif
    return NULL;
}


void* on_center_pb_get_role_label_info_req(client::get_role_label_info_req* req, void* ud, size_t* rsp_len)
{
    center_service_t* w = (center_service_t*)ud;

    int32 size = req->role_id_size();
    // 暂时每次最多查20个
    for (int32 i = 0; i < size && i < 20; ++i) {
        const online_role* oi = w->oi_mgr.get_online_info(req->role_id(i));
        if (oi == NULL) {
            continue;
        }

        client::get_role_label_info_req via;
        via.add_role_id(req->role_id(i));
        via.mutable_svr_head()->CopyFrom(req->svr_head());
        center_send_scene(w, *oi, via);
    }
    
    return NULL;
}

