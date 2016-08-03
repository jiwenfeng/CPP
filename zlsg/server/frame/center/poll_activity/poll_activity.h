#ifndef __POLL_ACTIVITY_H__
#define __POLL_ACTIVITY_H__

#include "define.h"
#include "pb_mgr.h"
#include "pb_writer.h"
#include "config_activity.h"

struct activity_info {
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

	activity_info():activity_id(0),
		open_type(0),interval_day(0),start_year(0),
		start_month(0),start_day(0),start_hour(0),
		start_minute(0),start_second(0),last_time(0),fixed_day(0),repeat_time(0)
	{
		
	}

	activity_info& operator=(config_activity& rhs)
	{
		activity_id = rhs.activity_id;
		open_type = rhs.open_type;
		interval_day = rhs.interval_day;
		start_year = rhs.start_year;
		start_month = rhs.start_month;
		start_day = rhs.start_day;
		start_hour = rhs.start_hour;
		start_minute = rhs.start_minute;
		start_second = rhs.start_second;
		last_time = rhs.last_time;
		fixed_day = rhs.fixed_day;
		repeat_time = rhs.repeat_time;

		return *this;
	}
};

class poll_activity : public pb_writer
{
public:
	poll_activity(mysql::tb_activity_open* activity_open_db = NULL);
	poll_activity(activity_info info);
	virtual ~poll_activity();

	int save(amtsvr_context* ctx);

	int remove();

public:
	void on_process(bool& need_brocast,uint32& back_door_type);

	bool check_activity(uint32& back_door_type);

	void on_open();

	void on_close();

	bool is_same_day();

	uint32 get_remain_time();

	uint32 mut_activity_info(config_activity& info);

public:

	inline bool is_activiting(){ return is_launch_;}

	inline const activity_info get_activity_info(){return _act_info;}

	inline void set_open_type(uint32 open_type){ _db_activity_open->set_open_type(open_type);}

	inline uint32 get_close_time()const{ return _close_time;}

	inline uint32 get_open_time()const{ return _open_time;}

	inline bool get_is_change_time(){ return _is_change_time;}

	inline void set_is_change_time(bool change){ _is_change_time = change; }

	inline void set_activity_id(uint32 act_id){ _act_info.activity_id = act_id;}

public:

	void add_reward_flag();

	void reset_reward_flag();
	// 后台配置数据覆盖配置表数据
	void reset_act_data(uint32 open_type,uint32 start_time,uint32 last_time,uint32 replace_time);

	inline bool is_reward(){return _db_activity_open->reward_flag() == 1;}

private:

	activity_info				_act_info;
	uint32						_open_time;
	uint32						_close_time;
	uint32						_first_open;
	uint32					    _first_close;
	bool						is_launch_;                    // 是否正在进行中`?~
	bool                        _change;
	mysql::tb_activity_open*	_db_activity_open;
	bool						_is_change_time;
	bool						_backdoor_change;			// 后台数据改变需要广播下发
	bool						_first_load;
};

#endif