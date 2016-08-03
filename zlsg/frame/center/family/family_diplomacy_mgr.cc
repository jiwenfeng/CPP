#include "family_diplomacy_mgr.h"

family_diplomacy_mgr::family_diplomacy_mgr()
{
}

family_diplomacy_mgr::~family_diplomacy_mgr()
{
    multimap<uint32, family_diplomacy*>::iterator i = family_diplomacy_map_.begin();
    for (; i != family_diplomacy_map_.end(); ++i) {
        delete i->second;
    }

    family_diplomacy_map_.clear();

	multimap<uint32, family_diplomacy*>::iterator it = _remove.begin();
	for (; it != _remove.end(); ++it) {
		delete it->second;
	}

	_remove.clear();
	_del_vec.clear();
}

#define CUR hash_map_obj<CACHE_KEY, cfg_cache_obj, CFG_CACHE_BULB_NUM>::pair_t

int family_diplomacy_mgr::init(struct cmq_t* l)
{
	CUR* cur = (CUR*)CONFMGR->get_family_diplomacy_head();

	while (cur != NULL) {
		google::protobuf::Message* p = cur->mutable_val().pop_msg();
		mysql::tb_family_diplomacy* tfl = static_cast<mysql::tb_family_diplomacy*>(p);
		cur = (CUR*)cur->get_next();
		if (tfl != NULL) {
			family_diplomacy* fl = new family_diplomacy(tfl);
			if (fl == NULL){
				continue;
			}

			family_diplomacy_map_.insert(pair<uint32, family_diplomacy*>(tfl->family_id(), fl));
		}
	}
    
    return 0;
}

//int family_domain_mgr::update(client::family_domain_update* fdu)
//{
//    bool set = false;
//    multimap<uint32, family_domain*>::iterator i = _domain.find(fdu->scene_id());
//    for (; i != _domain.end(); ++i) {
//        if (i->second->get()->scene_id() == fdu->scene_id()) {
//            i->second->set_family_id(fdu->family_id());
//            set = true;
//        }
//    }
//
//    if (!set) {
//        family_domain* fl = new family_domain(fdu->family_id(), fdu->scene_id());
//        _domain.insert(pair<uint32, family_domain*>(fdu->scene_id(), fl));
//    }
//    
//    return 0;
//}

int family_diplomacy_mgr::remove(uint32 family_id,uint32 target_id,bool& is_remove_ally)
{
	multimap<uint32, family_diplomacy*>::iterator itr = family_diplomacy_map_.find(family_id);
	for (;itr != family_diplomacy_map_.end();++itr) {
		if (itr->second->get_family_diplomacy()->target_id() == target_id && itr->first == family_id){
			if (itr->second->get_family_diplomacy()->relation_type() == OPT_FORIGN_TYPE_ALLY){
				is_remove_ally = true;
			}
			itr->second->remove();
			_remove.insert(pair<uint32, family_diplomacy*>(itr->first, itr->second));
		}
	}

	multimap<uint32, family_diplomacy*>::iterator itr_1 = family_diplomacy_map_.find(family_id);
	while (itr_1 != family_diplomacy_map_.end()) {
		if (itr_1->second->get_family_diplomacy()->target_id() == target_id && itr_1->first == family_id) {
			multimap<uint32, family_diplomacy*>::iterator erase_iter = itr_1++;
			family_diplomacy_map_.erase(erase_iter);
		} else {
			++itr_1;
		}
	}

    return 0;
}

int family_diplomacy_mgr::remove(uint32 family_id)
{
	multimap<uint32, family_diplomacy*>::iterator itr = family_diplomacy_map_.begin();

	for (;itr != family_diplomacy_map_.end();++itr) {
		if (itr->first == family_id){
			itr->second->remove();
			_remove.insert(pair<uint32, family_diplomacy*>(itr->first, itr->second));
		}
		//else {
		//	break;
		//}
	}

	multimap<uint32, family_diplomacy*>::iterator itr_1 = family_diplomacy_map_.begin();
	while (itr_1 != family_diplomacy_map_.end()) {
		if (itr_1->first == family_id) {
			multimap<uint32, family_diplomacy*>::iterator erase_iter = itr_1++;
			family_diplomacy_map_.erase(erase_iter);
		} else {
			++itr_1;
		}
	}

	return 0;
}

int family_diplomacy_mgr::update_diplomacy_relation(uint32 target_id)
{
	multimap<uint32, family_diplomacy*>::iterator itr = family_diplomacy_map_.begin();

	for (;itr != family_diplomacy_map_.end();++itr) {
		if (itr->second->get_family_diplomacy()->target_id() == target_id){
			itr->second->remove();
			_remove.insert(pair<uint32, family_diplomacy*>(itr->first, itr->second));
		}
		//else {
		//	break;
		//}
	}

	multimap<uint32, family_diplomacy*>::iterator itr_1 = family_diplomacy_map_.begin();
	while (itr_1 != family_diplomacy_map_.end()) {
		if (itr_1->second->get_family_diplomacy()->target_id() == target_id) {
			multimap<uint32, family_diplomacy*>::iterator erase_iter = itr_1++;
			family_diplomacy_map_.erase(erase_iter);
		} else {
			++itr_1;
		}
	}

	return 0;
}

int family_diplomacy_mgr::save(amtsvr_context* ctx)
{
    multimap<uint32, family_diplomacy*>::iterator i = family_diplomacy_map_.begin();
    for (; i != family_diplomacy_map_.end(); ++i) {
        i->second->save(ctx);
    }

	multimap<uint32, family_diplomacy*>::iterator j = _remove.begin();
	for (; j != _remove.end(); ++j) {
		j->second->save(ctx);
		delete j->second;
	}

	_remove.clear();
    
    return 0;
}
// 外交关系操作
bool family_diplomacy_mgr::operate_family_foreign(uint32 opt_type,uint32 family_id,uint32 target_id)
{
	switch(opt_type)
	{
	case OPT_FORIGN_TYPE_ALLY:// 同意结盟
		{
			return agree_ally(opt_type,family_id,target_id);
		}
		break;
	case OPT_FORIGN_TYPE_HOSTILITY:// 敌盟
		{
			return add_hostility(opt_type,family_id,target_id);
		}
		break;
	case OPT_FORIGN_TYPE_CHECKING:// 申请同盟
		{
			return apply_ally(opt_type,family_id,target_id);
		}
		break;
	case  OPT_FORIGN_TYPE_REJECT:// 拒绝结盟
		{
			return reject_ally(opt_type,family_id,target_id);
		}
		break;
	default:
		return false;
		break;
	}

	return true;
}
// 申请结盟
bool family_diplomacy_mgr::apply_ally(uint32 opt_type,uint32 family_id,uint32 target_id)
{
	multimap<uint32, family_diplomacy*>::iterator itr = family_diplomacy_map_.find(family_id);
	bool is_new = true;
	for (;itr != family_diplomacy_map_.end(); ++itr){
		if (itr->second->get_family_diplomacy()->target_id() == target_id && itr->first == family_id){
			if (itr->second->get_family_diplomacy()->relation_type() == opt_type ||
				 itr->second->get_family_diplomacy()->relation_type() ==  OPT_FORIGN_TYPE_HOSTILITY ){
				return false;
			}

			is_new = false;
		}
	}

	uint32 cur_time = (uint32)time(NULL);

	if (is_new){
		family_diplomacy* info = new family_diplomacy(family_id,target_id,opt_type,cur_time);
		family_diplomacy_map_.insert(std::pair<uint32, family_diplomacy*>(family_id,info));
	}

	return true;
}
// 添加敌盟
bool family_diplomacy_mgr::add_hostility(uint32 opt_type,uint32 family_id,uint32 target_id)
{
	multimap<uint32, family_diplomacy*>::iterator itr = family_diplomacy_map_.find(family_id);
	bool is_new = true;
	for (;itr != family_diplomacy_map_.end(); ++itr){
		if (itr->second->get_family_diplomacy()->target_id() == target_id && itr->first == family_id){
			if (itr->second->get_family_diplomacy()->relation_type() == opt_type){
				return false;
			}

			itr->second->change_relaition_type(opt_type);

			return true;

			is_new = false;
		}
	}

	if (is_new){
		family_diplomacy* info = new family_diplomacy(family_id,target_id,opt_type);
		family_diplomacy_map_.insert(std::pair<uint32, family_diplomacy*>(family_id,info));
		return true;
	}

	return false;
}
// 同意结盟
bool family_diplomacy_mgr::agree_ally(uint32 opt_type,uint32 family_id,uint32 target_id)
{
	bool is_new = true;
	multimap<uint32, family_diplomacy*>::iterator itr = family_diplomacy_map_.find(family_id);
	for (;itr != family_diplomacy_map_.end(); ++itr){
		if (itr->second->get_family_diplomacy()->target_id() == target_id && itr->first == family_id){
			itr->second->change_relaition_type(opt_type);
			is_new = false;
		}
	}

	if (is_new){
		family_diplomacy* info = new family_diplomacy(family_id,target_id,opt_type);

		family_diplomacy_map_.insert(std::pair<uint32, family_diplomacy*>(family_id,info));
	}

	return true;
}
// 拒绝结盟
bool family_diplomacy_mgr::reject_ally(uint32 opt_type,uint32 family_id,uint32 target_id)
{
	multimap<uint32, family_diplomacy*>::iterator itr = family_diplomacy_map_.find(target_id);

	for (;itr != family_diplomacy_map_.end(); ++itr){
		if (itr->second->get_family_diplomacy()->target_id() == family_id && itr->first == target_id){
			bool is_remove_ally = false;
			remove(target_id,family_id,is_remove_ally);
			break;
		}
	}

	return true;
}
// 获取关系数据
uint32 family_diplomacy_mgr::get_relation_type(uint32 family_id,uint32 target_id)
{

	multimap<uint32, family_diplomacy*>::iterator itr = family_diplomacy_map_.find(family_id);
	for (;itr != family_diplomacy_map_.end(); ++itr){
		if (itr->second->get_family_diplomacy()->target_id() == target_id && itr->first == family_id){
			return itr->second->get_family_diplomacy()->relation_type();
		}
	}

	return 0;
}

uint32 family_diplomacy_mgr::get_relation_family_id(uint32 family_id,uint32 relation_type,vector<uint32>& vec_family_id)
{

	multimap<uint32, family_diplomacy*>::iterator itr = family_diplomacy_map_.find(family_id);
	for (;itr != family_diplomacy_map_.end(); ++itr){
		if (itr->second->get_family_diplomacy()->relation_type() == relation_type && itr->first == family_id){
			 vec_family_id.push_back(itr->second->get_family_diplomacy()->target_id());
		}
	}

	return 0;
}

void family_diplomacy_mgr::fill_diplomacy_info(uint32 family_id,client::ally_family_list_rsp& rsp_msg)
{
	multimap<uint32, family_diplomacy*>::iterator itr = family_diplomacy_map_.find(family_id);
	for (;itr != family_diplomacy_map_.end();++itr){
		if (itr->first == family_id){
			client::ally_info* info = rsp_msg.add_info();
			info->set_camp_type(itr->second->get_family_diplomacy()->relation_type());
			info->set_family_id(itr->second->get_family_diplomacy()->target_id());
		}
	}
}

bool family_diplomacy_mgr::clean_up_relation_by_hostility(uint32 family_id,uint32 target_id)
{
	multimap<uint32, family_diplomacy*>::iterator itr_1 = family_diplomacy_map_.find(family_id);
	for (; itr_1 != family_diplomacy_map_.end();++itr_1){
		if (itr_1->second->get_family_diplomacy()->target_id() == target_id && itr_1->first == family_id){
			if (itr_1->second->get_family_diplomacy()->relation_type() == OPT_FORIGN_TYPE_ALLY){
				bool is_remove_ally = false;
				remove(family_id,target_id,is_remove_ally);
				break;
			}
		}
	}

	return true;
}

bool family_diplomacy_mgr::clear_apply_relation()
{
	_del_vec.clear();

	multimap<uint32, family_diplomacy*>::iterator itr = family_diplomacy_map_.begin();
	uint32 past_time = static_cast<uint32>(CONFMGR->get_basic_value(10105));
	uint32 cur_time = (uint32)time(NULL);
	for(;itr != family_diplomacy_map_.end(); ++itr){
		if (itr->second->get_family_diplomacy()->relation_type() == OPT_FORIGN_TYPE_CHECKING){
			if (cur_time >= itr->second->get_family_diplomacy()->apply_time() + past_time){
				_del_vec.push_back(itr->second);
			}
		}
	}

	bool is_remove_ally = false;
	for (uint32 i = 0; i < _del_vec.size(); ++i){
		remove(_del_vec[i]->get_family_diplomacy()->family_id(),_del_vec[i]->get_family_diplomacy()->target_id(),is_remove_ally);
	}

	_del_vec.clear();

	return true;
}

uint32 family_diplomacy_mgr::get_relation_count(uint32 family_id,uint32 relation_type)
{
	uint32 count = 0;
	multimap<uint32, family_diplomacy*>::iterator itr = family_diplomacy_map_.find(family_id);
	for (; itr != family_diplomacy_map_.end();++itr){
		if (itr->first == family_id && itr->second->get_family_diplomacy()->relation_type() == relation_type){
			++count;
		}
	}

	return count;
}