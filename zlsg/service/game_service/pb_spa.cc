#include "player_role.h"
#include "player_role_statis_mgr.h"
#include "scene_user.h"
#include "scene_inst.h"
#include "game_interface.h"

void* on_pb_enter_spa_req(client::enter_spa_req* req, void* ud, size_t* rsp_len)
{
    struct game_service_t* ss = (struct game_service_t*)ud;
    scene_user* su = ss->sim->get_user(req->svr_head().role_id());
    if (su == NULL){
        return NULL;
    }

    scene_inst* si = su->get_scene();
    if (si == NULL) {// || si->is_spa()){
        return NULL;
    }

    if (su->get_player_role().get_role_statis_mgr().join_spa()) {
        su->add_activity(2 * ACTIVITY_SPA);
    }

    client::enter_spa_rsp rsp;
    bool leave_scene = false;
    client::jump_user ju;
    ju.set_role_id(su->get_role_id());
    rsp.set_ret(client::ESRE_SUCCESS);
    su->send_client(rsp);
    if (leave_scene) {
        ss->sim->logout_scene(su, su->get_scene());
        su->send_center(ss->ctx, ju);
    }
    
    amtsvr_billlog(ss->ctx, "%u|%s|%d", su->get_role_id(), req->GetTypeName().c_str(), (int)(rsp.ret()));

    return NULL;
}

void* on_pb_leave_spa_req(client::leave_spa_req* req, void* ud, size_t* rsp_len)
{
    struct game_service_t* ss = (struct game_service_t*)ud;
    scene_user* su = ss->sim->get_user(req->svr_head().role_id());
    if (su == NULL){
        return NULL;
    }

    scene_inst* si = su->get_scene();
    if (si == NULL) {// || !si->is_spa()){
        return NULL;
    }

    su->trans_back_user();
    amtsvr_billlog(ss->ctx, "%u|%s", su->get_role_id(), req->GetTypeName().c_str());

    return NULL;
}

void* on_pb_spa_action_req(client::spa_action_req* req, void* ud, size_t* rsp_len)
{
    struct game_service_t* ss = (struct game_service_t*)ud;
    scene_user* su = ss->sim->get_user(req->svr_head().role_id());
    if (su == NULL) {
        return NULL;
    }

    scene_inst* si = su->get_scene();
    if (si == NULL) { // || !si->is_spa()){
        return NULL;
    }

//     uint32 now = (uint32)time(NULL);
//     if (!CONFMGR->is_in_spa_time(now)){
//         su->trans_back_user();
//         return NULL;
//     }

    spa_action_msg rsp;
    uint32 action_times = 0;
    if (req->action_type() == client::SATE_TIMES){ // 获取次数
        action_times = su->get_spa_action_times();
        rsp.set_ret(client::SARE_SUCCESS);
    } else {
        if (req->svr_head().role_id() == req->dst_role_id()){
            return NULL;
        }

        scene_user* dst_su =  ss->sim->get_user(req->dst_role_id()); // 场景内与其它玩家互动
        if (dst_su != NULL){
            scene_inst* dst_si = su->get_scene();
            if (dst_si == NULL || dst_si != si){
                dst_su = NULL;
            }
        }

        if (dst_su != NULL){
            bool add_other_exp = false;
            rsp.set_ret(su->spa_action_req(action_times, add_other_exp));
            if (add_other_exp){
                dst_su->spa_action_by_other();
            }
            amtsvr_billlog(ss->ctx, "%u|%s|%u|%u|%u", 
                su->get_role_id(), req->GetTypeName().c_str(), dst_su->get_role_id(), (uint32)req->action_type(), (uint32)rsp.ret());
        } else {
            rsp.set_ret(client::SARE_OUT);
        }
    }

    rsp.set_action_type(req->action_type());
    rsp.set_times(action_times);
    rsp.set_src_role_id(su->get_role_id());
    rsp.set_dst_role_id(req->dst_role_id());
    if (req->action_type() == client::SATE_TIMES || rsp.ret() != client::SARE_SUCCESS){
        su->send_client(rsp);
    } else {
        si->scene_broadcast(*su, rsp, false); // 互动成功才广播
    }
    
    return NULL;
}
