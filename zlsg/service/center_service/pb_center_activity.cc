#include "client.poll_activity.pb.h"
#include "config_mgr.h"
#include "offline_role_mgr.h"
#include "center_interface.h"
#include "config_activity_item.h"

void* on_center_pb_activity_info_req(client::activity_info_req* req, void* ud, size_t* rsp_len)
{
	center_service_t* w = (center_service_t*)ud;

	client::activity_info_rsp rsp_msg;

	w->poa_mgr->activity_info_req(req,rsp_msg);

	center_send_client(w, req->svr_head(), rsp_msg);

	amtsvr_billlog(w->ctx, "%u|%s|%u", req->svr_head().role_id(), req->GetTypeName().c_str(),req->activity_id());

	return NULL;
}

void* on_center_pb_activity_shop_req(client::activity_shop_req* req, void* ud, size_t* rsp_len)
{
	center_service_t* w = (center_service_t*)ud;

	client::activity_shop_c2g rsp_msg;

	int ret = w->poa_mgr->buy_activity_shop_req(req,rsp_msg);
	if (ret == 0){
		center_send_scene(w,req->svr_head().role_id(),rsp_msg);
	}
	
	amtsvr_billlog(w->ctx, "%u|%s|%u", req->svr_head().role_id(), req->GetTypeName().c_str(),req->activity_id());

	return NULL;
}
void send_erro_client(center_service_t* w,uint32 act_id,uint32 result_type,uint32 limit_type)
{
	client::activity_reward_rsp rsp;



}

void* on_center_pb_activity_reward_req(client::activity_reward_req* req, void* ud, size_t* rsp_len)
{
	center_service_t* w = (center_service_t*)ud;

	client::activity_reward_c2g rsp_msg;
	client::activity_reward_rsp reward_rsp;
	reward_rsp.set_activity_id(req->activity_id());
	reward_rsp.set_activity_type(req->activity_type());
	poll_activity* act_info = w->poa_mgr->get_activity_info(req->activity_id());

	if (act_info == NULL){
		return NULL;
	}

	if (!act_info->is_activiting()){
		reward_rsp.set_result_type(2);
		center_send_client(w, req->svr_head(), reward_rsp);
		return NULL;
	}

	const config_activity_item* act_item_info = CONFMGR->get_config_activity_item_mgr().get_config_activity_item(req->activity_type());
	if (act_item_info == NULL){
		reward_rsp.set_result_type(3);
		center_send_client(w, req->svr_head(), reward_rsp);
		return NULL;
	}

	if (act_item_info->get_item_order() > 32){
		return NULL;
	}

	// 判断领取条件：
	const std::map<int, int>& limit_map_ = act_item_info->get_a_conditon();
	std::map<int, int>::const_iterator it = limit_map_.begin();
	// 是否符合条件
	bool is_match = false;
	for (;it != limit_map_.end(); ++it){
		if (it->first == REWARD_TYPE_LEVEL){
			return NULL;
		}else if (it->first == REWARD_TYPE_KING){
			family* family_info = w->cf_mgr->get_family_by_role_id(req->svr_head().role_id());
			if (family_info == NULL){
				return NULL;
			}
			family_role_value* member_info = w->cf_mgr->get_member_info(family_info->get_family_id(),req->svr_head().role_id());
			if (member_info == NULL){
				return NULL;
			}

			if (it->second == 1){// 国王奖励
				if (w->ct_mgr->get_family_id() == family_info->get_family_id()
					&& w->cf_mgr->get_chief(family_info->get_family_id()) == req->svr_head().role_id()){
						is_match = true;
				}else{
					reward_rsp.set_result_type(7);
					center_send_client(w, req->svr_head(), reward_rsp);
					return NULL;
				}
			}else if (it->second == 2){// 王都军团奖励
				uint32 now_time = (uint32)time(NULL);
				// 加入军团时间不足48小时的成员不能领取王都军奖励；
				if (member_info->_time + 48*3600 > now_time){
					reward_rsp.set_result_type(6);
					center_send_client(w, req->svr_head(), reward_rsp);
					return NULL;
				}

				if (w->ct_mgr->get_family_id() == family_info->get_family_id()){
					is_match = true;
				}
			}
		}else if (it->first == REWARD_TYPE_GEM_LEVEL){
			activity_record* record_info = w->poa_mgr->get_record_mgr().get_act_record_info(req->activity_id());
			if (record_info == NULL){
				return NULL;
			}

			uint32 old_value = record_info->get_his_data(req->svr_head().role_id(),it->first);
			uint32 new_value = record_info->get_new_data(req->svr_head().role_id(),it->first);
			
			if (new_value - old_value >= static_cast<uint32>(it->second)){
				is_match = true;
			}

		}else if (it->first == REWARD_TYPE_POWER || it->first == REWARD_TYPE_VIP_LEVEL 
			   || it->first == REWARD_TYPE_MOUNT || it->first == REWARD_TYPE_BOW 
			   || it->first == REWARD_TYPE_DOUBLE_INGOT){
			is_match = true;
		}else{
			reward_rsp.set_result_type(5);
			center_send_client(w, req->svr_head(), reward_rsp);
			return NULL;
		}
	}

	if (is_match){
		int ret = w->poa_mgr->activity_reward_req(req,rsp_msg);
		if (ret == 0){
			center_send_scene(w, req->svr_head().role_id(), rsp_msg);
		}
	}

	amtsvr_billlog(w->ctx, "%u|%s|%u", req->svr_head().role_id(), req->GetTypeName().c_str(),req->activity_id());

	return NULL;
}

void* on_center_pb_activity_shop_g2c(client::activity_shop_g2c* req, void* ud, size_t* rsp_len)
{
	center_service_t* w = (center_service_t*)ud;

	client::activity_shop_rsp rsp_msg;

	w->poa_mgr->activity_record_opt(req,rsp_msg);

	center_send_client_by_role_id(w,req->role_id(),rsp_msg);

	return NULL;
}

void* on_center_pb_activity_reward_g2c(client::activity_reward_g2c* req, void* ud, size_t* rsp_len)
{
	center_service_t* w = (center_service_t*)ud;

	client::activity_reward_rsp rsp_msg;

	int ret = w->poa_mgr->activity_record_reward_opt(req,rsp_msg);

	if (ret == 0){
		center_send_client_by_role_id(w,req->role_id(),rsp_msg);
	}
	

	return NULL;
}


void* on_center_pb_load_activity_list_center(client::load_activity_list_center* req, void* ud, size_t* rsp_len)
{
	center_service_t* w = (center_service_t*)ud;

	client::activity_list_rsp rsp_msg;

	std::map<uint32,uint32> his_map_;//key: act_id ,value : limit_type
	his_map_.clear();
	w->poa_mgr->activity_status_list(req,rsp_msg,his_map_);

	center_send_client_by_role_id(w,req->role_id(),rsp_msg);

	std::map<uint32,uint32>::iterator itr = his_map_.begin();

	for (;itr != his_map_.end(); ++itr){
		if (itr->second == REWARD_TYPE_GEM_LEVEL){
			// 宝石记录数据初始化
			w->poa_mgr->get_record_mgr().create_limit_record(itr->first,req->role_id(),itr->second);
		}else if(itr->second == REWARD_TYPE_POWER){
			client::activity_his_record_c2g game_rsp;
			game_rsp.set_activity_id(itr->first);
			game_rsp.set_limit_type(itr->second);
			game_rsp.set_role_id(req->role_id());
			center_send_scene(w,req->role_id(),game_rsp);
		}
	}

	return NULL;
}

void* on_center_pb_activity_his_record_g2c(client::activity_his_record_g2c* req, void* ud, size_t* rsp_len)
{
	center_service_t* w = (center_service_t*)ud;

	
	w->poa_mgr->activity_record_his_info(req);


	return NULL;
}

void* on_center_pb_update_activity_record_g2c(client::update_activity_record_g2c* req, void* ud, size_t* rsp_len)
{
	center_service_t* w = (center_service_t*)ud;

	w->poa_mgr->update_his_record(req);

	return NULL;
}