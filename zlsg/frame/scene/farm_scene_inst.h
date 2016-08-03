#ifndef __FARM_SCENE_INST_H__
#define __FARM_SCENE_INST_H__

#include "raid_scene_inst.h"

class farm_scene_npc;

class farm_scene_inst : public raid_scene_inst
{
public:
    farm_scene_inst(uint32 id, const game_map* p, const raid_scene* s, struct amtsvr_context* c, const int m = DEFAUTL_SCENE_INST_USER_NUM);

    virtual ~farm_scene_inst();

    void on_update_farm_info(client::update_farm_info* ufi);

    virtual int scene_push_init_info(scene_user* su);

    virtual int update(uint32 tick, bool is_one_sec, bool is_fiv_sec, bool is_ten_sec, bool is_thirty_sec, bool is_one_minute);

    virtual int enter_scene(scene_user* u);

private:
    int get_land_pos(uint32 land_id, map_postion& mp);

    void init_farm_info(client::farm_info& fi);

    void reset_farm_info(client::farm_info& src, client::farm_info& dst);

    void reset_land_info(client::farm_land_info* src, client::farm_land_info* dst, uint32 now, vector<uint64>& tmns, vector<scene_npc*>& sns);

    void update_farm_info();

    void update_land_info(client::farm_land_info* fli, uint32 now, vector<uint64>& tmns, vector<scene_npc*>& sns);

    farm_scene_npc* generate_land_npc(client::farm_land_info* fli, uint32 now);

    void scene_broadcast_farm_info();

private:
    bool is_efi_init;
    client::farm_info efi;
    uint64 land_npcs[client::FPE_MAX_FARM_LAND_NUM];
};

#endif
