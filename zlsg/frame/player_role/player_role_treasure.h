#ifndef __PLAYER_ROLE_TREASURE_H__
#define __PLAYER_ROLE_TREASURE_H__

#include "treasure_parser.h"

class player_role_treasure : public treasure_parser,public pb_writer
{
public:
    player_role_treasure(mysql::tb_role_treasure* db = NULL);

    virtual ~player_role_treasure();

	int set_db_treasure(mysql::tb_role_treasure* b);

    int save_treasure(uint32 role_id, amtsvr_context* ctx);

	int attach_role_treasure(int pb_count, cmq_svr_pb_ctx* pb_ctx);
public:


public:


private:
    mysql::tb_role_treasure* db_role_treasure;
};

#endif
