#include "cfg_parser.h"
#include "config_mounts_skill.h"

config_mounts_skill::config_mounts_skill(void)
{
}

config_mounts_skill::~config_mounts_skill(void)
{
}

config_mounts_skill_mgr::config_mounts_skill_mgr(TTableGroup_s* p)
        : _mounts_skill(p)
{
    init(p);
}


config_mounts_skill_mgr::~config_mounts_skill_mgr(void)
{
    clear();
}

int config_mounts_skill_mgr::init(TTableGroup_s* p)
{
    if (p == NULL) {
        return -1;
    }

    _mounts_skill = p;
    _table = _mounts_skill->GetTable("t_mount_skill");
    if (_table == NULL) {
        printf("not found mounts_skill\n");
        return -2;
    }

    cfg_parser cp;
    for (int i = 1; i <= _table->m_recordCount; ++i) {
        if (_table->GetField      (i, "SkillID"         ) == 0) {
            continue;
        }

        config_mounts_skill* p = new config_mounts_skill;
        p->_skill_id                         = _table->GetField      (i, "SkillID"               );
		p->_skill_index                      = _table->GetField      (i, "SkillIndex"            );
		p->_skill_level                      = _table->GetField      (i, "SkillLevel"            );
		p->_show_need_advance                = _table->GetField      (i, "ShowNeedAdvance"       );
		p->_activation_need_advance          = _table->GetField      (i, "ActivationNeedAdvance" );
		p->_select_probability_1             = _table->GetField      (i, "SelectProbability1"    );
		p->_select_probability_2             = _table->GetField      (i, "SelectProbability2"    );
		p->_select_probability_3             = _table->GetField      (i, "SelectProbability3"    );
		p->_upgrade_need_exp                 = _table->GetField      (i, "UpgradeNeedExp"        );
		p->_addtion_exp_1                    = _table->GetField      (i, "AdditionExp1"          );
		p->_addtion_exp_2                    = _table->GetField      (i, "AdditionExp2"          );
		p->_addtion_exp_3                    = _table->GetField      (i, "AdditionExp3"          );
		p->_manual_active                    = _table->GetField      (i, "ManualActive"          );

		cpf_0 f0;// map
		f0.set_cfg(&p->_consume_money_game);
		cp.parser<cpf_0>(_table->GetFieldString(i, "ConsumeMoneyGame").c_str(), ";", f0);
		cpf_0 f1;// map
		f1.set_cfg(&p->_consume_money_cash);
		cp.parser<cpf_0>(_table->GetFieldString(i, "ConsumeMoneyCash").c_str(), ";", f1);
		cpf_0 f2;// map
		f2.set_cfg(&p->_consume_repute);
		cp.parser<cpf_0>(_table->GetFieldString(i, "ConsumeRepute").c_str(), ";", f2);

		_cp.insert(std::pair<int32, config_mounts_skill*>(p->_skill_id,p));
		_money_pay.insert(std::pair<int32,config_mounts_skill*>(p->_activation_need_advance,p));
// 		printf("ActivationNeedAdvance = %d ", p->_activation_need_advance);
    }

    return 0;
}

int config_mounts_skill_mgr::clear()
{
    for (std::map<uint32, config_mounts_skill*>::iterator i = _cp.begin(); i != _cp.end(); ++i) {
        if (i->second != NULL) {
            delete i->second;
        }
    }

    _cp.clear();

    return 0;
}

const config_mounts_skill* config_mounts_skill_mgr::get_config_mounts_skill(uint32 skill_id) const
{
    std::map<uint32, config_mounts_skill*>::const_iterator itr = _cp.find(skill_id);

	for (; itr != _cp.end(); ++itr)
	{
		return itr->second;
	}

	return NULL;
}

const config_mounts_skill* config_mounts_skill_mgr::get_config_mounts_money_pay(uint32 activation_need_advance) const
{
	std::map<uint32, config_mounts_skill*>::const_iterator itr = _money_pay.find(activation_need_advance);

	for (; itr != _money_pay.end(); ++itr)
	{
		return itr->second;
	}

	return NULL;
}