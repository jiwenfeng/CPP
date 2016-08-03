#ifndef __CHORES_INTERFACE_H__
#define __CHORES_INTERFACE_H__

#include "pb_dispatcher.h"
#include "pb_mgr.h"
#include "base_time.h"
#include "msg_handler_pool.h"
#include "farm_mgr.h"

#define CHORES_TIMER_INTERVAL  "100"  // 1S一次定时更新

struct chores_service_t : public timer_tags
{
    struct amtsvr_context* ctx;
    uint32 self;
    pb_dispatcher_s pd;
    uint32 tenms;
    std::map<uint32, uint32> gss;   // scene_id --->game service_id 
    
    farm_mgr fa_mgr;

    chores_service_t()
    {
        ctx = NULL;
        self = 0;
        tenms = 0;
    }
};

int chores_dispatcher_init(struct chores_service_t* c);

int chores_send_center(chores_service_t* c, const google::protobuf::Message& msg);

int chores_send_game(chores_service_t* c, uint32 scene_id, const google::protobuf::Message& msg);

int chores_send_game(chores_service_t* c, const client::server_head& svr_head, const google::protobuf::Message& msg);

int chores_send_client(chores_service_t* c, const client::server_head& svr_head, const google::protobuf::Message& msg);

#endif
