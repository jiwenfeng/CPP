#include "game_interface.h"
#include "scene_user.h"
#include "scene_inst.h"

void* on_pb_send_flower(client::send_flower* req, void* ud, size_t* rsp_len)
{
    struct game_service_t* ss = (struct game_service_t*)ud;
    scene_user* su = ss->sim->get_user(req->svr_head().role_id());
    if (su == NULL){
        return NULL;
    }

    scene_inst* si = su->get_scene();
    if (si == NULL){
        return NULL;
    }

    client::recive_flower rf;
    client::send_flower_rsp rsp;
    rsp.set_ret(client::SFRE_SUCCESS);
    su->send_client(rsp);
    if (rsp.ret() == client::SFRE_SUCCESS){
        game_send_center(ss, rf);
    }
    amtsvr_billlog(ss->ctx, "%u|%s|%s|%u|%u|%u|%u|%u", 
        su->get_role_id(), req->GetTypeName().c_str(), req->name().c_str(), (uint32)rsp.ret(), 
        (uint32)req->type(), req->grid_index(), req->dst_role_id(), (uint32)rf.number());
    
    return NULL;
}

void* on_pb_recive_flower(client::recive_flower* rf, void* ud, size_t* rsp_len)
{
    struct game_service_t* ss = (struct game_service_t*)ud;
    scene_user* su = ss->sim->get_user(rf->reciver_role_id());
    if (su == NULL){
        return NULL;
    }

    scene_inst* si = su->get_scene();
    if (si == NULL){
        return NULL;
    }

    //su->on_pb_recive_flower(rf);
    amtsvr_billlog(ss->ctx, "%u|%s|%u|%u", su->get_role_id(), rf->GetTypeName().c_str(), rf->sender_role_id(), (uint32)rf->number());

    return NULL;
}
