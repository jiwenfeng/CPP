#include "config_mounts.h"

config_mounts_mgr::config_mounts_mgr()
{

}


config_mounts_mgr::~config_mounts_mgr(void)
{
    clear();
}

int config_mounts_mgr::init(TTableGroup_s* group_base, TTableGroup_s* group_attr)
{
    if (group_base == NULL || group_attr == NULL) {
        return -1;
    }

    TTable* table_base = group_base->GetTable("t_mount");
    TTable* table_attr = group_attr->GetTable("t_mount_attr");
    if (table_base == NULL || table_attr == NULL) {
        printf("not found t_mount or t_mount_attr\n");
        return -2;
    }

    cfg_parser cp;
    for (int i = 1; i <= table_base->m_recordCount; ++i) {
        if (table_base->GetField      (i, "MountID"         ) == 0) {
            continue;
        }

        config_mounts_base* base = new config_mounts_base;
        base->_mount_id                    = table_base->GetField(i, "MountID"    );
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
		base->_monster_id                  = table_base->GetField(i, "MonsterID");
		base->_advance_bless_time          = table_base->GetField(i, "AdvanceBlessTime");
		base->_use_item_record             = table_base->GetField(i, "use_item_record");
        base->_next_mount_id               = table_base->GetField(i, "NextMountID");
		base->_have_addition               = table_base->GetField(i, "haveAddition");
		base->_record_max_value            = table_base->GetField(i, "record_max_value");
		base->_potential_addtion           = table_base->GetField(i, "potentialAddtion");
		base->_mount_name				   = table_base->GetFieldString(i, "MountName");

		cpf f3;
		f3.set_cfg(&base->_talent_skill);
		cp.parser<cpf>(table_base->GetFieldString(i, "TalentSkill").c_str(), ";", f3);

		cpf f4;
		f4.set_cfg(&base->_upgrade_replace_item);
		cp.parser<cpf>(table_base->GetFieldString(i, "upgrade_replace_item").c_str(), ";", f4);

		cpf f5;
		f5.set_cfg(&base->_skill_id);
		cp.parser<cpf>(table_base->GetFieldString(i, "SkillID").c_str(), ";", f5);

		cpf f6;
		f6.set_cfg(&base->_advance_need_item);
		cp.parser<cpf>(table_base->GetFieldString(i, "AdvanceNeedItem").c_str(), ";", f6);

        std::map<int, config_mounts*>::iterator it = _content.find(base->_mount_id);
        if (it == _content.end()) {
            config_mounts* cfg = new config_mounts;
            cfg->set_base(base);
            _content.insert(std::pair<int, config_mounts*>(base->_mount_id, cfg));
        } else {
            it->second->set_base(base);
        }
    }

    for (int i = 1; i <= table_attr->m_recordCount; ++i) {
        if (table_attr->GetField(i, "MountID") == 0) {
            continue;
        }

        config_mounts_attr* attr = new config_mounts_attr;
        attr->_mount_id    = table_attr->GetField(i, "MountID"   );
        attr->_player_job  = table_attr->GetField(i, "PlayerJob" );
        attr->_mount_level = table_attr->GetField(i, "MountLevel");

        cpf_0 f0;
        f0.set_cfg(&attr->_game_attribute);
        cp.parser<cpf_0>(table_attr->GetFieldString(i, "GameAttribute").c_str(), ";", f0);

        std::map<int, config_mounts*>::iterator it = _content.find(attr->_mount_id);
        if (it == _content.end()) {
            config_mounts* cfg = new config_mounts;
            cfg->insert_attr(attr);
            _content.insert(std::pair<int, config_mounts*>(attr->_mount_id, cfg));
        } else {
            it->second->insert_attr(attr);
        }
    }

    return 0;
}

int config_mounts_mgr::init(TTableGroup_s* p)
{
    return 0;
}

int config_mounts_mgr::clear()
{
    for (std::map<int, config_mounts*>::iterator i = _content.begin(); i != _content.end(); ++i) {
        if (i->second != NULL) {
            delete i->second;
        }
    }

    _content.clear();

    return 0;
}

const config_mounts* config_mounts_mgr::get_config_mounts(uint32 mounts_id) const
{
    std::map<int, config_mounts*>::const_iterator it = _content.find(mounts_id);
    if (it == _content.end()) {
        return NULL;
    }
    
    return it->second;
}

const config_mounts* config_mounts_mgr::get_config_mounts_min() const
{
    if (_content.empty()) {
        return NULL;
    }
    
    return _content.begin()->second;
}

const config_mounts* config_mounts_mgr::get_config_mounts_max() const
{
	if (_content.empty()) {
		return NULL;
	}

	return _content.rbegin()->second;
}

float config_mounts_mgr::calculate_mount_power(uint32 mount_id,uint32 mount_level) const
{
    float default_value = 0.0;
    const config_mounts* p = get_config_mounts(mount_id);
    if (p == NULL) {
        return default_value;
    }

    const std::map<int, std::map<int, config_mounts_attr*> >& m = p->get_attr();
    const std::map<int, std::map<int, config_mounts_attr*> >::const_iterator m_job = m.find(0);
    if (m_job == m.end()) {
        return default_value;
    }

    const std::map<int, config_mounts_attr*>::const_iterator m_job_level = m_job->second.find(mount_level);
    if (m_job_level == m_job->second.end() || m_job_level->second == NULL) {
        return default_value;
    }

    const std::map<int, int>& game_attribute = m_job_level->second->get_game_attribute();

    float base_value[15] = {0.0f,0.2f,0.0f,5.0f,0.0f,7.5f,2.0f,2.0f,25.0f,1.0f,30.0f,0.0f,5.0f,0.0f,0.0f};
    float total_power = 0; 
    std::map<int, int>::const_iterator it_game = game_attribute.begin();
    for (; it_game != game_attribute.end(); ++it_game){
        total_power += base_value[it_game->first] * it_game->second;
    }

    return total_power;
}
