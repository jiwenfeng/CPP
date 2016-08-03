// 副本场景实例类
#ifndef __TEAM_SCENE_INST_H__
#define __TEAM_SCENE_INST_H__

#include "client.raid_operate.pb.h"
#include "raid_scene_inst.h"
#include "raid_scene.h"

class scene_robot;

class team_scene_inst : public raid_scene_inst
{
public:
	team_scene_inst(uint32 id, const game_map* p, const raid_scene* s, struct amtsvr_context* c, const int m = DEFAUTL_SCENE_INST_USER_NUM);
	virtual ~team_scene_inst(){}

public:
	virtual int init();
	virtual int on_kill_role(scene_role* atk, scene_role* def);
	virtual int on_raid_complete(bool failure = false);
	virtual int enter_scene(scene_user* psu);
	virtual int enter_scene(scene_npc * sn);
	virtual void on_role_leave(scene_role* psu);
	virtual bool relive(scene_role *sr) { return false; }
	virtual void on_role_enter(scene_role *u);
    virtual bool is_friend(scene_role* psr0, scene_role* psr1);
    virtual bool is_scene_npc_all_dead();
    virtual bool is_need_broadcast_npc_name() { return true; }
	virtual int push(client::robot_attr *ra);
public:
	uint32 calc_tao_lve(scene_user *su);
	uint32 calc_pills_num();
	uint32 calc_stone_num();

	void get_reward_item(scene_user *psu, client::raid_result_info *rri);

protected:
	int team_broadcast(google::protobuf::Message &msg);

protected:
	uint32                      _team_id;         // 队伍ID
	map<uint32, uint32>			_kill;			  // 杀怪统计
	vector<rect>				_pos;			  // 出生点
	uint32						_i;
};

#endif
