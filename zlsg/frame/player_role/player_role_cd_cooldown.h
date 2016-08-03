#ifndef __PLAYER_ROLE_CD_COOLDOWN_H__
#define __PLAYER_ROLE_CD_COOLDOWN_H__

#include "cd_cooldown_parser.h"

class player_role_cd_cooldown : public cd_cooldown_parser, public pb_writer
{
public:
    player_role_cd_cooldown(mysql::tb_role_cd_cooldown* b = NULL);

    virtual ~player_role_cd_cooldown();

    int set_db_cd_cooldown(mysql::tb_role_cd_cooldown* b);
	// Ð´ÈëÊý¾Ý
    int save_cd_cooldown(uint32 role_id, amtsvr_context* ctx);

    int attach_role_cd_cooldown(int pb_count, cmq_svr_pb_ctx* pb_ctx);

	bool add_cooldown_by_id(uint32 type_id);

	uint32 get_cooldown_by_id(uint32 type_id);
public:

protected:
    mysql::tb_role_cd_cooldown* db_role_cd_cooldown;   

};


#endif

