#ifndef __RAID_CENTER_H__
#define __RAID_CENTER_H__

#include "define.h"
#include "pb_mgr.h"
#include "pb_writer.h"

class raid_center : public pb_writer
{
public:
    raid_center(mysql::tb_raid_center* trc = NULL);
    virtual ~raid_center();

public:
    int set(uint32 raid_id, uint32 role_id, uint32 time);

    mysql::tb_raid_center* get();

    int save(amtsvr_context* ctx);

private:
    mysql::tb_raid_center* _db_raid_center;
    bool _change;
};

#endif /* __RAID_CENTER_H__ */
