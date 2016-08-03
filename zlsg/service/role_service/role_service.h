#ifndef __ROLE_SERVICE_H__
#define __ROLE_SERVICE_H__

/*
#ifdef _cplusplus 
extern "C" 
{
#endif*/

#include "amtsvr.h"

struct role_service_t;

struct role_service_t* role_create(void);

int role_init(struct role_service_t* a, struct amtsvr_context* ctx, char* args);

void role_release(struct role_service_t* a);

/*
#ifdef _cplusplus 
}
#endif*/


#endif
