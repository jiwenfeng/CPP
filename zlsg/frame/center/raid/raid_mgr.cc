#include "raid_mgr.h"

raid_mgr::raid_mgr()
{
}

raid_mgr::~raid_mgr()
{
    map<uint32, raid_center*>::iterator i = _center.begin();
    for (; i != _center.end(); ++i) {
        delete i->second;
    }
    _center.clear();
}

#define CUR hash_map_obj<CACHE_KEY, cfg_cache_obj, CFG_CACHE_BULB_NUM>::pair_t

int raid_mgr::init(struct cmq_t* l)
{
    CUR* cur = (CUR*)CONFMGR->get_raid_center_head();
    while (cur != NULL) {
        google::protobuf::Message* p = cur->mutable_val().pop_msg();
        mysql::tb_raid_center* trc = static_cast<mysql::tb_raid_center*>(p);
        cur = (CUR*)cur->get_next();
        if (trc != NULL) {
            raid_center* rc = new raid_center(trc);
            _center.insert(pair<uint32, raid_center*>(trc->raid_id(), rc));
        }
    }
    
    return 0;
}

int raid_mgr::update(client::raid_info_update* riu)
{
    map<uint32, raid_center*>::iterator i = _center.find(riu->raid_id());
    if (i == _center.end()) {
        return -1;
    }

    raid_center* p = new raid_center;
    p->set(riu->raid_id(), riu->time(), riu->role_id());
    _center.insert(pair<uint32, raid_center*>(riu->raid_id(), p));

    return 0;
}

raid_center* raid_mgr::get(uint32 raid_id)
{
    map<uint32, raid_center*>::iterator i = _center.find(raid_id);
    if (i == _center.end()) {
        return NULL;
    }

    return i->second;
}

int raid_mgr::save(amtsvr_context* ctx)
{
    map<uint32, raid_center*>::iterator i = _center.begin();
    for (; i != _center.end(); ++i) {
        i->second->save(ctx);
    }

    return 0;
}
