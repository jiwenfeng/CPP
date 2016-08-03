#include "config_raid.h"
#include "game_map_mgr.h"
#include "scene_user.h"
#include "scene_npc.h"
#include "client.world_boss.pb.h"
#include "boss_scene_inst.h"
#include "scene_lose_mgr.h"
#include "config_npc.h"
#include "client.operate_mail.pb.h"

#define BOSS_STANDBY	0
#define BOSS_ESCAPED	1
#define BOSS_DEAD		2

boss_scene_inst::boss_scene_inst(uint32 id, const game_map* p, const scene* s, struct amtsvr_context* c, const int m /*= DEFAUTL_WORLDBOSS_SCENE_USER_NUM */) : scene_inst(id, p, s, c, m), _status(BOSS_ESCAPED)
{
    _lock = 0;
}

void boss_scene_inst::refresh_boss()
{
	if(_status == BOSS_STANDBY)
	{
		return;
	}
	const config_raid *cr = CONFMGR->get_config_raid(get_scene_id());
	if(cr == NULL)
	{
		amtsvr_log("config raid %u not found\n", get_scene_id());
		return;
	}
	uint32 id = cr->get_map_monster_id();
	const map_postion &p = cr->get_monster_postion();
    const origin_npc* o = GAME_MAP_MGR->get_npc(id);
    if (o == NULL) {
        amtsvr_log("scene:%u npc:%u not found\n", get_scene_id(), id);
        return ;
    }
	scene_npc *n = generate_npc_inst(*o, (uint32)time(NULL), p);
	if(n == NULL)
	{
		amtsvr_log("generate npc failed:%d:%d\n", get_scene_id(), id);
		return ;
	}
	screen_push_npc(n);
	_status = BOSS_STANDBY;
	send_boss_status();
}

void boss_scene_inst::announce_boss_killed(scene_role *def)
{
	if(def == NULL)
	{
		return ;
	}
	scene_role *sr = def->get_hatest_role();
	if(sr == NULL)
	{
		return ;
	}
	scene_user *su = static_cast<scene_user *>(sr);
	const config_raid *cr = CONFMGR->get_config_raid(get_scene_id());
	if(cr == NULL)
	{
		return ;
	}
	scene_npc *npc = static_cast<scene_npc *>(def);
	if(npc == NULL)
	{
		return ;
	}
    const config_npc* cn = npc->get_origin_npc().get_config_npc();
	if(def == NULL)
	{
		return ;
	}
	client::broadcast_msg_req req;
	req.set_area_id(40);
	client::content_data *cd1 = req.add_data();
	cd1->set_s(su->get_role_name());
//	client::content_data *cd2 = req.add_data();
//	cd2->set_n(cr->get_map_common_line());
	client::content_data *cd3 = req.add_data();
	cd3->set_s(cn->get_npc_name1());
	send_center(req);
}

int boss_scene_inst::on_kill_role(scene_role *atk, scene_role *def)
{
    if (def == NULL) {
		return scene_inst::on_kill_role(atk, def);
    }
    if (def->get_role_type() == RTE_USER) {
        client::dead_in_raid d;
        d.set_time(get_relive_timelong());
        def->send_client(d);
    }
	if(_status != BOSS_STANDBY)
	{
		return scene_inst::on_kill_role(atk, def);
	}
	if(def->get_role_type() == RTE_NPC) {
		scene_npc *npc = static_cast<scene_npc *>(def);
		if(!npc->get_origin_npc().is_world_boss())
		{
			return scene_inst::on_kill_role(atk, def);
		}
		_status = BOSS_DEAD;
		send_boss_status();
		announce_boss_killed(def);
	}
	return scene_inst::on_kill_role(atk, def);
}

void boss_scene_inst::on_role_disappear(scene_role *sr)
{
	if(sr->get_role_type() == RTE_NPC)
	{
		scene_npc *sn = static_cast<scene_npc *>(sr);
		if(sn->get_origin_npc().is_world_boss())
		{
			_status = BOSS_ESCAPED;
			send_boss_status();
		}
	}
	return scene_inst::on_role_disappear(sr);
}

void boss_scene_inst::send_boss_status()
{
	client::boss_status bs;
	bs.set_raid_id(get_scene_id());
	bs.set_status(_status);
	send_center(bs);
}

bool boss_scene_inst::is_friend(scene_role* psr0, scene_role* psr1)
{
	if(NULL == psr0 || NULL == psr1)
	{
		return scene_inst::is_friend(psr0, psr1);
	}
	if(psr0->same_master(psr1))
	{
		return true;
	}
	if(((psr0->get_role_type() == RTE_USER || psr0->get_role_type() == RTE_PET) && psr1->get_role_type() == RTE_NPC) || ((psr0->get_role_type() == RTE_NPC) && (psr1->get_role_type() == RTE_USER || psr1->get_role_type() == RTE_PET)))
	{
		return false;
	}
	if(psr0->get_status().atk_mode() == client::role_status_attack_mode_attack_mod_all)
	{
		return false;
	}
	if(psr0->is_counter_role(psr1))
	{
		return false;
	}
	if(psr0->get_status().atk_mode() == client::role_status_attack_mode_attack_mod_peace || psr0->get_status().atk_mode() == client::role_status_attack_mode_attack_mod_null)
	{
		return true;
	}
	if(psr0->get_status().atk_mode() == client::role_status_attack_mode_attack_mod_family)
	{
		uint32 f0 = psr0->get_family_id();
		uint32 f1 = psr1->get_family_id();
		if(f0 == 0 || f1 == 0)
		{
			return false;
		}
		scene_role *sr = psr0->get_master();
		if(sr == NULL)
		{
			sr = psr0;
		}
		if(sr->get_role_type() != RTE_USER) // it's a damn robot
		{
			return false;
		}
		return static_cast<scene_user *>(sr)->is_friend_family(f1);
	}
	return false;
}
