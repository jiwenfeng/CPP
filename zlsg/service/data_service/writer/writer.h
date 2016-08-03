#ifndef __DW_WRITER_H__
#define __DW_WRITER_H__

/*
#ifdef _cplusplus 
extern "C" 
{
#endif*/

#include "amtsvr.h"

    struct writer_t;

    struct writer_t* writer_create(void);

    int writer_init(struct writer_t *w, struct amtsvr_context *ctx, char *args);

    void writer_release(struct writer_t *w);

/*
#ifdef _cplusplus 
}
#endif*/

#endif
