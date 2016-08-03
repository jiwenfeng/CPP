#ifndef __PLAYER_ROLE_POTENTIAL_H__
#define __PLAYER_ROLE_POTENTIAL_H__

#include "potential_parser.h"

class player_role_potential : public potential_parser, public pb_writer
{
public:
    player_role_potential(mysql::tb_role_potential* b = NULL);

    virtual ~player_role_potential();

    int set_db_potential(mysql::tb_role_potential* b);
	// Ð´ÈëÊý¾Ý
    int save_potential(uint32 role_id, amtsvr_context* ctx);

    int attach_role_potential(int pb_count, cmq_svr_pb_ctx* pb_ctx);


public:

protected:
    mysql::tb_role_potential* db_role_potential;   

};


#endif

