#ifndef __GAME_SERVICE_H__
#define __GAME_SERVICE_H__

/*
#ifdef _cplusplus 
extern "C" 
{
#endif*/

#include "amtsvr.h"

struct game_service_t;

struct game_service_t* game_create(void);

int game_init(struct game_service_t* s, struct amtsvr_context* ctx, char* args);

void game_release(struct game_service_t* s);

/*
#ifdef _cplusplus 
}
#endif*/

#endif
