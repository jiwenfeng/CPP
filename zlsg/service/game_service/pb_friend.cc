#include "game_interface.h"
#include "scene_user.h"
#include "scene_inst.h"

//void* on_game_pb_friend_add_req(client::friend_add_req* req, void* ud, size_t* rsp_len)
//{
//    PB_FUNC_GUARD(req);
//
//    psu->get_player_role().get_role_statis_mgr().get_target().add_friend();
//
//    return NULL;
//}

void* on_game_pb_friend_recommend_req(client::friend_recommend_req* req, void* ud, size_t* rsp_len)
{
    PB_FUNC_GUARD(req);

    scene_inst* si = psu->get_scene();
    vector<uint32> ids;
    si->get_scene_users(ids);
    size_t size = ids.size();
    if (size == 0) {
        return NULL;
    }
    
    friend_recommend_rsp rsp;
    for (size_t i = 0; i < size && i < 5; ++i) {
        rsp.add_role_id(ids[i]);
    }
    
    game_send_client(ss, req->svr_head().client_id(), rsp);

    return NULL;
}

