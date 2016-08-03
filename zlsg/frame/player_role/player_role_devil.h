#ifndef __PLAYER_ROLE_DEVIL_H__
#define __PLAYER_ROLE_DEVIL_H__

#include "comm_define.h"
#include "mysql.tb_role_devil.pb.h"
#include "client.devil.pb.h"

class player_role_devil : public pb_writer
{
public:
	player_role_devil() : _trd(NULL) {}
	~player_role_devil();
	void init_role_devil(uint32 role_id);
	void save(uint32 role_id, amtsvr_context *ctx);
	void attach_role_devil(int count, cmq_svr_pb_ctx *pb_ctx);

public:
	bool is_stage_award(uint32 id);
	void set_stage_award(uint32 id);
	void add_raid_info(uint32 raid_id, uint32 star_num, uint32 tick, uint32 champion);
	int get_reward_type(uint32 raid_id);
	int reset_raid(uint32 raid_id);
	const client::devil_raid_info *get_devil_raid(uint32 raid_id) const;
	const std::map<uint32, client::devil_raid_info> &get_devil_raids() const { return _raid; }
	const std::map<uint32, uint32> &get_stage_info() const { return _stage; }
	bool raid_is_perfect(uint32 raid_id);
	bool raid_is_champion(uint32 raid_id);
	bool raid_is_complete(uint32 raid_id);
	void fill_devil_raids(client::devil_stage_list &list);
	int fill_devil_raid_info(uint32 id, client::devil_raid_detail &drd);

private:
	int serialize_stage(string &str);
	int serialize_raid(string &str);
	int unserialize_stage(const string &str);
	int unserialize_raid(const string &str);

private:
	mysql::tb_role_devil *_trd;
	std::map<uint32, uint32> _stage;
	std::map<uint32, client::devil_raid_info> _raid;
};

#endif /* __PLAYER_ROLE_DEVIL_H__ */
