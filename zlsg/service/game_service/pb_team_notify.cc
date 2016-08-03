#include "config_raid.h"
#include "game_interface.h"
#include "scene_user.h"
#include "client.raid_operate.pb.h"
#include "tower_scene_inst.h"

void *on_pb_team_join_check(client::team_join_check *req, void *ud, size_t *rsp_len)
{
	PB_FUNC_GUARD_VIA(req);
	client::join_team_rsp rsp;
	if(psu->get_team_id() != 0)
	{
		rsp.set_ret(client::in_team);
		psu->send_client(rsp);
		return NULL;
	}
	if(psu->get_scene()->get_scene_type() != MAP_TYPE_NORMAL || ss->sim->get_space_user(psu->get_role_id()) != NULL)
	{
		rsp.set_ret(client::main_limit);
		psu->send_client(rsp);
		return NULL;
	}
	int ret = check_enter_raid(req->raid_id(), client::enter_raid_req_type_elite, psu);
	if(ret != 0)
	{
		rsp.set_ret(ret);
		psu->send_client(rsp);
		return NULL;
	}
	game_send_center(ss, *req);
	return NULL;
}

void *on_pb_create_team_req(client::create_team_req *req, void *ud, size_t *rsp_len)
{
	PB_FUNC_GUARD(req);
	client::create_team_rsp rsp;
	if(psu->get_team_id() != 0)
	{
		rsp.set_ret(client::in_team);
		psu->send_client(rsp);
		return NULL;
	}
	if(psu->get_scene()->get_scene_type() != MAP_TYPE_NORMAL ||
			ss->sim->get_space_user(psu->get_role_id()) != NULL)
	{
		rsp.set_ret(client::main_limit);
		psu->send_client(rsp);
		return NULL;
	}
	int ret = check_enter_raid(req->raid_id(), client::enter_raid_req_type_elite, psu);
	if(ret != 0)
	{
		rsp.set_ret(ret);
		psu->send_client(rsp);
		return NULL;
	}
	game_send_center(ss, *req);
	return NULL;
}

void *on_pb_team_add_robot(client::team_add_robot *req, void *ud, size_t *rsp_len)
{
	PB_FUNC_GUARD_VIA(req);
	for(int i = 0; i < req->robot_size(); ++i)
	{
		psu->add_target_id(req->robot(i));
	}
	return NULL;
}
