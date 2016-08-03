#ifndef __CFG_CACHE_MGR_H__
#define __CFG_CACHE_MGR_H__

#include "cfg_cache_ctx.h"
#include "BinTable.h"
#include "MapConfig.h"

#define DEFAULT_CFG_PB_BLUB_NUM 300

class cfg_cache_mgr
{
public:
    cfg_cache_mgr();

    virtual ~cfg_cache_mgr();

    cfg_cache_ctx* get_cache_ctx(const char* type_name);

    std::map<std::string, TTableGroup_s*>* get_cache_cfg();
    std::map<std::string, MapConfig_s*>*   get_cache_map();

private:
    hash_map_obj<string, cfg_cache_ctx, DEFAULT_CFG_PB_BLUB_NUM> mapctxs;
    std::map<std::string, TTableGroup_s*> cfg;
    std::map<std::string, MapConfig_s*>   mm;
};

#endif
