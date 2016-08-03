#ifndef __CONFIG_SKILL_H__
#define __CONFIG_SKILL_H__

#include "cfg_cache_mgr.h"
#include "config_base.h"

enum SKILL_TYPE {
    ST_INITIATIVE  = 1,
    ST_PASSIVE     = 2,
    ST_ATK_TIRGGER = 3,
    ST_DEF_TIRGGER = 4,
    ST_SWITCH      = 5,
	ST_ATTRIBUTE   = 6,
	ST_BOW		   = 7,
};
    
enum METHOD_SHAPE_TYPE {
    MST_POINT        = 1,
    MST_LINE         = 2,
    MST_CONE         = 3,
    MST_CIRCLE       = 4,
    MST_TRIGEMINAL   = 5,
    MST_POINT_AROUND = 6,
    MST_MOVE_CIRCLE  = 7,
};

enum SKILL_MISS_BLOW {
    SMB_NOT_MISS_BLOW = 0,
    SMB_MISS_BLOW     = 1,
};

enum SKILL_BELONG {
    SB_DEFAULT    = 1,
    SB_MOUNTS     = 2,
    SB_CAVALRY    = 3,
    SB_ARCHER     = 4,// ¹­¼ý
    SB_ANGER      = 5,
    SB_BEAUTY_MIN = 11,
    SB_BEAUTY_MAX = 18,
	SB_GEM		  = 777,// ±¦Ê¯
};

class config_effect;

class config_skill {
public:
    config_skill();
    ~config_skill();

public:
    inline uint32                    get_skill_id                 () const { return _skill_id                 ; }
	inline string                    get_skill_name               () const { return _skill_name               ; }
    //inline uint32                    get_skill_level              () const { return _skill_level              ; }
    inline uint32                    get_skill_type               () const { return _skill_type               ; }
    inline uint32                    get_skill_index              () const { return _skill_index              ; }
    inline uint32                    get_trigger_probability      () const { return _trigger_probability      ; }
    inline uint32                    get_fight_power              () const { return _fight_power              ; }
    //inline uint32                    get_learn_need_levels        () const { return _learn_need_levels        ; }
    inline uint32                    get_learn_need_item_id       () const { return _learn_need_item_id       ; }
    inline uint32                    get_upgrade_need_levels      () const { return _upgrade_need_levels      ; }
    inline uint32                    get_upgrade_need_yuan_li     () const { return _upgrade_need_yuan_li     ; }
    inline uint32                    get_upgrade_need_item_id     () const { return _upgrade_need_item_id     ; }
    inline uint32                    get_upgrade_need_item_num    () const { return _upgrade_need_item_num    ; }
    inline uint32                    get_upgrade_need_game_money  () const { return _upgrade_need_game_money  ; }
    inline uint32                    get_upgrade_probability      () const { return _upgrade_probability      ; }
    inline uint32                    get_upgrade_skill_id         () const { return _upgrade_skill_id         ; }
    inline uint32                    get_upgrade_need_timer       () const { return _upgrade_need_timer       ; }
    inline uint32                    get_upgrade_manual_need_money() const { return _upgrade_manual_need_money; }
    inline uint32                    get_use_limit                () const { return _use_limit                ; }
    inline uint32                    get_repair_block             () const { return _repair_block             ; }
    inline uint32                    get_can_trigger_buff         () const { return _can_trigger_buff         ; }
    inline uint32                    get_consume_mp               () const { return _consume_mp               ; }
    inline uint32                    get_collision_distance       () const { return _collision_distance       ; }
    inline uint32                    get_object_max_level         () const { return _object_max_level         ; }
    inline int                       get_collision_speed_param    () const { return _collision_speed_param    ; }
    inline uint32                    get_cool_tick                () const { return _cool_tick                ; }
    inline uint32                    get_cool_group_id            () const { return _cool_group_id            ; }
    inline uint32                    get_cool_group_tick          () const { return _cool_group_tick          ; }
    inline int                       get_hate_value               () const { return _hate_value               ; }
    inline uint32                    get_singing_tick             () const { return _singing_tick             ; }
    inline uint32                    get_action_tick              () const { return _action_tick              ; }
    inline uint32                    get_is_harm                  () const { return _is_harm                  ; }
    inline uint32                    get_is_death_repel           () const { return _is_death_repel           ; }
    inline uint32                    get_correction               () const { return _correction               ; }
    inline uint32                    get_affect_object_max        () const { return _affect_object_max        ; }
    inline uint32                    get_method_shape             () const { return _method_shape             ; }
    inline uint32                    get_line_length              () const { return _line_length              ; }
    inline uint32                    get_line_width               () const { return _line_width               ; }
    inline uint32                    get_round_radius             () const { return _round_radius             ; }
    inline uint32                    get_arc_angle                () const { return _arc_angle                ; }
    inline uint32                    get_arc_radius               () const { return _arc_radius               ; }
    inline uint32                    get_affect_gain_proportion   () const { return _affect_gain_proportion   ; }
    inline uint32                    get_is_ignore_defense        () const { return _is_ignore_defense        ; }
    inline uint32                    get_miss_blow                () const { return _miss_blow                ; }
    inline uint32                    get_hit_rate                 () const { return _hit_rate                 ; }
	inline uint32                    get_skills_belonging         () const { return _skills_belonging         ; }
    inline const std::vector<int>&   get_need_job                 () const { return _need_job                 ; }
    inline const std::vector<int>&   get_effect_object_type       () const { return _effect_object_type       ; }
    inline const std::vector<int>&   get_method_list              () const { return _method_list              ; }
    inline const std::vector<int>&   get_self_method_list         () const { return _self_method_list         ; }
    inline const std::map<int, int>& get_game_attribute           () const { return _game_attribute           ; }
    inline uint32                    get_attack_time              () const { return _attack_time              ; }
	inline uint32					 get_key					  () const { return _key					  ; }
	inline const std::map<std::string, int>& get_learn_need_levels     () const { return _learn_need_levels        ; }

public:
    inline uint32 get_source() const { return _source; }

public:
    uint32 get_distance() const;
    inline uint32 get_flash   () const { return true; }
    inline const std::vector<const config_effect*> get_effects() const { return _effects; }
    inline void get_next_skills(std::vector<const config_skill*>& skills) const;

public:
    uint32                     _skill_id                 ;
	string					   _skill_name				 ;						
    uint32                     _skill_level              ;
    uint32                     _skill_type               ;
    uint32                     _skill_index              ;
    uint32                     _trigger_probability      ;
    uint32                     _fight_power              ;
    //uint32                     _learn_need_levels        ;
    uint32                     _learn_need_item_id       ;
    uint32                     _upgrade_need_levels      ;
    uint32                     _upgrade_need_yuan_li     ;
    uint32                     _upgrade_need_item_id     ;
    uint32                     _upgrade_need_item_num    ;
    uint32                     _upgrade_need_game_money  ;
    uint32                     _upgrade_probability      ;
    uint32                     _upgrade_skill_id         ;
    uint32                     _upgrade_need_timer       ;
    uint32                     _upgrade_manual_need_money;
    uint32                     _use_limit                ;
    uint32                     _repair_block             ;
    uint32                     _can_trigger_buff         ;
    uint32                     _consume_mp               ;
    uint32                     _collision_distance       ;
    uint32                     _object_max_level         ;
    int                        _collision_speed_param    ;
    uint32                     _cool_tick                ;
    uint32                     _cool_group_id            ;
    uint32                     _cool_group_tick          ;
    int                        _hate_value               ;
    uint32                     _singing_tick             ;
    uint32                     _action_tick              ;
    uint32                     _is_harm                  ;
    uint32                     _is_death_repel           ;
    uint32                     _correction               ;
    uint32                     _affect_object_max        ;
    uint32                     _method_shape             ;
    uint32                     _line_length              ;
    uint32                     _line_width               ;
    uint32                     _round_radius             ;
    uint32                     _arc_angle                ;
    uint32                     _arc_radius               ;
    uint32                     _affect_gain_proportion   ;
    uint32                     _is_ignore_defense        ;
    uint32                     _miss_blow                ;
    uint32                     _hit_rate                 ;
	uint32                     _skills_belonging         ;
    std::vector<int>           _need_job                 ;
    std::vector<int>           _effect_object_type       ;
    std::vector<int>           _method_list              ;
    std::vector<int>           _self_method_list         ;
    std::map<int, int>         _game_attribute           ;
    uint32                     _attack_time              ;
	uint32			           _key						 ;
	std::map<std::string, int> _learn_need_levels        ;

public:
    uint32                     _source;

public:
    std::vector<const config_effect*> _effects;
};

class config_effect_mgr;

class config_skill_mgr : public config_base_mgr
{
public:
    config_skill_mgr(TTableGroup_s* p = NULL);
    virtual ~config_skill_mgr();

public:
    virtual int init(TTableGroup_s* p);
    int init(TTableGroup_s* p, const char* table_name = "t_skill");
    int link_effects(config_effect_mgr* pcem);
    int clear();

public:
    const TTable* get_table() const { return _table; }
    const config_skill* get_config_skill(uint32 skill_id);
    void get_config_skill_by_job(uint32 job, uint32 level, std::vector<const config_skill*>& cs);

public:
    void quick_init(TTableGroup_s* skill, TTableGroup_s* skill_role);
    void quick_load(TTable* table, uint32 source);
    const config_skill* quick_read_by_id(uint32 id);
    void quick_read_by_job(uint32 job, uint32 level, std::vector<const config_skill*>& cs);
    const config_skill* quick_read(TTable* table, uint32 index, uint32 source, uint32 container);

private:
    void set_table_group(TTableGroup_s* skill, TTableGroup_s* role_skill);
    const config_skill* read_skill(uint32 skill_id);
    const config_skill* load_skill(TTable* table, uint32 skill_id, uint32 source);

private:
    TTableGroup_s* _skill;
    TTableGroup_s* _skill_role;
    TTable*        _table;
    std::map<uint32, config_skill*> _cs;
    std::multimap<uint32, config_skill*> _mcs;

    std::map<uint32, table_index>      _quick_by_id;
    std::multimap<uint32, table_index> _quick_by_job;
    int _lock;
};

#endif // __CONFIG_SKILL_H__
