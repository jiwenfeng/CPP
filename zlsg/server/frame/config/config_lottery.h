#ifndef __CONFIG_HUNT_H__
#define __CONFIG_HUNT_H__

// 宝箱寻宝
#include "cfg_cache_mgr.h"
#include "cfg_parser.h"
#include "config_base.h"

class config_lottery
{
public:
    config_lottery();
    ~config_lottery();
	
public:
	inline const uint32 get_lottery_id() const { return _lottery_id; }
	inline const uint32 get_lottery_prop_id()	 const { return _prop_id; }
	inline const uint32 get_lottery_type() const { return _type; }
	inline const uint32 get_lottery_item_type()	 const { return _item_type; }
	inline const uint32 get_lottery_max() const { return _max; }
	inline const uint32 get_lottery_probability() const { return _probability;}
	inline const uint32 get_lottery_min_lvl() const { return _min_lvl; }
	inline const uint32 get_lottery_max_lvl() const { return _max_lvl; }
	inline const uint32 is_group() const { return _is_group; }
	inline const uint32 get_job() const { return _job; }

public:
	uint32 _lottery_id;					// 
	uint32 _prop_id;					// 物品ID
	uint32 _type;						// 寻宝类型
	uint32 _item_type;					// 橙色装备标示
	uint32 _max;						// 当天最大抽取数量
	uint32 _probability;				// 物品互斥几率
	uint32 _min_lvl;					// 限制最小玩家等级
	uint32 _max_lvl;					// 限制最大玩家等级
	uint32 _is_group;					// 是否掉落组
	uint32 _job;						// 所属职业
};

class config_lottery_mgr : public config_base_mgr
{
public:
    config_lottery_mgr(TTableGroup_s* p = NULL);
    ~config_lottery_mgr();
	const std::vector<uint32> &get_config_lottery_list(uint32 type) const;
	const config_lottery *get_config_lottery(uint32 id);

public:
    int                 init(TTableGroup_s* p);
    int                 clear();

public:
    const TTable*       get_table() const { return _table; }

private:
    TTableGroup_s*						_lottery;
    TTable*								_table;
	map<uint32, config_lottery *>		_list;
	std::vector<uint32>					_gold;
	std::vector<uint32>					_bronze;
};

#endif // __CONFIG_HUNT_H__
