#ifndef __POLL_ACTIVITY_MGR_H__
#define __POLL_ACTIVITY_MGR_H__

#include <map>
#include "comm_define.h"
#include "poll_activity.h"
#include "activity_record_mgr.h"
#include "activity_promotions_mgr.h"

const uint32 act_reward[32] = {0x00000000,0x00000001,0x00000002,0x00000004,0x00000008,0x00000010,0x00000020,0x00000040,
	0x00000080,0x00000100,0x00000200,0x00000400,0x00000800,0x00001000,0x00002000,0x00004000,
	0x00008000,0x00010000,0x00020000,0x00040000,0x00080000,0x00100000,0x00200000,0x00400000,
	0x00800000,0x01000000,0x02000000,0x04000000,0x08000000,0x10000000,0x20000000,0x40000000};
//	标志位设置
#define INSERT_FLAG(value, flag)	{(value) |= (flag);}
#define REMOVE_FLAG(value, flag)	{(value) &= ~(flag);}
#define HAS_FLAG(value, flag)		( ((value) & (flag)) != 0 ? true : false )

// 限购活动类型，兼容所有限购活动
class poll_activity_mgr
{
public:
	poll_activity_mgr();

	virtual ~poll_activity_mgr();

	int init(struct cmq_t* l);

	int save(amtsvr_context* ctx);
public:
	// 请求单个活动的数据
	int activity_info_req(client::activity_info_req* req,client::activity_info_rsp& rsp_msg);
	// 请求活动购买
	int buy_activity_shop_req(client::activity_shop_req* req,client::activity_shop_c2g& rsp_msg);
	// 请求活动奖励的领取
	int activity_reward_req(client::activity_reward_req* req,client::activity_reward_c2g& rsp_msg);
	// 记录数据
	int activity_record_opt(client::activity_shop_g2c* req,client::activity_shop_rsp& rsp_msg);
	// 记录领取奖励数据
	int activity_record_reward_opt(client::activity_reward_g2c* req,client::activity_reward_rsp& rsp_msg);

	int activity_status_list(client::load_activity_list_center* req,client::activity_list_rsp& rsp_msg,std::map<uint32,uint32>& his_map_);

	int activity_record_his_info(client::activity_his_record_g2c* req);

	int update_his_record(client::update_activity_record_g2c* req);
public:
	int remove_activity(uint32 acitivity_id);
	// 检测活动(开启 or 关闭)
	//void on_process(struct center_service_t* w);

	poll_activity* get_activity_info(uint32 activity_id);

	uint32 set_activity_open_type(uint32 activity_id,uint32 open_type);

	uint32 reset_activity_open_data(uint32 activity_id,uint32 open_type,uint32 start_time,uint32 last_time,uint32 replate_time);

	inline std::map<uint32,poll_activity*>& get_poll_activity_list(){return _activity_map;}

	inline bool get_is_load(){ return is_load;}

	inline void set_load(bool load){ is_load = load;}

	inline activity_record_mgr& get_record_mgr(){return _activity_record_mgr;}

	inline activity_promotions_mgr& get_promotions_mgr(){return _activity_promotions_mgr;}
public:
	// true 为奖励过了
	bool is_reward(uint32 activity_id);

	void reset_reward_flag(uint32 activity_id);

	void add_reward_flag(uint32 activity_id);
private:
	activity_record_mgr			_activity_record_mgr;

	activity_promotions_mgr		_activity_promotions_mgr;

	std::map<uint32,poll_activity*> _activity_map;

	bool is_load;
};

#endif