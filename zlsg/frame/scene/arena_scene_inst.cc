#include "config_npc.h"
#include "arena_scene_inst.h"
#include "scene_user.h"
#include "scene_npc.h"
#include "game_map_mgr.h"
#include "arena_mgr.h"

arena_scene_inst::arena_scene_inst(uint32 id, const game_map* p, const raid_scene* s, struct amtsvr_context* c, const int m)
: raid_scene_inst(id, p, s, c, m)
{
	_finish = false;
	_load = false;
}

void arena_scene_inst::load_npc(scene_user *psu)
{
	if(_load)
	{
		return ;
	}
	const std::vector<uint32> &v = psu->get_target();
	for(size_t i = 0; i < v.size(); ++i)
	{
		client::load_target_data_req req;
		req.set_role_id(psu->get_role_id());
		req.set_target(v[i]);
		req.set_scene_id(get_scene_id());
		send_center(req);
	}
}

int arena_scene_inst::enter_scene(scene_user *psu)
{
	psu->mutable_status()->set_val(client::RSE_STATUS_ATTKMODE, role_status_attack_mode_attack_mod_all);
	return raid_scene_inst::enter_scene(psu);
}

int arena_scene_inst::on_raid_complete(bool failure /*= false*/)
{
	std::vector<scene_user *> sus;
	get_scene_users(sus);
	for(size_t i = 0; i < sus.size(); ++i)
	{
		send_fight_result(failure ? client::FIGHT_LOSE : client::FIGHT_WIN);
	}
    return 0;
}

int arena_scene_inst::leave_scene(scene_user *su)
{
	if(!_finish) {
		send_fight_result(client::FIGHT_LOSE);
	}
	_finish = true;
	set_close(true);

	return raid_scene_inst::leave_scene(su);
}

void arena_scene_inst::send_fight_result(client::FIGHT_RET_E ret)
{
	scene_user *su = get_first_user();
	if (su == NULL) {
		set_close(true);
		return ;
	}
	const std::vector<uint32> &v = su->get_target();
	for(size_t i = 0; i < v.size(); ++i)
	{
		send_arena_reward(su, (int)ret);
		client::arena_fight_ret req;
		req.set_role_id(su->get_role_id());
		req.set_target(v[i]);
		req.set_ret(ret);
		send_center(req);
	}
	su->clear_target();
}

int arena_scene_inst::on_kill_role(scene_role *atk, scene_role *def)
{
	if(NULL == def || def->get_role_type() == RTE_USER)
	{
		return raid_scene_inst::on_kill_role(atk, def);
	}
	if(def->get_role_type() == RTE_PET)
	{
		scene_role *master = def->get_master();
		if(NULL == master)
		{
			return raid_scene_inst::on_kill_role(atk, def);
		}
		if(master->is_robot())
		{
			scene_robot *robot = static_cast<scene_robot *>(master);
			robot->call_backup();
		}
	}
	return raid_scene_inst::on_kill_role(atk, def);
}

int arena_scene_inst::push(client::robot_attr *ra)
{
	map_postion p(1427, 461, 0);
	scene_robot *sr = generate_robot(ra, p, ra->config_id(), 0, 0);
	if(NULL == sr)
	{
		return -1;
	}
	_load = true;

	std::vector<scene_user *> sus;
	get_scene_users(sus);
	for(size_t i = 0; i < sus.size(); ++i)
	{
		amtsvr_billlog(get_ctx(), "%u|client.arena_fight_begin|%s|%u|%u", sus[i]->get_role_id(), sr->get_role_name(), sus[i]->get_old_challenge_count(), sus[i]->get_arena_used_times());
	}

	return 0;
}

void arena_scene_inst::send_arena_reward(scene_user *su, int ret)
{
	int v = CONFMGR->get_basic_value(10040 + ret);
	if (-1 == v) {
		return;
	}
	su->add_money(POPULARITY, v);
	su->send_client_change_attr();
}

bool arena_scene_inst::is_scene_npc_all_dead()
{
	if(!_load)
	{
		return false;
	}
	return raid_scene_inst::is_scene_npc_all_dead();
}

void arena_scene_inst::fight_begin(scene_user *psu)
{
	load_npc(psu);
	raid_scene_inst::set_scene_ready(true, psu);
}
