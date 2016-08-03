#ifndef AMTSVR_SERVER_H
#define AMTSVR_SERVER_H

#include <stdint.h>
#include <stdlib.h>

struct amtsvr_context;
struct amtsvr_message;
struct amtsvr_monitor;

struct amtsvr_context * amtsvr_context_new(const char * name, const char * parm);
void amtsvr_context_grab(struct amtsvr_context *);
struct amtsvr_context * amtsvr_context_release(struct amtsvr_context *);
uint32_t amtsvr_context_handle(struct amtsvr_context *);
void amtsvr_context_init(struct amtsvr_context *, uint32_t handle);
int amtsvr_context_push(uint32_t handle, struct amtsvr_message *message);
void amtsvr_context_send(struct amtsvr_context * context, void * msg, size_t sz, uint32_t source, int type, int session);
int amtsvr_context_newsession(struct amtsvr_context *);
int amtsvr_context_message_dispatch(struct amtsvr_monitor *);	// return 1 when block
int amtsvr_context_total();

void amtsvr_context_endless(uint32_t handle);	// for monitor

int   get_amtsvr_ctx_mq_count(struct amtsvr_context* ctx);
void* get_amtsvr_ctx_mq_data(struct amtsvr_context* ctx, int index);

#endif
