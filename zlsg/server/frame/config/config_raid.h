#ifndef __CONFIG_RAID_H__
#define __CONFIG_RAID_H__

#include "cfg_cache_mgr.h"
#include "pb_mgr.h"
#include "config_base.h"
#include "cfg_parser.h"

enum RAID_TYPE_E
{
    RAID_NULL            = 10000 ,
    RAID_PUSHFIG         = 10001 ,  // 渡劫推图
    RAID_PET             = 10002 ,  // 宠物副本
    RAID_WEAPON          = 10003 ,  // 仙园
    RAID_PROPS           = 10004 ,  // 合欢谷
    RAID_EXP             = 10005 ,  // 经验副本
    RAID_LEARN           = 10006 ,  // 仙宗战
    RAID_MAZE            = 10007 ,  // 无
    RAID_LAYER           = 10008 ,  // 爬塔副本
    RAID_MONEY           = 10009 ,  // 铜钱副本
    RAID_STONE           = 10010,  // 灵石副本
    RAID_MASTER_CAVE     = 10011,  // 灵脉仙境 
    RAID_FAMILY_LAYER    = 10012,  // 家族副本(爬塔)
    RAID_FAMILY_SCENE    = 10013,  // 家族副本(场景)
    RAID_FAMILY_CONQUEST = 10014,  // 无
    RAID_TEAM_MONEY      = 10015,  // 铜钱副本(组队)
    RAID_FAMILY_FRONTIER = 10016,  // 土豪
    RAID_SIX_ROUND       = 10017,  // 无
    RAID_ROLE_MATCH      = 10018,  // 竞技场
    RAID_ROLE_FIGHT      = 10019,  // 无
    RAID_MAX,                   // 限制
};

enum MAP_TYPE
{
    MAP_TYPE_NORMAL = 1,     // 普通场景
    MAP_TYPE_RAID      ,     // 副本
    MAP_TYPE_BATTLE    ,     // 战场
	MAP_TYPE_WORLD_BOSS,	 // 世界BOSS
	MAP_TYPE_ARENA	   ,	 // 竞技场
	MAP_TYPE_PET	   ,	 // 美人关
	MAP_TYPE_TEAM	   ,	 // 组队副本
	MAP_TYPE_LEVELS	   ,	 // 过关斩将
	MAP_TYPE_CYCLE     ,     // 沙场百战
	MAP_TYPE_DEVIL	   ,	 // 魔化神将
    MAP_TYPE_HOT       ,     // 温泉
};

class config_raid
{
public:
    config_raid() {}
    virtual ~config_raid() { }

public:
    inline const uint32              get_map_id                 () const { return _map_id                 ; }
    inline const uint32              get_map_type               () const { return _map_type               ; }
    inline const uint32              get_need_energy            () const { return _need_energy            ; }
	inline const uint32				 get_need_power				() const { return _need_power			  ; }
    inline const uint32              get_d_2_need_energy        () const { return _d_2_need_energy        ; }
	inline const uint32				 get_max_member				() const { return _max_member			  ; }
    inline const uint32              get_standard_time          () const { return _standard_time          ; }
	inline const uint32				 get_enter_lv				() const { return _lv_limit;}
    inline const uint32              get_battle_ui_id           () const { return _battle_ui_id           ; }
    inline const std::vector<int>&   get_reward_item            () const { return _reward_item            ; }
    inline const uint32              get_reward_money           () const { return _reward_money           ; }
    inline const uint32              get_reward_experience      () const { return _reward_experience      ; }
    inline const std::vector<int>&   get_reward_item_2          () const { return _reward_item_2          ; }
    inline const uint32              get_reward_money_2         () const { return _reward_money_2         ; }
    inline const uint32              get_reward_experience_2    () const { return _reward_experience_2    ; }
    inline const uint32              get_reward_tao_lue         () const { return _reward_tao_lue         ; }
    inline const std::string&        get_monster_appeare_script () const { return _monster_appeare_script ; }
    inline const std::string&        get_screen_buff            () const { return _screen_buff            ; }
    inline const std::string&        get_success_condition      () const { return _success_condition      ; }
    inline const std::string&        get_fail_condition         () const { return _fail_condition         ; }
    inline const std::string&        get_district_open_condition() const { return _district_open_condition; }
    inline const std::string&        get_success_condition_2    () const { return _success_condition_2    ; }
    inline const std::string&        get_success_condition_3    () const { return _success_condition_3    ; }
    inline const std::map<int, int>& get_sweep_prop             () const { return _sweep_prop             ; }
    inline const std::map<int, int>& get_sweep_prop_to          () const { return _sweep_prop_to          ; }
	const rect &					 get_revive_rect(uint32 camp)const;
	inline const uint32				 get_map_monster_id() const { return _map_monster_id; }

	inline const map_postion&		 get_monster_postion() const { return _pos; }
    inline const uint32              get_fail_need_energy_1     () const { return _fail_need_energy_1     ; }
    inline const uint32              get_fail_need_energy_2     () const { return _fail_need_energy_2     ; }
	inline const std::vector<exp_region>& get_exp_region() const {return _exp_region; }
	inline const uint32				 get_ex_map_id				() const { return _ex_map_id;				}
	inline const uint32				 get_father_map_id			() const { return _father_map_id		  ; }
	inline const uint32				 get_map_common_line		() const { return _map_common_line		  ; }

public:
    inline bool is_raid() const { return get_map_type() == MAP_TYPE_RAID; }
	inline bool is_normal() const{ return get_map_type() == MAP_TYPE_NORMAL; }
	inline bool is_battle() const {return get_map_type() == MAP_TYPE_BATTLE ;}
    inline const vector<int>& get_evaluation() const { return _evaluation; }
	const rect &get_revive_region(uint32 camp);

public:
    uint32             _map_id                 ;
    uint32             _map_type               ;
    uint32             _need_energy            ;
	uint32			   _max_member			   ;
	uint32			   _need_power			   ;
    uint32             _d_2_need_energy        ;
    uint32             _standard_time          ;
    uint32             _battle_ui_id           ;
    uint32             _map_monster_id         ;
	uint32			   _father_map_id		   ;
    std::vector<int>   _reward_item            ;
    uint32             _reward_money           ;
    uint32             _reward_experience      ;
    std::vector<int>   _reward_item_2          ;
    uint32             _reward_money_2         ;
    uint32             _reward_experience_2    ;
    uint32             _reward_tao_lue         ;
    std::string        _monster_appeare_script ;
    std::string        _screen_buff            ;
    std::string        _success_condition      ;
    std::string        _fail_condition         ;
    std::string        _district_open_condition;
    std::vector<int>   _evaluation             ;
    std::string        _success_condition_2    ;
    std::string        _success_condition_3    ;
    std::map<int, int> _sweep_prop             ;
    std::map<int, int> _sweep_prop_to          ;
    rect			   _revive_region_1		   ;
    rect			   _revive_region_2		   ;
    rect			   _revive_region_3		   ;
    uint32			   _lv_limit			   ;
    map_postion		   _pos					   ;
    uint32             _fail_need_energy_1     ;
    uint32             _fail_need_energy_2     ;
	std::vector<exp_region> _exp_region		   ;
	uint32			   _ex_map_id			   ;
	uint32			   _map_common_line		   ;
};

class config_raid_mgr : public config_base_mgr
{
public:
    config_raid_mgr(TTableGroup_s* p = NULL);
    ~config_raid_mgr();

public:
    int                init(TTableGroup_s* p);
    int                clear();

public:
    const TTable*      get_table() const { return _table; }
    const config_raid* get_config_raid(uint32 raid_id) const;
	const rect &get_revive_rect(uint32 raid_id, uint32 camp) const;
	const std::vector<exp_region>& get_exp_region(uint32 raid_id) const;

public:
	void format_rect(const string & str, rect &r);
	void format_pos(const string &str, map_postion &pos);
	bool is_normal(uint32 id) const;
	bool is_battle(uint32 id) const;

public:
    void insert(config_raid* pcr);

private:
    TTableGroup_s*                     _raid;
    TTable*                          _table;
    std::map<uint32, config_raid*>   _cr;
};

#endif
