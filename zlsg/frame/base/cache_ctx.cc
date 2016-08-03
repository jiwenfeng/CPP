#include "cache_ctx.h"

void cache_ctx::set_key_name(const string& name)
{
    size_t n = veckey_name.size();
    if (n >= MAX_CACHE_KEY_NUM){
        return;
    }

    for (size_t i = 0; i < n; ++i){
        if (veckey_name[i] == name){
            return;
        }
    }

    veckey_name.push_back(name);
}

const vector<string>& cache_ctx::get_key_name()
{
    return veckey_name;
}

int cache_ctx::put(google::protobuf::Message* p)
{
    const google::protobuf::Reflection* ref  = p->GetReflection();
    const google::protobuf::Descriptor* desc = p->GetDescriptor();
    assert(ref != NULL && desc != NULL);

    size_t n = veckey_name.size();
    uint32 key[MAX_CACHE_KEY_NUM] = {0,0};
    for (size_t i = 0; i < n ; ++i){
        const google::protobuf::FieldDescriptor* field = desc->FindFieldByName(veckey_name[i]);
        if (field == NULL){
            return -1;
        }
        key[i] = ref->GetUInt32(*p, field);
    }

    CACHE_KEY k(key[0], key[1]);
    return put(k, p);
}

int cache_ctx::put(uint32 k, google::protobuf::Message* p)
{
    if (k == 0){
        return put(p);
    }

    CACHE_KEY key(k, 0);
    return put(key, p); 
}

int cache_ctx::put(uint32 k1, uint32 k2, google::protobuf::Message* p)
{
    if (k1 == 0 && k2 == 0){
        return put(p);
    } 
    
    CACHE_KEY key(k1, k2);
    return put(key, p); 
}

int cache_ctx::put(CACHE_KEY& key, google::protobuf::Message* p)
{
    cache_obj* po = NULL;
    map<CACHE_KEY, cache_obj*>::iterator it = mapobjs.find(key);
    if (it != mapobjs.end()){
        po = it->second;
        if (po->msg != p){
            delete po->msg;
            po->msg = p;
        }

        if (po->prev != NULL){
            po->prev->next = po->next;
            if (po->next != NULL){
                po->next->prev = po->prev;
            } else {
                tail = po->prev;
            }
            po->prev = NULL;
        } else { 
            return 0;
        }
    } else {
        po = new cache_obj(key, p);
        mapobjs.insert(pair<CACHE_KEY, cache_obj*>(key, po));
    }

    po->next = head;
    if (head != NULL){
        head->prev = po;
    }
    if (tail == NULL){
        tail = po;
    }
    head = po;

    return 0;
}

google::protobuf::Message* cache_ctx::get(uint32 k)
{
    CACHE_KEY key(k, 0);
    return get(key);
}

google::protobuf::Message* cache_ctx::get(uint32 k1, uint32 k2)
{
    CACHE_KEY key(k1, k2);
    return get(key);
}
 
google::protobuf::Message* cache_ctx::get(CACHE_KEY& key)
{
    map<CACHE_KEY, cache_obj*>::iterator it = mapobjs.find(key);
    if (it != mapobjs.end()){
        return it->second->msg;
    }
    return NULL;
}

