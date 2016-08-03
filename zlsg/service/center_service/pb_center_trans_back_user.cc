#include "center_interface.h"

void* on_pb_center_trans_back_user(server::trans_back_user* req, void* ud, size_t* rsp_len)
{
    center_service_t* w = (center_service_t*)ud;
    int size = req->role_id_size();
    if (size <= 0) {
        return NULL;
    }

    center_send_scene_ex(w, req->game_handle(), *req);

    return NULL;
}
