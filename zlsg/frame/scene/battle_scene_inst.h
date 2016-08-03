#ifndef __BATTLE_SCENE_INST_H__
#define __BATTLE_SCENE_INST_H__

#include "scene_inst.h"
#include "battle_scene.h"
#include "client.city.pb.h"

#define DEFAULT_BATTLE_SCENE_USER_NUM 3000

enum BATTLE_TYPE {
    BATTLE_NORMAL = 1,
    BATTLE_DOMAIN,
    BATTLE_SIEGE,
};

class battle_scene_inst : public scene_inst
{
public:
    battle_scene_inst(uint32 id, const game_map* p, const battle_scene* s, struct amtsvr_context* c, const int m = DEFAULT_BATTLE_SCENE_USER_NUM );
    virtual ~battle_scene_inst();
	virtual int init();
    virtual int enter_scene(scene_user* u);
    virtual int leave_scene(scene_user* u);
	virtual void on_role_enter(scene_role *su);
    virtual int update(uint32 tick, bool is_one_sec, bool is_fiv_sec, bool is_ten_sec, bool is_thirty_sec, bool is_one_minute);
public:
	client::CITY_OPT_ERR_NO_E call(scene_user *su, uint32 family_id);
	int response_call(scene_user *su, scene_user *chief);
	int is_call_expired(uint32 family_id);
private:
	std::map<uint32, int> _call;
};

#endif
