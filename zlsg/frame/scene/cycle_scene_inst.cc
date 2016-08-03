#include "cycle_scene_inst.h"
#include "config_raid.h"
#include "config_cycle.h"
#include "config_props.h"
#include "config_npc.h"
#include "player_role.h"
#include "player_role_cycle.h"
#include "client.raid_operate.pb.h"
#include "raid_over.h"

cycle_scene_inst::cycle_scene_inst(uint32 id, const game_map* p, const raid_scene* s, struct amtsvr_context* c, const int m) : raid_scene_inst(id, p, s, c, m)
{
	_has_helper = false;
}

int cycle_scene_inst::init()
{
	const config_raid* cr = get_map().get_config_raid();
	if(cr == NULL)
	{
		return -1;
	}
	const std::vector<exp_region>& tmp = cr->get_exp_region();
	if(tmp.empty())
	{
		return -1;
	}
	_pos = tmp[0]._r;
	return raid_scene_inst::init();
}

void cycle_scene_inst::load_team(scene_user *su)
{
	const std::vector<uint32> &v = su->get_target();
	for(size_t i = 0; i < v.size(); ++i)
	{
		client::load_target_data_req req;
		req.set_role_id(su->get_role_id());
		req.set_target(v[i]);
		req.set_scene_id(get_scene_id());
		send_center(req);
	}
	su->clear_target();
}

int cycle_scene_inst::load_enemy(scene_user *su)
{
	player_role_cycle &prc = su->get_player_role().get_role_cycle();
	const config_cycle *cc = CONFMGR->get_config_cycle(prc.get_cur_level());
	if(NULL == cc)
	{
		amtsvr_log("can't find config cycle info %u", prc.get_cur_level());
		return -1;
	}

    const origin_npc* o = GAME_MAP_MGR->get_npc(cc->get_monster_id());
	if(o == NULL)
	{
		amtsvr_log("generate npc failed:%u\n", cc->get_monster_id());
		return -2;
	}
	map_postion p(_pos[1].top_left.x, _pos[1].top_left.y, 0);
	scene_npc *npc = generate_npc_inst(*o, (uint32)time(NULL), p);
	if(NULL == npc)
	{
		amtsvr_log("flag enter scene failed\n");
		return -3;
	}
	return 0;
}

void cycle_scene_inst::on_role_enter(scene_role *sr)
{
	if(sr->get_role_type() != RTE_USER)
	{
		return raid_scene_inst::on_role_enter(sr);
	}
	scene_user *su = static_cast<scene_user *>(sr);
	load_team(su);
	load_enemy(su);
	raid_scene_inst::on_role_enter(sr);
}

int cycle_scene_inst::enter_scene(scene_user *su)
{
	su->set_camp(1);
	return raid_scene_inst::enter_scene(su);
}

int cycle_scene_inst::enter_scene(scene_npc *sn)
{
	if(sn->get_role_type() == RTE_PET)
	{
		return raid_scene_inst::enter_scene(sn);
	}
	if(sn->is_robot())
	{
		sn->set_camp(1);
	}
	else
	{
		sn->set_camp(0);
	}
	return raid_scene_inst::enter_scene(sn);
}

int cycle_scene_inst::push(client::robot_attr *ra)
{
	map_postion p(_pos[2].top_left.x, _pos[2].top_left.y, 0);
	scene_robot *sr = generate_robot(ra, p, ra->config_id(), 0, 0);
	if(NULL == sr)
	{
		return -1;
	}
	_has_helper = true;
	return 0;
}

void cycle_scene_inst::fill_reward(uint32 level, uint32 reward_type, client::raid_result_info *rri, scene_user *psu)
{
	const config_cycle *cc = CONFMGR->get_config_cycle(level);
	if(NULL == cc)
	{
		return ;
	}
	psu->fill_role_data(rri->mutable_rd());
	vector<client::props_info> props;
	switch(reward_type)
	{
		case 0:
			psu->add_drop_props(cc->get_first_reward(), rri);
			break;
		case 1:
			psu->add_drop_props(cc->get_again_reward(), rri);
			break;
		default:
			rri = NULL;
			break;
	}
}

void cycle_scene_inst::fill_reward(uint32 level, uint32 reward_type, uint32 fastest, client::raid_result_msg *rrm, scene_user *psu)
{	
	const config_cycle *cc = CONFMGR->get_config_cycle(level);
	if(NULL == cc)
	{
		return ;
	}
	client::raid_result_info *rri = NULL;
	if(fastest)
	{
		rri = rrm->add_reward2();
		psu->fill_role_data(rri->mutable_rd());
		psu->add_drop_props(cc->get_fast_reward(), rri);
	}
	switch(reward_type)
	{
		case 0:
			rri = rrm->add_reward1();
			psu->fill_role_data(rri->mutable_rd());
			psu->add_drop_props(cc->get_first_reward(), rri);
			break;
		case 1:
			rri = rrm->add_info();
			psu->fill_role_data(rri->mutable_rd());
			psu->add_drop_props(cc->get_again_reward(), rri);
			break;
		default:
			rri = NULL;
			break;
	}
}

int cycle_scene_inst::on_raid_complete(bool failure)
{
	uint32 used_tick = time_utils::tenms() - get_start_tick();
	std::vector<scene_user *> v;
	get_scene_users(v);
	for(size_t i = 0; i < v.size(); ++i)
	{
		if(failure)
		{
			client::raid_result_msg rrm;
			rrm.set_raid_id(get_scene_id());
			rrm.set_star(0);
			rrm.set_tick(used_tick);
			get_raid_over()->fill_failure(rrm.mutable_failure());
			v[i]->send_client(rrm);
		}
		else
		{
			on_raid_success(v[i], used_tick);
		}
	}
	return 0;
}

void cycle_scene_inst::on_raid_success(scene_user *su, uint32 tick)
{
	if(NULL == su)
	{
		return ;
	}
	player_role_cycle &prc = su->get_player_role().get_role_cycle();
	uint32 cur_level = prc.get_cur_level();
	client::cycle_update_record_req req;
	req.set_level(cur_level);
	req.set_role_id(su->get_role_id());
	req.set_raid_id(get_scene_id());
	get_raid_over()->fill_success(req.mutable_success());
	req.set_tick(tick);
	if(!_has_helper && !prc.award_fastest_prize(cur_level))
	{
		req.set_update(1);
	}
	else
	{
		req.set_update(0);
	}
	send_center(req);
}

bool cycle_scene_inst::is_scene_npc_all_dead()
{
    shobj* cur = _npc_map.get_all_objs_head();
    for (; cur != NULL; cur = cur->next[SLPE_LIST]) {
        scene_npc* n = static_cast<scene_npc*>(cur->sr);
        if (n->get_status().is_dead()) {
            continue;
        }
        if (n->get_real_role_type() == RTE_USER) {
            continue;
        }
		if (n->get_role_type() == RTE_PET) {
			continue;
		}
		if (n->is_robot())
		{
			continue;
		}
        return false;
	}
    return true;
}

bool cycle_scene_inst::is_friend(scene_role *sr0, scene_role *sr1)
{
	if(sr0 == NULL || sr1 == NULL)
	{
		return scene_inst::is_friend(sr0, sr1);
	}
	return sr0->get_camp() == sr1->get_camp();
}
