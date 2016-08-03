#ifndef __LEVELS_SCENE_INST_H__
#define __LEVELS_SCENE_INST_H__

#include "raid_scene_inst.h"

class scene_robot;

class levels_scene_inst : public raid_scene_inst
{
public:
    levels_scene_inst(uint32 id, const game_map* p, const raid_scene* s, struct amtsvr_context* c, const int m = DEFAUTL_SCENE_INST_USER_NUM);
    ~levels_scene_inst();

public:
    virtual int init();
    virtual int on_kill_role(scene_role* atk, scene_role* def);
    virtual int on_raid_complete(bool failure = false);
    virtual int enter_scene(scene_user* psu);
    virtual void on_role_leave(scene_role* psu);
    virtual bool is_friend(scene_role* sr1, scene_role* sr2);
    virtual bool is_scene_npc_all_dead();
	virtual void set_scene_ready(bool ready, scene_user *psu);
public:
    void load_npc(scene_user *su);

private:
    scene_npc *generate_robot(uint32 id, uint32 lv, uint32 power, const map_postion &p, uint32 team_id);
    void load_my_enemy(scene_user *su);

protected:
    uint32                      _team_id;		      // 队伍ID
    vector<rect>				_atk_pos;			  // 攻击方出生点
    vector<rect>				_def_pos;			  // 防守方出生点

private:
    map<uint32, origin_npc *>	_on;
	bool						_load;
};

#endif

