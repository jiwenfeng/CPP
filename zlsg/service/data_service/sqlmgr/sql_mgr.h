#ifndef __SQL_MGR_SERVICE_H__
#define __SQL_MGR_SERVICE_H__

/*
#ifdef _cplusplus 
extern "C" 
{
#endif*/

#include "amtsvr.h"

#define SQLMGR_PING_TIME "500"  // 每隔5000ms检查一次mysql连接状态

struct sqlmgr_t;

struct sqlmgr_t* sqlmgr_create(void);

int sqlmgr_init(struct sqlmgr_t *s, struct amtsvr_context *ctx, char *args);

void sqlmgr_release(struct sqlmgr_t *s);

/*
#ifdef _cplusplus 
}
#endif*/

#endif
