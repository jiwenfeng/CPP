#include "stdafx.h"
#include "amtsvr.h"
#include "amtsvr_server.h"
#include "amtsvr_module.h"
#include "amtsvr_handle.h"
#include "amtsvr_mq.h"
#include "amtsvr_timer.h"
#include "amtsvr_env.h"
#include "amtsvr_monitor.h"

#include <string.h>
#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#ifndef _WIN32
#include <stdbool.h>
#endif // _WIN32

#ifdef CALLING_CHECK

#define CHECKCALLING_BEGIN(ctx) assert(__sync_lock_test_and_set(&ctx->calling,1) == 0);
#define CHECKCALLING_END(ctx) __sync_lock_release(&ctx->calling);
#define CHECKCALLING_INIT(ctx) ctx->calling = 0;
#define CHECKCALLING_DECL int calling;

#else

#define CHECKCALLING_BEGIN(ctx)
#define CHECKCALLING_END(ctx)
#define CHECKCALLING_INIT(ctx)
#define CHECKCALLING_DECL

#endif

struct amtsvr_context {
    void * instance;
    struct amtsvr_module * mod;
    uint32_t handle;
    int ref;
    char result[32];
    void * cb_ud;
    amtsvr_cb cb;
    int session_id;
    uint32_t forward;
    struct message_queue *queue;
    bool init;
    bool endless;

    CHECKCALLING_DECL
};

static int g_total_context = 0;

int amtsvr_context_total() {
    return g_total_context;
}

static void _context_inc() {
    __sync_fetch_and_add(&g_total_context,1);
}

static void _context_dec() {
    __sync_fetch_and_sub(&g_total_context,1);
}

static void _id_to_hex(char * str, uint32_t id) {
    int i;
    static char hex[16] = { '0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F' };
    str[0] = ':';
    for (i=0;i<8;i++) {
        str[i+1] = hex[(id >> ((7-i) * 4))&0xf];
    }
    str[9] = '\0';
}

struct amtsvr_context* amtsvr_context_new(const char * name, const char *param) {
    struct amtsvr_module * mod = amtsvr_module_query(name);

    if (mod == NULL) {
        return NULL;
    }

    void *inst = amtsvr_module_instance_create(mod);
    if (inst == NULL) {
        return NULL;
    }

    struct amtsvr_context * ctx = (struct amtsvr_context *)malloc(sizeof(*ctx));
    CHECKCALLING_INIT(ctx);

    ctx->mod = mod;
    ctx->instance = inst;
    ctx->ref = 2;
    ctx->cb = NULL;
    ctx->cb_ud = NULL;
    ctx->session_id = 0;

    ctx->forward = 0;
    ctx->init = false;
    ctx->endless = false;
    ctx->handle = amtsvr_handle_register(ctx);
    struct message_queue * queue = ctx->queue = amtsvr_mq_create(ctx->handle);
    // init function maybe use ctx->handle, so it must init at last
    _context_inc();

    CHECKCALLING_BEGIN(ctx);
    int r = amtsvr_module_instance_init(mod, inst, ctx, param);
    CHECKCALLING_END(ctx);
    if (r == 0) {
        struct amtsvr_context * ret = amtsvr_context_release(ctx);
        if (ret) {
            ctx->init = true;
        }
        amtsvr_mq_force_push(queue);
        if (ret) {
            printf("[:%x] launch %s %s\n",ret->handle, name, param ? param : "");
        }
        return ret;
    } else {
        amtsvr_context_release(ctx);
        amtsvr_handle_retire(ctx->handle);
        return NULL;
    }
}

int amtsvr_context_newsession(struct amtsvr_context *ctx) {
    // session always be a positive number
    int session = (++ctx->session_id) & 0x7fffffff;
    return session;
}

void amtsvr_context_grab(struct amtsvr_context *ctx) {
    __sync_add_and_fetch(&ctx->ref,1);
}

static void _delete_context(struct amtsvr_context *ctx) {
    amtsvr_module_instance_release(ctx->mod, ctx->instance);
    amtsvr_mq_mark_release(ctx->queue);
    free(ctx);
    _context_dec();
}

struct amtsvr_context * amtsvr_context_release(struct amtsvr_context *ctx) {
    if (__sync_sub_and_fetch(&ctx->ref,1) == 0) {
        _delete_context(ctx);
        return NULL;
    }

    return ctx;
}

int amtsvr_context_push(uint32_t handle, struct amtsvr_message *message) {
    struct amtsvr_context * ctx = amtsvr_handle_grab(handle);
    if (ctx == NULL) {
        return -1;
    }

    amtsvr_mq_push(ctx->queue, message);
    amtsvr_context_release(ctx);

    return 0;
}

void amtsvr_context_endless(uint32_t handle) {
    struct amtsvr_context * ctx = amtsvr_handle_grab(handle);
    if (ctx == NULL) {
        return ;
    }

    ctx->endless = true;
    amtsvr_context_release(ctx);
}

static void _send_message(uint32_t des, struct amtsvr_message *msg) {
    if (amtsvr_context_push(des, msg)) {
        free(msg->data);
        amtsvr_syslog(NULL, "Drop message from %x forward to %x (size=%d)", msg->source, des, (int)msg->sz);
    }
}

static int _forwarding(struct amtsvr_context *ctx, struct amtsvr_message *msg) {
    if (ctx->forward) {
        uint32_t des = ctx->forward;
        ctx->forward = 0;
        _send_message(des, msg);
        return 1;
    }
    return 0;
}

static void _dispatch_message(struct amtsvr_context *ctx, struct amtsvr_message *msg) {
    assert(ctx->init);
    CHECKCALLING_BEGIN(ctx);
    int type = msg->sz >> HANDLE_REMOTE_SHIFT;
    size_t sz = msg->sz & HANDLE_MASK;
    int reserve = ctx->cb(ctx, ctx->cb_ud, type, msg->session, msg->source, msg->data, sz);
    reserve |= _forwarding(ctx, msg);
    if (!reserve) {
        free(msg->data);
    }
    CHECKCALLING_END(ctx);
}

int amtsvr_context_message_dispatch(struct amtsvr_monitor *sm) {
    struct message_queue * q = amtsvr_globalmq_pop();
    if (q == NULL) {
        return 1;
    }

    uint32_t handle = amtsvr_mq_handle(q);

    struct amtsvr_context * ctx = amtsvr_handle_grab(handle);
    if (ctx == NULL) {
        int s = amtsvr_mq_release(q);
        if (s > 0) {
            amtsvr_syslog(NULL, "Drop message queue %x (%d messages)", handle, s);
        }

        return 0;
    }

    struct amtsvr_message msg;
    if (amtsvr_mq_pop(q, &msg)) {
        amtsvr_context_release(ctx);
        return 0;
    }

    amtsvr_monitor_trigger(sm, msg.source , handle);

    if (ctx->cb == NULL) {
        free(msg.data);
        amtsvr_syslog(NULL, "Drop message from %x to %x without callback , size = %d", msg.source, handle, (int)msg.sz);
    } else {
        _dispatch_message(ctx, &msg);
    }

    assert(q == ctx->queue);
    amtsvr_mq_pushglobal(q);
    amtsvr_context_release(ctx);

    amtsvr_monitor_trigger(sm, 0, 0);

    return 0;
}


uint32_t amtsvr_queryname(struct amtsvr_context * context, const char * name) {
    switch(name[0]) {
    case ':': return strtoul(name+1,NULL,16);
    case '.': return amtsvr_handle_findname(name + 1);
    }
    amtsvr_syslog(context, "Don't support query global name %s",name);
    return 0;
}

const char * amtsvr_command(struct amtsvr_context * context, const char * cmd , const char * param) {
    if (strcmp(cmd,"TIMEOUT") == 0) {
        char * session_ptr = NULL;
        int ti = strtol(param, &session_ptr, 10);
        int session = amtsvr_context_newsession(context);
        amtsvr_timeout(context->handle, ti, session);
        sprintf(context->result, "%d", session);
        return context->result;
    }

    if (strcmp(cmd,"LOCK") == 0) {
        if (context->init == false) {
            return NULL;
        }
        amtsvr_mq_lock(context->queue, context->session_id+1);
        return NULL;
    }

    if (strcmp(cmd,"REG") == 0) {
        if (param == NULL || param[0] == '\0') {
            sprintf(context->result, ":%x", context->handle);
            return context->result;
        } else if (param[0] == '.') {
            return amtsvr_handle_namehandle(context->handle, param + 1);
        } 
        return NULL;
    }

    if (strcmp(cmd,"NAME") == 0) {
        int size = strlen(param);
        char* name   = new char[size+1];
        char* handle = new char[size+1];
        sscanf(param,"%s %s",name,handle);
        if (handle[0] != ':') {
            delete []name;
            delete []handle;
            return NULL;
        }

        uint32_t handle_id = strtoul(handle+1, NULL, 16);
        if (handle_id == 0) {
            delete []name;
            delete []handle;
            return NULL;
        }

        if (name[0] == '.') {
            const char* ret = amtsvr_handle_namehandle(handle_id, name + 1);
            delete []name;
            delete []handle;
            return ret;
        } 

        delete []name;
        delete []handle;
        return NULL;
    }

    if (strcmp(cmd,"NOW") == 0) {
        uint32_t ti = amtsvr_gettime();
        sprintf(context->result,"%u",ti);
        return context->result;
    }

    if (strcmp(cmd,"EXIT") == 0) {
        amtsvr_handle_retire(context->handle);
        return NULL;
    }

    if (strcmp(cmd,"KILL") == 0) {
        uint32_t handle = 0;
        if (param[0] == ':') {
            handle = strtoul(param+1, NULL, 16);
        } else if (param[0] == '.') {
            handle = amtsvr_handle_findname(param+1);
        } else {
            amtsvr_syslog(context, "Can't kill %s",param);
            // todo : kill global service
        }
        if (handle) {
            /*struct amtsvr_context * ret = */amtsvr_handle_retire(handle);
//             if (ret == NULL) {
//             }
        }
        return NULL;
    }

    if (strcmp(cmd,"LAUNCH") == 0) {
        size_t sz = strlen(param);
        char* tmp = new char[sz+1];
        strcpy(tmp,param);
        char * args = tmp;
        char * mod = strsep(&args, " \t\r\n");
        args = strsep(&args, "\r\n");
        struct amtsvr_context * inst = amtsvr_context_new(mod,args);
        if (inst == NULL) {
            fprintf(stderr, "Launch %s %s failed\n",mod,args);
            delete []tmp;
            return NULL;
        } else {
            _id_to_hex(context->result, inst->handle);
            delete []tmp;
            return context->result;
        }
    }

    if (strcmp(cmd,"GETENV") == 0) {
        return amtsvr_getenv(param);
    }

    if (strcmp(cmd,"SETENV") == 0) {
        size_t sz = strlen(param);
        char* key = new char[sz+1];
        int i;
        for (i=0;param[i] != ' ' && param[i];i++) {
            key[i] = param[i];
        }

        if (param[i] == '\0') {
            delete []key;
            return NULL;
        }

        key[i] = '\0';
        param += i+1;

        amtsvr_setenv(key,param);
        delete []key;
        return NULL;
    }

    if (strcmp(cmd,"STARTTIME") == 0) {
        uint32_t sec = amtsvr_gettime_fixsec();
        sprintf(context->result,"%u",sec);
        return context->result;
    }

    if (strcmp(cmd,"GROUP") == 0) {
        return NULL;
    }

    if (strcmp(cmd,"ENDLESS") == 0) {
        if (context->endless) {
            strcpy(context->result, "1");
            context->endless = false;
            return context->result;
        }
        return NULL;
    }

    if (strcmp(cmd,"ABORT") == 0) {
        amtsvr_handle_retireall();
        return NULL;
    }

    return NULL;
}

void amtsvr_forward(struct amtsvr_context * context, uint32_t destination) {
    assert(context->forward == 0);
    context->forward = destination;
}

static void _filter_args(struct amtsvr_context * context, int type, int *session, void ** data, size_t * sz) {
    int needcopy = !(type & PTYPE_TAG_DONTCOPY);
    int allocsession = type & PTYPE_TAG_ALLOCSESSION;
    type &= 0xff;

    if (allocsession) {
        assert(*session == 0);
        *session = amtsvr_context_newsession(context);
    }

    if (needcopy && *data) {
        char * msg = (char *)malloc(*sz+1);
        memcpy(msg, *data, *sz);
        msg[*sz] = '\0';
        *data = msg;
    }

    assert((*sz & HANDLE_MASK) == *sz);
    *sz |= type << HANDLE_REMOTE_SHIFT;
}

int amtsvr_send(struct amtsvr_context * context, uint32_t source, uint32_t destination , int type, int session, void * data, size_t sz) {
    _filter_args(context, type, &session, (void **)&data, &sz);

    if (source == 0) {
        source = context->handle;
    }

    if (destination == 0) {
        return session;
    }
    struct amtsvr_message smsg;
    smsg.source = source;
    smsg.session = session;
    smsg.data = data;
    smsg.sz = sz;

    if (amtsvr_context_push(destination, &smsg)) {
        free(data);
        amtsvr_syslog(NULL, "Drop message from %x to %x (type=%d)(size=%d)", source, destination, type, (int)(sz & HANDLE_MASK));
        return -1;
    }

    return session;
}

int amtsvr_broadcast(struct amtsvr_context * context, uint32_t * dests, int n, void* msg, size_t sz)
{
    if (n == 0){
        free(msg);
        return 0;
    }
    int i = 1;
    for (; i < n; ++i){
        void* buf = malloc(sz);
        memcpy(buf, msg, sz);
        amtsvr_send(context, 0, dests[i], PTYPE_TEXT|PTYPE_TAG_DONTCOPY, 0, buf, sz);
    }
    amtsvr_send(context, 0, dests[0], PTYPE_TEXT|PTYPE_TAG_DONTCOPY, 0, msg, sz);
    //struct amtsvr_multicast_message* multi_msg = amtsvr_multicast_create(msg, sz, context->handle);
    //amtsvr_multicast_cast(context, multi_msg, dests, n);
    return 0;
}

int amtsvr_sendname(struct amtsvr_context * context, const char * addr , int type, int session, void * data, size_t sz) {
    uint32_t source = context->handle;
    uint32_t des = 0;
    if (addr[0] == ':') {
        des = strtoul(addr+1, NULL, 16);
    } else if (addr[0] == '.') {
        des = amtsvr_handle_findname(addr + 1);
        if (des == 0) {
            if (type & PTYPE_TAG_DONTCOPY){
                free(data);
            }
            amtsvr_syslog(context, "Drop message to %s", addr);
            return session;
        }
    } else {
        if (type & PTYPE_TAG_DONTCOPY){
            free(data);
        }
        amtsvr_syslog(context, "Drop message to %s", addr);
        return session;
    }

    return amtsvr_send(context, source, des, type, session, data, sz);
}

uint32_t amtsvr_context_handle(struct amtsvr_context *ctx) {
    return ctx->handle;
}

void amtsvr_context_init(struct amtsvr_context *ctx, uint32_t handle) {
    ctx->handle = handle;
}

void amtsvr_callback(struct amtsvr_context * context, void *ud, amtsvr_cb cb) {
    context->cb = cb;
    context->cb_ud = ud;
}

void amtsvr_context_send(struct amtsvr_context * ctx, void * msg, size_t sz, uint32_t source, int type, int session) {
    struct amtsvr_message smsg;
    smsg.source = source;
    smsg.session = session;
    smsg.data = msg;
    smsg.sz = sz | type << HANDLE_REMOTE_SHIFT;

    amtsvr_mq_push(ctx->queue, &smsg);
}

int get_amtsvr_ctx_mq_count(struct amtsvr_context* ctx)
{
    return get_amtsvr_mq_count(ctx->queue);
}

void* get_amtsvr_ctx_mq_data(struct amtsvr_context* ctx, int index)
{
    return get_amtsvr_mq_data(ctx->queue, index);
}
