#include "writer.h"
#include "writer_role_mgr.h"
#include "msg_handler_pool.h"

static int writer_cb(struct amtsvr_context* ctx, void* ud, int type, int session, uint32_t source, const void * msg, size_t sz)
{
    writer_t* w = (writer_t*)ud;
    if (type == PTYPE_TIMER) {
        w->wrm.write();
        amtsvr_command(w->wrm.get_ctx(), "TIMEOUT", WRITER_TIMER_INTERVAL);

        return 0;
    } else if (type == PTYPE_SYSTEM) {
        const char* cmd = (const char*)msg;
        if (strcmp(cmd, "write_all") == 0) {
            w->wrm.write_all();
            w->wrm.close_all();
            amtsvr_syslog(ctx, "writer_service|will exit now|");

            amtsvr_sendname(ctx, SYS_LOG_NAME, PTYPE_SYSTEM, 0, (void*)"write_all", sizeof("write_all"));
            amtsvr_sendname(ctx, BILL_LOG_NAME, PTYPE_SYSTEM, 0, (void*)"write_all", sizeof("write_all"));

            amtsvr_command(ctx, "EXIT", NULL);
        }

        return 0;
    }

    msg_handler* mi = static_cast<msg_handler*>((void*)msg);
    uint32 opt_type = mi->flag;
    uint32 key1 = mi->ext1;
    uint32 key2 = mi->ext2;
    google::protobuf::Message* req = mi->msg;
    mi->msg = NULL;
    MSG_HANDLER_POOL->push(mi);
    if (req == NULL) {
        return 1;
    }

    w->wrm.push(opt_type, key1, key2, req);

    return 1;
}

struct writer_t* writer_create(void)
{
    writer_t* w = new writer_t;

    return w;
}

int writer_init(struct writer_t *w, struct amtsvr_context *ctx, char *args)
{
    w->wrm.set_ctx(ctx);

    amtsvr_callback(ctx, w, writer_cb);
    amtsvr_command(ctx, "REG", WRITER_NAME);
    amtsvr_command(w->wrm.get_ctx(), "TIMEOUT", WRITER_TIMER_INTERVAL);

    return 0;
}

void writer_release(struct writer_t *w)
{
    amtsvr_log("\n");
    delete w;
}
