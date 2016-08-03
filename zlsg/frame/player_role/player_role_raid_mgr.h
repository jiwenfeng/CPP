#ifndef __PLAYER_ROLE_RAID_MGR_H__
#define __PLAYER_ROLE_RAID_MGR_H__

#include "player_role_raid.h"

class player_role_raid_mgr
{
public:
    player_role_raid_mgr();
    virtual ~player_role_raid_mgr();

public:
    int attach_role_raid(int pb_count, cmq_svr_pb_ctx* pb_ctx);

    int push(mysql::tb_role_raid* p);
    int push(uint32 role_id, uint32 raid_id, uint32 star_num, uint32 complete_tick, uint32 complete_time, uint32 type = 0, uint32 count = 0);

	int modify_raid_star(uint32 raid_id, uint32 star_num);
    int set_auto(uint32 raid_id, uint32 type);

    void save(amtsvr_context* ctx);

	inline const std::map<uint32, player_role_raid>& get_all_raids() { return _raids; }

public:
    player_role_raid* mutable_raid(uint32 raid_id);
    void fill_raid_info_msg(client::raid_info_msg& rim);
    void fill_raid_info_msg(client::raid_info_msg& rim, uint32 raid_id);

protected:
    std::map<uint32, player_role_raid> _raids;  
};

#endif // __PLAYER_ROLE_RAID_MGR_H__
