#ifndef __TEAM_MGR_H__
#define __TEAM_MGR_H__

#include <map>
#include "define.h"
#include "pb_mgr.h"
#include "comm_define.h"

using namespace std;

#define MAX_TEAM_MEMBER_NUM 4

class team_mgr
{
public:
    team_mgr();

    ~team_mgr();

public:

    uint32 create_team(uint32 captain, uint32 raid_id);

    int join_member(uint32 team_id, uint32 role_id);

    int leave_team(uint32 role_id, bool notify = true);

	int change_ready_status(uint32 role_id, bool status = true);

    int get_team_members(uint32 team_id, vector<uint32>& roles);

    client::team_info* get_team_info(uint32 team_id);

    client::team_info* get_my_team(uint32 role_id);

	bool is_all_ready(uint32 team_id);

	bool is_same_team(uint32 r1, uint32 r2);

    uint32 get_team_captain(uint32 team_id);

	uint32 get_team_id(uint32 role_id);

	uint32 get_member_count(uint32 team_id);

	void attach_ctx(amtsvr_context *ctx) { _ctx = ctx; }

	void change_team_status(uint32 team_id, uint32 status);

	uint32 get_team_status(uint32 team_id);

	uint32 get_team_raid(uint32 team_id);
protected:
    uint32 alloc_team_id();
	int team_send_center(google::protobuf::Message &coi);
	int send_leave_team_notify(uint32 role_id, uint32 type);
	int cancel_team(uint32 team_id, bool notify = true);

private:
    uint32 _team_id;                 // 分配的队伍id
    map<uint32, client::team_info> _teams; // 队伍ID-->队伍信息
    map<uint32, uint32> _role_team;        // role_id->team_id
	amtsvr_context *_ctx;
};

#endif
