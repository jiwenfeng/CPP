#include "explore_mgr.h"

explore_mgr::explore_mgr() 
{
}

explore_mgr::~explore_mgr()
{
    multimap<uint32, explore_center*>::iterator i = _map.begin();
    for (; i != _map.end(); ++i) {
        delete i->second;
    }
    _map.clear();
}

#define CUR hash_map_obj<CACHE_KEY, cfg_cache_obj, CFG_CACHE_BULB_NUM>::pair_t

int explore_mgr::init(struct cmq_t* l)
{
    CUR* cur = NULL;// (CUR*)CONFMGR->get_explore_detail_head();
    while (cur != NULL){
        mysql::tb_role_explore_detail* trc = NULL;
        google::protobuf::Message* p = cur->mutable_val().pop_msg();
        trc = static_cast<mysql::tb_role_explore_detail*>(p);
        cur = (CUR*)cur->get_next();
        if (trc != NULL){
            explore_center* ec = new explore_center(trc);
            _map.insert(pair<uint32, explore_center*>(trc->time(), ec));
        }
    }

    return 0;
}

int explore_mgr::update(client::explore_update_list* eul)
{
    for (int i = 0; i < eul->eu_size(); ++i) {
        client::explore_update* eu = eul->mutable_eu(i);
        explore_center* ec = new explore_center;
        ec->set(eu);
        _map.insert(pair<uint32, explore_center*>(ec->get()->time(), ec));
    }
    
    return 0;
}

int explore_mgr::get(vector<explore_center>& vtr)
{
    return 0;
}

int explore_mgr::save(amtsvr_context* ctx)
{
    return 0;
}

int explore_mgr::fill_explore_list(client::broadcast_explore_list_rsp& rsp)
{
    multimap<uint32, explore_center*>::reverse_iterator rhead = _map.rbegin();
    for (uint32 i = 0; i < 50 && rhead != _map.rend(); ++i, ++rhead) {
        client::explore_update* eu = rsp.add_eu();
        eu->set_role_id      (rhead->second->get()->role_id());
        eu->set_time         (rhead->second->get()->time());
        eu->set_place_id     (rhead->second->get()->place_id());
        eu->set_goods_id     (rhead->second->get()->goods_id());
        eu->set_goods_num    (rhead->second->get()->goods_num());
        eu->set_goods_quality(rhead->second->get()->goods_quality());
    }

    return 0;
}
