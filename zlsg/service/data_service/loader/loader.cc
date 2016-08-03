#include "loader.h"
#include "loader_interface.h"

static int loader_cb(struct amtsvr_context* ctx, void* ud, int type, int session, uint32_t source, const void * msg, size_t sz)
{
    loader_t* l = (loader_t*)ud;

    if (type == PTYPE_SYSTEM) {
        if (memcmp(msg, CLOSE_ALL, strlen(CLOSE_ALL)) == 0) {
            amtsvr_syslog(ctx, "loader_service|will exit now|");
            amtsvr_command(ctx, "EXIT", NULL);
            return 0;
        }
    }

    if (type == PTYPE_RESPONSE) { // 处理sqlmgr的返回数据
        l->lr_mgr.proc_sqlmgr_rsp((struct cmq_svr_msg_t*)msg, session, source);
    } else {
        msg_handler* mi = static_cast<msg_handler*>((void*)msg);
        google::protobuf::Message* req = mi->msg;
        uint32 role_id = mi->ext1;
        mi->msg = NULL;
        MSG_HANDLER_POOL->push(mi);
        if (req == NULL) {
            return 1;
        }

        l->lr_mgr.proc_load_req(req,  role_id, session, source);
        return 1; // 不让底层删除请求包，因为在loader那里要attach
    }

    return 0;
}

struct loader_t* loader_create(void)
{
    loader_t* l = new loader_t;

    return l;
}

int loader_init(struct loader_t *l, struct amtsvr_context *ctx, char *args)
{
    l->ctx = ctx;

    amtsvr_callback(ctx, l, loader_cb);
    amtsvr_command(ctx, "REG", LOADER_NAME);

    return 0;
}

void loader_release(struct loader_t *l)
{
    amtsvr_log("\n");
    delete l;
}
