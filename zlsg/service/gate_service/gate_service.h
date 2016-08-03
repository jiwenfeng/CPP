#ifndef __GATE_SERVICE_H__
#define __GATE_SERVICE_H__

/*
#ifdef _cplusplus 
extern "C" 
{
#endif*/

#include "amtsvr.h"

    struct gate;

    struct gate * gate_create(void);

    int gate_init(struct gate *g , struct amtsvr_context * ctx, char * parm);

    void gate_release(struct gate *g);

/*
#ifdef _cplusplus 
}
#endif*/

#endif
