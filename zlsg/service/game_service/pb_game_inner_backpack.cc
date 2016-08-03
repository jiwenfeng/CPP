#include "player_role.h"
#include "scene_user.h"
#include "scene_inst.h"
#include "game_interface.h"

void fill_tail(inner::inner_tail* tail);

void* on_game_pb_getBackpackGoodsList_req(inner::getBackpackGoodsList_req* req, void* ud, size_t* rsp_len)
{
    struct game_service_t* ss = NULL;                               
    ss = (struct game_service_t*)ud;                                
    scene_user* psu = ss->sim->get_user(req->user(0).id());  
    if (psu == NULL){                                                
        return NULL;                                                
    }
    
    inner::getBackpackGoodsList_rsp rsp;
    psu->get_player_role().fill_role_goods(rsp.add_data());
    fill_tail(rsp.mutable_tail());
    game_send_inner(ss, req->head(), rsp);
    
    return NULL;
}

void* on_game_pb_updateBackpackGoods_req(inner::updateBackpackGoods_req* req, void* ud, size_t* rsp_len)
{
    struct game_service_t* ss = NULL;                               
    ss = (struct game_service_t*)ud;                                
    scene_user* psu = ss->sim->get_user(req->update(0).user().id());  
    if (psu == NULL){                                                
        return NULL;                                                
    }
    
    inner::getBackpackGoodsList_rsp rsp;
    psu->get_player_role().update_role_goods(req->mutable_update(0));
    psu->send_client_change_props_msg();
    fill_tail(rsp.mutable_tail());
    game_send_inner(ss, req->head(), rsp);

    
    
    return NULL;
}
