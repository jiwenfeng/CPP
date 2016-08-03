#ifndef __TOWER_SCENE_INST_H__
#define __TOWER_SCENE_INST_H__

#include <bitset>
#include "raid_scene_inst.h"

#define MAX_BOX_NUM 4

class tower_scene_inst : public raid_scene_inst
{
public:
    tower_scene_inst(uint32 id, const game_map* p, const raid_scene* s, struct amtsvr_context* c, const int m = DEFAUTL_SCENE_INST_USER_NUM);
    virtual ~tower_scene_inst();

public:
    int on_raid_complete(bool failure = false);

    uint32 tower_type();

    int on_time_reach();

    virtual int scene_push_init_info(scene_user* su);

public:
    static const uint32 TOWER_SINGLE = 0;
    static const uint32 TOWER_TEAM   = 1;

protected:
    bool reinit_time;
};

#endif /* __TOWER_SCENE_INST_H__ */
