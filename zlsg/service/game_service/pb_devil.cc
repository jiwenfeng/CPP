#include "game_interface.h"
#include "player_role.h"
#include "player_role_devil.h"
#include "client.devil.pb.h"
#include "config_devil.h"
#include "config_npc.h"

// 领取完美礼包
void *on_pb_game_devil_award_req(client::devil_award_req *req, void *ud, size_t *rsp_len)
{
	PB_FUNC_GUARD(req);
	client::devil_award_rsp rsp;
	rsp.set_id(req->id());
	if(!psu->has_func_open(DEVIL_FUNC_ID))
	{
		rsp.set_ret(1);
		psu->send_client(rsp);
		return NULL;
	}
	if(!psu->is_can_add_props())
	{
		rsp.set_ret(1);
		psu->send_client(rsp);
		return NULL;
	}
	player_role_devil &prd = psu->get_player_role().get_role_devil();
	if(prd.is_stage_award(req->id()))
	{
		rsp.set_ret(1);
		psu->send_client(rsp);
		return NULL;
	}
	const vector<uint32> &v = CONFMGR->get_all_maps_by_stage(req->id());
	for(size_t i = 0; i < v.size(); ++i)
	{
		if(!prd.raid_is_perfect(v[i]))
		{
			rsp.set_ret(1);
			psu->send_client(rsp);
			return NULL;
		}
	}
	uint32 id = CONFMGR->get_stage_reward(req->id());
	if(id == 0)
	{
		rsp.set_ret(0);
		psu->send_client(rsp);
		return NULL;
	}
	psu->add_drop_props(id);
	prd.set_stage_award(req->id());
	rsp.set_ret(0);
	psu->send_client(rsp);
	return NULL;
}
