#ifndef __CLIENT_SERVICE_H__
#define __CLIENT_SERVICE_H__

/*
#ifdef _cplusplus 
extern "C" 
{
#endif*/

#include "amtsvr.h"

    struct sender;

    struct sender * sender_create(void);

    int sender_init(struct sender *c, struct amtsvr_context *ctx, const char * args);

    void sender_release(struct sender *c);

/*
#ifdef _cplusplus 
}
#endif*/

#endif
