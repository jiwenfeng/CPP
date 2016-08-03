#ifndef __PLAYER_ROLE_EXPLORE_DETAIL_H__
#define __PLAYER_ROLE_EXPLORE_DETAIL_H__

#include "comm_define.h"

class player_role_explore_detail : public pb_writer
{
public:
    player_role_explore_detail(mysql::tb_role_explore_detail* tec = NULL);
    virtual ~player_role_explore_detail();

public:
    int set(uint32 role_id, uint32 time, uint32 place_id, uint32 goods_id, uint32 goods_num, uint32 goods_quality);

    mysql::tb_role_explore_detail* get();

    int save(amtsvr_context* ctx);

private:
    mysql::tb_role_explore_detail* _db_role_explore_detail;
    bool _change;
};


#endif /* __PLAYER_ROLE_EXPLORE_DETAIL_H__ */
