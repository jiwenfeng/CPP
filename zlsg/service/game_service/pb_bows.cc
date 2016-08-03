#include "game_interface.h"
#include "player_role.h"
#include "player_role_bows.h"
#include "player_role_skill_mgr.h"
#include "scene_user.h"
#include "client.role_bows.pb.h"
#include "config_bows.h"
#include "player_role_props.h"

void fill_bows_info(const mysql::tb_role_bows& bows, client::bows_info& pmi)
{
	pmi.set_bow_id(bows.bow_id());
	pmi.set_advance_time(bows.advance_time());
	pmi.set_advance_bless(bows.advance_bless());
	pmi.set_bow_level(bows.bow_level());
	pmi.set_bow_exp(bows.bow_exp());
	pmi.set_bow_power(bows.bow_power());
}
void cal_expend(uint32 id,uint32 count,std::map<uint32,uint32>& total_expend);
// 弓箭进阶
void* on_game_pb_upgrade_bows_req(client::upgrade_bows_req* req, void* ud, size_t* rsp_len)
{
	PB_FUNC_GUARD(req);

	player_role_bows& player_bows = psu->get_player_role().get_player_role_bows();
	mysql::tb_role_bows* trb = player_bows.mutable_db();
	if (trb == NULL || trb->bow_id() == 0){
		return NULL;
	}

	// 根据ID获取当前弓箭基础数据与属性加成数据模版表
	const config_bows* pcm = CONFMGR->get_config_bows(trb->bow_id());
	if (pcm == NULL || pcm->get_base() == NULL) {
		return NULL;
	}
	// 获得下一阶的弓箭基础数据与属性加成数据模版表
	const config_bows* next_bows = CONFMGR->get_config_bows(pcm->get_base()->get_next_bow_id());
	if (next_bows == NULL) {
		return NULL;
	}

	props_parser& pack_obj =  psu->get_player_role().get_role_props().get_pack();
	bool is_enough_item = false;
	uint32 need_count = static_cast<uint32>(next_bows->get_base()->get_advance_need_item_num());
	const std::vector<int>& need_item_vec = next_bows->get_base()->get_advance_need_item();
	uint32 cur_total_count = 0;
	for (uint32 i = 0; i < need_item_vec.size(); ++i){
		cur_total_count += pack_obj.get_props_num(need_item_vec[i]);

	}

	// 消耗记录：
	std::map<uint32 ,uint32> expend_map_;
	expend_map_.clear();

	if (cur_total_count < need_count){
		// 检测替代道具是否足够
		const std::vector<int>& replace_item_vec = next_bows->get_base()->get_upgrade_replace_item();
		uint32 replace_count = 0;
		for (uint32 i = 0; i < replace_item_vec.size(); ++i){
			replace_count = pack_obj.get_props_num(static_cast<uint32>(replace_item_vec[i]));
			if (replace_count>=  need_count){
				// 扣除道具
				if (pack_obj.remove_props(static_cast<uint32>(replace_item_vec[i]),need_count) == 0){
					is_enough_item = true;
					break;
				}
			}else{
				pack_obj.remove_props(static_cast<uint32>(replace_item_vec[i]),replace_count);
				need_count -= replace_count;
			}
		}
	}
	else{
		// 扣除道具
		is_enough_item = true;

		psu->sub_super_props(need_item_vec,need_count,expend_map_);
	}

	if (!is_enough_item){
		return NULL;
	}

	psu->send_client_change_props_msg();
	// 先增加次数
	trb->set_advance_num(trb->advance_num() + 1);

	bool is_advance = false;
	uint32 advance_bless_max = static_cast<uint32>(next_bows->get_base()->get_advance_bless_max());
	uint32 advance_count_min = static_cast<uint32>(next_bows->get_base()->get_advance_count_min());
	uint32 advance_count_max = static_cast<uint32>(next_bows->get_base()->get_advance_count_max());
	// 进阶成功几率计算
	if (trb->advance_bless() >= advance_bless_max){
		is_advance = true;
	}
	else{
		if (trb->advance_num() < advance_count_min){
			is_advance = false;
		}
		else if (trb->advance_num() >= advance_count_max){
			is_advance = true;
		}
		else if(advance_count_min <= trb->advance_num() && trb->advance_num() < advance_count_max){
			int rand_value = static_cast<int>(mtrandom::rand_mid32(0,10000));
			if (next_bows->get_base()->get_advance_ratio() >= rand_value){
				is_advance = true;
			}
		}
	}

	if (is_advance){
		// 进阶成功
		// 数据保存
		trb->set_bow_id(pcm->get_base()->get_next_bow_id());
		trb->set_advance_num(0);
		trb->set_advance_time(0);
		trb->set_advance_bless(0);
		trb->set_bow_power(trb->bow_level());
		trb->set_bow_exp(trb->bow_exp());
		uint32 bow_power = static_cast<uint32>(ceil(GET_CONFIG_MGR(config_bows_mgr)->calculate_bow_power(trb->bow_id(),trb->bow_level())));
		trb->set_bow_power(bow_power);
		// 更新到排行榜
		psu->update_bows_rank(trb->bow_id(), trb->bow_level());
		// 下发给客户端
		client::upgrade_bows_rsp rsp;
		rsp.set_ret(0);// 0成功 1失败
		client::bows_info* info = rsp.add_mi();
		fill_bows_info(*trb,*info);
		psu->send_client(rsp);

		if (psu->get_attr().get_val(BOW_ID) != 0){
			psu->mutable_attr()->set_val(BOW_ID, pcm->get_base()->get_next_bow_id());
			psu->get_player_role().get_query_attr().set_mounts_id(pcm->get_base()->get_next_bow_id());
			psu->calculate_attr();
			psu->mutable_attr()->set_val(BOW_ID, pcm->get_base()->get_next_bow_id());
			psu->send_client_change_attr();
		}

		// 处理人物蓄射速度属性
		uint32 cur_speed = psu->calculate_role_bow_speed(trb->bow_id(),trb->bow_level());

		psu->mutable_attr()->set_val(BOW_SPEED,cur_speed);
		psu->get_player_role().get_query_attr().set_bow_speed(cur_speed);
		psu->calculate_attr();
		psu->mutable_attr()->set_val(BOW_SPEED,cur_speed);
		psu->send_client_change_attr();
		// 全服公告 0不公告 1公告
		if (next_bows->get_base()->get_success_broadcast() == 1){

			client::broadcast_msg_req req;
			req.set_area_id(48);
			std::vector<client::content_data> data_vec;
			data_vec.clear();
			client::content_data data_1;
			data_1.set_s(psu->get_role_name());
			data_vec.push_back(data_1);

			client::content_data data_2;
			data_2.set_n(trb->bow_id());
			data_vec.push_back(data_2);

			client::content_data data_3;
			data_3.set_s(next_bows->get_base()->get_bow_name());
			data_vec.push_back(data_3);

			for (uint32 i = 0; i < data_vec.size(); ++i){
				client::content_data* data_info = req.add_data();
				data_info->CopyFrom(data_vec[i]);
			}

			psu->send_center(ss->ctx, req);
		}
		CHECK_TARGET_PROGRESS(psu, BOW_LEVEL, pcm->get_base()->get_next_bow_id());
	}
	else{
		// 进阶失败
		// 若没有祝福值，则为第一次增加祝福值，插入时间
		if (trb->advance_bless() == 0){
			trb->set_advance_time((uint32)time(NULL));
		}
		uint32 max_bless = static_cast<uint32>(next_bows->get_base()->get_advance_fail_belss_max());
		uint32 min_bless = static_cast<uint32>(next_bows->get_base()->get_advance_fail_bless_min());
		uint32 rand_bless = mtrandom::rand_mid32(min_bless,max_bless);
		uint32 total_bless = trb->advance_bless() + rand_bless;
		if (total_bless >= static_cast<uint32>(next_bows->get_base()->get_advance_bless_max())){
			total_bless = static_cast<uint32>(next_bows->get_base()->get_advance_bless_max());
		}
		//数据库保存：
		trb->set_advance_bless(total_bless);

		// 下发给客户端
		client::upgrade_bows_rsp rsp;
		rsp.set_ret(1);// 0成功 1失败
		client::bows_info* info = rsp.add_mi();
		fill_bows_info(*trb,*info);
		psu->send_client(rsp);
	}

	amtsvr_billlog(ss->ctx, "%u|%s|%u", psu->get_role_id(), req->GetTypeName().c_str(), req->bow_id());

	return NULL;
}

void* on_game_pb_bows_bless_time_req(client::bows_bless_time_req* req, void* ud, size_t* rsp_len)
{
	PB_FUNC_GUARD(req);

	client::bows_bless_time_rsp rsp_msg;
	player_role_bows& m = psu->get_player_role().get_player_role_bows();
	mysql::tb_role_bows* pdb = m.mutable_db();
	if (pdb != NULL && pdb->bow_id() != 0) {
		if (pdb->advance_bless() != 0){
			const config_bows* bow_info = CONFMGR->get_config_bows(pdb->bow_id());
			if (bow_info != NULL){
				uint32 clear_time = bow_info->get_base()->get_advance_bless_time();
				if (clear_time != 0){
					uint32 valid_time = pdb->advance_time() + clear_time;
					uint32 system_time = (uint32)time(NULL);
					if (valid_time >= system_time){
						uint32 remain_time = valid_time - system_time;
						rsp_msg.set_remain_time(remain_time);
						psu->send_client(rsp_msg);
					}
				}
			}
		}
	}

	amtsvr_billlog(ss->ctx, "%u|%s|%u", psu->get_role_id(), req->GetTypeName().c_str(), req->unixtime());

	return NULL;
}
