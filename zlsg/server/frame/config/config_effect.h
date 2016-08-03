#ifndef __config_effect_H__
#define __config_effect_H__

#include "cfg_cache_mgr.h"
#include "cfg_parser.h"
#include "config_base.h"

enum FREEZE_TYPE {
    FT_MOVE      = 0x00000001,
    FT_SKILL     = 0x00000002,
    FT_PK        = 0x00000004,
    FT_EQUIP_ATK = 0x00000008,
    FT_EQUIP_DEF = 0x00000010,
    FT_BLIND     = 0x00000020,
    FT_ATK       = 0x00000040,
    FT_DEF       = 0x00000080,
    FT_MP        = 0x00000100,
    FT_RAND_MOVE = 0x00000200,
    FT_WEEK      = 0x00000400,
    FT_HURTED    = 0x00000800,
    FT_DEC_HURM  = 0x00001000,
    FT_MAX       = 0xFFFFFFFF,
};

enum BUFFER_TYPE_E
{
    BTE_BUFFER_NULL   = 0, // 非BUFFER类
    BTE_BUFFER_TIME   = 1, // 计时类
    BTE_BUFFER_FORVER = 2, // 永久性效果
    BTE_BUFFER_NUMBER = 3, // 数值形的
};

enum EFFECT_REPEAT_TYPE {
    ERT_ADD   = 0, // 0=效果叠加;
    ERT_REP   = 1, // 1=效果替换;
    ERT_NAN   = 2, // 2=重复无效;
    ERT_CLS   = 3, // 3=重复关闭;
    ERT_ADD_T = 4, // 0=效果叠加时间;
    ERT_ADD_V = 5, // 0=效果叠加数值;
};

enum EFFECT_SCENE_BUFF {
    ESB_NOT_SCENE_BUFF = 0,
    ESB_SCENE_BUFF     = 1,
};

enum EFFECT_USEFUL_TYPE {
    EUT_USEFUL   = 0,
    EUT_HARMFUL  = 1,
    EUT_NOT_SHOW = 2,
};

enum EFFECT_OBJECT_TYPE {
    EOT_OWN          = 0,
    EOT_ENEMY        = 1,
    EOT_FRIEND       = 2,
    EOT_FRIEND_BODY  = 3,
    EOT_NEUTRAL      = 4,
    EOT_ENEMY_NORMAL = 5,
};

enum EFFECT_MOVE_TYPE {
    EMT_REPEL      = 0,
    EMT_POINT      = 1,
    EMT_TRANS      = 2,
    EMT_PULL       = 3,
    EMT_SCENE_RAND = 4,
    EMT_BASE_RAND  = 5,
    EMT_BLOW_FLY   = 6,
};

enum SUMMON_PERFORMANCE {
    SP_FACE   = 1,
    SP_ROUND  = 2,
    SP_RANDOM = 3,
};

enum REPEL_POINT {
    RP_ATK = 1,
    RP_DEF = 2,
};

class config_skill;

class config_effect {
public:
    config_effect(void);
    ~config_effect(void);

public:
    inline const uint32                    get_effect_id                () const { return _effect_id                ; }
    inline const uint32                    get_levels                   () const { return _levels                   ; }
    inline const uint32                    get_is_buff                  () const { return _is_buff                  ; }
    inline const uint32                    get_is_scene_buff            () const { return _is_scene_buff            ; }
    inline const uint32                    get_buff_probability         () const { return _buff_probability         ; }
    inline const uint32                    get_buff_useful_type         () const { return _buff_useful_type         ; }
    inline const uint32                    get_next_trigger_skill_id    () const { return _next_trigger_skill_id    ; }
    inline const uint32                    get_duration_tick            () const { return _duration_tick            ; }
    inline const uint32                    get_interval_tick            () const { return _interval_tick            ; }
    inline const uint32                    get_repeat_type              () const { return _repeat_type              ; }
    inline const uint32                    get_repeat_max               () const { return _repeat_max               ; }
    inline const uint32                    get_repeat_levels            () const { return _repeat_levels            ; }
    inline const int32                     get_durable_max              () const { return _durable_max              ; }
    inline const cpf_val_2&                get_each_bear_harm           () const { return _each_bear_harm           ; }
    inline const int32                     get_over_back_proportion     () const { return _over_back_proportion     ; }
    inline const int32                     get_over_back_max            () const { return _over_back_max            ; }
    inline const cpf_val_2&                get_affect_hp                () const { return _affect_hp                ; }
    inline const cpf_val_2&                get_affect_mp                () const { return _affect_mp                ; }
    inline const uint32                    get_offline_death_remove     () const { return _offline_death_remove     ; }
    inline const uint32                    get_is_move                  () const { return _is_move                  ; }
    inline const uint32                    get_move_probability         () const { return _move_probability         ; }
    inline const uint32                    get_move_type                () const { return _move_type                ; }
    inline const uint32                    get_move_range               () const { return _move_range               ; }
    inline const uint32                    get_move_speed               () const { return _move_speed               ; }
    inline const uint32                    get_is_summon                () const { return _is_summon                ; }
    inline const uint32                    get_rebound_count            () const { return _rebound_count            ; }
    inline const uint32                    get_rebound_proportion       () const { return _rebound_proportion       ; }
    inline const std::vector<int>&         get_summon_monster_list      () const { return _summon_monster_list      ; }
    inline const uint32                    get_immunity_probability     () const { return _immunity_probability     ; }
    inline const std::vector<int>&         get_immunity_buffer          () const { return _immunity_buffer          ; }
    inline const std::vector<int>&         get_effect_object_type       () const { return _effect_object_type       ; }
    inline const std::vector<int>&         get_freeze_list              () const { return _freeze_list              ; }
    inline const std::map<int, cpf_val_2>& get_affect_list              () const { return _affect_list              ; }
    inline const uint32                    get_is_clear_buff            () const { return _is_clear_buff            ; }
    inline const uint32                    get_clear_buff_probability   () const { return _clear_buff_probability   ; }
    inline       uint32                    get_clear_useful_buff_level  () const { return _clear_useful_buff_level  ; }
    inline       uint32                    get_clear_harmful_buff_level () const { return _clear_harmful_buff_level ; }
    inline const std::vector<int>&         get_clear_buff_list          () const { return _clear_buff_list          ; }
    inline uint32                          get_call_monster_performance () const { return _call_monster_performance ; }
    inline uint32                          get_call_monster_round_radius() const { return _call_monster_round_radius; }
    inline uint32                          get_repel_point              () const { return _repel_point              ; }
	inline uint32                          get_upgrade_need_level       () const { return _upgrade_need_level       ; }
	inline uint32                          get_upgrade_effect_id       () const { return _upgrade_effect_id			; }
	inline uint32                          get_upgrade_need_money       () const { return _upgrade_need_money       ; }
	inline const std::vector<int>&         get_effect_type_id           () const { return _effect_type_id           ; }
    inline uint32                          get_out_raid_clear_buff      () const { return _out_raid_clear_buff      ; }
    inline uint32                          get_taunt                    () const { return _taunt                    ; }
    inline uint32                          get_taunt_time               () const { return _taunt_time               ; }
    inline uint32                          get_seckill                  () const { return _seckill                  ; }
    inline const std::map<int, cpf_val_2>& get_seckill_hp               () const { return _seckill_hp               ; }
    inline const std::map<int, cpf_val_2>& get_effect_correction_i      () const { return _effect_correction_i      ; }
    inline uint32                          get_effect_correction_f      () const { return _effect_correction_f      ; }
    inline uint32                          get_next_trigger_effect_id         () const { return _next_trigger_effect_id         ; }
    inline uint32                          get_next_trigger_effect_count      () const { return _next_trigger_effect_count      ; }
    inline uint32                          get_next_trigger_effect_probability() const { return _next_trigger_effect_probability; }
    inline uint32                          get_next_trigger_effect_object     () const { return _next_trigger_effect_object     ; }

public:
    inline const uint32              get_fl                      () const { return this->_fl; }
    inline const bool                is_buff                     () const { return this->get_is_buff() == 1; }
    inline const bool                is_scene_buff               () const { return this->get_is_scene_buff() == ESB_SCENE_BUFF; }
    inline const uint32              effect_code                 () const { return this->get_freeze_list().size() == 0 ? 1 : 2; }
    inline const BUFFER_TYPE_E       buff_type                   () const { return this->get_duration_tick() < (uint32)-1 ? BTE_BUFFER_TIME : BTE_BUFFER_FORVER; }
    inline const bool                is_death_remove             () const { return this->get_offline_death_remove() == 1; }
    inline const bool                is_move                     () const { return this->get_is_move() == 1 && (this->get_move_type() == 0 || this->get_move_type() == 6); }
    inline const bool                is_flash                    () const { return this->get_is_move() == 1 && this->get_move_type() == 1; }
    inline const bool                is_swap                     () const { return this->get_is_move() == 1 && this->get_move_type() == 2; }
    inline const bool                is_summon                   () const { return this->get_is_summon() == 1; }
    inline const config_skill*       get_next_trigger_skill      () const { return this->_next_trigger_skill; }
    inline const bool                is_taunt                    () const { return this->get_taunt() == 1; }
    inline const bool                is_seckill                  () const { return this->get_seckill() == 1; }
    inline const bool                is_clear_buff               () const { return this->_is_clear_buff != 0; }
    inline       bool                is_halo                     () const { return _halo == 2;                     }
    inline const std::vector<const config_effect*>& get_halo_effect    () const { static std::vector<const config_effect*> i; return i; }
    inline       bool                is_dismount                 () const { return _is_Dismount == 2;         }

public:
    uint32                   _key                             ;
    uint32                   _effect_id                       ;
    uint32                   _method_id                       ;
    string                   _method_name                     ;
    uint32                   _levels                          ;
    std::vector<int>         _effect_object_type              ;
    uint32                   _is_buff                         ;
    uint32                   _is_scene_buff                   ;
    uint32                   _buff_probability                ;
    uint32                   _buff_useful_type                ;
    std::vector<int>         _broadcast_type                  ;
    uint32                   _next_trigger_skill_id           ;
    uint32                   _next_trigger_count              ;
    uint32                   _next_triggert_probability       ;
    uint32                   _duration_tick                   ;
    uint32                   _interval_tick                   ;
    uint32                   _repeat_type                     ;
    uint32                   _repeat_max                      ;
    uint32                   _repeat_levels                   ;
    std::vector<int>         _trigger_remove                  ;
    int32                    _durable_max                     ;
    cpf_val_2                _each_bear_harm                  ;
    int32                    _over_back_proportion            ;
    int32                    _over_back_max                   ;
    cpf_val_2                _affect_hp                       ;
    cpf_val_2                _affect_mp                       ;
    std::vector<int>         _immunity_list                   ;
    uint32                   _monster_hide                    ;
    uint32                   _player_hide                     ;
    std::vector<int>         _freeze_list                     ;
    uint32                   _affect_param                    ;
    uint32                   _affect_proportion               ;
    std::map<int, cpf_val_2> _affect_list                     ;
    uint32                   _be_cleared                      ;
    uint32                   _offline_death_remove            ;
    uint32                   _offline_timer                   ;
    uint32                   _owner_offline_death_remove      ;
    uint32                   _share_troop                     ;
    uint32                   _harm_check                      ;
    uint32                   _is_clear_buff                   ;
    uint32                   _clear_buff_probability          ;
    uint32                   _clear_useful_buff_level         ;
    uint32                   _clear_harmful_buff_level        ;
    std::vector<int>         _clear_buff_list                 ;
    uint32                   _clear_useful_anti_buff_level    ;
    uint32                   _clear_harmful_anti_buff_level   ;
    uint32                   _is_rebirth                      ;
    uint32                   _rebirth_probability             ;
    uint32                   _rebirth_hp                      ;
    uint32                   _rebirth_mp                      ;
    uint32                   _is_move                         ;
    uint32                   _move_probability                ;
    uint32                   _move_type                       ;
    uint32                   _move_range                      ;
    uint32                   _move_speed                      ;
    uint32                   _is_summon                       ;
    uint32                   _summon_probability              ;
    std::vector<int>         _summon_monster_list             ;
    uint32                   _damage_add                      ;
    uint32                   _damage_suck                     ;
    uint32                   _is_more_income                  ;
    uint32                   _more_income                     ;
    uint32                   _immunity_probability            ;
    std::vector<int>         _immunity_buffer                 ;
    uint32                   _rebound_count                   ;
    uint32                   _rebound_proportion              ;
    uint32                   _icon_mini_ui                    ;
    uint32                   _icon_ui                         ;
    uint32                   _icon_tips                       ;
    uint32                   _effect_shape                    ;
    uint32                   _tips                            ;
    uint32                   _add_buff_shape                  ;
    uint32                   _add_buff_msg                    ;
    uint32                   _remove_buff_shape               ;
    uint32                   _remove_buff_msg                 ;
    uint32                   _buff_type                       ;
    uint32                   _buff_pool_value                 ;
    uint32                   _is_Dismount                     ;
    uint32                   _buff_show_in_panel              ;
    uint32                   _buff_show_in_time               ;
    uint32                   _range_affect                    ;
    uint32                   _more_damage                     ;
    uint32                   _buff_effect_id                  ;
    uint32                   _harm_trigger_skill_id           ;
    uint32                   _affect_type                     ;
    uint32                   _immunity_freeze                 ;
    uint32                   _immunity_affect                 ;
    uint32                   _is_shift_add                    ;
    uint32                   _is_shift_del                    ;
    uint32                   _call_monster_performance        ;
    uint32                   _call_monster_round_radius       ;
    uint32                   _repel_point                     ;
    uint32					 _upgrade_need_level		      ;
	uint32					 _upgrade_effect_id				  ;
    uint32					 _upgrade_need_money		      ;
    std::vector<int>         _effect_type_id                  ;
    uint32                   _out_raid_clear_buff             ;
    uint32                   _taunt                           ;
    uint32                   _taunt_time                      ;
    uint32                   _seckill                         ;
    std::map<int, cpf_val_2> _seckill_hp                      ;
    std::map<int, cpf_val_2> _effect_correction_i             ;
    uint32                   _effect_correction_f             ;
    uint32                   _halo                            ;
    uint32                   _next_trigger_effect_id          ;
    uint32                   _next_trigger_effect_count       ;
    uint32                   _next_trigger_effect_probability ;
    uint32                   _next_trigger_effect_object      ;

public:
    uint32              _fl;
    const config_skill* _next_trigger_skill;
};

class config_effect_mgr : public config_base_mgr
{
public:
    config_effect_mgr(TTableGroup_s* p = NULL);
    virtual ~config_effect_mgr(void);
    
public:
    virtual int init(TTableGroup_s* p);
    int init(TTableGroup_s* p, const char* table_name = "t_effect");
    int clear();

public:
    const TTable* get_table() const { return _table; }
    const std::map<uint32, config_effect*>& get_all_effect() const { return _cb; }
    const config_effect* get_config_effect(uint32 id, uint32 level = 1);
    const config_effect* get_config_effect_by_key(uint32 key, uint32 level = 1);

public:
    void quick_init(TTableGroup_s* effect, TTableGroup_s* role_effect);
    void quick_load(TTable* table, uint32 source);
    const config_effect* quick_read_by_id(uint32 id);
    const config_effect* quick_read_by_key(uint32 key, uint32 level);
    const config_effect* quick_read(TTable* table, uint32 id);

public:
    void set_table_group(TTableGroup_s* effect, TTableGroup_s* role_effect);

private:
    const config_effect* read_effect(uint32 id);
    const config_effect* load_effect(TTable* table, uint32 id, uint32 source);


private:
    TTableGroup_s* _effect;
    TTableGroup_s* _role_effect;
    TTable*        _table;
    std::map<uint32, config_effect*>      _cb;
    std::multimap<uint32, config_effect*> _ces;

    std::map<uint32, table_index>      _quick_by_id;
    std::multimap<uint32, table_index> _quick_by_key;
    int _lock;
};

#endif // __config_effect_H__
