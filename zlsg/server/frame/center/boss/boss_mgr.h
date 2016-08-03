#ifndef __BOSS_MGR_H__
#define __BOSS_MGR_H__

#include "define.h"
#include "pb_writer.h"
#include "comm_define.h"

class boss_mgr
{
public:
	boss_mgr() {};
	~boss_mgr() {};
	int update(amtsvr_context *ctx);
	int init();
	uint32 get_refresh_time() {return m_refresh_time; }
	void refresh_world_boss(amtsvr_context *ctx);
	void change_boss_status(uint32 raid_id, uint32 status);
	uint32 get_boss_status(uint32 raid_id);

private:

	uint32 get_next_refresh_time();
	int send_game(amtsvr_context *ctx, google::protobuf::Message &coi, uint32 ssi);
	int send_center(amtsvr_context *ctx, google::protobuf::Message &coi);
	int broadcast_refresh_msg(amtsvr_context *ctx, bool &ret);

public:
	vector<uint32> m_boss_scene;
	map<uint32, uint32> _boss_status;

private:
	uint32 m_refresh_time;
	uint32 m_before;//刷新前广播
	uint32 m_after;//刷新后广播
	uint32 _broadcast;
	bool m_before_send;
	bool m_after_send;
	bool m_broadcast;
};
#endif
