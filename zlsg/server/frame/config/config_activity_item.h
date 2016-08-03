#ifndef __CONFIG_ACTIVITY_ITEM_H__
#define __CONFIG_ACTIVITY_ITEM_H__

#include "cfg_cache_mgr.h"
#include "cfg_parser.h"
#include "config_base.h"

class config_activity_item
{
public:
    config_activity_item();
    ~config_activity_item();

public:
	// GET
	inline uint32                              get_activity_type()		const { return activity_type		;}
	inline uint32                              get_activity_id()		const { return activity_id		    ;}
	inline uint32                              get_item_order()		    const { return item_order		    ;}
	inline const std::map<int, int>&		   get_a_conditon()		    const { return a_conditon		    ;}
	inline uint32							   get_get_item()		    const { return get_item		        ;}
	//inline uint32                              get_agio_item()		    const { return agio_item			;}
	inline const std::map<int, int>&		   get_buy_times()		    const { return buy_times		    ;}
	inline uint32                              get_is_reset()		    const { return is_reset				;}
	inline const std::map<int, int>&		   get_b_item_price2()		const { return b_item_price2		;}
	inline uint32                              get_start_time()		    const { return start_time			;}
	inline uint32                              get_pass_time()		    const { return pass_time			;}
	inline uint32                              get_equip_order()		const { return equip_order		    ;}

public:

public:
	// 模版表数据
	uint32                       activity_type                    ; // 编号
	uint32				         activity_id					  ; // 关联活动ID（1=坐骑丹领取；2=坐骑丹折扣；）
	uint32						 item_order						  ; // 物品在该活动中的顺序(程序识别顺序用)
	std::map<int,int>			 a_conditon						  ; // 领取条件（条件ID|阶级;）【1=坐骑阶数】
	uint32						 get_item						  ; // 领取物品（物品ID1|数量;ID2|数量;）
	uint32                       agio_item						  ; // 折扣礼包ID（物品ID）
	std::map<int,int>			 buy_times						  ; // 可购买礼包次数（对象ID|数量；）【1=个人；2=单服】购买活动用，不填表示没次数限制
	uint32					     is_reset						  ; // 是否每日重置（0不重置；1重置）
	std::map<int,int>			 b_item_price2					  ; // 折扣活动礼包打折价格（元宝）兑换所需的东西
	uint32						 start_time						  ; // 活动区间下限（距离开服时间X秒）
	uint32						 pass_time						  ; // 活动区间上限（距离开服时间X秒）
	uint32						 equip_order					  ; // 锻造排行榜排名
};

class config_activity_item_mgr : public config_base_mgr
{
public:
    config_activity_item_mgr(TTableGroup_s* p = NULL);
    ~config_activity_item_mgr();

public:
    int                 init(TTableGroup_s* p);
    int                 clear();

public:
    const TTable*				get_table() const { return _table; }
    const config_activity_item* get_config_activity_item(uint32 activity_type) const;
	const uint32 get_act_vec(uint32 activity_id,std::vector<config_activity_item*>& item_vec) const;
	const uint32 get_act_limit_type(uint32 activity_id)const;
private:
    TTableGroup_s*									_act_item;
    TTable*											_table;
    std::map<uint32, config_activity_item*>         _cp;
};

#endif // __CONFIG_MALL_H__
