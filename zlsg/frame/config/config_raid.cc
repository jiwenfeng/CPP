#include "config_mgr.h"
#include "config_raid.h"


const rect &config_raid::get_revive_rect(uint32 camp)const
{
	switch(camp)
	{
		case 0:
			return _revive_region_3;
		case 1:
			return _revive_region_2;
		default:
			return _revive_region_1;
	}
	assert(0);
}

config_raid_mgr::config_raid_mgr(TTableGroup_s* p/* = NULL*/)
    : _raid(p)
{
    init(p);
}

config_raid_mgr::~config_raid_mgr()
{
    clear();
}


void config_raid_mgr::format_pos(const string &str, map_postion &pos)
{
	sscanf(str.c_str(), "%d,%d", &pos.x, &pos.y);
	pos.z = 0;
}

void config_raid_mgr::format_rect(const string & str, rect &r)
{
	int x1 = 0, y1 = 0, x2 = 0, y2 = 0;
	sscanf(str.c_str(), "%d_%d_%d_%d", &x1, &y1, &x2, &y2);
	r.set(x1, y1, x2, y2);
}

int config_raid_mgr::init(TTableGroup_s* p)
{
    if (p == NULL) {
        return -1;
    }

    _raid = p;
    _table = _raid->GetTable("t_scene");
    if (_table == NULL) {
        printf("not found t_scene\n");
        return -2;
    }

    cfg_parser cp;
    for (int i = 1; i <= _table->m_recordCount; ++i) {
        int id = _table->GetField(i, "MapID");
        if (id == 0) {
            continue;
        }

        config_raid* p = NULL;
        if (_cr.find(id) != _cr.end()) {
            p = _cr[id];
        } else {
            p = new config_raid;
        }

        p->_map_id                  = _table->GetField      (i, "MapID"                     );
        p->_map_type                = _table->GetField      (i, "MapType"                   );
        p->_need_energy             = _table->GetField      (i, "NeedEnergy"                );
		p->_max_member				= _table->GetField		(i, "NeedGS");
		p->_need_power				= _table->GetField		(i, "D2NeedGs");
        p->_d_2_need_energy         = _table->GetField      (i, "D2NeedEnergy"              );
        p->_standard_time           = _table->GetField      (i, "StandardTime"              );
        p->_battle_ui_id            = _table->GetField      (i, "BattleUIID"                );
        p->_reward_money            = _table->GetField      (i, "RewardMoney"               );
        p->_reward_experience       = _table->GetField      (i, "RewardExperience"          );
        p->_reward_money_2          = _table->GetField      (i, "RewardMoney2"              );
		p->_father_map_id			= _table->GetField		(i, "FatherMapID");
        p->_reward_experience_2     = _table->GetField      (i, "RewardExperience2"         );
        p->_reward_tao_lue          = _table->GetField      (i, "RewardTaoLue"              );
        p->_monster_appeare_script  = _table->GetFieldString(i, "MonsterAppeareScript"      );
        p->_screen_buff             = _table->GetFieldString(i, "ScreenBuff"                );
        p->_success_condition       = _table->GetFieldString(i, "SuccessCondition"          );
        p->_fail_condition          = _table->GetFieldString(i, "FailCondition"             );
        p->_district_open_condition = _table->GetFieldString(i, "DistrictOpenCondition"     );
        p->_success_condition_2     = _table->GetFieldString(i, "SuccessCondition2"         );
        p->_success_condition_3     = _table->GetFieldString(i, "SuccessCondition3"         );

        p->_fail_need_energy_1      = _table->GetField      (i, "FailNeedEnergy"            );
        p->_fail_need_energy_2      = _table->GetField      (i, "FailD2NeedEnergy"          );

		p->_lv_limit				= _table->GetField		(i, "CopyLevelLimit");
		p->_map_monster_id			= _table->GetField		(i, "MapMosterID");
		p->_ex_map_id				= _table->GetField		(i, "ExMapID");
        p->_evaluation.push_back(_table->GetField(i, "Evaluation1"));
        p->_evaluation.push_back(_table->GetField(i, "Evaluation2"));
        p->_evaluation.push_back(_table->GetField(i, "Evaluation3"));
//         p->_evaluation.push_back(_table->GetField(i, "Evaluation4"));
//         p->_evaluation.push_back(_table->GetField(i, "Evaluation5"));

		p->_map_common_line			= _table->GetField		(i, "MapCommonLine");
        cpf f;
        f.set_cfg(&p->_reward_item);
        cp.parser<cpf>(_table->GetFieldString(i, "RewardItem").c_str(), ";", f);
        f.set_cfg(&p->_reward_item_2);
        cp.parser<cpf>(_table->GetFieldString(i, "RewardItem2").c_str(), ";", f);

        cpf_0 f0;
        f0.set_cfg(&p->_sweep_prop);
        cp.parser<cpf_0>(_table->GetFieldString(i, "SweepProp").c_str(), ";", f0);
        f0.set_cfg(&p->_sweep_prop_to);
        cp.parser<cpf_0>(_table->GetFieldString(i, "SweepPropTo").c_str(), ";", f0);

		std::string revive_1 = _table->GetFieldString(i, "DefensePoint");
		format_rect(revive_1, p->_revive_region_1);
		std::string revive_2 = _table->GetFieldString(i, "AttackPoint");
		format_rect(revive_2, p->_revive_region_2);
		std::string revive_3 = _table->GetFieldString(i, "NeutralPoint");
		format_rect(revive_3, p->_revive_region_3);
		std::string pos = _table->GetFieldString(i, "MonsterCoordinate");
		format_pos(pos, p->_pos);

		cpf_6 f6;
		f6.set_cfg(&(p->_exp_region));
		cp.parser<cpf_6>(_table->GetFieldString(i, "ExpArea").c_str(), "\\", f6);
        _cr.insert(std::pair<uint32, config_raid*>(p->_map_id, p));
    }

    return 0;
}

int config_raid_mgr::clear()
{
    std::map<uint32, config_raid*>::iterator i = _cr.begin();
    while (i != _cr.end()) {
        if (i->second != NULL) {
            delete i->second;
        }

        ++i;
    }

    _cr.clear();

    return 0;
}

const config_raid* config_raid_mgr::get_config_raid(uint32 raid_id) const
{
    std::map<uint32, config_raid*>::const_iterator i = _cr.find(raid_id);
    if (i == _cr.end()) {
        return NULL;
    }

    return i->second;
}

const rect &config_raid_mgr::get_revive_rect(uint32 raid_id, uint32 camp) const
{
	std::map<uint32, config_raid *>::const_iterator i = _cr.find(raid_id);
	if(i == _cr.end())
	{
		static rect r(0, 0, 0, 0);
		return r;
	}
	return i->second->get_revive_rect(camp);
}

const std::vector<exp_region>& config_raid_mgr::get_exp_region(uint32 raid_id) const 
{
	std::map<uint32, config_raid *>::const_iterator i = _cr.find(raid_id);
	if(i == _cr.end())
	{
		static std::vector<exp_region> v;
		return v;
	}
	return i->second->get_exp_region(); 
}

bool config_raid_mgr::is_normal(uint32 raid_id) const
{
	std::map<uint32, config_raid *>::const_iterator i = _cr.find(raid_id);
	if(i == _cr.end())
	{
		return false;
	}
	return i->second->is_normal();
}

bool config_raid_mgr::is_battle(uint32 raid_id) const
{
	std::map<uint32, config_raid *>::const_iterator i = _cr.find(raid_id);
	if(i == _cr.end())
	{
		return false;
	}
	return i->second->is_battle();
}

void config_raid_mgr::insert(config_raid* pcr)
{
    _cr[pcr->get_map_id()] = pcr;
}
