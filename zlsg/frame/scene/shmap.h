#ifndef __SHMAP_H_0629__
#define __SHMAP_H_0629__

#include "scene_define.h"

enum SHOBJ_LIST_POINT_E
{
    SLPE_LIST  = 0, // 整体链表
    SLPE_TICK  = 1, // 定时器链表
    SLPE_MAX,
};

class scene_role;

struct shobj
{
    shobj* prev[SLPE_MAX];
    shobj* next[SLPE_MAX];
    shobj* fnext;
    shobj* hnext;
    scene_role* sr;

    shobj()
    {
        reset();
    }

    virtual ~shobj()
    {
        reset();
    }

    virtual void reset()
    {
        prev[SLPE_LIST] = NULL;
        prev[SLPE_TICK] = NULL;

        next[SLPE_LIST] = NULL;
        next[SLPE_TICK] = NULL;

        fnext = NULL;
        hnext = NULL;
        sr = NULL;
    }
};

#define DEFAULT_SHMAP_OBJ_NUM 3000  // 固定对象数目
#define DEFAULT_BULCKET_NUM   500   // 桶的个数

class shmap
{
public:
    shmap();

    virtual ~shmap();

    int init(bool br = true);

    int push(uint64 key, scene_role* sr);

    int remove(uint64 key);

    scene_role* mut(uint64 key);

    inline shobj* get_all_objs_head() { return all_objs_head; }
    inline const shobj* get_all_objs_head() const { return all_objs_head; }
    
    inline shobj* get_tick_objs_head() { return tick_objs_head; }

    int push_to_tick(uint64 key);

    int remove_from_tick(uint64 key);

    inline bool has_obj() const { return num > 0; }

    inline uint32 obj_num() const { return num; }

protected:
    bool check_exist(uint32 index, uint64 key, scene_role* sr);

    inline uint32 hash(uint64 key) { return (uint32)(key % DEFAULT_BULCKET_NUM); }
    
    inline bool check_space() { return free_objs_head != NULL; }

    shobj* alloc();

    shobj* find(uint64 key);

protected:
    uint32 num;
    shobj  objs[DEFAULT_SHMAP_OBJ_NUM]; // 对象池
    shobj* hashs[DEFAULT_BULCKET_NUM];  // 哈希桶池

    shobj* free_objs_head;
    shobj* all_objs_head;
    shobj* tick_objs_head;
};

#endif
