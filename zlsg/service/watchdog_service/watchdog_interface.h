#ifndef __WATCHDOG_INTERFACE_H__
#define __WATCHDOG_INTERFACE_H__

#include <map>

#include "define.h"
#include "pb_dispatcher.h"
#include "cmq_svr_msg.h"
#include "amtsvr.h"

using namespace std;

struct agent_unit_t{
    int    fd;
    uint32 agent_id;
    uint32 client_id;

    agent_unit_t()
    {
        fd = 0;
        agent_id = 0;
        client_id = 0;
    }

    agent_unit_t& operator=(const agent_unit_t& rhs)
    {
        fd = rhs.fd;
        agent_id = rhs.agent_id;
        client_id = rhs.client_id;
        return *this;
    }
}; 

struct watchdog_service_t
{
    map<int, agent_unit_t> au_map;  // gate中的id--->client
    int port;
    int max_agent;
    int buffer;
    uint32 gate;
    uint32 self;
    int inner;     // 是否内部的PHP接口，或者内部的充值、GMS相应的接口

    std::map<int, int> node_agent;

    struct amtsvr_context* ctx;
    pb_dispatcher_s pd;
};

int wadog_dispatcher_init(struct watchdog_service_t* w);

#endif
