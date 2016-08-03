#include "cache_mgr.h"
#include "pb_mgr.h"

void cache_mgr::get_dw_ctxs(vector<cache_dwctx*>& cdw)
{
    map<string, cache_dwctx*>::iterator it = mapdctxs.begin();
    for (; it != mapdctxs.end(); ++it){
        cdw.push_back(it->second);
    }
}

cache_mgr::cache_mgr()
{
    map<string, pb_obj*>::iterator it;
    /*map<string, pb_obj*>& cfg_mappbs = PBMGR->get_cfg_pb_maps();
    it = cfg_mappbs.begin();
    for (; it != cfg_mappbs.end(); ++it){
        cache_ctx *ctx = new cache_ctx();
        size_t n = it->second->vec_key.size();
        for (size_t i = 0; i < n; ++i){
            ctx->set_key_name(it->second->vec_key[i]);
        }
        reg(it->second->msg->GetTypeName(), ctx);
    }*/

    map<string, pb_obj*>& data_mappbs = PBMGR->get_data_pb_maps();
    it = data_mappbs.begin();
    for (; it != data_mappbs.end(); ++it){
        cache_dwctx *dwctx = new cache_dwctx;
        size_t n = it->second->vec_key.size();
        for (size_t i = 0; i < n; ++i){
            dwctx->set_key_name(it->second->vec_key[i]);
        }

        if (reg_dwctx(it->second->msg->GetTypeName(), dwctx) != 0) {
            delete dwctx;
        }
    }
}

cache_mgr::~cache_mgr()
{
    map<string, cache_ctx*>::iterator it = mapctxs.begin();
    while (it != mapctxs.end()) {
        if (it->second != NULL) {
            delete it->second;
        }
        ++it;
    }
    mapctxs.clear();

    map<string, cache_dwctx*>::iterator i = mapdctxs.begin();
    while (i != mapdctxs.end()) {
        if (i->second != NULL) {
            delete i->second;
        }
        ++i;
    }
    mapdctxs.clear();
}

int cache_mgr::reg(const string& type_name, cache_ctx *ctx)
{
    map<string, cache_ctx*>::iterator it = mapctxs.find(type_name);
    if (it != mapctxs.end()){
        return -1;
    }

    mapctxs.insert(pair<string, cache_ctx*>(type_name, ctx));
    return 0;
}


cache_ctx* cache_mgr::get_cache_ctx(const char* type_name)
{
     map<string, cache_ctx*>::iterator it = mapctxs.find(type_name);
     if (it == mapctxs.end()){
         return NULL;
     }
     return it->second;
}


int cache_mgr::reg_dwctx(const string& type_name, cache_dwctx *dwctx)
{
    map<string, cache_dwctx*>::iterator it = mapdctxs.find(type_name);
    if (it != mapdctxs.end()){
        return -1;
    }

    mapdctxs.insert(pair<string, cache_dwctx*>(type_name, dwctx));

    return 0;
}

cache_dwctx* cache_mgr::get_dwctx(const char* type_name)
{
    map<string, cache_dwctx*>::iterator it = mapdctxs.find(type_name);
    if (it == mapdctxs.end()) {
        return NULL;
    }

    return it->second;
}
