#ifndef __PLAYER_ROLE_MOUNTS_H__
#define __PLAYER_ROLE_MOUNTS_H__

#include "comm_define.h"

class player_role_mounts : public pb_writer
{
public:
    player_role_mounts(mysql::tb_role_mounts* p = NULL);
    virtual ~player_role_mounts();

public:
    int attach(int pb_count, cmq_svr_pb_ctx* pb_ctx);

    int init(mysql::tb_role_mounts* p);

    int save(amtsvr_context* ctx);
    
	void upgrate_mount_level(uint32 player_level,uint32 add_exp);

public:
    const mysql::tb_role_mounts& get_db();
    mysql::tb_role_mounts* mutable_db();

private:
    mysql::tb_role_mounts* _db;
};

#endif // __PLAYER_ROLE_MOUNTS_H__
