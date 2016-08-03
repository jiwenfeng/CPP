#if 0
#include "siege_mgr.h"

siege_mgr::siege_mgr()
{
}

siege_mgr::~siege_mgr()
{
    for (map<uint32, siege_center*>::iterator i = _siege.begin(); i != _siege.end(); ++i) {
        delete i->second;
    }

    _siege.clear();
}

#define CUR hash_map_obj<CACHE_KEY, cfg_cache_obj, CFG_CACHE_BULB_NUM>::pair_t

int siege_mgr::init(struct cmq_t* l)
{
    CUR* cur = (CUR*)CONFMGR->get_siege_warfare_head();
    while (cur != NULL) {
        google::protobuf::Message* p  = cur->mutable_val().pop_msg();
        mysql::tb_siege_warfare*   tf = static_cast<mysql::tb_siege_warfare*>(p);
        cur = (CUR*)cur->get_next();
        if (tf == NULL) {
            continue;
        }

        siege_center* f = new siege_center(tf);
        _siege.insert(pair<uint32, siege_center*>(tf->id(), f));
    }

    if (_siege.empty()) {
        _siege.insert(pair<uint32, siege_center*>(1, new siege_center()));
    }
    
    return 0;
}

int siege_mgr::save(amtsvr_context* ctx)
{
    for (map<uint32, siege_center*>::iterator i = _siege.begin(); i != _siege.end(); ++i) {
        i->second->save(ctx);
    }

    return 0;
}

int siege_mgr::update_official(client::broadcast_siege_official* req)
{
    map<uint32, siege_center*>::iterator i = _siege.find(1);
    if (i == _siege.end()) {
        _siege.insert(pair<uint32, siege_center*>(1, new siege_center()));
        
        return update_official(req);
    }

    i->second->update(req);
    
    return 0;
}

int siege_mgr::city_official(client::query_city_official_rsp& rsp)
{
    map<uint32, siege_center*>::iterator i = _siege.find(1);
    if (i == _siege.end()) {
        return -1;
    }

    i->second->fill(rsp);
    
    return 0;
}

int siege_mgr::city_camp(client::query_city_camp_rsp& rsp)
{
    for (map<uint32, uint32>::iterator i = _city_camp.begin(); i != _city_camp.end(); ++i) {
        if (i->second == 6) {
            rsp.add_atk(i->first);
        } else {
            rsp.add_def(i->first);
        }
    }
    
    return 0;
}

int siege_mgr::set_city_camp(client::query_city_camp_rsp& rsp)
{
    _city_camp.clear();
    for (int32 i = 0; i < rsp.atk_size(); ++i) {
        _city_camp.insert(pair<uint32, uint32>(rsp.atk(i), 6));
    }

    for (int32 i = 0; i < rsp.def_size(); ++i) {
        _city_camp.insert(pair<uint32, uint32>(rsp.def(i), 5));
    }
    
    return 0;
}
    
int siege_mgr::is_start(client::query_siege_start_rsp& rsp)
{
    rsp.set_time(0);
    rsp.set_use_time(1);
    return 0;
}
#endif
