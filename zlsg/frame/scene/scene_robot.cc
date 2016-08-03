#include "config_npc.h"
#include "scene_robot.h"
#include "scene_inst.h"
#include "scene_pet.h"
#include "config_pet_skill.h"

scene_robot::scene_robot(const origin_npc& n, const uint64 id)
: scene_npc(n, id), _cfg(NULL), _ori(NULL), _pet(NULL), _backup(NULL)
{
	_mirror = false;
	_status = false;
}

scene_robot::~scene_robot()
{
	if (_cfg != NULL) {
		delete _cfg;
		_cfg = NULL;
	}

	if (_ori != NULL) {
		delete _ori;
		_ori = NULL;
	}
	if (_pet != NULL) {
		delete _pet;
		_pet = NULL;
	}
	if (_backup != NULL) {
		delete _backup;
		_backup = NULL;
	}
}

void scene_robot::attach_pet(const client::pet_db_info &gi)
{
	_pet = new client::pet_db_info(gi);
}

void scene_robot::attach_backup(const client::pet_db_info &gi)
{
	_backup = new client::pet_db_info(gi);
	_status = true;
}

int scene_robot::call_backup()
{
	if(NULL == _backup)
	{
		return -1;
	}
	if(!has_backup())
	{
		return -2;
	}
	if(get_status().is_dead())
	{
		return -3;
	}
	set_backup_status(false);
	return generate_pet(_backup);
}

void scene_robot::on_dead(uint32 tick)
{
	std::map<uint64, scene_role *> &pets = get_pets();
	std::map<uint64, scene_role *>::iterator it;
	for(it = pets.begin(); it != pets.end(); ++it)
	{
		 it->second->on_killed(NULL);
	}
	return scene_npc::on_dead(tick);
}

int scene_robot::call_pet()
{
	if(_pet == NULL)
	{
		return -1;
	}
	return generate_pet(_pet);
}

int scene_robot::generate_pet(client::pet_db_info *pet)
{
	scene_inst *psi = get_scene();
	if(NULL == psi)
	{
		return -2;
	}
	const origin_npc *n = GAME_MAP_MGR->get_npc(pet->pet_id());
	if(NULL == n)
	{
		return -3;
	}
	map_postion mp = get_scene_pos();
	get_scene()->calc_role_cell_to_map_postion(this, 1, mp.x, mp.y);
	scene_pet *sp = new scene_pet(*n, pet, psi->generate_pet_id(), this);
	if(pet->pfa_size() > 0)
	{
		map<int, int> attr;
		for(int i = 0; i < pet->pfa_size(); ++i)
		{
			attr.insert(make_pair(pet->pfa(i).id(), pet->pfa(i).num()));
		}
		sp->set_origin_attr(attr);
	}
	map<uint32, vector<skill> >::iterator i = _pet_skill.find(pet->pet_id());
	if(i != _pet_skill.end())
	{
		sp->init_scene_pet_skill(i->second);
	}
#if 0
	if(!is_mirror())
	{
		const config_pet_skill *cps = CONFMGR->get_config_pet_skill(pet->pet_id(), pet->pet_level());
		if(cps != NULL)
		{
			sp->init_scene_pet_skill(cps->get_pet_skill());
		}
	}
#endif
	add_pet(sp);
	sp->set_born_pos(mp);
	sp->set_scene_pos(mp);
	sp->set_camp(get_camp());
	sp->set_team_id(get_team_id());
	psi->enter_scene(sp);
	psi->screen_push_pet(sp);
	return 0;
}

int scene_robot::re_init_attr(const client::robot_attr *ra)
{
	set_role_name(ra->role_name().c_str(), ra->role_name().length());
	_attr.set_sex(ra->profession());
	_attr.set_profession(ra->profession());
	_attr.set_mounts_id(ra->mount_id());
	_attr.set_armor_id(ra->armor_id());
	_attr.set_weapon_id(ra->weapon_id());
	_attr.set_move_speed(ra->move_speed());
	_attr.set_max_hp(ra->hp_max());
	_attr.set_cur_hp(ra->hp_max());
	_attr.set_attack_min(ra->attack_min());
	_attr.set_attack_max(ra->attack_max());
	_attr.set_defense_min(ra->defense_min());
	_attr.set_defense_max(ra->defense_max());
	_attr.set_hit_point(ra->hit());
	_attr.set_dodge(ra->dodge());
	_attr.set_vip_level(ra->vip_level());
	get_origin_npc().mutable_attr()->CopyFrom(_attr);
	return 0;
}

void scene_robot::fill_role_data(client::role_data& rd) const
{
	rd.set_role_id(get_role_id());
	rd.set_inst_id(get_inst_id());
	rd.set_role_typ(client::role_data_TYPE_USER);
}

void scene_robot::fill_role_data(client::role_data* rd) const
{
	if (rd != NULL) {
		fill_role_data(*rd);
	}
}

// 排除掉美人技能
void scene_robot::rule_out_skill()
{
    std::vector<skill> &v = mutable_skill()->get_all_skill();
	vector<skill>::iterator it;
	for(it = v.begin(); it != v.end(); )
	{
		const config_skill *cs = CONFMGR->get_config_skill((*it).id);
		if(cs == NULL || cs->get_skills_belonging() > 6)
		{
			it = v.erase(it);
		}
		else
		{
			it++;
		}
	}
	mutable_skill()->init_min_atk_range();
	mutable_skill()->init_max_anger();
}

void scene_robot::init_pet_skill(const client::pet_db_info &pet, bool flag)
{
	uint32 pet_id = pet.pet_id();
	uint32 pet_level = pet.pet_level();
	vector<skill> &v = _pet_skill[pet_id];
	if(flag)
	{
		const role_skill& master_skill = get_skill();
	    master_skill.get_pet_skill(pet_id, pet_level, v);
	}
	else
	{
		const config_pet_skill *cps = CONFMGR->get_config_pet_skill(pet_id, pet_level);
		if(NULL == cps)
		{
			return ;
		}
		const vector<uint32> &skill_vec = cps->get_pet_skill();
		for(size_t i = 0; i < skill_vec.size(); ++i)
		{
			const config_skill *cs = CONFMGR->get_config_skill(skill_vec[i]);
			if(cs == NULL)
			{
				continue;
			}
			v.push_back(skill(*cs));
		}
	}
}

int scene_robot::init_robot(const client::robot_attr *ra)
{
	re_init_attr(ra);
	uint32 config_id = ra->config_id();
	bool flag = config_id == 1110 || config_id == 1112 || config_id == 1113;
	if(ra->has_pet())
	{
		init_pet_skill(ra->pet(), flag);
		attach_pet(ra->pet());
	}
	if(ra->has_backup())
	{
		init_pet_skill(ra->backup(), flag);
		attach_backup(ra->backup());
	}
	rule_out_skill();
	call_pet();
	return 0;
}
