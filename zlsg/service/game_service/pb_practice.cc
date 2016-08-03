#include "config_potential.h"
#include "config_upgrade.h"
#include "client.practice.pb.h"
#include "client.role_potential.pb.h"
#include "game_interface.h"
#include "player_role.h"
#include "player_role_potential.h"
#include "player_role_skill_mgr.h"
#include "scene_user.h"
#include "scene_inst.h"
#include "config_mail.h"
string get_attr_name(int type);
void fill_comm_practice_info(scene_user* psu, client::comm_practice_info* cpi)
{
    if (psu != NULL && cpi != NULL) {
        cpi->set_level(psu->get_attr().level());
        psu->fill_role_name(*cpi->mutable_rn());
        psu->fill_client_pos(*cpi->mutable_pos());
    }
}

// 打坐请求
void* on_pb_practice_req(client::practice_req* req, void* ud, size_t* rsp_len)
{
    PB_FUNC_GUARD(req);

    scene_inst* psi = psu->get_scene();
    if (psi == NULL) {
        return NULL;
    }

    const scene_role_status& status = psu->get_status();
    if (status.meditate()) {
        psu->stop_practice();
        client::practice_rsp rsp;
        rsp.set_ret(client::PRE_STOP_SUCCESS);
        psu->send_client(ss->ctx, rsp);

        return NULL;
    }
    
    if (!psu->can_practice() || status.comm_practice()) {
        return NULL;
    }

    psu->practice();

    client::practice_rsp rsp;
    rsp.set_ret(client::PRE_SUCCESS);
    psu->send_client(ss->ctx, rsp);

    return NULL;
}

void* on_pb_comm_practice_req(client::comm_practice_req* req, void* ud, size_t* rsp_len)
{
    PB_FUNC_GUARD(req);

    scene_inst* psi = psu->get_scene();
    if (psi == NULL) {
        return NULL;
    }

    const scene_role_status& status = psu->get_status();
    scene_user* dst_su = NULL;
    if (status.comm_practice()) {
        psu->stop_practice();
        dst_su = psi->get_scene_user(psu->get_comm_practice_user());
        if (dst_su == NULL) {
            return NULL;
        }
        
        dst_su->stop_practice();
        if (req->dst_rd().inst_id() == dst_su->get_inst_id()) {
            return NULL;
        }
    }

    if (!psu->can_comm_practice()) {
        return NULL;
    }
    
    dst_su = psi->get_scene_user(req->dst_rd().inst_id());
    if (dst_su == NULL) {
        return NULL;
    }

    bool agree = false;
    if (!dst_su->can_comm_practice(&agree)) {
        return NULL;
    }

    if (!agree) {
        client::comm_practice_invite cpi;
        fill_comm_practice_info(psu, cpi.mutable_cpi());
        dst_su->send_client(ss->ctx, cpi);
        return NULL;
    }

    psu->comm_practice(req->dst_rd().inst_id());
    dst_su->comm_practice(psu->get_inst_id());

    return NULL;
}

void* on_pb_comm_practice_list_req(client::comm_practice_list_req* cplr, void* ud, size_t* rsp_len)
{
    struct game_service_t* ss = (struct game_service_t*)ud;
    scene_user* su = ss->sim->get_user(cplr->svr_head().role_id());
    if (su == NULL) {
        return NULL;
    }

    scene_inst* si = su->get_scene();
    if (si == NULL) {
        return NULL;
    }

    if (!su->can_get_comm_practice_list()) {
        return NULL;
    }

    client::comm_practice_list_rsp rsp;
    const shmap& user_map = si->get_user_map();
    const shobj* cur = user_map.get_all_objs_head();
    for (; cur != NULL; cur = cur->next[SLPE_LIST]) {
        scene_user* other = static_cast<scene_user*>(cur->sr);
        if (other == NULL || other->is_same_role(*su)) {
            continue;
        }
        
        if (other->can_comm_practice()) {
            fill_comm_practice_info(other, rsp.add_cpi());
        }
    }

    su->send_client(ss->ctx, rsp);

    return NULL;
}

void* on_pb_comm_practice_invite_result(client::comm_practice_invite_result* cpir, void* ud, size_t* rsp_len)
{
    struct game_service_t* ss = (struct game_service_t*)ud;
    scene_user* su = ss->sim->get_user(cpir->svr_head().role_id());
    if (su == NULL) {
        return NULL;
    }

    scene_inst* si = su->get_scene();
    if (si == NULL) {
        return NULL;
    }

    if (!cpir->agree() || !su->can_comm_practice()) {
        return NULL;
    }

    uint64 src_inst_id = cpir->cpi().rn().rd().inst_id();
    scene_user* src_su = si->get_scene_user(src_inst_id);
    if (src_su == NULL) {
        return NULL;
    }

    if (!src_su->can_comm_practice()) {
        return NULL;
    }
    
    su->comm_practice(src_inst_id);
    src_su->comm_practice(su->get_inst_id());


    return NULL;
}

void* on_pb_auto_agree_comm_practice(client::auto_agree_comm_practice* cacp, void* ud, size_t* rsp_len)
{
    struct game_service_t* ss = (struct game_service_t*)ud;
    scene_user* su = ss->sim->get_user(cacp->svr_head().role_id());
    if (su == NULL) {
        return NULL;
    }

    if (cacp->has_get() && cacp->get()) {
        client::auto_agree_comm_practice rsp;
        rsp.set_auto_agree(su->get_auto_agree_comm_practice());
        su->send_client(ss->ctx, rsp);
        return NULL;
    }
    
    su->set_auto_agree_comm_practice(cacp->auto_agree());

    return NULL;
}

void* on_game_pb_stop_comm_practice_req(client::stop_comm_practice_req* req, void* ud, size_t* rsp_len)
{
    struct game_service_t* ss = (struct game_service_t*)ud;
    scene_user* su = ss->sim->get_user(req->svr_head().role_id());
    if (su == NULL) {
        return NULL;
    }
    
    scene_inst* si = su->get_scene();
    if (si == NULL) {
        return NULL;
    }
    
    const scene_role_status& status = su->get_status();
    
    if (status.comm_practice()) {
        su->stop_practice();
        scene_user* dst_su = si->get_scene_user(su->get_comm_practice_user());
        if (dst_su == NULL) {
            return NULL;
        }

        dst_su->practice();
        return NULL;
    }

    return NULL;
}

void* on_game_pb_activate_potential_req(client::act_potential_req* req, void* ud, size_t* rsp_len)
{
	struct game_service_t* ss = (struct game_service_t*)ud;
	scene_user* su = ss->sim->get_user(req->svr_head().role_id());
	if (su == NULL) {
		return NULL;
	}

	scene_inst* si = su->get_scene();
	if (si == NULL) {
		return NULL;
	}
	player_role_potential& player_potential = su->get_player_role().get_potentials();
 	uint32 cur_level = su->mutable_attr()->get_val(LEVEL);
	uint32 cur_strategy = su->mutable_attr()->get_val(MILITARY_STRATEGY);
	uint32 cur_job = su->mutable_attr()->get_val(PROFESSION);
	// 获取当前编号的心法数据
	const config_potential* potential_info = CONFMGR->get_config_potential_mgr().get_config_potential(req->index());
	if (potential_info == NULL) {
		return NULL;
	}
	// 获得经验配置规则
	const config_upgrade* upgrade_info = CONFMGR->get_config_upgrade(cur_job,cur_level);
	if (upgrade_info == NULL) {
		return NULL;
	}
	// 检测前置道纹是否激活
	uint32 pre_index = potential_info->get_pre_potential();
	if (pre_index != 0) {
		client::acupoint_info pre_point_info;
		player_potential.get_acupoint_info(pre_index,pre_point_info);
		if (pre_point_info.status() == 0) {
			return NULL;
		}
	}
	// 检测人物等级、编号、韬略值
	if (cur_level < potential_info->get_need_level() || req->index() > 120 || cur_strategy < potential_info->get_need_potential()) {
		return NULL;
	}

	client::acupoint_info point_info;
	player_potential.get_acupoint_info(req->index(),point_info);
	// 进阶成功率计算
	bool is_activate = false;
	uint32 now = (uint32)time(NULL);
	uint32 open_day = amtsvr_svr_start_day(now);
	uint32 need_count_min = potential_info->get_need_count_min();
	uint32 need_count_max = potential_info->get_need_count_max();
	uint32 open_server_relief = potential_info->get_open_server_relief();
	float base_value = (float)(open_day / open_server_relief);
	// 修正最小所需次数 
	uint32 amendment = 0;
	if ((need_count_min - base_value) > 0) {
		amendment = static_cast<uint32>(need_count_min - base_value);
	}

	if ((amendment <= point_info.act_count()) &&(need_count_max > point_info.act_count() )) {
		uint32 rand_value = mtrandom::rand_mid32(0,10000);
		if (potential_info->get_need_probability() >= rand_value) {
			is_activate = true;
		}
	}else if (need_count_max <=  point_info.act_count()) {
		is_activate = true;
	}

	// 消费统计：
	string index_name = potential_info->get_number_name();
	uint32 new_mil_stra = cur_strategy - potential_info->get_need_potential();
	uint32 is_poten = 0;
	uint64 l_old_exp = su->mutable_attr()->get_val_64(EXPERIENCE);

	su->mutable_attr()->set_val(MILITARY_STRATEGY,new_mil_stra);
	su->send_client_change_attr();
	if (is_activate) {
		// 属性记录
		std::map<int,int> old_attr_map_;
		std::map<int,int> new_attr_map_;
		old_attr_map_.clear();
		new_attr_map_.clear();
		new_attr_map_ = potential_info->get_game_attribute();
		int l_old_value = 0;
		std::map<int,int>::iterator it_new = new_attr_map_.begin();
		// 填充属性前值;
		for (; it_new != new_attr_map_.end(); ++it_new){
			ROLE_ATTRIBUTE type = ATTR_CFG_TO_CODE[it_new->first];
			l_old_value = su->get_attr(type);
			old_attr_map_.insert(pair<int,int>(it_new->first,l_old_value));
		}
		uint32 act_skill_id = 0;
		std::vector<uint32> add_skill_vec;
		std::vector<uint32> del_skill_vec;
		add_skill_vec.clear();
		del_skill_vec.clear();
		// 检测是否有技能激活
		const std::vector<int>& reward_skill_vec = potential_info->get_reward_skill_list();
		if (reward_skill_vec.size() != 0) {
			for (size_t i = 0; i < reward_skill_vec.size(); ++i) {
				act_skill_id = reward_skill_vec[i];
				su->get_player_role().get_role_skill_mgr().add_skill(req->svr_head().role_id(),reward_skill_vec[i]);
				const config_skill* reward_skill_info = CONFMGR->get_config_skill(reward_skill_vec[i]);
				if (reward_skill_info != NULL) {
					skill sk(*reward_skill_info);
					su->mutable_skill()->add_skill(sk);
				}
			}
		}
		// 检测是否有技能删除
		const std::vector<int>& clear_skill_vec = potential_info->get_clear_skill_list();
		if (clear_skill_vec.size() != 0){
			for (size_t j = 0; j < clear_skill_vec.size(); ++j){
				su->get_player_role().get_role_skill_mgr().delete_skill(clear_skill_vec[j]);
				su->mutable_skill()->delete_skill(clear_skill_vec[j]);
			}
		}
		// 数据库处理
		point_info.set_index(req->index());
		point_info.set_status(1);
		player_potential.add_potential(point_info);
		su->add_normal_exp(upgrade_info->get_potential_exp_success());
		// 更新心法到排行榜
		su->update_potential_rank(req->index());
		// 统计心法

		// 下发穴位激活信息
		client::act_potential_rsp act_rsp;
		act_rsp.set_result_type(act_potential_rsp_result_success);
		client::client_point_info* ai = act_rsp.add_point_info();
		ai->set_index(point_info.index());
		ai->set_status(point_info.status());
		su->send_client(act_rsp);

		// 更新人物属性
		su->calculate_attr();
		su->send_client_change_attr();
		// 记录：填充属性新值
		std::map<int,int>::iterator it_l = new_attr_map_.begin();
		for (;it_l != new_attr_map_.end();++it_l){
			ROLE_ATTRIBUTE type = ATTR_CFG_TO_CODE[it_l->first];
			it_l->second = su->get_attr(type);
		}
		// 记录：
		std::string p = "";
		std::map<int ,int>::iterator it_o = old_attr_map_.begin();
		char tmp[1024] = {0};
		for (; it_o != old_attr_map_.end();++it_o){
			std::map<int,int>::iterator it_n = new_attr_map_.find(it_o->first);
			if (it_new != new_attr_map_.end()){
				memset(tmp, 0, sizeof(tmp));
				string name = get_attr_name(it_o->first);
				snprintf(tmp, sizeof(tmp), "|属性:%s,前值:%u/后值:%u",name.c_str(),it_o->second,it_n->second);
				p += tmp;
			}
		}
		// 记录分享的玩家经验
		std::map<string,client::record_exp> record_around_map_;
		record_around_map_.clear();
		// 经验冲击波。
		vector<scene_role *> vsr;
		si->get_around_role(su->get_scene_pos(), vsr, CONFMGR->get_basic_value(10143), true, false);
		uint32 around_player_level = 0;
		uint32 around_player_job = 0;
		// 特效广播范围：
		vector<scene_role *> efff_vsr;
		si->get_screen_role(su->get_scene_pos(), efff_vsr, true, false);
		client::effects_brocast_rsp eff_rsp;
		eff_rsp.set_effect_type(1);
		eff_rsp.set_role_id(su->get_role_id());
		for (size_t k = 0; k < efff_vsr.size(); ++k) {
			scene_user* eff_su = ss->sim->get_user(efff_vsr[k]->get_role_id());
			if (eff_su == NULL){
				continue;
			}

			eff_su->send_client(eff_rsp);
		}
		// 离线周期
		uint32 offline_interval = 0;
		uint32 potential_share_max = 0;
		client::potential_share_rsp share_rsp;
		share_rsp.set_name(su->get_role_name());
		share_rsp.set_role_id(su->get_inst_id());
		for (size_t j = 0; j < vsr.size(); ++j) {
			if (vsr[j]->get_role_id() == su->get_role_id()){  
				continue;
			}
			string around_name = vsr[j]->get_role_name();
			client::record_exp record_info;
			record_info.set_old_exp(0);
			record_info.set_new_exp(0);
			record_info.set_add_exp(0);
			around_player_level = vsr[j]->mutable_attr()->get_val(LEVEL);
			around_player_job = vsr[j]->mutable_attr()->get_val(PROFESSION);
			record_info.set_old_exp(static_cast<uint32>(vsr[j]->mutable_attr()->get_val_(EXPERIENCE)));
			const config_upgrade* upgrade_info = CONFMGR->get_config_upgrade(around_player_job,around_player_level);
			if (upgrade_info == NULL) {
				continue;
			}
			scene_user* arount_su = ss->sim->get_user(vsr[j]->get_role_id());
			if (arount_su == NULL) {
				continue;
			}

			float today = static_cast<float>(arount_su->get_player_role().get_online_time());
			float off_day = static_cast<float>(arount_su->get_player_role().get_offline_time());

			if (static_cast<uint32>(off_day) == 0){
				offline_interval = 1;
			}else{
				offline_interval = static_cast<uint32>(ceil((today - off_day) / 86400.0f));
			}
			
			// 设置总计可分享心法冲击波经验
			if (offline_interval >= 1) {
				uint32 share_exp = upgrade_info->get_potential_daily_exp_impact_max();
				share_exp = share_exp * offline_interval;
				arount_su->mutable_attr()->set_val(POTENTIAL_SHARE_MAX,share_exp);
				potential_share_max = share_exp;
			}
			else {
				arount_su->mutable_attr()->set_val(POTENTIAL_SHARE_MAX,upgrade_info->get_potential_daily_exp_impact_max());
				potential_share_max = upgrade_info->get_potential_daily_exp_impact_max();
			}

			uint32 total_potential_exp = arount_su->get_attr(POTENTIAL_ACCUMULATION);
			uint32 total_add = total_potential_exp + upgrade_info->get_potential_exp_impact(); 
			if (total_add < potential_share_max) {
				arount_su->mutable_attr()->set_val(POTENTIAL_ACCUMULATION,total_add);
				if (upgrade_info->get_potential_exp_impact() != 0){
					arount_su->add_exp(upgrade_info->get_potential_exp_impact());
					share_rsp.set_add_exp(upgrade_info->get_potential_exp_impact());
					record_info.set_add_exp(upgrade_info->get_potential_exp_impact());
					//玩家【XXX】心法激活成功，您因足够靠近而获得了经验加成：XXXXX send
					arount_su->send_client(share_rsp);
				}
				record_info.set_new_exp(static_cast<uint32>(arount_su->mutable_attr()->get_val_(EXPERIENCE)));
			}
			else {
				arount_su->mutable_attr()->set_val(POTENTIAL_ACCUMULATION,potential_share_max);
				uint32 real_add = potential_share_max - total_potential_exp;
				if (real_add != 0){
					arount_su->add_exp(real_add);
					share_rsp.set_add_exp(real_add);
					record_info.set_add_exp(real_add);
					//玩家【XXX】心法激活成功，您因足够靠近而获得了经验加成：XXXXX send
					arount_su->send_client(share_rsp);
				}

				record_info.set_new_exp(static_cast<uint32>(arount_su->mutable_attr()->get_val_(EXPERIENCE)));
			}

			record_around_map_.insert(pair<string,client::record_exp>(around_name,record_info));
		}

		if (potential_info->get_is_world_broadcast() == 1) {
			client::broadcast_msg_req req;
			req.set_area_id(44);
			std::vector<client::content_data> data_vec;
			data_vec.clear();
			client::content_data data_1;
			data_1.set_s(su->get_role_name());
			data_vec.push_back(data_1);

			client::content_data data_2;
			data_2.set_s(potential_info->get_number_name());
			data_vec.push_back(data_2);
			for (uint32 i = 0; i < data_vec.size(); ++i){
				client::content_data* data_info = req.add_data();
				data_info->CopyFrom(data_vec[i]);
			}

			su->send_center(ss->ctx, req);
		}

		client::role_skill_msg rsm;
		su->fill_role_skill_msg(rsm);
		su->send_client(rsm);
		std::map<string,client::record_exp>::iterator it_ar = record_around_map_.begin();
		string ar = "";
		char tar[1024] = {0};
		for (; it_ar != record_around_map_.end(); ++it_ar){
			memset(tar, 0, sizeof(tar));
			snprintf(tar, sizeof(tar), "|玩家:%s,获得经验:%u,前值:%u/后值:%u,",it_ar->first.c_str(),it_ar->second.add_exp(),it_ar->second.old_exp(),it_ar->second.new_exp());
			ar += tar;
		}
		//消耗韬略值XX|前值XX|后值XX|成功激活心法XX（心法名）|获得XX属性|前值XX|后值XX|激活心法技能XX；
		//获得经验XX|前值XX|后值XX|分享经验XX|影响玩家ID（玩家名）|前值XX|后值XX|玩家ID（玩家名）|前值XX|后值XX|玩家ID（玩家名）|前值XX|后值XX
		is_poten = 1;
		uint64 l_new_exp = su->mutable_attr()->get_val_(EXPERIENCE);
		amtsvr_billlog(ss->ctx, "%u|%s|%u|%u|%u|%u|%s%s|%u|%u|%llu|%llu%s", su->get_role_id(), req->GetTypeName().c_str(),
		is_poten,potential_info->get_need_potential(),cur_strategy,new_mil_stra,index_name.c_str(),p.c_str(),act_skill_id,upgrade_info->get_potential_exp_success(),
		l_old_exp,l_new_exp,ar.c_str());
	}else {
		// 数据库处理
		point_info.set_index(req->index());
		point_info.set_status(0);
		point_info.set_act_count(point_info.act_count()+1);
		player_potential.add_potential(point_info);

		su->add_exp(upgrade_info->get_potential_exp_fail());
		// 下发失败结果
		client::act_potential_rsp act_rsp;
		act_rsp.set_result_type(act_potential_rsp_result_failed);
		client::client_point_info* ai = act_rsp.add_point_info();
		ai->set_index(point_info.index());
		ai->set_status(point_info.status());
		su->send_client(ss->ctx,act_rsp);
		// 消耗韬略值XX|前值XX|后值XX|激活心法XX（心法名）失败|获得经验XX|前值XX|后值XX|
		uint64 l_new_exp = su->mutable_attr()->get_val_(EXPERIENCE);
		amtsvr_billlog(ss->ctx, "%u|%s|%u|%u|%u|%u|%s|%u|%llu|%llu", su->get_role_id(), req->GetTypeName().c_str(),
		is_poten,potential_info->get_need_potential(),cur_strategy,new_mil_stra,index_name.c_str(),upgrade_info->get_potential_exp_fail(),l_old_exp,l_new_exp);
	}

	CHECK_TARGET_PROGRESS(su, POTENTION_ACTIVE, point_info.index());

	return NULL;
}

void* on_game_pb_potential_info_list_req(client::potential_info_list* req, void* ud, size_t* rsp_len)
{
	struct game_service_t* ss = (struct game_service_t*)ud;
	scene_user* su = ss->sim->get_user(req->svr_head().role_id());
	if (su == NULL) {
		return NULL;
	}

	client::potential_info_list_rsp list_rsp;
	client::role_data* rd = list_rsp.mutable_rd();
	rd->set_role_id(su->get_player_role().get_db_player_role()->role_id());
	rd->set_inst_id(su->get_inst_id());
	rd->set_role_typ((client::role_data_TYPE)su->get_role_type());
	const std::map<uint32, client::acupoint_info>& potential_map_ = su->get_player_role().get_potentials().get_all_potential_info();
	std::map<uint32, client::acupoint_info>::const_iterator itr = potential_map_.begin();
	for (; itr != potential_map_.end(); ++itr){
		if (itr->second.status() != 1){
			continue;
		}
		client::client_point_info* point_info = list_rsp.add_point_info();
		point_info->set_index(itr->second.index());
		point_info->set_status(itr->second.status());
	}


	su->send_client(list_rsp);

	// 这里需要处理每次登陆只能请求一次
	return NULL;
}
