#ifndef __CHORES_SERVICE_H__
#define __CHORES_SERVICE_H__

#include "amtsvr.h"

struct chores_service_t;

struct chores_service_t* chores_create(void);

int chores_init(struct chores_service_t* c, struct amtsvr_context* ctx, char* args);

void chores_release(struct chores_service_t* c);

#endif
