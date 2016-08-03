#include "player_role_statis_mgr.h"
#include "player_role.h"
#include "player_role_cycle.h"
#include "game_interface.h"
#include "client.cycle.pb.h"
#include "config_cycle.h"
#include "client.raid_operate.pb.h"
#include "cycle_scene_inst.h"

void *on_pb_cycle_challenge_req(client::cycle_challenge_req *req, void *ud, size_t *rsp_len)
{
	PB_FUNC_GUARD(req);
	if(!psu->has_func_open(CYCLE_FUNC_ID))
	{
		return NULL;
	}
	player_role_cycle &prc = psu->get_player_role().get_role_cycle();
	uint32 top = prc.get_top_level();
	if(top == 0)
	{
		top = 1;
	}
	const config_cycle *cc_cur = CONFMGR->get_config_cycle(top);
	if(cc_cur == NULL)
	{
		return NULL;
	}
	const config_cycle *cc_challenge = CONFMGR->get_config_cycle(req->level());
	if(cc_challenge == NULL)
	{
		return NULL;
	}
	if(cc_challenge->get_difficulty() == cc_cur->get_difficulty())
	{
		if(req->level() > top + 1)
		{
			return NULL;
		}
	}
	else
	{
		if(req->level() != top + 1)
		{
			return NULL;
		}
	}
	if(req->has_helper() && req->helper() != 0)
	{
		player_role_statis_mgr &mgr = psu->get_player_role().get_role_statis_mgr();
		if((uint32)CONFMGR->get_basic_value(10136) <= mgr.get_cycle_invite_count())
		{
			return NULL;
		}
		if(prc.is_helped_me(req->helper()))
		{
			return NULL;
		}
		psu->add_target_id(req->helper());
		mgr.add_cycle_invite_count(1);
		prc.add_helper(req->helper());
		client::cycle_add_helped_count cahc;
		cahc.set_role_id(req->helper());
		game_send_center(ss, cahc);
	}
	prc.set_cur_level(req->level());
	client::enter_raid_req err;
	err.set_raid_id(80101);
	err.set_t(client::enter_raid_req_type_normal);
	client::server_head *svr_head = err.mutable_svr_head();
	svr_head->CopyFrom(req->svr_head());
	game_send_scene(ss, ss->self, err);
	return NULL;
}

void *on_pb_cycle_helpers_req(client::cycle_helpers_req *req, void *ud, size_t *rsp_len)
{
	PB_FUNC_GUARD(req);
	if(!psu->has_func_open(CYCLE_FUNC_ID))
	{
		return NULL;
	}
	if(psu->get_family_id() == 0)
	{
		return NULL;
	}
	player_role_cycle &prc = psu->get_player_role().get_role_cycle();
	client::cycle_fetch_helpers cfh;
	client::server_head *svr_head = cfh.mutable_svr_head();
	svr_head->CopyFrom(req->svr_head());
	cfh.set_level(req->level());
	const std::map<uint32, uint32> &helpers = prc.get_all_helpers();
	std::map<uint32, uint32>::const_iterator i;
	cfh.add_except(psu->get_role_id());
	for(i = helpers.begin(); i != helpers.end(); ++i)
	{
		cfh.add_except(i->first);
	}
	game_send_center(ss, cfh);
	return NULL;
}

void *on_pb_cycle_mopup_req(client::cycle_mopup_req *req, void *ud, size_t *rsp_len)
{
	PB_FUNC_GUARD(req);
	if(!psu->has_func_open(CYCLE_FUNC_ID))
	{
		return NULL;
	}
	if(!psu->is_can_add_props())
	{
		return NULL;
	}
	player_role_cycle &prc = psu->get_player_role().get_role_cycle();
	uint32 top = prc.get_top_level();
	const config_cycle *cc = CONFMGR->get_config_cycle(top);
	client::cycle_mopup_rsp rsp;
	rsp.set_time((uint32)time(NULL));
	if(NULL == cc)
	{
		psu->send_client(rsp);
		return NULL;
	}
	uint32 start = CONFMGR->get_start_level(cc->get_difficulty());
	while(start <= top)
	{
		if(prc.get_level_reward_type(start) == 1)
		{
			client::cycle_mopup_result *cmr = rsp.add_cmr();
			cmr->set_level(start);
			client::raid_result_info *rri = cmr->mutable_rri();
			cycle_scene_inst::fill_reward(start, 1, rri, psu);
			prc.update_pass_level_time(start);
		}
		++start;
	}
	CHECK_TARGET_PROGRESS(psu, CYCLE_MOPUP);
	prc.set_cur_level(top);
	psu->send_client(rsp);
	return NULL;
}

void *on_pb_cycle_info_req(client::cycle_info_req *req, void *ud, size_t *rsp_len)
{
	PB_FUNC_GUARD(req);
	if(!psu->has_func_open(CYCLE_FUNC_ID))
	{
		return NULL;
	}
	player_role_cycle &prc = psu->get_player_role().get_role_cycle();
	client::cycle_info_rsp rsp;
	rsp.set_role_id(req->svr_head().role_id());
	uint32 level = req->level();
	if(level == 0)
	{
		level = prc.get_cur_level();
	}
	rsp.set_level(level);
	rsp.set_top(prc.get_top_level());
	rsp.set_record(prc.get_level_used_tick(level));
	rsp.set_award(prc.award_fastest_prize(level));
	rsp.set_type(prc.get_level_reward_type(level));

	player_role_statis_mgr &mgr = psu->get_player_role().get_role_statis_mgr();
	uint32 limit = (uint32)CONFMGR->get_basic_value(10136);
	uint32 used = mgr.get_cycle_invite_count();
	if(limit <= used)
	{
		rsp.set_count(0);
	}
	else
	{
		rsp.set_count(limit - used);
	}
	game_send_center(ss, rsp);
	return NULL;
}

void *on_pb_cycle_update_record_rsp(client::cycle_update_record_rsp *req, void *ud, size_t *rsp_len)
{
	PB_FUNC_GUARD_VIA(req);
	client::raid_result_msg rrm;
	rrm.set_star(0);
	rrm.set_raid_id(req->raid_id());
	rrm.set_tick(req->tick());
	rrm.mutable_success()->CopyFrom(req->success());
	if(req->has_role_name() && req->has_tick())
	{
		rrm.set_value1(req->role_name());
		rrm.set_value2(req->record());
	}
	player_role_cycle &prc = psu->get_player_role().get_role_cycle();
	uint32 reward_type = prc.get_level_reward_type(req->level());
	cycle_scene_inst::fill_reward(req->level(), reward_type, req->update(), &rrm, psu);
	psu->send_client(rrm);
	prc.update_role_level(psu->get_role_id(), req->level(), req->tick(), !req->update());
	return NULL;
}
