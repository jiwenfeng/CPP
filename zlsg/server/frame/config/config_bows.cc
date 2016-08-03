#include "config_bows.h"

config_bows_mgr::config_bows_mgr()
{

}


config_bows_mgr::~config_bows_mgr(void)
{
    clear();
}

int config_bows_mgr::init(TTableGroup_s* group_base, TTableGroup_s* group_attr)
{
    if (group_base == NULL || group_attr == NULL) {
        return -1;
    }

    TTable* table_base = group_base->GetTable("t_bow");
    TTable* table_attr = group_attr->GetTable("t_bow_attr");
    if (table_base == NULL || table_attr == NULL) {
        printf("not found t_bow or t_bow_attr\n");
        return -2;
    }

    cfg_parser cp;
    for (int i = 1; i <= table_base->m_recordCount; ++i) {
        if (table_base->GetField      (i, "BowID"         ) == 0) {
            continue;
        }

        config_bows_base* base = new config_bows_base;
        base->_bow_id                      = table_base->GetField(i, "BowID"    );
        base->_advance                     = table_base->GetField(i, "Advance"    );
		base->_max_skill_level             = table_base->GetField(i, "MaxSkillLevel"    );
		//base->_advance_need_item           = table_base->GetField(i, "AdvanceNeedItem"    );
		base->_advance_need_item_num       = table_base->GetField(i, "AdvanceNeedItemNum"    );
		base->_advance_count_min           = table_base->GetField(i, "AdvanceCountMin");
		base->_advance_count_max           = table_base->GetField(i, "AdvanceCountMax");
		base->_advance_ratio               = table_base->GetField(i,"AdvanceRatio");
		base->_advance_fail_bless_min      = table_base->GetField(i,"AdvanceFailBlessMin");
		base->_advance_fail_belss_max      = table_base->GetField(i,"AdvanceFailBlessMax");
		base->_advance_bless_max           = table_base->GetField(i, "AdvanceBlessMax");
		base->_success_broadcast           = table_base->GetField(i, "SuccessBroadcast");
		//base->_monster_id                  = table_base->GetField(i, "MonsterID");
		base->_advance_bless_time          = table_base->GetField(i, "AdvanceBlessTime");
		base->_use_item_record             = table_base->GetField(i, "use_item_record");
        base->_next_bow_id               = table_base->GetField(i, "NextBowID");
		base->_have_addition               = table_base->GetField(i, "haveAddition");
		base->_record_max_value            = table_base->GetField(i, "record_max_value");
		base->_potential_addtion           = table_base->GetField(i, "potentialAddtion");
		base->_bow_name				   = table_base->GetFieldString(i, "BowName");

		//cpf f3;
		//f3.set_cfg(&base->_talent_skill);
		//cp.parser<cpf>(table_base->GetFieldString(i, "TalentSkill").c_str(), ";", f3);

		cpf f4;
		f4.set_cfg(&base->_upgrade_replace_item);
		cp.parser<cpf>(table_base->GetFieldString(i, "upgrade_replace_item").c_str(), ";", f4);

		//cpf f5;
		//f5.set_cfg(&base->_skill_id);
		//cp.parser<cpf>(table_base->GetFieldString(i, "SkillID").c_str(), ";", f5);

		cpf f6;
		f6.set_cfg(&base->_advance_need_item);
		cp.parser<cpf>(table_base->GetFieldString(i, "AdvanceNeedItem").c_str(), ";", f6);

        std::map<int, config_bows*>::iterator it = _content.find(base->_bow_id);
        if (it == _content.end()) {
            config_bows* cfg = new config_bows;
            cfg->set_base(base);
            _content.insert(std::pair<int, config_bows*>(base->_bow_id, cfg));
        } else {
            it->second->set_base(base);
        }
    }

    for (int i = 1; i <= table_attr->m_recordCount; ++i) {
        if (table_attr->GetField(i, "BowID") == 0) {
            continue;
        }

        config_bows_attr* attr = new config_bows_attr;
        attr->_bow_id    = table_attr->GetField(i, "BowID"   );
       // attr->_player_job  = table_attr->GetField(i, "BowLevel" );
        attr->_bow_level = table_attr->GetField(i, "BowLevel");
		attr->_bow_speed = table_attr->GetField(i, "Bspeed");

        cpf_0 f0;
        f0.set_cfg(&attr->_game_attribute);
        cp.parser<cpf_0>(table_attr->GetFieldString(i, "GameAttribute").c_str(), ";", f0);

        std::map<int, config_bows*>::iterator it = _content.find(attr->_bow_id);
        if (it == _content.end()) {
            config_bows* cfg = new config_bows;
            cfg->insert_attr(attr);
            _content.insert(std::pair<int, config_bows*>(attr->_bow_id, cfg));
        } else {
            it->second->insert_attr(attr);
        }
    }

    return 0;
}

int config_bows_mgr::init(TTableGroup_s* p)
{
    return 0;
}

int config_bows_mgr::clear()
{
    for (std::map<int, config_bows*>::iterator i = _content.begin(); i != _content.end(); ++i) {
        if (i->second != NULL) {
            delete i->second;
        }
    }

    _content.clear();

    return 0;
}

const config_bows* config_bows_mgr::get_config_bows(uint32 bow_id) const
{
    std::map<int, config_bows*>::const_iterator it = _content.find(bow_id);
    if (it == _content.end()) {
        return NULL;
    }
    
    return it->second;
}

const config_bows* config_bows_mgr::get_config_bows_min() const
{
    if (_content.empty()) {
        return NULL;
    }
    
    return _content.begin()->second;
}

const config_bows* config_bows_mgr::get_config_bows_max() const
{
	if (_content.empty()) {
		return NULL;
	}

	return _content.rbegin()->second;
}


float config_bows_mgr::calculate_bow_power(uint32 bow_id,uint32 bow_level) const
{
	float base_value[15] = {0.0f,0.2f,0.0f,5.0f,0.0f,7.5f,2.0f,2.0f,25.0f,1.0f,30.0f,0.0f,0.0f,0.0f,0.0f};
	float default_value = 0.0;
	const config_bows* p = get_config_bows(bow_id);
	if (p == NULL) {
		return default_value;
	}

	const std::multimap<int, config_bows_attr*>& m = p->get_attr();

	std::multimap<int, config_bows_attr*>::const_iterator it = m.find(bow_level);
	for (; it != m.end(); ++it){
		if (static_cast<uint32>(it->second->get_bow_id()) == bow_id && static_cast<uint32>(it->first) == bow_level){
			const std::map<int, int>& game_attribute = it->second->get_game_attribute();
			std::map<int, int>::const_iterator it_game = game_attribute.begin();
			for (; it_game != game_attribute.end(); ++it_game){
				default_value += base_value[it_game->first] * it_game->second;
			}
		}
	}

	return default_value;
}

const config_bows_attr* config_bows_mgr::get_config_bows_attr(uint32 bow_id, uint32 lvl) const
{
	std::map<int, config_bows*>::const_iterator it = _content.find(bow_id);
	if (it == _content.end()) {
		return NULL;
	}

	const std::multimap<int, config_bows_attr*> bow_attr_map_ = it->second->get_attr();
	std::multimap<int, config_bows_attr*>::const_iterator it_attr = bow_attr_map_.begin();
	for (; it_attr != bow_attr_map_.end();++it_attr){
		if (static_cast<uint32>(it_attr->second->get_bow_id()) == bow_id && static_cast<uint32>(it_attr->second->get_bow_level()) == lvl){
			return it_attr->second;
		}
	}

	return NULL;
}