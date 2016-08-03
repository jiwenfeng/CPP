#ifndef __CONFIG_GEM_ACT_H__
#define __CONFIG_GEM_ACT_H__

#include "cfg_cache_mgr.h"
#include "cfg_parser.h"
#include "config_base.h"

class config_gem_act
{
public:
    config_gem_act();
    ~config_gem_act();

public:
	// GET
	inline uint32                              get_gem_act_id()		const { return gem_act_id		;}
	inline uint32                              get_act_type()		const { return act_type			;}
	inline uint32                              get_sequence()		const { return sequence			;}
	inline const std::map<int, int>&		   get_act_cond()		const { return act_cond			;}
	inline const std::map<int, int>&		   get_act_attr()		const { return act_attr			;}
public:

public:
	// 模版表数据
	uint32                       gem_act_id                     ; // 激活ID
	uint32						 act_type						; // 激活类型（1=宝石附加属性，2=宝石镶嵌框，2=宝石技能框）
	uint32						 sequence						; // 序号
	std::map<int,int>			 act_cond						; // 激活条件，格式如：激活条件枚举_数值
	std::map<int,int>			 act_attr						; // 激活属性（格式：属性枚举_属性值;属性枚举_属性值）
};

class config_gem_act_mgr : public config_base_mgr
{
public:
    config_gem_act_mgr(TTableGroup_s* p = NULL);
    ~config_gem_act_mgr();

public:
    int                 init(TTableGroup_s* p);
    int                 clear();

public:
    const TTable*       get_table() const { return _table; }
    const config_gem_act* get_config_gem_act(uint32 gem_act_id) const;
	uint32 get_config_gem_act(uint32 act_type,uint32 cond_value) const;// 
	void get_config_gem_act_attr(uint32 act_type,uint32 cond_value,std::multimap<int,int>& act_attr_map_)const;
	uint32 get_gem_skill_max_count()const;
private:
    TTableGroup_s*                          _gem_act;
    TTable*                                 _table;
    std::map<uint32, config_gem_act*>		_cp;
};

#endif // __CONFIG_MALL_H__
