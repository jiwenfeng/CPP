#ifndef __AGENT_SERVICE_H__
#define __AGENT_SERVICE_H__

/*
#ifdef _cplusplus 
extern "C" 
{
#endif*/

#include "amtsvr.h"

struct agent_service_t;

struct agent_service_t* agent_create(void);

int agent_init(struct agent_service_t* a, struct amtsvr_context* ctx, char* args);

void agent_release(struct agent_service_t* a);

/*
#ifdef _cplusplus 
}
#endif*/

#endif
