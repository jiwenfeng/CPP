#ifndef __CONFIG_GEM_ACTIVITY_H__
#define __CONFIG_GEM_ACTIVITY_H__

#include "cfg_cache_mgr.h"
#include "cfg_parser.h"
#include "config_base.h"

class config_gem
{
public:
    config_gem();
    ~config_gem();

public:
	// GET
	inline uint32                              get_gem_type()					const { return gem_type					;}
	inline uint32                              get_gem_lv()						const { return gem_lv					;}
	inline const std::map<int, int>&		   get_game_attr()					const { return game_attr				;}
	inline uint32                              get_up_exp()						const { return up_exp					;}
	inline const std::map<int, succinct_info>& get_add_exp_list()				const { return add_exp_list				;}
	inline const std::vector<int>&			   get_upgrade_goods()				const { return upgrade_goods			;}
	inline uint32                              get_upgrade_id()					const { return upgrade_id				;}
	inline uint32                              get_need_count()					const { return need_count				;}
	inline const std::vector<int>&			   get_using_component()			const { return using_component			;}
public:

public:
	// 模版表数据
	uint32						 gem_type					; // 宝石的类型
	uint32                       gem_lv                     ; // 品阶
	std::map<int,int>			 game_attr					; // 增加的属性序列
	uint32						 up_exp						; // 升级所需总经验
	std::map<int, succinct_info> add_exp_list				; // 升级一次获得经验值(格式：经验_概率_暴击类型；经验_概率_暴击类型；经验_概率_暴击类型，根据经验对应获取的概率从中选中一个，获取概率为万分比）1=小暴击，2=中暴击，3=大暴击
	std::vector<int>				 upgrade_goods				; // 升级消耗道具（优先消耗此道具，此道具等于任意一类，并且可与其他类型组合消耗）格式：物品ID;消耗序号_物品ID
	uint32                       upgrade_id                 ; // 升级后宝石ID
	uint32                       need_count                 ; // 升级道具消耗数量
	std::vector<int>			 using_component		    ; // 可镶嵌部件（根据类型：1=武器、2=衣服、3=腰带、4=头盔、5=项链、6=鞋子）配置格式：类型;类型;类型
};

class config_gem_mgr : public config_base_mgr
{
public:
    config_gem_mgr(TTableGroup_s* p = NULL);
    ~config_gem_mgr();

public:
    int                 init(TTableGroup_s* p);
    int                 clear();

public:
    const TTable*       get_table() const { return _table; }
    const config_gem*   get_config_gem(uint32 gem_type,uint32 gem_level) const;

private:
    TTableGroup_s*                          _gem;
    TTable*                                 _table;
    std::multimap<uint32, config_gem*>      _cp;
};

#endif // __CONFIG_MALL_H__
