#include "center_interface.h"
#include "client.raid_operate.pb.h"
#include "config_raid.h"

static int match_robot(center_service_t *w, vector<uint32> &v, uint32 n, vector<uint32> except, uint32 lv, uint32 power)
{
	vector<uint32> vec;
	w->aus_mgr.get_users_greater_lvl(vec, lv, power, except);
	if(vec.size() < n)
	{
		return -1;
	}
	for(uint32 i = 0; i < n; ++i)
	{
		uint32 j = mtrandom::rand_int32(vec.size() - 1);
		v.push_back(vec[j]);
		vector<uint32>::iterator it = vec.begin() + j;
		vec.erase(it);
	}
	return 0;
}

static void broadcast_team_msg(center_service_t *w, uint32 team_id, google::protobuf::Message &msg)
{
	vector<uint32> v;
	w->tm_mgr.get_team_members(team_id, v);
	for(size_t i = 0; i < v.size(); ++i)
	{
		const online_role *oi = w->oi_mgr.get_online_info(v[i]);
		if(!oi)
		{
			continue;
		}
		center_send_client(w, *oi, msg);
	}
}

static void send_member_kill_info(center_service_t *w, const vector<uint32> &v, uint32 team_id)
{
	for(size_t i = 0; i < v.size(); ++i)
	{
		const mysql::sp_load_all_user_smy *smy = w->aus_mgr.get_user_smy(v[i]);
		if(!smy)
		{
			return ;
		}	
		client::team_kill_statistics_rsp tks;
		tks.set_role_id(v[i]);
		tks.set_role_name(smy->role_name());
		tks.set_num(0);
		broadcast_team_msg(w, team_id, tks);
	}
}

void update_team_info(center_service_t *w, uint32 team_id)
{
	client::team_info *ti = w->tm_mgr.get_team_info(team_id);
	if(ti == NULL)
	{
		return ;
	}
	client::team_info_rsp rsp;
	rsp.set_team_id(ti->team_id());
	rsp.set_raid_id(ti->raid_id());
	for(int i = 0; i < ti->member_size(); ++i)
	{
		uint32 role_id = ti->member(i).role_id();
		const mysql::sp_load_all_user_smy *smy = w->aus_mgr.get_user_smy(role_id);
		if(!smy)
		{
			continue;
		}
		client::team_member *tm = rsp.add_tm();
		tm->set_role_id(role_id);
		tm->set_level(smy->level());
		tm->set_name(smy->role_name());
		tm->set_profession(smy->profession());
		tm->set_power(smy->fighting_power());
		tm->set_captain(ti->captain() == role_id);
		tm->set_ready(ti->member(i).ready());
		tm->set_vip_lv(smy->vip_level());
	}
	for(int i = 0; i < ti->member_size(); ++i)
	{
		const online_role *oi = w->oi_mgr.get_online_info(ti->member(i).role_id());
		if(!oi)
		{
			continue;
		}
		center_send_client(w, *oi, rsp);
	}
}

void *on_pb_center_create_team_req(client::create_team_req *req, void *ud, size_t *rsp_len)
{
	center_service_t *w = (center_service_t *)ud;
	uint32 team_id = w->tm_mgr.create_team(req->svr_head().role_id(), req->raid_id());
	if(team_id == 0)
	{
		return NULL;
	}
	update_team_info(w, team_id);
    amtsvr_billlog(w->ctx, "%s|%u|create team %d\n", req->GetTypeName().c_str(), req->svr_head().role_id(), team_id);
	return NULL;
}

void *on_pb_center_fire_member_req(client::fire_member_req *req, void *ud, size_t *rsp_len)
{
	center_service_t *w = (center_service_t *)ud;
	uint32 role_id = req->svr_head().role_id();
	uint32 team_id = w->tm_mgr.get_team_id(role_id);
	if(team_id == 0)
	{
		return NULL;
	}
	uint32 captain = w->tm_mgr.get_team_captain(team_id);
	if(captain != role_id && role_id != req->role_id())
	{
		return NULL;
	}
	if(!w->tm_mgr.is_same_team(role_id, req->role_id()))
	{
		return NULL;
	}
	// 小明离开队伍
	int ret = w->tm_mgr.leave_team(req->role_id());
	if(ret != 0)
	{
		return NULL;
	}
	// 向其他人广播小明离开
	update_team_info(w, team_id);
    amtsvr_billlog(w->ctx, "%s|%u|fired %u\n", req->GetTypeName().c_str(), role_id, req->role_id());
	return NULL;
}

void *on_pb_center_team_notify(client::team_notify *req, void *ud, size_t *rsp_len)
{
	center_service_t *w = (center_service_t *)ud;
	const online_role *oi = w->oi_mgr.get_online_info(req->role_id());
	if(oi == NULL)
	{
		return NULL;
	}
	client::fire_member_rsp rsp;
	rsp.set_type(req->type());
	center_send_client(w, *oi, rsp);
	return NULL;
}

void *on_pb_center_join_team_req(client::join_team_req *req, void *ud, size_t *rsp_len)
{
	center_service_t *w = (center_service_t *)ud;
	uint32 role_id = req->svr_head().role_id();
	client::team_info *ti = w->tm_mgr.get_team_info(req->team_id());
	client::join_team_rsp rsp;
	if(ti == NULL)
	{
		rsp.set_ret(client::team_not_exist);
		center_send_client(w, req->svr_head(), rsp);
		return NULL;
	}
	const config_raid* cr = CONFMGR->get_config_raid(ti->raid_id());
	if(cr == NULL)
	{
		rsp.set_ret(client::raid_error);
		center_send_client(w, req->svr_head(), rsp);
		return NULL;
	}
	uint32 n = w->tm_mgr.get_member_count(req->team_id());
	if(cr->get_max_member() <= n)
	{
		rsp.set_ret(client::team_full);
		center_send_client(w, req->svr_head(), rsp);
		return NULL;
	}
	if(ti->status() != client::team_info_status_e_normal)
	{
		rsp.set_ret(client::team_closed);
		center_send_client(w, req->svr_head(), rsp);
		return NULL;
	}
	client::team_join_check tjk;
	tjk.set_role_id(role_id);
	tjk.set_team_id(req->team_id());
	tjk.set_raid_id(ti->raid_id());
	center_send_scene(w, req->svr_head(), tjk);
    amtsvr_billlog(w->ctx, "%s|%u|join %u\n", req->GetTypeName().c_str(), role_id, req->team_id());
	return NULL;
}

void *on_pb_center_team_join_check(client::team_join_check *req, void *ud, size_t *rsp_len)
{
	center_service_t *w = (center_service_t *)ud;
	const online_role *oi = w->oi_mgr.get_online_info(req->role_id());
	if(oi == NULL)
	{
		return NULL;
	}
	client::join_team_rsp rsp;
	int ret = w->tm_mgr.join_member(req->team_id(), req->role_id());
	if(ret != 0)
	{
		rsp.set_ret((client::raid_check_result_e)ret);
		center_send_client(w, *oi, rsp);
		return NULL;
	}
	rsp.set_ret(0);
	rsp.set_raid_id(req->raid_id());
	center_send_client(w, *oi, rsp);
	update_team_info(w, req->team_id());
	return NULL;
}

void *on_pb_center_team_ready_req(client::team_ready_req *req, void *ud, size_t *rsp_len)
{
	center_service_t *w = (center_service_t *)ud;
	int ret = w->tm_mgr.change_ready_status(req->svr_head().role_id(), req->status());
	if(-1 == ret)
	{
		return NULL;
	}
	uint32 team_id = w->tm_mgr.get_team_id(req->svr_head().role_id());
	if(0 == team_id)
	{
		return NULL;
	}
	update_team_info(w, team_id);
    amtsvr_billlog(w->ctx, "%s|%u|%u\n", req->GetTypeName().c_str(), req->svr_head().role_id(), req->status());
	return NULL;
}

void *on_pb_center_team_enter_prepare_req(client::team_enter_prepare_req *req, void *ud, size_t *rsp_len)
{
	center_service_t *w = (center_service_t *)ud;
	uint32 team_id = w->tm_mgr.get_team_id(req->svr_head().role_id());
	client::team_enter_prepare_rsp rsp;
	if(team_id == 0)
	{
		rsp.set_ret(1);
		center_send_client(w, req->svr_head(), rsp);
		return NULL;
	}
	uint32 captain = w->tm_mgr.get_team_captain(team_id);
	if(captain != req->svr_head().role_id())
	{
		rsp.set_ret(1);
		center_send_client(w, req->svr_head(), rsp);
		return NULL;
	}
	if(!w->tm_mgr.is_all_ready(team_id))
	{
		rsp.set_ret(2);
		center_send_client(w, req->svr_head(), rsp);
		return NULL;
	}
	rsp.set_ret(0);
	vector<uint32> v;
	w->tm_mgr.get_team_members(team_id, v);
	for(size_t i = 0; i < v.size(); ++i)
	{
		const online_role *oi = w->oi_mgr.get_online_info(v[i]);
		if(!oi)
		{
			continue;
		}
		center_send_client(w, *oi, rsp);
	}
	w->tm_mgr.change_team_status(team_id, 2);
    amtsvr_billlog(w->ctx, "%s|%u\n", req->GetTypeName().c_str(), req->svr_head().role_id());
	return NULL;
}

void *on_pb_center_team_enter_raid_req(client::team_enter_raid_req *req, void *ud, size_t *rsp_len)
{
	center_service_t *w = (center_service_t *)ud;
	uint32 team_id = w->tm_mgr.get_team_id(req->svr_head().role_id());
	client::team_enter_raid_rsp rsp;
	if(w->tm_mgr.get_team_status(team_id) != (uint32)client::team_info_status_e_ready)
	{
		rsp.set_ret(4);
		broadcast_team_msg(w, team_id, rsp);
		w->tm_mgr.leave_team(req->svr_head().role_id(), false);
		return NULL;
	}
	uint32 captain = w->tm_mgr.get_team_captain(team_id);
	vector<uint32> v;
	w->tm_mgr.get_team_members(team_id, v);
	vector<uint32> m(v.begin(), v.end());
	uint32 raid_id = w->tm_mgr.get_team_raid(team_id);
	const config_raid* cr = CONFMGR->get_config_raid(raid_id);
	int need = cr->get_max_member() - v.size();
	if(need > 0)
	{
		client::team_add_robot tar;
		tar.set_role_id(captain);
		vector<uint32> r;
		int ret = match_robot(w, r, need, v, cr->get_enter_lv(), cr->get_need_power());
		if(ret != 0)
		{
			rsp.set_ret(3);
			broadcast_team_msg(w, team_id, rsp);
			w->tm_mgr.leave_team(req->svr_head().role_id(), false);
			return NULL;
		}
		for(size_t i = 0; i < r.size(); ++i)
		{
			tar.add_robot(r[i]);
			m.push_back(r[i]);
		}
		center_send_scene(w, req->svr_head(), tar);
	}
	rsp.set_ret(0);
	for(size_t i = 0; i < v.size(); ++i)
	{
		const online_role *oi = w->oi_mgr.get_online_info(v[i]);
		if(!oi)
		{
			continue;
		}
		client::team_raid_enter_cmd cmd;
		cmd.set_role_id(v[i]);
		cmd.set_team_id(team_id);
		cmd.set_raid_id(raid_id);
		center_send_scene(w, *oi, cmd);
		center_send_client(w, *oi, rsp);
	}
	if(raid_id == 70301 || raid_id == 70302)
	{
		send_member_kill_info(w, m, team_id);
	}
	w->tm_mgr.leave_team(captain, false);
    amtsvr_billlog(w->ctx, "%s|%u\n", req->GetTypeName().c_str(), req->svr_head().role_id());
	return NULL;
}
