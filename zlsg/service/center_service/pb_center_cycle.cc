#include "center_interface.h"
#include "client.cycle.pb.h"
#include "family_mgr.h"

void *on_pb_center_cycle_challenge_req(client::cycle_challenge_req *req, void *ud, size_t *rsp_len)
{
	center_service_t *w = (center_service_t *)ud;
	if(NULL == w)
	{
		return NULL;
	}
	if(req->has_helper() && req->helper() != 0)
	{
		uint32 f1 = w->cf_mgr->get_family_id(req->svr_head().role_id());
		uint32 f2 = w->cf_mgr->get_family_id(req->helper());
		if(f1 != f2)
		{
			return NULL;
		} 
		if(w->cc_mgr.get_left_helped_count(req->helper()) == 0)
		{
			return NULL;
		}
		if(!w->cc_mgr.level_passed(req->helper(), req->level()))
		{
			return NULL;
		}
	}
	center_send_scene(w, req->svr_head(), *req);
	return NULL;
}

void *on_pb_center_cycle_fetch_helpers(client::cycle_fetch_helpers *req, void *ud, size_t *rsp_len)
{
	center_service_t *w = (center_service_t *)ud;
	if(NULL == w)
	{
		return NULL;
	}
	uint32 f1 = w->cf_mgr->get_family_id(req->svr_head().role_id());
	vector<uint32> v1, v2;
	w->cf_mgr->get_member(f1, v1);
	for(size_t i = 0; i < v1.size(); ++i)
	{
		bool flag = true;
		for(int j = 0; j < req->except_size(); ++j)
		{
			if(req->except(j) == v1[i])
			{
				flag = false;
				break;
			}
		}
		if(flag)
		{
			v2.push_back(v1[i]);
		}
	}
	client::cycle_helpers_rsp rsp;
	rsp.set_time((uint32)time(NULL));
	for(size_t i = 0; i < v2.size(); ++i)
	{
		if(!w->cc_mgr.level_passed(v2[i], req->level()))
		{
			continue;
		}
		uint32 count = w->cc_mgr.get_left_helped_count(v2[i]);
		if(count == 0)
		{
			continue;
		}
		const mysql::sp_load_all_user_smy *smy = w->aus_mgr.get_user_smy(v2[i]);
		if(NULL == smy)
		{
			continue;
		}
		client::cycle_helper_info *chi = rsp.add_chi();
		chi->set_role_id(v2[i]);
		chi->set_role_name(smy->role_name());
		chi->set_fighting_power(smy->fighting_power());
		chi->set_count(count);
	}
	center_send_client(w, req->svr_head(), rsp);
	return NULL;
}

void *on_pb_center_cycle_add_helped_count(client::cycle_add_helped_count *req, void *ud, size_t *rsp)
{
	center_service_t *w = (center_service_t *)ud;
	if(NULL == w)
	{
		return NULL;
	}
	w->cc_mgr.add_helped_count(req->role_id(), 1);
	return NULL;
}

void *on_pb_center_cycle_info_rsp(client::cycle_info_rsp *rsp, void *ud, size_t *rsp_len)
{
	center_service_t *w = (center_service_t *)ud;
	if(NULL == w)
	{
		return NULL;
	}
	uint32 role_id = 0, tick = 0;
	w->cc_mgr.get_fastest_info(rsp->level(), role_id, tick);
	if(role_id != 0 && tick != 0)
	{
		const mysql::sp_load_all_user_smy *smy = w->aus_mgr.get_user_smy(role_id);
		if(NULL != smy)
		{
			rsp->set_role_name(smy->role_name());
			rsp->set_tick(tick);
		}
	}
	center_send_client_by_role_id(w, rsp->role_id(), *rsp);
	return NULL;
}

void *on_pb_center_cycle_update_record_req(client::cycle_update_record_req *req, void *ud, size_t *rsp_len)
{
	center_service_t *w = (center_service_t *)ud;
	if(NULL == w)
	{
		return NULL;
	}
	client::cycle_update_record_rsp rsp;
	rsp.set_role_id(req->role_id());
	rsp.set_level(req->level());
	rsp.set_tick(req->tick());
	rsp.set_raid_id(req->raid_id());
	rsp.mutable_success()->CopyFrom(req->success());
	if(req->update() == 1)
	{
		int ret = w->cc_mgr.add_passed_level(req->role_id(), req->level(), req->tick());
		rsp.set_update(ret);
	}
	else
	{
		rsp.set_update(req->update());
	}
	uint32 role_id = 0;
	uint32 record = 0;
	w->cc_mgr.get_fastest_info(req->level(), role_id, record);
	if(role_id != 0 && record != 0)
	{
		const mysql::sp_load_all_user_smy *smy = w->aus_mgr.get_user_smy(role_id);
		if(NULL != smy)
		{
			rsp.set_role_name(smy->role_name());
			rsp.set_record(record);
		}
	}
	center_send_scene(w, req->role_id(), rsp);
	return NULL;
}
