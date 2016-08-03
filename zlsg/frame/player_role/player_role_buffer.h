#ifndef __PLAYER_ROLE_BUFFER_H__
#define __PLAYER_ROLE_BUFFER_H__

#include "buffer_parser.h"

class player_role_buffer : public buffer_parser, public pb_writer
{
public:
    player_role_buffer(mysql::tb_role_buffer* b = NULL);

    virtual ~player_role_buffer();

    int set_db_buffer(mysql::tb_role_buffer* b);

    int save(uint32 role_id, amtsvr_context* ctx);

    int attach_role_buffer(int pb_count, cmq_svr_pb_ctx* pb_ctx);

public:

protected:
    mysql::tb_role_buffer* db_role_buffer;   

};


#endif

