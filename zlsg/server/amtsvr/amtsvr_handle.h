#ifndef AMTSVR_CONTEXT_HANDLE_H
#define AMTSVR_CONTEXT_HANDLE_H

#include <stdint.h>

#define HANDLE_MASK 0xffffff
#define HANDLE_REMOTE_SHIFT 24

struct amtsvr_context;

uint32_t amtsvr_handle_register(struct amtsvr_context *);
struct amtsvr_context * amtsvr_handle_retire(uint32_t handle);
struct amtsvr_context * amtsvr_handle_grab(uint32_t handle);
void amtsvr_handle_retireall();

uint32_t amtsvr_handle_findname(const char * name);
const char * amtsvr_handle_namehandle(uint32_t handle, const char *name);

void amtsvr_handle_init(int harbor);

void amtsvr_handle_release();

#endif
