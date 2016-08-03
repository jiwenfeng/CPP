#ifndef __PB_AGENT_H__
#define __PB_AGENT_H__

#include "server.node.pb.h"

void* on_agent_register_node_req(server::register_node_req* req, void* ud, size_t* rsp_len);

#endif // __PB_AGENT_H__