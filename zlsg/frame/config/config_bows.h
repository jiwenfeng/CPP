#ifndef __CONFIG_BOWS_H__
#define __CONFIG_BOWS_H__

#include "cfg_cache_mgr.h"
#include "cfg_parser.h"
#include "config_base.h"

class config_bows_base
{
public:
    inline int                     get_bow_id				 () const { return _bow_id				  ; }
    inline int                     get_max_skill_level       () const { return _max_skill_level       ; }
    inline const std::vector<int>& get_advance_need_item     () const { return _advance_need_item     ; }
    inline int                     get_advance_need_item_num () const { return _advance_need_item_num ; }
    inline int                     get_advance_count_min     () const { return _advance_count_min     ; }
    inline int                     get_advance_count_max     () const { return _advance_count_max     ; }
    inline int                     get_advance_ratio         () const { return _advance_ratio         ; }
    inline int                     get_advance_fail_bless_min() const { return _advance_fail_bless_min; }
    inline int                     get_advance_fail_belss_max() const { return _advance_fail_belss_max; }
    inline int                     get_advance_bless_max     () const { return _advance_bless_max     ; }
    inline int                     get_success_broadcast     () const { return _success_broadcast     ; }
    //inline int                     get_monster_id            () const { return _monster_id            ; }
    inline int                     get_advance_bless_time    () const { return _advance_bless_time    ; }
    //inline const std::vector<int>& get_talent_skill          () const { return _talent_skill          ; }
    inline const std::vector<int>& get_upgrade_replace_item  () const { return _upgrade_replace_item  ; }
    inline int                     get_use_item_record       () const { return _use_item_record       ; }
    inline int                     get_next_bow_id			 () const { return _next_bow_id           ; }
    inline int                     get_have_addition         () const { return _have_addition         ; }
    inline int                     get_record_max_value      () const { return _record_max_value      ; }
    inline int                     get_potential_addtion     () const { return _potential_addtion     ; }
	//inline const std::vector<int>& get_skill_id              () const { return _skill_id              ; }
	inline std::string             get_bow_name				 () const { return _bow_name              ; }
public:
    int              _bow_id                ;// 弓箭ID
    int              _advance               ;// 弓箭阶数
    int              _max_skill_level       ;// 本阶弓箭所允许的最大技能数
    std::vector<int> _advance_need_item     ;// 进阶所需消耗材料ID
    int              _advance_need_item_num ;// 每次消耗材料数量
    int              _advance_count_min     ;// 进阶成功所需进阶次数最小值
    int              _advance_count_max     ;// 进阶成功所需进阶次数最大值
    int              _advance_ratio         ;// 服务器端计算用进阶成功几率（万分比）
    int              _advance_fail_bless_min;// 每次失败增加祝福值区间min
    int              _advance_fail_belss_max;// 每次失败增加祝福值区间max
    int              _advance_bless_max     ;// 祝福值上限值
    int              _success_broadcast     ;// 进阶成功是否全服公告（0不公告，1公告）
    //int              _monster_id            ;// 关联怪物ID(使用怪物配置信息)
    int              _advance_bless_time    ;// 祝福值清理时间
    //std::vector<int> _talent_skill          ;// 天赋技能(技能id;技能id)
    std::vector<int> _upgrade_replace_item  ;// 升级可使用替代道具列表(itemid;itemid)
    int              _use_item_record       ;// 幻化丹潜能丹的使用次数限制(：类型1|物品id1_限制数量1;类型2|物品id2_限制数量2)（0=幻化丹；1=潜能丹)
    int              _next_bow_id         ;// 下一阶的坐骑ID
    int              _have_addition         ;// 是否拥有属性加成
    int              _record_max_value      ;// 最大幻化值
    int              _potential_addtion     ;// 潜能加成值
	//std::vector<int> _skill_id              ;// 技能ID
	std::string		 _bow_name			;// 坐骑名
};

class config_bows_attr
{
public:
    inline int                       get_bow_id      () const { return _bow_id      ; }
    //inline int                       get_player_job    () const { return _player_job    ; }
    inline int                       get_bow_level   () const { return _bow_level   ; }
    inline const std::map<int, int>& get_game_attribute() const { return _game_attribute; }
    inline int                       get_bow_speed () const { return _bow_speed ; }

public:
    int                _bow_id      ;// 弓箭ID
   // int                _player_job    ;// 角色职业类型(每一职业对应不同等级属性不同)
    int                _bow_level   ;// 弓箭等级
    std::map<int, int> _game_attribute;// 属性加成列表
    int				   _bow_speed	;
};

class config_bows
{
public:
    ~config_bows() {
        if (_base != NULL) {
            delete _base;
            _base = NULL;
        }

		std::multimap<int, config_bows_attr*>::iterator it = _attr.begin();
		for (; it != _attr.end(); ++it){
			delete it->second;
		}

		_attr.clear();
    }
public:
    inline void set_base(config_bows_base* base) { _base = base; }
    inline void insert_attr(config_bows_attr* attr) {
        _attr.insert(std::pair<int, config_bows_attr*>(attr->get_bow_level(), attr));
    }
    
public:
    inline const config_bows_base*                                 get_base() const { return _base; }
    inline const std::multimap<int, config_bows_attr*>&		       get_attr() const { return _attr; }
    
public:
    config_bows_base*                                _base;
    std::multimap<int, config_bows_attr*>            _attr;
};

class config_bows_mgr : public config_base_mgr
{
public:
    config_bows_mgr();
    virtual ~config_bows_mgr();

public:
    virtual int init(TTableGroup_s* p);
    int                 init(TTableGroup_s* group_base, TTableGroup_s* group_attr);
    int                 clear();
    const TTable*       get_table() const { return _table; }

public:
    const config_bows*      get_config_bows(uint32 bow_id) const;
    const config_bows*      get_config_bows_min() const;
	const config_bows*      get_config_bows_max() const;
    const config_bows_base* get_config_bows_base(uint32 bow_id) const;
    const config_bows_attr* get_config_bows_attr(uint32 bow_id, uint32 lvl) const;

	float calculate_bow_power(uint32 bow_id,uint32 bow_level) const;

private:
    TTableGroup_s*                          _group;
    TTable*                               _table;
    std::map<int, config_bows*>         _content;
};

#endif // __CONFIG_BOWS_H__
