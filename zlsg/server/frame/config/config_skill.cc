#include "cfg_parser.h"
#include "config_effect.h"
#include "config_skill.h"

#ifndef WIN32
#include <unistd.h>
#endif

config_skill::config_skill()
{
    _skill_id = 0;
}

config_skill::~config_skill()
{
}

uint32 config_skill::get_distance() const
{
    switch (get_method_shape()) {
    case MST_POINT       : return 1;
    case MST_LINE        : return get_line_length();
    case MST_CONE        : return get_arc_radius();
    case MST_CIRCLE      : return get_round_radius();
    case MST_TRIGEMINAL  : return get_arc_radius();
    case MST_POINT_AROUND: return get_line_length();
    case MST_MOVE_CIRCLE : return get_round_radius();
    default : break;
    }

    return 0;
}

void config_skill::get_next_skills(std::vector<const config_skill*>& skills) const
{
    for (size_t i = 0; i < _effects.size(); ++i) {
        skills.push_back(_effects[i]->get_next_trigger_skill());
    }
}

config_skill_mgr::config_skill_mgr(TTableGroup_s* p)
    : _skill(p)
{
    init(p, "t_skill");
    _lock = 0;
}

config_skill_mgr::~config_skill_mgr(void)
{
    clear();
}

int config_skill_mgr::init(TTableGroup_s* p, const char* table_name /* = "t_skill" */)
{
    if (p == NULL) {
        return -1;
    }

    _skill = p;
    _table = _skill->GetTable(table_name);
    if (_table == NULL) {
        printf("not found %s\n", table_name);
        return -2;
    }

    uint32 source = strcmp(table_name, "t_skill") == 0 ? 0 : 1;

    cfg_parser cp;
    for (int i = 1; i <= _table->m_recordCount; ++i) {
        uint32 id = _table->GetField(i, "SkillID");
        if (id == 0) {
            continue;
        }

        if (_cs.find(id) != _cs.end()) {
            printf("%s:%d config_skill repeat id = %d\n", __FILE__, __LINE__, id);
            continue;
        }

        config_skill* p = new config_skill;
        p->_skill_id                  = id;
		p->_skill_name				  = _table->GetFieldString(i, "SkillName"		);
        p->_skill_level               = _table->GetField(i, "SkillLevel"            );
        p->_skill_type                = _table->GetField(i, "SkillType"             );
        p->_skill_index               = _table->GetField(i, "SkillIndex"            );
        p->_trigger_probability       = _table->GetField(i, "TriggerProbability"    );
        p->_fight_power               = _table->GetField(i, "FightPower"            );
        //p->_learn_need_levels         = _table->GetField(i, "LearnNeedLevels"       );
        p->_learn_need_item_id        = _table->GetField(i, "LearnNeedItemID"       );
        p->_upgrade_need_levels       = _table->GetField(i, "UpgradeNeedLevels"     );
        p->_upgrade_need_yuan_li      = _table->GetField(i, "UpgradeNeedYuanLi"     );
        p->_upgrade_need_item_id      = _table->GetField(i, "UpgradeNeedItemID"     );
        p->_upgrade_need_item_num     = _table->GetField(i, "UpgradeNeedItemNum"    );
        p->_upgrade_need_game_money   = _table->GetField(i, "UpgradeNeedGameMoney"  );
        p->_upgrade_probability       = _table->GetField(i, "UpgradeProbability"    );
        p->_upgrade_skill_id          = _table->GetField(i, "UpgradeSkillID"        );
        p->_upgrade_need_timer        = _table->GetField(i, "UpgradeNeedTimer"      );
        p->_upgrade_manual_need_money = _table->GetField(i, "UpgradeManualNeedMoney");
        p->_use_limit                 = _table->GetField(i, "useLimit"              );
        p->_repair_block              = _table->GetField(i, "RepairBlock"           );
        p->_can_trigger_buff          = _table->GetField(i, "CanTriggerBuff"        );
        p->_consume_mp                = _table->GetField(i, "ConsumeMP"             );
        p->_collision_distance        = _table->GetField(i, "CollisionDistance"     );
        p->_object_max_level          = _table->GetField(i, "ObjectMaxLevel"        );
        p->_collision_speed_param     = _table->GetField(i, "CollisionSpeedParam"   );
        p->_cool_tick                 = _table->GetField(i, "CoolTick"              );
        p->_cool_group_id             = _table->GetField(i, "CoolGroupID"           );
        p->_cool_group_tick           = _table->GetField(i, "CoolGroupTick"         );
        p->_hate_value                = _table->GetField(i, "HateValue"             );
        p->_singing_tick              = _table->GetField(i, "SingingTick"           );
        p->_action_tick               = _table->GetField(i, "ActionTick"            );
        p->_is_harm                   = _table->GetField(i, "isHarm"                );
        p->_is_death_repel            = _table->GetField(i, "isDeathRepel"          );
        p->_correction                = _table->GetField(i, "Correction"            );
        p->_affect_object_max         = _table->GetField(i, "AffectObjectMax"       );
        p->_method_shape              = _table->GetField(i, "MethodShape"           );
        p->_line_length               = _table->GetField(i, "LineLength"            );
        p->_line_width                = _table->GetField(i, "LineWidth"             );
        p->_round_radius              = _table->GetField(i, "RoundRadius"           );
        p->_arc_angle                 = _table->GetField(i, "ArcAngle"              );
        p->_arc_radius                = _table->GetField(i, "ArcRadius"             );
        p->_affect_gain_proportion    = _table->GetField(i, "AffectGainProportion"  );
        p->_is_ignore_defense         = _table->GetField(i, "isIgnoreDefense"       );
        p->_miss_blow                 = _table->GetField(i, "MissBlow"              );
        p->_hit_rate                  = _table->GetField(i, "HitRate"               );
		p->_skills_belonging          = _table->GetField(i, "SkillsBelonging"       );
        p->_attack_time               = _table->GetField(i, "AttackTime"            );
		p->_key						  = _table->GetField(i, "Key"                   );
        cpf f;
        f.set_cfg(&p->_need_job);
        cp.parser<cpf>(_table->GetFieldString(i, "NeedJob").c_str(), ";", f);
        f.set_cfg(&p->_effect_object_type);
        cp.parser<cpf>(_table->GetFieldString(i, "EffectObjectType").c_str(), ";", f);
        f.set_cfg(&p->_method_list);
        cp.parser<cpf>(_table->GetFieldString(i, "MethodList").c_str(), ";", f);
        f.set_cfg(&p->_self_method_list); 
        cp.parser<cpf>(_table->GetFieldString(i, "SelfMethodList").c_str(), ";", f);

        cpf_0 f0;
        f0.set_cfg(&p->_game_attribute);
        cp.parser<cpf_0>(_table->GetFieldString(i, "GameAttribute").c_str(), ";", f0);

		cpf_4 f4;
		f4.set_cfg(&p->_learn_need_levels);
		cp.parser<cpf_4>(_table->GetFieldString(i, "LearnNeedLevels").c_str(), ";", f4);

        // printf("%s:%d, skill %d\n", __FILE__, __LINE__, p->_skill_id);
        _cs.insert(std::pair<uint32, config_skill*>(p->_skill_id, p));

        p->_source = source;
        if (source == 0) {
            continue;
        }

        for (size_t j = 0; j < p->_need_job.size(); ++j) {
            _mcs.insert(std::pair<uint32, config_skill*>(p->_need_job[j], p));
        }
    }

    //std::vector<const config_skill*> vcs;
    //get_config_skill_by_job(1, 1, vcs);
    //if (vcs.size() != 0){
    //    for (size_t i = 0; i < vcs.size(); ++i) {
    //        printf("%s:%d, skill %d\n", __FILE__, __LINE__, vcs[i]->get_skill_id());
    //    }
    //} else {
    //    printf("%s:%d, not skill\n", __FILE__, __LINE__);
    //}

	//std::map<uint32, config_skill*>::iterator itr = _cs.begin();
	//for (;itr != _cs.end(); ++itr){
	//	const std::map<std::string,int>& need_level_limit_map_ = itr->second->get_learn_need_levels();
	//	std::map<std::string,int>::const_iterator it = need_level_limit_map_.begin();
	//	for (;it != need_level_limit_map_.end();++it){
	//		printf(" it->frist = %s , limit_value = %d \n",it->first.c_str(),it->second);
	//	}
	//}

    return 0;
}

int config_skill_mgr::init(TTableGroup_s* p)
{
    return 0;
}

int config_skill_mgr::clear()
{
    for (std::map<uint32, config_skill*>::iterator i = _cs.begin(); i != _cs.end(); ++i) {
        if (i->second != NULL) {
            delete i->second;
        }
    }

    _cs.clear();

    return 0;
}

const config_skill* config_skill_mgr::get_config_skill(uint32 id)
{
    if (id == 0) {
        return NULL;
    }

    std::map<uint32, config_skill*>::const_iterator i = _cs.find(id);
    if (i == _cs.end()) {
        return quick_read_by_id(id);
    }

    return i->second;
}

void config_skill_mgr::get_config_skill_by_job(uint32 job, uint32 level, std::vector<const config_skill*>& cs)
{
    std::multimap<uint32, config_skill*>::const_iterator i = _mcs.find(job);
    for (; i != _mcs.end(); ++i) {
        //printf("%s:%d, id = %d lvl = %d\n", __FILE__, __LINE__, i->second->_skill_id, i->second->_learn_need_levels);
        if (i->first != job) {
            break;
        }

        const std::map<std::string, int>& learn_level_map_ = i->second->get_learn_need_levels();
        std::map<std::string, int>::const_iterator itr = learn_level_map_.begin();
        uint32 need_player_level = 0;
        for (;itr != learn_level_map_.end(); ++itr) {
            need_player_level = static_cast<uint32>(itr->second);
        }

		if (need_player_level == 0) {
			continue;
		}

		if (learn_level_map_.size() != 0) {
			if (need_player_level != level) {
				continue;
			}
		}

		cs.push_back(i->second);
    }

    if (cs.empty()) {
        quick_read_by_job(job, level, cs);
    }
}

int config_skill_mgr::link_effects(config_effect_mgr* pcem)
{
    std::map<uint32, config_skill*>::iterator i = _cs.begin();
    while (i != _cs.end()) {
        const std::vector<int>& vml = i->second->get_method_list();
        for (size_t j = 0; j < vml.size(); ++j) {
            const config_effect* ce = pcem->get_config_effect(vml[j]);
            if (ce == NULL) {
                printf("%s:%d, skill id = %d link effect id = %d not found\n", __FILE__, __LINE__, i->first, vml[j]);
                continue ;
            }

            i->second->_effects.push_back(ce);
        }

        ++i;
    }
    
    return 0;
}

const config_skill* config_skill_mgr::load_skill(TTable* table, uint32 skill_id, uint32 source)
{
    cfg_parser cp;
    config_skill* p = NULL;
    for (int i = 1; i <= table->m_recordCount; ++i) {
        uint32 id = table->GetField(i, "SkillID");
        if (id == 0 || id != skill_id) {
            continue;
        }

        std::map<uint32, config_skill*>::iterator it = _cs.find(id);
        if (it != _cs.end()) {
            p = it->second;
            break;
        }

        p = new config_skill;
        p->_skill_id                  = id;
        p->_skill_level               = table->GetField(i, "SkillLevel"            );
        p->_skill_type                = table->GetField(i, "SkillType"             );
        p->_skill_index               = table->GetField(i, "SkillIndex"            );
        p->_trigger_probability       = table->GetField(i, "TriggerProbability"    );
        p->_fight_power               = table->GetField(i, "FightPower"            );
        //p->_learn_need_levels         = table->GetField(i, "LearnNeedLevels"       );
        p->_learn_need_item_id        = table->GetField(i, "LearnNeedItemID"       );
        p->_upgrade_need_levels       = table->GetField(i, "UpgradeNeedLevels"     );
        p->_upgrade_need_yuan_li      = table->GetField(i, "UpgradeNeedYuanLi"     );
        p->_upgrade_need_item_id      = table->GetField(i, "UpgradeNeedItemID"     );
        p->_upgrade_need_item_num     = table->GetField(i, "UpgradeNeedItemNum"    );
        p->_upgrade_need_game_money   = table->GetField(i, "UpgradeNeedGameMoney"  );
        p->_upgrade_probability       = table->GetField(i, "UpgradeProbability"    );
        p->_upgrade_skill_id          = table->GetField(i, "UpgradeSkillID"        );
        p->_upgrade_need_timer        = table->GetField(i, "UpgradeNeedTimer"      );
        p->_upgrade_manual_need_money = table->GetField(i, "UpgradeManualNeedMoney");
        p->_use_limit                 = table->GetField(i, "useLimit"              );
        p->_repair_block              = table->GetField(i, "RepairBlock"           );
        p->_can_trigger_buff          = table->GetField(i, "CanTriggerBuff"        );
        p->_consume_mp                = table->GetField(i, "ConsumeMP"             );
        p->_collision_distance        = table->GetField(i, "CollisionDistance"     );
        p->_object_max_level          = table->GetField(i, "ObjectMaxLevel"        );
        p->_collision_speed_param     = table->GetField(i, "CollisionSpeedParam"   );
        p->_cool_tick                 = table->GetField(i, "CoolTick"              );
        p->_cool_group_id             = table->GetField(i, "CoolGroupID"           );
        p->_cool_group_tick           = table->GetField(i, "CoolGroupTick"         );
        p->_hate_value                = table->GetField(i, "HateValue"             );
        p->_singing_tick              = table->GetField(i, "SingingTick"           );
        p->_action_tick               = table->GetField(i, "ActionTick"            );
        p->_is_harm                   = table->GetField(i, "isHarm"                );
        p->_is_death_repel            = table->GetField(i, "isDeathRepel"          );
        p->_correction                = table->GetField(i, "Correction"            );
        p->_affect_object_max         = table->GetField(i, "AffectObjectMax"       );
        p->_method_shape              = table->GetField(i, "MethodShape"           );
        p->_line_length               = table->GetField(i, "LineLength"            );
        p->_line_width                = table->GetField(i, "LineWidth"             );
        p->_round_radius              = table->GetField(i, "RoundRadius"           );
        p->_arc_angle                 = table->GetField(i, "ArcAngle"              );
        p->_arc_radius                = table->GetField(i, "ArcRadius"             );
        p->_affect_gain_proportion    = table->GetField(i, "AffectGainProportion"  );
        p->_is_ignore_defense         = table->GetField(i, "isIgnoreDefense"       );
        p->_miss_blow                 = table->GetField(i, "MissBlow"              );
        p->_hit_rate                  = table->GetField(i, "HitRate"               );
        p->_skills_belonging          = table->GetField(i, "SkillsBelonging"       );
        p->_attack_time               = table->GetField(i, "AttackTime"            );
        p->_key						  = table->GetField(i, "Key"                   );

        cpf f;
        f.set_cfg(&p->_need_job);
        cp.parser<cpf>(table->GetFieldString(i, "NeedJob").c_str(), ";", f);
        f.set_cfg(&p->_effect_object_type);
        cp.parser<cpf>(table->GetFieldString(i, "EffectObjectType").c_str(), ";", f);
        f.set_cfg(&p->_method_list);
        cp.parser<cpf>(table->GetFieldString(i, "MethodList").c_str(), ";", f);
        f.set_cfg(&p->_self_method_list); 
        cp.parser<cpf>(table->GetFieldString(i, "SelfMethodList").c_str(), ";", f);

        cpf_0 f0;
        f0.set_cfg(&p->_game_attribute);
        cp.parser<cpf_0>(table->GetFieldString(i, "GameAttribute").c_str(), ";", f0);

        cpf_4 f4;
        f4.set_cfg(&p->_learn_need_levels);
        cp.parser<cpf_4>(table->GetFieldString(i, "LearnNeedLevels").c_str(), ";", f4);

        // printf("%s:%d, skill %d\n", __FILE__, __LINE__, p->_skill_id);
        _cs.insert(std::pair<uint32, config_skill*>(p->_skill_id, p));

        p->_source = source;
        if (source == 0) {
            break;
        }

        for (size_t j = 0; j < p->_need_job.size(); ++j) {
            _mcs.insert(std::pair<uint32, config_skill*>(p->_need_job[j], p));
        }

        break;
    }

    return p;
}

const config_skill* config_skill_mgr::read_skill(uint32 skill_id)
{
    if (skill_id == 0) {
        return NULL;
    }

    const config_skill* p = NULL;
    _table = _skill->GetTable("t_skill");
    if (_table == NULL) {
        printf("not found t_skill\n");
    } else {
        const config_skill* p = load_skill(_table, skill_id, 0);
        if (p != NULL) {
            return p;
        }
    }

    _table = _skill_role->GetTable("t_skill_role");
    if (_table == NULL) {
        printf("not found t_skill_role\n");
    } else {
        p = load_skill(_table, skill_id, 1);
    }

    return p;
}

void config_skill_mgr::set_table_group(TTableGroup_s* skill, TTableGroup_s* skill_role)
{
    if (skill == NULL || skill_role == NULL) {
        printf("%s:%d table group error skill or skill_role \n", __FILE__, __LINE__);
    }
    _skill      = skill;
    _skill_role = skill_role;
}

void config_skill_mgr::quick_init(TTableGroup_s* skill, TTableGroup_s* skill_role)
{
    set_table_group(skill, skill_role);

    if (skill != NULL) {
        _table = skill->GetTable("t_skill");
        if (_table == NULL) {
            printf("%s:%d not found t_skill\n", __FILE__, __LINE__);
            return ;
        }

        quick_load(_table, 0);
    }

    if (skill_role != NULL) {
        _table = skill_role->GetTable("t_skill_role");
        if (_table == NULL) {
            printf("%s:%d not found t_skill_role\n", __FILE__, __LINE__);
            return ;
        }

        quick_load(_table, 1);
    }
}

void config_skill_mgr::quick_load(TTable* table, uint32 source)
{
    cfg_parser cp;
    for (int i = 1; i <= _table->m_recordCount; ++i) {
        uint32 effect_id  = _table->GetField      (i, "SkillID");

        table_index ti;
        ti._index = i;
        ti._source = source;

        _quick_by_id.insert(std::pair<uint32, table_index>(effect_id, ti));

        if (source == 1) {
            cpf f;
            std::vector<int> need_job;
            f.set_cfg(&need_job);
            cp.parser<cpf>(table->GetFieldString(i, "NeedJob").c_str(), ";", f);
            for (size_t size = 0; size < need_job.size(); ++size) {
                _quick_by_job.insert(std::pair<uint32, table_index>(need_job[size], ti));
            }
        }
    }
}

const config_skill* config_skill_mgr::quick_read_by_id(uint32 id)
{
    int ret = 1;
    do {
        ret = __sync_lock_test_and_set(&_lock, 1);
        if (ret != 0) {
            usleep(1);
        }
    } while (ret != 0);


    std::map<uint32, table_index>::iterator i = _quick_by_id.find(id);
    if (i == _quick_by_id.end()) {
        printf("%s:%d not found config_skill id:%d\n", __FILE__, __LINE__, id);
        __sync_lock_release(&_lock);

        return NULL;
    }

    if (i->second._source == 0) {
        _table = _skill->GetTable("t_skill");
        if (_table == NULL) {
            printf("%s:%d not found t_skill\n", __FILE__, __LINE__);
            __sync_lock_release(&_lock);

            return NULL;
        }

        __sync_lock_release(&_lock);
        return quick_read(_table, i->second._index, 0, 0);
    }

    if (i->second._source == 1) {
        _table = _skill_role->GetTable("t_skill_role");
        if (_table == NULL) {
            printf("%s:%d not found t_skill_role\n", __FILE__, __LINE__);
            __sync_lock_release(&_lock);
            return NULL;
        }

        __sync_lock_release(&_lock);
        return quick_read(_table, i->second._index, 1, 0);
    }

    __sync_lock_release(&_lock);
    return NULL;
}

void config_skill_mgr::quick_read_by_job(uint32 job, uint32 level, std::vector<const config_skill*>& cs)
{
    int ret = 1;
    do {
        ret = __sync_lock_test_and_set(&_lock, 1);
        if (ret != 0) {
            usleep(1);
        }
    } while (ret != 0);

    std::multimap<uint32, table_index>::iterator i = _quick_by_job.find(job);
    if (i == _quick_by_job.end()) {
        printf("%s:%d not found config_skill job:%d\n", __FILE__, __LINE__, job);
        __sync_lock_release(&_lock);

        return ;
    }

    while (i != _quick_by_job.end() && i->first == job) {
        if (i->second._source == 0) {
            _table = _skill->GetTable("t_skill");
            if (_table == NULL) {
                printf("%s:%d not found t_skill\n", __FILE__, __LINE__);
                continue;
            }

            quick_read(_table, i->second._index, 0, 0);
        }

        if (i->second._source == 1) {
            _table = _skill_role->GetTable("t_skill_role");
            if (_table == NULL) {
                printf("%s:%d not found t_skill_role\n", __FILE__, __LINE__);
                continue;
            }

            quick_read(_table, i->second._index, 1, 1);
        }

        ++i;
    }

    std::multimap<uint32, config_skill*>::const_iterator it = _mcs.find(job);
    for (; it != _mcs.end(); ++it) {
        if (it->first != job) {
            break;
        }

        const std::map<std::string, int>& learn_level_map_ = it->second->get_learn_need_levels();
        std::map<std::string, int>::const_iterator itr = learn_level_map_.begin();
        uint32 need_player_level = 0;
        for (; itr != learn_level_map_.end(); ++itr) {
            need_player_level = static_cast<uint32>(itr->second);
        }

        if (need_player_level == 0) {
            continue;
        }

        if (learn_level_map_.size() != 0) {
            if (need_player_level > level) {
                continue;
            }
        }

        cs.push_back(it->second);
    }

    __sync_lock_release(&_lock);
}

const config_skill* config_skill_mgr::quick_read(TTable* table, uint32 index, uint32 source, uint32 container)
{
    cfg_parser cp;
    config_skill* p = NULL;
    for (int i = index; i <= table->m_recordCount; ++i) {
        uint32 id = table->GetField(i, "SkillID");
        if (id == 0) {
            continue;
        }

        std::map<uint32, config_skill*>::iterator it = _cs.find(id);
        if (it != _cs.end()) {
            p = it->second;
            if (container == 1) {
                for (size_t j = 0; j < p->_need_job.size(); ++j) {
                    _mcs.insert(std::pair<uint32, config_skill*>(p->_need_job[j], p));
                }
            }

            break;
        }

        p = new config_skill;
        p->_skill_id                  = id;
        p->_skill_level               = table->GetField(i, "SkillLevel"            );
        p->_skill_type                = table->GetField(i, "SkillType"             );
        p->_skill_index               = table->GetField(i, "SkillIndex"            );
        p->_trigger_probability       = table->GetField(i, "TriggerProbability"    );
        p->_fight_power               = table->GetField(i, "FightPower"            );
        //p->_learn_need_levels         = table->GetField(i, "LearnNeedLevels"       );
        p->_learn_need_item_id        = table->GetField(i, "LearnNeedItemID"       );
        p->_upgrade_need_levels       = table->GetField(i, "UpgradeNeedLevels"     );
        p->_upgrade_need_yuan_li      = table->GetField(i, "UpgradeNeedYuanLi"     );
        p->_upgrade_need_item_id      = table->GetField(i, "UpgradeNeedItemID"     );
        p->_upgrade_need_item_num     = table->GetField(i, "UpgradeNeedItemNum"    );
        p->_upgrade_need_game_money   = table->GetField(i, "UpgradeNeedGameMoney"  );
        p->_upgrade_probability       = table->GetField(i, "UpgradeProbability"    );
        p->_upgrade_skill_id          = table->GetField(i, "UpgradeSkillID"        );
        p->_upgrade_need_timer        = table->GetField(i, "UpgradeNeedTimer"      );
        p->_upgrade_manual_need_money = table->GetField(i, "UpgradeManualNeedMoney");
        p->_use_limit                 = table->GetField(i, "useLimit"              );
        p->_repair_block              = table->GetField(i, "RepairBlock"           );
        p->_can_trigger_buff          = table->GetField(i, "CanTriggerBuff"        );
        p->_consume_mp                = table->GetField(i, "ConsumeMP"             );
        p->_collision_distance        = table->GetField(i, "CollisionDistance"     );
        p->_object_max_level          = table->GetField(i, "ObjectMaxLevel"        );
        p->_collision_speed_param     = table->GetField(i, "CollisionSpeedParam"   );
        p->_cool_tick                 = table->GetField(i, "CoolTick"              );
        p->_cool_group_id             = table->GetField(i, "CoolGroupID"           );
        p->_cool_group_tick           = table->GetField(i, "CoolGroupTick"         );
        p->_hate_value                = table->GetField(i, "HateValue"             );
        p->_singing_tick              = table->GetField(i, "SingingTick"           );
        p->_action_tick               = table->GetField(i, "ActionTick"            );
        p->_is_harm                   = table->GetField(i, "isHarm"                );
        p->_is_death_repel            = table->GetField(i, "isDeathRepel"          );
        p->_correction                = table->GetField(i, "Correction"            );
        p->_affect_object_max         = table->GetField(i, "AffectObjectMax"       );
        p->_method_shape              = table->GetField(i, "MethodShape"           );
        p->_line_length               = table->GetField(i, "LineLength"            );
        p->_line_width                = table->GetField(i, "LineWidth"             );
        p->_round_radius              = table->GetField(i, "RoundRadius"           );
        p->_arc_angle                 = table->GetField(i, "ArcAngle"              );
        p->_arc_radius                = table->GetField(i, "ArcRadius"             );
        p->_affect_gain_proportion    = table->GetField(i, "AffectGainProportion"  );
        p->_is_ignore_defense         = table->GetField(i, "isIgnoreDefense"       );
        p->_miss_blow                 = table->GetField(i, "MissBlow"              );
        p->_hit_rate                  = table->GetField(i, "HitRate"               );
        p->_skills_belonging          = table->GetField(i, "SkillsBelonging"       );
        p->_attack_time               = table->GetField(i, "AttackTime"            );
        p->_key						  = table->GetField(i, "Key"                   );

        cpf f;
        f.set_cfg(&p->_need_job);
        cp.parser<cpf>(table->GetFieldString(i, "NeedJob").c_str(), ";", f);
        f.set_cfg(&p->_effect_object_type);
        cp.parser<cpf>(table->GetFieldString(i, "EffectObjectType").c_str(), ";", f);
        f.set_cfg(&p->_method_list);
        cp.parser<cpf>(table->GetFieldString(i, "MethodList").c_str(), ";", f);
        f.set_cfg(&p->_self_method_list); 
        cp.parser<cpf>(table->GetFieldString(i, "SelfMethodList").c_str(), ";", f);

        cpf_0 f0;
        f0.set_cfg(&p->_game_attribute);
        cp.parser<cpf_0>(table->GetFieldString(i, "GameAttribute").c_str(), ";", f0);

        cpf_4 f4;
        f4.set_cfg(&p->_learn_need_levels);
        cp.parser<cpf_4>(table->GetFieldString(i, "LearnNeedLevels").c_str(), ";", f4);

        p->_source = source;

        // printf("%s:%d, skill %d\n", __FILE__, __LINE__, p->_skill_id);
        _cs.insert(std::pair<uint32, config_skill*>(p->_skill_id, p));

        if (container == 1) {
            if (source == 0) {
                break;
            }

            for (size_t j = 0; j < p->_need_job.size(); ++j) {
                _mcs.insert(std::pair<uint32, config_skill*>(p->_need_job[j], p));
            } 
        }

        break;
    }

    return p;
}
