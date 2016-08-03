#include "raid_center_mgr.h"

raid_center_mgr::raid_center_mgr()
{
    _change = false;
}

raid_center_mgr::~raid_center_mgr()
{
}

#define CUR hash_map_obj<CACHE_KEY, cfg_cache_obj, CFG_CACHE_BULB_NUM>::pair_t

int  raid_center_mgr::init(struct cmq_t* l)
{
    CUR* cur = (CUR*)CONFMGR->get_raid_center_head();

    while (cur != NULL) {
        google::protobuf::Message* p = cur->mutable_val().pop_msg();
        mysql::tb_raid_center* tfl = static_cast<mysql::tb_raid_center*>(p);
        cur = (CUR*)cur->get_next();
        if (tfl != NULL) {
            _raid_center.insert(std::pair<uint32, mysql::tb_raid_center*>(tfl->raid_id(), tfl));
            _writer_type[tfl->raid_id()] = WOTE_UPDATE;
        }
    }

    return 0;
}

int  raid_center_mgr::save(amtsvr_context* ctx)
{
    if (!_change) {
        return -1;
    }

    std::map<uint32, mysql::tb_raid_center*>::iterator i = _raid_center.begin();
    for (; i != _raid_center.end(); ++i) {
        mysql::tb_raid_center* tfl = new mysql::tb_raid_center(*(i->second));
        writer_type = WOTE_NULL;
        std::map<uint32, WRITER_OPERATE_TYPE_E>::iterator it_type =  _writer_type.find(tfl->raid_id());
        if (it_type != _writer_type.end()){
             writer_type = it_type->second;
             it_type->second = WOTE_NULL;
        } else {
             _writer_type[tfl->raid_id()] = WOTE_NULL;
        } // 这里如果传入WOTE_NULL write函数会调用delete,再调用 tfl就是非法内存读， 麻烦看清楚再搞
        
        CACHE_KEY k(tfl->raid_id(), 0);
        write(ctx, k, tfl);
    }

    _change = false;
    
    return 0;
}

