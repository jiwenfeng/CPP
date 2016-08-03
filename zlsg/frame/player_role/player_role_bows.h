#ifndef __PLAYER_ROLE_BOWS_H__
#define __PLAYER_ROLE_BOWS_H__

#include "comm_define.h"
#include "client.role_bows.pb.h"
// ¹­¼ýÏµÍ³
class player_role_bows : public pb_writer
{
public:
    player_role_bows(mysql::tb_role_bows* p = NULL);
    virtual ~player_role_bows();

public:
    int attach(int pb_count, cmq_svr_pb_ctx* pb_ctx);

    int init(mysql::tb_role_bows* p);

    int save(amtsvr_context* ctx);
    
	int create_bows(uint32 role_id);

	void upgrate_bow_level(uint32 player_level,uint32 add_exp);

public:
    const mysql::tb_role_bows& get_db();
    mysql::tb_role_bows* mutable_db();

private:
    mysql::tb_role_bows* _db;
};

#endif // __PLAYER_ROLE_BOWS_H__
