#ifndef __PLAYER_ROLE_GEM_H__
#define __PLAYER_ROLE_GEM_H__

#include "gem_parser.h"
#include "config_gem.h"
class player_role_gem : public gem_parser, public pb_writer
{
public:
    player_role_gem(mysql::tb_role_gem* b = NULL);

    virtual ~player_role_gem();

    int set_db_gem(mysql::tb_role_gem* b);
	// Ð´ÈëÊý¾Ý
    int save_gem(uint32 role_id, amtsvr_context* ctx);

    int attach_role_gem(int pb_count, cmq_svr_pb_ctx* pb_ctx);

	void fill_gem_pos_info(uint32 gem_pos,gem_level_up_rsp& rsp_msg);

	bool check_is_max(uint32 gem_pos);

public:

protected:
    mysql::tb_role_gem* db_role_gem;   

};


#endif

