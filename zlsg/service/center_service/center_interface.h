#ifndef __CENTER_INTERFACE_H__
#define __CENTER_INTERFACE_H__

#include "pb_mgr.h"
#include "pb_dispatcher.h"
#include "center_map_mgr.h"
#include "online_role_mgr.h"
#include "mail_center_mgr.h"
#include "role_smy_mgr.h"
#include "friends_mgr.h"
#include "boss_mgr.h"
#include "rank_mgr.h"
#include "team_mgr.h"
#include "explore_mgr.h"
#include "sys_msg.h"
#include "ban_user.h"
#include "msg_handler_pool.h"
#include "arena_mgr.h"
#include "mate_mgr.h"
#include "city_welfare_mgr.h"
#include "siege_mgr.h"
#include "family_mgr.h"
#include "arena_robot_mgr.h"
#include "poll_activity_mgr.h"
#include "cycle_mgr.h"
#include "devil_mgr.h"
#include "notice_mgr.h"
#include "illegal_word.h"

#define WORLD_TIMER_INTERVAL      "100"    // 1秒1次定时器回调
#define WORLD_SAVE_DATA_INTERVAL   500     // 5秒一次定时回存world的数据
#define WORLD_ONE_MINUTE_INTERVAL  6000    // 1分钟定时器
#define WORLD_FIVE_MINUTE_INTERVAL 30000    // 1分钟定时器
#define WORLD_ONE_SECOND_INTERVAL  100     // 1秒钟定时器
#define WORLD_TEN_SECOND_INTERVAL  1000    // 10秒钟定时器
#define WORLD_ONE_HOUR_INTERVAL    36000    // 1小时钟定时器
#define WORLD_SAVE_DATA_NUM        50      // 定时器一次回写数据数目

class player_role;
class offline_role_mgr;
class family_mgr;
class raid_center_mgr;
class randnpc_mgr;
class role_key_mgr;
class poll_activity_mgr;
class db_log;

struct center_service_t
{
    struct amtsvr_context*		ctx;
    pb_dispatcher_s				pd;
    online_role_mgr				oi_mgr;
    offline_role_mgr*			ou_mgr;
    role_smy_mgr				aus_mgr;
    center_map_mgr				wm_mgr;
    mail_center_mgr				wg_mcm;
    friends_mgr					fm_mgr;
    boss_mgr					bm_mgr;
    rank_mgr					rm_mgr;
    team_mgr					tm_mgr;
    family_mgr*					cf_mgr;
    explore_mgr					em_mgr;
    sys_msg						cs_mgr;
    ban_user					bu_mgr;
    arena_mgr					ar_mgr;
    raid_center_mgr*			rcm_mgr;
    mate_mgr					mm_mgr;
    randnpc_mgr*				rn_mgr;
	siege_mgr *					ct_mgr;
    city_welfare_mgr*			cwm_mgr;
    robot_mgr					rbt_mgr;
	cycle_mgr					cc_mgr;
    role_key_mgr*				_role_key;
	poll_activity_mgr*			poa_mgr;
	devil_mgr					dm_mgr;
    db_log*						_log;
	notice_mgr					notic_mgr;
	illegal_word_mgr			iw_mgr;
    // 数据缓写模块
    uint32 last_save_data;

    // 定时器
    uint32 five_minute_timer;
    uint32 one_minute_timer;
    uint32 ten_second_timer;
    uint32 one_second_timer;
    uint32 tenms;
    uint32 label_time;
    uint32 day_update_time;

    bool _gm;

    center_service_t() {
        ctx = NULL;

        ou_mgr = NULL;
        cf_mgr = NULL;

        rcm_mgr = NULL;
        rn_mgr  = NULL;

        ct_mgr    = NULL;
        cwm_mgr   = NULL;
        _role_key = NULL;
		poa_mgr   = NULL;
        _log      = NULL;
        last_save_data    = 0;
        five_minute_timer = 0;
        one_minute_timer  = 0;
        ten_second_timer  = 0;
        one_second_timer  = 0;
        tenms             = 0;
        label_time        = 0;
        day_update_time   = 0;

        _gm = false;
    }
};

int center_dispatcher_init(struct center_service_t* w);

int center_save_all_data(struct center_service_t* w);

int center_save_data(struct center_service_t* w);

int center_one_second_timer(struct center_service_t* w);

int center_ten_second_timer(struct center_service_t* w);

int center_one_minute_timer(struct center_service_t* w);

int center_one_hour_timer(struct center_service_t* w);

int center_load_role_req(struct center_service_t* w, uint32 role_id, google::protobuf::Message& msg);

int center_send_inner(struct center_service_t* w, const inner::inner_head& head, google::protobuf::Message& msg);

int center_send_scene(struct center_service_t* w, const client::server_head& head, google::protobuf::Message& msg);

int center_send_scene(struct center_service_t* w, uint32 role_id, google::protobuf::Message& msg);

int center_send_scene(struct center_service_t* w, const online_role& oi, google::protobuf::Message& msg);

int center_send_scene_ex(struct center_service_t* w,  uint32 scene_svr_id, google::protobuf::Message& msg);

int center_send_scene_by_scene_id(struct center_service_t* w, uint32 scene_id, google::protobuf::Message& msg);

int center_send_scene_by_service_id(struct center_service_t* w, uint32 service_id, google::protobuf::Message& msg);

int center_send_client(center_service_t* w, const online_role& oi, google::protobuf::Message& msg);

int center_send_client(center_service_t* w, const client::server_head& head, google::protobuf::Message& msg);

int center_send_client(center_service_t* w, uint32 client_id, google::protobuf::Message& msg);

int center_send_client_by_role_id(center_service_t* w, uint32 role_id, google::protobuf::Message& msg);

int center_send_agent(center_service_t* w, uint32 agent_id, google::protobuf::Message& msg);

int center_close_agent(center_service_t* w, const online_role& oi);

int center_send_role(center_service_t* w, google::protobuf::Message& msg);

player_role* center_get_player_role(center_service_t* w, google::protobuf::Message& msg, uint32 role_id);

void center_add_friends_degree(center_service_t* w, uint32 src_role_id, uint32 dst_role_id, uint32 degree_val);

void center_on_user_offline(center_service_t* w, uint32 role_id);

int ceneter_send_chores(center_service_t* w, const google::protobuf::Message& msg);

void send_new_mail(center_service_t *w,uint32 role_id,const client::mail_info& mi);

uint32 get_mail_past_time(uint32 mail_type,bool has_adjunct = false);

void send_broadcast_area_msg(center_service_t *w,client::broadcast_area_rsp& area_msg);
void send_broadcast_area_msg(center_service_t *w, uint32 msg_id, const vector<client::content_data> &data);

void load_user_data(center_service_t *w, client::load_target_data_req req);

void init_scene_sculpture(center_service_t *w);
// 活动的开关
void poll_activity_on_process(center_service_t *w);
void send_rank_reward(center_service_t *w);
void on_area_brocast(center_service_t *w);

void update_team_info(center_service_t *w, uint32 team_id);

#endif
