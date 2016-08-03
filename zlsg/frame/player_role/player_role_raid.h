#ifndef __PLAYER_ROLE_RAID_H__
#define __PLAYER_ROLE_RAID_H__

#include "comm_define.h"
#include "client.raid_operate.pb.h"

class player_role_raid : public pb_writer
{
public:
    player_role_raid(mysql::tb_role_raid* d = NULL);
    player_role_raid(const player_role_raid& prr);
    virtual ~player_role_raid();

public:
    int init(mysql::tb_role_raid* d);

    inline mysql::tb_role_raid* mutable_raid() {return _db_role_raid;}

    inline void detach() { _db_role_raid = NULL; }
    
    void save(amtsvr_context* ctx);
    
    void release();

	const uint32 get_elite_raid_enter_count() const;

public:
    int create(uint32 role_id, uint32 raid_id, uint32 star_num, uint32 complete_tick, uint32 complete_time, uint32 type, uint32 count = 0);

    int set(uint32 star_num, uint32 complete_tick, uint32 complete_time, uint32 type);

    int set_auto(uint32 type);

    const uint32 get_star_num() const;

	const uint32 get_elite_star_num() const;

	void modify_raid_star(uint32 num);

    const uint32 get_complete_count(uint32 type) const;

    void fill_info(client::raid_info& ri);
    
protected:
    mysql::tb_role_raid* _db_role_raid; 
};

#endif // __PLAYER_ROLE_RAID_H__
