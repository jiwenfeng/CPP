#ifndef __PB_WRITER_H__
#define __PB_WRITER_H__

#include "amtsvr.h"
#include "msg_handler_pool.h"
#include "cache_dwctx.h"

class pb_writer
{
public:
    pb_writer()
    {
        writer_type = WOTE_UPDATE;
    }

    virtual ~pb_writer()
    {
    }

    inline int write(amtsvr_context* ctx, CACHE_KEY& key, google::protobuf::Message* msg)
    {
        if (writer_type == WOTE_NULL) {
            delete msg;
            return -1;
        }

        msg_handler* mi = MSG_HANDLER_POOL->pop();
        if (mi == NULL) {
            delete msg;
            return -2;
        }
        
        mi->msg = msg;
        mi->flag = writer_type;
        mi->ext1 = key.first;
        mi->ext2 = key.second;

        return amtsvr_sendname(ctx, WRITER_NAME, PTYPE_CLIENT|PTYPE_TAG_DONTCOPY, 0, (void*)mi, sizeof(msg_handler));
    }

    inline void set_type(WRITER_OPERATE_TYPE_E wt)
    {
        writer_type = wt;
    }

    inline pb_writer& operator=(const pb_writer& rhs)
    {
        writer_type = rhs.writer_type;
        return *this;
    }

protected:
    WRITER_OPERATE_TYPE_E writer_type;
};

#endif
