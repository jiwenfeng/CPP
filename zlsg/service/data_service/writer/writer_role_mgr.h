#ifndef __WRITER_ROLE_MGR_H__
#define __WRITER_ROLE_MGR_H__

#include "cmq_svr_msg.h"
#include "pb_mgr.h"
#include "cache_dwriter.h"
#include "pb_writer.h"

#define DEFAULT_WRITE_NUM 1024         // 每张表一次写入的最大数目
#define WRITER_TIMER_INTERVAL  "20"    // 200ms一次定时更新

class writer_role_mgr
{
public:
    writer_role_mgr();

    virtual ~writer_role_mgr();

public:
    int push(uint32 type, uint32 key1, uint32 key2, google::protobuf::Message* msg);

    int write();

    int write_all();

    inline void set_ctx(amtsvr_context* c)
    {
        ctx = c;
        cdw.attach(&cm, ctx);
    }

    inline amtsvr_context* get_ctx() {return ctx;}

    int close_all();

private:
    cache_dwriter     cdw;
    cache_mgr         cm;
    amtsvr_context*   ctx;
};

struct writer_t
{
    writer_role_mgr wrm;
};

#endif
