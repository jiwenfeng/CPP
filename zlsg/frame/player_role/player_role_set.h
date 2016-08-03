#ifndef __PLAYER_ROLE_SET_H__
#define __PLAYER_ROLE_SET_H__

#include "set_parser.h"

#define SHORTCUT_MAX_GRID 19
#define SHORTCUT_MAX_TYPE 2

class player_role_set : public pb_writer, set_parser 
{
public:
    player_role_set(mysql::tb_role_set* s = NULL);
    ~player_role_set();

    int attach_role_set(int pb_count, cmq_svr_pb_ctx* pb_ctx);

public:
    int init(mysql::tb_role_set* s);
    
    int save(amtsvr_context* ctx);
    
public:
    void fill_shortcut(client::shortcut_rsp& rsp);
    void set_shortcut(client::shortcut_set_req* req, client::shortcut_set_rsp& rsp);

    void fill_handup(client::handup_rsp& rsp);
    void set_handup(client::handup_set_req* req, client::handup_set_rsp& rsp);

private:
    mysql::tb_role_set*  _db_role_set;
};

#endif /* __PLAYER_ROLE_SET_H__ */
