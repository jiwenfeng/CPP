#ifndef _WIN32
#include <unistd.h>
#endif // _WIN32
#include "msg_handler_pool.h"
#include "offline_role_mgr.h"
#include "family_mgr.h"
#include "raid_center_mgr.h"
#include "randnpc_mgr.h"
#include "game_map_mgr.h"
#include "role_key.h"
#include "center_interface.h"
#include "link_lua.h"
#include "db_log.h"
#include "center_service.h"

void close_center(struct amtsvr_context* ctx, center_service_t* w)
{
    //GAME_EXIT = true;
    if (GAME_EXIT) {
        center_save_all_data(w);

        uint32 size = WATCHDOG_NAMES.size();
        for (uint32 i = 0; i < size; ++i) {
            amtsvr_sendname(ctx, WATCHDOG_NAMES[i].c_str(), PTYPE_SYSTEM, 0, (void*)CLOSE_ALL, sizeof(CLOSE_ALL));
        }

        size = NODE_NAMES.size();
        for (uint32 i = 0; i < size; ++i) {
            amtsvr_sendname(ctx, NODE_NAMES[i].c_str(), PTYPE_SYSTEM, 0, (void*)CLOSE_ALL, sizeof(CLOSE_ALL));
        }

        amtsvr_sendname(ctx, ROLE_NAME, PTYPE_SYSTEM, 0, (void*)CLOSE_ALL, sizeof(CLOSE_ALL));
        amtsvr_sendname(ctx, LOADER_NAME, PTYPE_SYSTEM, 0, (void*)CLOSE_ALL, sizeof(CLOSE_ALL));
        amtsvr_sendname(w->ctx, WRITER_NAME, PTYPE_SYSTEM, 0, (void*)"write_all", sizeof("write_all"));

        amtsvr_command(ctx, "EXIT", NULL);
        amtsvr_syslog(ctx, "center_service|will exit now|");
    }
}

static int center_cb_timer(struct amtsvr_context* ctx, center_service_t* w)
{
    if (GAME_EXIT) {
        return 0;
    }

#ifdef __INNER_DEBUG__PACKAGE_
    /*static bool fresh_randnpc = false;
    if (!fresh_randnpc){
        client::randnpc_set& rns = w->rn_mgr.get_rns();
        uint32 size = rns.rn_size();
        for (uint32 i = 0; i < size; ++i){
            client::randnpc rn(rns.rn(i));
            center_send_scene_by_sceneid(w, rn.scene_id(), rn);
        }
        fresh_randnpc = true;
    }*/
#endif

    w->tenms = time_utils::tenms();
    if (w->tenms - w->last_save_data >= WORLD_SAVE_DATA_INTERVAL) {
        center_save_data(w);
        w->last_save_data = w->tenms;
    }

    if (w->tenms - w->one_second_timer >= WORLD_ONE_SECOND_INTERVAL) {
        w->one_second_timer = w->tenms;
        center_one_second_timer(w);
    }

    if (w->tenms - w->ten_second_timer >= WORLD_TEN_SECOND_INTERVAL) {
        w->ten_second_timer = w->tenms;
        center_ten_second_timer(w);
    }

    if (w->tenms - w->one_minute_timer >= WORLD_ONE_MINUTE_INTERVAL) {
        w->one_minute_timer = w->tenms;
        center_one_minute_timer(w);
    }

//     if (w->tenms - w->five_minute_timer >= WORLD_ONE_FIVE_INTERVAL) {
//         w->five_minute_timer = w->tenms;
//         center_five_minute_timer(w);
//     }

	if (w->tenms - w->one_minute_timer >= WORLD_ONE_HOUR_INTERVAL) {
		w->one_minute_timer = w->tenms;
		center_one_hour_timer(w);
	}

#ifdef TEST_MEMCHECK
    test_memcheck_player_role(ctx, w);
#endif

    amtsvr_command(w->ctx, "TIMEOUT", WORLD_TIMER_INTERVAL);

    return 0;
}

static int center_cb(struct amtsvr_context* ctx, void* ud, int type, int session, uint32_t source, const void* msg, size_t sz)
{
    center_service_t* w = (center_service_t*)ud;
    if (type == PTYPE_TIMER) {
        return center_cb_timer(ctx, w);
    } else if (type == PTYPE_SYSTEM && GAME_EXIT) {
        uint32 scene_id = atoi((char*)msg);
        w->wm_mgr.delete_scene_service(scene_id);
        if (w->wm_mgr.get_scene_service().empty()) {
            close_center(ctx, w);
        }

        return 0;
    }

    msg_handler* mh = static_cast<msg_handler*>((void*)msg);
    google::protobuf::Message* pb = mh->msg;
    mh->msg = NULL;
    MSG_HANDLER_POOL->push(mh);
    if (pb == NULL) {
        return 1;
    }

    size_t rsp_len = 0;
    void* rsp = w->pd.on_pb_msg(pb, w, &rsp_len);
    delete pb;
    if (rsp != NULL) {
        amtsvr_send(ctx, 0, source, PTYPE_CLIENT|PTYPE_TAG_DONTCOPY, session, rsp, rsp_len);
    }

    return 1; // 回归对象池
}

#define SAFE_NEW_PTR(P, T) if (P == NULL) { P = new T; }

struct center_service_t* center_create(void)
{
    struct center_service_t* w = new center_service_t;
    SAFE_NEW_PTR(w->ou_mgr   , offline_role_mgr);
    SAFE_NEW_PTR(w->cf_mgr   , family_mgr      );
    SAFE_NEW_PTR(w->rcm_mgr  , raid_center_mgr );
    SAFE_NEW_PTR(w->rn_mgr   , randnpc_mgr     );
    SAFE_NEW_PTR(w->ct_mgr   , siege_mgr      );
    SAFE_NEW_PTR(w->cwm_mgr  , city_welfare_mgr);
    SAFE_NEW_PTR(w->_role_key, role_key_mgr     );
	SAFE_NEW_PTR(w->poa_mgr, poll_activity_mgr  );

    return w;
}

static int center_init_map_mgr(struct center_service_t* w, struct amtsvr_context* ctx, char* args)
{
    int wc = 0;
    while (!GAME_MAP_MGR->ready()) {
        if (wc++ >= 30){ // 15秒钟地图数据仍未初始化完成，DB出了问题，停止启动世界数据
            return 1;
        }
        usleep(500000);
    }

    int ret = w->wm_mgr.init(ctx, args);
    if (ret != 0){
        return 1;
    }

    server::scene_service_msg ssm;
    const map<uint32, uint32>& tmp_ids = w->wm_mgr.get_scene_service();
    map<uint32, uint32>::const_iterator it = tmp_ids.begin();
    for (; it != tmp_ids.end(); ++it){
        server::scene_service_info* ssi = ssm.add_ssi();
        ssi->set_scene_id(it->first);
        ssi->set_service_id(it->second);
    }
    //ceneter_send_chores(w, ssm);

    return 0;
}

int center_init(struct center_service_t* w, struct amtsvr_context* ctx, char* args)
{
    mtrandom::_seed((uint32)time(NULL));
    w->ctx = ctx;

    int ret = center_init_map_mgr(w, ctx, args);
    if (ret != 0){
        return 1;
    }

    ret = center_dispatcher_init(w);
    if (ret != 0){
        return 1;
    }
    if (w->aus_mgr.init() != 0){
        return 1;
    }

    w->mm_mgr.attach_ctx(ctx);
    w->ar_mgr.attach_ctx(ctx);

    amtsvr_callback(ctx, (void*)w, center_cb);
    amtsvr_command(ctx, "REG", CENTER_NAME);

    amtsvr_command(w->ctx, "TIMEOUT", WORLD_TIMER_INTERVAL);

    link_lua lua;
    lua.init_reg_pb();
    lua.do_reg();

    if (w->_log == NULL) {
        w->_log = new db_log(ctx);
    }

    return 0;
}

#define SAFE_DELETE_PTR(P) if (P != NULL) { delete P; P = NULL; }

void center_release(struct center_service_t* w)
{
    amtsvr_log("\n");

	SAFE_DELETE_PTR(w->poa_mgr   );
    SAFE_DELETE_PTR(w->_role_key );
    SAFE_DELETE_PTR(w->ou_mgr    );
    SAFE_DELETE_PTR(w->cf_mgr    );
    SAFE_DELETE_PTR(w->rcm_mgr   );
    SAFE_DELETE_PTR(w->rn_mgr    );
    SAFE_DELETE_PTR(w->ct_mgr    );
    SAFE_DELETE_PTR(w->cwm_mgr   );
    SAFE_DELETE_PTR(w->_log      );
    SAFE_DELETE_PTR(w            );
}
