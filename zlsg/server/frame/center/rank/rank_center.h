#ifndef __RANK_CENTER_H__
#define __RANK_CENTER_H__

#include "define.h"
#include "pb_mgr.h"
#include "pb_writer.h"
#if 0
class rank_center : public pb_writer
{
public:
    rank_center(mysql::tb_rank_center* trc = NULL);
    virtual ~rank_center();

public:
    int set(uint32 index, uint32 Type, uint64 id, uint32 val);

    mysql::tb_rank_center* get();

    int save(amtsvr_context* ctx);

private:
    mysql::tb_rank_center* db_rank_center;
    bool change;
};
#endif
#endif /* __RANK_CENTER_H__ */
