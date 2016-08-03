#ifndef __CONFIG_TREASURE_H__
#define __CONFIG_TREASURE_H__

#include "cfg_cache_mgr.h"
#include "cfg_parser.h"
#include "config_base.h"

class config_treasure
{
public:
    config_treasure();
    ~config_treasure();

public:
	// GET
	inline uint32                              get_key_id			    () const {return key_id			     ;}
	inline uint32                              get_item_idx			    () const {return item_idx			 ;}
	inline uint32                              get_amount_max_today		() const {return amount_max_today	 ;}
	inline uint32                              get_probability			() const {return probability		 ;}
	inline uint32                              get_single_price			() const {return single_price		 ;}
	inline uint32                              get_level_min			() const {return level_min			 ;}
	inline uint32                              get_level_max			() const {return level_max			 ;}
	inline uint32                              get_min_quantity			() const {return min_quantity		 ;}
	inline uint32                              get_max_quantity			() const {return max_quantity		 ;}
public:

public:
	// 模版表数据
	uint32						  key_id					   ; // 抽奖key值
	uint32						  item_idx                     ; // 物品ID
	uint32						  amount_max_today			   ; // 当天最大抽取数量
	uint32						  probability				   ; // 物品互斥几率
	uint32						  single_price				   ; // 物品单价（熔炼值）
	uint32						  level_min					   ; // 玩家大于等级此等级
	uint32						  level_max					   ; // 玩家小于等于此等级
	uint32						  min_quantity				   ; // 物品数量区间最小值
	uint32						  max_quantity			       ; // 物品数量区间最大值
};

class config_treasure_mgr : public config_base_mgr
{
public:
    config_treasure_mgr(TTableGroup_s* p = NULL);
    ~config_treasure_mgr();

public:
    int                 init(TTableGroup_s* p);
    int                 clear();

public:
    const TTable*       get_table() const { return _table; }
    const config_treasure* get_config_treasure(uint32 key_id) const;
	const std::map<uint32, config_treasure*>& get_config_treasure_map() const;
private:
    TTableGroup_s*                           _treasure;
    TTable*                                 _table;
    std::map<uint32, config_treasure*>		_cp;
};

#endif // __CONFIG_TREASURE_H__
