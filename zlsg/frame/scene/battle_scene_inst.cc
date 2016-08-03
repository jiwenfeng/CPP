#include <algorithm>
#include "scene_user.h"
#include "family_mgr.h"
#include "battle_scene_inst.h"

battle_scene_inst::battle_scene_inst(uint32 id, const game_map* p, const battle_scene* s, struct amtsvr_context* c, const int m)
	: scene_inst(id, p, s, c, m)/*, bsc(s)*/
{
}

battle_scene_inst::~battle_scene_inst()
{
	_call.clear();
}

int battle_scene_inst::enter_scene(scene_user* u)
{
    return scene_inst::enter_scene(u);
}

int battle_scene_inst::leave_scene(scene_user* u)
{
    return scene_inst::leave_scene(u);
}

void battle_scene_inst::on_role_enter(scene_role *sr)
{
	if(sr->get_role_type() != RTE_USER)
	{
		scene_inst::on_role_enter(sr);
		return;
	}
	scene_user *u = static_cast<scene_user *>(sr);
	if(u->get_family_position_id() == CHIEF)
	{
		client::siege_call_status scs;
		std::map<uint32, int>::iterator it = _call.find(u->get_family_id());
		if(it == _call.end())
		{
			scs.set_time(0);
		}
		else
		{
			int need = CONFMGR->get_basic_value(10052) / 100;
			int left = max(it->second + need - (int)time(NULL), 0);
			scs.set_time(left * 100);
		}
		u->send_client(scs);
	}
	scene_inst::on_role_enter(sr);
}

int battle_scene_inst::init()
{
    return scene_inst::init();
}

int battle_scene_inst::update(uint32 tick, bool is_one_sec, bool is_fiv_sec, bool is_ten_sec, bool is_thirty_sec, bool is_one_minute)
{
    return scene_inst::update(tick, is_one_sec, is_fiv_sec, is_ten_sec, is_thirty_sec, is_one_minute);
}

client::CITY_OPT_ERR_NO_E battle_scene_inst::call(scene_user *su, uint32 family_id)
{
	if(su->get_family_position_id() != CHIEF)
	{
		return client::CHIEF_LIMIT;
	}
	if(su->get_status().is_dead())
	{
		return client::ROLE_DEAD;
	}
	std::map<uint32, int>::iterator it = _call.find(family_id);
	int now = static_cast<int>(time(NULL));
	if(it != _call.end())
	{
		if(now - it->second < (CONFMGR->get_basic_value(10052) / 100))
		{
			return client::CD_TIME;
		}
	}
	std::vector<scene_user *> us;
	std::vector<scene_user *>::iterator i;
	get_scene_users(us);
	client::siege_call_info sci;
	sci.set_family_id(family_id);
	for(i = us.begin(); i != us.end(); ++i)
	{
		if((*i)->get_family_id() == family_id && (*i) != su && !(*i)->get_status().is_dead())
		{
			(*i)->send_client(sci);
		}
	}
	_call[family_id] = now;
	return client::SUCCESS;
}

// 响应召唤
int battle_scene_inst::response_call(scene_user *su, scene_user *chief)
{
	cell_pos cp = to_cell_pos(chief->get_scene_pos());
	cell_pos pos;
	if(!get_cell().get_empty(cp, pos))
	{
		return -1;
	}
    trans_pos(su, to_map_postion(pos));
	return 0;
}

// 召唤是否过期
int battle_scene_inst::is_call_expired(uint32 family_id)
{
	std::map<uint32, int>::iterator it = _call.find(family_id);
	if(it == _call.end())
	{
		return 1;
	}
	int now = static_cast<int>(time(NULL));
	if(now - it->second > (CONFMGR->get_basic_value(10053) / 100))
	{
		_call.erase(it);
		return 1;
	}
	return 0;
}
