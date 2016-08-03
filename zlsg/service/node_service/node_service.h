#ifndef __NODE_SERVICE_H__
#define __NODE_SERVICE_H__

/*
#ifdef _cplusplus 
extern "C" 
{
#endif*/

#include "amtsvr.h"

struct node_service_t;

struct node_service_t* node_create(void);

int node_init(struct node_service_t* w, struct amtsvr_context* ctx, char* args);

void node_release(struct node_service_t* w);

/*
#ifdef _cplusplus 
}
#endif*/

#endif
