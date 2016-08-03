#include "game_interface.h"
#include "player_role.h"
#include "player_role_props.h"
#include "player_role_statis_mgr.h"
#include "scene_user.h"
#include "scene_inst.h"
#include "config_activity_item.h"
#include "config_props.h"
#include "config_base.h"
#include "player_role_mounts.h"
#include "config_npc.h"
#include "player_role_gem.h"
#include "player_role_bows.h"

void get_price(int id,ROLE_ATTRIBUTE& pay_type, bool& is_item)
{
	if (id == CONFMGR->get_basic_value(10245)){
		pay_type = TAEL;
	}else if (id == CONFMGR->get_basic_value(10246)){
		pay_type = INGOT;
	}else if (id == CONFMGR->get_basic_value(10247)){
		pay_type = MILITARY_STRATEGY;
	}else if (id == CONFMGR->get_basic_value(10248)){
		pay_type = PHYSICAL_POWER;
	}else if (id == CONFMGR->get_basic_value(10251)){
		pay_type = STRATEGY;
	}
	else{
		is_item = true;
	}
}
void* on_pb_activity_shop_c2g(client::activity_shop_c2g* req, void *ud, size_t* rsp_len)
{

	PB_FUNC_GUARD_VIA(req);

	client::activity_shop_rsp rsp_msg;
	client::activity_shop_g2c center_rsp;
	const config_activity_item* act_item_info = CONFMGR->get_config_activity_item_mgr().get_config_activity_item(req->activity_type());

	if (act_item_info == NULL){
		rsp_msg.set_result_type(4);
		psu->send_client(rsp_msg);

		center_rsp.set_activity_id(req->activity_id());
		center_rsp.set_activity_type(req->activity_type());
		center_rsp.set_item_count(req->item_count());
		center_rsp.set_role_id(req->role_id());
		center_rsp.set_is_sucess(false);
		game_send_center(ss,center_rsp);
		return NULL;
	}
	
	uint32 drop_id = act_item_info->get_get_item();
	if (drop_id == 0){
		rsp_msg.set_result_type(4);
		psu->send_client(rsp_msg);

		center_rsp.set_activity_id(req->activity_id());
		center_rsp.set_activity_type(req->activity_type());
		center_rsp.set_item_count(req->item_count());
		center_rsp.set_role_id(req->role_id());
		center_rsp.set_is_sucess(false);
		game_send_center(ss,center_rsp);

		return NULL;
	}

	const std::map<int, int>& price_map_ =  act_item_info->get_b_item_price2();
	std::map<int, int>::const_iterator it = price_map_.begin();
	for (;it != price_map_.end(); ++ it){
		ROLE_ATTRIBUTE pay_type;
		bool is_item = false;
		get_price(it->first,pay_type,is_item);

		if (is_item){
			if (psu->get_props_num(it->first) < static_cast<uint32>(it->second)){
				center_rsp.set_activity_id(req->activity_id());
				center_rsp.set_activity_type(req->activity_type());
				center_rsp.set_item_count(req->item_count());
				center_rsp.set_role_id(req->role_id());
				center_rsp.set_is_sucess(false);
				game_send_center(ss,center_rsp);
				return NULL;
			}
		}else{
			if (psu->get_attr(pay_type) < static_cast<uint32>(it->second)){
				center_rsp.set_activity_id(req->activity_id());
				center_rsp.set_activity_type(req->activity_type());
				center_rsp.set_item_count(req->item_count());
				center_rsp.set_role_id(req->role_id());
				center_rsp.set_is_sucess(false);
				game_send_center(ss,center_rsp);
				return NULL;
			}
		}
	}

	// 扣除东西
	std::map<int, int>::const_iterator itr = price_map_.begin();
	bool money_change = false;
	for (;itr != price_map_.end();++itr){
		ROLE_ATTRIBUTE pay_type;
		bool is_item = false;
		get_price(itr->first,pay_type,is_item);
		if (is_item){
			psu->sub_props(static_cast<uint32>(itr->first),static_cast<uint32>(itr->second));
		}else{
			uint32 old_money = 0;
			if (pay_type == INGOT){
				old_money = psu->get_attr(INGOT);
			}

			psu->sub_money(pay_type,static_cast<uint32>(itr->second));
			money_change = true;
			if (pay_type == INGOT){
				psu->sub_money_log(MC_ACTIVITY_SPEND, MC_BUY_ACTIVITY, old_money, psu->get_attr(INGOT));
			}
		}
	}


	vector<client::props_info> props_vec;
	props_vec.clear();
	psu->add_drop_props(drop_id,props_vec, req->close_time());
	psu->send_client_change_props_msg();
	if (money_change){
		psu->send_client_change_attr();
	}
	
	for (uint32 i = 0; i < props_vec.size(); ++i){
		client::props_info* info = center_rsp.add_pro_info();
		info->CopyFrom(props_vec[i]);
	}

	// 发给center做记录
	center_rsp.set_activity_id(req->activity_id());
	center_rsp.set_activity_type(req->activity_type());
	center_rsp.set_item_count(req->item_count());
	center_rsp.set_role_id(req->role_id());
	center_rsp.set_is_sucess(true);
	game_send_center(ss,center_rsp);

	return NULL;
}

void send_reward_erro(client::activity_reward_c2g* req,scene_user* psu,game_service_t* ss,uint32 result_type,client::activity_reward_rsp rsp_msg,client::activity_reward_g2c center_rsp)
{
	rsp_msg.set_activity_id(req->activity_id());
	rsp_msg.set_result_type(result_type);
	rsp_msg.set_activity_type(req->activity_type());
	psu->send_client(rsp_msg);

	center_rsp.set_activity_id(req->activity_id());
	center_rsp.set_activity_type(req->activity_type());
	center_rsp.set_role_id(req->role_id());
	center_rsp.set_is_success(false);
	game_send_center(ss,center_rsp);
}

void* on_pb_activity_reward_c2g(client::activity_reward_c2g* req, void *ud, size_t* rsp_len)
{
	PB_FUNC_GUARD_VIA(req);

	client::activity_reward_rsp rsp_msg;
	client::activity_reward_g2c center_rsp;
	const config_activity_item* act_item_info = CONFMGR->get_config_activity_item_mgr().get_config_activity_item(req->activity_type());
	if (act_item_info == NULL){
		send_reward_erro(req,psu,ss,3,rsp_msg,center_rsp);	
		return NULL;
	}

	// 判断领取条件
	const std::map<int, int>& limit_map_ = act_item_info->get_a_conditon();
	std::map<int, int>::const_iterator it = limit_map_.begin();
	for (;it != limit_map_.end(); ++it){
		if (it->first == REWARD_TYPE_MOUNT){
			// 坐骑阶数
			if (psu->get_player_role().get_mounts().get_db().mount_id() < static_cast<uint32>(it->second)){
				send_reward_erro(req,psu,ss,4,rsp_msg,center_rsp);
				return NULL;
			}
		}else if (it->first == REWARD_TYPE_POWER || it->first == REWARD_TYPE_GEM_LEVEL || it->first == REWARD_TYPE_LEVEL){
			return NULL;
		}else if (it->first == REWARD_TYPE_VIP_LEVEL){
			// vip等级
			if (psu->get_attr(ROLE_VIP_LEVEL) < static_cast<uint32>(it->second)){
				send_reward_erro(req,psu,ss,4,rsp_msg,center_rsp);
				return NULL;
			}
		}else if (it->first == REWARD_TYPE_BOW){
			// 弓箭阶数
			if (psu->get_player_role().get_player_role_bows().get_db().bow_id() < static_cast<uint32>(it->second)){
				send_reward_erro(req,psu,ss,4,rsp_msg,center_rsp);
				return NULL;
			}
		}
	}

	uint32 drop_id = act_item_info->get_get_item();
	vector<client::props_info> props_vec;
	props_vec.clear();
	psu->add_drop_props(drop_id,props_vec, req->close_time());
	psu->send_client_change_props_msg();
	for (uint32 i = 0; i < props_vec.size(); ++i){
		client::props_info* info = center_rsp.add_pro_info();
		info->CopyFrom(props_vec[i]);
	}

	// 通知center 更新领取标记
	
	center_rsp.set_activity_id(req->activity_id());
	center_rsp.set_activity_type(req->activity_type());
	center_rsp.set_role_id(req->role_id());
	center_rsp.set_is_success(true);
	game_send_center(ss,center_rsp);

	return NULL;
}

void* on_pb_activity_his_record_c2g(client::activity_his_record_c2g* req, void *ud, size_t* rsp_len)
{
	PB_FUNC_GUARD_VIA(req);

	client::activity_his_record_g2c rsp_msg;
	rsp_msg.set_activity_id(req->activity_id());
	rsp_msg.set_role_id(req->role_id());
	if (req->limit_type()  == REWARD_TYPE_POWER){
		// 更新有效有且只有一次
		// 若活动已经记录过历史最高战力了，更新到center 也不会覆盖
		uint32 his_power = psu->get_attr(EQUIP_POWER);
		if (his_power == 0){
			his_power = psu->cal_equip_reward_power();
			psu->mutable_attr()->set_val(EQUIP_POWER,his_power);
			psu->send_client_change_attr();
		}
		psu->update_equip_power_rank(REWARD_TYPE_POWER,psu->get_attr(EQUIP_POWER));
		rsp_msg.set_old_value(his_power);
		rsp_msg.set_limit_type(REWARD_TYPE_POWER);
		psu->send_center(rsp_msg);
		return NULL;
	}

	return NULL;
}