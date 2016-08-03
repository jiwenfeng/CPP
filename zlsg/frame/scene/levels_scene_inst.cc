#include "config_raid.h"
#include "config_npc.h"
#include "config_giant.h"
#include "config_giantraid.h"
#include "config_upgrade.h"
#include "player_role.h"
#include "player_role_raid_mgr.h"
#include "player_role_levels.h"
#include "game_map_mgr.h"
#include "create_npc_rule.h"
#include "raid_over.h"
#include "region_over.h"
#include "scene_npc.h"
#include "scene_robot.h"
#include "scene_user.h"
#include "levels_scene_inst.h"

levels_scene_inst::levels_scene_inst(uint32 id, const game_map* p, const raid_scene* s, struct amtsvr_context* c, const int m)
    : raid_scene_inst(id, p, s, c, m)/*, _rsc(s)*/
{
    _team_id      = 1;
	_load = false;
}

levels_scene_inst::~levels_scene_inst()
{
    map<uint32, origin_npc *>::iterator it;
    for(it = _on.begin(); it != _on.end(); ++it)
    {
        delete it->second;
    }
    _on.clear();
}

int levels_scene_inst::init()
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
	for(size_t i = 0; i < 2; ++i)
	{
		_atk_pos.push_back(tmp[0]._r[i]);
	}
	for(size_t i = 2; i < 5; ++i)
	{
		_def_pos.push_back(tmp[0]._r[i]);
	}
    return raid_scene_inst::init();
}

int levels_scene_inst::on_kill_role(scene_role* atk, scene_role* def)
{
    if((def->get_team_id() == 0 && def->get_role_type() != RTE_PET) || def->get_role_type() == RTE_USER)
    {
        get_raid_over()->mark_dead_role(def);
        get_region_over()->mark_dead_role(def);
    }
    return scene_inst::on_kill_role(atk, def);
}

void get_drops(uint32 drop_id, std::vector<client::props_info>& drops);

int levels_scene_inst::on_raid_complete(bool failure /*= false*/)
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
#ifndef __LIB_MINI_SERVER__
    for (size_t i = 0; i < sus.size(); ++i) 
	{
        client::raid_result_info *info = msg.add_info();
        sus[i]->fill_role_data(info->mutable_rd());
        player_role_levels &prl = sus[i]->get_player_role().get_player_role_levels();
        uint32 cur_level = prl.get_cur_level();
        const config_giantraid *cg = CONFMGR->get_config_giantraid(cur_level);
        if(cg == NULL)
        {
            continue;
        }
        const config_upgrade *cu = CONFMGR->get_config_upgrade(sus[i]->get_attr().get_val(PROFESSION), sus[i]->get_attr().get_val(LEVEL));
        if(cu == NULL)
        {
            continue;
        }
        uint32 money = cg->get_reward_money() * cu->get_bz_obtain() / 10000;
        info->set_coin(money);
        info->set_strategy(cg->get_reward_resource());
		std::vector<client::props_info> props;
		for(size_t i = 0; i < cg->get_reward_item().size(); ++i)
		{
			make_props(cg->get_reward_item().at(i), props, sus[i]->get_job());
		}
		for(size_t i = 0; i < props.size(); ++i)
		{
			client::props_info* ppi = info->add_props();
			ppi->CopyFrom(props[i]);
		}
    }
#endif

    if (failure) {
        get_raid_over()->fill_failure(msg.mutable_failure());
    } else {
        get_raid_over()->fill_success(msg.mutable_success());
    }

    for (size_t i = 0; i < sus.size(); ++i) {
#ifdef __LIB_MINI_SERVER__
        msg.mutable_verify()->set_max_hp(sus[i]->get_attr(MAX_HP));
        msg.mutable_verify()->set_max_atk(sus[i]->get_attr(ATTACK_MAX));
        msg.mutable_verify()->set_max_def(sus[i]->get_attr(DEFENSE_MAX));
		msg.mutable_verify()->set_bow_index(sus[i]->get_attr(BOW_INDEX));

        if (failure) {
            sus[i]->send_client(msg);
        } else {
            client::mini_server_end_msg end;
            end.mutable_msg()->CopyFrom(msg);
            sus[i]->send_client(end);
        }
#endif // __LIB_MINI_SERVER__
	}
	return 0;
}

int levels_scene_inst::enter_scene(scene_user* u)
{
    u->set_team_id(1);
	u->set_camp(1);
    int ret = raid_scene_inst::enter_scene(u);
    if (ret != 0) {
        return ret;
    }
    return 0;
}

void levels_scene_inst::on_role_leave(scene_role* psu)
{
    if (_user_map.obj_num() == 0) {
        set_close(true);
    }

    psu->set_team_id(0);
	psu->set_camp(0);

    return scene_inst::on_role_leave(psu);
}

scene_npc *levels_scene_inst::generate_robot(uint32 id, uint32 lv, uint32 power, const map_postion &p, uint32 team_id)
{
	const config_npc *cn = CONFMGR->get_config_giant(id);
    if(cn == NULL)
    {
        return NULL;
    }
    origin_npc *o = new origin_npc(cn);
    if(o == NULL)
    {
        return NULL;
    }
    o->init();
    scene_npc *n = generate_npc_inst(*o, (uint32)time(NULL), p, GNTE_NORMAL, false);
    if(n == NULL)
    {
        delete o;
        return NULL;
    }
    origin_npc &origin_self = n->get_origin_npc();
    for (uint32 i = MAX_HP; i < MOVE_SPEED; ++i) 
    {
        origin_self.mutable_attr()->multi_val((ROLE_ATTRIBUTE)i, power / 10000.0f);
        n->mutable_attr()->multi_val((ROLE_ATTRIBUTE)i, power / 10000.0f);
    }
	n->mutable_attr()->set_val((ROLE_ATTRIBUTE)LEVEL, lv);
    n->set_team_id(team_id);
	n->set_camp(team_id);
    if(scene_inst::enter_scene(n) != 0)
    {
        delete o;
        delete n;
        return NULL;
    }
	screen_push_npc(n);
    _on.insert(make_pair(id, o));
    return n;
}

bool levels_scene_inst::is_scene_npc_all_dead()
{
	if(!_load)
	{
		return false;
	}
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
        if(n->get_team_id() != 0)
        {
            continue;
        }
        return false;
    }
    return true;
}

void levels_scene_inst::load_my_enemy(scene_user *su)
{
    player_role &pr = su->get_player_role();
    player_role_levels &prl = pr.get_player_role_levels();
    uint32 cur_level = prl.get_cur_level();
    const client::level_info *li = prl.get_level_info(cur_level);
    if(li == NULL)
    {
		amtsvr_log("%d level get level info failed\n", cur_level);
        return ;
    }
    for(int i = 0; i < li->lhi_size(); ++i)
    {
        rect r = _def_pos[i];
        map_postion p(r.top_left.x, r.top_left.y, 0);
        const client::level_hero_info& lhi = li->lhi(i);
		scene_npc *sn = generate_robot(lhi.id(), lhi.level(), lhi.power(), p, 0);
        if(sn == NULL)
        {
			amtsvr_log("generate robot failed:%u %u %d\n", lhi.id(), lhi.level() / 10 * 10, lhi.power());
            continue;
        }
    }
}

void levels_scene_inst::load_npc(scene_user *su)
{
	if(_load)
	{
		return;
	}
    load_my_enemy(su);
	_load = true;
}

bool levels_scene_inst::is_friend(scene_role *sr1, scene_role *sr2)
{
    if(sr1 == NULL || sr2 == NULL)
    {
        return false;
    }
	return sr1->get_team_id() == sr2->get_team_id();
}

void levels_scene_inst::set_scene_ready(bool ready, scene_user *psu)
{
	load_npc(psu);
	raid_scene_inst::set_scene_ready(ready, psu);
}
