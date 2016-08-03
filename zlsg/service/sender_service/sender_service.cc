#include "sender_service.h"

#ifndef _WIN32
#include <arpa/inet.h>
#endif // _WIN32
#include <stdint.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct sender {
    int gate;
    uint8_t id[4];
    int node_id;
};

static int node_cb(struct amtsvr_context * context, struct sender * c, int type, int session, uint32_t source, const void * msg, size_t sz) {
    // 加入node_id 重新打包

    // 发向结点代理

    return 1;
}

static int _cb(struct amtsvr_context * context, void * ud, int type, int session, uint32_t source, const void * msg, size_t sz) {
    if (msg == NULL || sz == 0) {
        return 1;
    }

    struct sender * c = (struct sender * )ud;

    if (c->node_id != 0) {
        return node_cb(context, c, type, session, source, msg, sz);
    }

    // tmp will be free by gate.
    // see gate/mread.c : mread_push()
    uint8_t *tmp = (uint8_t*)msg; //malloc(sz + 4 + 4);
    memcpy(tmp, c->id, 4);
    *((uint32_t*)(tmp + 4)) = htonl(sz - 8);
    //tmp[4] = (sz >> 8) & 0xff;
    //tmp[5] = sz & 0xff;
    //memcpy(tmp+8, msg, sz);

//     if (sz >= 16384) {
//         amtsvr_log("proto size too long sz:%u\n", sz);
//         return 1;
//     }

    amtsvr_send(context, source, c->gate, PTYPE_CLIENT | PTYPE_TAG_DONTCOPY, 0, tmp, sz); //+8);

    return 1;
}

int sender_init(struct sender *c, struct amtsvr_context *ctx, const char * args) {
    int fd = 0, gate = 0, id = 0, node_id = 0;
    sscanf(args, "%d %d %d %d", &fd, &gate, &id, &node_id);
    if (gate == 0) {
        amtsvr_syslog(ctx, "Invalid init sender %s",args);
        return 1;
    }

    c->gate  = gate;
    c->id[0] = id & 0xff;
    c->id[1] = (id >> 8) & 0xff;
    c->id[2] = (id >> 16) & 0xff;
    c->id[3] = (id >> 24) & 0xff;
    c->node_id = node_id;
    amtsvr_callback(ctx, c, _cb);

    return 0;
}

struct sender * sender_create(void) {
    struct sender *c = (struct sender *)malloc(sizeof(*c));
    memset(c,0,sizeof(*c));
    return c;
}

void sender_release(struct sender *c) {
    printf("%s:%d client service|sender_release|%d|%d|\n", __FILE__, __LINE__, c->gate, c->id[0]);
    free(c);
}
