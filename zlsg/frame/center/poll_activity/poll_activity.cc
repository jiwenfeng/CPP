#include "poll_activity.h"
#include "base_time.h"

poll_activity::poll_activity(activity_info info): _act_info(info),_change(false)
{
	is_launch_ = false;
	_open_time = 0;
	_close_time = 0;
	_first_open = 0;
	_first_close = 0;
	_is_change_time = false;
	_backdoor_change = false;
	_first_load = false;
	if (_act_info.activity_id != 0){
		_db_activity_open = new mysql::tb_activity_open;
		_db_activity_open->set_activity_id(_act_info.activity_id);
		_db_activity_open->set_open_type(BACKGROUND_ACTIVITY_OPEN_TYPE_OPEN);
		_db_activity_open->set_reward_flag(0);
		writer_type = WOTE_INSERT;

		_change = true;
	}
	
}

poll_activity::poll_activity(mysql::tb_activity_open* activity_open_db):_change(false), _db_activity_open(activity_open_db)
{
	is_launch_ = false;
	_is_change_time = false;
	_first_load = false;
	_open_time = 0;
	_close_time = 0;
	_first_open = 0;
	_first_close = 0;

	set_activity_id(activity_open_db->activity_id());
}

poll_activity::~poll_activity()
{
	if (_db_activity_open != NULL) {
		delete _db_activity_open;
		_db_activity_open = NULL;
	}

	is_launch_ = false;
	_open_time = 0;
	_close_time = 0;
	_first_close = 0;
	_first_close = 0;
}

bool poll_activity::check_activity(uint32& back_door_type)
{
	if (back_door_type == BACKGROUND_ACTIVITY_OPEN_TYPE_STOP){
		return false;
	}

	uint32 now = (uint32)time(NULL);
	switch(_act_info.open_type)
	{
	case ACTIVITY_OPEN_TYPE_OPEN:// 新区第几天
		{
			if (!_first_load){
				uint32 start_time = static_cast<uint32>(amtsvr_svr_start_time());
				_open_time = start_time + (_act_info.interval_day - 1)* 24 * 3600;
				_close_time = _open_time + _act_info.last_time;
				_first_open = _open_time;
				_first_close = _close_time;

				_is_change_time = true;
				_first_load = true;
			}
		}
		break;
	case ACTIVITY_OPEN_TYPE_DATE:// 指定年月日
		{
			if (!_first_load){
				_open_time = time_utils::make_time_year(_act_info.start_year,_act_info.start_month,
				_act_info.start_day,_act_info.start_hour,_act_info.start_minute,_act_info.start_second);

				_close_time = _open_time + _act_info.last_time;

				_is_change_time = true;
				_first_load = true;
			}
		}
		break;
	case ACTIVITY_OPEN_TYPE_CIR:// 指定星期几循环
		{
			uint32 old_open_time = _open_time;
			_open_time = time_utils::make_time_day(_act_info.fixed_day,_act_info.start_hour,_act_info.start_minute,_act_info.start_second);
			_close_time = _open_time + _act_info.last_time;
			
			if (old_open_time != _open_time){
				_is_change_time = true;
			}
		}
		break;
	default:
		return false;
		break;
	}

	if (_open_time != 0 && _close_time != 0){
		bool is_open = false;
		if (now < _open_time || now > _close_time){
			if (_act_info.open_type == ACTIVITY_OPEN_TYPE_CIR){
				back_door_type = BACKGROUND_ACTIVITY_OPEN_TYPE_ALREADY;
			}else if (_act_info.open_type == ACTIVITY_OPEN_TYPE_DATE){
				if (now < _open_time){
					back_door_type = BACKGROUND_ACTIVITY_OPEN_TYPE_ALREADY;
				}else{
					back_door_type = BACKGROUND_ACTIVITY_OPEN_TYPE_CLOSE;
				}
			}else if (_act_info.open_type == ACTIVITY_OPEN_TYPE_OPEN){
				if (now > _close_time && _act_info.repeat_time == 0){
					back_door_type = BACKGROUND_ACTIVITY_OPEN_TYPE_CLOSE;
				}else if (now < _open_time && _act_info.repeat_time == 0){
					back_door_type = BACKGROUND_ACTIVITY_OPEN_TYPE_ALREADY;
				}
			}

			is_open = false;
		}else{
			is_open = true;

			back_door_type = BACKGROUND_ACTIVITY_OPEN_TYPE_OPEN;// 开启中
		}

		// 算循环能否开：
		if (!is_open && _act_info.open_type == ACTIVITY_OPEN_TYPE_OPEN && _act_info.repeat_time != 0){
			if (now > _open_time){
				uint32 interval = _act_info.repeat_time + _act_info.last_time;
				uint32 n = (now - _first_open) / interval;
				if (n == 0){
					// 只能在间隔时间内
					back_door_type = BACKGROUND_ACTIVITY_OPEN_TYPE_ALREADY;// 关闭中但有循环

					return false;
				}else{
					// 下一次的开启时间 (n * interval + _open_time)
					_open_time = n * interval + _first_open;
					_close_time = _open_time + _act_info.last_time;
					_is_change_time  = true;
					if (now < _open_time || now > _close_time){
						back_door_type = BACKGROUND_ACTIVITY_OPEN_TYPE_ALREADY;// 关闭中但有循环
						return false;
					}else{
						back_door_type = BACKGROUND_ACTIVITY_OPEN_TYPE_OPEN;// 开启中
						return true;
					}
				}
			}else{
				back_door_type = BACKGROUND_ACTIVITY_OPEN_TYPE_ALREADY;
				return false;
			}
		}

		return is_open;
	}

	return false;
}

void poll_activity::on_process(bool& need_brocast,uint32& back_door_type)
{
	if (check_activity(back_door_type)){
		if (!is_launch_){
			on_open();
			need_brocast = true;
			_backdoor_change = false;
		}
	}else{
		if (is_launch_){
			on_close();
			need_brocast = true;
		}
	}
}

void poll_activity::on_open()
{
	is_launch_ = true;

	reset_reward_flag();
}

void poll_activity::on_close()
{
	is_launch_ = false;
}

int poll_activity::save(amtsvr_context* ctx)
{
	if (_db_activity_open == NULL || !_change) {
		return -1;
	}

	mysql::tb_activity_open* p = new mysql::tb_activity_open(*_db_activity_open);
	CACHE_KEY k(p->activity_id(), 0);
	write(ctx, k, p);
	writer_type = WOTE_UPDATE;

	_change = false;

	return 0;
}

int poll_activity::remove()
{
	if (_db_activity_open != NULL) {
		delete _db_activity_open;
		_db_activity_open = NULL;
	}

	return 0;
}

uint32 poll_activity::get_remain_time()
{
	uint32 now = (uint32)time(NULL);
	if (_close_time > now){
		return _close_time - now;
	}else{
		return 0;
	}
}

bool poll_activity::is_same_day()
{
	uint32 now = (uint32)time(NULL);
	if (!time_utils::is_same_day(_open_time,now)){
		// 不是同一天
		return false;
	}

	return true;
}

uint32 poll_activity::mut_activity_info(config_activity& info)
{
	_act_info.operator=(info);

	if (_db_activity_open == NULL){
		_db_activity_open = new mysql::tb_activity_open;
		_db_activity_open->set_activity_id(info.activity_id);
		_db_activity_open->set_open_type(ACTIVITY_OPEN_TYPE_OPEN);
		_db_activity_open->set_reward_flag(0);
		writer_type = WOTE_INSERT;
	}else{

		writer_type = WOTE_UPDATE;
	}

	_change = true;

	return 0;
}

void poll_activity::add_reward_flag()
{
	if (_db_activity_open != NULL){
		if (_db_activity_open->reward_flag() != 1){
			_db_activity_open->set_reward_flag(1);

			writer_type = WOTE_UPDATE;

			_change = true;
		}
	}
}

void poll_activity::reset_reward_flag()
{
	if (_db_activity_open != NULL){
		if (_db_activity_open->reward_flag() != 0){
			_db_activity_open->set_reward_flag(0);

			writer_type = WOTE_UPDATE;

			_change = true;
		}
	}
}

void poll_activity::reset_act_data(uint32 open_type,uint32 start_time,uint32 last_time,uint32 replace_time)
{

	// 根据开启类型处理开始时间，结束时间
	switch(open_type)
	{
	case ACTIVITY_OPEN_TYPE_OPEN:// 开服第几天，start_time 第几天
		{
			uint32 ser_start_time = static_cast<uint32>(amtsvr_svr_start_time());
			_open_time = ser_start_time + (start_time - 1)* 24 * 3600;
			_close_time = _open_time + _act_info.last_time;
			_first_open = _open_time;
			_first_close = _close_time;

		}
		break;
	case ACTIVITY_OPEN_TYPE_DATE:// 指定年月日
		{
			_open_time = start_time;

			_close_time = _open_time + _act_info.repeat_time;

			_first_open = _open_time;

			_first_close = _close_time;
		}
		break;
	case ACTIVITY_OPEN_TYPE_CIR:// 指定星期几循环
		{
			// start_time 解析成具体星期几，几分几秒 赋值到_act_info:
			time_t cir = start_time;
			struct tm tm_temp;
			localtime_r(&cir, &tm_temp);

			if (tm_temp.tm_wday == 0){
				_act_info.fixed_day = 7;
			}else{
				_act_info.fixed_day = tm_temp.tm_wday;
			}

			_act_info.start_hour = tm_temp.tm_hour;
			_act_info.start_minute = tm_temp.tm_min;
			_act_info.start_second = tm_temp.tm_sec;
		}
		break;
	default:
		{
			return;
		}
		break;
	}

	_act_info.open_type = open_type;

	_act_info.last_time = last_time;

	_act_info.repeat_time = replace_time;

	_is_change_time = true;

	is_launch_ = false;
}