// 仅支持用于配置初始化后一次性写入，后面不再变更的需求模块，多线程无加锁共享读
#ifndef __HASH_MAP_OBJ_H__
#define __HASH_MAP_OBJ_H__

#include <string>
#include "define.h"

using namespace std;

#define CFG_CACHE_BULB_NUM 1000

template<typename KEY, typename VALUE, size_t BUCK_NUM>
class hash_map_obj
{
public:
    struct pair_t
    {
    public:
        KEY     key;
        VALUE   val;
        struct pair_t* next[2]; // 0 表示冲突链的下一指针 1 表示整个链表的指针

        pair_t()
        {
            next[0] = NULL;
            next[1] = NULL;
        }

        ~pair_t()
        {
            
        }

        const struct pair_t* get_next() const
        {
            return next[1];
        }

        struct pair_t* get_next()
        {
            return next[1];
        }

        const VALUE& get_val() const
        {
            return val;
        }

        VALUE& mutable_val()
        {
            return val;
        }

        const KEY& get_key() const
        {
            return key;
        }

    };

public:
    hash_map_obj()
    {
        count = 0;
        memset(objs, 0 ,sizeof(objs));
        
        head = NULL;
    }

    virtual ~hash_map_obj()
    {
        pair_t* tmp;
        pair_t* cur = head;
        while (cur != NULL){
            tmp = cur->next[1];
            delete cur;
            cur = tmp;
        }
        head = NULL;
    }

    int insert(KEY& k, VALUE& v)
    {
        size_t index = hash(k) % BUCK_NUM;
        pair_t* cur = objs[index];        
        while (cur != NULL){
            if (cur->key == k){
                return -1;
            }

            cur = cur->next[0];
        }

        pair_t* obj = new pair_t;
        obj->key = k;
        obj->val = v;

        obj->next[0] = objs[index];
        objs[index] = obj;

        obj->next[1] = head;
        head = obj;

        ++count;

        return 0;
    }

    const VALUE* find(KEY& k) const
    {
        size_t index = hash(k) % BUCK_NUM;
        VALUE* v = NULL;
        pair_t* cur = objs[index];
        while (cur != NULL){
            if (cur->key == k){
                v = &(cur->val);
                break;
            }
            cur = cur->next[0];
        }

        return v;
    }

    VALUE* find(KEY& k)
    {
        size_t index = hash(k) % BUCK_NUM;
        VALUE* v = NULL;
        pair_t* cur = objs[index];
        while (cur != NULL){
            if (cur->key == k){
                v = &(cur->val);
                break;
            }
            cur = cur->next[0];
        }

        return v;
    }

    const pair_t* get_head() const { return head; }
    pair_t* get_head() { return head; }

private:
    size_t hash(const char* k) const
    {
        unsigned long ul = 0;
        for ( ; *k; ++k)
        {
            ul = 5 * ul + *k;
        }

        return size_t(ul);
    }

    size_t hash(const string& k) const
    {
        return hash(k.c_str());
    }
    
    size_t hash(uint32 k) const
    {
        return k;
    }

    size_t hash(int k) const
    {
        return k;
    }
    
    size_t hash(uint64 k) const
    {
        return UINT64_LOW(k) * UINT64_HIGH(k) & 0xFFFFFFFF;
    }

    size_t hash(int64 k) const
    {
        return UINT64_LOW(k) * UINT64_HIGH(k) & 0xFFFFFFFF;
    }

    size_t hash(std::pair<uint32, uint32>& k) const
    {
        uint32 high = k.first;
        uint32 low = k.second;
        uint64 tmp = UINT64_MAKE(high, low);
        return hash(tmp);
    }

private:
    uint32  count;
    pair_t* objs[BUCK_NUM];
    
    pair_t* head;
};


#endif
