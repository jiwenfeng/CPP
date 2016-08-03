#ifndef __CONFIG_ACTIVITY_H__
#define __CONFIG_ACTIVITY_H__

#include "cfg_cache_mgr.h"
#include "cfg_parser.h"
#include "config_base.h"

// 轮询活动表
class config_activity
{
public:
    config_activity();
    ~config_activity();

public:
	// GET
	inline uint32                              get_activity_id()		const { return activity_id		;}
	inline uint32                              get_open_type()		    const { return open_type		;}
	inline uint32                              get_interval_day()		const { return interval_day		;}
	inline uint32                              get_start_year()		    const { return start_year		;}
	inline uint32                              get_start_month()		const { return start_month		;}
	inline uint32                              get_start_day()		    const { return start_day		;}
	inline uint32                              get_start_hour()		    const { return start_hour		;}
	inline uint32                              get_start_minute()		const { return start_minute		;}
	inline uint32                              get_start_second()		const { return start_second		;}
	inline uint32                              get_last_time()		    const { return last_time		;}
	inline uint32                              get_fixed_day()		    const { return fixed_day		;}
	inline uint32                              get_repeat_time()		const { return repeat_time		;}

public:

public:
	// 模版表数据
	uint32                       activity_id                     ; // 活动ID
	uint32                       open_type                       ; // 开启类型
	uint32                       interval_day                    ; // 开区、合区第几天开启
	uint32                       start_year                      ; // 指定开启年
	uint32                       start_month                     ; // 指定开启月
	uint32                       start_day                       ; // 指定开启日
	uint32						 start_hour						 ; // 指定开启时
	uint32						 start_minute					 ; // 指定开启分
	uint32						 start_second					 ; // 指定开启秒
	uint32                       last_time                       ; // 活动持续时间（秒）
	uint32                       fixed_day                       ; // 指定星期几（1为星期一，7为星期天）
	uint32						 repeat_time					 ; // 循环间隔时间（秒），配置0为不循环，上一轮活动结束后算起
};

class config_activity_mgr : public config_base_mgr
{
public:
    config_activity_mgr(TTableGroup_s* p = NULL);
    ~config_activity_mgr();

public:
    int                 init(TTableGroup_s* p);
    int                 clear();

public:
    const TTable*       get_table() const { return _table; }
    const config_activity* get_config_activity(uint32 activity_id) const;
	const std::map<uint32, config_activity*>& get_config_activity_list() const;

private:
    TTableGroup_s*                            _activity;
    TTable*                                   _table;
    std::map<uint32, config_activity*>        _cp;
};

#endif // __CONFIG_MALL_H__
