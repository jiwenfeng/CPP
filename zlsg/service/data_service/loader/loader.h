#ifndef __LOADER_H__
#define __LOADER_H__

/*
#ifdef _cplusplus 
extern "C" 
{
#endif
*/

#include "amtsvr.h"

struct loader_t;

struct loader_t* loader_create(void);

int loader_init(struct loader_t *l, struct amtsvr_context *ctx, char *args);

void loader_release(struct loader_t *l);

/*
#ifdef _cplusplus 
}
#endif*/

#endif
