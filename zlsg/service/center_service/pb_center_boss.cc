#include "center_interface.h"

void *on_pb_center_boss_info(client::boss_info_req *req, void *ud, size_t *rsp_len)
{
    center_service_t* w = (center_service_t*)ud;

	if(NULL == w)
	{
		return NULL;
	}
	client::boss_info_rsp rsp;
	rsp.set_timelong(w->bm_mgr.get_refresh_time() - (uint32)time(NULL));
	for(int i = 0; i < req->bs_size(); ++i)
	{
		const client::boss_status& bs = req->bs(i);
		uint32 status = w->bm_mgr.get_boss_status(bs.raid_id());
		client::boss_status *bss = rsp.add_bs();
		bss->set_raid_id(bs.raid_id());
		bss->set_status(status);
	}
    center_send_client(w, req->svr_head(), rsp);
    amtsvr_billlog(w->ctx, "%u|%s|%u", req->svr_head().role_id(), req->GetTypeName().c_str(), rsp.timelong());
	return NULL;
}

void *on_pb_center_refresh_msg(client::boss_refresh_msg_rsp *rsp, void *ud, size_t *rsp_len)
{
	center_service_t *w = (center_service_t *)ud;
	if(NULL == w)
	{
		return NULL;
	}

	const map<uint32, online_role>& all_online = w->oi_mgr.get_all_online_info();
	map<uint32, online_role>::const_iterator it = all_online.begin();

	for(; it != all_online.end(); ++it)
	{
		if(it->second.in_main_town())
		{
			center_send_client(w, it->second, *rsp);
		}
	}
	return NULL;
}

void *on_pb_center_boss_status(client::boss_status *bs, void *ud, size_t *rsp_len)
{
	center_service_t *w = (center_service_t *)ud;
	if(NULL == w)
	{
		return NULL;
	}
	w->bm_mgr.change_boss_status(bs->raid_id(), bs->status());
	return NULL;
}
