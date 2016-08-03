// Notice : 只能单线程持有，未做多线程互斥保护
#ifndef __CACHE_MGR_H__
#define __CACHE_MGR_H__

#include "cache_ctx.h"
#include "cache_dwctx.h"

class cache_mgr
{
public:
    cache_mgr();

    virtual ~cache_mgr();

    int reg(const string& type_name, cache_ctx *ctx);

    cache_ctx* get_cache_ctx(const char* type_name);

    int reg_dwctx(const string& type_name, cache_dwctx *dwctx);

    cache_dwctx* get_dwctx(const char* type_name);

    void get_dw_ctxs(vector<cache_dwctx*>& cdw);

private:
    map<string, cache_ctx*>        mapctxs;  // 内存中的数据cache
    map<string, cache_dwctx*>      mapdctxs; // 缓写数据队列
};

#endif

