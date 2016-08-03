#include "center_interface.h"

static void center_broadcast_explore_msg(center_service_t* w, client::broadcast_explore_list_rsp* cm)
{
    w->oi_mgr.brodcast(w->ctx, *cm);
}


void* on_pb_broadcast_explore_list_req(client::broadcast_explore_list_req* req, void* ud, size_t* rsp_len)
{
    center_service_t* w = (center_service_t*)ud;

    client::broadcast_explore_list_rsp rsp;
    w->em_mgr.fill_explore_list(rsp);
    center_send_client(w, req->svr_head(), rsp);
    
    return NULL;
}

void* on_pb_explore_update_list(client::explore_update_list* req, void* ud, size_t* rsp_len)
{
    center_service_t* w = (center_service_t*)ud;

    w->em_mgr.update(req);
    client::broadcast_explore_list_rsp rsp;
    rsp.mutable_eu()->CopyFrom(req->eu());
    center_broadcast_explore_msg(w, &rsp);

    return NULL;
}
