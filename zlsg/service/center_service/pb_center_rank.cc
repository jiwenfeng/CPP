#include "center_interface.h"
#include "config_activity_item.h"

void *on_pb_rank_add_data(client::rank_add_data *raa, void *ud, size_t *rsp_len)
{
	center_service_t *w = (center_service_t *)ud;
	if(!w)
	{
		return NULL;
	}
	w->rm_mgr.add_to_rank(raa->rt(), raa->ru());
	return NULL;
}

void *on_pb_act_rank_add_data(client::act_rank_add_data *raa, void *ud, size_t *rsp_len)
{
	center_service_t *w = (center_service_t *)ud;

	uint32 act_limit_type = 0;
	uint32 activity_id = 0;
	std::map<uint32,poll_activity*>& act_map_ = w->poa_mgr->get_poll_activity_list();
	std::map<uint32,poll_activity*>::iterator it = act_map_.begin();
	for (; it != act_map_.end(); ++it){
		if (it->second->is_activiting()){
			act_limit_type = CONFMGR->get_config_activity_item_mgr().get_act_limit_type(it->first);
			if (act_limit_type == raa->limit_value()){
				activity_id = it->first;
				break;
			}
		}
	}

	// 证明活动开启了~
	if (activity_id != 0){
		// 获取记录的旧的值，若没有则不设置排行榜，先设置活动记录的旧值
		activity_record* record_info = w->poa_mgr->get_record_mgr().get_act_record_info(activity_id);
		if (record_info != NULL){
			if (raa->rt() == RANK_EQUIP_POWER){
				uint32 old_value = record_info->get_his_data(raa->ru().role_id(),raa->limit_value());
				if (old_value == 0){
					// 设置old_value
					w->poa_mgr->get_record_mgr().add_his_record(activity_id,raa->ru().role_id(),raa->ru().first(),raa->limit_value());
					// 设置new_value
					w->poa_mgr->get_record_mgr().add_his_record(activity_id,raa->ru().role_id(),raa->ru().first(),raa->limit_value());
				}else{
					uint32 new_value = record_info->get_new_data(raa->ru().role_id(),raa->limit_value());
					if (raa->ru().first() > new_value){
						w->poa_mgr->get_record_mgr().add_his_record(activity_id,raa->ru().role_id(),raa->ru().first(),raa->limit_value());
					}

					if (raa->ru().first() > old_value){
						// 新的提升值
						uint32 rank_value = raa->ru().first() - old_value;
						// 旧的提升值
						uint32 old_rank = w->rm_mgr.get_act_rank_value(raa->rt(),raa->ru().role_id());
						if (old_rank == 0){
							client::rank_rule info;
							info.CopyFrom(raa->ru());
							info.set_first(rank_value);
							w->rm_mgr.add_to_act_rank(raa->rt(), info);
						}else{
							if (old_rank < rank_value){
								client::rank_rule info;
								info.CopyFrom(raa->ru());
								info.set_first(rank_value);
								w->rm_mgr.add_to_act_rank(raa->rt(), info);
							}
						}
					}
				}
			}else if(raa->rt() == RANK_GEMS_INTEGRAL){
				w->poa_mgr->get_record_mgr().add_his_record(activity_id,raa->ru().role_id(),raa->ru().first(),raa->limit_value());
				uint32 old_gem = w->rm_mgr.get_act_rank_value(raa->rt(),raa->ru().role_id());
				client::rank_rule info;
				info.CopyFrom(raa->ru());
				// info.first() 为积分增加值
				info.set_first(old_gem + info.first());
				w->rm_mgr.add_to_act_rank(raa->rt(), info);

			}
		}
	}

	return NULL;
}

void *on_pb_rank_list_req(client::rank_list_req *req, void *ud, size_t *rsp_len)
{
	center_service_t *w = (center_service_t *)ud;
	if(!w)
	{
		return NULL;
	}
	const vector<client::rank_rule> &rr = w->rm_mgr.get_rank_list(req->rt());
	client::rank_list_rsp rsp;
	rsp.set_rt(req->rt());
	uint32 rank_num = 0;
	for(size_t i = 0; i < rr.size(); ++i)
	{
		const mysql::sp_load_all_user_smy *smy = w->aus_mgr.get_user_smy(rr[i].role_id());
		if(smy == NULL)
		{
			continue;
		}

		if (req->rt() == RANK_EQUIP_POWER || req->rt() == RANK_GEMS_INTEGRAL){
			rank_num = i + 1;
			if (rank_num >= 100){
				break;
			}
		}

		client::rank_item *ri = rsp.add_ri();
		ri->set_role_id(rr[i].role_id());
		ri->set_value(rr[i].first());
		ri->set_role_name(smy->role_name());
		ri->set_vip_lv(smy->vip_level());
		ri->set_profession(smy->profession());
	}
	center_send_client(w, req->svr_head(), rsp);
	return NULL;
}

void *on_pb_center_open_server_time_req(client::open_server_time_req *req, void *ud, size_t *rsp_len)
{

	center_service_t *w = (center_service_t *)ud;

	client::open_server_time_rsp rsp_msg;
	uint32 now = (uint32)time(NULL);
	uint32 time_value = now - amtsvr_svr_start_time();

	rsp_msg.set_unixtime(time_value);

	center_send_client(w, req->svr_head(), rsp_msg);

	return NULL;
}