#include "game_service.h"
#include "game_interface.h"
#include "msg_handler_pool.h"
#include "game_map_mgr.h"

#ifdef __TEST_LUA__
#include "link_lua.h"
#include "client.script.pb.h"
#endif // __TEST_LUA__

/*
static int remove_lose_scene_user(game_service_t* s, vector<scene_user*>& remove_users)
{
    uint32 size = remove_users.size();
    if (size == 0){
        return 0;
    }

    client::offline o;
    client::server_head* head = o.mutable_svr_head();
    
    for (uint32 i = 0; i < size; ++i){
        scene_user* su = remove_users[i];
        if (su == NULL){
            continue;
        }
        su->detach_scene();

        o.set_user_id(0);
        o.set_role_id(su->get_role_id());
        o.set_scene_server_id(0);
        head->set_role_id(su->get_role_id());
        head->set_agent_id(su->get_agent_id());
        head->set_client_id(su->get_client_id());

        game_user_offline(s, su, NULL, &o);
    }

    return 0;
}*/

static int stop_game_service(struct amtsvr_context* ctx, game_service_t* s)
{
    s->sim->save();
	s->sim->release();

    amtsvr_command(ctx, "EXIT", NULL);
    amtsvr_syslog(ctx, "game_service|will exit now|%d|", s->sim->get_game_map()->get_id());

    // 通知center 关闭scene服务
    char buf[32] = {0};
    sprintf(buf, "%u", s->sim->get_game_map()->get_id());
    amtsvr_sendname(ctx, CENTER_NAME, PTYPE_SYSTEM, 0, buf, strlen(buf));

    return 0;
}

static int game_timer_cb(struct amtsvr_context* ctx, game_service_t* s, int type, int session, uint32_t source, const void* msg, size_t sz)
{
    s->tenms = time_utils::tenms();

    bool is_one_sec    = s->judge_one_seconds   (s->tenms);
    bool is_fiv_sec    = s->judge_five_seconds  (s->tenms);
    bool is_ten_sec    = s->judge_ten_seconds   (s->tenms);
    bool is_thirty_sec = s->judge_thirty_seconds(s->tenms);
    bool is_one_minute = s->judge_one_minute    (s->tenms);

    s->sim->update(s->tenms, is_one_sec, is_fiv_sec, is_ten_sec, is_thirty_sec, is_one_minute);

    amtsvr_command(s->ctx, "TIMEOUT", SCENE_TIMER_INTERVAL);

    return 0;
}

static int game_cb(struct amtsvr_context* ctx, void* ud, int type, int session, uint32_t source, const void* msg, size_t sz)
{
    game_service_t* s = (game_service_t*)ud;

    if (type == PTYPE_TIMER) {

        if (GAME_EXIT) {
            return stop_game_service(ctx, s);
        }

        return game_timer_cb(ctx, s, type, session, source, msg, sz);
    }

    msg_handler* mh = static_cast<msg_handler*>((void*)msg);
    google::protobuf::Message* pb = mh->msg;
    mh->msg = NULL;
    MSG_HANDLER_POOL->push(mh);
    if (pb == NULL) {
        return 1;
    }

#ifdef __TEST_LUA__
    if (type == PTYPE_SCRIPT) {
        //client::script_pb_msg* spm = static_cast<client::script_pb_msg*>(pb);
        //s->lua->do_pb(s, spm->type_name(), spm->bin(), spm->mutable_svr_head());
        delete pb;
        return 1;
    }
#endif // __TEST_LUA__

    size_t rsp_len = 0;
    void* rsp = s->pd.on_pb_msg(pb, s, &rsp_len);
    delete pb;
    if (rsp != NULL) {
        amtsvr_send(s->ctx, 0, source, PTYPE_CLIENT|PTYPE_TAG_DONTCOPY, session, rsp, rsp_len);
    }

    return 1;
}

struct game_service_t* game_create(void)
{
    game_service_t* sst = new game_service_t;
    sst->sim = NULL;

    return sst;
}

#ifdef __TEST_LUA__
int game_service_t::_lock = 0;
#endif // __TEST_LUA__

static int scene_inst_mgr_init(struct game_service_t* s, struct amtsvr_context* ctx, uint32 map_id, uint32 scene_id)
{
    const game_map* pmap = GAME_MAP_MGR->get_map(map_id);
    if (pmap == NULL) {
        return -1;
    }

#ifdef __TEST_LUA__
//     int ret = __sync_lock_test_and_set(&game_service_t::_lock, 1);
    if (s->lua == NULL/* && ret == 0*/) {
        //s->lua = new link_lua;
        //s->lua->init_game_pb();
    }
#endif // __TEST_LUA__

    const std::map<uint32, scene*>& scs = pmap->get_scene_map();
    for (std::map<uint32, scene*>::const_iterator it = scs.begin(); it != scs.end(); ++it) {
        if (it->first != scene_id) {
            continue;
        }

        scene_inst_mgr* sim = new scene_inst_mgr(pmap, it->first, ctx);
        if (sim->init() != 0) {
            delete sim;
            continue;
        }

        s->sim = sim;
        return 0;
    }

    return -2;
}

int game_init(struct game_service_t* s, struct amtsvr_context* ctx, char* args)
{
    uint32 max_session = 0;
    uint32 max_session_user = 0;
    uint32 map_id = 0;
    uint32 scene_id = 0;
    sscanf(args, "%u %u %u %u", &max_session, &max_session_user, &map_id, &scene_id);
    if (max_session == 0 || max_session_user == 0 || map_id == 0) {
        return 1;
    }

    s->ctx = ctx;

    if (scene_inst_mgr_init(s, ctx, map_id, scene_id) != 0) {
        return 1;
    }

    game_dispatcher_init(s);

    s->self = strtoul(amtsvr_command(ctx, "REG", NULL) + 1 , NULL, 16);

    amtsvr_callback(ctx, (void*)s, game_cb);
    amtsvr_command(s->ctx, "TIMEOUT", SCENE_TIMER_INTERVAL);

    return 0;
}

void game_release(struct game_service_t* s)
{
    amtsvr_log("%u\n", s->sim->get_game_map()->get_id());

#ifdef __TEST_LUA__
//     int ret = __sync_lock_test_and_set(&game_service_t::_lock, 2);
    if (s->lua != NULL/* && ret == 1*/) {
//         amtsvr_log("lock:%d, s:%p lua:%p\n", game_service_t::_lock, s, game_service_t::lua);
        delete s->lua;
        s->lua = NULL;
    }
#endif // __TEST_LUA__

    if (s->sim != NULL) {
        delete s->sim;
    }

    delete s;
}
