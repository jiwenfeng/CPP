#include "poll_activity_mgr.h"
#include "config_activity.h"
#include "poll_activity.h"
#include "config_activity_item.h"

poll_activity_mgr::poll_activity_mgr()
{

	is_load = false;
}


poll_activity_mgr::~poll_activity_mgr()
{
	std::map<uint32, poll_activity*>::iterator it = _activity_map.begin();
	for (;it != _activity_map.end(); ++it){
		it->second->remove();
		delete it->second;
	}

	_activity_map.clear();
}

#define CUR hash_map_obj<CACHE_KEY, cfg_cache_obj, CFG_CACHE_BULB_NUM>::pair_t

int poll_activity_mgr::init(struct cmq_t* l)
{
	CUR* cur = (CUR*)CONFMGR->get_activity_open_head();

	while (cur != NULL) {
		google::protobuf::Message* p = cur->mutable_val().pop_msg();
		mysql::tb_activity_open* tfl = static_cast<mysql::tb_activity_open*>(p);
		cur = (CUR*)cur->get_next();
		if (tfl != NULL) {
			poll_activity* fl = new poll_activity(tfl);
			if (fl == NULL){
				continue;
			}

			_activity_map.insert(pair<uint32, poll_activity*>(tfl->activity_id(), fl));
		}
	}

	_activity_record_mgr.init(l);

	_activity_promotions_mgr.init(l);

	return 0;
}

int poll_activity_mgr::save(amtsvr_context* ctx)
{
	std::map<uint32,poll_activity*>::iterator it = _activity_map.begin();
	for (; it != _activity_map.end(); ++it){
		it->second->save(ctx);
	}

	_activity_record_mgr.save(ctx);

	_activity_promotions_mgr.save(ctx);

	return 0;
}

int poll_activity_mgr::remove_activity(uint32 acitivity_id)
{
	std::map<uint32,poll_activity*>::iterator it = _activity_map.find(acitivity_id);
	
	if (it != _activity_map.end()){
		delete it->second;
		it->second = NULL;
	}

	_activity_record_mgr.remove(acitivity_id);

	return 0;
}

poll_activity* poll_activity_mgr::get_activity_info(uint32 activity_id)
{
	std::map<uint32,poll_activity*>::iterator it = _activity_map.find(activity_id);
	if (it != _activity_map.end()){
		return it->second;
	}

	return NULL;
}

int poll_activity_mgr::activity_info_req(client::activity_info_req* req,client::activity_info_rsp& rsp_msg)
{
	rsp_msg.set_activity_id(req->activity_id());

	poll_activity* act_info = get_activity_info(req->activity_id());

	if (act_info == NULL){
		return -1;
	}

	client::act_open_info* info = rsp_msg.add_info();
	info->set_activity_id(act_info->get_activity_info().activity_id);
	info->set_is_activity(act_info->is_activiting());
	if (act_info->is_activiting()){
		info->set_remain_time(act_info->get_remain_time());
	}

	client::act_db_info* record_info = rsp_msg.add_db_info();

	int ret = 0;

	ret = _activity_record_mgr.fill_record_info(req->activity_id(),req->svr_head().role_id(),*record_info);

	

	return ret;
}

int poll_activity_mgr::buy_activity_shop_req(client::activity_shop_req* req,client::activity_shop_c2g& rsp_msg)
{
	poll_activity* act_info = get_activity_info(req->activity_id());

	if (act_info == NULL){
		return -1;
	}

	if (!act_info->is_activiting()){
		return -1;
	}

	const config_activity_item* act_item_info = CONFMGR->get_config_activity_item_mgr().get_config_activity_item(req->item_id());
	if (act_item_info == NULL){
		return -1;
	}

	const std::map<int, int>& buy_time_map_ = act_item_info->get_buy_times();
	std::map<int, int>::const_iterator it = buy_time_map_.begin();
	uint32 buy_type = 0;
	uint32 limit_count = 0;
	for (;it != buy_time_map_.end();++it){
		buy_type = static_cast<uint32>(it->first);
		limit_count = static_cast<uint32>(it->second);
		break;
	}

	uint32 cur_count = 0;
	// 判断是否购买上限
	if (!_activity_record_mgr.get_buy_record_count(req->activity_id(),req->svr_head().role_id(),buy_type,req->item_id(),cur_count)){
		return -1;
	}

	if (cur_count >= limit_count || (cur_count + req->item_count()) > limit_count){
		return -1;
	}
	
	// 先增加记录：
	// 记录
	_activity_record_mgr.add_buy_record_data(req->activity_id(),req->svr_head().role_id(),buy_type,req->item_count(),req->item_id());

	rsp_msg.set_activity_id(req->activity_id());
	rsp_msg.set_activity_type(req->item_id());
	rsp_msg.set_item_count(req->item_count());
	rsp_msg.set_role_id(req->svr_head().role_id());
	rsp_msg.set_close_time(act_info->get_close_time());

	return 0;
}

int poll_activity_mgr::activity_record_opt(client::activity_shop_g2c* req,client::activity_shop_rsp& rsp_msg)
{
	// 
	poll_activity* act_info = get_activity_info(req->activity_id());

	if (act_info == NULL){
		rsp_msg.set_result_type(3);
		return -1;
	}

	const config_activity_item* act_item_info = CONFMGR->get_config_activity_item_mgr().get_config_activity_item(req->activity_type());
	if (act_item_info == NULL){
		rsp_msg.set_result_type(4);
		return -1;
	}

	const std::map<int, int>& buy_time_map_ = act_item_info->get_buy_times();
	std::map<int, int>::const_iterator it = buy_time_map_.begin();
	uint32 buy_type = 0;
	for (;it != buy_time_map_.end();++it){
		buy_type = static_cast<uint32>(it->first);
		break;
	}

	if (!req->is_sucess()){
		// 移除所记录的
		_activity_record_mgr.remove_record_count(req->activity_id(),req->role_id(),buy_type,req->activity_type(),req->item_count());
		rsp_msg.set_result_type(1);
		return -1;
	}

	rsp_msg.set_result_type(0);

	for (int i = 0; i < req->pro_info_size(); ++i){
		client::props_info* info = rsp_msg.add_pro_info();
		info->CopyFrom(req->pro_info(i));
	}

	client::act_db_info* info = rsp_msg.add_db_info();
	_activity_record_mgr.fill_record_info(req->activity_id(),req->role_id(),*info);

	return 0;
}

int poll_activity_mgr::activity_reward_req(client::activity_reward_req* req,client::activity_reward_c2g& rsp_msg)
{
	poll_activity* act_info = get_activity_info(req->activity_id());

	if (act_info == NULL){
		return -1;
	}

	const config_activity_item* act_item_info = CONFMGR->get_config_activity_item_mgr().get_config_activity_item(req->activity_type());
	if (act_item_info == NULL){
		return -1;
	}

	// 判断是否领取过
	uint32 cur_flag = act_reward[act_item_info->get_item_order()];
	uint32 need_flag = 0;
	if (!_activity_record_mgr.get_person_reward_flag(req->activity_id(),req->svr_head().role_id(),req->activity_type(),need_flag)){
		return -1;
	}

	if (HAS_FLAG(need_flag,cur_flag)){
		return -1;
	}
	// 先插入记录：
	// 插入标记
	INSERT_FLAG(need_flag,cur_flag);
	// 存储记录need_flag
	_activity_record_mgr.set_person_reward_flag(req->activity_id(),req->svr_head().role_id(),need_flag,req->activity_type());

	rsp_msg.set_activity_id(req->activity_id());
	rsp_msg.set_activity_type(req->activity_type());
	rsp_msg.set_role_id(req->svr_head().role_id());
	rsp_msg.set_close_time(act_info->get_close_time());

	return 0;
}

int poll_activity_mgr::activity_record_reward_opt(client::activity_reward_g2c* req,client::activity_reward_rsp& rsp_msg)
{
	rsp_msg.set_activity_id(req->activity_id());
	rsp_msg.set_activity_type(req->activity_type());

	poll_activity* act_info = get_activity_info(req->activity_id());

	if (act_info == NULL){
		rsp_msg.set_result_type(2);
		return -1;
	}

	const config_activity_item* act_item_info = CONFMGR->get_config_activity_item_mgr().get_config_activity_item(req->activity_type());
	if (act_item_info == NULL){
		rsp_msg.set_result_type(4);
		return -1;
	}

	uint32 cur_flag = act_reward[act_item_info->get_item_order()];

	uint32 need_flag = 0;

	_activity_record_mgr.get_person_reward_flag(req->activity_id(),req->role_id(),req->activity_type(),need_flag);

	if (!req->is_success()){
		// 移除标记
		REMOVE_FLAG(need_flag,cur_flag);
		// 移除记录
		_activity_record_mgr.remove_reward_flag(req->activity_id(),req->role_id(),need_flag);

		rsp_msg.set_result_type(5);

		return -1;
	}

	rsp_msg.set_result_type(0);
	for (int i = 0; i < req->pro_info_size(); ++i){
		client::props_info* info = rsp_msg.add_pro_info();
		info->CopyFrom(req->pro_info(i));
	}

	return 0;
}

int poll_activity_mgr::activity_status_list(client::load_activity_list_center* req,client::activity_list_rsp& rsp_msg,std::map<uint32,uint32>& his_map_)
{
	std::map<uint32,poll_activity*>::iterator it = _activity_map.begin();
	rsp_msg.set_time(0);
	uint32 act_limit_type = 0;
	for (;it != _activity_map.end(); ++it){
		client::activity_base* info = rsp_msg.add_info();
		info->set_activity_id(it->first);
		info->set_activity_status(it->second->is_activiting());
		if (it->second->is_activiting()){
			act_limit_type = CONFMGR->get_config_activity_item_mgr().get_act_limit_type(it->first);
			if (act_limit_type == REWARD_TYPE_POWER || act_limit_type == REWARD_TYPE_GEM_LEVEL){
				his_map_.insert(std::pair<uint32,uint32>(it->first,act_limit_type));
			}
		}
	}

	return 0;
}

int poll_activity_mgr::activity_record_his_info(client::activity_his_record_g2c* req)
{
	_activity_record_mgr.add_his_record(req->activity_id(),req->role_id(),req->old_value(),req->limit_type());

	return 0;
}

int poll_activity_mgr::update_his_record(client::update_activity_record_g2c* req)
{
	uint32 act_limit_type = 0;
	std::map<uint32,poll_activity*>::iterator it = _activity_map.begin();
	for (;it != _activity_map.end();++it){
		if (it->second->is_activiting()){
			act_limit_type = CONFMGR->get_config_activity_item_mgr().get_act_limit_type(it->first);
			if (act_limit_type == REWARD_TYPE_POWER || act_limit_type == REWARD_TYPE_GEM_LEVEL){
				if (act_limit_type == req->limit_type()){
					_activity_record_mgr.add_his_record(it->first,req->role_id(),req->value_count(),req->limit_type());
				}

			}
		}
	}

	return 0;
}

uint32 poll_activity_mgr::set_activity_open_type(uint32 activity_id,uint32 open_type)
{
	std::map<uint32,poll_activity*>::iterator it = _activity_map.find(activity_id);

	if (it == _activity_map.end()){
		return 1;
	}

	it->second->set_open_type(open_type);

	return 0;
}

uint32 poll_activity_mgr::reset_activity_open_data(uint32 activity_id,uint32 open_type,uint32 start_time,uint32 last_time,uint32 replate_time)
{
	std::map<uint32,poll_activity*>::iterator it = _activity_map.find(activity_id);
	if (_activity_map.size() != 0){
		if (it != _activity_map.end()){
			it->second->reset_act_data(open_type,start_time,last_time,replate_time);
			return 0;
		}
	}

	// 若找不到就新增一个活动
	activity_info info;
	info.activity_id = activity_id;
	poll_activity* poll_obj = new poll_activity(info);
	poll_obj->reset_act_data(open_type,start_time,last_time,replate_time);
	_activity_map.insert(std::pair<uint32,poll_activity*>(activity_id,poll_obj));

	return 0;
}

bool poll_activity_mgr::is_reward(uint32 activity_id)
{
	std::map<uint32,poll_activity*>::iterator it = _activity_map.find(activity_id);

	if (it == _activity_map.end()){
		return true;
	}

	return it->second->is_reward();
}

void poll_activity_mgr::reset_reward_flag(uint32 activity_id)
{
	std::map<uint32,poll_activity*>::iterator it = _activity_map.find(activity_id);

	if (it == _activity_map.end()){
		return;
	}

	it->second->reset_reward_flag();
}

void poll_activity_mgr::add_reward_flag(uint32 activity_id)
{
	std::map<uint32,poll_activity*>::iterator it = _activity_map.find(activity_id);

	if (it == _activity_map.end()){
		return;
	}

	it->second->add_reward_flag();
}