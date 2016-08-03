#include "scene_role.h"
#include "shmap.h"

shmap::shmap()
{
    init();
}

shmap::~shmap()
{
}

bool shmap::check_exist(uint32 index, uint64 key, scene_role* sr)
{
    bool ret = false;
    for (shobj* osr = hashs[index]; osr != NULL; osr = osr->hnext){
        if (osr->sr->get_inst_id() == key){
            ret = true;
            break;
        }
    }

    return ret;
}

int shmap::push(uint64 key, scene_role* sr)
{
    uint32 index = hash(key);
    if (check_exist(index, key, sr)){
        return 0;
    }

    shobj* new_sr = alloc();
    if (new_sr == NULL){
        return -1;
    }

    ++num;
    new_sr->sr = sr;

    // 处理冲突链表
    new_sr->hnext = hashs[index];
    hashs[index] = new_sr;

    // 处理整体链表
    new_sr->next[SLPE_LIST] = all_objs_head;
    if (all_objs_head != NULL){
        all_objs_head->prev[SLPE_LIST] = new_sr;
    }
    all_objs_head = new_sr;

    return 0;
}

shobj* shmap::alloc()
{
    if (free_objs_head == NULL){
        return NULL;
    }

    shobj* tmp = free_objs_head;
    free_objs_head = tmp->fnext;
    tmp->fnext = NULL;

    return tmp;
}

int shmap::remove(uint64 key)
{
    uint32 index = hash(key);
    shobj* prev = NULL;
    shobj* cur = hashs[index];
    while (cur != NULL) {
        if (cur->sr->get_inst_id() == key) {
            break;
        }

        prev = cur;
        cur = cur->hnext;
    }

    if (cur == NULL) {
        return -1;
    }

    --num;

    // 处理掉哈希冲突的挂载
    if (prev != NULL) {
        prev->hnext = cur->hnext;
    } else {
        hashs[index] = cur->hnext;
    }

    // 处理掉全部数据表,一定是在全部数据中
    if (cur->prev[SLPE_LIST] == NULL) {
        all_objs_head = cur->next[SLPE_LIST];
    } else {
        cur->prev[SLPE_LIST]->next[SLPE_LIST] = cur->next[SLPE_LIST];
    }

    if (cur->next[SLPE_LIST] != NULL) {
        cur->next[SLPE_LIST]->prev[SLPE_LIST] = cur->prev[SLPE_LIST];
    }

    // 处理掉定时器表,先要确认是否在定时器中
    if (cur->next[SLPE_TICK] != NULL || cur->prev[SLPE_TICK] != NULL || tick_objs_head == cur) {
        if (cur->prev[SLPE_TICK] == NULL){
            tick_objs_head = cur->next[SLPE_TICK];
        } else {
            cur->prev[SLPE_TICK]->next[SLPE_TICK] = cur->next[SLPE_TICK];
        }
        if (cur->next[SLPE_TICK] != NULL){
            cur->next[SLPE_TICK]->prev[SLPE_TICK] = cur->prev[SLPE_TICK];
        }
    }

    // 重置数据，回归空闲链表
    cur->reset();
    cur->fnext = free_objs_head;
    free_objs_head = cur;

    return 0;
}

scene_role* shmap::mut(uint64 key)
{
    shobj* obj = find(key);
    if (obj == NULL) {
        return NULL;
    }

    return obj->sr;
}

shobj* shmap::find(uint64 key)
{
    uint32 index = hash(key);
    for (shobj* osr = hashs[index]; osr != NULL; osr = osr->hnext) {
        if (osr->sr->get_inst_id() == key) {
            return osr;
        }
    }

    return NULL;
}

int shmap::push_to_tick(uint64 key)
{
    shobj* obj = find(key);
    if (obj == NULL) {
        return -1;
    }

    if (obj->next[SLPE_TICK] != NULL || obj->prev[SLPE_TICK] != NULL 
        || tick_objs_head == obj) {
            return 0;
    }

    obj->next[SLPE_TICK] = tick_objs_head;
    if (tick_objs_head != NULL) {
        tick_objs_head->prev[SLPE_TICK] = obj;
    }

    tick_objs_head = obj;

    return 0;
}

int shmap::remove_from_tick(uint64 key)
{
    shobj* obj = find(key);
    if (obj == NULL) {
        return -1;
    }

    if (obj->next[SLPE_TICK] == NULL && obj->prev[SLPE_TICK] == NULL 
        && tick_objs_head != obj) {
            return 0;
    }

    if (obj->prev[SLPE_TICK] == NULL) {
        tick_objs_head = obj->next[SLPE_TICK];
    } else {
        obj->prev[SLPE_TICK]->next[SLPE_TICK] = obj->next[SLPE_TICK];
    }
    if (obj->next[SLPE_TICK] != NULL) {
        obj->next[SLPE_TICK]->prev[SLPE_TICK] = obj->prev[SLPE_TICK];
    }

    obj->prev[SLPE_TICK] = NULL;
    obj->next[SLPE_TICK] = NULL;

    return 0;
}

int shmap::init(bool br)
{
    num = 0;
    memset(hashs, 0, sizeof(hashs));

    int i = DEFAULT_SHMAP_OBJ_NUM - 1;
    free_objs_head = &(objs[i]);
    for (; i > 0; --i) {
        if (br){
            objs[i].reset();
        }
        objs[i].fnext = &(objs[i-1]); 
    }
    objs[0].reset();

    all_objs_head = NULL;
    tick_objs_head = NULL;

    return 0;
}
