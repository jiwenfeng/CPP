#ifndef __NODE_INTERFACE_H__
#define __NODE_INTERFACE_H__

#include <map>

#include "define.h"
#include "pb_dispatcher.h"
#include "cmq_svr_msg.h"
#include "amtsvr.h"
#ifndef _WIN32
#include <unistd.h>
#endif
#define NODE_BUFFER_LENGTH 8096

struct node_role {
    uint32 role_id;
    uint32 agent_id;
    uint32 client_id;
};

struct node_service_t
{
    int node;

    char           ip[MAX_IP_LEN];
    unsigned short port;
    int            server_id;
    int            socket;

    char           buffer[NODE_BUFFER_LENGTH];
    size_t         read_length;

    std::map<uint32, node_role> roles;

    struct amtsvr_context* ctx;
    pb_dispatcher_s pd;
};

int node_dispatcher_init(struct node_service_t* w);

int node_send_server(struct node_service_t* w, const char* buf, int len);

#endif
