#ifndef __ACTIVITY_RECORD_MGR_H__
#define __ACTIVITY_RECORD_MGR_H__

#include <map>
#include "comm_define.h"
#include "activity_record.h"

// 记录活动的数据
class activity_record_mgr
{
public:
	activity_record_mgr();

	virtual ~activity_record_mgr();

	int init(struct cmq_t* l);

	int save(amtsvr_context* ctx);

	int add_record(uint32 activity_id);

	int add_buy_record_data(uint32 activity_id,uint32 role_id,uint32 buy_type,uint32 item_count,uint32 activity_type);

	bool get_buy_record_count(uint32 activity_id,uint32 role_id,uint32 buy_type,uint32 act_type,uint32& record_count);

	bool remove_record_count(uint32 activity_id,uint32 role_id,uint32 buy_type,uint32 act_type,uint32 delete_count);

	int add_his_record(uint32 act_id,uint32 role_id,uint32 value_count,uint32 limit_type);

	int create_limit_record(uint32 act_id,uint32 role_id,uint32 limit_type,uint32 value_count = 0);

	int remove(uint32 activity_id);

	int is_need_reset(uint32 activity_id);

	activity_record* get_act_record_info(uint32 activity_id);

	bool get_person_reward_flag(uint32 activity_id,uint32 role_id,uint32 act_type,uint32& flag);

	bool set_person_reward_flag(uint32 activity_id, uint32 role_id,uint32 flag,uint32 act_type);

	bool remove_reward_flag(uint32 activity_id, uint32 role_id,uint32 flag);

	int fill_record_info(uint32 activity_id,uint32 role_id,client::act_db_info& info);

	bool is_record(uint32 activity_id);

	bool set_record(uint32 activity_id,bool record);
public:
	inline std::map<uint32,activity_record*>& get_poll_activity_list(){ return _activity_map;}

private:

	std::map<uint32,activity_record*> _activity_map;

	std::map<uint32, activity_record*> _remove;

	bool _change;
};

#endif