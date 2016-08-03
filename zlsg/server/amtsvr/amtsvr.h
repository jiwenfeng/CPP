#ifndef AMTSVR_H
#define AMTSVR_H
 
#ifdef _WIN32
#include <LZSGConfig.h>
#endif // _WIN32

#include <stddef.h>
#include <stdint.h>

#include "amtsvr_error.h"

#define PTYPE_TEXT             0
#define PTYPE_RESPONSE         1
#define PTYPE_MULTICAST        2
#define PTYPE_CLIENT           3
#define PTYPE_SYSTEM           4
#define PTYPE_HARBOR           5
#define PTYPE_TIMER            6
#define PTYPE_SCRIPT           7
#define PTYPE_TAG_DONTCOPY     0x10000
#define PTYPE_TAG_ALLOCSESSION 0x20000

struct amtsvr_context;

void amtsvr_syslog(struct amtsvr_context * context, const char *msg, ...);
void amtsvr_billlog(struct amtsvr_context * context, const char *msg, ...);

const char * amtsvr_command(struct amtsvr_context * context, const char * cmd , const char * parm);
uint32_t amtsvr_queryname(struct amtsvr_context * context, const char * name);
int amtsvr_send(struct amtsvr_context * context, uint32_t source, uint32_t destination , int type, int session, void * msg, size_t sz);
int amtsvr_sendname(struct amtsvr_context * context, const char * destination , int type, int session, void * msg, size_t sz);

int amtsvr_broadcast(struct amtsvr_context * context, uint32_t * dests, int n, void* msg, size_t sz);

void amtsvr_forward(struct amtsvr_context *, uint32_t destination);

typedef int (*amtsvr_cb)(struct amtsvr_context * context, void *ud, int type, int session, uint32_t source , const void * msg, size_t sz);
void amtsvr_callback(struct amtsvr_context * context, void *ud, amtsvr_cb cb);

// 根据unix time获取当前登录第几天,开服当天及以前为第1天
void set_amtsvr_svr_start_time(unsigned int start_svr_time);
unsigned int amtsvr_svr_start_day(unsigned int now);
unsigned int amtsvr_svr_start_time();

#endif
