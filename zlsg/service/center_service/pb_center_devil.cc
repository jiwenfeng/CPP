#include "center_interface.h"
#include "mysql.tb_center_devil.pb.h"
#include "client.devil.pb.h"

void *on_pb_center_devil_info_req(client::devil_info_req *req, void *ud, size_t *rsp_len)
{
	center_service_t *w = (center_service_t *)ud;
	if(NULL == w)
	{
		return NULL;
	}
	client::devil_info_rsp rsp;
	rsp.set_id(req->id());
	const mysql::tb_center_devil *tcd = w->dm_mgr.get_pass_info(req->id());
	if(NULL == tcd)
	{
		center_send_client(w, req->svr_head(), rsp);
		return NULL;
	}
	const mysql::sp_load_all_user_smy *smy = w->aus_mgr.get_user_smy(tcd->role_id());
	if(NULL == smy)
	{
		center_send_client(w, req->svr_head(), rsp);
		return NULL;
	}
	rsp.set_name(smy->role_name());
	rsp.set_tick(tcd->time());
	center_send_client(w, req->svr_head(), rsp);
	return NULL;
}

void *on_pb_center_devil_fight_over(client::devil_fight_over *req, void *ud, size_t *rsp_len)
{
	center_service_t *w = (center_service_t *)ud;
	if(NULL == w)
	{
		return NULL;
	}
	const online_role *oi = w->oi_mgr.get_online_info(req->role_id());
	if(NULL == oi)
	{
		return NULL;
	}
	client::devil_fight_result rsp;
	rsp.set_role_id(req->role_id());
	rsp.set_raid_id(req->raid_id());
	rsp.set_used_tick(req->tick());
	rsp.set_star(req->star());
	if(req->has_success())
	{
		client::raid_success_info *success = rsp.mutable_success();
		success->CopyFrom(req->success());
	}
	if(req->has_failure())
	{
		client::raid_failure_info *failure = rsp.mutable_failure();
		failure->CopyFrom(req->failure());
	}
	if(req->champion())
	{
		rsp.set_champion(0);
	}
	else
	{
		int ret = w->dm_mgr.add_pass_level(req->raid_id(), req->role_id(), req->tick());
		rsp.set_champion(ret);
	}
	const mysql::tb_center_devil *tcd = w->dm_mgr.get_pass_info(req->raid_id());
	if(NULL == tcd)
	{
		center_send_scene(w, *oi, rsp);
		return NULL;
	}
	const mysql::sp_load_all_user_smy *smy = w->aus_mgr.get_user_smy(tcd->role_id());
	if(NULL == smy)
	{
		center_send_scene(w, *oi, rsp);
		return NULL;
	}
	rsp.set_role_name(smy->role_name());
	rsp.set_tick(tcd->time());
	center_send_scene(w, *oi, rsp);
	return NULL;
}
