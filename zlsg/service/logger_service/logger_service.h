#ifndef __AMTSVR_LOGGER_H__
#define __AMTSVR_LOGGER_H__

/*
#ifdef _cplusplus 
extern "C" 
{
#endif*/

    #include "amtsvr.h"

    struct logger;

    struct logger * logger_create(void);

    int logger_init(struct logger * inst, struct amtsvr_context *ctx, const char * parm);

    void logger_release(struct logger * inst);

/*
#ifdef _cplusplus 
}
#endif*/

#endif
