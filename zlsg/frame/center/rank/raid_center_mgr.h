#ifndef __RAID_CENTER_MGR_H__
#define __RAID_CENTER_MGR_H__

#include "comm_define.h"

class raid_center_mgr : public pb_writer
{
public:
    raid_center_mgr();
    ~raid_center_mgr();

public:
    int init(struct cmq_t* l);

    int save(amtsvr_context* ctx);

public:
    
private:
    std::map<uint32, mysql::tb_raid_center*> _raid_center;
    std::map<uint32, WRITER_OPERATE_TYPE_E>  _writer_type;
    bool _change;
};

#endif // __RAID_CENTER_MGR_H__
