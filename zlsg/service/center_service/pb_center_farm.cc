#include "center_interface.h"

void* on_center_pb_farm_entered_msg(client::farm_entered_msg* msg, void* ud, size_t* rsp_len)
{
    center_service_t* w = (center_service_t*)ud;
    if (w == NULL) {
        return NULL;
    }

    center_send_client_by_role_id(w, msg->farmer_id(), *msg);

    return NULL;
}

void* on_center_pb_farm_stolen_msg(client::farm_stolen_msg* req, void* ud, size_t* rsp_len)
{
    center_service_t* w = (center_service_t*)ud;
    if (w == NULL) {
        return NULL;
    }

    center_send_client_by_role_id(w, req->farm_role_id(), *req);
        
    return NULL;
}

