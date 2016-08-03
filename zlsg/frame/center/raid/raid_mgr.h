#ifndef __RAID_MGR_H__
#define __RAID_MGR_H__

#include "comm_define.h"
#include "raid_center.h"

class raid_mgr
{
public:
    raid_mgr();
    virtual ~raid_mgr();

public:
    int init(struct cmq_t* l);

    int update(client::raid_info_update* riu);
    
    raid_center* get(uint32 raid_id);

    int save(amtsvr_context* ctx);

private:
    map<uint32, raid_center*> _center;
};

#endif /* __RAID_MGR_H__ */
