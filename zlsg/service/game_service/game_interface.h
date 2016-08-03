#ifndef __GAME_INTERFACE_H__
#define __GAME_INTERFACE_H__

#include "pb_dispatcher.h"
#include "scene_inst_mgr.h"
#include "pb_mgr.h"
#include "scene_user.h"
#include "scene_inst.h"

#define SCENE_TIMER_INTERVAL  "10"  // 100MS一次定时更新
#define SCENE_SAVE_DATA_NUM   10    // 定时器一次回写数据数目

namespace client {
    class offline;
}

class link_lua;

struct game_service_t : public timer_tags
{
    struct amtsvr_context* ctx;
    pb_dispatcher_s pd;
    uint32 self;

    uint32 tenms; // 以10ms为单位的取时，程序运行时为0

    scene_inst_mgr* sim;
    bool close;

#ifdef __TEST_LUA__
    link_lua* lua;
    static int _lock;
#endif // __TEST_LUA__

    game_service_t() {
        ctx   = NULL;
        self  = 0;
        tenms = 0;
        sim   = NULL;
        close = false;
#ifdef __TEST_LUA__
        lua = NULL;
#endif // __TEST_LUA__
    }
};

int game_dispatcher_init(struct game_service_t* s);

int game_send_client(struct game_service_t* w, const client::server_head& head, google::protobuf::Message& msg);
int game_send_client(struct game_service_t* s, uint32 client_id, google::protobuf::Message& msg);
int game_send_client(struct game_service_t* pgs, uint32 client_id, const char* name, const char* bin, uint32 len);

int game_send_scene(struct game_service_t *s, uint32 svr_scene_id, google::protobuf::Message &msg);

int game_send_center(struct game_service_t* s, google::protobuf::Message& msg);

int game_send_chores(struct game_service_t* s, google::protobuf::Message& msg);


int game_broadcast_by_center(struct game_service_t* s,client::broadcast_area_to_center& rsp_msg);

int send_scene_area_msg(struct game_service_t* s,uint32 svr_scene_id,client::broadcast_area_rsp& area_msg);

#ifndef __LIB_MINI_SERVER__
int game_send_node(struct game_service_t* s, uint32 role_id, google::protobuf::Message& msg);
int game_send_inner(struct game_service_t* s, const inner::inner_head& head, google::protobuf::Message& msg);

int fill_target_list(client::target_list_rsp &rsp, scene_user *psu, client::TARGET_TYPE_E type);
#endif // __LIB_MINI_SERVER__

void game_user_offline(struct game_service_t* s, scene_user* su);
void game_user_offline(struct game_service_t* s, scene_user* su, scene_inst* si, client::offline* p);

int game_close_agent(struct game_service_t* s, uint32 agent_id);

void game_exit_raid_cmd(struct game_service_t* ss, uint32 role_id);


int check_enter_raid(uint32 raid_id, uint32 raid_type, scene_user* psu, bool enter = false);

#define PB_FUNC_GUARD(MSG) struct game_service_t* ss = NULL;            \
    ss = (struct game_service_t*)ud;                                    \
    scene_user* psu = ss->sim->get_user(MSG->svr_head().role_id());     \
    if (psu == NULL) {                                                  \
        return NULL;                                                    \
    }

#define PB_FUNC_GUARD_VIA(MSG) struct game_service_t* ss = NULL;        \
    ss = (struct game_service_t*)ud;                                    \
    scene_user* psu = ss->sim->get_user(MSG->role_id());                \
    if (psu == NULL) {                                                  \
        return NULL;                                                    \
    }

#define PB_FUNC_GUARD_DEST(ROLE_ID) struct game_service_t* ss = NULL;   \
    ss = (struct game_service_t*)ud;                                    \
    scene_user* psu = ss->sim->get_user(ROLE_ID);                       \
    if (psu == NULL) {                                                  \
        return NULL;                                                    \
    }

#define PB_FUNC_GUARD_FIND(ROLE_ID) struct game_service_t* ss = NULL;   \
    ss = (struct game_service_t*)ud;                                    \
    scene_user* psu = ss->sim->find_user(ROLE_ID);                      \
    if (psu == NULL) {                                                  \
        return NULL;                                                    \
    }

#endif
