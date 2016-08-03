#include "config_raid.h"
#include "config_npc.h"
#include "player_role.h"
#include "player_role_raid_mgr.h"
#include "game_map_mgr.h"
#include "scene_user.h"
#include "scene_npc.h"
#include "region_over.h"
#include "team_scene_inst.h"
#include "scene_robot.h"

team_scene_inst::team_scene_inst(uint32 id, const game_map* p, const raid_scene* s, struct amtsvr_context* c, const int m)
: raid_scene_inst(id, p, s, c, m)
{
	_team_id      = 0;
	_i			  = 0;
	_ready		  = false;
}

int team_scene_inst::init()
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

int team_scene_inst::on_kill_role(scene_role* atk, scene_role* def)
{
	if(def->get_role_type() == RTE_PET)
	{
		scene_role *sr = def->get_master();
		if(sr != NULL && sr->get_role_type() != RTE_USER)
		{
			scene_robot *robot = static_cast<scene_robot *>(sr);
			robot->call_backup();
		}
	}
	if(NULL == atk || ((atk->get_role_type() != RTE_USER) && atk->get_feature() != MT_USER_FRIEND))
	{
		return raid_scene_inst::on_kill_role(atk, def);
	}
	uint32 role_id = atk->get_role_id();
	_kill[role_id] += 1;
	uint32 scene_id = get_scene_id();
	if(scene_id == 70301 || scene_id == 70302)
	{
		client::team_kill_statistics_rsp rsp;
		rsp.set_role_id(role_id);
		rsp.set_role_name(atk->get_role_name());
		rsp.set_num(_kill[role_id]);
		team_broadcast(rsp);
	}
	return raid_scene_inst::on_kill_role(atk, def);
}

void team_scene_inst::get_reward_item(scene_user *psu, client::raid_result_info *rri)
{
	uint32 scene_id = get_scene_id();
	vector<client::props_info> props;
	if(scene_id == 70501)
	{
		make_props(50105001, calc_stone_num(), props);
		make_props(50085001, calc_pills_num(), props);
		for(size_t i = 0; i < props.size(); ++i)
		{
			client::props_info *pi = rri->add_props();
			pi->CopyFrom(props[i]);
		}
		psu->add_props(props);
	}
	else if(scene_id == 70502)
	{
		make_props(50105001, calc_stone_num() * 2, props);
		make_props(50085001, calc_pills_num() * 2, props);
		for(size_t i = 0; i < props.size(); ++i)
		{
			client::props_info *pi = rri->add_props();
			pi->CopyFrom(props[i]);
		}
		psu->add_props(props);
	}
	else
	{
		const config_raid* cr = get_map().get_config_raid();
		if(cr == NULL)
		{
			return ;
		}
		psu->add_props(cr->get_reward_item_2(), rri);
	}
}

int team_scene_inst::on_raid_complete(bool failure /*= false*/)
{
	_complete_time = (uint32)time(NULL);
	settlement(SETTLE_TYPE_COMPLETE);
	uint32 used_tick = time_utils::tenms() - get_start_tick();
	client::raid_result_msg msg;
	msg.set_raid_id(get_raid_id());
	msg.set_star(get_raid_scene().get_evaluation(used_tick));
	msg.set_tick(used_tick);
	std::vector<scene_user*> sus;
	get_scene_users(sus);
	const config_raid* cr = get_map().get_config_raid();
	if(NULL == cr)
	{
		return 0;
	}
	if (failure) 
	{
		get_raid_over()->fill_failure(msg.mutable_failure());
	} 
	else 
	{
		get_raid_over()->fill_success(msg.mutable_success());
	}
	for (size_t i = 0; i < sus.size(); ++i) 
	{
		if (!failure) 
		{
			client::raid_result_info* info = msg.add_info();
			sus[i]->fill_role_data(info->mutable_rd());
			info->set_coin(cr->get_reward_money_2());
			info->set_exp(cr->get_reward_experience_2());
			uint32 num = calc_tao_lve(sus[i]) + cr->get_reward_tao_lue();
			info->set_tao_lue(num);
			get_reward_item(sus[i], info);
			sus[i]->add_money(TAEL, cr->get_reward_money_2());
			sus[i]->add_normal_exp(cr->get_reward_experience_2());
			sus[i]->add_money(MILITARY_STRATEGY, num);
			sus[i]->send_client_change_attr();
			client::raid_info_msg rim;
			sus[i]->get_player_role().get_raid_mgr().push(sus[i]->get_role_id(), get_raid_id(), 1, 0, (uint32)time(NULL), get_type(), 1);
			sus[i]->get_player_role().get_raid_mgr().fill_raid_info_msg(rim, get_raid_id());
			sus[i]->send_client(rim);
			sus[i]->on_raid_complete(get_raid_id(), get_type(), msg.star());
		}
#ifndef __LIB_MINI_SERVER__
		sus[i]->send_client(msg);
		msg.mutable_info()->Clear();
#endif // __LIB_MINI_SERVER__
	}
	suspend(300);
	return 0;
}

int team_scene_inst::enter_scene(scene_npc *sn)
{
	if(sn->get_feature() == MT_USER_FRIEND)
	{
		sn->set_team_id(get_team_id());
		sn->set_camp(DEFENCE);
		uint32 ids[] = {7040161, 7040261};
		scene_role *sr = NULL;
		for(size_t i = 0; i < sizeof(ids) / sizeof(ids[0]); ++i)
		{
			sr = get_scene_npc_by_role_id(ids[i]);
			if(sr != NULL)
			{
				break;
			}
		}
		if(sr != NULL && sr != sn && sn->get_role_type() != RTE_PET)
		{
			sr->add_pet(sn);
		}

	}
	int ret = raid_scene_inst::enter_scene(sn);
	if(ret != 0)
	{
		return ret;
	}
	return 0;
}

int team_scene_inst::enter_scene(scene_user* u)
{
	u->set_camp(DEFENCE);
	u->set_team_id(get_team_id());
	int ret = raid_scene_inst::enter_scene(u);
	if (ret != 0) 
	{
		return ret;
	}
	return 0;
}

void team_scene_inst::on_role_leave(scene_role* psu)
{
	if(psu->get_role_type() == RTE_USER)
	{
		get_raid_over()->mark_dead_role(psu);
	    get_region_over()->mark_dead_role(psu);
	}
	psu->set_team_id(0);
	return raid_scene_inst::on_role_leave(psu);
}

void team_scene_inst::on_role_enter(scene_role *sr)
{
	if(sr->get_role_type() != RTE_USER)
	{
		raid_scene_inst::on_role_enter(sr);
		return;
	}
	scene_user *u = static_cast<scene_user *>(sr);
	const std::vector<uint32> &v = u->get_target();
	for(size_t i = 0; i < v.size(); ++i)
	{
		client::load_target_data_req req;
		req.set_role_id(u->get_role_id());
		req.set_target(v[i]);
		req.set_scene_id(get_scene_id());
		send_center(req);
	}
	rect r = _pos[_i % _pos.size()];
	map_postion p(r.top_left.x, r.top_left.y, 0);
	sr->set_scene_pos(p);
	sr->set_born_pos(p);
	++_i;
	CHECK_TARGET_PROGRESS(u, PASS_TEAM_RAID, get_scene_id());
	raid_scene_inst::on_role_enter(sr);
}

int team_scene_inst::push(client::robot_attr *ra)
{
	rect r = _pos[_i % _pos.size()];
	++_i;
	map_postion p(r.top_left.x, r.top_left.y, 0);
	scene_robot *sr = generate_robot(ra, p, ra->config_id(), MT_USER_FRIEND, 0);
	if(sr == NULL) 
	{
		return -1;
	}
	return 0;
}

int team_scene_inst::team_broadcast(google::protobuf::Message &msg)
{
	vector<scene_user *> v;
	get_scene_users(v);
	for(size_t i = 0; i < v.size(); ++i)
	{
		v[i]->send_client(msg);
	}
	return 0;
}

bool team_scene_inst::is_friend(scene_role* sr1, scene_role* sr2)
{
	if(sr1 == NULL || sr2 == NULL)
	{
		return false;
	}
	uint32 t1 = sr1->get_team_id();
	uint32 t2 = sr2->get_team_id();
	return t1 == t2;
}

bool team_scene_inst::is_scene_npc_all_dead()
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
        if (n->get_origin_npc().get_monster_type() == MT_USER_FRIEND ||
            n->get_origin_npc().get_monster_type() == MT_TEMP_PET) {
                continue;
        }
        return false;
    }
    return true;
}

uint32 team_scene_inst::calc_tao_lve(scene_user *su)
{
	int base = 0;
	switch(get_scene_id())
	{
		case 70301:
			base = CONFMGR->get_basic_value(10189);
			break;
		case 70302:
			base = CONFMGR->get_basic_value(10190);
			break;
		default:
			return 0;
	}
	map<uint32, uint32>::iterator it = _kill.find(su->get_role_id());
	if(it == _kill.end())
	{
		return 0;
	}
	return base * it->second;
}

uint32 team_scene_inst::calc_stone_num()
{
	map<uint32, uint32>::iterator it;
	uint32 num = 0;
	for(it = _kill.begin(); it != _kill.end(); ++it)
	{
		num += it->second;
	}
	if(0 <= num && num < 30)
	{
		return (uint32)floor(CONFMGR->get_basic_value(10191) / 100.0 * ceil((num + 1) / 3.0)) + (uint32)floor(CONFMGR->get_basic_value(10194) / 100.0);
	}
	if(30 <= num && num < 90)
	{
		return (uint32)floor(CONFMGR->get_basic_value(10192) / 100.0 * ceil((num + 1) / 3.0)) + (uint32)floor(CONFMGR->get_basic_value(10195) / 100.0);
	}
	return (uint32)floor(CONFMGR->get_basic_value(10193) / 100.0 * ceil((num + 1) / 3.0)) + (uint32)floor(CONFMGR->get_basic_value(10196) / 100.0);
}

uint32 team_scene_inst::calc_pills_num()
{
	map<uint32, uint32>::iterator it;
	uint32 num = 0;
	for(it = _kill.begin(); it != _kill.end(); ++it)
	{
		num += it->second;
	}
	if(0 <= num && num < 30)
	{
		return (uint32)floor(CONFMGR->get_basic_value(10197) / 100.0 * ceil((num + 1) / 3.0)) + (uint32)floor(CONFMGR->get_basic_value(10200) / 100.0);
	}
	if(30 <= num && num < 90)
	{
		return (uint32)floor(CONFMGR->get_basic_value(10198) / 100.0 * ceil((num + 1) / 3.0)) + (uint32)floor(CONFMGR->get_basic_value(10201) / 100.0);
	}
	return (uint32)floor(CONFMGR->get_basic_value(10199) / 100.0 * ceil((num + 1) / 3.0)) + (uint32)floor(CONFMGR->get_basic_value(10202) / 100.0);
}
