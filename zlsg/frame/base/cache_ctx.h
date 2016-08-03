#ifndef __CACHE_CONTEXT_H__
#define __CACHE_CONTEXT_H__

#include <map>
#include <vector>

#include "define.h"
#include "pb_coder.h"

using namespace std;

struct cache_obj_t
{
    CACHE_KEY key;
    google::protobuf::Message* msg;
    uint32 flag;
};

class cache_obj :public cache_obj_t
{
public:   
    cache_obj *prev; 
    cache_obj *next;

public:
    cache_obj()
    {
        msg = NULL;
        prev = NULL;
        next = NULL;
        flag = 0;
    }

    cache_obj(CACHE_KEY& k, google::protobuf::Message* p, uint32 f = 0)
    {
        key = k;
        msg = p;
        prev = NULL;
        next = NULL;
        flag = f;
    }

    virtual ~cache_obj()
    {
        
    }

    inline google::protobuf::Message* pop_msg()
    {
        google::protobuf::Message* tmp = msg;
        msg = NULL;
        return tmp;
    }
};

class cache_ctx
{
public:
    cache_ctx()
    {
        head = NULL;
        tail = NULL;
    }

    virtual ~cache_ctx()
    {
    }

    void set_key_name(const string& name);
    const vector<string>& get_key_name();

    cache_obj* get_obj_head()
    {
        return head;
    }

    cache_obj* get_obj_tail()
    {
        return tail;
    }

    int put(google::protobuf::Message* p);
    int put(uint32 k, google::protobuf::Message* p);
    int put(uint32 k1, uint32 k2, google::protobuf::Message* p);
    int put(CACHE_KEY& key, google::protobuf::Message* p);

    google::protobuf::Message* get(uint32 k);
    google::protobuf::Message* get(uint32 k1, uint32 k2);
    google::protobuf::Message* get(CACHE_KEY& key);


protected:
    map<CACHE_KEY, cache_obj*> mapobjs; 
    cache_obj* head;
    cache_obj* tail;
    vector<string> veckey_name;
};



#endif
