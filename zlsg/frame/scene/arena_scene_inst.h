#ifndef __ARENA_SCENE_INST_H__
#define __ARENA_SCENE_INST_H__

#include "base_time.h"
#include "raid_scene_inst.h"
#include "scene_robot.h"
#include "scene_inst.h"

#define DEFAUTL_ARENA_SCENE_USER_NUM 3000

class arena_scene_inst : public raid_scene_inst
{
public:
    arena_scene_inst(uint32 id, const game_map* p, const raid_scene* s, struct amtsvr_context* c, const int m = DEFAUTL_ARENA_SCENE_USER_NUM);
    virtual ~arena_scene_inst() {}

public:
    virtual int on_kill_role(scene_role* atk, scene_role* npc);
	virtual int leave_scene(scene_user *su);
	virtual bool is_need_broadcast_npc_name() { return true; }
	virtual bool relive(scene_role *sr) { return false; }
	virtual bool is_scene_npc_all_dead();
    virtual int push(client::robot_attr *ra);
    virtual int on_raid_complete(bool failure = false);
	virtual void set_scene_ready(bool ready, scene_user *psu) {}
	virtual int enter_scene(scene_user *su);

public:
	void load_npc(scene_user *psu);
	void fight_begin(scene_user *psu);

public:
    static void send_arena_reward(scene_user *su, int ret);

private:
    void send_fight_result(client::FIGHT_RET_E ret);

private:
    uint32	_close_time;
	bool	_finish;
	bool	_load;
};

#endif
