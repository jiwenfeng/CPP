#ifndef __CACHE_DWRITE_CTX_H__
#define __CACHE_DWRITE_CTX_H__

#include "cache_ctx.h"

enum WRITER_OPERATE_TYPE_E
{
    WOTE_NULL   = 0, // 不做任何数据库操作
    WOTE_INSERT = 1, // 插入
    WOTE_UPDATE = 2, // 更新
    WOTE_REMOVE = 3, // 删除
};


// pb到mysql的缓写单元map
class cache_dwctx : public cache_ctx
{
public:
    cache_dwctx()
    {
        head = NULL;
        tail = NULL;
    }

    virtual ~cache_dwctx()
    {
    }

    // push,pop，队列操作方法，用于cache缓写
    int push(uint32 k, google::protobuf::Message* msg, uint32 type);
    int push(uint32 k1, uint32 k2, google::protobuf::Message* msg, uint32 type);
    int push(CACHE_KEY& key, google::protobuf::Message* msg, uint32 type);

    google::protobuf::Message* pop(CACHE_KEY& key, uint32& type);

private:
};


#endif
