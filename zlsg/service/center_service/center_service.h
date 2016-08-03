#ifndef __CENTER_SERVICE_H__
#define __CENTER_SERVICE_H__

/*
#ifdef _cplusplus 
extern "C" 
{
#endif*/

#include "amtsvr.h"

struct center_service_t;

struct center_service_t* center_create(void);

int center_init(struct center_service_t* w, struct amtsvr_context* ctx, char* args);

void center_release(struct center_service_t* w);

/*
#ifdef _cplusplus 
}
#endif*/

#endif
