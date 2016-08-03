#include "gate_service.h"
#include "mread.h"

#ifndef _WIN32
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#endif // _WIN32
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdarg.h>

struct connection {
    uint32_t agent;
    uint32_t client;
    int connection_id;
    int uid;
};

struct gate {
    struct mread_pool * pool;
    uint32_t watchdog;
    uint32_t broker;
    int id_index;
    int cap;
    int max_connection;
    int client_tag;
    int header_size;
    struct connection ** agent;
    struct connection * map;
};

struct gate* gate_create(void) {
    struct gate * g = (struct gate *)malloc(sizeof(*g));
    memset(g,0,sizeof(*g));
    return g;
}

static inline struct connection * 
    _id_to_agent(struct gate *g,int uid) {
        struct connection * agent = g->agent[uid & (g->cap - 1)];
        if (agent && agent->uid == uid) {
            return agent;
        }
        return NULL;
}

static void _parm(char *msg, int sz, int command_sz) {
    while (command_sz < sz) {
        if (msg[command_sz] != ' ')
            break;
        ++command_sz;
    }
    int i;
    for (i=command_sz;i<sz;i++) {
        msg[i-command_sz] = msg[i];
    }
    msg[i-command_sz] = '\0';
}

static void _forward_agent(struct gate * g, int id, uint32_t agentaddr, uint32_t clientaddr) {
    struct connection * agent = _id_to_agent(g,id);
    if (agent) {
        agent->agent = agentaddr;
        agent->client = clientaddr;
    }
}

static void _ctrl(struct amtsvr_context * ctx, struct gate * g, const void * msg, int sz) {
    if (sz == 0) {
        return;
    }

    char* tmp = (char*)malloc(sizeof(char) * (sz + 1));
    memcpy(tmp, msg, sz);
    tmp[sz] = '\0';
    char * command = tmp;
    int i;

    for (i=0;i<sz;i++) {
        if (command[i]==' ') {
            break;
        }
    }

    do {
        if (memcmp(command,"kick",i)==0) {
            _parm(tmp, sz, i);
            int uid = strtol(command , NULL, 10);
            struct connection * agent = _id_to_agent(g,uid);
            if (agent) {
                int connection_id = agent->connection_id;
                mread_close_client(g->pool,connection_id);
            }

            break;
        }
        if (memcmp(command,"forward",i)==0) {
            _parm(tmp, sz, i);
            char * client = tmp;
            char * idstr = strsep(&client, " ");
            if (client == NULL) {
                break;
            }
            int id = strtol(idstr , NULL, 10);
            char * agent = strsep(&client, " ");
            if (client == NULL) {
                break;
            }
            uint32_t agent_handle = strtoul(agent+1, NULL, 16);
            uint32_t client_handle = strtoul(client+1, NULL, 16);
            _forward_agent(g, id, agent_handle, client_handle);
            break;
        }
        if (memcmp(command,"broker",i)==0) {
            _parm(tmp, sz, i);
            g->broker = amtsvr_queryname(ctx, command);
            break;
        }
        if (memcmp(command,"start",i) == 0) {
            amtsvr_command(ctx,"TIMEOUT","0");
            break;
        }

        amtsvr_syslog(ctx, "[gate] Unkown command : %s", command);
    } while (false);

    free(tmp);
}


static void _report(struct gate *g, struct amtsvr_context * ctx, const char * data, ...) {
    if (g->watchdog == 0) {
        return;
    }

    va_list ap;
    va_start(ap, data);
    char tmp[1024];
    int n = vsnprintf(tmp, sizeof(tmp), data, ap);
    va_end(ap);

    amtsvr_send(ctx, 0, g->watchdog, PTYPE_TEXT,  0, tmp, n);
}

static void _forward(struct amtsvr_context * ctx,struct gate *g, int uid, void * data, size_t len) {
    if (g->broker) {
        amtsvr_send(ctx, 0, g->broker, g->client_tag, 0, data, len);
        return;
    }

    struct connection * agent = _id_to_agent(g,uid);
    if (agent == NULL)
        return;
    if (agent->agent) {
        amtsvr_send(ctx, agent->client, agent->agent, g->client_tag, 0 , data, len);
    } else if (g->watchdog) {
        char * tmp = (char *)malloc(len + 32);
        int n = snprintf(tmp,len+32,"%d data ",uid);
        memcpy(tmp+n,data,len);
        amtsvr_send(ctx, 0, g->watchdog, PTYPE_TEXT | PTYPE_TAG_DONTCOPY, 0, tmp, len + n);
    }
}

static int _gen_id(struct gate * g, int connection_id) {
    int uid = ++g->id_index;
    int i;
    for (i=0;i<g->cap;i++) {
        int hash = (uid + i) & (g->cap - 1);
        if (g->agent[hash] == NULL) {
            uid = uid + i;
            struct connection * conn = &g->map[connection_id];
            conn->uid = uid;
            g->agent[hash] = conn;
            g->id_index = uid;
            return uid;
        }
    }
    assert(0);
    return uid;
}

static void _remove_id(struct gate *g, int uid) {
    struct connection ** pconn = &g->agent[uid & (g->cap - 1)];
    struct connection * conn = *pconn;
    if (conn) {
        assert(conn->uid == uid);
        conn->uid = 0;
        conn->agent = 0;
        *pconn = NULL;
    }
}

static int _read_cb(struct gate *g, struct amtsvr_context * ctx, int connection_id)
{
    if (connection_id < 0) {
        return 0;
    }

    struct mread_pool * m = g->pool;
    int id = g->map[connection_id].uid;
    if (id == 0) {
        id = _gen_id(g, connection_id);
        int fd = mread_socket(m , connection_id);
        struct sockaddr_in remote_addr;
        socklen_t len = sizeof(struct sockaddr_in);
        getpeername(fd, (struct sockaddr *)&remote_addr, &len);
        _report(g, ctx, "%d open %d %s:%u",id,fd,inet_ntoa(remote_addr.sin_addr),ntohs(remote_addr.sin_port));
    }

    uint8_t * plen = (uint8_t *)mread_pull(m,g->header_size);
    if (plen == NULL) {
        if (mread_closed(m)) {
            _remove_id(g,id);
            _report(g, ctx, "%d close", id);
        }
        return 0;
    }

    if (*plen == 't'){ //tgw_l7_forward\r\nHost: s2.app100713133.qqopenapp.com:8001\r\n\r\n  tgw包头
        // tgw的客户端特殊处理，发定长64个字节, 已经读取了4个字节，再读取60个字节
        void * data = mread_pull(m, 60);
        if (data == NULL) {
            if (mread_closed(m)) {
                _remove_id(g,id);
                _report(g, ctx, "%d close", id);
            }
            return 0;
        }
        mread_yield(m);
        return 0;
    }

    int len ;
    if (g->header_size == 2) {
        len = plen[0] << 8 | plen[1];
    } else {
        len = plen[0] << 24 | plen[1] << 16 | plen[2] << 8 | plen[3];
    }

    if (len <= 0 || len > 65535) {
        struct connection * agent = _id_to_agent(g, id);
        if (agent) {
            mread_close_client(g->pool,connection_id);
        }
        return 0;
    }

    void * data = mread_pull(m, len);
    if (data == NULL) {
        if (mread_closed(m)) {
            _remove_id(g,id);
            _report(g, ctx, "%d close", id);
        }
        return 0;
    }

    _forward(ctx, g, id, data, len);
    mread_yield(m);
    return 0;
}

static int _write_cb(struct gate *g, struct amtsvr_context * ctx, int type, int session, uint32_t source, const void * msg, size_t sz)
{
    if (sz <= 4) {
        amtsvr_syslog(ctx, "Invalid client message from %x",source);
        return 0;
    }
    struct mread_pool * m = g->pool;
    // The first 4 bytes in msg are the id of socket, write following bytes to it
    const uint8_t * data = (const uint8_t *)msg;
    uint32_t uid = data[0] | data[1] << 8 | data[2] << 16 | data[3] << 24;
    struct connection * agent = _id_to_agent(g,uid);
    if (agent) {
        int id = agent->connection_id;
        mread_push(m, id, (void *)(data+4), sz - 4, (void *)data);
        return 1;
    } else {
        amtsvr_syslog(ctx, "Invalid client id %d from %x",(int)uid,source);
        return 0;
    }
}

static int _cb(struct amtsvr_context * ctx, void * ud, int type, int session, uint32_t source, const void * msg, size_t sz) {
    struct gate *g = (struct gate *)ud;
    if (type == PTYPE_TEXT) {
        _ctrl(ctx, g , msg , (int)sz);
        return 0;
    } else if (type == PTYPE_CLIENT) {
        return _write_cb(g, ctx, type, session, source, msg, sz);
    } /*else if (type == PTYPE_SYSTEM) {
        if (memcmp(msg, "close_all", strlen("close_all")) == 0) {
            amtsvr_command(ctx, "EXIT", NULL);
        }
        return 0;
    }*/

    assert(type == PTYPE_TIMER);
    struct mread_pool * m = g->pool;
    int connection_id = mread_poll(m,10);	// timeout : 10ms
    _read_cb(g, ctx, connection_id);

    amtsvr_command(ctx, "TIMEOUT", "0");

    return 0;
}

int gate_init(struct gate *g , struct amtsvr_context * ctx, char * parm) {
    int port = 0;
    int max = 0;
    int buffer = 0;
    int sz = strlen(parm)+1;
    char* watchdog = (char*)malloc(sz * sizeof(char));
    char* binding  = (char*)malloc(sz * sizeof(char));
    int client_tag = 0;
    char header;
    int n = sscanf(parm, "%c %s %s %d %d %d",&header,watchdog, binding,&client_tag , &max,&buffer);
    int ret = 0;
    do {
        if (n<4) {
            amtsvr_syslog(ctx, "Invalid gate parm %s",parm);
            ret = 1;
            break;
        }

        if (max <=0 ) {
            amtsvr_syslog(ctx, "Need max conntection");
            ret = 1;
            break;
        }

        if (header != 'S' && header !='L') {
            amtsvr_syslog(ctx, "Invalid data header style");
            ret = 1;
            break;
        }

        if (client_tag == 0) {
            client_tag = PTYPE_CLIENT;
        }

        char * portstr = strchr(binding,':');
        uint32_t addr = INADDR_ANY;
        if (portstr == NULL) {
            port = strtol(binding, NULL, 10);
            if (port <= 0) {
                amtsvr_syslog(ctx, "Invalid gate address %s",parm);
                ret = 1;
                break;
            }
        } else {
            port = strtol(portstr + 1, NULL, 10);
            if (port <= 0) {
                amtsvr_syslog(ctx, "Invalid gate address %s",parm);
                ret = 1;
                break;
            }
            portstr[0] = '\0';
            addr=inet_addr(binding);
        }
        struct mread_pool * pool = mread_create(addr, port, max, buffer);
        if (pool == NULL) {
            amtsvr_syslog(ctx, "Create gate %s failed",parm);
            ret = 1;
            break;
        }
        if (watchdog[0] == '!') {
            g->watchdog = 0;
        } else {
            g->watchdog = amtsvr_queryname(ctx, watchdog);
            if (g->watchdog == 0) {
                amtsvr_syslog(ctx, "Invalid watchdog %s",watchdog);
                ret =  1;
                break;
            }
        }

        g->pool = pool;
        int cap = 1;
        while (cap < max) {
            cap *= 2;
        }
        g->cap = cap;
        g->max_connection = max;
        g->id_index = 0;
        g->client_tag = client_tag;
        g->header_size = header=='S' ? 2 : 4;

        g->agent = (struct connection **)malloc(cap * sizeof(struct connection *));
        memset(g->agent, 0, cap * sizeof(struct connection *));

        g->map  = (struct connection *)malloc(max * sizeof(struct connection));
        memset(g->map, 0, max * sizeof(struct connection));
        for (int i = 0; i < max; ++i) {
            g->map[i].connection_id = i;
        }

        amtsvr_callback(ctx,g,_cb);
    } while(0);

    free(watchdog);
    free(binding );

    return ret;
}

void gate_release(struct gate *g)
{
    amtsvr_log("\n");
    if (g->pool) {
        mread_close(g->pool);
    }

    if (g->agent) {
        free(g->agent);
        g->agent = NULL;
    }

    if (g->map != NULL) {
        free(g->map);
        g->map = NULL;
    }

    free(g);
}
