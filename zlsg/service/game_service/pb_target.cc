#include "game_interface.h"
#include "client.target.pb.h"
#include "player_role_mission.h"
#include "config_target.h"
#include "player_role.h"
#include "player_role_vip.h"
#include "scene_role_target.h"
#include "base_time.h"

// 日常任务

int fill_target_list(client::target_list_rsp &rsp, scene_user *psu, client::TARGET_TYPE_E type)
{
	const map<uint32, config_target *> &v = CONFMGR->get_target_list();
	map<uint32, config_target *>::const_iterator i;
	player_role_mission &prm = psu->get_player_role().get_player_role_target();
	uint32 now = (uint32)time(NULL);
	uint32 cur_line = -1;
	uint32 level = psu->get_level();
	uint32 mount_id = psu->get_attr().mounts_id();
	for(i = v.begin(); i != v.end(); ++i)
	{
		const config_target *t = i->second;
		if(t->get_config_target_type() != (uint32)type)
		{
			continue;
		}
		uint32 line = t->get_config_target_line();
		if(line == cur_line)
		{
			continue;
		}
		uint32 level_limit = t->get_config_target_level_limit();
		uint32 mount_limit = t->get_config_target_mount_limit();
		if(level_limit != 0 && level_limit > level)
		{
			continue;
		}
		if(mount_limit != 0 && mount_limit > mount_id)
		{
			continue;
		}
		uint32 hour = t->get_config_target_start_hour();
		uint32 last = t->get_config_target_last();
		if(last != 0 && !time_utils::is_in_time_range(now, 0, hour, 0, 0, last))
		{
			continue;
		}
		const client::target_info *info = prm.get_target_info(t->id);
		if(info && info->status() == client::STATUS_FINISH)
		{
			continue;
		}
		client::target_item *ti = rsp.add_ti();
		if(!info)
		{
			ti->set_id(i->first);
			ti->set_status(client::STATUS_DOING);
			ti->set_num(0);
		}
		else
		{
			ti->set_id(i->first);
			ti->set_status(info->status());
			ti->set_num(info->num());
		}
		if(t->get_config_target_subtype() == 26)
		{
			uint32 left = (uint32)max((int)psu->get_player_role().get_player_role_vip().get_month_timeout() - (int)now, 0);
			ti->set_extra(left);
		}
		cur_line = line;
	}
	return 0;
}

void *on_pb_target_list_req(client::target_list_req *req, void *ud, size_t *rsp_len)
{
	PB_FUNC_GUARD(req);
	client::target_list_rsp rsp;
	rsp.set_type(req->type());
	fill_target_list(rsp, psu, req->type());
	psu->send_client(rsp);
	return NULL;
}

void *on_pb_target_prize_req(client::target_prize_req *req, void *ud, size_t *rsp_len)
{
	PB_FUNC_GUARD(req);
	if (!psu->is_can_add_props()){
		return NULL;
	}
	
	player_role_mission &prm = psu->get_player_role().get_player_role_target();
	client::target_prize_rsp rsp;
	rsp.set_id(req->id());
	if(!prm.target_can_prize(req->id()))
	{
		rsp.set_status(client::target_prize_rsp_status_e_STATUS_LIMIT);
		psu->send_client(rsp);
		return NULL;
	}
	const config_target *ct = CONFMGR->get_config_target(req->id());
	if(!ct)
	{
		return NULL;
	}
	uint32 prev = ct->get_config_target_prev();
	if(prev != 0 && !prm.target_has_finished(prev))
	{
		rsp.set_status(client::target_prize_rsp_status_e_NEED_PREV);
		psu->send_client(rsp);
		return NULL;
	}
#if 0
	if(ct->prev != 0 && !prm.target_has_finished(ct->prev))
	{
		rsp.set_status(client::target_prize_rsp_status_e_NEED_PREV);
		psu->send_client(rsp);
		return NULL;
	}
	std::map<int, int>::const_iterator it;
	for(it = ct->items.begin(); it != ct->items.end(); ++it)
	{
		if(!psu->check_pack_space(it->first))
		{
			rsp.set_status(client::target_prize_rsp_status_e_PACK_LIMIT);
			psu->send_client(rsp);
			return NULL;
		}
	}
#endif
	string result;
	char tmp[1024] = {0};
	snprintf(tmp, 1024, "领取成就ID%u(%s)", req->id(), ct->get_descrition().c_str());
	result += tmp;
	std::map<int, int>::const_iterator it;
	const map<int, int> &v = ct->get_config_target_reward();
	for(it = v.begin(); it != v.end(); ++it)
	{
		uint32 old_val = psu->get_props_num(it->first);
		psu->add_props(it->first, it->second);
		uint32 new_val = psu->get_props_num(it->first);
		char tmp[1024] = {0};
		snprintf(tmp, 1024, " 获得道具%u(%s) 数量%u(前值%u/后值%u)", it->first, CONFMGR->get_prop_name(it->first).c_str(), it->second, old_val, new_val);
	}
	uint32 ingot = ct->get_config_target_ingot();
	if(ingot != 0)
	{
		char tmp[256] = {0};
		uint32 old_val = psu->get_attr().get_val(INGOT);
		psu->add_money(INGOT, ingot);
		uint32 new_val = psu->get_attr().get_val(INGOT);
		snprintf(tmp, 256, " 获得元宝 数量%u(前值%u/后值%u)", ingot, old_val, new_val);
		result += tmp;
	}
	uint32 copper = ct->get_config_target_copper();
	if(copper != 0)
	{
		char tmp[256] = {0};
		uint32 old_val = psu->get_attr().get_val(TAEL);
		psu->add_money(TAEL, ct->copper);
		uint32 new_val = psu->get_attr().get_val(TAEL);
		snprintf(tmp, 256, " 获得铜钱 数量%u(前值%u/后值%u)", copper, old_val, new_val);
		result += tmp;
	}
	uint32 power = ct->get_config_target_power();
	if(power != 0)
	{
		char tmp[256] = {0};
		uint32 old_val = psu->get_attr().get_val(PHYSICAL_POWER);
		psu->add_money(PHYSICAL_POWER, ct->power);
		uint32 new_val = psu->get_attr().get_val(PHYSICAL_POWER);
		snprintf(tmp, 256, " 获得体力 数量%u(前值%u/后值%u)", power, old_val, new_val);
		result += tmp;
	}
	uint32 exp = ct->get_config_target_exp();
	if(exp != 0)
	{
		char tmp[256] = {0};
		uint64 old_val = psu->get_experience();
		psu->add_normal_exp(ct->exp);
		uint64 new_val = psu->get_experience();
		snprintf(tmp, 256, " 获得经验 数量%u(前值%lu/后值%lu)", exp, old_val, new_val);
		result += tmp;
	}
	uint32 taolve = ct->get_config_target_taolve();
	if(taolve != 0)
	{
		char tmp[256] = {0};
		uint32 old_val = psu->get_attr().get_val(MILITARY_STRATEGY);
		psu->add_money(MILITARY_STRATEGY, ct->taolve);
		uint32 new_val = psu->get_attr().get_val(MILITARY_STRATEGY);
		snprintf(tmp, 256, " 获得韬略 数量%u(前值%u/后值%u)", taolve, old_val, new_val);
		result += tmp;
	}
	prm.set_target_status(req->id(), client::STATUS_FINISH);
	rsp.set_status(client::target_prize_rsp_status_e_SUCCESS);
	uint32 next = ct->get_config_target_next();
	if(next != 0)
	{
		const client::target_info *info = prm.get_target_info(next);
		client::target_item *ti = rsp.mutable_ti();
		ti->set_id(next);
		if(info)
		{
			ti->set_num(info->num());
			ti->set_status(info->status());
		}
		else
		{
			ti->set_num(0);
			ti->set_status(client::STATUS_DOING);
		}
	}
	psu->send_client_change_attr();
	psu->send_client_change_props_msg();
    psu->calculate_attr();
	psu->send_client(rsp);
	amtsvr_billlog(ss->ctx, "%u|%s|%s", psu->get_role_id(), req->GetTypeName().c_str(), result.c_str());
	return NULL;
}
