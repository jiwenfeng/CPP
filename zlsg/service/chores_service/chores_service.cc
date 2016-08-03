#include "chores_interface.h"

static int chores_timer_cb(struct amtsvr_context* ctx, chores_service_t* c, int type, int session, uint32_t source, const void* msg, size_t sz)
{
    c->tenms = time_utils::tenms();

    amtsvr_command(c->ctx, "TIMEOUT", CHORES_TIMER_INTERVAL);
    return 0;
}

static int chores_cb(struct amtsvr_context* ctx, void* ud, int type, int session, uint32_t source, const void* msg, size_t sz)
{
    chores_service_t* c = (chores_service_t*)ud;
    if (type == PTYPE_TIMER){
        return chores_timer_cb(ctx, c, type, session, source, msg, sz);
    }

    msg_handler* mh = static_cast<msg_handler*>((void*)msg);
    google::protobuf::Message* pb = mh->msg;
    mh->msg = NULL;
    MSG_HANDLER_POOL->push(mh);
    if (pb == NULL){
        return 1;
    }

    size_t rsp_len = 0;
    void* rsp = c->pd.on_pb_msg(pb, c, &rsp_len);
    delete pb;
    if (rsp != NULL){
        amtsvr_send(c->ctx, 0, source, PTYPE_CLIENT|PTYPE_TAG_DONTCOPY, session, rsp, rsp_len);
    }  

    return 1;
}

struct chores_service_t* chores_create(void)
{
    chores_service_t* c = new chores_service_t;

    return c;
}

int chores_init(struct chores_service_t* c, struct amtsvr_context* ctx, char* args)
{
    c->ctx = ctx;
    chores_dispatcher_init(c);
    c->fa_mgr.attach_ctx(ctx);
    c->fa_mgr.init();
    c->self = strtoul(amtsvr_command(ctx, "REG", CHORES_NAME)+1 , NULL, 16);
    
    amtsvr_callback(ctx, (void*)c, chores_cb);
    //amtsvr_command(c->ctx, "TIMEOUT", CHORES_TIMER_INTERVAL); // 目前来看不需要启动定时器驱动

    return 0;
}

void chores_release(struct chores_service_t* c)
{
    delete c;
}

