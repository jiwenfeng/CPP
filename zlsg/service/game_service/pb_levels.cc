#include "game_interface.h"
#include "client.levels.pb.h"
#include "player_role.h"
#include "player_role_levels.h"
#include "player_role_statis_mgr.h"
#include "player_role_raid_mgr.h"
#include "config_map.h"
#include "config_giant.h"

// 关卡信息
void *on_pb_levels_list_req(client::levels_list_req *req, void *ud, size_t *rsp_len)
{
	PB_FUNC_GUARD(req);
	if(!psu->has_func_open(LEVEL_FUNC_ID))
	{
		return NULL;
	}
	player_role &pr = psu->get_player_role();
	player_role_levels &prl = pr.get_player_role_levels();
	client::levels_list_rsp rsp;
	prl.fill_role_levels_list(rsp);
	psu->send_client(rsp);
	return NULL;
}

// 重置关卡
void *on_pb_levels_reset_req(client::levels_reset_req *req, void *ud, size_t *rsp_len)
{
	PB_FUNC_GUARD(req);
	if(!psu->has_func_open(LEVEL_FUNC_ID))
	{
		return NULL;
	}
	player_role &pr = psu->get_player_role();
	player_role_statis_mgr &mgr = pr.get_role_statis_mgr();
	client::levels_reset_rsp rsp;
	if(mgr.get_levels_reset_times() >= (uint32)CONFMGR->get_basic_value(10109))
	{
		rsp.set_ret(1);
		psu->send_client(rsp);
		return NULL;
	}
	rsp.set_ret(0);
	mgr.add_levels_reset_times();
	pr.reset_role_levels();
	psu->send_client(rsp);
	return NULL;
}
