#include "game_interface.h"
#include "player_role.h"
#include "player_role_gem.h"
#include "player_role_skill_mgr.h"
#include "scene_user.h"


// 宝石列表请求
void* on_pb_gem_list_req(client::gem_list_req* req, void* ud, size_t* rsp_len)
{
	PB_FUNC_GUARD(req);

	client::gem_list_rsp rsp_msg;

	psu->on_pb_gem_list_req(req,rsp_msg);

	psu->send_client(rsp_msg);

	amtsvr_billlog(ss->ctx, "%u|%s|%u", psu->get_role_id(), req->GetTypeName().c_str(), req->time());

	return NULL;
}

void* on_pb_gem_level_up_req(client::gem_level_up_req* req, void* ud, size_t* rsp_len)
{
	PB_FUNC_GUARD(req);

	client::gem_level_up_rsp rsp_msg;

	psu->on_pb_gem_level_up_req(req,rsp_msg);

	psu->send_client(rsp_msg);

	amtsvr_billlog(ss->ctx, "%u|%s|%u", psu->get_role_id(), req->GetTypeName().c_str(), req->gem_pos());

	return NULL;
}