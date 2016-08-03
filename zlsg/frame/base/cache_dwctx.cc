//#include "amtsvr.h"
#include "cache_dwctx.h"
#include "pb_mgr.h"

int cache_dwctx::push(uint32 k, google::protobuf::Message* msg, uint32 type)
{
    CACHE_KEY key(k, 0);
    return push(key, msg, type);
}

int cache_dwctx::push(uint32 k1, uint32 k2, google::protobuf::Message* msg, uint32 type)
{
    CACHE_KEY key(k1, k2);
    return push(key, msg, type);
}
 
int cache_dwctx::push(CACHE_KEY& key, google::protobuf::Message* msg, uint32 type)
{
    //amtsvr_log("%d, %d, %d, %s\n", type, key.first, key.second, msg->DebugString().c_str());
    map<CACHE_KEY, cache_obj*>::iterator it = mapobjs.find(key);
    if (it != mapobjs.end()) {
        if (type == WOTE_REMOVE) {
            it->second->flag = type;
        }

        if (it->second->flag == WOTE_UPDATE) {
            if (type == WOTE_INSERT) {
                it->second->flag = type;
            }
        }

        if (it->second->msg != msg) {
            it->second->msg->CopyFrom(*msg);
            delete msg;
        }

        return 0;
    }

    cache_obj* p = new cache_obj(key, msg, type);
    if (head == NULL) {
        head = tail = p;
    } else {
        tail->next = p;
        tail = p;
    }

    mapobjs.insert(pair<CACHE_KEY, cache_obj*>(key, p));

    return 0;
}
 
google::protobuf::Message* cache_dwctx::pop(CACHE_KEY& key, uint32& type)
{
    if (head == NULL) {
        return NULL;
    }

    cache_obj* p = head;
    head = head->next;
    if (head == NULL) {
        tail = NULL;
    }
    
    google::protobuf::Message* msg = p->pop_msg();
    key = p->key;
    type = p->flag;
    
    mapobjs.erase(p->key);
    delete p;

    return msg;
}

