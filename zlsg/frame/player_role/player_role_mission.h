/*
 *
 * 成就和每日任务
 *
 * */

#ifndef __PLAYER_ROLE_MISSION_H__
#define __PLAYER_ROLE_MISSION_H__

#include "comm_define.h"
#include "client.target.pb.h"
#include "mysql.tb_role_target.pb.h"

class player_role_mission : public pb_writer
{
public:
	player_role_mission(mysql::tb_role_target *trt = NULL);
	~player_role_mission();
public:
	void save(amtsvr_context *ctx, uint32 role_id);
	void clear_daily_progress();
	void attach_role_target(int count, cmq_svr_pb_ctx *pb_ctx);
	bool target_can_prize(uint32 id) const;
	bool target_has_finished(uint32 id) const;
	bool target_is_doing(uint32 id) const;
	const client::target_info *get_target_info(uint32 id);
private:
	int serialize(string &str);
	int unserialize(const string &str);

public:
	uint32 modify_target_progress(uint32 id, uint32 num, bool flag);
	void set_target_status(uint32 id, client::TARGET_STATUS_E status);
private:
	map<uint32, client::target_info *> _target;
	mysql::tb_role_target *_trt;
	uint32 _time;
};

#endif
