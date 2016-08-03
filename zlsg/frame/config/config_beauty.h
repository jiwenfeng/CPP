#ifndef __CONFIG_BEAUTY_H__
#define __CONFIG_BEAUTY_H__

#include "cfg_cache_mgr.h"
#include "cfg_parser.h"
#include "config_base.h"


class config_beauty
{
public:
    config_beauty();
    ~config_beauty();

public:
	// GET
	inline uint32								get_general_id				() const { return general_id		    ;}
	inline string								get_general_name			() const { return general_name		    ;}
	inline uint32								get_general_level			() const { return general_level		    ;}
	inline uint32								get_master_level			() const { return master_level		    ;}
	inline uint32								get_up_need_exp				() const { return up_need_exp		    ;}
	inline const std::map<int32,int32>&			get_recruit_need_item       () const { return recruit_need_item		;}
	inline const std::map<int32,int32>&			get_fit_need_item			() const { return fit_need_item         ;}
	inline const std::map<int32,int32>&			get_fit_add_property		() const { return fit_add_property      ;}
	inline uint32								get_fight_capacity			() const { return fight_capacity		;}
	inline uint32								get_fight_hppercent			() const { return fight_hppercent		;}
	inline uint32								get_fight_atkpercent		() const { return fight_atkpercent		;}
	inline uint32								get_fight_defpercent		() const { return fight_defpercent		;}
	inline const std::vector<int>&				get_general_skill			() const { return general_skill			;}
	inline uint32								get_magic_id				() const { return magic_id				;}
	inline uint32								get_recruit_level			() const { return recruit_level			;}
	inline uint32								get_skill_belong			() const { return skill_belong			;}
	inline uint32								get_fit_times				() const { return fit_times				;}
	inline const std::map<int32,int32>&			get_game_attribute		    () const { return game_attribute        ;}
public:

public:
	// 模版表数据
	uint32                                      general_id                ;// 武将ID
	string										general_name			  ;// 美人名字
	uint32										general_level			  ;// 武将等级
	uint32										master_level			  ;// 升级所需主人等级
	uint32										up_need_exp				  ;// 升级所需经验
	std::map<int32,int32>						recruit_need_item		  ;// 招募消耗道具（道具ID|数量）
	std::map<int32,int32>						fit_need_item			  ;// 合体消耗物品及数量(物品ID_数量;物品ID_数量;)
	std::map<int32,int32>						fit_add_property		  ;// 本次合体增加主人属性(属性_增加值：职业;属性_增加值;)
	uint32										fight_capacity            ;// 战斗力数值
	uint32										fight_hppercent           ;// 升级拥有主人生命属性万分比
	uint32										fight_atkpercent          ;// 升级拥有主人攻击属性万分比
	uint32										fight_defpercent          ;// 升级拥有主人防御属性万分比
	std::vector<int>							general_skill			  ;// 美人携带技能【技能ID；技能ID】
	uint32										magic_id				  ;// 美人神兵ID
	uint32										recruit_level			  ;// 招募美人所需主人的等级限制
	uint32										skill_belong		      ;// 美人技能归属ID(11=祝融，12=蔡文姬，13=黄月英，14=孙尚香，15=大乔，16=小乔，17=甄姬，18=貂蝉)
	uint32										fit_times				  ;// 已缠绵次数（后端判断缠绵消耗）
	std::map<int32,int32>						game_attribute		      ;// 美人属性
};

class config_beauty_mgr : public config_base_mgr
{
public:
    config_beauty_mgr(TTableGroup_s* p = NULL);
    ~config_beauty_mgr();

public:
    int                 init(TTableGroup_s* p);
    int                 clear();

public:
    const TTable*       get_table() const { return _table; }
	bool  check_is_have(uint32 id,uint32 level);
    const config_beauty* get_config_beauty(uint32 general_id,uint32 general_level) const;
	const config_beauty* get_config_beauty_by_lingering_count(uint32 general_id,uint32 lingering_count) const;
private:
    TTableGroup_s*                            _beauty;
    TTable*                                 _table;
    std::multimap<uint32, config_beauty*>  _cp;
};

#endif // __CONFIG_UPEQUIP_H__
