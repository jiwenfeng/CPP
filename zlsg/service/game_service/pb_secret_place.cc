#include "game_interface.h"
#include "scene_user.h"
#include "scene_inst.h"

void* on_pb_explore_secret_place_req(client::explore_secret_place_req* req, void* ud, size_t* rsp_len)
{
    PB_FUNC_GUARD(req);
    explore_secret_place_rsp rsp;
    client::explore_update_list via;
    game_send_client(ss, req->svr_head().client_id(), rsp);

    if (via.eu_size() > 0) {
        game_send_center(ss, via);
    }

    amtsvr_billlog(ss->ctx, "%u|%s|%u|%u|%u|%d", psu->get_role_id(), req->GetTypeName().c_str(), req->place_id(), req->explore_num(), rsp.place_id(), rsp.psp_size());

    return NULL;
}

void* on_pb_last_explore_secret_place_req(client::last_explore_secret_place_req* req, void* ud, size_t* rsp_len)
{
    PB_FUNC_GUARD(req);

    last_explore_secret_place_rsp rsp;
    game_send_client(ss, req->svr_head().client_id(), rsp);

    return NULL;
}

