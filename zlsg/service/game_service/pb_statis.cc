#include "player_role.h"
#include "player_role_statis_mgr.h"
#include "scene_user.h"
#include "scene_inst.h"
#include "game_interface.h"

void* on_pb_role_statis_req(client::role_statis_req* req, void* ud, size_t* rsp_len)
{
    PB_FUNC_GUARD(req);

    role_statis_rsp rsp;
    player_role_statis_mgr& statis_mgr = psu->get_player_role().get_role_statis_mgr();
    statis_mgr.fill(req, rsp);
    game_send_client(ss, req->svr_head().client_id(), rsp);

    return NULL;
}
// 签到列表请求 
void* on_pb_sign_list_req(client::sign_list_req* req, void* ud, size_t* rsp_len)
{
	PB_FUNC_GUARD(req);

	client::sign_list_rsp rsp_msg;

	psu->on_pb_sign_list_req(rsp_msg);

	psu->send_client(rsp_msg);

	return NULL;
}
// 签到
void* on_pb_sign_in_req(client::sign_in_req* req, void* ud, size_t* rsp_len)
{
	PB_FUNC_GUARD(req);

	client::sign_in_rsp rsp_msg;

	psu->on_pb_sign_in_req(req,rsp_msg);

	psu->send_client(rsp_msg);

	amtsvr_billlog(ss->ctx, "%u|%s|%u", psu->get_role_id(), req->GetTypeName().c_str(), req->sign_date());

	return NULL;
}
// 开服奖励领取
void* on_pb_land_reward_req(client::land_reward_req* req, void* ud, size_t* rsp_len)
{
	PB_FUNC_GUARD(req);

	client::land_reward_rsp rsp_msg;

	psu->on_pb_land_reward_req(req,rsp_msg);


	psu->send_client(rsp_msg);

	amtsvr_billlog(ss->ctx, "%u|%s|%u", psu->get_role_id(), req->GetTypeName().c_str(), req->land_day());

	return NULL;
}