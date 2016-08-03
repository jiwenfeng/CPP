#ifndef __CONFIG_MALL_H__
#define __CONFIG_MALL_H__

#include "cfg_cache_mgr.h"
#include "cfg_parser.h"
#include "config_base.h"

class config_mall
{
public:
    config_mall();
    ~config_mall();

public:
	// GET
	inline uint32                              get_mall_id()		const { return mall_id		;}
	inline uint32                              get_item_id()		const { return item_id		;}
	inline int								   get_pay_type()		const { return pay_type		;}
	inline int								   get_price_value()	const { return price_value	;}
	inline const std::map<int, int>&		   get_extra_limit()    const { return extra_limit	;}
	inline uint32                              get_daily_quota()	const { return daily_quota	;}
public:

public:
	// 模版表数据
	uint32                       mall_id                     ; // 商店ID
	uint32						 item_id				     ; // 物品ID
	int					         pay_type					 ; // 支付类型
	int							 price_value				 ; // 价格
	std::map<int,int>            extra_limit			     ; // 购买限制条件
	uint32						 daily_quota				 ; // 每日限购次数
};

class config_mall_mgr : public config_base_mgr
{
public:
    config_mall_mgr(TTableGroup_s* p = NULL);
    ~config_mall_mgr();

public:
    int                 init(TTableGroup_s* p);
    int                 clear();

public:
    const TTable*       get_table() const { return _table; }
    const config_mall* get_config_mall(uint32 mall_id,uint32 item_id) const;

private:
    TTableGroup_s*                            _mall;
    TTable*                                 _table;
    std::multimap<uint32, config_mall*>     _cp;
};

#endif // __CONFIG_MALL_H__
