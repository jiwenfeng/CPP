#include "google/protobuf/message.h"
#include "cfg_cache_ctx.h"

cfg_cache_obj::cfg_cache_obj()
    : msg(NULL)
{
}

cfg_cache_obj::cfg_cache_obj(google::protobuf::Message* p)
    : msg(p)
{
}

cfg_cache_obj::~cfg_cache_obj()
{
}

const google::protobuf::Message* cfg_cache_obj::get_msg() const
{
    return msg;
}

google::protobuf::Message* cfg_cache_obj::pop_msg()
{
    google::protobuf::Message* tmp = const_cast<google::protobuf::Message*>(msg);
    msg = NULL;
    return tmp;
}

void cfg_cache_obj::release()
{
    if (msg != NULL) {
        delete msg;
        msg = NULL;
    }
}

cfg_cache_ctx::cfg_cache_ctx()
{
    key_name_count = 0;
}

cfg_cache_ctx::~cfg_cache_ctx()
{

}

void cfg_cache_ctx::set_key_name(const string& name)
{
    if (key_name_count >= MAX_CACHE_KEY_NUM) {
        return;
    }

    for (size_t i = 0; i < key_name_count; ++i) {
        if (key_names[i] == name){
            return;
        }
    }

    key_names[key_name_count++] = name;
}

int cfg_cache_ctx::put(google::protobuf::Message* p)
{
    const google::protobuf::Reflection* ref  = p->GetReflection();
    const google::protobuf::Descriptor* desc = p->GetDescriptor();
    assert(ref != NULL && desc != NULL);

    uint32 key[MAX_CACHE_KEY_NUM] = {0,0};
    for (size_t i = 0; i < key_name_count ; ++i) {
        const google::protobuf::FieldDescriptor* field = desc->FindFieldByName(key_names[i]);
        if (field == NULL) {
            return -1;
        }

        key[i] = ref->GetUInt32(*p, field);
    }

    CACHE_KEY k(key[0], key[1]);
    return put(k, p);
}

int cfg_cache_ctx::put(uint32 k, google::protobuf::Message* p)
{
    if (k == 0) {
        return put(p);
    }

    CACHE_KEY key(k, 0);
    return put(key, p);
}

int cfg_cache_ctx::put(uint32 k1, uint32 k2, google::protobuf::Message* p)
{
    if (k1 == 0 && k2 == 0) {
        return put(p);
    } 

    CACHE_KEY key(k1, k2);
    return put(key, p); 
}

int cfg_cache_ctx::put(CACHE_KEY& key, google::protobuf::Message* p)
{
    cfg_cache_obj obj(p);
    return mapobjs.insert(key, obj);
}

const google::protobuf::Message* cfg_cache_ctx::get(uint32 k) const
{
    CACHE_KEY key(k, 0);
    return get(key);
}

const google::protobuf::Message* cfg_cache_ctx::get(uint32 k1, uint32 k2) const
{
    CACHE_KEY key(k1, k2);
    return get(key);
}

const google::protobuf::Message* cfg_cache_ctx::get(CACHE_KEY& key) const
{
    const cfg_cache_obj* obj = mapobjs.find(key);
    if (obj == NULL) {
        return NULL;
    }

    return obj->get_msg();
}

void cfg_cache_ctx::release()
{
    hash_map_obj<CACHE_KEY, cfg_cache_obj, CFG_CACHE_BULB_NUM>::pair_t* head = mapobjs.get_head();
    while (head != NULL) {
        cfg_cache_obj& obj = head->mutable_val();
        obj.release();
        head = head->get_next();
    }
}
