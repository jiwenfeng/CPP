#include "game_interface.h"
#include "player_role.h"
#include "player_role_label_mgr.h"
#include "scene_user.h"
#include "scene_inst.h"

void* on_pb_label_list_req(client::label_list_req* req, void* ud, size_t* rsp_len)
{
    PB_FUNC_GUARD(req);

    client::label_list_rsp rsp;
    psu->get_player_role().get_role_label_mgr().label_list(req, rsp);
    psu->send_client(rsp);

    return NULL;
}

void* on_pb_operate_label_req(client::operate_label_req* req, void* ud, size_t* rsp_len)
{
    PB_FUNC_GUARD(req);

    client::operate_label_rsp rsp;
    int ret = psu->get_player_role().get_role_label_mgr().operate_label(req, rsp);
    if (ret == 0 && req->type() != client::operate_label_req_OPERATE_TYPE_REMOVE) {
//         psu->mutable_attr()->set_val(ATTR_LABEL, req->label_code());
    } else if (ret == 0 && req->type() == client::operate_label_req_OPERATE_TYPE_REMOVE) {
//         psu->mutable_attr()->set_val(ATTR_LABEL, 0);
    }

    psu->send_client_change_attr();
    psu->send_client(rsp);

    return NULL;
}

void* on_game_pb_get_role_label_info_req(client::get_role_label_info_req* req, void* ud, size_t* rsp_len)
{
    struct game_service_t* ss = (struct game_service_t*)ud;
    scene_user* su = ss->sim->get_user(req->svr_head().role_id());
    if (su == NULL){
        return NULL;
    }

    client::get_role_label_info_rsp rsp;
    su->get_player_role().get_role_label_mgr().use_label_list(rsp);
    game_send_client(ss, req->svr_head().client_id(), rsp);

    return NULL;
}

