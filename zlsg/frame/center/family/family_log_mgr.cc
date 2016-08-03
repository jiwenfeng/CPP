#include "family_log_mgr.h"

family_log_mgr::family_log_mgr()
{
}

family_log_mgr::~family_log_mgr()
{
    multimap<uint32, family_log*>::iterator i = _log.begin();
    for (; i != _log.end(); ++i) {
        delete i->second;
    }
    
    _log.clear();
    
    std::vector<family_log*>::iterator j = _remove.begin();
    for (; j != _remove.end(); ++j) {
        delete *j;
    }

    _remove.clear();
}

#define CUR hash_map_obj<CACHE_KEY, cfg_cache_obj, CFG_CACHE_BULB_NUM>::pair_t

int family_log_mgr::init(struct cmq_t* l)
{
	init_log_smy_id();

    CUR* cur = (CUR*)CONFMGR->get_family_log_head();

    while (cur != NULL) {
        google::protobuf::Message* p = cur->mutable_val().pop_msg();
        mysql::tb_family_log* tfl = static_cast<mysql::tb_family_log*>(p);
        cur = (CUR*)cur->get_next();
		if (tfl == NULL){
			continue;
		}

		if (set_log_lgsid(*tfl) != 0){
			delete tfl;
			continue;
		}

		 family_log* fl = new family_log(tfl);
		_log.insert(pair<uint32, family_log*>(tfl->family_id(), fl));

    }
    
    return 0;
}

int family_log_mgr::init_log_smy_id()
{
	const hash_map_obj<CACHE_KEY, cfg_cache_obj, CFG_CACHE_BULB_NUM>::pair_t* cur = CONFMGR->get_gdata_max_role_head();
	while (cur != NULL){
		const mysql::sp_get_gdata_max_role* sggmr = static_cast<const mysql::sp_get_gdata_max_role*>(cur->get_val().get_msg());
		cur = cur->get_next();
		log_sid& ffs = _lgsids[sggmr->svr_id()];
		ffs.set(sggmr->svr_id(), 0);
	}


	return 0;
}

int family_log_mgr::set_log_lgsid(const mysql::tb_family_log& tlf)
{
	uint32 sfid = tlf.id();
	uint32 svr_id = GET_SVR_ID(sfid);
	map<uint32, log_sid>::iterator it = _lgsids.find(svr_id);
	if (it == _lgsids.end()){
		return -1;
	}

	uint32 max_fid = it->second.get_log_id();
	uint32 fid = GET_FAMILY_ID(sfid);
	if (fid > max_fid){
		it->second.set(svr_id, fid);
	}

	return 0;
}

int family_log_mgr::save(amtsvr_context* ctx)
{
    std::vector<family_log*>::iterator i = _modify.begin();
    for (; i != _modify.end(); ++i) {
        (*i)->save(ctx);
    }

    _modify.clear();

    std::vector<family_log*>::iterator j = _remove.begin();
    for (; j != _remove.end(); ++j) {
        (*j)->save(ctx);
        delete *j;
    }
    
    _remove.clear();
    
    return 0;
}

bool log_time_sort(family_log* a,family_log* b)
{
	return a->get()->time() > b->get()->time();
}

int family_log_mgr::insert(uint32 family_id, uint32 type, uint32 key, uint32 value,string family_name,uint32 smy_id)
{
    family_log* fl = new family_log(family_id, (uint32)time(NULL), type, key, value,family_name,smy_id);
    _log.insert(pair<uint32, family_log*>(family_id, fl));
    _modify.push_back(fl);
    
	check_log_time(family_id);

    return 0;
}

bool family_log_mgr::generate_log_id(uint32 svr_id, uint32& fid)
{
	map<uint32, log_sid>::iterator it = _lgsids.find(svr_id);
	if (it == _lgsids.end()){
		return false;
	}

	fid = it->second.get();
	if (fid == 0){
		return false;
	}

	return true;
}

int family_log_mgr::check_log_time(uint32 family_id)
{
	vector<family_log*> log_vec;
	log_vec.clear();
	std::multimap<uint32, family_log*>::iterator itr = _log.find(family_id);
	for (;itr != _log.end(); ++itr){
		if (itr->first == family_id){
			log_vec.push_back(itr->second);
		}
		
	}

	vector<uint32> delete_vec;
	delete_vec.clear();
	if (log_vec.size() > 30){
		// ≈≈–Ú
		std::sort(log_vec.begin(),log_vec.end(),log_time_sort);
		for (size_t i = 30; i < log_vec.size(); ++i){
			delete_vec.push_back(log_vec[i]->get()->id());
		}
	}
	
	for (uint32 j = 0; j < delete_vec.size(); ++j){
		delete_log(family_id,delete_vec[j]);
	}

	return 0;
}

int family_log_mgr::delete_log(uint32 family_id,uint32 log_id)
{
	std::multimap<uint32, family_log*>::iterator i = _log.find(family_id);
	for (;i != _log.end();++i){
		if (i->second != NULL) {
		   if (i->second->get()->id() == log_id && i->first == family_id){
				i->second->remove();
				_remove.push_back(i->second);
				_log.erase(i);
				break;
		   }
		}
	}

	return 0;
}

int family_log_mgr::remove(uint32 family_id)
{
    std::multimap<uint32, family_log*>::iterator i = _log.find(family_id);
    while (i != _log.end()) {
        if (i->first != family_id) {
            break;
        }
        
        if (i->second != NULL) {
            i->second->remove();
            _remove.push_back(i->second);
        }
        
        _log.erase(i++);
    }

    return 0;
}

int family_log_mgr::fill_log(uint32 family_id, client::family_log_rsp& rsp)
{
    rsp.set_family_id(family_id);
    
    multimap<uint32, family_log*>::iterator i = _log.find(family_id);
    for (; i != _log.end(); ++i) {
        if (family_id != i->first) {
            break;
        }
        
        family_log_info* fli = rsp.add_fli();
        i->second->fill(fli);
    }

    return 0;
}

