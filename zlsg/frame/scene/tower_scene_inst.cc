#include "scene_user.h"
#include "tower_scene_inst.h"

tower_scene_inst::tower_scene_inst(uint32 id,
                                   const game_map* p,
                                   const raid_scene* s,
                                   struct amtsvr_context* c,
                                   const int m/*= DEFAULT_SCENE_INST_USER_NUM*/)
    : raid_scene_inst(id, p, s, c, m)
{
    reinit_time = false;
}

tower_scene_inst::~tower_scene_inst()
{
}

int tower_scene_inst::on_raid_complete(bool failure /*= false*/)
{
    if (raid_scene_inst::on_raid_complete() != 0) {
        return -1;
    }

    vector<scene_user*> sus;
    get_scene_users(sus);
    if (sus.size() == 0 || sus.size() > 1 || sus[0] == NULL) {
        return -2;
    }

    uint32 size = sus.size();
    for (uint32 i = 0; i < size; ++i) {
        //sus[i]->on_tower_complete(tower_type(), rsc->get_battle_ui_id(), false);
    }

    return 0;
}

int tower_scene_inst::on_time_reach()
{
    vector<scene_user*> sus;
    get_scene_users(sus);
    uint32 size = sus.size();
    for (uint32 i = 0; i < size; ++i) {
        //sus[i]->on_tower_complete(tower_type(), rsc->get_battle_ui_id(), true);
        if (sus[i]->trans_back_user_at_once() != 0) {
            destroy_user(sus[i]);
        }
    }

    return 0;
}

uint32 tower_scene_inst::tower_type()
{
    if (_rsc->get_min_team_num() > 0) {
        return 1;
    }

    return 0;
}

int tower_scene_inst::scene_push_init_info(scene_user* su)
{
    if (!reinit_time){
        _start_time = (uint32)time(NULL);
        reinit_time = true;
    }
    
    return 0;
}

