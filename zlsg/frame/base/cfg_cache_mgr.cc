#include "pb_mgr.h"
#include "cfg_cache_mgr.h"

cfg_cache_mgr::cfg_cache_mgr()
{
    std::map<std::string, pb_obj*>& cfg_mappbs = PBMGR->get_cfg_pb_maps();
    std::map<std::string, pb_obj*>::iterator it = cfg_mappbs.begin();
    for (; it != cfg_mappbs.end(); ++it){
        cfg_cache_ctx ctx;
        size_t n = it->second->vec_key.size();
        for (size_t i = 0; i < n; ++i){
            ctx.set_key_name(it->second->vec_key[i]);
        }
        std::string type_name = it->second->msg->GetTypeName();
        mapctxs.insert(type_name, ctx);
    }
}

cfg_cache_mgr::~cfg_cache_mgr()
{
    hash_map_obj<std::string, cfg_cache_ctx, DEFAULT_CFG_PB_BLUB_NUM>::pair_t* head = mapctxs.get_head();
    while (head != NULL) {
        cfg_cache_ctx& ctx = head->mutable_val();
        ctx.release();
        head = head->get_next();
    }

    std::map<std::string, MapConfig_s*>::iterator i = mm.begin();
    while (i != mm.end()) {
        delete i->second;
        ++i;
    }

    mm.clear();
}


cfg_cache_ctx* cfg_cache_mgr::get_cache_ctx(const char* type_name)
{
    std::string name = type_name;
    return mapctxs.find(name);
}

std::map<std::string, TTableGroup_s*>* cfg_cache_mgr::get_cache_cfg()
{
    return &cfg;
}

std::map<std::string, MapConfig_s*>* cfg_cache_mgr::get_cache_map()
{
    return &mm;
}