#ifndef __AGENT_INTERFACE_H__
#define __AGENT_INTERFACE_H__

#include "define.h"
#include "pb_dispatcher.h"

#define AGENT_CHECK_INTERVAL "1000" // �룬socket����һ��Ҫͨ����֤��Ŀ��������¼����£�10����ʱ���Ѿ��㹻,Ҳ������Ᵽ��
#define AGENT_HEART_INTERVAL 180     // 180��δ�յ��ͻ��˵����ݰ�������Ϊ�ͻ��������Ѿ��Ͽ��������ر�
#define MAX_ROLE_PER_USER    5      // ÿ���û������5����ɫ

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
    int    inner;         // �Ƿ�Ϊ�ڲ��Ľӿ�,ΪGMS�ȿ��ٵĵ�ַ,������ֵ�����ˡ����ʼ��Ƚӿ�

    char   user_name[MAX_USER_NAME_LEN];
    uint32 user_id;

    client::role_info_rsp* rir[MAX_ROLE_PER_USER];

    uint32 svr_id;
    uint32 role_id;
    bool   login_auth_pass; // ��¼У���Ƿ�ͨ�� 
    uint32 scene_server_id; // Ŀǰ�û����ڵĳ�������ID
    uint32 package_index;   // �ͻ��˷���Э�������

    bool   is_node;           // �Ƿ�ڵ����
    uint32 scene_server_type; // ������������ 0 ���� 1 ���

    pb_dispatcher_s pd;
    pb_dispatcher_s inner_pd; // �ڲ�����ӿ�,ΪGMS�ȿ��ٵĵ�ַ,������ֵ�����ˡ����ʼ��Ƚӿ�

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
