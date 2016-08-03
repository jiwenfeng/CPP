#include "config_raid.h"
#include "game_interface.h"
#include "scene_user.h"
#include "scene_inst.h"
#include "farm_scene_inst.h"
#include "raid_scene_inst.h"

void* on_pb_farm_create_req(client::farm_create_req* req, void* ud, size_t* rsp_len)
{
    struct game_service_t* ss = (struct game_service_t*)ud;
    scene_user* su = ss->sim->get_user(req->svr_head().role_id());
    if (su == NULL){
        return NULL;
    }

    int ret = -1;
    if (req->advanced()){
        ret = su->sub_money(INGOT, 200);
    } else {
        ret = su->sub_money(TAEL, 300000);
    }

    if (ret != 0){
        client::farm_create_rsp rsp;
        rsp.set_ret(client::FOE_MONEY_LIMIT);
        su->send_client(ss->ctx, rsp);
        return NULL;
    }

    su->send_client_change_attr();
    game_send_chores(ss, *req);

    amtsvr_billlog(ss->ctx, "%u|%s|%u", su->get_role_id(), req->GetTypeName().c_str(),req->advanced() ? 2:1);

    return NULL;
}

void* on_pb_farm_enter_req(client::farm_enter_req* req, void* ud, size_t* rsp_len)
{
    struct game_service_t* ss = (struct game_service_t*)ud;
    scene_user* su = ss->sim->get_user(req->svr_head().role_id());
    if (su == NULL){
        return NULL;
    }

    uint32 farm_role_id = req->role_id();
    if (farm_role_id == 0){
        farm_role_id = su->get_role_id();
    }

    /*if (farm_role_id != su->get_role_id()) {
        client::farm_entered_msg msg;
        msg.set_role_id(su->get_role_id());
        msg.set_farmer_id(farm_role_id);
        game_send_center(ss, msg);
        }*/

    scene_inst* si = su->get_scene();
    if (si != NULL && si->get_scene_type() == MAP_TYPE_RAID) {
        raid_scene_inst* rsi = static_cast<raid_scene_inst*>(si);
        if (rsi->get_farmer_id() == farm_role_id){ // 同场景跳转不做处理
            return NULL;
        }
    }

    //su->set_farm_role_id(farm_role_id);
    client::jump_user ju;
    ju.set_role_id(su->get_role_id());
    /*if (su->fill_raid_trans_jump(570, ju) != 0){
        return NULL;
        }*/
    ju.set_same_scene(false);

    ss->sim->logout_scene(su, su->get_scene());
    su->send_center(ss->ctx, ju);

    return NULL;
}

void* on_pb_farm_sow_req(client::farm_sow_req* req, void* ud, size_t* rsp_len)
{
    struct game_service_t* ss = (struct game_service_t*)ud;
    scene_user* su = ss->sim->get_user(req->svr_head().role_id());
    if (su == NULL){
        return NULL;
    }

//     const mysql::tb_farm_set* tfs = CONFMGR->get_config_farm_mgr().get_farm_set(req->serial());
//     if (tfs == NULL || tfs->seed_goods_id() == 0){
//         client::farm_sow_rsp rsp;
//         rsp.set_ret(client::FOE_SYS_ERROR);
//         return NULL;
//     }
// 
//     int ret = su->sub_props(tfs->seed_goods_id(), 1);
//     if (ret != 0){
//         client::farm_sow_rsp rsp;
//         rsp.set_ret(client::FOE_NO_SEED);
//         return NULL;
//     }

    game_send_chores(ss, *req);

//     amtsvr_billlog(ss->ctx, "%d|%s|%u|%u", su->get_role_id(), req->GetTypeName().c_str(), req->serial(), tfs->seed_goods_id());

    return NULL;
}

void* on_pb_update_farm_info(client::update_farm_info* ufi, void* ud, size_t* rsp_len)
{
    struct game_service_t* ss = (struct game_service_t*)ud;
    ss->sim->on_update_farm_info(ufi);
    return NULL;
}

void* on_pb_farm_steal_req(client::farm_steal_req* req, void* ud, size_t* rsp_len)
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

    if (si->get_map().get_sub_type() != RAID_WEAPON){
        return NULL;
    }

    farm_scene_inst* fsi = static_cast<farm_scene_inst*>(si);
    if (fsi->get_farmer_id() == 0 
        || fsi->get_farmer_id() == req->svr_head().role_id()){
        client::farm_steal_rsp rsp;
        rsp.set_ret(client::FOE_CANT_STEAL_SELF);
        su->send_client(ss->ctx, rsp);
        return NULL;
    }

    // 判断可偷取的次数

    client::farm_steal_msg msg;
    msg.mutable_req()->CopyFrom(*req);
    msg.set_farm_role_id(fsi->get_farmer_id());
    game_send_chores(ss, msg);

    return NULL;
}


void* on_pb_farm_stolen_msg(client::farm_stolen_msg* msg, void* ud, size_t* rsp_len)
{
    struct game_service_t* ss = (struct game_service_t*)ud;
    scene_user* su = ss->sim->get_user(msg->thief_role_id());
    if (su == NULL){
        return NULL;
    }

    uint32 goods_id = msg->steal_goods_id();
    uint32 num  = msg->steal_num();
    su->add_props(goods_id, num);
    su->send_center(ss->ctx, *msg);

    // 增加今日偷窃次数

    amtsvr_billlog(ss->ctx, "%u|%s|%u|%u", su->get_role_id(), msg->GetTypeName().c_str(), goods_id, num);

    return NULL;
}

void* on_pb_farm_improve_req(client::farm_improve_req* req, void* ud, size_t* rsp_len)
{
    struct game_service_t* ss = (struct game_service_t*)ud;
    scene_user* su = ss->sim->get_user(req->svr_head().role_id());
    if (su == NULL){
        return NULL;
    }

    uint32 goods_id = 0;
    switch (req->type())
    {
    case client::FITE_ONE_HOUR:
        {
            goods_id = 2989;
        }
        break;
    case client::FITE_TWO_HOUR:
        {
            goods_id = 2990;
        }
        break;
    case client::FITE_FIVE_HOUR:
        {
            goods_id = 2991;
        }
        break;
    default:
        break;
    }

    if (goods_id == 0){
        return NULL;
    }

    if (su->sub_props(goods_id, 1) != 0){
        client::farm_improve_rsp rsp;
        rsp.set_ret(client::FOE_NO_LCHOR);
        su->send_client(ss->ctx, rsp);
        return NULL;
    }

    game_send_chores(ss, *req);

    amtsvr_billlog(ss->ctx, "%u|%s|%u", su->get_role_id(), req->GetTypeName().c_str(), goods_id);

    return NULL;
}

void* on_pb_farm_gain_msg(client::farm_gain_msg* msg, void* ud, size_t* rsp_len)
{
    struct game_service_t* ss = (struct game_service_t*)ud;
    scene_user* su = ss->sim->get_user(msg->farm_role_id());
    if (su == NULL){
        return NULL;
    }

    string logs = "";
    int size = msg->fgus_size();
    char tmp[128] = {0};
    if (size > 0){
        for (int i = 0; i < size; ++i){
            su->add_props(msg->fgus(i).goods_id(), msg->fgus(i).goods_num(), 1, 0, false);
            snprintf(tmp, sizeof(tmp), "|%u|%u", msg->fgus(i).goods_id(), msg->fgus(i).goods_num());
        }
        su->send_client_change_props_msg();
    }

    amtsvr_billlog(ss->ctx, "%u|%s|%s", su->get_role_id(), msg->GetTypeName().c_str(), logs.c_str());
    
    return NULL;
}

void* on_pb_farm_gain_money_rsp(client::farm_gain_money_rsp* rsp, void* ud, size_t* rsp_len)
{
    struct game_service_t* ss = (struct game_service_t*)ud;
    scene_user* su = ss->sim->get_user(rsp->role_id());
    if (su == NULL){
        return NULL;
    }

    if (rsp->ret() == client::FOE_SUCCESS) {
//         su->add_money(TAEL, rsp->money());
        su->send_client_change_attr();
    }
    su->send_client(ss->ctx, *rsp);

    amtsvr_billlog(ss->ctx, "%u|%s|%u", su->get_role_id(), rsp->GetTypeName().c_str(), rsp->money());
   
    return NULL;
}

