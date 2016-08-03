#ifndef __WORLDBOSS_SCENE_INST_H__
#define __WORLDBOSS_SCENE_INST_H__

#include "boss_scene.h"
#include "scene_inst.h"

#define DEFAUTL_WORLDBOSS_SCENE_USER_NUM 3000 

class boss_scene_inst:public scene_inst
{
public:
    boss_scene_inst(uint32 id, const game_map* p, const scene* s, struct amtsvr_context* c, const int m = DEFAUTL_WORLDBOSS_SCENE_USER_NUM );
    ~boss_scene_inst(){}

public:
    virtual int on_kill_role(scene_role* atk, scene_role* npc);
	virtual int get_relive_timelong() { return CONFMGR->get_basic_value(10146);}
    virtual bool is_friend(scene_role* psr0, scene_role* psr1);
	virtual bool relive(scene_role *sr) { return false; }
	virtual void on_role_disappear(scene_role *sr);

    int* get_lock() { return &_lock; }

private:
	void send_boss_status();
	void announce_boss_killed(scene_role *sr);

public:
	void refresh_boss();

private:
	uint32 _status;
    int _lock;
};

#endif
