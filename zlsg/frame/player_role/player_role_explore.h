#ifndef __PLAYER_ROLE_EXPLORE_H__
#define __PLAYER_ROLE_EXPLORE_H__

#include "comm_define.h"
#include "player_role_explore_detail.h"

class player_role_explore : public pb_writer
{
public:
    player_role_explore(mysql::tb_role_explore* p = NULL);
    virtual ~player_role_explore();

public:
    int init(uint32 role_id, mysql::tb_role_explore* tre);

    int init_detail(mysql::tb_role_explore_detail* tred);

    int save(amtsvr_context* ctx);

    int save_detail(amtsvr_context* ctx);
    
    int attach_role_explore(int pb_count, cmq_svr_pb_ctx* pb_ctx);
    int attach_role_explore_detail(int pb_count, cmq_svr_pb_ctx* pb_ctx);
    
public:
    
private:
//     uint32                                        _career;
    mysql::tb_role_explore*                       _explore;
    multimap<uint32, player_role_explore_detail*> _detail;
};

#endif /* __PLAYER_ROLE_EXPLORE_H__ */
