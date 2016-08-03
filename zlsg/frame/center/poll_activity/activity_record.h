#ifndef __ACTIVITY_RECORD_H__
#define __ACTIVITY_RECORD_H__

#include "define.h"
#include "pb_mgr.h"
#include "pb_writer.h"
#include "config_activity.h"
#include "record_parser.h"

#define MAX_ROLE_BUF_NUM 128

#define MAX_ACTIVITY_RECORD_BOLB_LEN 65535  // POTENTIAL最大缓存长度 

class activity_record : public record_parser, public pb_writer
{
public:
	activity_record(mysql::tb_activity_record* activity_record_db = NULL);
	activity_record(uint32 activity_id);
	virtual ~activity_record();

	int save(amtsvr_context* ctx);

	void create_record(uint32 activity_id);

	void add_buy_record_data(uint32 role_id,uint32 buy_type,uint32 item_count,uint32 act_type);

	uint32 get_buy_record_data(uint32 role_id,uint32 buy_type,uint32 act_type);

	void add_person_data(uint32 role_id,uint32 item_count,uint32 act_type);

	uint32 get_person_data_count(uint32 role_id,uint32 act_type);

	void add_server_data(uint32 item_count,uint32 act_type);

	void add_his_data(uint32 role_id,uint32 limit_type,uint32 value_count);

	bool is_has_record(uint32 role_id);

	void creat_limit_record(uint32 role_id,uint32 limit_type,uint32 value_count = 0);

	uint32 get_his_data(uint32 role_id,uint32 limit_type);

	uint32 get_new_data(uint32 role_id,uint32 limit_type);

	uint32 get_server_data_count(uint32 act_type);

	uint32 get_person_reward_flag(uint32 role_id,uint32 act_type);

	bool  set_person_reward_flag(uint32 role_id,uint32 flag,uint32 act_type);

	bool  remove_person_reward_flag(uint32 role_id,uint32 flag);

	bool remove_record_count(uint32 role_id,uint32 buy_type,uint32 act_type,uint32 delete_count);

	int remove();

	bool check_reset();

	inline bool get_is_record(){ return is_record;}

	inline void set_record(bool record){ is_record = record;}

public:

	inline bool today_is_reset(){ return _today_is_reset;}

	inline void set_today_is_reset(bool value_type) { _today_is_reset = value_type;}

	inline mysql::tb_activity_record* get_db_poll_activity() { return _db_activity_record; }

public:

	void fill_record_data(uint32 role_id,client::act_db_info& info);

private:
	// 数据的记录
	mysql::tb_activity_record*     _db_activity_record;
	uint32						   _activity_id;
	bool _today_is_reset;
	bool _change;

	bool is_record;
};

#endif