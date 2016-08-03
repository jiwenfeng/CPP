#include "cfg_parser.h"
#include "config_potential.h"

config_potential::config_potential(void)
{
}

config_potential::~config_potential(void)
{
}

config_potential_mgr::config_potential_mgr(TTableGroup_s* p)
        : _potential(p)
{
    init(p);
}


config_potential_mgr::~config_potential_mgr(void)
{
    clear();
}

int config_potential_mgr::init(TTableGroup_s* p)
{
    if (p == NULL) {
        return -1;
    }

    _potential = p;
    _table = _potential->GetTable("t_potential");
    if (_table == NULL) {
        printf("not found potential\n");
        return -2;
    }

    cfg_parser cp;
    for (int i = 1; i <= _table->m_recordCount; ++i) {
        if (_table->GetField      (i, "potentialID"         ) == 0) {
            continue;
        }

        config_potential* p = new config_potential;
		p->_potential_id                         = _table->GetField			(i, "potentialID"               );
		p->_oder_id                              = _table->GetField			(i, "orderID"                   );
		p->_pre_potential                        = _table->GetField			(i, "prePotential"              );
		p->_need_level							 = _table->GetField			(i, "NeedLevel"                 );
		p->_need_potential                       = _table->GetField			(i, "NeedYuanLi"                );
		p->_need_game_money						 = _table->GetField			(i, "NeedGameMoney"             );
		p->_need_probability                     = _table->GetField			(i, "NeedProbability"           );
		p->_open_server_relief                   = _table->GetField			(i, "openServerRelief"          );
		p->_need_count_min                       = _table->GetField			(i, "NeedCountMin"              );
		p->_need_count_max                       = _table->GetField			(i, "NeedCountMax"              );
		p->_number_name							 = _table->GetFieldString   (i, "numberName"				);
		p->_is_world_broadcast					 = _table->GetField			(i, "isWorldBroadcast"          );
		cpf_0 p0(&p->_game_attribute);// map
		cp.parser<cpf_0>(_table->GetFieldString(i, "GameAttribute").c_str(), ";", p0);

		cpf p1; // vector
		p1.set_cfg(&p->_reward_skill_list);
		cp.parser<cpf>(_table->GetFieldString(i, "RewardSkillList").c_str(), ";", p1);

		cpf p2; // vector
		p2.set_cfg(&p->_clear_skill_list);
		cp.parser<cpf>(_table->GetFieldString(i, "ClearSkillList").c_str(), ";", p2);

		cpf_0 p3(&p->_append_attribute);
		cp.parser<cpf_0>(_table->GetFieldString(i, "GameAttribute").c_str(), ";", p3);

		cpf_0 p4(&p->_append_need_item);
		cp.parser<cpf_0>(_table->GetFieldString(i, "appendNeedItem").c_str(), ";", p4);
		_cp.insert(std::pair<int32, config_potential*>(p->_potential_id,p));
    }

    return 0;
}

int config_potential_mgr::clear()
{
    for (std::map<uint32, config_potential*>::iterator i = _cp.begin(); i != _cp.end(); ++i) {
        if (i->second != NULL) {
            delete i->second;
        }
    }

    _cp.clear();

    return 0;
}

const config_potential* config_potential_mgr::get_config_potential(uint32 potential_id) const
{
    std::map<uint32, config_potential*>::const_iterator itr = _cp.find(potential_id);

	for (; itr != _cp.end(); ++itr)
	{
		return itr->second;
	}

	return NULL;
}
