#ifndef __CONFIG_LAND_H__
#define __CONFIG_LAND_H__

#include "cfg_cache_mgr.h"
#include "cfg_parser.h"
#include "config_base.h"



// 开服奖励配置类
class config_land
{
public:
    config_land();
    ~config_land();

public:
	// GET
	inline uint32                              get_land_id			()const { return land_id				;}
	inline const std::map<int, succinct_info>& get_item_1			()const { return item_1				    ;}
	inline const std::map<int, succinct_info>& get_item_2			()const { return item_2					;}
	inline const std::map<int, succinct_info>& get_item_3			()const { return item_3				    ;}
	inline uint32                              get_yuan_bao			()const { return yuan_bao				;}
	inline uint32                              get_land_money		()const { return land_money				;}
	inline const std::map<int, int>&		   get_land_condition	()const { return land_condition			;}
public:

public:
	// 模版表数据
	uint32                       land_id                     ; // 登陆天数
	std::map<int, succinct_info> item_1						 ; // 登陆奖励物品1（职业ID_道具ID_数量；）【职业ID：0=男战，2=女术，5=全职业】
	std::map<int, succinct_info> item_2						 ; // 物品2（职业ID_道具ID_数量；）【职业ID：0=男战，2=女术，5=全职业】
	std::map<int, succinct_info> item_3						 ; // 物品3（职业ID_道具ID_数量；）【职业ID：0=男战，2=女术，5=全职业】
	uint32						 yuan_bao					 ; // 奖励元宝
	uint32						 land_money					 ; // 奖励铜钱
	std::map<int, int>		     land_condition				 ; //领取条件（条件ID|条件）【条件：1=玩家等级】
};


class config_land_mgr : public config_base_mgr
{
public:
    config_land_mgr(TTableGroup_s* p = NULL);
    ~config_land_mgr();

public:
    int                 init(TTableGroup_s* p);
    int                 clear();

public:
    const TTable*       get_table() const { return _table; }
    const config_land* get_config_land(uint32 land_id) const;

private:
    TTableGroup_s*                            _land;
    TTable*                                   _table;
    std::map<uint32, config_land*>            _cp;
};

#endif // __CONFIG_LAND_H__
