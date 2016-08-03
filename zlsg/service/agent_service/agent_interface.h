#ifndef __AGENT_INTERFACE_H__
#define __AGENT_INTERFACE_H__

#include "define.h"
#include "pb_dispatcher.h"

#define AGENT_CHECK_INTERVAL "1000" // 秒，socket连接一定要通用验证，目测正常登录情况下，10秒钟时间已经足够,也用来检测保活
#define AGENT_HEART_INTERVAL 180     // 180秒未收到客户端的数据包，即认为客户端连接已经断开，主动关闭
#define MAX_ROLE_PER_USER    5      // 每个用户最多有5个角色

namespace client {
    class role_info_rsp;
    class server_head;
};

namespace inner {
    class inner_head;
};

struct agent_service_t
{
    agent_service_t() {
        ctx  = NULL;
        exit = false;

        index    = 0;
        shutdown = false;

        self          = 0;
        gate          = 0;
        watch_dog     = 0;
        client        = 0;
        connect_time  = 0;
        last_rcv_time = 0;
        recv_count    = 0;
        send_count    = 0;
        //ip[MAX_IP_LEN - 1] = '\0';
        memset(ip, 0, sizeof(ip));
        port  = 0;
        inner = 0;

        //user_name[MAX_USER_NAME_LEN - 1] = '\0';
        memset(user_name, 0, sizeof(user_name));
        user_id = 0;

        memset(rir, 0, sizeof(rir));

        svr_id            = 0;
        role_id           = 0;
        login_auth_pass   = false;
        scene_server_id   = 0;
        package_index     = 0;

        is_node           = false;
        scene_server_type = 0;
    }

    amtsvr_context* ctx;
    bool exit;

    int    index;
    bool   shutdown;
    uint32 self;
    uint32 gate;
    uint32 watch_dog;
    uint32 client;
    uint32 connect_time;
    uint32 last_rcv_time;
    uint32 recv_count;
    uint32 send_count;
    char   ip[MAX_IP_LEN];
    uint32 port;
    int    inner;         // 是否为内部的接口,为GMS等开辟的地址,包括充值、踢人、发邮件等接口

    char   user_name[MAX_USER_NAME_LEN];
    uint32 user_id;

    client::role_info_rsp* rir[MAX_ROLE_PER_USER];

    uint32 svr_id;
    uint32 role_id;
    bool   login_auth_pass; // 登录校验是否通过 
    uint32 scene_server_id; // 目前用户所在的场景服务ID
    uint32 package_index;   // 客户端发送协议包序列

    bool   is_node;           // 是否节点代理
    uint32 scene_server_type; // 场景服务类型 0 本地 1 跨服

    pb_dispatcher_s pd;
    pb_dispatcher_s inner_pd; // 内部处理接口,为GMS等开辟的地址,包括充值、踢人、发邮件等接口

    std::map<uint32, uint32> link_agent;
};

int agesvr_dispatcher_init(struct agent_service_t* a);

void set_svr_head(client::server_head* svr_head, struct agent_service_t* a);

void set_inner_head(inner::inner_head* in_head, struct agent_service_t* a);

void kick_agent(struct agent_service_t* a);

void lose_agent(struct agent_service_t* a);

void agent_send_client(struct agent_service_t* a, int session, const google::protobuf::Message& msg);
void agent_send_watchdog(struct agent_service_t* a, const char* cmd);
void agent_send_log(struct agent_service_t* a, google::protobuf::Message* msg);

#endif
