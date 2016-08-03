#include "server.node.pb.h"
#include "agent_interface.h"

void* on_agent_register_node_req(server::register_node_req* req, void* ud, size_t* rsp_len)
{
    agent_service_t* a = static_cast<agent_service_t*>(ud);

    char buf[128] = { 0 };
    agent_send_watchdog(a, buf);

    server::register_node_rsp rsp;
    agent_send_client(a, 0, rsp);

    return NULL;
}