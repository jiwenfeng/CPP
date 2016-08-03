#include "server.node.pb.h"
#include "client.offline.pb.h"
#include "watchdog_service.h"
#include "watchdog_interface.h"
#include <string.h>
#include "pb_mgr.h"
#include "pb_coder.h"

static int watchdog_open_agent_node(watchdog_service_t* w, const char* param, size_t sz)
{
    int id;
    int node_id = 0;
    sscanf(param, "open_node %d %d", &id, &node_id);

    char tmp[128] = {0};
    snprintf(tmp, sizeof(tmp), "client %d %u %d %d", 0, w->gate, id, node_id);
    const char *client_addr = amtsvr_command(w->ctx, "LAUNCH", tmp);
    if (client_addr == NULL) {
        return -1;
    }

    unsigned int client_id = strtoul(client_addr + 1 , NULL, 16);
    if (client_id  == 0) {
        return -2;
    }

    snprintf(tmp, sizeof(tmp), "agent %u %u %u %s %d %d %d", w->gate, w->self, client_id, "1", id, w->inner, node_id);
    const char* agent_addr = amtsvr_command(w->ctx, "LAUNCH", tmp);
    if (agent_addr == NULL) {
        amtsvr_command(w->ctx, "KILL", client_addr);
        return -3;
    }

    unsigned long agent_id = strtoul(agent_addr + 1 , NULL, 16);
    if (agent_id == 0) {
        amtsvr_command(w->ctx, "KILL", client_addr);
        return -4;
    }

    w->node_agent[id] = agent_id;

    server::node_agent_rsp rsp;
    rsp.set_role_id(id);
    rsp.set_agent_id(agent_id);
    int req_len = 0;
    void* req_msg = svr_encode(rsp, req_len);
    amtsvr_send(w->ctx, 0, node_id, PTYPE_CLIENT | PTYPE_TAG_DONTCOPY, 0, (void*)req_msg, (size_t)req_len);

    return 0;
}

//"1 open 19 127.0.0.1:53202"
static int watchdog_open_agent(watchdog_service_t* w, const char* param, size_t sz)
{
    agent_unit_t agent;
    int id;
    char addr[128] = {0};
    sscanf(param, "%d open %d %s", &id, &agent.fd, addr);

    char tmp[128] = {0};
    snprintf(tmp, sizeof(tmp), "client %d %u %d", agent.fd, w->gate, id);
    const char *client_addr = amtsvr_command(w->ctx, "LAUNCH", tmp);
    if (client_addr == NULL) {
        return -1;
    }

    agent.client_id = strtoul(client_addr + 1 , NULL, 16);
    if (agent.client_id  == 0) {
        return -2;
    }

    snprintf(tmp, sizeof(tmp), "agent %u %u %u %s %d %d", w->gate, w->self, agent.client_id, addr, id, w->inner);
    const char* agent_addr = amtsvr_command(w->ctx, "LAUNCH", tmp);
    if (agent_addr == NULL) {
        amtsvr_command(w->ctx, "KILL", client_addr);
        return -3;
    }

    agent.agent_id = strtoul(agent_addr + 1 , NULL, 16);
    if (agent.agent_id == 0) {
        amtsvr_command(w->ctx, "KILL", client_addr);
        return -4;
    }

    w->au_map[id] = agent;
    snprintf(tmp, sizeof(tmp), "forward %d %s %s", id, agent_addr, client_addr);
    amtsvr_send(w->ctx, 0, w->gate, PTYPE_TEXT, 0, (void*)tmp, strlen(tmp)+1);

    return 0;
}

static void send_offline_msg(watchdog_service_t* w, uint32 agent_id)
{
    client::offline o;
    o.set_user_id(0);
    o.set_role_id(0);
    o.set_scene_server_id(0);
    o.set_active(true);
    int req_len = 0;
    void* req_msg = svr_encode(o, req_len);

    amtsvr_send(w->ctx, 0, agent_id, PTYPE_CLIENT | PTYPE_TAG_DONTCOPY, 0, (void*)req_msg, (size_t)req_len);
}

// "1 close"
static int watchdog_close_agent(watchdog_service_t* w, const char* param, size_t sz)
{
    int id;
    sscanf(param, "%d close", &id);
    map<int, agent_unit_t>::iterator it = w->au_map.find(id);
    if (it == w->au_map.end()) {
        return -1;
    }

    send_offline_msg(w, it->second.agent_id); // 给agent发送退出消息

    amtsvr_send(w->ctx, 0, it->second.client_id, PTYPE_CLIENT | PTYPE_TAG_DONTCOPY, 0, NULL, 0);
    char tmp[128] = {0};
    snprintf(tmp, sizeof(tmp), ":%x", it->second.client_id);
    amtsvr_command(w->ctx, "KILL", tmp);

    w->au_map.erase(id);

    return 0;
}

static void watchdog_close_all(watchdog_service_t* w)
{
    char tmp[128] = {0};
    snprintf(tmp, sizeof(tmp), ":%x", w->gate);
    amtsvr_command(w->ctx, "KILL", tmp);

    std::map<int, agent_unit_t>::iterator it = w->au_map.begin();
    for (; it != w->au_map.end(); ++it) {
        send_offline_msg(w, it->second.agent_id); // 给agent发送退出消息

        amtsvr_send(w->ctx, 0, it->second.client_id, PTYPE_CLIENT | PTYPE_TAG_DONTCOPY, 0, NULL, 0);
        char tmp[128] = {0};
        snprintf(tmp, sizeof(tmp), ":%x", it->second.client_id);
        amtsvr_command(w->ctx, "KILL", tmp);
    }

    w->au_map.clear();
}

static int watchdog_trans_data(watchdog_service_t* w, int type, int session, const char* param, size_t sz)
{
    int id;
    sscanf(param, "%d data ", &id);
    map<int, agent_unit_t>::iterator it = w->au_map.find(id);
    if (it == w->au_map.end()) {
        return -1;
    }

    const char* data = strstr(param, "data ") + strlen("data ");
    sz -= (data - param);
    char* buf = (char*)malloc(sz);
    memcpy(buf, data, sz);
    amtsvr_send(w->ctx, it->second.client_id, it->second.agent_id, type|PTYPE_TAG_DONTCOPY, session, (void*)buf, sz);

    return 0;
}

static int watchdog_proc_cmd(watchdog_service_t* w, int type, int session, uint32_t source, const void* msg, size_t sz)
{
    const char* tmp = (const char*)msg;
    if (strstr(tmp, "open") != NULL) {
        watchdog_open_agent(w, (const char*)msg, sz);
        return 0;
    }

    if (strstr(tmp, "close") != NULL) {
        watchdog_close_agent(w, (const char*)msg, sz);
        return 0;
    }

    if (strstr(tmp, "data") != NULL) {
        watchdog_trans_data(w, type, session,(const char*)msg, sz);
        return 0;
    }

    return 0;
}

static int proc_shutdown_agent(struct amtsvr_context* ctx, watchdog_service_t* w, const char* msg, size_t sz)
{
    int id = 0;
    sscanf(msg, "shutdown %d", &id);
    map<int, agent_unit_t>::iterator it = w->au_map.find(id);
    if (it == w->au_map.end()) {
        return -1;
    }

    char tmp[128] = {0};

    snprintf(tmp, sizeof(tmp), "kick %d", id);
    amtsvr_send(ctx, 0, w->gate, PTYPE_TEXT, 0, tmp, strlen(tmp)+1);

    snprintf(tmp, sizeof(tmp), ":%x", it->second.client_id);
    amtsvr_command(w->ctx, "KILL", tmp);

    snprintf(tmp, sizeof(tmp), ":%x", it->second.agent_id);
    amtsvr_command(w->ctx, "KILL", tmp);

    w->au_map.erase(id);

    return 0;
}

static int proc_lose_conn_agent(struct amtsvr_context* ctx, watchdog_service_t* w, const char* msg, size_t sz)
{
    int id = 0;
    sscanf(msg, "lose_conn %d", &id);
    map<int, agent_unit_t>::iterator it = w->au_map.find(id);
    if (it == w->au_map.end()) {
        return -1;
    }

    char tmp[128] = {0};

    send_offline_msg(w, it->second.agent_id); // 给agent发送退出消息

    snprintf(tmp, sizeof(tmp), "kick %d", id);
    amtsvr_send(ctx, 0, w->gate, PTYPE_TEXT, 0, tmp, strlen(tmp)+1);

    w->au_map.erase(id);

    return 0;
}

static int watchdog_proc_msg_cb(struct amtsvr_context* ctx, void* ud, int type, int session, uint32_t source, const void* msg, size_t sz)
{
    watchdog_service_t* w = (watchdog_service_t*)ud;
    if (source == w->gate) {
        watchdog_proc_cmd(w, type, session, source, msg, sz);
        return 0;
    }

    if (memcmp(msg, LOSE_CONN, strlen(LOSE_CONN)) == 0) {
        proc_lose_conn_agent(ctx, w, (const char*)msg, sz);
        return 0;
    }

    if (memcmp(msg, SHUTDOWN_CMD, strlen(SHUTDOWN_CMD)) == 0) {
        proc_shutdown_agent(ctx, w, (const char*)msg, sz);
        return 0;
    }

    if (memcmp(msg, CLOSE_ALL, strlen(CLOSE_ALL)) == 0) {
        watchdog_close_all(w);
        amtsvr_command(ctx, "EXIT", NULL);
        return 0;
    }

    if (memcmp(msg, OPEN_NODE, strlen(OPEN_NODE)) == 0) {
        watchdog_open_agent_node(w, (const char*)msg, sz);
        return 0;
    }

    google::protobuf::Message* pb = svr_decode((const char*)msg, (int)sz);
    if (pb == NULL) {
        return 0;
    }

    size_t rsp_len = 0;
    w->pd.on_pb_msg(pb, w, &rsp_len);
    delete pb;

    return 0;
}

struct watchdog_service_t* watchdog_create(void)
{
    struct watchdog_service_t* w = new watchdog_service_t;

    return w;
}

// 端口 最大用户数 buffer
int watchdog_init(struct watchdog_service_t* w, struct amtsvr_context* ctx, char* args)
{
    char ip[MAX_IP_LEN] = {0};
    sscanf(args, "%s %d %d %d %d", ip, &w->port, &w->max_agent, &w->buffer, &w->inner);
    if (w->port == 0) {
        amtsvr_syslog(ctx, "watchdog_init : args failed(%s)", args);
        return 1;
    }

    char tmp[512];
    w->self = strtoul(amtsvr_command(ctx, "REG", NULL)+1 , NULL, 16);
    sprintf(tmp,"gate L %s %s:%d 0 %d %d", amtsvr_command(ctx, "REG", NULL), ip, w->port, w->max_agent, w->buffer);
    const char * gate_addr = amtsvr_command(ctx, "LAUNCH", tmp);
    if (gate_addr == NULL) {
        amtsvr_syslog(ctx, "watchdog_init : launch gate failed");
        return 1;
    }

    w->gate = strtoul(gate_addr + 1 , NULL, 16);
    if (w->gate == 0) {
        amtsvr_syslog(ctx, "watchdog_init : launch gate invalid %s", gate_addr);
        return 1;
    }

    wadog_dispatcher_init(w);
    w->ctx = ctx;
    amtsvr_callback(ctx, w, watchdog_proc_msg_cb);

    amtsvr_send(ctx, 0, w->gate, PTYPE_TEXT, 0, (void*)"start", sizeof("start"));
    if (w->inner) {
        amtsvr_command(ctx, "REG", ".watchdog_inner");
        WATCHDOG_NAMES.push_back(".watchdog_inner");
    } else {
        char reg_name[128] = {0};
        snprintf(reg_name, sizeof(reg_name), ".watchdog_%d", w->port); 
        amtsvr_command(ctx, "REG", reg_name);//".watchdog");
        WATCHDOG_NAMES.push_back(reg_name);
    }

    return 0;
}

void watchdog_release(struct watchdog_service_t* w)
{
    amtsvr_log("\n");
    delete w;
}
