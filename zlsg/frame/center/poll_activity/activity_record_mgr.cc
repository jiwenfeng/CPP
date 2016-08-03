#include "activity_record_mgr.h"
#include "config_activity.h"


activity_record_mgr::activity_record_mgr()
{
	
}


activity_record_mgr::~activity_record_mgr()
{
	std::map<uint32, activity_record*>::iterator i = _activity_map.begin();
	for (; i != _activity_map.end(); ++i) {
		delete i->second;
	}

	_activity_map.clear();

	std::map<uint32, activity_record*>::iterator it = _remove.begin();
	for (; it != _remove.end(); ++it) {
		delete it->second;
	}

	_remove.clear();
}

#define CUR hash_map_obj<CACHE_KEY, cfg_cache_obj, CFG_CACHE_BULB_NUM>::pair_t

int activity_record_mgr::init(struct cmq_t* l)
{
	CUR* cur = (CUR*)CONFMGR->get_activity_record_head();

	while (cur != NULL) {
		google::protobuf::Message* p = cur->mutable_val().pop_msg();
		mysql::tb_activity_record* tfl = static_cast<mysql::tb_activity_record*>(p);
		cur = (CUR*)cur->get_next();
		if (tfl != NULL) {
			activity_record* fl = new activity_record(tfl);
			if (fl == NULL){
				continue;
			}

			_activity_map.insert(pair<uint32, activity_record*>(tfl->activity_id(), fl));
		}
	}

	return 0;
}

int activity_record_mgr::save(amtsvr_context* ctx)
{
	std::map<uint32, activity_record*>::iterator i = _activity_map.begin();
	for (; i != _activity_map.end(); ++i) {
		i->second->save(ctx);
	}

	std::map<uint32, activity_record*>::iterator j = _remove.begin();
	for (; j != _remove.end(); ++j) {
		j->second->save(ctx);
		delete j->second;
	}

	_remove.clear();

	return 0;
}

int activity_record_mgr::add_record(uint32 activity_id)
{
	std::map<uint32,activity_record*>::iterator it = _activity_map.find(activity_id);

	if (it != _activity_map.end()){
		return -1;
	}


	activity_record* record_info = new activity_record(activity_id);

	_activity_map.insert(std::pair<uint32,activity_record*>(activity_id,record_info));

	return 0;
}

int activity_record_mgr::add_buy_record_data(uint32 activity_id,uint32 role_id,uint32 buy_type,uint32 item_count,uint32 act_type)
{
	std::map<uint32,activity_record*>::iterator it = _activity_map.find(activity_id);

	if (it == _activity_map.end()){
		return -1;
	}

	it->second->add_buy_record_data(role_id,buy_type,item_count,act_type);

	return 0;
}

int activity_record_mgr::add_his_record(uint32 act_id,uint32 role_id,uint32 value_count,uint32 limit_type)
{
	std::map<uint32,activity_record*>::iterator it = _activity_map.find(act_id);

	if (it == _activity_map.end()){
		return -1;
	}

	if (it->second->is_has_record(role_id)){
		it->second->add_his_data(role_id,limit_type,value_count);
	}else{
		it->second->creat_limit_record(role_id,limit_type,value_count);
	}
	
	return 0;
}

int activity_record_mgr::create_limit_record(uint32 act_id,uint32 role_id,uint32 limit_type,uint32 value_count)
{
	std::map<uint32,activity_record*>::iterator it = _activity_map.find(act_id);

	if (it == _activity_map.end()){
		return -1;
	}

	if (!it->second->is_has_record(role_id)){
		it->second->creat_limit_record(role_id,limit_type,value_count);
	}
	
	return 0;
}

bool activity_record_mgr::get_buy_record_count(uint32 activity_id,uint32 role_id,uint32 buy_type,uint32 act_type,uint32& record_count)
{

	std::map<uint32,activity_record*>::iterator it = _activity_map.find(activity_id);

	if (it == _activity_map.end()){
		return false;
	}

	record_count = it->second->get_buy_record_data(role_id,buy_type,act_type);

	return true;
}

int activity_record_mgr::remove(uint32 activity_id)
{
	std::map<uint32,activity_record*>::iterator it = _activity_map.find(activity_id);

	if (it == _activity_map.end()){
		return -1;
	}

	it->second->remove();

	_remove.insert(std::pair<uint32,activity_record*>(it->first,it->second));

	_activity_map.erase(it);

	return 0;
}

int activity_record_mgr::is_need_reset(uint32 activity_id)
{
	std::map<uint32,activity_record*>::iterator it = _activity_map.find(activity_id);

	if (it == _activity_map.end()){
		return -1;
	}

	it->second->check_reset();

	return 0;
}

activity_record* activity_record_mgr::get_act_record_info(uint32 activity_id)
{
	std::map<uint32,activity_record*>::iterator it = _activity_map.find(activity_id);
	
	if (it != _activity_map.end()){
		return it->second;
	}

	return NULL;
}

int activity_record_mgr::fill_record_info(uint32 activity_id,uint32 role_id,client::act_db_info& info)
{
	std::map<uint32,activity_record*>::iterator it = _activity_map.find(activity_id);
	if (it != _activity_map.end()){
		it->second->fill_record_data(role_id,info);
		return 0;
	}

	return -1;
}

bool activity_record_mgr::get_person_reward_flag(uint32 activity_id,uint32 role_id,uint32 act_type,uint32& flag)
{
	std::map<uint32,activity_record*>::iterator it = _activity_map.find(activity_id);
	if (it != _activity_map.end()){
		flag =  it->second->get_person_reward_flag(role_id,act_type);
		return true;
	}

	return false;
}


bool activity_record_mgr::set_person_reward_flag(uint32 activity_id, uint32 role_id,uint32 flag,uint32 act_typ)
{
	std::map<uint32,activity_record*>::iterator it = _activity_map.find(activity_id);
	if (it != _activity_map.end()){
		it->second->set_person_reward_flag(role_id,flag,act_typ);
	}



	return true;
}

bool activity_record_mgr::remove_reward_flag(uint32 activity_id, uint32 role_id,uint32 flag)
{
	std::map<uint32,activity_record*>::iterator it = _activity_map.find(activity_id);
	if (it != _activity_map.end()){
		it->second->remove_person_reward_flag(role_id,flag);
	}

	return true;
}

bool activity_record_mgr::remove_record_count(uint32 activity_id,uint32 role_id,uint32 buy_type,uint32 act_type,uint32 delete_count)
{
	std::map<uint32,activity_record*>::iterator it = _activity_map.find(activity_id);
	if (it != _activity_map.end()){
		it->second->remove_record_count(role_id,buy_type,act_type,delete_count);
	}

	return true;
}

bool activity_record_mgr::is_record(uint32 activity_id)
{
	std::map<uint32,activity_record*>::iterator it = _activity_map.find(activity_id);
	if (it != _activity_map.end()){
		return it->second->get_is_record();
	}

	return true;
}

bool activity_record_mgr::set_record(uint32 activity_id,bool record)
{
	std::map<uint32,activity_record*>::iterator it = _activity_map.find(activity_id);
	if (it != _activity_map.end()){
		it->second->set_record(record);
	}

	return true;
}