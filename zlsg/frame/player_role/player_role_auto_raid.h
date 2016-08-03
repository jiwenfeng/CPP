#ifndef __PLAYER_ROLE_AUTO_RAID_H__
#define __PLAYER_ROLE_AUTO_RAID_H__

#include "comm_define.h"
#include "client.raid_operate.pb.h"

#define MAX_AUTO_RAID_BOLB_LEN 66560

class player_role_auto_raid : public pb_writer {
public:
    player_role_auto_raid(uint32 role_id, mysql::tb_role_auto_raid* auto_raid = NULL);
    virtual ~player_role_auto_raid();

public:
    client::auto_raid_info& get() { return _info; }
    int save(amtsvr_context* ctx);

public:
    void set_auto_raid(uint32 raid_id, uint32 need_time, uint32 count, uint32 type);

protected:
    int init(uint32 role_id, mysql::tb_role_auto_raid* s);

    void clear();

    int parse(const string& in);

    int serialize(string& out);

private:
    mysql::tb_role_auto_raid* _auto_raid;
    client::auto_raid_info    _info;
};

#endif // __PLAYER_ROLE_AUTO_RAID_H__