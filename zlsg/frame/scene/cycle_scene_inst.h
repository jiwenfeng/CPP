#ifndef __CYCLE_SCENE_INST_H__
#define __CYCLE_SCENE_INST_H__

#include "raid_scene_inst.h"
#include "scene_robot.h"
#include "scene_role.h"
#include "scene_user.h"
#include "scene_npc.h"
#include "scene_inst.h"
#include "client.cycle.pb.h"

#define DEFAUTL_CYCLE_SCENE_USER_NUM 3000

class cycle_scene_inst : public raid_scene_inst
{
public:
    cycle_scene_inst(uint32 id, const game_map* p, const raid_scene* s, struct amtsvr_context* c, const int m = DEFAUTL_CYCLE_SCENE_USER_NUM);
    virtual ~cycle_scene_inst() {}

public:
    virtual int init();
	virtual void on_role_enter(scene_role *sr);
	virtual int enter_scene(scene_npc *sn);
	virtual int enter_scene(scene_user *su);
	virtual bool is_friend(scene_role *sr0, scene_role *sr1);
	virtual bool is_scene_npc_all_dead();

public:
    virtual int push(client::robot_attr *ra);
    virtual int on_raid_complete(bool failure = false);

private:
	void load_team(scene_user *su);
	int load_enemy(scene_user *su);
	void on_raid_success(scene_user *su, uint32 tick);

public:
	static void fill_reward(uint32 level, uint32 reward_type, uint32 fastest, client::raid_result_msg *rrm, scene_user *psu);
	static void fill_reward(uint32 level, uint32 reward_type, client::raid_result_info *rri, scene_user *psu);

private:
	bool _has_helper;
	std::vector<rect> _pos;			  // ³öÉúµã
};

#endif
