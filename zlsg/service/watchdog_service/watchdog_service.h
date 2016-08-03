#ifndef __WATCHDOG_SERVICE_H__
#define __WATCHDOG_SERVICE_H__

/*
#ifdef _cplusplus 
extern "C" 
{
#endif*/

#include "amtsvr.h"

struct watchdog_service_t;

struct watchdog_service_t* watchdog_create(void);

int watchdog_init(struct watchdog_service_t* w, struct amtsvr_context* ctx, char* args);

void watchdog_release(struct watchdog_service_t* w);

/*
#ifdef _cplusplus 
}
#endif*/

#endif
