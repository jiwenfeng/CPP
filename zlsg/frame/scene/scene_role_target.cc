#include "scene_user.h"
#include "player_role_vip.h"
#include "client.target.pb.h"
#include "player_role.h"
#include "player_role_mission.h"
#include "scene_role_target.h"
#include "config_target.h"

map<uint32, void *> scene_role_target::_func_ptrs;

void scene_role_target::init_target_func()
{
	_func_ptrs.insert(make_pair(ROLE_UPGRADE, (void *)check_user_upgrade_target));
	_func_ptrs.insert(make_pair(PASS_COPY, (void *)check_raid_target));			
	_func_ptrs.insert(make_pair(PET_UPGRADE, (void *)check_pet_upgrade_target));
	_func_ptrs.insert(make_pair(PET_LINGERING, (void *)check_pet_linger_target));
	_func_ptrs.insert(make_pair(PET_NUMBER, (void *)check_pet_num_target));
	_func_ptrs.insert(make_pair(MOUNT_ADVANCE, (void *)check_mount_advance_target));
	_func_ptrs.insert(make_pair(MOUNT_UPGRADE, (void *)check_mount_upgrade_target));
	_func_ptrs.insert(make_pair(POTENTION_ACTIVE, (void *)check_pentation_target));
	_func_ptrs.insert(make_pair(EQUIP_UP_STAR, (void *)check_equip_star_target));
	_func_ptrs.insert(make_pair(EQUIP_XILIAN, (void *)check_equip_xilian_target));	
	_func_ptrs.insert(make_pair(ROLE_SKILL_UPGRADE, (void *)check_user_skill_upgrade_target));	
	_func_ptrs.insert(make_pair(MOUNT_SKILL_UPGRADE, (void *)check_mount_skill_upgrade_target));
	_func_ptrs.insert(make_pair(PET_SKILL_UPGRADE, (void *)check_pet_skill_upgrade_target));
	_func_ptrs.insert(make_pair(SMELT_EQUIP, (void *)check_smelt_equip_target));
	_func_ptrs.insert(make_pair(TREASURE, (void *)check_treasure_target));
	_func_ptrs.insert(make_pair(ARENA_CHALLENGE, (void *)check_arena_challenge_target));	
	_func_ptrs.insert(make_pair(FAMILY_DONATE, (void *)check_family_donate_target));
	_func_ptrs.insert(make_pair(BUY_COPPER, (void *)check_buy_copper_target));
	_func_ptrs.insert(make_pair(GET_PHYSIC_POWER, (void *)check_get_power_target));
	_func_ptrs.insert(make_pair(GET_INGOT, (void *)check_get_ingot_target));
	_func_ptrs.insert(make_pair(PASS_TEAM_RAID, (void *)check_team_raid_target));
	_func_ptrs.insert(make_pair(PASS_PET_RAID, (void *)check_pet_raid_target));
	_func_ptrs.insert(make_pair(PASS_LEVELS_RAID, (void *)check_levels_raid_target));
	_func_ptrs.insert(make_pair(ROLE_SKILL_UPGRADE_STAR, (void *)check_user_skill_upgrade_star_target));
	_func_ptrs.insert(make_pair(BUY_PHYSICAL_POWER, (void *)check_buy_physical_power));	
	_func_ptrs.insert(make_pair(SHOPPING, (void *)check_shopping_target));
	_func_ptrs.insert(make_pair(CYCLE_MOPUP, (void *)check_cycle_mopup_target));	
	_func_ptrs.insert(make_pair(DEVIL_RESET, (void *)check_devil_reset_target));	
	_func_ptrs.insert(make_pair(GEM_UPGRADE, (void *)check_gem_upgrade_target));
	_func_ptrs.insert(make_pair(GEM_LEVEL, (void *)check_gem_level_target));
	_func_ptrs.insert(make_pair(BOW_LEVEL, (void *)check_bow_level_target));
}

void *scene_role_target::get_func_ptr(uint32 type)
{
	 map<uint32, void *>::iterator i = _func_ptrs.find(type);
	 if(i == _func_ptrs.end()) 
	 {
		 return NULL;
	 }
	 return i->second;
}

// 升级
int scene_role_target::check_user_upgrade_target(scene_user *psu, uint32 type, uint32 lv)
{
	client::target_progress tp;
	tp.set_value(lv);
	return scene_role_target::check_progress(psu, type, &tp);
}

// 美人关
int scene_role_target::check_pet_raid_target(scene_user *psu, uint32 type, uint32 raid_id, uint32 num)
{
	client::target_progress tp;
	tp.set_value(num);
	tp.add_equal(raid_id);
	return scene_role_target::check_progress(psu, type, &tp);
}

// 过关斩将
int scene_role_target::check_levels_raid_target(scene_user *psu, uint32 type, uint32 level)
{
	client::target_progress tp;
	tp.set_value(1);
	tp.add_equal(level);
	tp.add_great(level);
	return scene_role_target::check_progress(psu, type, &tp);
}

// 组队副本
int scene_role_target::check_team_raid_target(scene_user *psu, uint32 type, uint32 raid_id)
{
	client::target_progress tp;
	tp.add_equal(raid_id);
	tp.set_value(1);
	return scene_role_target::check_progress(psu, type, &tp);
}

// 过副本
int scene_role_target::check_raid_target(scene_user *psu, uint32 subtype, uint32 raid_id, uint32 star, uint32 type, uint32 num /* = 1*/)
{
	client::target_progress tp;
	switch(star)
	{
		case 1:
			tp.add_great(3);
			break;
		case 3:
			tp.add_great(1);
			break;
		default:
			tp.add_great(star);
			break;
	}
	tp.set_value(num);
	tp.add_equal(type + 1);
	tp.add_equal(raid_id);
	return scene_role_target::check_progress(psu, subtype, &tp);
}

// 招募美人
int scene_role_target::check_pet_num_target(scene_user *psu, uint32 type)
{
	client::target_progress tp;
	tp.set_value(psu->get_pet_count());
	return scene_role_target::check_progress(psu, type, &tp);
}

// 美人升级
int scene_role_target::check_pet_upgrade_target(scene_user *psu, uint32 type, uint32 lv)
{
	client::target_progress tp;
	tp.add_great(lv);
	int num = 0;
	player_role_pet &role_pet = psu->get_player_role().get_pets();
	const map<uint32, client::pet_db_info> &pets = role_pet.get_all_pet_db_info();
	map<uint32, client::pet_db_info>::const_iterator it;
	for(it = pets.begin(); it != pets.end(); ++it)
	{
		if(it->second.pet_level() >= lv)
		{
			num++;
		}
	}
	tp.set_value(num);
	return scene_role_target::check_progress(psu, type, &tp);
}

int scene_role_target::check_smelt_equip_target(scene_user *psu, uint32 type)
{
	client::target_progress tp;
	tp.set_value(1);
	return scene_role_target::check_progress(psu, type, &tp);
}

// 美人缠绵
int scene_role_target::check_pet_linger_target(scene_user *psu, uint32 type, uint32 count)
{
	client::target_progress tp;
	tp.add_great(count);
	int num = 0;
	player_role_pet &role_pet = psu->get_player_role().get_pets();
	const map<uint32, client::pet_db_info> &pets = role_pet.get_all_pet_db_info();
	map<uint32, client::pet_db_info>::const_iterator it;
	for(it = pets.begin(); it != pets.end(); ++it)
	{
		if(it->second.lingering_count() >= count)
		{
			num++;
		}
	}
	tp.set_value(num);
	return scene_role_target::check_progress(psu, type, &tp);
}

// 坐骑升级
int scene_role_target::check_mount_upgrade_target(scene_user *psu, uint32 type, uint32 lv)
{
	client::target_progress tp;
	tp.set_value(lv);
	return scene_role_target::check_progress(psu, type, &tp);
}

// 坐骑进阶
int scene_role_target::check_mount_advance_target(scene_user *psu, uint32 type, uint32 id)
{
	client::target_progress tp;
	tp.set_value(id);
	return scene_role_target::check_progress(psu, type, &tp);
}

// 激活心法
int scene_role_target::check_pentation_target(scene_user *psu, uint32 type, uint32 id)
{
	client::target_progress tp;
	tp.set_value(id);
	return scene_role_target::check_progress(psu, type, &tp);
}

// 装备升星
int scene_role_target::check_equip_star_target(scene_user *psu, uint32 type, uint32 n)
{
	client::target_progress tp;
	tp.set_value(n);
	return scene_role_target::check_progress(psu, type, &tp);
}

// 装备洗练
int scene_role_target::check_equip_xilian_target(scene_user *psu, uint32 type)
{
	client::target_progress tp;
	tp.set_value(1);
	return scene_role_target::check_progress(psu, type, &tp);
}

// 玩家技能升级
int scene_role_target::check_user_skill_upgrade_target(scene_user *psu, uint32 type)
{
	client::target_progress tp;
	tp.set_value(1);
	return scene_role_target::check_progress(psu, type, &tp);
}

// 玩家技能升星
int scene_role_target::check_user_skill_upgrade_star_target(scene_user *psu, uint32 type, uint32 id)
{
	client::target_progress tp;
	tp.set_value(1);
	tp.add_equal(id);
	return scene_role_target::check_progress(psu, type, &tp);
}

// 坐骑技能升级
int scene_role_target::check_mount_skill_upgrade_target(scene_user *psu, uint32 type)
{
	client::target_progress tp;
	tp.set_value(1);
	return scene_role_target::check_progress(psu, type, &tp);
}

// 寻宝
int scene_role_target::check_treasure_target(scene_user *psu, uint32 type, uint32 n)
{
	client::target_progress tp;
	tp.set_value(n);
	return scene_role_target::check_progress(psu, type, &tp);
}

// 美人技能升级
int scene_role_target::check_pet_skill_upgrade_target(scene_user *psu, uint32 type)
{
	client::target_progress tp;
	tp.set_value(1);
	return scene_role_target::check_progress(psu, type, &tp);
}

// 竞技场挑战
int scene_role_target::check_arena_challenge_target(scene_user *psu, uint32 type)
{
	client::target_progress tp;
	tp.set_value(1);
	return scene_role_target::check_progress(psu, type, &tp);
}

// 军团捐献
int scene_role_target::check_family_donate_target(scene_user *psu, uint32 subtype, uint32 type)
{
	client::target_progress tp;
	tp.add_equal(type);
	tp.set_value(1);
	return scene_role_target::check_progress(psu, subtype, &tp);
}

// 购买铜钱
int scene_role_target::check_buy_copper_target(scene_user *psu, uint32 type)
{
	client::target_progress tp;
	tp.set_value(1);
	return scene_role_target::check_progress(psu, type, &tp);
}

int scene_role_target::check_buy_physical_power(scene_user *psu, uint32 type)
{
	client::target_progress tp;
	tp.set_value(1);
	return scene_role_target::check_progress(psu, type, &tp);
}

int scene_role_target::check_shopping_target(scene_user *psu, uint32 subtype, uint32 type)
{
	client::target_progress tp;
	tp.set_value(1);
	tp.add_equal(type);
	return scene_role_target::check_progress(psu, subtype, &tp);
}

// 在线领取体力
int scene_role_target::check_get_power_target(scene_user *psu, uint32 type)
{
	client::target_progress tp;
	time_t now = time(NULL);
	struct tm *tm= localtime(&now);
	tp.add_great(tm->tm_hour);
	tp.add_great(tm->tm_min);
	tp.add_great(tm->tm_sec);
	tp.add_less(tm->tm_hour);
	tp.add_less(tm->tm_min);
	tp.add_less(tm->tm_sec);
	tp.set_value(1);
	return scene_role_target::check_progress(psu, type, &tp);
}

int scene_role_target::check_get_ingot_target(scene_user *psu, uint32 type)
{
	player_role_vip &v = psu->get_player_role().get_player_role_vip();
	if(v.get_month_timeout() <= (uint32)time(NULL))
	{
		return 0;
	}
	client::target_progress tp;
	tp.set_value(1);
	return scene_role_target::check_progress(psu, type, &tp);
}

int scene_role_target::check_cycle_mopup_target(scene_user *psu, uint32 type)
{
	client::target_progress tp;
	tp.set_value(1);
	return scene_role_target::check_progress(psu, type, &tp);
}

int scene_role_target::check_devil_reset_target(scene_user *psu, uint32 type)
{
	client::target_progress tp;
	tp.set_value(1);
	return scene_role_target::check_progress(psu, type, &tp);
}

int scene_role_target::check_gem_upgrade_target(scene_user *psu, uint32 type)
{
	client::target_progress tp;
	tp.set_value(1);
	return scene_role_target::check_progress(psu, type, &tp);
}

int scene_role_target::check_gem_level_target(scene_user *psu, uint32 type, uint32 n)
{
	client::target_progress tp;
	tp.set_value(n);
	return scene_role_target::check_progress(psu, type, &tp);
}

int scene_role_target::check_bow_level_target(scene_user *psu, uint32 type, uint32 n)
{
	client::target_progress tp;
	tp.set_value(n);
	return scene_role_target::check_progress(psu, type, &tp);
}

bool scene_role_target::check_target_cond(const config_target *ct, const target_progress *tp)
{
	const client::target_complete_cond &tcc = ct->get_config_target_complete_cond();
	if(tcc.equal_size() > tp->equal_size())
	{
		return false;
	}
	if(tcc.great_size() > tp->great_size())
	{
		return false;
	}
	if(tcc.less_size() > tp->less_size())
	{
		return false;
	}
	for(int i = 0; i < tcc.equal_size(); ++i)
	{
		bool ret = false;
		for(int j = 0; j < tcc.equal(i).value_size(); ++j)
		{
			if(tp->equal(i) == tcc.equal(i).value(j))
			{
				ret = true;
				break;
			}
		}
		if(!ret)
		{
			return false;
		}
	}
	for(int i = 0; i < tcc.great_size(); ++i)
	{
		bool ret = false;
		for(int j = 0; j < tcc.great(i).value_size(); ++j)
		{
			if(tp->great(i) >= tcc.great(i).value(j))
			{
				ret = true;
				break;
			}
		}
		if(!ret)
		{
			return false;
		}
	}
	for(int i = 0; i < tcc.less_size(); ++i)
	{
		bool ret = false;
		for(int j = 0; j < tcc.less(i).value_size(); ++j)
		{
			if(tp->less(i) <= tcc.less(i).value(j))
			{
				ret = true;
				break;
			}
		}
		if(!ret)
		{
			return false;
		}
	}
	return true;
}

int scene_role_target::check_progress(scene_user *psu, uint32 type, const target_progress *tp)
{
	player_role &pr = psu->get_player_role();
	player_role_mission &prm = pr.get_player_role_target();
	client::point_tips_rsp rsp;
	bool flag = false;
	const vector<config_target *> &v = CONFMGR->get_sub_targets(type);
	uint32 level = psu->get_level();
	uint32 mount = psu->get_attr().get_val(MOUNTS_ID);
	uint32 now = (uint32)time(NULL);
	for(size_t i = 0; i < v.size(); ++i)
	{
		const config_target *t = v[i];
		if(t->get_config_target_subtype() != type)
		{
			continue;
		}
		uint32 level_limit = t->get_config_target_level_limit();
		if(level_limit != 0 && level_limit > level)
		{
			continue;
		}
		uint32 mount_limit = t->get_config_target_mount_limit();
		if(mount_limit != 0 && mount_limit > mount)
		{
			continue;
		}
		uint32 hour = t->get_config_target_start_hour();
		uint32 last = t->get_config_target_last();
		if(last != 0 && !time_utils::is_in_time_range(now, 0, hour, 0, 0, last))
		{
			continue;
		}
		uint32 target_id = t->get_config_target_id();
		if(!prm.target_is_doing(target_id))
		{
			continue;
		}
		if(!scene_role_target::check_target_cond(t, tp))
		{
			continue;
		}
		uint32 n = prm.modify_target_progress(target_id, tp->value(), t->get_config_target_type() == client::DAILY_MISSION);
		if(n >= t->get_config_target_complete())
		{
			prm.set_target_status(target_id, client::STATUS_PRIZE);
			uint32 prev = t->get_config_target_prev();
			if(prev != 0 && !prm.target_has_finished(prev))
			{
				continue;
			}
			rsp.add_fc((client::TARGET_TYPE_E)t->get_config_target_type());
			flag = true;
		}
	}
	if(flag)
	{
		psu->send_client(rsp);
	}
	return 0;
}
