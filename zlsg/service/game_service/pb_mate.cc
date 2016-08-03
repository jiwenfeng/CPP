#include "game_interface.h"
#include "player_role.h"
#include "player_role_ext_attr.h"
#include "player_role_statis_mgr.h"
#include "scene_user.h"
#include "scene_inst.h"

void* on_pb_mate_create_msg(client::mate_create_msg* msg, void* ud, size_t* rsp_len)
{
    struct game_service_t* ss = (struct game_service_t*)ud;
    scene_user* su = ss->sim->get_user(msg->role_id());
    if (su == NULL){
        return NULL;
    }

    // 扣除道具 // 2778是创建小伙伴关系的道具ID
    int ret = su->sub_props(2778, 1);
    msg->set_ret(ret == 0 ? client::MORE_SUCCESS : client::MORE_MATERIAL_LIMIT);
    game_send_center(ss, *msg);

    amtsvr_billlog(ss->ctx, "%u|%s|%u|%u|%u", su->get_role_id(), msg->GetTypeName().c_str(), ret, msg->ret(), msg->mate_role_id());

    return NULL;
}

void* on_pb_depth_mate_msg(client::depth_mate_msg* msg, void* ud, size_t* rsp_len)
{
    struct game_service_t* ss = (struct game_service_t*)ud;
    scene_user* su = ss->sim->get_user(msg->role_id());
    if (su == NULL){
        return NULL;
    }

    int ret = -1;
    // 扣除道具
    if (msg->need_ingot()){
        ret = su->sub_props(2810, 1);
        if (ret != 0){
            ret = su->sub_money(INGOT, 99);  // 一次99元宝
            su->send_client_change_attr();
        }
    } else {
        ret = su->sub_money(TAEL, 100000); // 一次10万铜钱
        su->send_client_change_attr();
    }

    if (ret == 0){
        msg->set_ret(client::MORE_SUCCESS); 
    } else {
        if (msg->need_ingot()){
            msg->set_ret(client::MORE_NO_INGOT);
        } else {
            msg->set_ret(client::MORE_NO_TAEL);
        }
    }
    
    game_send_center(ss, *msg);

    amtsvr_billlog(ss->ctx, "%u|%s|%u|%u|%u", su->get_role_id(), msg->GetTypeName().c_str(), msg->need_ingot() ? 1 : 0, ret, msg->ret());
    
    return NULL;
}


void* on_pb_depth_mate_end_msg(client::depth_mate_end_msg* msg, void* ud, size_t* rsp_len)
{
    struct game_service_t* ss = (struct game_service_t*)ud;
    scene_user* su = ss->sim->get_user(msg->role_id());
    if (su == NULL){
        return NULL;
    }

    bool advanced = 0;
    // 增加属性
    if (msg->ret() == client::MORE_SUCCESS){
        if (msg->has_advanced() && msg->advanced()){
            advanced = 1;
//             player_role_ext_attr_mgr& pream = su->get_player_role().get_player_role_ext_attr_mgr();
//             pream.add_ext_attr(su->get_role_id(), REASTE_MATE, 165, 24, 15, 15, 5, 9);
        } else {
//             player_role_ext_attr_mgr& pream = su->get_player_role().get_player_role_ext_attr_mgr();
//             pream.add_ext_attr(su->get_role_id(), REASTE_MATE, 110, 16, 10, 10, 3, 6);
        }
        su->send_client_change_attr();
    }

    su->send_client(ss->ctx, *msg);

    amtsvr_billlog(ss->ctx, "%u|%s|%u|%u|%u", su->get_role_id(), msg->GetTypeName().c_str(), advanced, msg->ret(), msg->mate_role_id());

    return NULL;
}

void* on_pb_mate_remove_req(client::mate_remove_req* msg, void* ud, size_t* rsp_len)
{
    struct game_service_t* ss = (struct game_service_t*)ud;
    scene_user* su = ss->sim->get_user(msg->svr_head().role_id());
    if (su == NULL){
        return NULL;
    }

    // 扣除道具 // 2778是创建小伙伴关系的道具ID
    int ret = su->sub_props(2778, 1);
    client::mate_remove_msg rsp;
    rsp.set_ret(ret == 0 ? client::MORE_SUCCESS :  client::MORE_MATERIAL_LIMIT);
    rsp.set_role_id(msg->svr_head().role_id());
    game_send_center(ss, rsp);

    amtsvr_billlog(ss->ctx, "%u|%s|%u|%u", su->get_role_id(), msg->GetTypeName().c_str(), ret, rsp.ret());

    return NULL;
}

void* on_game_pb_mate_award_req(client::mate_award_req* req, void* ud, size_t* rsp_len)
{
    struct game_service_t* ss = (struct game_service_t*)ud;
    scene_user* su = ss->sim->get_user(req->svr_head().role_id());
    if (su == NULL){
        return NULL;
    }

    if (su->get_player_role().get_role_statis_mgr().get_mate_reward(req->type()) && req->type() < 12) {
        game_send_center(ss, *req);
    } else {
        mate_award_rsp rsp;
        rsp.set_ret(2);
        game_send_client(ss, req->svr_head().client_id(), rsp);
    }
    
    return NULL;
}

void* on_game_pb_mate_award_via(client::mate_award_via* via, void* ud, size_t* rsp_len)
{
    struct game_service_t* ss = (struct game_service_t*)ud;
    scene_user* su = ss->sim->get_user(via->role_id());
    if (su == NULL) {
        return NULL;
    }
    
    mate_award_rsp rsp;
    if (via->check()) {
        su->add_props(via->award_id(), 1);
        rsp.set_ret(0);
    } else {
        rsp.set_ret(1);
    }
    
    su->send_client(rsp);
    return NULL;
}


