#ifndef __CACHE_DWRITE_CTX_H__
#define __CACHE_DWRITE_CTX_H__

#include "cache_ctx.h"

enum WRITER_OPERATE_TYPE_E
{
    WOTE_NULL   = 0, // �����κ����ݿ����
    WOTE_INSERT = 1, // ����
    WOTE_UPDATE = 2, // ����
    WOTE_REMOVE = 3, // ɾ��
};


// pb��mysql�Ļ�д��Ԫmap
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

    // push,pop�����в�������������cache��д
    int push(uint32 k, google::protobuf::Message* msg, uint32 type);
    int push(uint32 k1, uint32 k2, google::protobuf::Message* msg, uint32 type);
    int push(CACHE_KEY& key, google::protobuf::Message* msg, uint32 type);

    google::protobuf::Message* pop(CACHE_KEY& key, uint32& type);

private:
};


#endif
