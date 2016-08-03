#include "family_domain_mgr.h"

family_domain_mgr::family_domain_mgr()
{
}

family_domain_mgr::~family_domain_mgr()
{
    multimap<uint32, family_domain*>::iterator i = _domain.begin();
    for (; i != _domain.end(); ++i) {
        delete i->second;
    }
    _domain.clear();
}

#define CUR hash_map_obj<CACHE_KEY, cfg_cache_obj, CFG_CACHE_BULB_NUM>::pair_t

int family_domain_mgr::init(struct cmq_t* l)
{
    CUR* cur = (CUR*)CONFMGR->get_family_domain_head();

    while (cur != NULL) {
        google::protobuf::Message* p = cur->mutable_val().pop_msg();
        mysql::tb_family_domain* tfl = static_cast<mysql::tb_family_domain*>(p);
        cur = (CUR*)cur->get_next();
        if (tfl != NULL) {
            family_domain* fl = new family_domain(tfl);
            _domain.insert(pair<uint32, family_domain*>(tfl->scene_id(), fl));
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

int family_domain_mgr::remove(uint32 family_id)
{
    multimap<uint32, family_domain*>::iterator i = _domain.begin();
    for (; i != _domain.end(); ++i) {
        if (i->second->get()->family_id() == family_id) {
            i->second->set_family_id(0);
        }
    }

    return 0;
}

int family_domain_mgr::save(amtsvr_context* ctx)
{
    multimap<uint32, family_domain*>::iterator i = _domain.begin();
    for (; i != _domain.end(); ++i) {
        i->second->save(ctx);
    }

    // map<uint32, family_domain*>::iterator j = _remove.begin();
    // for (; j != _remove.end(); ++j) {
    //     j->second->save(ctx);
    //     delete j->second;
    // }
    // _remove.clear();
    
    return 0;
}

uint32 family_domain_mgr::get_num(uint32 family_id)
{
    multimap<uint32, family_domain*>::iterator i = _domain.begin();
    uint32 num = 0;
    for (; i != _domain.end(); ++i) {
        if (i->second->get()->family_id() == family_id) {
            ++num;
        }
    }
    return num;
}

uint32 family_domain_mgr::get_owner(uint32 scene_id)
{
    multimap<uint32, family_domain*>::iterator i = _domain.begin();
    for (; i != _domain.end(); ++i) {
        if (i->second->get()->scene_id() == scene_id) {
            return i->second->get()->family_id();
        }
    }

    return 0;
}

void family_domain_mgr::get_scene(uint32 family_id, std::vector<uint32>& sis)
{
    multimap<uint32, family_domain*>::iterator i = _domain.begin();
    for (; i != _domain.end(); ++i) {
        if (i->second->get()->family_id() == family_id) {
            sis.push_back(i->second->get()->scene_id());
        }
    }
}


