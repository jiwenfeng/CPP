#include "cfg_parser.h"
#include "config_robot.h"
#include "config_name.h"
#include <cmath>

config_robot_mgr::config_robot_mgr(TTableGroup_s* p)
    : _robot(p)
{
    init(p);
}

config_robot_mgr::~config_robot_mgr(void)
{
    clear();
}

int config_robot_mgr::init(TTableGroup_s* p)
{
    if (p == NULL) {
        return -1;
    }

    _robot = p;
    _table = _robot->GetTable("t_robot");
    if (_table == NULL) {
        printf("not found t_robot\n");
        return -2;
    }

    cfg_parser cp;
    for (int i = 1; i <= _table->m_recordCount; ++i) {
        uint32 id = _table->GetField(i, "RobotID");
		if(_cb.find(id) != _cb.end())
		{
			continue;
		}
        config_robot* p = new config_robot;
		p->_config_id	= id;
		p->_robot_level = _table->GetField(i, "RobotLv");
		p->_robot_job = _table->GetField(i, "RobotJob");
		p->_robot_combat = _table->GetField(i, "RobotCombat");
		p->_skill_combat = _table->GetField(i, "SkillCombat");
        cpf_0 p0(&p->_game_attribute);
        cp.parser<cpf_0>(_table->GetFieldString(i, "RobotAttribute").c_str(), ";", p0);
		cpf_0 p1(&p->_g_game_attribute);
		cp.parser<cpf_0>(_table->GetFieldString(i, "RGeneralAttribute").c_str(), ";", p1);
		p->_robot_ai = _table->GetFieldString(i, "RobotAI");
		p->_mount_id = _table->GetField(i, "RMountID");
		p->_mount_skill = _table->GetField(i, "RMountSkill");
		p->_weapon_id = _table->GetField(i, "RItemID1");
		p->_armor_id = _table->GetField(i, "RItemID2");
		p->_general_id = _table->GetField(i, "RGeneralID");
		p->_general_level = _table->GetField(i, "RGeneralLv");
		p->_standby_pet_id = _table->GetField(i, "RGeneralID2");
		p->_standby_pet_level = _table->GetField(i, "RGeneralLv2");
		p->_percent = _table->GetField(i, "RobotPercent");
		sscanf(_table->GetFieldString(i, "RAttrFloat").c_str(), "%d|%d", &p->_min, &p->_max);
		p->_attack_cool = _table->GetField(i, "RAttackCool");
		p->_attack_mode = _table->GetField(i, "RAttackMode");
		p->_corpse_remain_time = _table->GetField(i, "RCorpseRemainTime");
		p->_random_move_range = _table->GetField(i, "RRandomMoveRange");
		p->_visible_range = _table->GetField(i, "RVisibleRange");
		p->_follow_range = _table->GetField(i, "RFollowRange");
		p->_monster_type = _table->GetField(i, "RMonsterType");
		p->_default_skill = _table->GetField(i, "RDefaultSkill");

        char buf[32] = {0};
        for (int j = 1; j <= 9; ++j) {
            sprintf(buf, "RSkill%d", j);
            uint32 skill_id = _table->GetField(i, buf);
            if (skill_id != 0) {
                p->_skills.push_back(skill_id);
            }
        }

        p->_skills.push_back(p->_default_skill);

		_cb.insert(make_pair(id, p));
    }

    return 0;
}

int config_robot_mgr::clear()
{
    for (std::map<uint32, config_robot*>::iterator i = _cb.begin(); i != _cb.end(); ++i) 
	{
        if (i->second != NULL) 
		{
            delete i->second;
        }
    }

    _cb.clear();

    return 0;
}

const config_robot *config_robot_mgr::get_config_robot(uint32 id) const
{
	std::map<uint32, config_robot *>::const_iterator it = _cb.find(id);
	if(it == _cb.end())
	{
		return NULL;
	}
	return it->second;
}

string config_robot_mgr::get_robot_ai(uint32 id) const
{
	std::map<uint32, config_robot *>::const_iterator it = _cb.find(id);
	if(it == _cb.end())
	{
		return "";
	}
	return it->second->_robot_ai;
}

const std::vector<uint32> &config_robot_mgr::get_skills(uint32 id) const
{
	std::map<uint32, config_robot *>::const_iterator it = _cb.find(id);
	if(it == _cb.end())
	{
		static vector<uint32> v;
		return v;
	}
	return it->second->_skills;
}

