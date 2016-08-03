#include "role_service.h"
#include "role_interface.h"
#include "msg_handler_pool.h"

void proc_auths_rsp(role_service_t* a, struct cmq_svr_msg_t* msg, int session, uint32_t source);

static int role_cb(struct amtsvr_context* ctx, void* ud, int type, int session, uint32_t source, const void* msg, size_t sz)
{
    role_service_t* a = (role_service_t*)ud;

    if (type == PTYPE_RESPONSE) { // 处理sqlmgr的返回数据
        struct cmq_svr_msg_t* pm = static_cast<struct cmq_svr_msg_t*>((void*)msg);
        proc_auths_rsp(a, pm, session, source);
        // 释放msg
        delete pm;
        return 1;
    } else if (type == PTYPE_SYSTEM) {
        if (memcmp(msg, CLOSE_ALL, strlen(CLOSE_ALL)) == 0) {
            amtsvr_syslog(ctx, "role_service|will exit now|");
            amtsvr_command(ctx, "EXIT", NULL);
            return 0;
        }
    }

    msg_handler* mh = static_cast<msg_handler*>((void*)msg);
    google::protobuf::Message* pb = mh->msg;
    mh->msg = NULL;
    MSG_HANDLER_POOL->push(mh);
    if (pb == NULL) {
        return 1;
    }

    size_t rsp_len = 0;
    void* rsp = a->pd.on_pb_msg(pb, a, &rsp_len);
    delete pb;
    if (rsp != NULL) {
        amtsvr_send(ctx, 0, source, PTYPE_CLIENT|PTYPE_TAG_DONTCOPY, session, rsp, rsp_len);
    }

    return 1;
}

struct role_service_t* role_create(void)
{
    struct role_service_t* a = new role_service_t;

    return a;
}

int role_init(struct role_service_t* a, struct amtsvr_context* ctx, char* args)
{
    a->ctx = ctx;

    if (role_dispatcher_init(a) != 0) {
        return 1;
    }

    amtsvr_callback(ctx, (void*)a, role_cb);
    amtsvr_command(ctx, "REG", ROLE_NAME);

    const hash_map_obj<CACHE_KEY, cfg_cache_obj, CFG_CACHE_BULB_NUM>::pair_t* cur = CONFMGR->get_gamesvr_head();
    while (cur != NULL) {
        const mysql::tb_gamesvr* tg = static_cast<const mysql::tb_gamesvr*>(cur->get_val().get_msg());
        cur = cur->get_next();
        a->push_svr(tg->svr_id()); 
    }

    cur = CONFMGR->get_gdata_max_role_head();
    while (cur != NULL) {
        const mysql::sp_get_gdata_max_role* sggmr = static_cast<const mysql::sp_get_gdata_max_role*>(cur->get_val().get_msg());
        cur = cur->get_next();
        a->push(sggmr->max_role_id()); 
        //printf("max_role_id %d\n", sggmr->max_role_id());
    }

    msg_handler* mh = MSG_HANDLER_POOL->pop();
    if (mh != NULL) {
        server::init_data_req* idr = new server::init_data_req;
        idr->set_time((uint32)time(NULL));
        mh->msg = idr;

        amtsvr_sendname(ctx, CENTER_NAME, PTYPE_CLIENT|PTYPE_TAG_DONTCOPY, 0, (void*)mh, sizeof(msg_handler));
    }

    return 0;
}

void role_release(struct role_service_t* a)
{
    amtsvr_log("\n");
    delete a;
}
