#ifndef __EXPLORE_CENTER_H__
#define __EXPLORE_CENTER_H__

#include "define.h"
#include "pb_mgr.h"
#include "pb_writer.h"

class explore_center : public pb_writer
{
public:
    explore_center(mysql::tb_role_explore_detail* tred = NULL);
    virtual ~explore_center();

public:
    int set(client::explore_update* eu);

    mysql::tb_role_explore_detail* get();

    int save(amtsvr_context* ctx);

private:
    mysql::tb_role_explore_detail* _explore;
    bool                           _change;
};

#endif /* __EXPLORE_CENTER_H__ */
