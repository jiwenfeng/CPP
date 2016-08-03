#ifndef __CFG_CACHE_CONTEXT_H__
#define __CFG_CACHE_CONTEXT_H__

#include "define.h"
#include "hash_map_obj.h"

class cfg_cache_obj
{
public:
    cfg_cache_obj();
    cfg_cache_obj(google::protobuf::Message* p);
    virtual ~cfg_cache_obj();

public:
    const google::protobuf::Message* get_msg() const;
    google::protobuf::Message* pop_msg();    // 强制转换，仅用于启动时加载数据的模块使用，例如邮件中心

    void release();

private:
    const google::protobuf::Message* msg;
};

class cfg_cache_ctx
{
public:
    cfg_cache_ctx();
    virtual ~cfg_cache_ctx();

public:
    inline const hash_map_obj<CACHE_KEY, cfg_cache_obj, CFG_CACHE_BULB_NUM>::pair_t* get_obj_head() const {return mapobjs.get_head();};

    int put(google::protobuf::Message* p);
    int put(uint32 k, google::protobuf::Message* p);
    int put(uint32 k1, uint32 k2, google::protobuf::Message* p);
    int put(CACHE_KEY& key, google::protobuf::Message* p);

    const google::protobuf::Message* get(uint32 k) const;
    const google::protobuf::Message* get(uint32 k1, uint32 k2) const;
    const google::protobuf::Message* get(CACHE_KEY& key) const;

    void set_key_name(const string& name);

    void release();

protected:
    hash_map_obj<CACHE_KEY, cfg_cache_obj, CFG_CACHE_BULB_NUM> mapobjs;

    uint32 key_name_count;
    string key_names[MAX_CACHE_KEY_NUM];
};

#endif
