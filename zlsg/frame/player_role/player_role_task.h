#ifndef __PLAYER_ROLE_TASK_H__
#define __PLAYER_ROLE_TASK_H__

#include "task_parser.h"

class player_role_task : public task_parser, public pb_writer
{
public:
    player_role_task();
    virtual ~player_role_task();

public:
    int init(mysql::tb_role_task* db);

    int save(amtsvr_context* ctx, uint32 role_id);

    int attach_role_task(int pb_count, cmq_svr_pb_ctx* pb_ctx);

protected:
    mysql::tb_role_task* db_role_task;
};

#endif
