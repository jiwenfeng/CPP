#include "config_mgr.h"
#include "config_effect.h"


#ifndef WIN32
#include <unistd.h>
#endif


config_effect::config_effect(void)
{
    _fl                 = 0;
    _next_trigger_skill = NULL;
}

config_effect::~config_effect(void)
{
}

config_effect_mgr::config_effect_mgr(TTableGroup_s* p)
    : _effect(p)
{
    init(p, "t_effect");
    _lock = 0;
}


config_effect_mgr::~config_effect_mgr(void)
{
    clear();
}

int config_effect_mgr::init(TTableGroup_s* p, const char* table_name /* = "t_effect" */)
{
    if (p == NULL) {
        return -1;
    }

    _effect = p;
    _table = _effect->GetTable(table_name);
    if (_table == NULL) {
        printf("not found %s\n", table_name);
        return -2;
    }

    cfg_parser cp;
    for (int i = 1; i <= _table->m_recordCount; ++i) {
        uint32 effect_id  = _table->GetField      (i, "MethodID"                 );
        if (effect_id == 0) {
            continue;
        }

        if (_cb.find(effect_id) != _cb.end()) {
            printf("%s:%d config_effect repeat id = %d\n", __FILE__, __LINE__, effect_id);
            continue;
        }

        config_effect* p = new config_effect;
        p->_key                           = _table->GetField      (i, "Key"                      );
        p->_effect_id                     = _table->GetField      (i, "MethodID"                 );
        p->_method_id                     = _table->GetField      (i, "MethodID"                 );
        p->_method_name                   = _table->GetFieldString(i, "MethodName"               );
        p->_levels                        = _table->GetField      (i, "Levels"                   );

        p->_is_buff                       = _table->GetField      (i, "isBuff"                   );
        p->_is_scene_buff                 = _table->GetField      (i, "isSceneBuff"              );
        p->_buff_probability              = _table->GetField      (i, "BuffProbability"          );
        p->_buff_useful_type              = _table->GetField      (i, "BuffUsefulType"           );

        p->_next_trigger_skill_id         = _table->GetField      (i, "NextTriggerSkillID"       );
        p->_next_trigger_count            = _table->GetField      (i, "NextTriggerCount"         );
        p->_next_triggert_probability     = _table->GetField      (i, "NextTriggertProbability"  );
        p->_duration_tick                 = _table->GetField      (i, "DurationTick"             );
        p->_interval_tick                 = _table->GetField      (i, "IntervalTick"             );
        p->_repeat_type                   = _table->GetField      (i, "RepeatType"               );
        p->_repeat_max                    = _table->GetField      (i, "RepeatMax"                );
        p->_repeat_levels                 = _table->GetField      (i, "RepeatLevels"             );

        p->_durable_max                   = _table->GetField      (i, "DurableMax"               );

        p->_over_back_proportion          = _table->GetField      (i, "OverBackProportion"       );
        p->_over_back_max                 = _table->GetField      (i, "OverBackMax"              );
        p->_monster_hide                  = _table->GetField      (i, "MonsterHide"              );
        p->_player_hide                   = _table->GetField      (i, "PlayerHide"               );

        p->_affect_param                  = _table->GetField      (i, "AffectParam"              );
        p->_affect_proportion             = _table->GetField      (i, "AffectProportion"         );

        p->_be_cleared                    = _table->GetField      (i, "BeCleared"                );
        p->_offline_death_remove          = _table->GetField      (i, "OfflineDeathRemove"       );
        p->_offline_timer                 = _table->GetField      (i, "OfflineTimer"             );
        p->_owner_offline_death_remove    = _table->GetField      (i, "OwnerOfflineDeathRemove"  );
        p->_share_troop                   = _table->GetField      (i, "ShareTroop"               );
        p->_harm_check                    = _table->GetField      (i, "HarmCheck"                );
        p->_is_clear_buff                 = _table->GetField      (i, "isClearBuff"              );
        p->_clear_buff_probability        = _table->GetField      (i, "ClearBuffProbability"     );
        p->_clear_useful_buff_level       = _table->GetField      (i, "ClearUsefulBuffLevel"     );
        p->_clear_harmful_buff_level      = _table->GetField      (i, "ClearHarmfulBuffLevel"    );

        p->_clear_useful_anti_buff_level  = _table->GetField      (i, "ClearUsefulAntiBuffLevel" );
        p->_clear_harmful_anti_buff_level = _table->GetField      (i, "ClearHarmfulAntiBuffLevel");
        p->_is_rebirth                    = _table->GetField      (i, "isRebirth"                );
        p->_rebirth_probability           = _table->GetField      (i, "RebirthProbability"       );
        p->_rebirth_hp                    = _table->GetField      (i, "RebirthHP"                );
        p->_rebirth_mp                    = _table->GetField      (i, "RebirthMP"                );
        p->_is_move                       = _table->GetField      (i, "isMove"                   );
        p->_move_probability              = _table->GetField      (i, "MoveProbability"          );
        p->_move_type                     = _table->GetField      (i, "MoveType"                 );
        p->_move_range                    = _table->GetField      (i, "MoveRange"                );
        p->_move_speed                    = _table->GetField      (i, "MoveSpeed"                );
        p->_is_summon                     = _table->GetField      (i, "isSummon"                 );
        p->_summon_probability            = _table->GetField      (i, "SummonProbability"        );
        p->_damage_add                    = _table->GetField      (i, "DamageAdd"                );
        p->_damage_suck                   = _table->GetField      (i, "DamageSuck"               );
        p->_is_more_income                = _table->GetField      (i, "isMoreIncome"             );
        p->_more_income                   = _table->GetField      (i, "MoreIncome"               );
        p->_immunity_probability          = _table->GetField      (i, "ImmunityProbability"      );

        p->_rebound_count                 = _table->GetField      (i, "ReboundCount"             );
        p->_rebound_proportion            = _table->GetField      (i, "ReboundProportion"        );
        p->_icon_mini_ui                  = _table->GetField      (i, "IconMiniUI"               );
        p->_icon_ui                       = _table->GetField      (i, "IconUI"                   );
        p->_icon_tips                     = _table->GetField      (i, "IconTips"                 );
        p->_effect_shape                  = _table->GetField      (i, "EffectShape"              );
        p->_tips                          = _table->GetField      (i, "Tips"                     );
        p->_add_buff_shape                = _table->GetField      (i, "AddBuffShape"             );
        p->_add_buff_msg                  = _table->GetField      (i, "AddBuffMsg"               );
        p->_remove_buff_shape             = _table->GetField      (i, "RemoveBuffShape"          );
        p->_remove_buff_msg               = _table->GetField      (i, "RemoveBuffMsg"            );
        p->_buff_type                     = _table->GetField      (i, "BuffType"                 );
        p->_buff_pool_value               = _table->GetField      (i, "BuffPoolValue"            );
        p->_is_Dismount                   = _table->GetField      (i, "isDismount"               );
        p->_buff_show_in_panel            = _table->GetField      (i, "buff_show_in_panel"       );
        p->_buff_show_in_time             = _table->GetField      (i, "buff_show_in_time"        );
        p->_range_affect                  = _table->GetField      (i, "RangeAffect"              );
        p->_more_damage                   = _table->GetField      (i, "MoreDamage"               );
        p->_buff_effect_id                = _table->GetField      (i, "buff_effect_id"           );
        p->_harm_trigger_skill_id         = _table->GetField      (i, "HarmTriggerSkillID"       );
        p->_affect_type                   = _table->GetField      (i, "AffectType"               );
        p->_immunity_freeze               = _table->GetField      (i, "ImmunityFreeze"           );
        p->_immunity_affect               = _table->GetField      (i, "ImmunityAffect"           );
        p->_is_shift_add                  = _table->GetField      (i, "isShiftAdd"               );
        p->_is_shift_del                  = _table->GetField      (i, "isShiftDel"               );
        p->_call_monster_performance      = _table->GetField      (i, "CallMonsterPerformance"   );
        p->_call_monster_round_radius     = _table->GetField      (i, "CallMonsterRoundRadius"   );
        p->_repel_point                   = _table->GetField      (i, "RepelPoint"               );
		p->_upgrade_need_level            = _table->GetField      (i, "UpgradeNeedLevels"        );
		p->_upgrade_effect_id             = _table->GetField      (i, "UpgradeEffectID"			 );
		p->_upgrade_need_money            = _table->GetField      (i, "UpgradeNeedGameMoney"     );
        p->_out_raid_clear_buff           = _table->GetField      (i, "OutRaidClearBuff"         );
        p->_taunt                         = _table->GetField      (i, "Taunt"                    );
        p->_taunt_time                    = _table->GetField      (i, "TauntTime"                );
        p->_seckill                       = _table->GetField      (i, "Seckill"                  );
        p->_halo                          = _table->GetField      (i, "Halo"                     );
        p->_next_trigger_effect_id          = _table->GetField       (i, "NextTriggerEffectID"         );
        p->_next_trigger_effect_count       = _table->GetField       (i, "NextTriggerEffectCount"      );
        p->_next_trigger_effect_probability = _table->GetField       (i, "NextTriggerEffectProbability");
        p->_next_trigger_effect_object      = _table->GetField       (i, "NextTriggerEffectObject"     );

        cpf f;
        f.set_cfg(&p->_effect_object_type);
        cp.parser<cpf>(_table->GetFieldString(i, "EffectObjectType").c_str(), ";", f);
        f.set_cfg(&p->_broadcast_type);
        cp.parser<cpf>(_table->GetFieldString(i, "BroadcastType").c_str(), ";", f);
        f.set_cfg(&p->_trigger_remove);
        cp.parser<cpf>(_table->GetFieldString(i, "TriggerRemove").c_str(), ";", f);
        f.set_cfg(&p->_immunity_list);
        cp.parser<cpf>(_table->GetFieldString(i, "ImmunityList").c_str(), ";", f);
        f.set_cfg(&p->_freeze_list);
        cp.parser<cpf>(_table->GetFieldString(i, "FreezeList").c_str(), ";", f);
        f.set_cfg(&p->_immunity_buffer);
        cp.parser<cpf>(_table->GetFieldString(i, "ImmunityBuffer").c_str(), ";", f);
        f.set_cfg(&p->_summon_monster_list);
        cp.parser<cpf>(_table->GetFieldString(i, "SummonMonsterList").c_str(), ";", f);
        f.set_cfg(&p->_effect_type_id);
        cp.parser<cpf>(_table->GetFieldString(i, "EffectTypeID").c_str(), ";", f);
        f.set_cfg(&p->_clear_buff_list);
        cp.parser<cpf>(_table->GetFieldString(i, "ClearBuffList").c_str(), ";", f);

        p->_fl = 0;
        for (size_t size = 0; size < p->_freeze_list.size(); ++size) {
            switch (p->_freeze_list[size]) {
            case 0 : p->_fl |= FT_MOVE     ; break;
            case 1 : p->_fl |= FT_SKILL    ; break;
            case 2 : p->_fl |= FT_PK       ; break;
            case 3 : p->_fl |= FT_EQUIP_ATK; break;
            case 4 : p->_fl |= FT_EQUIP_DEF; break;
            case 5 : p->_fl |= FT_BLIND    ; break;
            case 6 : p->_fl |= FT_ATK      ; break;
            case 7 : p->_fl |= FT_DEF      ; break;
            case 8 : p->_fl |= FT_MP       ; break;
            case 9 : p->_fl |= FT_RAND_MOVE; break;
            case 10: p->_fl |= FT_WEEK     ; break;
            case 11: p->_fl |= FT_HURTED   ; break;
            case 12: p->_fl |= FT_DEC_HURM ; break;
            default: break;
            }
        }

        cpf_2_0 ebh(p->_each_bear_harm);
        cp.parser<cpf_2_0>(_table->GetFieldString(i, "EachBearHarm").c_str(), ";", ebh);
        cpf_2_0 hp(p->_affect_hp);
        cp.parser<cpf_2_0>(_table->GetFieldString(i, "AffectHP").c_str(), ";", hp);
        p->_affect_hp.type = 24;// GAME_ATTRIBUTE_CUR_HP;
        cpf_2_0 mp(p->_affect_mp);
        cp.parser<cpf_2_0>(_table->GetFieldString(i, "AffectMP").c_str(), ";", mp);
        p->_affect_mp.type = 27;//GAME_ATTRIBUTE_CUR_MP;
        cpf_2 f2;
        f2.set_cfg(&p->_affect_list);
        cp.parser<cpf_2>(_table->GetFieldString(i, "AffectList").c_str(), ";", f2);

        f2.set_cfg(&p->_seckill_hp);
        cp.parser<cpf_2>(_table->GetFieldString(i, "SeckillHP").c_str(), ";", f2);

        f2.set_cfg(&p->_effect_correction_i);
        cp.parser<cpf_2>(_table->GetFieldString(i, "EffectCorrection").c_str(), ";", f2);

        if (p->_next_trigger_skill_id != 0) {
            p->_next_trigger_skill = CONFMGR->get_config_skill(p->_next_trigger_skill_id);
        }

        _cb.insert(std::pair<uint32, config_effect*>(p->_effect_id, p));
        _ces.insert(std::pair<uint32, config_effect*>(p->_key, p));
    }

    return 0;
}

int config_effect_mgr::init(TTableGroup_s* p)
{
    return 0;
}

int config_effect_mgr::clear()
{
    for (std::map<uint32, config_effect*>::iterator i = _cb.begin(); i != _cb.end(); ++i) {
        if (i->second != NULL) {
            delete i->second;
        }
    }

    _cb.clear();

    return 0;
}

const config_effect* config_effect_mgr::get_config_effect(uint32 id, uint32 level /* = 1 */)
{
    if (id == 0) {
        return NULL;
    }

    std::map<uint32, config_effect*>::const_iterator i = _cb.find(id);
    if (i == _cb.end()) {
        return quick_read_by_id(id);
    }

    return i->second;
}

const config_effect* config_effect_mgr::get_config_effect_by_key(uint32 key, uint32 level /*= 1*/)
{
    if (key == 0) {
        return NULL;
    }

    std::multimap<uint32, config_effect*>::const_iterator i = _ces.find(key);
    while (i != _ces.end()) {
        if (i->second->_key != key) {
            break;
        }

        if (i->second->_levels == level) {
            return i->second;
        }

        ++i;
    }

    return quick_read_by_key(key, level);
}

void config_effect_mgr::set_table_group(TTableGroup_s* effect, TTableGroup_s* role_effect)
{
    _effect = effect;
    _role_effect = role_effect;
}

const config_effect* config_effect_mgr::read_effect(uint32 id)
{
    _table = _effect->GetTable("t_effect");
    if (_table == NULL) {
        printf("not found t_effect\n");
    }

    const config_effect* p = load_effect(_table, id, 0);
    if (p != NULL) {
        return p;
    }

    _table = _role_effect->GetTable("t_role_effect");
    if (_table == NULL) {
        printf("not found t_role_effect\n");
    }

    p = load_effect(_table, id, 1);

    return p;
}

const config_effect* config_effect_mgr::load_effect(TTable* table, uint32 id, uint32 source)
{
    cfg_parser cp;
    config_effect* p = NULL;
    for (int i = 1; i <= table->m_recordCount; ++i) {
        uint32 effect_id  = table->GetField      (i, "MethodID"                 );
        if (effect_id == 0 || id != effect_id) {
            continue;
        }

        std::map<uint32, config_effect*>::iterator it = _cb.find(effect_id);
        if (it != _cb.end()) {
            p = it->second;
            break;
        }

        p = new config_effect;
        p->_key                           = table->GetField      (i, "Key"                      );
        p->_effect_id                     = table->GetField      (i, "MethodID"                 );
        p->_method_id                     = table->GetField      (i, "MethodID"                 );
        p->_method_name                   = table->GetFieldString(i, "MethodName"               );
        p->_levels                        = table->GetField      (i, "Levels"                   );

        p->_is_buff                       = table->GetField      (i, "isBuff"                   );
        p->_is_scene_buff                 = table->GetField      (i, "isSceneBuff"              );
        p->_buff_probability              = table->GetField      (i, "BuffProbability"          );
        p->_buff_useful_type              = table->GetField      (i, "BuffUsefulType"           );

        p->_next_trigger_skill_id         = table->GetField      (i, "NextTriggerSkillID"       );
        p->_next_trigger_count            = table->GetField      (i, "NextTriggerCount"         );
        p->_next_triggert_probability     = table->GetField      (i, "NextTriggertProbability"  );
        p->_duration_tick                 = table->GetField      (i, "DurationTick"             );
        p->_interval_tick                 = table->GetField      (i, "IntervalTick"             );
        p->_repeat_type                   = table->GetField      (i, "RepeatType"               );
        p->_repeat_max                    = table->GetField      (i, "RepeatMax"                );
        p->_repeat_levels                 = table->GetField      (i, "RepeatLevels"             );

        p->_durable_max                   = table->GetField      (i, "DurableMax"               );

        p->_over_back_proportion          = table->GetField      (i, "OverBackProportion"       );
        p->_over_back_max                 = table->GetField      (i, "OverBackMax"              );
        p->_monster_hide                  = table->GetField      (i, "MonsterHide"              );
        p->_player_hide                   = table->GetField      (i, "PlayerHide"               );

        p->_affect_param                  = table->GetField      (i, "AffectParam"              );
        p->_affect_proportion             = table->GetField      (i, "AffectProportion"         );

        p->_be_cleared                    = table->GetField      (i, "BeCleared"                );
        p->_offline_death_remove          = table->GetField      (i, "OfflineDeathRemove"       );
        p->_offline_timer                 = table->GetField      (i, "OfflineTimer"             );
        p->_owner_offline_death_remove    = table->GetField      (i, "OwnerOfflineDeathRemove"  );
        p->_share_troop                   = table->GetField      (i, "ShareTroop"               );
        p->_harm_check                    = table->GetField      (i, "HarmCheck"                );
        p->_is_clear_buff                 = table->GetField      (i, "isClearBuff"              );
        p->_clear_buff_probability        = table->GetField      (i, "ClearBuffProbability"     );
        p->_clear_useful_buff_level       = table->GetField      (i, "ClearUsefulBuffLevel"     );
        p->_clear_harmful_buff_level      = table->GetField      (i, "ClearHarmfulBuffLevel"    );
        p->_clear_useful_anti_buff_level  = table->GetField      (i, "ClearUsefulAntiBuffLevel" );
        p->_clear_harmful_anti_buff_level = table->GetField      (i, "ClearHarmfulAntiBuffLevel");
        p->_is_rebirth                    = table->GetField      (i, "isRebirth"                );
        p->_rebirth_probability           = table->GetField      (i, "RebirthProbability"       );
        p->_rebirth_hp                    = table->GetField      (i, "RebirthHP"                );
        p->_rebirth_mp                    = table->GetField      (i, "RebirthMP"                );
        p->_is_move                       = table->GetField      (i, "isMove"                   );
        p->_move_probability              = table->GetField      (i, "MoveProbability"          );
        p->_move_type                     = table->GetField      (i, "MoveType"                 );
        p->_move_range                    = table->GetField      (i, "MoveRange"                );
        p->_move_speed                    = table->GetField      (i, "MoveSpeed"                );
        p->_is_summon                     = table->GetField      (i, "isSummon"                 );
        p->_summon_probability            = table->GetField      (i, "SummonProbability"        );
        p->_damage_add                    = table->GetField      (i, "DamageAdd"                );
        p->_damage_suck                   = table->GetField      (i, "DamageSuck"               );
        p->_is_more_income                = table->GetField      (i, "isMoreIncome"             );
        p->_more_income                   = table->GetField      (i, "MoreIncome"               );
        p->_immunity_probability          = table->GetField      (i, "ImmunityProbability"      );

        p->_rebound_count                 = table->GetField      (i, "ReboundCount"             );
        p->_rebound_proportion            = table->GetField      (i, "ReboundProportion"        );
        p->_icon_mini_ui                  = table->GetField      (i, "IconMiniUI"               );
        p->_icon_ui                       = table->GetField      (i, "IconUI"                   );
        p->_icon_tips                     = table->GetField      (i, "IconTips"                 );
        p->_effect_shape                  = table->GetField      (i, "EffectShape"              );
        p->_tips                          = table->GetField      (i, "Tips"                     );
        p->_add_buff_shape                = table->GetField      (i, "AddBuffShape"             );
        p->_add_buff_msg                  = table->GetField      (i, "AddBuffMsg"               );
        p->_remove_buff_shape             = table->GetField      (i, "RemoveBuffShape"          );
        p->_remove_buff_msg               = table->GetField      (i, "RemoveBuffMsg"            );
        p->_buff_type                     = table->GetField      (i, "BuffType"                 );
        p->_buff_pool_value               = table->GetField      (i, "BuffPoolValue"            );
        p->_is_Dismount                   = table->GetField      (i, "isDismount"               );
        p->_buff_show_in_panel            = table->GetField      (i, "buff_show_in_panel"       );
        p->_buff_show_in_time             = table->GetField      (i, "buff_show_in_time"        );
        p->_range_affect                  = table->GetField      (i, "RangeAffect"              );
        p->_more_damage                   = table->GetField      (i, "MoreDamage"               );
        p->_buff_effect_id                = table->GetField      (i, "buff_effect_id"           );
        p->_harm_trigger_skill_id         = table->GetField      (i, "HarmTriggerSkillID"       );
        p->_affect_type                   = table->GetField      (i, "AffectType"               );
        p->_immunity_freeze               = table->GetField      (i, "ImmunityFreeze"           );
        p->_immunity_affect               = table->GetField      (i, "ImmunityAffect"           );
        p->_is_shift_add                  = table->GetField      (i, "isShiftAdd"               );
        p->_is_shift_del                  = table->GetField      (i, "isShiftDel"               );
        p->_call_monster_performance      = table->GetField      (i, "CallMonsterPerformance"   );
        p->_call_monster_round_radius     = table->GetField      (i, "CallMonsterRoundRadius"   );
        p->_repel_point                   = table->GetField      (i, "RepelPoint"               );
        p->_upgrade_need_level            = table->GetField      (i, "UpgradeNeedLevels"        );
        p->_upgrade_need_money            = table->GetField      (i, "UpgradeNeedGameMoney"     );
        p->_out_raid_clear_buff           = table->GetField      (i, "OutRaidClearBuff"         );
        p->_taunt                         = table->GetField      (i, "Taunt"                    );
        p->_taunt_time                    = table->GetField      (i, "TauntTime"                );
        p->_seckill                       = table->GetField      (i, "Seckill"                  );
        p->_halo                          = table->GetField      (i, "Halo"                     );
        p->_next_trigger_effect_id          = _table->GetField       (i, "NextTriggerEffectID"         );
        p->_next_trigger_effect_count       = _table->GetField       (i, "NextTriggerEffectCount"      );
        p->_next_trigger_effect_probability = _table->GetField       (i, "NextTriggerEffectProbability");
        p->_next_trigger_effect_object      = _table->GetField       (i, "NextTriggerEffectObject"     );

        cpf f;
        f.set_cfg(&p->_effect_object_type);
        cp.parser<cpf>(table->GetFieldString(i, "EffectObjectType").c_str(), ";", f);
        f.set_cfg(&p->_broadcast_type);
        cp.parser<cpf>(table->GetFieldString(i, "BroadcastType").c_str(), ";", f);
        f.set_cfg(&p->_trigger_remove);
        cp.parser<cpf>(table->GetFieldString(i, "TriggerRemove").c_str(), ";", f);
        f.set_cfg(&p->_immunity_list);
        cp.parser<cpf>(table->GetFieldString(i, "ImmunityList").c_str(), ";", f);
        f.set_cfg(&p->_freeze_list);
        cp.parser<cpf>(table->GetFieldString(i, "FreezeList").c_str(), ";", f);
        f.set_cfg(&p->_immunity_buffer);
        cp.parser<cpf>(table->GetFieldString(i, "ImmunityBuffer").c_str(), ";", f);
        f.set_cfg(&p->_summon_monster_list);
        cp.parser<cpf>(table->GetFieldString(i, "SummonMonsterList").c_str(), ";", f);
        f.set_cfg(&p->_effect_type_id);
        cp.parser<cpf>(table->GetFieldString(i, "EffectTypeID").c_str(), ";", f);
        f.set_cfg(&p->_clear_buff_list);
        cp.parser<cpf>(_table->GetFieldString(i, "ClearBuffList").c_str(), ";", f);

        p->_fl = 0;
        for (size_t size = 0; size < p->_freeze_list.size(); ++size) {
            switch (p->_freeze_list[size]) {
            case 0 : p->_fl |= FT_MOVE     ; break;
            case 1 : p->_fl |= FT_SKILL    ; break;
            case 2 : p->_fl |= FT_PK       ; break;
            case 3 : p->_fl |= FT_EQUIP_ATK; break;
            case 4 : p->_fl |= FT_EQUIP_DEF; break;
            case 5 : p->_fl |= FT_BLIND    ; break;
            case 6 : p->_fl |= FT_ATK      ; break;
            case 7 : p->_fl |= FT_DEF      ; break;
            case 8 : p->_fl |= FT_MP       ; break;
            case 9 : p->_fl |= FT_RAND_MOVE; break;
            case 10: p->_fl |= FT_WEEK     ; break;
            case 11: p->_fl |= FT_HURTED   ; break;
            case 12: p->_fl |= FT_DEC_HURM ; break;
            default: break;
            }
        }

        cpf_2_0 ebh(p->_each_bear_harm);
        cp.parser<cpf_2_0>(table->GetFieldString(i, "EachBearHarm").c_str(), ";", ebh);
        cpf_2_0 hp(p->_affect_hp);
        cp.parser<cpf_2_0>(table->GetFieldString(i, "AffectHP").c_str(), ";", hp);
        p->_affect_hp.type = 24;// GAME_ATTRIBUTE_CUR_HP;
        cpf_2_0 mp(p->_affect_mp);
        cp.parser<cpf_2_0>(table->GetFieldString(i, "AffectMP").c_str(), ";", mp);
        p->_affect_mp.type = 27;//GAME_ATTRIBUTE_CUR_MP;
        cpf_2 f2;
        f2.set_cfg(&p->_affect_list);
        cp.parser<cpf_2>(table->GetFieldString(i, "AffectList").c_str(), ";", f2);

        f2.set_cfg(&p->_seckill_hp);
        cp.parser<cpf_2>(table->GetFieldString(i, "SeckillHP").c_str(), ";", f2);

        f2.set_cfg(&p->_effect_correction_i);
        cp.parser<cpf_2>(table->GetFieldString(i, "EffectCorrection").c_str(), ";", f2);

        if (p->_next_trigger_skill_id != 0) {
            p->_next_trigger_skill = CONFMGR->get_config_skill(p->_next_trigger_skill_id);
        }

        _cb.insert(std::pair<uint32, config_effect*>(p->_effect_id, p));
        _ces.insert(std::pair<uint32, config_effect*>(p->_key, p));

        break;
    }

    return p;
}

void config_effect_mgr::quick_init(TTableGroup_s* effect, TTableGroup_s* role_effect)
{
    _effect = effect;
    _table = _effect->GetTable("t_effect");
    if (_table == NULL) {
        printf("not found t_effect\n");
        return ;
    }

    quick_load(_table, 0);

    _role_effect = role_effect;
    _table = _role_effect->GetTable("t_effect_role");
    if (_table == NULL) {
        printf("not found t_effect_role\n");
        return ;
    }

    quick_load(_table, 1);
}

void config_effect_mgr::quick_load(TTable* table, uint32 source)
{
    for (int i = 1; i <= _table->m_recordCount; ++i) {
        uint32 effect_id  = _table->GetField      (i, "MethodID"                 );
        uint32 Key  = _table->GetField      (i, "Key"                 );
        table_index ti;
        ti._index = i;
        ti._source = source;
        _quick_by_id.insert(std::pair<uint32, table_index>(effect_id, ti));
        _quick_by_key.insert(std::pair<uint32, table_index>(Key, ti));
    }
}

const config_effect* config_effect_mgr::quick_read_by_id(uint32 id)
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
        printf("%s:%d %s, not found config_effect id:%u\n", __FILE__, __LINE__, __FUNCTION__, id);
        __sync_lock_release(&_lock);
        return NULL;
    }

    if (i->second._source == 0) {
        _table = _effect->GetTable("t_effect");
        if (_table == NULL) {
            printf("not found t_effect\n");
            __sync_lock_release(&_lock);
            return NULL;
        }

        __sync_lock_release(&_lock);
        return quick_read(_table, i->second._index);
    }

    if (i->second._source == 1) {
        _table = _role_effect->GetTable("t_effect_role");
        if (_table == NULL) {
            printf("not found t_effect_role\n");
            __sync_lock_release(&_lock);
            return NULL;
        }

        __sync_lock_release(&_lock);
        return quick_read(_table, i->second._index);
    }

    __sync_lock_release(&_lock);
    return NULL;
}

const config_effect* config_effect_mgr::quick_read_by_key(uint32 key, uint32 level)
{
    int ret = 1;
    do {
        ret = __sync_lock_test_and_set(&_lock, 1);
        if (ret != 0) {
            usleep(1);
        }
    } while (ret != 0);

    std::multimap<uint32, table_index>::iterator i = _quick_by_key.find(key);
    if (i == _quick_by_key.end()) {
        printf("%s:%d not found config_effect key:%u\n", __FILE__, __LINE__, key);
        __sync_lock_release(&_lock);
        return NULL;
    }

    while (i != _quick_by_key.end() && i->first == key) {
        if (i->second._source == 0) {
            _table = _effect->GetTable("t_effect");
            if (_table == NULL) {
                printf("not found t_effect %u\n", key);
                continue;
            }

            quick_read(_table, i->second._index);
        }

        if (i->second._source == 1) {
            _table = _role_effect->GetTable("t_effect_role");
            if (_table == NULL) {
                printf("not found t_effect_role %u\n", key);
                continue;
            }

            quick_read(_table, i->second._index);
        }

        ++i;
    }

    std::multimap<uint32, config_effect*>::const_iterator it = _ces.find(key);
    while (it != _ces.end()) {
        if (it->second->_key != key) {
            break;
        }

        if (it->second->_levels == level) {
            __sync_lock_release(&_lock);
            return it->second;
        }

        ++it;
    }

    __sync_lock_release(&_lock);

    return NULL;
}

const config_effect* config_effect_mgr::quick_read(TTable* table, uint32 id)
{
    cfg_parser cp;
    config_effect* p = NULL;
    for (int i = id; i <= table->m_recordCount; ++i) {
        uint32 effect_id  = table->GetField      (i, "MethodID"                 );
        if (effect_id == 0) {
            break;
        }

        std::map<uint32, config_effect*>::iterator it = _cb.find(effect_id);
        if (it != _cb.end()) {
            p = it->second;
            break;
        }

        p = new config_effect;
        p->_key                           = table->GetField      (i, "Key"                      );
        p->_effect_id                     = table->GetField      (i, "MethodID"                 );
        p->_method_id                     = table->GetField      (i, "MethodID"                 );
        p->_method_name                   = table->GetFieldString(i, "MethodName"               );
        p->_levels                        = table->GetField      (i, "Levels"                   );

        p->_is_buff                       = table->GetField      (i, "isBuff"                   );
        p->_is_scene_buff                 = table->GetField      (i, "isSceneBuff"              );
        p->_buff_probability              = table->GetField      (i, "BuffProbability"          );
        p->_buff_useful_type              = table->GetField      (i, "BuffUsefulType"           );

        p->_next_trigger_skill_id         = table->GetField      (i, "NextTriggerSkillID"       );
        p->_next_trigger_count            = table->GetField      (i, "NextTriggerCount"         );
        p->_next_triggert_probability     = table->GetField      (i, "NextTriggertProbability"  );
        p->_duration_tick                 = table->GetField      (i, "DurationTick"             );
        p->_interval_tick                 = table->GetField      (i, "IntervalTick"             );
        p->_repeat_type                   = table->GetField      (i, "RepeatType"               );
        p->_repeat_max                    = table->GetField      (i, "RepeatMax"                );
        p->_repeat_levels                 = table->GetField      (i, "RepeatLevels"             );

        p->_durable_max                   = table->GetField      (i, "DurableMax"               );

        p->_over_back_proportion          = table->GetField      (i, "OverBackProportion"       );
        p->_over_back_max                 = table->GetField      (i, "OverBackMax"              );
        p->_monster_hide                  = table->GetField      (i, "MonsterHide"              );
        p->_player_hide                   = table->GetField      (i, "PlayerHide"               );

        p->_affect_param                  = table->GetField      (i, "AffectParam"              );
        p->_affect_proportion             = table->GetField      (i, "AffectProportion"         );

        p->_be_cleared                    = table->GetField      (i, "BeCleared"                );
        p->_offline_death_remove          = table->GetField      (i, "OfflineDeathRemove"       );
        p->_offline_timer                 = table->GetField      (i, "OfflineTimer"             );
        p->_owner_offline_death_remove    = table->GetField      (i, "OwnerOfflineDeathRemove"  );
        p->_share_troop                   = table->GetField      (i, "ShareTroop"               );
        p->_harm_check                    = table->GetField      (i, "HarmCheck"                );
        p->_is_clear_buff                 = table->GetField      (i, "isClearBuff"              );
        p->_clear_buff_probability        = table->GetField      (i, "ClearBuffProbability"     );
        p->_clear_useful_buff_level       = table->GetField      (i, "ClearUsefulBuffLevel"     );
        p->_clear_harmful_buff_level      = table->GetField      (i, "ClearHarmfulBuffLevel"    );
        p->_clear_useful_anti_buff_level  = table->GetField      (i, "ClearUsefulAntiBuffLevel" );
        p->_clear_harmful_anti_buff_level = table->GetField      (i, "ClearHarmfulAntiBuffLevel");
        p->_is_rebirth                    = table->GetField      (i, "isRebirth"                );
        p->_rebirth_probability           = table->GetField      (i, "RebirthProbability"       );
        p->_rebirth_hp                    = table->GetField      (i, "RebirthHP"                );
        p->_rebirth_mp                    = table->GetField      (i, "RebirthMP"                );
        p->_is_move                       = table->GetField      (i, "isMove"                   );
        p->_move_probability              = table->GetField      (i, "MoveProbability"          );
        p->_move_type                     = table->GetField      (i, "MoveType"                 );
        p->_move_range                    = table->GetField      (i, "MoveRange"                );
        p->_move_speed                    = table->GetField      (i, "MoveSpeed"                );
        p->_is_summon                     = table->GetField      (i, "isSummon"                 );
        p->_summon_probability            = table->GetField      (i, "SummonProbability"        );
        p->_damage_add                    = table->GetField      (i, "DamageAdd"                );
        p->_damage_suck                   = table->GetField      (i, "DamageSuck"               );
        p->_is_more_income                = table->GetField      (i, "isMoreIncome"             );
        p->_more_income                   = table->GetField      (i, "MoreIncome"               );
        p->_immunity_probability          = table->GetField      (i, "ImmunityProbability"      );

        p->_rebound_count                 = table->GetField      (i, "ReboundCount"             );
        p->_rebound_proportion            = table->GetField      (i, "ReboundProportion"        );
        p->_icon_mini_ui                  = table->GetField      (i, "IconMiniUI"               );
        p->_icon_ui                       = table->GetField      (i, "IconUI"                   );
        p->_icon_tips                     = table->GetField      (i, "IconTips"                 );
        p->_effect_shape                  = table->GetField      (i, "EffectShape"              );
        p->_tips                          = table->GetField      (i, "Tips"                     );
        p->_add_buff_shape                = table->GetField      (i, "AddBuffShape"             );
        p->_add_buff_msg                  = table->GetField      (i, "AddBuffMsg"               );
        p->_remove_buff_shape             = table->GetField      (i, "RemoveBuffShape"          );
        p->_remove_buff_msg               = table->GetField      (i, "RemoveBuffMsg"            );
        p->_buff_type                     = table->GetField      (i, "BuffType"                 );
        p->_buff_pool_value               = table->GetField      (i, "BuffPoolValue"            );
        p->_is_Dismount                   = table->GetField      (i, "isDismount"               );
        p->_buff_show_in_panel            = table->GetField      (i, "buff_show_in_panel"       );
        p->_buff_show_in_time             = table->GetField      (i, "buff_show_in_time"        );
        p->_range_affect                  = table->GetField      (i, "RangeAffect"              );
        p->_more_damage                   = table->GetField      (i, "MoreDamage"               );
        p->_buff_effect_id                = table->GetField      (i, "buff_effect_id"           );
        p->_harm_trigger_skill_id         = table->GetField      (i, "HarmTriggerSkillID"       );
        p->_affect_type                   = table->GetField      (i, "AffectType"               );
        p->_immunity_freeze               = table->GetField      (i, "ImmunityFreeze"           );
        p->_immunity_affect               = table->GetField      (i, "ImmunityAffect"           );
        p->_is_shift_add                  = table->GetField      (i, "isShiftAdd"               );
        p->_is_shift_del                  = table->GetField      (i, "isShiftDel"               );
        p->_call_monster_performance      = table->GetField      (i, "CallMonsterPerformance"   );
        p->_call_monster_round_radius     = table->GetField      (i, "CallMonsterRoundRadius"   );
        p->_repel_point                   = table->GetField      (i, "RepelPoint"               );
        p->_upgrade_need_level            = table->GetField      (i, "UpgradeNeedLevels"        );
        p->_upgrade_need_money            = table->GetField      (i, "UpgradeNeedGameMoney"     );
        p->_out_raid_clear_buff           = table->GetField      (i, "OutRaidClearBuff"         );
        p->_taunt                         = table->GetField      (i, "Taunt"                    );
        p->_taunt_time                    = table->GetField      (i, "TauntTime"                );
        p->_seckill                       = table->GetField      (i, "Seckill"                  );
        p->_halo                          = table->GetField      (i, "Halo"                     );
        p->_next_trigger_effect_id          = _table->GetField       (i, "NextTriggerEffectID"         );
        p->_next_trigger_effect_count       = _table->GetField       (i, "NextTriggerEffectCount"      );
        p->_next_trigger_effect_probability = _table->GetField       (i, "NextTriggerEffectProbability");
        p->_next_trigger_effect_object      = _table->GetField       (i, "NextTriggerEffectObject"     );

        cpf f;
        f.set_cfg(&p->_effect_object_type);
        cp.parser<cpf>(table->GetFieldString(i, "EffectObjectType").c_str(), ";", f);
        f.set_cfg(&p->_broadcast_type);
        cp.parser<cpf>(table->GetFieldString(i, "BroadcastType").c_str(), ";", f);
        f.set_cfg(&p->_trigger_remove);
        cp.parser<cpf>(table->GetFieldString(i, "TriggerRemove").c_str(), ";", f);
        f.set_cfg(&p->_immunity_list);
        cp.parser<cpf>(table->GetFieldString(i, "ImmunityList").c_str(), ";", f);
        f.set_cfg(&p->_freeze_list);
        cp.parser<cpf>(table->GetFieldString(i, "FreezeList").c_str(), ";", f);
        f.set_cfg(&p->_immunity_buffer);
        cp.parser<cpf>(table->GetFieldString(i, "ImmunityBuffer").c_str(), ";", f);
        f.set_cfg(&p->_summon_monster_list);
        cp.parser<cpf>(table->GetFieldString(i, "SummonMonsterList").c_str(), ";", f);
        f.set_cfg(&p->_effect_type_id);
        cp.parser<cpf>(table->GetFieldString(i, "EffectTypeID").c_str(), ";", f);
        f.set_cfg(&p->_clear_buff_list);
        cp.parser<cpf>(_table->GetFieldString(i, "ClearBuffList").c_str(), ";", f);

        p->_fl = 0;
        for (size_t size = 0; size < p->_freeze_list.size(); ++size) {
            switch (p->_freeze_list[size]) {
            case 0 : p->_fl |= FT_MOVE     ; break;
            case 1 : p->_fl |= FT_SKILL    ; break;
            case 2 : p->_fl |= FT_PK       ; break;
            case 3 : p->_fl |= FT_EQUIP_ATK; break;
            case 4 : p->_fl |= FT_EQUIP_DEF; break;
            case 5 : p->_fl |= FT_BLIND    ; break;
            case 6 : p->_fl |= FT_ATK      ; break;
            case 7 : p->_fl |= FT_DEF      ; break;
            case 8 : p->_fl |= FT_MP       ; break;
            case 9 : p->_fl |= FT_RAND_MOVE; break;
            case 10: p->_fl |= FT_WEEK     ; break;
            case 11: p->_fl |= FT_HURTED   ; break;
            case 12: p->_fl |= FT_DEC_HURM ; break;
            default: break;
            }
        }

        cpf_2_0 ebh(p->_each_bear_harm);
        cp.parser<cpf_2_0>(table->GetFieldString(i, "EachBearHarm").c_str(), ";", ebh);
        cpf_2_0 hp(p->_affect_hp);
        cp.parser<cpf_2_0>(table->GetFieldString(i, "AffectHP").c_str(), ";", hp);
        p->_affect_hp.type = 24;// GAME_ATTRIBUTE_CUR_HP;
        cpf_2_0 mp(p->_affect_mp);
        cp.parser<cpf_2_0>(table->GetFieldString(i, "AffectMP").c_str(), ";", mp);
        p->_affect_mp.type = 27;//GAME_ATTRIBUTE_CUR_MP;
        cpf_2 f2;
        f2.set_cfg(&p->_affect_list);
        cp.parser<cpf_2>(table->GetFieldString(i, "AffectList").c_str(), ";", f2);

        f2.set_cfg(&p->_seckill_hp);
        cp.parser<cpf_2>(table->GetFieldString(i, "SeckillHP").c_str(), ";", f2);

        f2.set_cfg(&p->_effect_correction_i);
        cp.parser<cpf_2>(table->GetFieldString(i, "EffectCorrection").c_str(), ";", f2);

        if (p->_next_trigger_skill_id != 0) {
            p->_next_trigger_skill = CONFMGR->get_config_skill(p->_next_trigger_skill_id);
        }

        _cb.insert(std::pair<uint32, config_effect*>(p->_effect_id, p));
        _ces.insert(std::pair<uint32, config_effect*>(p->_key, p));

        break;
    }

    return p;
}
