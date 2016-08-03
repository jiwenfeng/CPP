#include "game_map_mgr.h"
#include "scene_user.h"
#include "farm_scene_npc.h"
#include "farm_scene_inst.h"

farm_scene_inst::farm_scene_inst(uint32 id, const game_map* p, const raid_scene* s, struct amtsvr_context* c, const int m)
    : raid_scene_inst(id, p, s, c, m)
{
    is_efi_init = false;
    memset(land_npcs, 0, sizeof(land_npcs));
}

farm_scene_inst::~farm_scene_inst()
{

}

void farm_scene_inst::on_update_farm_info(client::update_farm_info* ufi)
{
    client::farm_info* fi = ufi->mutable_fi();
    if (!is_efi_init){   // 初始化仙园的地图作物数据
        efi.CopyFrom(*fi);
        is_efi_init = true;
        init_farm_info(efi);
        return;
    }

    reset_farm_info(efi, *fi);
}

void farm_scene_inst::reset_farm_info(client::farm_info& src, client::farm_info& dst)
{
    int src_size = src.fli_size();
    int dst_size = dst.fli_size();
    if (src_size != dst_size){
        return;
    }

    if (src_size > client::FPE_MAX_FARM_LAND_NUM){
        src_size = client::FPE_MAX_FARM_LAND_NUM;
    }

    vector<uint64> tmns;
    vector<scene_npc*> sns;
    uint32 now = (uint32)time(NULL);

    for (int i = 0; i < src_size; ++i){
        reset_land_info(src.mutable_fli(i), dst.mutable_fli(i), now, tmns, sns);
    }

    clear_npc(tmns);
    screen_push_npcs(sns);
    efi.CopyFrom(dst);
}

int farm_scene_inst::update(uint32 tick, bool is_one_sec, bool is_fiv_sec, bool is_ten_sec, bool is_thirty_sec, bool is_one_minute)
{
    if (is_fiv_sec && is_efi_init){
        uint32 user_num = _user_map.obj_num(); // 无任何用户，场景不做更新
        if (user_num > 0){
            update_farm_info();
        }
    }

    return raid_scene_inst::update(tick, is_one_sec, is_fiv_sec, is_ten_sec, is_thirty_sec, is_one_minute);
}

void farm_scene_inst::update_farm_info()
{
    int size = efi.fli_size();
    if (size > client::FPE_MAX_FARM_LAND_NUM){
        size = client::FPE_MAX_FARM_LAND_NUM;
    }

    vector<uint64> tmns;
    vector<scene_npc*> sns;
    uint32 now = (uint32)time(NULL);

    for (int i = 0; i < size; ++i){
        update_land_info(efi.mutable_fli(i), now, tmns, sns);
    }

    clear_npc(tmns);
    screen_push_npcs(sns);

    if (tmns.size() > 0 || sns.size() > 0){
        scene_broadcast_farm_info();
    }
}

void farm_scene_inst::scene_broadcast_farm_info()
{
    client::update_farm_info ufi;
    ufi.set_is_create(false);
    ufi.mutable_fi()->CopyFrom(efi);
    scene_broadcast(ufi);
}

void farm_scene_inst::update_land_info(client::farm_land_info* fli, uint32 now, vector<uint64>& tmns, vector<scene_npc*>& sns)
{
    if (fli->serial() == 0 || fli->status() == client::FCSE_MATURE){ // 没有种的和已经成熟的不需要更新了
        return;
    }

    uint32 land_id = fli->land_id();
    if (land_id >= client::FPE_MAX_FARM_LAND_NUM){
        return;
    }

//     const mysql::tb_farm_set* tfs = CONFMGR->get_config_farm_mgr().get_farm_set(fli->serial());
//     if (tfs == NULL){
//         fli->set_serial(0);
//         return;
//     }
// 
//     int interval = now - fli->sow_time();
//     if (interval < 0){
//         interval = 0;
//     }
// 
//     client::FARM_CROP_STATUS_E status;
//     uint32 countdown;
//     if ((uint32)interval < tfs->growup_times()){ // 判断作物状态
//         status = client::FCSE_SEED;
//         countdown = tfs->growup_times() - interval;
//     } else if ((uint32)interval < (tfs->growup_times() + tfs->mature_times())){
//         status = client::FCSE_GROWUP;
//         countdown = tfs->growup_times() + tfs->mature_times() - interval;
//     } else {
//         status = client::FCSE_MATURE;
//         countdown = 0;
//     }
// 
//     if (status != fli->status()){ // 作物的状态发生改变
//         tmns.push_back(land_npcs[land_id]);
//         fli->set_status(status);
//         fli->set_countdown(countdown);
//         farm_scene_npc* fsn = generate_land_npc(fli, now);
//         if (fsn != NULL){
//             sns.push_back(fsn);
//             land_npcs[land_id] = fsn->get_inst_id();
//         } else {
//             land_npcs[land_id] = 0;
//         }
//     }
}

void farm_scene_inst::reset_land_info(client::farm_land_info* src, client::farm_land_info* dst, uint32 now, vector<uint64>& tmns, vector<scene_npc*>& sns)
{
    uint32 land_id = src->land_id();
    if (land_id >= client::FPE_MAX_FARM_LAND_NUM || land_id != dst->land_id()){
        return;
    }

    if (src->serial() == 0 && dst->serial() == 0) { // 依然是光秃秃的地
        return;
    }

    if (src->serial() == 0 && dst->serial() > 0) { // 原来没有，现在有，新种的
        farm_scene_npc* fsn = generate_land_npc(dst, now);
        if (fsn != NULL){
            sns.push_back(fsn);
            land_npcs[land_id] = fsn->get_inst_id();
        } else {
            land_npcs[land_id] = 0;
        }
        return;
    }

    if (src->serial() > 0 && dst->serial() == 0) {  // 原来有,现在没有, 这块地的作物消失
        if (land_npcs[land_id] > 0){
            tmns.push_back(land_npcs[land_id]);
        }
        return;
    }

    // 原来有,现在也有,要判断状态是否改变
    if (src->status() == dst->status()){
        return;
    }

    // 状态发生了改变,要清理原来的状态采集物
    if (land_npcs[land_id] > 0){
        tmns.push_back(land_npcs[land_id]);
    }

    farm_scene_npc* fsn = generate_land_npc(dst, now);
    if (fsn != NULL){
        sns.push_back(fsn);
        land_npcs[land_id] = fsn->get_inst_id();
    } else {
        land_npcs[land_id] = 0;
    }
}

void farm_scene_inst::init_farm_info(client::farm_info& fi)
{
    map_postion mp;
    vector<scene_npc*> sns;
    uint32 now = (uint32)time(NULL);
    int size = fi.fli_size();
    if (size > client::FPE_MAX_FARM_LAND_NUM){
        size = client::FPE_MAX_FARM_LAND_NUM;
    }

    for (int i = 0; i < size; ++i){
        farm_scene_npc* fsn = generate_land_npc(fi.mutable_fli(i),now);
        if (fsn != NULL){
            sns.push_back(fsn);
            land_npcs[i] = fsn->get_inst_id();
        } else {
            land_npcs[i] = 0;
        }
    }

    screen_push_npcs(sns);
}

farm_scene_npc* farm_scene_inst::generate_land_npc(client::farm_land_info* fli, uint32 now)
{
    if (fli == NULL){
        return NULL;
    }

    map_postion mp;
    uint32 land_id = fli->land_id();
    if (get_land_pos(land_id, mp) != 0){   // 查询应该生成采集物的位置
        return NULL;
    } 

    uint32 serial = fli->serial();
    if (serial == 0){ // 没有种植东西
        return NULL;
    }

    const farm_npc* fn = GAME_MAP_MGR->get_farm_npc_mgr().get(serial);// 找不到对应种子的配置
    if (fn == NULL){
        return NULL;
    }

//     const mysql::tb_farm_set* tfs = fn->get_tfs(); 
//     if (tfs == NULL){
//         return NULL;
//     }

    const origin_npc* on = fn->get_origin_npc(fli->status());
    if (on == NULL){
        return NULL;
    }

    return static_cast<farm_scene_npc*>(generate_npc_inst(*on, now, mp, GNTE_FARM));
}

int farm_scene_inst::get_land_pos(uint32 land_id, map_postion& mp)
{
    static const int LSPS[client::FPE_MAX_FARM_LAND_NUM][2] = {
        {531,309}, {640,358},  {741,414}, 
        {887,477}, {1004,533}, {1096,583}, 
        {352,395}, {466,449},  {566,498}, 
        {717,566}, {824,619},  {918,669}, 
        {183,497}, {290,546},  {394,595}, 
        {542,670}, {652,716},  {756,767}, 
    };

    if (land_id >= client::FPE_MAX_FARM_LAND_NUM){
        return -1;
    }

    mp.reset(LSPS[land_id][0], LSPS[land_id][1], 0) ;
    return 0;
}

int farm_scene_inst::scene_push_init_info(scene_user* su)
{
    client::farmer_role_msg msg;
    msg.set_role_id(_farmer_id);
    su->send_client(_ctx, msg);

    client::update_farm_info ufi;
    ufi.set_is_create(false);
    ufi.mutable_fi()->CopyFrom(efi);
    su->send_client(_ctx, ufi);

    return 0;
}

int farm_scene_inst::enter_scene(scene_user* u)
{
    if (u->get_role_id() != _farmer_id){
        client::farm_entered_msg fem;
        fem.set_farmer_id(_farmer_id);
        fem.set_role_id(u->get_role_id());
        send_center(fem);
    }

    return raid_scene_inst::enter_scene(u);
}
