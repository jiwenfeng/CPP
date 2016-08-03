#ifndef __PLAYER_ROLE_BLACK_SHOP_H__
#define __PLAYER_ROLE_BLACK_SHOP_H__

#include "black_shop_parser.h"
#include "client.role_props.pb.h"
class player_role_black_shop : public black_shop_parser, public pb_writer
{
public:
    player_role_black_shop(mysql::tb_role_black_shop* b = NULL);

    virtual ~player_role_black_shop();

    int set_db_black_shop(mysql::tb_role_black_shop* b);
	// Ð´ÈëÊý¾Ý
    int save_black_shop(uint32 role_id, amtsvr_context* ctx);

    int attach_role_black_shop(int pb_count, cmq_svr_pb_ctx* pb_ctx);

	bool reflesh_shop(uint32 base_denominator,std::map<uint32,shop_info> config_treasure_map_);
public:

protected:
    mysql::tb_role_black_shop* db_role_black_shop;   

};


#endif

