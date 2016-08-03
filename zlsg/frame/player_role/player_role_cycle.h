#ifndef __PLAYER_ROLE_CYCLE_H__
#define __PLAYER_ROLE_CYCLE_H__

#include "comm_define.h"
#include "mysql.tb_role_cycle.pb.h"
#include "client.cycle.pb.h"

class player_role_cycle : public pb_writer
{
public:
	player_role_cycle(mysql::tb_role_cycle *trc = NULL);
	~player_role_cycle();

public:
	int save(uint32 role_id, amtsvr_context *ctx);
	void attach_role_cycle(int count, cmq_svr_pb_ctx *pb_ctx);
	int init_role_cycle(uint32 role_id);
	int serialize_helpers(string &str);
	int unserialize_helpers(const string &str);
	int serialize_levels(string &str);
	int unserialize_levels(const string &str);

public:
	bool is_helped_me(uint32 role_id) const;
	void add_helper(uint32 role_id);
	void clear_helpers();
	void update_role_level(uint32 role_id, uint32 level, uint32 tick, uint32 update);
	const client::cycle_level_info *get_role_level_info(uint32 level) const;
	uint32 get_level_reward_type(uint32 level);
	uint32 get_top_level();
	uint32 get_cur_level();
	int reset_level();
	const std::map<uint32, uint32>& get_all_helpers() { return _helpers; }
	int set_cur_level(uint32 level);
	uint32 get_level_used_tick(uint32 level);
	bool award_fastest_prize(uint32 level);
	void update_pass_level_time(uint32 level);

private:
	mysql::tb_role_cycle *_trc;

	std::map<uint32, uint32> _helpers;
	std::map<uint32, client::cycle_level_info> _levels;
};

#endif
