#include "activity_promotions_mgr.h"


activity_promotions_mgr::activity_promotions_mgr()
{
	_is_copy = false;

}


activity_promotions_mgr::~activity_promotions_mgr()
{
	std::map<uint32,activity_promotions*>::iterator it = _promotions_map.begin();
	for (; it != _promotions_map.end(); ++it){
		delete it->second;
	}

	_promotions_map.clear();
}

#define CUR hash_map_obj<CACHE_KEY, cfg_cache_obj, CFG_CACHE_BULB_NUM>::pair_t

int activity_promotions_mgr::init(struct cmq_t* l)
{
	CUR* cur = (CUR*)CONFMGR->get_promotions_head();

	while (cur != NULL) {
		google::protobuf::Message* p = cur->mutable_val().pop_msg();
		mysql::tb_promotions* tfl = static_cast<mysql::tb_promotions*>(p);
		cur = (CUR*)cur->get_next();
		if (tfl != NULL) {
			activity_promotions* fl = new activity_promotions(tfl);
			if (fl == NULL){
				continue;
			}

			_promotions_map.insert(pair<uint32, activity_promotions*>(tfl->id(), fl));
		}
	}

	return 0;
}

int activity_promotions_mgr::save(amtsvr_context* ctx)
{
	std::map<uint32, activity_promotions*>::iterator i = _promotions_map.begin();
	for (; i != _promotions_map.end(); ++i) {
		i->second->save(ctx);
	}

	return 0;
}

int activity_promotions_mgr::create_new_promotions(uint32 act_id,string name,uint32 type,uint32 start_time,uint32 duration_time,uint32 loop_time)
{
	switch(type)
	{
	case ACTIVITY_OPEN_TYPE_OPEN:
		{
			uint32 ser_start_time = static_cast<uint32>(amtsvr_svr_start_time());
			start_time = ser_start_time + (start_time - 1)* 24 * 3600;
		}
		break;
	case ACTIVITY_OPEN_TYPE_DATE:
	case ACTIVITY_OPEN_TYPE_CIR:
		break;
	default:
		{
			return 0;
		}
	}

	std::map<uint32, activity_promotions*>::iterator i = _promotions_map.find(act_id);
	if (_promotions_map.size() != 0){
		if (i != _promotions_map.end()){ 
			// ¸üÐÂ
			i->second->update_pro(name,type,start_time,duration_time,loop_time);
			return 0;
		}
	}

	activity_promotions* ap = new activity_promotions(act_id,name,type,start_time,duration_time,loop_time);

	_promotions_map.insert(std::pair<uint32,activity_promotions*>(act_id,ap));

	return 0;
}

const uint32 activity_promotions_mgr::get_activity_status(uint32 activity_id)
{
	std::map<uint32, activity_promotions*>::iterator i = _promotions_map.find(activity_id);
	if (i != _promotions_map.end() && _promotions_map.size() != 0){
		return i->second->get_state();
	}

	return BACKGROUND_ACTIVITY_OPEN_TYPE_OPEN;
}

void activity_promotions_mgr::set_activity_status(uint32 activity_id,uint32 status_value)
{
	std::map<uint32, activity_promotions*>::iterator i = _promotions_map.find(activity_id);
	if (i != _promotions_map.end() && _promotions_map.size() != 0){
		i->second->set_act_status(status_value);
	}
}

void activity_promotions_mgr::update_activity_time(uint32 activity_id,uint32 start_time,uint32 close_time)
{
	std::map<uint32, activity_promotions*>::iterator i = _promotions_map.find(activity_id);
	if (_promotions_map.size() != 0){
		if (i != _promotions_map.end()){
			i->second->set_activity_time(start_time,close_time);
		}
	}
}