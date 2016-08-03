#include "game_interface.h"
#include "player_role.h"
#include "player_role_set.h"
#include "scene_user.h"
#include "scene_inst.h"

void* on_pb_shortcut_req(client::shortcut_req* req, void* ud, size_t* rsp_len)
{
    PB_FUNC_GUARD(req);

    shortcut_rsp rsp;
    psu->get_player_role().get_set().fill_shortcut(rsp);
    game_send_client(ss, req->svr_head().client_id(), rsp);

    return NULL;
}

void* on_pb_shortcut_set_req(client::shortcut_set_req* req, void* ud, size_t* rsp_len)
{
    PB_FUNC_GUARD(req);

    shortcut_set_rsp rsp;
    psu->get_player_role().get_set().set_shortcut(req, rsp);
    game_send_client(ss, req->svr_head().client_id(), rsp);

    return NULL;
}

void* on_pb_handup_req(client::handup_req* req, void* ud, size_t* rsp_len)
{
    PB_FUNC_GUARD(req);

    handup_rsp rsp;
    psu->get_player_role().get_set().fill_handup(rsp);
    game_send_client(ss, req->svr_head().client_id(), rsp);

    return NULL;
}

void* on_pb_handup_set_req(client::handup_set_req* req, void* ud, size_t* rsp_len)
{
    PB_FUNC_GUARD(req);

    handup_set_rsp rsp;
    psu->get_player_role().get_set().set_handup(req, rsp);
    game_send_client(ss, req->svr_head().client_id(), rsp);

    return NULL;
}
