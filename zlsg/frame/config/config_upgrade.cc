#include "cfg_parser.h"
#include "config_upgrade.h"


config_upgrade::config_upgrade(void)
{
}


config_upgrade::~config_upgrade(void)
{

}

config_upgrade_mgr::config_upgrade_mgr(void)
{
}


config_upgrade_mgr::~config_upgrade_mgr(void)
{
    clear();
}

int config_upgrade_mgr::init(TTableGroup_s* p)
{
    if (p == NULL) {
        return -1;
    }

    _upgrade = p;
    _table = _upgrade->GetTable("upgrade");
    if (_table == NULL) {
        printf("not found upgrade\n");
        return -2;
    }

    cfg_parser cp;
    for (int i = 1; i <= _table->m_recordCount; ++i) {
        config_upgrade* p = new config_upgrade;
        p->_ujob                                = _table->GetField(i, "uJob"								);
        p->_ulevel								= _table->GetField(i, "uLevel"								);
        p->_rest_income_interval				= _table->GetField(i, "restIncomeInterval"					);
		p->_potential_exp_fail				    = _table->GetField(i, "potentialExpFail"					);
		p->_potential_exp_success				= _table->GetField(i, "potentialExpSuccess"					);
		p->_potential_exp_impact				= _table->GetField(i, "potentialExpImpact"					);
		p->_potential_daily_exp_impact_max		= _table->GetField(i, "potentialDailyExpImpactMax"          );
		p->_life_tree_becharge_exp		        = _table->GetField(i, "LifeTreeBeChargeExp"					);
		p->_life_tree_watering_exp				= _table->GetField(i, "LifeTreeWateringExp"					);
		p->_life_tree_irrigate_exp				= _table->GetField(i, "LifeTreeIrrigateExp"					);
		p->_life_tree_daily_exp_max				= _table->GetField(i, "LifeTreeDailyExpMax"					);
		p->_daily_item_exp_max					= _table->GetField(i, "DailyItemExpMax"						);
		p->_qa_reward_exp						= _table->GetField(i, "QARewardExp"							);
		p->_qa_reward_strategy					= _table->GetField(i, "QARewardYuanLi"						);
		p->_city_war_exp						= _table->GetField(i, "CityWarExp"							);
        p->_max_melt_value						= _table->GetField(i, "MaxMeltValue"						);

		p->_bz_obtain							= _table->GetField(i, "BZObtain"							);
		p->_buy_coin							= _table->GetField(i, "BuyCoin"								);
		p->_giant_factor						= _table->GetField(i, "GiantFactor"							);
#ifndef _WIN32
        p->_uneed_exp            = strtoull(_table->GetFieldString(i, "uNeedExp").c_str(), NULL, 10);
#else
        p->_uneed_exp            = strtoul(_table->GetFieldString(i, "uNeedExp").c_str(), NULL, 10);
#endif // _WIN32
        // printf("config_upgrade job = %d level = %d need_exp = %lu rii = %d\n", p->_ujob, p->_ulevel, p->_uneed_exp, p->_rest_income_interval);

		cpf f0;
		f0.set_cfg(&p->_rest_exp);
		cp.parser<cpf>(_table->GetFieldString(i, "restExp").c_str(), ";", f0);

		cpf f1;
		f1.set_cfg(&p->_strategy);
		cp.parser<cpf>(_table->GetFieldString(i, "restYuanLi").c_str(), ";", f1);

        cpf_0 p0(&p->_game_attribute);
        cp.parser<cpf_0>(_table->GetFieldString(i, "GameAttribute").c_str(), ";", p0);

        _cu[p->_ujob].insert(std::pair<uint32, config_upgrade*>(p->_ulevel, p));
    }

    return 0;
}

int config_upgrade_mgr::clear()
{
    for (std::map<uint32, std::map<uint32, config_upgrade*> >::iterator i = _cu.begin(); i != _cu.end(); ++i) {
        for (std::map<uint32, config_upgrade*>::iterator j = i->second.begin(); j != i->second.end(); ++j) {
            if (j->second != NULL) {
                delete j->second;
            }
        }

        i->second.clear();
    }

    _cu.clear();

    return 0;
}

const config_upgrade* config_upgrade_mgr::get_config_upgrade(uint32 job, uint32 lvl)
{
    std::map<uint32, std::map<uint32, config_upgrade*> >::iterator i = _cu.find(job);
    if (i == _cu.end()) {
        return NULL;
    }

    std::map<uint32, config_upgrade*>::iterator j = i->second.find(lvl);
    if (j == i->second.end()) {
        return NULL;
    }

    return j->second;
}

uint32 config_upgrade_mgr::get_config_city_war_exp(uint32 job, uint32 lvl)
{
	std::map<uint32, std::map<uint32, config_upgrade *> >::iterator i = _cu.find(job);
	if(i == _cu.end())
	{
		return 0;
	}
	std::map<uint32, config_upgrade *>::iterator j = i->second.find(lvl);
	if(j == i->second.end())
	{
		return 0;
	}
	return j->second->get_city_war_exp();
}
