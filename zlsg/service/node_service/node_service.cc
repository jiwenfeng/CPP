#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include "server.node.pb.h"
#include "pb_mgr.h"
#include "pb_coder.h"
#include "msg_handler_pool.h"
#include "node_interface.h"
#include "node_service.h"

static int _do(struct node_service_t* service)
{
    google::protobuf::Message* pb = client_decode(service->buffer, service->read_length);
    if (pb == NULL) {
        return 1;
    }

    server::forward_msg* msg = static_cast<server::forward_msg*>(pb);
    std::map<uint32, node_role>::iterator i = service->roles.find(msg->id());
    if (i == service->roles.end()) {
        delete pb;
        return 2;
    }

    if (msg->type() == server::forward_msg_target_client) {
        amtsvr_send(service->ctx, 0, i->second.client_id, PTYPE_CLIENT, 0, (void*)msg->data().c_str(), msg->data().length());
    } else { // 服务器消息处理 更新用户数据相关
        amtsvr_send(service->ctx, 0, i->second.agent_id, PTYPE_CLIENT, 0, (void*)msg->data().c_str(), msg->data().length());
    }

    delete pb;

    if (service->read_length != 0) {
        return _do(service);
    }

    return 0;
}

static int _read(struct node_service_t* service)
{
    int bytes = 0;
    for ( ; ; ) {
#ifdef _WIN32
        bytes = recv(service->socket, service->buffer + service->read_length, NODE_BUFFER_LENGTH - service->read_length, 0);
#else
        bytes = read(service->socket, service->buffer + service->read_length, NODE_BUFFER_LENGTH - service->read_length);
#endif // _WIN32
        if (bytes > 0) {
            // 标记缓存占用量
            service->read_length += bytes;
            if (service->read_length >= NODE_BUFFER_LENGTH) {
                return 0;
            }

            continue;
        }

        if (bytes == 0) {
            // 读取完成
            return 0;
        }

        if (bytes == -1) {
            switch(errno) {
            case EWOULDBLOCK:
                // 暂时没有消息
                return 0;
            case EINTR:
                // 可忽略
                continue;
            default:
                // 致命错误 丢失所有的跨服数据 要把之前跨服玩家转移回主场景 再重置socket重连中心服
                return -1;
            }
        }
    }

    return 0;
}

static int _timer(struct amtsvr_context* ctx, struct node_service_t* service)
{
    _read(service);

    _do(service);

    amtsvr_command(ctx, "TIMEOUT", "0");

    return 0;
}

static int _msg(struct amtsvr_context* ctx, struct node_service_t* service, const void* msg, size_t sz)
{
    msg_handler* mh = static_cast<msg_handler*>((void*)msg);
    google::protobuf::Message* pb = mh->msg;
    mh->msg = NULL;
    MSG_HANDLER_POOL->push(mh);
    if (pb == NULL) {
        return 1;
    }

    if (pb->GetTypeName() == "server.node_role_req") {
        server::node_role_req* req = static_cast<server::node_role_req*>(pb);
        service->roles[req->role_id()].role_id   = req->role_id();
        service->roles[req->role_id()].agent_id  = req->agent_id();
        service->roles[req->role_id()].client_id = req->client_id();
        //delete pb;
        //return 0;
    }

    char buf[NODE_BUFFER_LENGTH] = {0};
    uint32 len = 0;
    client_encode(*pb, buf, NODE_BUFFER_LENGTH, len);
    node_send_server(service, buf, len);
    delete pb;

    return 0;
}

static void _ctrl(struct amtsvr_context * ctx, struct node_service_t * service, const void * msg, int sz) {
    if (sz == 0) {
        return;
    }

    //const char* tmp = (const char*)msg;
    //if (strstr(tmp, "open") != NULL) {
    //return 0;
    //}

    char* tmp = (char*)malloc(sizeof(char) * (sz + 1));
    memcpy(tmp, msg, sz);
    tmp[sz] = '\0';
    char * command = tmp;
    int i;
    for (i = 0; i < sz; ++i) {
        if (command[i] == ' ') {
            break;
        }
    }

    if (memcmp(command, "start", i) == 0) {
        amtsvr_command(ctx,"TIMEOUT","0");
    } else if (memcmp(command, "stop", i) == 0) {
        amtsvr_command(ctx, "EXIT", NULL);
        amtsvr_syslog(ctx, "node_service|will exit now|%d|", service->node);
    } else {
        amtsvr_syslog(ctx, "[node] Unkown command : %s", command);
    }

    free(tmp);
}

static int _system(struct amtsvr_context * ctx, struct node_service_t * service, const void * msg, int sz)
{
    if (memcmp(msg, CLOSE_ALL, strlen(CLOSE_ALL)) == 0) {
        amtsvr_command(ctx, "EXIT", NULL);
        amtsvr_syslog(ctx, "node_service|will exit now|%d|", service->node);
        return 0;
    }

    return 0;
}

static int _cb(struct amtsvr_context* ctx, void* ud, int type, int session, uint32_t source, const void* msg, size_t sz)
{
    node_service_t* service = (node_service_t*)ud;

    if (type == PTYPE_TIMER) {
        return _timer(ctx, service);
    } else if (type == PTYPE_CLIENT) {
        _msg(ctx, service, msg, sz);
    } else if (type == PTYPE_TEXT) {
        _ctrl(ctx, service, msg, sz);
    } else if (type == PTYPE_SYSTEM) {
        _system(ctx, service, msg, sz);
    }

    return 0;
}

static int register_node(struct node_service_t* w)
{
    server::register_node_req req;
    req.set_server_id(w->server_id);
    req.set_key("test");
    char buf[NODE_BUFFER_LENGTH] = {0};
    uint32 len = 0;
    client_encode(req, buf, NODE_BUFFER_LENGTH, len);
    node_send_server(w, buf, len);

    return 0;
}

struct node_service_t* node_create(void)
{
    struct node_service_t* service = new node_service_t;

    service->node = 0;

    memset(service->ip, 0, sizeof(service->ip));
    service->port      = 0;
    service->server_id = 0;
    service->socket    = 0;

    memset(service->buffer, 0, sizeof(service->buffer));
    service->read_length = 0;

    service->ctx = NULL;

    return service;
}

int node_init(struct node_service_t* w, struct amtsvr_context* ctx, char* args)
{
    w->ctx = ctx;
    node_dispatcher_init(w);
    amtsvr_callback(ctx, w, _cb);

    memset(w->ip, 0, sizeof(w->ip));
    sscanf(args, "%s %hd %d %d", w->ip, &w->port, &w->node, &w->server_id);

    w->socket = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(struct sockaddr_in));
    server_addr.sin_family      = AF_INET;
    server_addr.sin_port        = htons(w->port);
    server_addr.sin_addr.s_addr = inet_addr(w->ip);
    int ret = connect(w->socket, (const sockaddr*)&server_addr, sizeof(struct sockaddr));
    if (ret != 0) {
        amtsvr_syslog(ctx, "node_init : connect failed(%s)", args);
        return 1;
    }

    register_node(w);

#ifndef _WIN32
    ret = fcntl(w->socket, F_GETFL, 0);
    if ( -1 == ret || fcntl(w->socket, F_SETFL, ret | O_NONBLOCK) == -1) {
        printf("fcntl failed with error: %d\n", ret);
        return 1;
    }
#else
    u_long iMode = 1;
    ret = ioctlsocket(w->socket, FIONBIO, &iMode);
    if (ret != NO_ERROR) {
        printf("ioctlsocket failed with error: %d\n", ret);
        return 1;
    }
#endif //_WIN32
    //fcntl(w->socket, F_SETFL, O_NONBLOCK);

    char reg_name[128] = {0};
    snprintf(reg_name, sizeof(reg_name), ".node_%d", w->node);
    amtsvr_command(ctx, "REG", reg_name);//".node");
    NODE_NAMES.push_back(reg_name);

    return 0;
}

void node_release(struct node_service_t* w)
{
    amtsvr_log("\n");
    delete w;
}
