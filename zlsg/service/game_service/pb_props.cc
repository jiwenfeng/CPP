#include "config_upequip.h"
#include "config_cast_equip.h"
#include "config_treasure.h"
#include "game_interface.h"
#include "player_role.h"
#include "player_role_props.h"
#include "player_role_treasure.h"
#include "scene_user.h"
#include "scene_inst.h"
#include "effect_result.h"
#include "props_parser.h"
#include "config_props.h"
#include "config_melting_equip.h"
#include "scene_lose_mgr.h"
#include "config_equip_upgrade.h"
#include "config_bipize.h"
void strengthen_expend_calculate(uint32 props_id, uint32 old_level,uint32 new_level,uint32& expend_money,std::map<uint32,uint32>& expend_map_,uint32& opt_type);
uint32 get_amplification_by_type(uint32 config_type)
{
	switch(config_type)
	{
	case 1:// 生命
		return CONFMGR->get_basic_value(10168);
		break;
	case 3: // 攻击上限
		return CONFMGR->get_basic_value(10169);
		break;
	case 5: // 防御上限
		return CONFMGR->get_basic_value(10170);
		break;
	case 6: // 命中
		return CONFMGR->get_basic_value(10171);
		break;
	case 7:// 闪避
		return CONFMGR->get_basic_value(10172);
		break;
	case 9: // 回复
		return CONFMGR->get_basic_value(10173);
		break;
	case 12: // 稳定
		return CONFMGR->get_basic_value(10174);
		break;
	default:
		return 0;
		break;
	}

	return 0;
}

string get_attr_name(int type)
{
	string name = "";
	switch(type)
	{
	case 1:
		name = "生命";
		break;
	case 2:
		name = "攻击下限";
		break;
	case 3:
		name = "攻击";
		break;
	case 4:
		name = "防御下限";
		break;
	case 5:
		name = "防御";
		break;
	case 6:
		name = "命中";
		break;
	case 7:
		name = "闪避";
		break;
	case 8:
		name = "攻击速度";
		break;
	case 9:
		name = "恢复";
		break;
	case 10:
		name = "移动速度";
		break;
	case 11:
		name = "魔法";
		break;
	case 12:
		name = "稳定";
		break;
	case 13:
		name = "当前生命";
		break;
	case 14:
		name = "当前魔法";
		break;
	default:
		break;
	}


	return name;
}

bool is_max(grid_role_props* grp,const config_props* item_base_info,bool check_last = true)
{
	const std::map<int, cpf_val_3>& rand_attr_map_ = item_base_info->get_random_attribute();
	uint32 last_size = 0;
	if (check_last){
		last_size = grp->last_attr_size();
	}else{
		last_size = grp->attr_size();
	}
	// 检测是否满
	uint32 max_count = 0;
	for (uint32 k = 0; k < last_size; ++k){
		client::attribute* cur_last;
		if (check_last){
			cur_last = grp->mutable_last_attr(k);
		}else {
			cur_last = grp->mutable_attr(k);
		}

		if (cur_last == NULL){
			continue;
		}

		std::map<int, cpf_val_3>::const_iterator it = rand_attr_map_.find(cur_last->type());
		if (cur_last->value() == static_cast<uint32>(it->second._max)){
			max_count = max_count +1;
		}
	}

	if (last_size == max_count){
		return true;
	}

	return false;
}
uint32 check_same(grid_role_props* grp,uint32 match_size)
{
	uint32 is_same = 0;

	for (uint32 j = 0; j < match_size; ++j){
		client::attribute* cur_attr = grp->mutable_attr(j);
		client::attribute* cur_last_attr = grp->mutable_last_attr(j);
		if (cur_attr->type() == cur_last_attr->type() && cur_attr->value() == cur_last_attr->value()){
			is_same = is_same +1;
		}
	}

	return is_same;
}

bool baptize_opt(grid_role_props* grp,const config_props* item_base_info,uint32 baptize_type)
{
	const std::map<int, cpf_val_3>& rand_attr_map_ = item_base_info->get_random_attribute();
	// 洗练上限值
	uint32 cur_max = 0;
	// 增幅值
	int amplification_value = 0;
	// 随机最小的值
	uint32 rand_1 = 0;
	uint32 rand_2 = 0;
	int last_value = 0;
	grp->clear_last_attr();
	// 系数
	float coefficient_value = 0.0f;

	for (int32 i = 0; i < grp->attr_size(); ++i){
		client::attribute* attr = grp->mutable_attr(i);
		if (attr == NULL){
			continue;
		}
		amplification_value = static_cast<int>(get_amplification_by_type(attr->type()));
		if (amplification_value == 0){
			continue;
		}

		std::map<int, cpf_val_3>::const_iterator it_max = rand_attr_map_.find(attr->type());
		if (it_max == rand_attr_map_.end()){
			continue;
		}

		cur_max = it_max->second._max;

		if (attr->value() >= cur_max){
			client::attribute* last_attr = grp->add_last_attr();
			last_attr->set_type(attr->type());
			last_attr->set_value(attr->value());
			continue;
		}

		uint32 range_id = static_cast<uint32>(ceil((attr->value() / static_cast<float>(cur_max)) * 10.0f));
		const config_bipize* bipize_info = CONFMGR->get_config_bipize_mgr().get_config_bipize(range_id);
		if (bipize_info == NULL){
			continue;
		}


		switch(baptize_type)
		{
		case BAPTIZE_TYPE_MONEY:
			{
				coefficient_value = bipize_info->get_coin_lower_limit() / 100.0f;
			}
			break;
		case BAPTIZE_TYPE_ONE:
			{
				coefficient_value = bipize_info->get_ore1_lower_limit() / 100.0f;
			}
			break;
		case BAPTIZE_TYPE_TWO:
			{
				coefficient_value = bipize_info->get_ore2_lower_limit() / 100.0f;
			}
			break;
		default:
			return false;
			break;
		}

		int attr_value = static_cast<int>(attr->value()) - static_cast<int>(ceil(coefficient_value * amplification_value));
		if (attr_value < 0){
			rand_1 = 1;
		}else {
			rand_1 = max(1u,static_cast<uint32>(attr_value));
		}

		rand_2 = min(attr->value()+amplification_value,cur_max);

		last_value = static_cast<int>(mtrandom::rand_mid32(rand_1,rand_2));

		client::attribute* last_attr = grp->add_last_attr();
		last_attr->set_type(attr->type());
		last_attr->set_value(last_value);
	}

	return true;
}

bool baptize_rule(uint32 baptize_type,scene_user* psu,uint32 grid_index)
{
	player_role_props& prp = psu->get_player_role().get_role_props();
	// 获取洗练的物品所在的背包
	props_parser* pp = prp.get_pos_parser(grid_index);
	if (pp == NULL){
		return false;
	}

	// 获得当前格子物品的数据
	grid_role_props* grp = pp->mutable_props(grid_index);
	if (grp == NULL){
		return false;
	}

	// 获取物品的基础数据
	const config_props* item_base_info = CONFMGR->get_config_props_mgr().get_config_props(grp->props_id());

	if (item_base_info == NULL){
		return false;
	}

	// 检测是否满值
	if (is_max(grp,item_base_info,false)){
		return false;
	}

	// 检测东西够不够
	const std::map<int, succinct_info>& need_item_list_ = item_base_info->get_succinct_need_material();
	std::map<int, succinct_info>::const_iterator it_su = need_item_list_.find(baptize_type);
	if (it_su == need_item_list_.end()){
		return false;
	}

	uint32 cur_money = psu->get_attr(TAEL);

	uint32 item_id = static_cast<uint32>(it_su->second.item_id);
	uint32 need_value = static_cast<uint32>(it_su->second.item_count);

	if (baptize_type == BAPTIZE_TYPE_MONEY){
		if (cur_money < need_value){
			return false;
		}
	}else{
		props_parser& pack_obj = prp.get_pack();
		if (pack_obj.get_props_num(item_id) < need_value){
			return false;
		}
	}

	// 规则:
	baptize_opt(grp,item_base_info,baptize_type);
	
	uint32 match_size = grp->last_attr_size();
	uint32 same_count = check_same(grp,match_size);
	// 体验优化：
	while(match_size == same_count){
		if (is_max(grp,item_base_info)){
			break;
		}
		// 再洗
		baptize_opt(grp,item_base_info,baptize_type);

		same_count = check_same(grp,match_size);
	}

	prp.change_flag(grp->grid_index());
	uint32 l_old_value = 0;
	uint32 l_new_value = 0;
	// 扣除东西
	if (baptize_type == BAPTIZE_TYPE_MONEY){
		l_old_value = psu->get_attr(TAEL);
		// 扣钱
		psu->sub_money(TAEL,need_value);
		l_new_value = psu->get_attr(TAEL);
		psu->send_client_change_attr();
		// 记录消耗的铜钱
		grp->set_expend_money(grp->expend_money() + need_value);
	}else{
		l_old_value = psu->get_props_num(item_id);
		// 扣除道具
		psu->sub_props(item_id,need_value);
		l_new_value = psu->get_props_num(item_id);
		// 记录消耗的物品
		if (grp->info_size() == 0){
			client::expend_item_info* info = grp->add_info();
			info->set_expend_id(item_id);
			info->set_expend_count(need_value);
		}else{
			bool is_new = true;
			for (int i = 0;i < grp->info_size(); ++i){
				client::expend_item_info* info = grp->mutable_info(i);
				if (info == NULL){
					continue;
				}

				if (info->expend_id() == item_id){
					info->set_expend_count(info->expend_count() + need_value);
					is_new = false;
					break;
				}
			}

			if (is_new){
				client::expend_item_info* info = grp->add_info();
				info->set_expend_id(item_id);
				info->set_expend_count(need_value);
			}
		}
	}

	psu->send_client_change_props_msg();
	/////////////////////////////////////////////////log//////////////////////////////////////////////////////////////////////
	//洗练装备XX|消耗铜钱or一级洗练石or二级洗练石XX|前值XX|后值XX|洗出属性XX|值XX|洗出属性XX|值XX；洗出属性XX|值XX|洗出属性XX|值XX；
	string a = "";
	char ta[128] = {0};
	for (int b = 0; b < grp->last_attr_size(); ++b){
		const client::attribute& attr = grp->last_attr(b);
		memset(ta, 0, sizeof(ta));
		string name = get_attr_name(attr.type());
		snprintf(ta, sizeof(ta), "|洗出属性%s,值为%u", name.c_str(),attr.value());
		a += ta;
	}

	amtsvr_context* ctx = psu->get_scene()->get_ctx();
	amtsvr_billlog(ctx, "%u|client.baptize_req|%s|%u|%u|%u|%u%s", psu->get_role_id(),item_base_info->get_item_name().c_str(),
		baptize_type,need_value,l_old_value,l_new_value,a.c_str());

	return true;
}

//物品使用
void* on_pb_use_props_req(client::use_props_req* up, void* ud, size_t* rsp_len)
{
	struct game_service_t* ss = (struct game_service_t*)ud;
	scene_user* su = ss->sim->get_user(up->svr_head().role_id());
	if (su == NULL) {
		return NULL;
	}

	client::use_props_rsp rsp_msg;
	player_role_props& prp = su->get_player_role().get_role_props();
	// 获取出售的物品所在的背包
	props_parser* pp = prp.get_pos_parser(up->grid_index());

	if (pp == NULL){
		return NULL;
	}

	uint32 use_count = 0;
	// 先判断东西够不够
	use_count = pp->get_index_props_num(up->grid_index());

	if (use_count < up->use_num() || use_count == 0){
		return NULL;
	}

	// 获得当前格子物品的数据
	grid_role_props* grp = pp->mutable_props(up->grid_index());

	if (grp == NULL){
		return NULL;
	}
	// 获取物品的基础数据
	const config_props* item_base_info = CONFMGR->get_config_props_mgr().get_config_props(grp->props_id());

	if (item_base_info == NULL){
		return NULL;
	}
	// 过期
	if (grp->has_is_due()){
		if (grp->is_due()){
			rsp_msg.set_opt_type(ITEM_OPERATE_TYPE_ITEM_IS_DUE);
			rsp_msg.set_grid_index(up->grid_index());
			return NULL;
		}
	}

	ITEM_OPERATE_TYPE ret = su->props_use_effect(up->grid_index(),up->use_num(),up->rds());

	//su->send_client_change_props_msg();

	if (ret != ITEM_OPERATE_TYPE_SUCCESS){
		rsp_msg.set_opt_type(ret);
		rsp_msg.set_grid_index(up->grid_index());
		su->send_client(rsp_msg);
		return NULL;
	}
	else{
		rsp_msg.set_opt_type(ret);
		rsp_msg.set_grid_index(up->grid_index());
		su->send_client(rsp_msg);
	}

	// 删除物品
	if (pp->remove_props(item_base_info->get_item_id(),up->use_num()) != 0){
		return NULL;
	}

	su->send_client_change_props_msg();

	//amtsvr_billlog(ss->ctx, "%u|%s|%u|%u|%u|%u", su->get_role_id(), up->GetTypeName().c_str(), up->grid_index(), up->use_num(),rsp_msg.opt_type(),rsp_msg.grid_index());

    return NULL;
}

// 物品出售
void* on_pb_sell_props_req(client::sell_props_req* req, void* ud, size_t* rsp_len)
{
	struct game_service_t* ss = (struct game_service_t*)ud;
	scene_user* su = ss->sim->get_user(req->svr_head().role_id());
	if (su == NULL) {
		return NULL;
	}

	client::sell_props_rsp rsp_msg;
	rsp_msg.set_grid_index(req->grid_index());
	player_role_props& prp = su->get_player_role().get_role_props();
	// 获取出售的物品所在的背包
	props_parser* pp = prp.get_pos_parser(req->grid_index());

	if (pp == NULL){
		return NULL;
	}

	// 获得当前格子物品的数据
	grid_role_props* grp = pp->mutable_props(req->grid_index());

	if (grp == NULL){
		return NULL;
	}
	// 获取物品的基础数据
	const config_props* item_base_info = CONFMGR->get_config_props_mgr().get_config_props(grp->props_id());

	if (item_base_info == NULL){
		return NULL;
	}
	// 为0 则为不可出售物品
	if (item_base_info->get_is_shop_recover() == 0){
		rsp_msg.set_result_type(SELL_RESULT_NOT_SELL);
		su->send_client(rsp_msg);
		return NULL;
	}
	// 需要检测背包是否真的有那么多物品


	// 算钱
	uint32 sell_total = item_base_info->get_sales_price() * req->props_count();
	uint32 props_id = 0;
	// 删除物品
	if (pp->remove_props(req->grid_index(),req->props_count(),props_id) != 0){
		return NULL;
	}
	// 加钱
	su->add_money(TAEL,sell_total);
	
	rsp_msg.set_result_type(SELL_RESULT_SUCCESS);
	

	su->send_client_change_attr();
	su->send_client_change_props_msg();
	su->send_client(rsp_msg);

	amtsvr_billlog(ss->ctx, "%u|%s|%u|%u|%u|%u", su->get_role_id(), req->GetTypeName().c_str(), req->grid_index(), req->props_count(),rsp_msg.grid_index(),rsp_msg.result_type());

	return NULL;
}

void* on_pb_move_props_req(client::move_props_req* req, void* ud, size_t* rsp_len)
{
    struct game_service_t* ss = (struct game_service_t*)ud;
    scene_user* su = ss->sim->get_user(req->svr_head().role_id());
    if (su == NULL) {
        return NULL;
    }

    player_role_props& prp = su->get_player_role().get_role_props();
    if (prp.move_props(req->src_grid_index(), req->dst_grid_index()) != 0) {
        return NULL;
    }

	props_parser* src = prp.get_pos_parser(req->src_grid_index());
	if (src == NULL){
		return NULL;
	}

    grid_role_props* src_pi = src->mutable_props(req->src_grid_index());
	if (src_pi == NULL){
		return NULL;
	}

	const config_props* src_item = CONFMGR->get_config_props_mgr().get_config_props(src_pi->props_id());
	if (src_item == NULL){
		return NULL;
	}

	// 外观处理
	if (req->src_grid_index() == EQUIP_ARMOR || req->src_grid_index() == EQUIP_GRID_BEG){
		switch(src_item->get_item_sub_type())
		{
		case PST_WEAPON: 
			su->mutable_attr()->set_val(WEAPON_ID,prp.get_weapon_id());
			su->set_store_weapon(0);
#ifndef __LIB_MINI_SERVER__
			su->update_online_smy_info(client::SMY_ROLE_WEAPON_ID, prp.get_weapon_id());
#endif
			break;
		case PST_ARMOR:
			su->mutable_attr()->set_val(ARMOR_ID, prp.get_suit_id());
#ifndef __LIB_MINI_SERVER__
			su->update_online_smy_info(client::SMY_ROLE_ARMOR_ID,prp.get_suit_id());
#endif
			break;
		default:
			break;
		}

		su->send_client_change_attr();
	}

	if (req->src_grid_index() <= EQUIP_SHOE){
		su->calculate_attr();
		su->send_client_change_attr();
	}else {
		uint32 pet_id = req->src_grid_index() / 10;
		su->pet_attr_change(pet_id,PET_ATTR_CHANGE_TYPE_EQUIP);
	}

    su->send_client_change_props_msg();
	uint32 total_power = su->cal_equip_reward_power();
	if (total_power > su->get_attr(EQUIP_POWER)){
		su->mutable_attr()->set_val(EQUIP_POWER,total_power);
	}
	su->update_equip_power_rank(REWARD_TYPE_POWER,total_power);
	////////////////////////////////////////////////////////////////log//////////////////////////////////////////////////////////
	std::map<int,int> total_attr_map_;
	total_attr_map_.clear();
	// 基础属性：
	std::map<int,int> base_attr_map_;
	base_attr_map_.clear();
	base_attr_map_ = src_item->get_game_attribute();
	string base = "";
	std::map<int,int>::iterator it_base = base_attr_map_.begin();
	char tbase[256] = {0};
	for (; it_base != base_attr_map_.end();++it_base){
		memset(tbase, 0, sizeof(tbase));
		// %u|%u 属性类型|值
		string name = get_attr_name(it_base->first);
		snprintf(tbase, sizeof(tbase), "|基础属性:%s,值%u", name.c_str(),it_base->second);
		base += tbase;
		if (total_attr_map_.size() != 0){
			std::map<int,int>::iterator it = total_attr_map_.find(it_base->first);
			if (it != total_attr_map_.end()){
				it->second += it_base->second;
			}else{
				total_attr_map_.insert(pair<int,int>(it_base->first,it_base->second));
			}
		}else{
			total_attr_map_.insert(pair<int,int>(it_base->first,it_base->second));
		}
	}

	// 强化属性：
	string up = "";
	char tup[256] = {0};
	for (int w = 0; w < src_pi->upgrade_attr_size(); ++w){
		const client::attribute& attr = src_pi->upgrade_attr(w);
		memset(tup, 0, sizeof(tup));
		// %u|%u 属性类型|值
		string name = get_attr_name(attr.type());
		snprintf(tup, sizeof(tup), "|强化的属性:%s,值:%u", name.c_str(),attr.value());
		up += tup;
		if (total_attr_map_.size() != 0){
			std::map<int,int>::iterator it = total_attr_map_.find(static_cast<int>(attr.type()));
			if (it != total_attr_map_.end()){
				it->second += static_cast<int>(attr.value());
			}else{
				total_attr_map_.insert(pair<int,int>(static_cast<int>(attr.type()),static_cast<int>(attr.value())));
			}
		}else{
			total_attr_map_.insert(pair<int,int>(static_cast<int>(attr.type()),static_cast<int>(attr.value())));
		}
	}

	// 洗练的属性：
	string ra = "";
	char tra[256] = {0};
	for (int t = 0; t < src_pi->attr_size(); ++t){
		const client::attribute& attr = src_pi->attr(t);
		memset(tra, 0, sizeof(tra));
		// %u|%u 属性类型|值
		string name = get_attr_name(attr.type());
		snprintf(tra, sizeof(tra), "|洗练的属性:%s,值:%u", name.c_str(),attr.value());
		ra += tra;
		if (total_attr_map_.size() != 0){
			std::map<int,int>::iterator it = total_attr_map_.find(static_cast<int>(attr.type()));
			if (it != total_attr_map_.end()){
				it->second += static_cast<int>(attr.value());
			}else{
				total_attr_map_.insert(pair<int,int>(static_cast<int>(attr.type()),static_cast<int>(attr.value())));
			}
		}else{
			total_attr_map_.insert(pair<int,int>(static_cast<int>(attr.type()),static_cast<int>(attr.value())));
		}
	}

	// 玩家属性前后值
	string pl = "";
	char tpl[512] = {0};
	std::map<int,int>::iterator it_pl = total_attr_map_.begin();
	for (; it_pl != total_attr_map_.end(); ++it_pl){
		memset(tpl, 0, sizeof(tpl));
		ROLE_ATTRIBUTE type = ATTR_CFG_TO_CODE[it_pl->first];
		uint32 now_value = su->get_attr(type);
		string name = get_attr_name(it_pl->first);
		// %u|%u 属性类型|前值|后值
		snprintf(tpl, sizeof(tpl), "|玩家属性:%s,前值:%u /后值:%u", name.c_str(),now_value + it_pl->second,now_value);
		pl += tpl;
	}
	// 装备的属性：穿戴/卸下装备XX，装备等级XX，基础属性XX-XX；成长值XX；品质XX（蓝、紫、橙）；强化等级XX，强化属性XX；洗练属性XX,值XX；属性XX,值XX；属性XX,值XX；属性XX,值XX；
	// 穿脱后属性对比：玩家属性XX，前值XX/后值XX；属性XX，前值XX/后值XX；属性XX，前值XX/后值XX；
	uint32 is_up = 0;
	amtsvr_billlog(ss->ctx, "%u|%s|%u|%s|%u%s|%u|%u|%u%s%s%s", su->get_role_id(), req->GetTypeName().c_str(),is_up,src_item->get_item_name().c_str(),src_item->get_need_level(),
	base.c_str(),src_item->get_basic_upgrade(),src_item->get_name_color_index(),src_pi->level(),up.c_str(),ra.c_str(),pl.c_str());

    return NULL;
}

void* on_pb_equip_props_req(client::equip_props_req* req, void* ud, size_t* rsp_len)
{
    struct game_service_t* ss = (struct game_service_t*)ud;
    scene_user* su = ss->sim->get_user(req->svr_head().role_id());
    if (su == NULL) {
        return NULL;
    }

    // 检查参数
    player_role_props& prp = su->get_player_role().get_role_props();
    props_parser* src = prp.get_pos_parser(req->src_grid_index());
    if (src == NULL /*|| !src->is_depot_pack()*/) {
        return NULL;
    }

    props_parser* dst = prp.get_pos_parser(req->dst_grid_index());
    if (dst == NULL /*|| !dst->is_equip_pack()*/) {
        return NULL;
    }

    grid_role_props* src_pi = src->mutable_props(req->src_grid_index());
    if (src_pi == NULL || !src_pi->has_props()) {
        return NULL;
    }

	const config_props* src_item = CONFMGR->get_config_props_mgr().get_config_props(src_pi->props_id());
	if (src_item == NULL){
		return NULL;
	}

	if (src->is_depot_pack()){
		int cur_dst_sub = 0;
		bool match_job = false;
		int cur_job = static_cast<int>(su->get_attr().get_val(PROFESSION));
		int need_job = src_item->get_need_job();
		if (cur_job == need_job || need_job == 5){
			match_job = true;
		}
		if (need_job == 4 && (cur_job == 2 || cur_job == 1)){
			match_job = true;
		}

		cur_dst_sub = static_cast<int>(req->dst_grid_index());
		if (req->dst_grid_index() > EQUIP_SHOE){
			if (need_job == 6){
				match_job = true;
			}
			player_role_pet& player_pet = su->get_player_role().get_pets();
			uint32 equip_pet_id = req->dst_grid_index() / 10;
			cur_dst_sub = req->dst_grid_index() % 10;
			client::pet_db_info pet_info;
			if (!player_pet.get_pet_info(equip_pet_id,pet_info)){
				return NULL;
			}
		}
		// 检测职业、等级、类型
		if ( !match_job  || (static_cast<int>(su->get_attr().get_val(LEVEL)) < src_item->get_need_level())
			|| (src_item->is_equip() && src_item->is_pet_equip())){
			return NULL;
		}

		// 检测装备的位置的合法性
		// 位置：手还是腿还是。。
		int cur_src_sub = src_item->get_item_sub_type();
		////检测dst这个位置是否能装备
		if (cur_src_sub != cur_dst_sub ){
			return NULL;
		}
	}
	uint32 add_grid_index = 0;
    grid_role_props pi;
    if (dst->pop_props(req->dst_grid_index(), pi) == 0 && pi.has_props()) {
        // 若dst有物品，要把物品放回在装备背包
        add_grid_index = src->add_props_equip(pi);
    }

	if (dst->is_equip_pack()) {
		pi.set_bind(1);
	}

    // 移动数据
    if (prp.move_props(req->src_grid_index(), req->dst_grid_index()) != 0) {
        return NULL;
    }

	grid_role_props* dst_pi = dst->mutable_props(req->dst_grid_index());
	
	if (dst_pi == NULL){
		return NULL;
	}

	if (req->dst_grid_index() <= EQUIP_SHOE){
		// 更新人物属性
		su->calculate_attr();
	}else{
		uint32 cur_pet_id = req->dst_grid_index() / 10;
		su->pet_attr_change(cur_pet_id,PET_ATTR_CHANGE_TYPE_EQUIP);
	}
	// 外观处理
	if (req->dst_grid_index() == EQUIP_ARMOR || req->dst_grid_index() == EQUIP_GRID_BEG){
		switch(src_item->get_item_sub_type())
		{
		case PST_WEAPON:
			{
				uint32 king_weapon = su->get_king_weapon();
				if(king_weapon == 0)		// 不是国王
				{
					su->mutable_attr()->set_val(WEAPON_ID, prp.get_weapon_id());
				}
				else
				{
					if(su->get_attr().get_val(WEAPON_ID) == 0)
					{
						su->mutable_attr()->set_val(WEAPON_ID, king_weapon);
					}
					su->set_store_weapon(prp.get_weapon_id());
				}
#ifndef __LIB_MINI_SERVER__
				su->update_online_smy_info(client::SMY_ROLE_WEAPON_ID, su->get_attr().get_val(WEAPON_ID));
#endif
			}
			break;
		case PST_ARMOR:
			su->mutable_attr()->set_val(ARMOR_ID, prp.get_suit_id());
#ifndef __LIB_MINI_SERVER__
			su->update_online_smy_info(client::SMY_ROLE_ARMOR_ID,prp.get_suit_id());
#endif
			break;
		default:
			break;
		}
	}


	client::equip_props_rsp rsp_msg;
	rsp_msg.set_clear_grid_index(req->src_grid_index());
	if (pi.has_props()){
		rsp_msg.set_add_grid_index(add_grid_index);
	}

    // 下发通知
    su->send_client_change_attr();
    su->send_client_change_props_msg();
	su->send_client(rsp_msg);

	uint32 total_power = su->cal_equip_reward_power();
	if (total_power > su->get_attr(EQUIP_POWER)){
		su->mutable_attr()->set_val(EQUIP_POWER,total_power);
	}
	su->update_equip_power_rank(REWARD_TYPE_POWER,total_power);
	////////////////////////////////////////////////////////////////log//////////////////////////////////////////////////////////
	std::map<int,int> total_attr_map_;
	total_attr_map_.clear();
	// 基础属性：
	std::map<int,int> base_attr_map_;
	base_attr_map_.clear();
	base_attr_map_ = src_item->get_game_attribute();
	string base = "";
	std::map<int,int>::iterator it_base = base_attr_map_.begin();
	char tbase[1024] = {0};
	for (; it_base != base_attr_map_.end();++it_base){
		memset(tbase, 0, sizeof(tbase));
		// %u|%u 属性类型|值
		string name = get_attr_name(it_base->first);
		snprintf(tbase, sizeof(tbase), "|基础的属性:%s,值:%u", name.c_str(),it_base->second);
		base += tbase;
		if (total_attr_map_.size() != 0){
			std::map<int,int>::iterator it = total_attr_map_.find(it_base->first);
			if (it != total_attr_map_.end()){
				it->second += it_base->second;
			}else{
				total_attr_map_.insert(pair<int,int>(it_base->first,it_base->second));
			}
		}else{
			total_attr_map_.insert(pair<int,int>(it_base->first,it_base->second));
		}
	}

	// 强化属性：
	string up = "";
	char tup[1024] = {0};
	for (int w = 0; w < src_pi->upgrade_attr_size(); ++w){
		const client::attribute& attr = src_pi->upgrade_attr(w);
		memset(tup, 0, sizeof(tup));
		// %u|%u 属性类型|值
		string name = get_attr_name(attr.type());
		snprintf(tup, sizeof(tup), "|强化的属性:%s,值:%u", name.c_str(),attr.value());
		up += tup;
		if (total_attr_map_.size() != 0){
			std::map<int,int>::iterator it = total_attr_map_.find(static_cast<int>(attr.type()));
			if (it != total_attr_map_.end()){
				it->second += static_cast<int>(attr.value());
			}else{
				total_attr_map_.insert(pair<int,int>(static_cast<int>(attr.type()),static_cast<int>(attr.value())));
			}
		}else{
			total_attr_map_.insert(pair<int,int>(static_cast<int>(attr.type()),static_cast<int>(attr.value())));
		}
	}

	// 洗练的属性：
	string ra = "";
	char tra[1024] = {0};
	for (int t = 0; t < src_pi->attr_size(); ++t){
		const client::attribute& attr = src_pi->attr(t);
		memset(tra, 0, sizeof(tra));
		// %u|%u 属性类型|值
		string name = get_attr_name(attr.type());
		snprintf(tra, sizeof(tra), "|洗练的属性:%s,值:%u", name.c_str(),attr.value());
		ra += tra;
		if (total_attr_map_.size() != 0){
			std::map<int,int>::iterator it = total_attr_map_.find(static_cast<int>(attr.type()));
			if (it != total_attr_map_.end()){
				it->second += static_cast<int>(attr.value());
			}else{
				total_attr_map_.insert(pair<int,int>(static_cast<int>(attr.type()),static_cast<int>(attr.value())));
			}
		}else{
			total_attr_map_.insert(pair<int,int>(static_cast<int>(attr.type()),static_cast<int>(attr.value())));
		}
	}

	// 玩家属性前后值
	string pl = "";
	char tpl[512] = {0};
	std::map<int,int>::iterator it_pl = total_attr_map_.begin();
	for (; it_pl != total_attr_map_.end(); ++it_pl){
		memset(tpl, 0, sizeof(tpl));
		ROLE_ATTRIBUTE type = ATTR_CFG_TO_CODE[it_pl->first];
		uint32 now_value = su->get_attr(type);
		// %u|%u 属性类型|前值|后值
		string name = get_attr_name(it_pl->first);
		snprintf(tpl, sizeof(tpl), "|玩家属性:%s,前值:%u /后值:%u", name.c_str(),now_value - it_pl->second,now_value);
		pl += tpl;
	}
	// 装备的属性：穿戴/卸下装备XX，装备等级XX，基础属性XX-XX；成长值XX；品质XX（蓝、紫、橙）；强化等级XX，强化属性XX；洗练属性XX,值XX；属性XX,值XX；属性XX,值XX；属性XX,值XX；
	// 穿脱后属性对比：玩家属性XX，前值XX/后值XX；属性XX，前值XX/后值XX；属性XX，前值XX/后值XX；
	uint32 is_up = 1;
	amtsvr_billlog(ss->ctx, "%u|%s|%u|%s|%u%s|%u|%u|%u%s%s%s", su->get_role_id(), req->GetTypeName().c_str(),is_up,src_item->get_item_name().c_str(),src_item->get_need_level(),
		base.c_str(),src_item->get_basic_upgrade(),src_item->get_name_color_index(),src_pi->level(),up.c_str(),ra.c_str(),pl.c_str());

    return NULL;
}

void* on_pb_pick_up_props_req(client::pick_up_props_req* req, void* ud, size_t* rsp_len)
{
    PB_FUNC_GUARD(req);

    scene_inst* psi = psu->get_scene();
    if (psi == NULL) {
        return NULL;
    }
    client::pick_up_props_rsp rsp;
    rsp.mutable_index()->CopyFrom(req->index());
	if(!psu->is_can_add_props())
	{
		rsp.set_ret(client::pick_up_props_rsp_result_null);
		psu->send_client(rsp);
		return NULL;
	}
    scene_lose_mgr* slm = psi->get_scene_lose_mgr();
    client::drop_props_msg msg;
    for (int32 i = 0; i < req->index_size(); ++i) {
        scene_lose* sl = slm->get_lose(req->index(i));
        if (sl == NULL) {
             rsp.set_ret(client::pick_up_props_rsp_result_null);
             psu->send_client(rsp);
			 return NULL;
        } else {
			if(!slm->can_pick(req->index(i), req->svr_head().role_id()))
			{
				rsp.set_ret(client::pick_up_props_rsp_result_not_your);
				psu->send_client(rsp);
				return NULL;
			}
            psu->add_props(sl->pi(), false);

            msg.set_m(client::drop_props_msg_METHOD_DEL);
            client::drop_props_info* p = msg.add_dpi();
            p->CopyFrom(*sl);

            psi->on_pick_props(psu, p->mutable_pi());
            slm->pick_lose(req->index(i));
        }
    }

    rsp.set_ret(client::pick_up_props_rsp_result_success);
    psu->send_client(rsp);
    psu->send_client_change_props_msg();
    if (msg.dpi_size() != 0) {
        psi->scene_broadcast(*psu, msg);
    }

	amtsvr_billlog(ss->ctx, "%u|%s", psu->get_role_id(), req->GetTypeName().c_str());

    return NULL;
}
// 强化
void* on_pb_upgrade_equip_req(client::upgrade_equip_req* req, void* ud, size_t* rsp_len)
{
	struct game_service_t* ss = (struct game_service_t*)ud;
	scene_user* su = ss->sim->get_user(req->svr_head().role_id());
	if (su == NULL) {
		return NULL;
	}

	client::upgrade_equip_rsp upgrade_rsp;
	upgrade_rsp.set_grid_index(req->grid_index());

	player_role_props& prg = su->get_player_role().get_role_props();

	// 获取强化的物品所在的背包
	props_parser* pp = prg.get_pos_parser(req->grid_index());
	// 获取主背包
	props_parser& pack_obj = prg.get_pack();

	if (pp == NULL){
		return NULL;
	}
	// 获得当前格子物品的数据
	grid_role_props* grp = pp->mutable_props(req->grid_index());

	if (grp == NULL){
		return NULL;
	}
	uint32 cur_lvl = grp->level();
	// 获取物品的基础数据
	const config_props* base_info = CONFMGR->get_config_props_mgr().get_config_props(grp->props_id());

	if (base_info == NULL){
		return NULL;
	}

	// 如果它不是装备，则返回！
	if (base_info->get_item_type() != PT_EQUIP && base_info->get_item_type() != PT_PET_EQUIP){
		return NULL;
	}

	const config_equip_upgrade* upgrade_info = CONFMGR->get_config_equip_upgrade_mgr().get_config_equip_upgrade(grp->level());
	if (upgrade_info == NULL){
		return NULL;
	}
	uint32 l_old_item_lvl = grp->level();
	uint32 next_level = req->target_level();
	uint32 player_level = su->get_attr().get_val(LEVEL);

	if (next_level > player_level){
		upgrade_rsp.set_result_type(upgrade_equip_rsp_result_failed);
		su->send_client(upgrade_rsp);
		return NULL;
	}

	const config_equip_upgrade* next_upgrade_info = CONFMGR->get_config_equip_upgrade_mgr().get_config_equip_upgrade(next_level);
	if (next_upgrade_info == NULL) {
		return NULL;
	}

	// 规则：
	////////////////////////////////////////////
	// 总消耗的铜币
	uint32 expend_total_money = 0;
	std::map<uint32,uint32> expend_map_;
	expend_map_.clear();
	uint32 opt_type = 0;
	strengthen_expend_calculate(grp->props_id(),grp->level(),next_level,expend_total_money,expend_map_,opt_type);
	std::map<uint32,uint32>::iterator it = expend_map_.begin();
	uint32 l_expend_old = 0;
	for (; it != expend_map_.end(); ++it){
		l_expend_old = pack_obj.get_props_num(it->first);
		if (l_expend_old < it->second){
			return NULL;
		}
	}
	uint32 l_old_money = su->get_attr(TAEL);
	if (l_old_money < expend_total_money){
		return NULL;
	}
	// 升级前属性
	std::map<int,int> old_up_map_;
	old_up_map_.clear();
	
	for (int z = 0; z < grp->upgrade_attr_size(); ++z){
		const client::attribute& attr = grp->upgrade_attr(z);
		old_up_map_.insert(pair<int,int>(attr.type(),attr.value()));
	}


	// 规则
	bool is_success =  false;
	is_success = pp->strengthen_equip(grp->level(),next_level,req->grid_index());

	if (is_success){
		upgrade_rsp.set_result_type(upgrade_equip_rsp_result_success);
	}else {
		upgrade_rsp.set_result_type(upgrade_equip_rsp_result_failed);
	}

	// 扣道具
	std::map<uint32,uint32>::iterator it_sub = expend_map_.begin();
	uint32 l_less_expend = 0;
	for (;it_sub != expend_map_.end();++it_sub){
		l_less_expend += it_sub->second;
		su->sub_props(it_sub->first,it_sub->second);
	}

	if (expend_total_money != 0){
		su->sub_money(TAEL,expend_total_money);
	}

	su->send_client_change_attr();
	// 若强化的东东在身上 则计算属性
	if (req->grid_index() <= EQUIP_SHOE){
		su->calculate_attr();
	} else {
		uint32 pet_id = req->grid_index() / 10;
		su->pet_attr_change(pet_id,PET_ATTR_CHANGE_TYPE_EQUIP);
	}

	su->send_client_change_props_msg();
	su->send_client_change_attr();
	su->send_client(upgrade_rsp);
	uint32 total_power = su->cal_equip_reward_power();
	if (total_power > su->get_attr(EQUIP_POWER)){
		su->mutable_attr()->set_val(EQUIP_POWER,total_power);
	}
	su->update_equip_power_rank(REWARD_TYPE_POWER,total_power);
	CHECK_TARGET_PROGRESS(su, EQUIP_UP_STAR, req->target_level() - cur_lvl);
	///////////////////////////////////////////////////log////////////////////////////////////////////////////////
	// 升级后属性
	std::map<int,int> new_up_map_;
	new_up_map_.clear();
	for (int k = 0; k < grp->upgrade_attr_size(); ++k){
		const client::attribute& up_attr = grp->upgrade_attr(k);
		new_up_map_.insert(pair<int,int>(up_attr.type(),up_attr.value()));
	}

	string a = "";
	char tmp[1024] = {0};
	std::map<int,int>::iterator it_new = new_up_map_.begin();
	for (;it_new != new_up_map_.end(); ++it_new){
		memset(tmp, 0, sizeof(tmp));
		string name = get_attr_name(it_new->first);
		if (old_up_map_.size() != 0){
			std::map<int,int>::iterator it_old = old_up_map_.find(it_new->first);
			if (it_old == old_up_map_.end()){
				snprintf(tmp, sizeof(tmp), "|强化增加属性:%s,前值:%u/后值:%u",name.c_str(),0,it_new->second);
			}else{
				snprintf(tmp, sizeof(tmp), "|强化增加属性:%s,前值:%u/后值:%u",name.c_str(),it_old->second,it_new->second);
			}
		}else{
			snprintf(tmp, sizeof(tmp), "|强化增加属性:%s,前值:%u/后值:%u",name.c_str(),0,it_new->second);
		}

		a += tmp;
	}
	uint32 l_new_expend = l_expend_old - l_less_expend;
	uint32 l_new_money = su->get_attr(TAEL);
	// 装备XX|消耗强化石/升星石XX，前值XX|后值XX；消耗铜钱XX|前值XX|后值XX；强化等级XX|前值XX|后值XX；强化增加的属性XX|值XX，前值XX|后值XX；
	amtsvr_billlog(ss->ctx, "%u|%s|%s|%u|%u|%u|%u|%u|%u|%u|%u|%u%s", su->get_role_id(), req->GetTypeName().c_str(),base_info->get_item_name().c_str(),opt_type,l_less_expend,
	l_expend_old,l_new_expend,expend_total_money,l_old_money,l_new_money,l_old_item_lvl,grp->level(),a.c_str());

	return NULL;
}

void strengthen_expend_calculate(uint32 props_id, uint32 old_level,uint32 new_level,uint32& expend_money,std::map<uint32,uint32>& expend_map_,uint32& opt_type)
{
	// 获取物品的基础数据
	const config_props* base_info = CONFMGR->get_config_props_mgr().get_config_props(props_id);

	if (base_info == NULL){
		return;
	}

	if (old_level == new_level){
		return;
	}

	uint32 inter_count = new_level - old_level;
	uint32 cur_level = 0;
	uint32 interval_value = 0;
	// 强化石/升星石消耗基数（需要除10）/10.0f
	uint32 base_value = 0;
	// 强化石消耗系数
	uint32 expend_coefficient = 0;
	// 升星石消耗系数
	uint32 expend_coefficient_1 = 0;
	// 强化/升星消耗铜钱的基数(升星不用考虑，不消耗铜钱)基数读强化的基数
	uint32 money_base = 0;
	// 强化/升星消耗铜钱的系数(升星不用考虑，不消耗铜钱)
	uint32 money_coefficient = 0;

	uint32 need_count = 0;
	for (uint32 i = 1; i <= inter_count; ++i){
		cur_level = old_level + i;
		interval_value = cur_level % 10;
		if (interval_value > 0){
			opt_type = UT_STRENGTHE;
		}else{
			opt_type = UT_UPGRADE;
		}

		const config_equip_upgrade* upgrade_info = CONFMGR->get_config_equip_upgrade_mgr().get_config_equip_upgrade(cur_level-1);
		if (upgrade_info == NULL){
			return;
		}

		if (opt_type == UT_STRENGTHE){
			//强化
			base_value = static_cast<uint32>(base_info->get_upgrade_cost());
			expend_coefficient = upgrade_info->get_upgrade_stone_ratio();
			money_base = base_value;
			money_coefficient = upgrade_info->get_upgrade_coin_ratio();
			need_count =  static_cast<uint32>(ceil((expend_coefficient / 10000.0f) * (base_value /10.0f)));
			std::map<uint32,uint32>::iterator it_st = expend_map_.find(upgrade_info->get_upgrade_stone_id());
			if (it_st == expend_map_.end()){
				expend_map_.insert(pair<uint32,uint32>(upgrade_info->get_upgrade_stone_id(),need_count));
			}else{
				it_st->second += need_count;
			}
		}else{
			money_coefficient = 0;
			base_value = static_cast<uint32>(base_info->get_start_upgrade_cost());
			expend_coefficient_1 = upgrade_info->get_start_stone_ration();
			need_count = static_cast<uint32>(ceil(expend_coefficient_1 * (base_value /10.0f)));
			std::map<uint32,uint32>::iterator it_up = expend_map_.find(upgrade_info->get_start_stone_id());
			if (it_up == expend_map_.end()){
				expend_map_.insert(pair<uint32,uint32>(upgrade_info->get_start_stone_id(),need_count));
			}else{
				it_up->second += need_count;
			}
		}

		expend_money += static_cast<uint32>(ceil(money_coefficient * (base_value /10.0f)));
	}

}

float calculate_equip_power(const std::map<int, int>& game_attribute,std::multimap<int,int> attr_map_,const std::map<int, int>& upequip_map_);
// 熔炼
void* on_pb_melting_equip_req(client::melting_equip_req* req, void* ud, size_t* rsp_len)
{
	struct game_service_t* ss = (struct game_service_t*)ud;
	scene_user* su = ss->sim->get_user(req->svr_head().role_id());
	if (su == NULL) {
		return NULL;
	}

	player_role_props& prg = su->get_player_role().get_role_props();

	if (!prg.is_can_melting()){
		return NULL;
	}

	std::vector<uint32> grid_index_vec;
	grid_index_vec.clear();

	for (int i = 0; i < req->grid_index_size(); ++i){
		grid_index_vec.push_back(req->grid_index(i));
	}
	
	uint32 cur_melting = su->get_attr().get_val(MELTING);

	uint32 set_melting = cur_melting;
	// 累积总返还金钱数
	uint32 total_expend_money = 0;
	// 洗练返回的金钱
	uint32 baptize_money = 0;
	std::map<uint32,uint32> baptize_item_list_;
	baptize_item_list_.clear();
	uint32 cur_cal_melting = 0;
	std::map<uint32,uint32> expend_map_;
	expend_map_.clear();
	for (uint32 j = 0; j < grid_index_vec.size();++j){
		// 获取熔炼物品所在的背包
		props_parser* pp = prg.get_pos_parser(grid_index_vec[j]);

		if (pp == NULL || !pp->is_depot_pack()){
			return NULL;
		}
		// 获得当前格子物品的数据
		grid_role_props* grp = pp->mutable_props(grid_index_vec[j]);
		if (grp == NULL){
			return NULL;
		}
		// 获取物品的基础数据
		const config_props* item_base_info = CONFMGR->get_config_props_mgr().get_config_props(grp->props_id());
		if (item_base_info == NULL){
			return NULL;
		}

		uint32 item_id = 0;
		// 删除物品
		if (pp->remove_props(grid_index_vec[j],1,item_id) != 0){
			return NULL;
		}

		cur_cal_melting = item_base_info->get_smelting_coefficient();
		//cur_cal_melting = static_cast<uint32>(ceil(calculate_equip_power(item_base_info->get_game_attribute(),attr_map_,upequip_map_) / coefficient));
		set_melting += cur_cal_melting;

		// 强化返还：
		// 某个物品总返还的铜币
		uint32 expend_total_money = 0;
		uint32 opt_type = 0;
		strengthen_expend_calculate(grp->props_id(),0,grp->level(),expend_total_money,expend_map_,opt_type);

		total_expend_money += expend_total_money;
		// 洗练返回的东西
		baptize_money += grp->expend_money();
		for (int k = 0;k < grp->info_size(); ++k){
			client::expend_item_info* info = grp->mutable_info(k);
			if (info == NULL){
				continue;
			}
			baptize_item_list_.insert(std::pair<uint32,uint32>(info->expend_id(),info->expend_count()));
		}
		CHECK_TARGET_PROGRESS(su, SMELT_EQUIP);
	}

	if (total_expend_money !=0){
		su->add_money(TAEL,total_expend_money);
	}

	std::map<uint32,uint32>::iterator it_expend = expend_map_.begin();
	for (;it_expend != expend_map_.end();++it_expend){
		su->add_melting_prop(it_expend->first,it_expend->second);
	}

	if (baptize_money != 0){
		su->add_money(TAEL,baptize_money);
	}

	if (baptize_item_list_.size() != 0){
		std::map<uint32,uint32>::iterator itr = baptize_item_list_.begin();
		for (;itr != baptize_item_list_.end(); ++itr){
			su->add_melting_prop(itr->first,itr->second);
		}
	}

	su->mutable_attr()->set_val(MELTING,set_melting);

	su->send_client_change_attr();
	su->send_client_change_props_msg();
	client::melting_equip_rsp melting_result;
	melting_result.set_result_type(melting_equip_rsp_result_success);
	su->send_client(melting_result);

	amtsvr_billlog(ss->ctx, "%u|%s|%u", su->get_role_id(), req->GetTypeName().c_str(), req->melting_type());

	return NULL;
}

// 铸造
void* on_pb_cast_equip_req(client::cast_equip_req* req, void* ud, size_t* rsp_len)
{
	return NULL;

	struct game_service_t* ss = (struct game_service_t*)ud;
	scene_user* su = ss->sim->get_user(req->svr_head().role_id());
	if (su == NULL) {
		return NULL;
	}

	// 获取物品的基础数据
	const config_props* item_base_info = CONFMGR->get_config_props_mgr().get_config_props(req->equip_id());
	// 获取铸造数据
	const config_cast_equip* cast_info = CONFMGR->get_config_cast_equip_mgr().get_config_cast_equip(req->equip_id());
	
	player_role_props& prp = su->get_player_role().get_role_props();

	props_parser& depot_pack = prp.get_depot();

	if (item_base_info == NULL || cast_info == NULL){
		return NULL;
	}
	// 检测背包空间是否足够
	if (depot_pack.get_free_space() < 1)
	{
		return NULL;
	}

	bool check_level = false;
	int player_level = static_cast<int>(su->get_attr().get_val(LEVEL));
	int player_melting = static_cast<int>(su->get_attr().get_val(MELTING));
	const std::map<int ,int>& show_level_map_ = cast_info->get_show_level();
	std::map<int ,int>::const_iterator itr = show_level_map_.begin();
	for (;itr != show_level_map_.end();++itr){
		if (player_level >= itr->first && player_level < itr->second){
			check_level = true;
			break;
		}
	}

	// 检测等级与熔炼值是否满足条件
	if (!check_level || player_melting < cast_info->get_need_melting()){
		return NULL;
	}
	// 分母
	int denominator = 0;
	const std::multimap<int,cast_equip>& cast_item_map_ = cast_info->get_item_data();
	std::multimap<int,cast_equip>::const_iterator it = cast_item_map_.begin();
	for (;it != cast_item_map_.end(); ++it)
	{
		denominator += it->second.probability;
	}

	int rand_value = static_cast<int>(mtrandom::rand_mid32(0,denominator));
	client::props_info cast_item;
	int cur_melting = 0;
	//衰减数
	int attenuation = 0;
	cast_equip_rsp cast_result;
	// 反遍历
	std::multimap<int,cast_equip>::const_iterator it_1 = cast_item_map_.begin();
	for (;it_1 != cast_item_map_.end();++it_1){
		attenuation += it_1->second.probability;
		if (rand_value >= attenuation){
			continue;
		}

		depot_pack.makeup_props(it_1->second.equip_id,it_1->second.cast_count,item_base_info->get_quality(),item_base_info->get_bind_type(),*item_base_info,cast_item);

		cast_item.set_level(it_1->second.improve_level);
		const config_upequip* upequip_info =  CONFMGR->get_config_upequip_mgr().get_config_upequip(it_1->second.equip_id,cast_item.level());
		if (upequip_info == NULL){
			return NULL;
		}

		if (depot_pack.strengthen_cast_equip(cast_item,upequip_info) == 0){
			// 减去熔炼值
			cur_melting = player_melting - cast_info->get_need_melting();
			su->mutable_attr()->set_val(MELTING,cur_melting);
			depot_pack.add_cast_props(cast_item);
			cast_result.set_result_type(cast_equip_rsp_result_success);
			cast_result.mutable_pi()->CopyFrom(cast_item);
			break;
		}
	}

	// 下发物品改变
	su->send_client_change_props_msg();
	su->send_client(cast_result);
	su->send_client_change_attr();
	return NULL;
}
// 寻宝
void* on_pb_treasure_req(client::treasure_req* req, void* ud, size_t* rsp_len)
{
	return NULL;
	//struct game_service_t* ss = (struct game_service_t*)ud;
	//scene_user* su = ss->sim->get_user(req->svr_head().role_id());
	//if (su == NULL) {
	//	return NULL;
	//}

	//client::treasure_rsp rsp_msg;

	//// 判断熔炼值
	//uint32 one_count = static_cast<uint32>(CONFMGR->get_basic_value(10072));
	//uint32 total_need = one_count * req->treasure_count();
	//uint32 cur_melting = su->mutable_attr()->get_val(MELTING);
	//if (cur_melting < total_need){
	//	return NULL;
	//}
	//props_parser& main_pack = su->get_player_role().get_role_props().get_pack();
	//if (main_pack.get_free_space() < req->treasure_count()){
	//	rsp_msg.set_result_type(2);
	//	su->send_client(rsp_msg);
	//	return NULL;
	//}
	//
	//player_role_treasure& treasure_obj = su->get_player_role().get_role_treasures();
	//std::vector<uint32> treasure_slot_vec;
	//treasure_slot_vec.clear();
	//
	//for (uint32 i = 0; i < req->treasure_count(); ++i){
	//	uint32 base_denominator = 0;
	//	const std::map<uint32, config_treasure*>& config_treasure_map_ = CONFMGR->get_config_treasure_mgr().get_config_treasure_map();
	//	std::map<uint32, config_treasure*>::const_iterator itr = config_treasure_map_.begin();
	//	std::map<uint32,config_treasure*> new_config_treasure_map_;
	//	new_config_treasure_map_.clear();
	//	// 每次寻宝都需要重算一下分母
	//	for (;itr != config_treasure_map_.end(); ++itr){
	//		// 检测次数是否达到限制，算出分母
	//		client::treasure_db_info info;
	//		//if (!treasure_obj.get_treasure_info(itr->first,info)){
	//		//	continue;
	//		//}
	//		if (!treasure_obj.get_treasure_info(itr->first,info)){
	//			treasure_obj.create_treasure_info(itr->first,info);
	//		}
	//		if (info.item_count() < itr->second->get_obtain_top_limit()){
	//			base_denominator += itr->second->get_probability();
	//			new_config_treasure_map_.insert(std::pair<uint32, config_treasure*>(itr->first, itr->second));
	//		}
	//	}
	//	// 得出随机值
	//	int rand_value = static_cast<int>(mtrandom::rand_mid32(0,base_denominator));
	//	//衰减数
	//	int attenuation = 0;
	//	std::map<uint32, config_treasure*>::const_iterator it_1 = new_config_treasure_map_.begin();
	//	if (new_config_treasure_map_.size() == 0){
	//		break;
	//	}
	//	uint32 create_item_id = 0;
	//	uint32 cur_slot_id = 0;
	//	for (;it_1 != new_config_treasure_map_.end();++it_1){
	//		attenuation += it_1->second->get_probability();
	//		if (rand_value > attenuation){
	//			continue;
	//		}
	//		client::treasure_db_info cur_info;
	//		if (!treasure_obj.get_treasure_info(it_1->first,cur_info)){
	//			continue;
	//		}
	//		// 用格子取出当前物品的寻宝数据
	//		const config_treasure* treasure_base_info = CONFMGR->get_config_treasure_mgr().get_config_treasure(it_1->first);
	//		if (treasure_base_info == NULL){
	//			continue;
	//		}

	//		if (cur_info.item_count() >= treasure_base_info->get_obtain_top_limit()){
	//			continue;
	//		}
	//		create_item_id = treasure_base_info->get_item_id();
	//		cur_slot_id = treasure_base_info->get_item_slot();
	//		treasure_slot_vec.push_back(cur_slot_id);
	//		break;
	//	}
	//	// 再检测啊~我艹！！
	//	const config_props* item_base_info = CONFMGR->get_config_props_mgr().get_config_props(create_item_id);
	//	if (item_base_info == NULL){
	//		continue;
	//	}
	//	// 终于寻宝成功了~屌丝

	//	su->add_props(item_base_info->get_item_id(),1);

	//	// 记录当前物品的次数
	//	client::treasure_db_info info;
	//	if (!treasure_obj.get_treasure_info(cur_slot_id,info)){
	//		continue;
	//	}
	//	info.set_item_count(info.item_count() +1);
	//	treasure_obj.change_treasure_info(info);
	//}

	// 统计日常
	//su->modify_treasure_target(req->treasure_count());
	//if (treasure_slot_vec.size() == 0){
	//	rsp_msg.set_result_type(1);
	//}
	//else{
	//	uint32 real_need_melting = treasure_slot_vec.size() * one_count;
	//	uint32 last_melting = cur_melting - real_need_melting;
	//	su->mutable_attr()->set_val(MELTING,last_melting);

	//	su->send_client_change_props_msg();

	//	su->send_client_change_attr();
	//	rsp_msg.set_result_type(0);
	//}

	//for (uint32 z = 0; z < treasure_slot_vec.size(); ++z){
	//	rsp_msg.add_treasure_lost(treasure_slot_vec[z]);
	//}
	//su->send_client(rsp_msg);
	return NULL;
}
// 洗炼装备
void* on_pb_baptize_req(client::baptize_req* req, void* ud, size_t* rsp_len)
{
	PB_FUNC_GUARD(req);
	player_role_props& prp = psu->get_player_role().get_role_props();
	// 获取洗练的物品所在的背包
	props_parser* pp = prp.get_pos_parser(req->grid_index());
	if (pp == NULL){
		return NULL;
	}

	client::baptize_rsp rsp_msg;
	rsp_msg.set_grid_index(req->grid_index());
	bool is_success = false;

	is_success = baptize_rule(req->baptize_type(),psu,req->grid_index());

	if (is_success){
		rsp_msg.set_result_type(BAPTIZE_RESULT_TYPE_SUCESS);
	}else{
		rsp_msg.set_result_type(BAPTIZE_RESULT_TYPE_FAILD);
	}

	prp.change_flag(req->grid_index());

	psu->send_client(rsp_msg);

	CHECK_TARGET_PROGRESS(psu, EQUIP_XILIAN);

	//amtsvr_billlog(ss->ctx, "%u|%s|%u|%u", psu->get_role_id(), req->GetTypeName().c_str(), req->baptize_type(),req->grid_index());

	return NULL;
}


// 洗练保存
void* on_pb_baptize_save_req(client::baptize_save_req* req, void* ud, size_t* rsp_len)
{
	PB_FUNC_GUARD(req);

	client::baptize_save_rsp rsp_msg;
	rsp_msg.set_grid_index(req->grid_index());
	player_role_props& prp = psu->get_player_role().get_role_props();
	// 获取洗练的物品所在的背包
	props_parser* pp = prp.get_pos_parser(req->grid_index());
	if (pp == NULL){
		return NULL;
	}

	// 获得当前格子物品的数据
	grid_role_props* grp = pp->mutable_props(req->grid_index());
	if (grp == NULL){
		return NULL;
	}
	
	if (grp->last_attr_size() == 0){
		rsp_msg.set_result_type(1);
		psu->send_client(rsp_msg);
		return NULL;
	}

	if (grp->attr_size() != grp->last_attr_size()){
		rsp_msg.set_result_type(1);
		psu->send_client(rsp_msg);
		return NULL;
	}
	// 记录属性前值后值：
	std::multimap<int,client::record_attr> record_map_;
	record_map_.clear();
	for (int o = 0; o < grp->attr_size(); ++o){
		const client::attribute& attr = grp->attr(o);
		const client::attribute& l_attr = grp->last_attr(o);
		client::record_attr info;
		info.set_old_value(attr.value());
		info.set_new_value(l_attr.value());
		record_map_.insert(pair<int,record_attr>(attr.type(),info));
	}

	grp->mutable_attr()->CopyFrom(grp->last_attr());


	grp->clear_last_attr();
	prp.change_flag(req->grid_index());

	if (req->grid_index() <= EQUIP_SHOE){
		psu->calculate_attr();
		psu->send_client_change_attr();
	} else {
		uint32 pet_id = req->grid_index() / 10;
		psu->pet_attr_change(pet_id,PET_ATTR_CHANGE_TYPE_EQUIP);
	}

	psu->send_client_change_props_msg();
	rsp_msg.set_result_type(0);
	psu->send_client(rsp_msg);

	uint32 total_power = psu->cal_equip_reward_power();
	if (total_power > psu->get_attr(EQUIP_POWER)){
		psu->mutable_attr()->set_val(EQUIP_POWER,total_power);
	}

	psu->update_equip_power_rank(REWARD_TYPE_POWER,total_power);
	////////////////////////////////////////log/////////////////////////////////////////////////////////
	//保存装备XX，属性XX，前值XX/后值XX；属性XX，前值XX/后值XX；属性XX，前值XX/后值XX；洗出属性XX，值XX；
	string c = "";
	char tc[1024] = {0};
	std::multimap<int,record_attr>::iterator it_a = record_map_.begin();
	for (; it_a != record_map_.end(); ++it_a){
		memset(tc,0,sizeof(tc));
		string name = get_attr_name(it_a->first);
		snprintf(tc,sizeof(tc),"|属性:%s,前值:%u/后值%u",name.c_str(),it_a->second.old_value(),it_a->second.new_value());
		c += tc;
	}

	string b = "";
	char tb[1024] = {0};
	std::multimap<int,record_attr>::iterator it_b = record_map_.begin();
	for (; it_b != record_map_.end(); ++it_b){
		memset(tb,0,sizeof(tb));
		string name = get_attr_name(it_b->first);
		snprintf(tb,sizeof(tb),"|洗出属性:%s,值:%u",name.c_str(),it_b->second.new_value());
		b += tb;
	}

	const config_props* base_info = CONFMGR->get_config_props(grp->props_id());
	if (base_info == NULL){
		return NULL;
	}

	amtsvr_billlog(ss->ctx, "%u|%s|%s%s%s", psu->get_role_id(), req->GetTypeName().c_str(), base_info->get_item_name().c_str(),c.c_str(),b.c_str());
	
	return NULL;
}

void* on_pb_collect_explore_req(client::collect_explore_req* req, void* ud, size_t* rsp_len)
{
	PB_FUNC_GUARD(req);

	client::collect_explore_rsp rsp_msg;

	int ret = psu->get_player_role().get_role_props().move_explore_to_bag();

	if (ret == 0){
		rsp_msg.set_result_type(COLLECT_RESULT_TYPE_SUCCESS);
	}else{
		rsp_msg.set_result_type(COLLECT_RESULT_TYPE_NOT_ENOUGH_SPACE);
	}

	psu->send_client_change_props_msg();

	psu->send_client(rsp_msg);

	amtsvr_billlog(ss->ctx, "%u|%s|%u", psu->get_role_id(), req->GetTypeName().c_str(), req->collect_type());

	return NULL;
}
// 黑市商店列表请求
void* on_pb_black_shop_req(client::black_shop_req* req, void* ud, size_t* rsp_len)
{
	PB_FUNC_GUARD(req);
	client::black_shop_rsp rsp_msg;

	psu->on_pb_black_shop_req(req,rsp_msg);

	psu->send_client(rsp_msg);

	amtsvr_billlog(ss->ctx, "%u|%s|%u", psu->get_role_id(), req->GetTypeName().c_str(), req->unixtime());

	return NULL;
}
// 手动刷新
void* on_pb_black_shop_reflesh_req(client::black_shop_reflesh_req* req, void* ud, size_t* rsp_len)
{
	PB_FUNC_GUARD(req);
	client::black_shop_reflesh_rsp rsp_msg;

	psu->on_pb_black_shop_reflesh_req(req,rsp_msg);
	

	psu->send_client(rsp_msg);

	amtsvr_billlog(ss->ctx, "%u|%s|%u", psu->get_role_id(), req->GetTypeName().c_str(), req->unixtime());

	return NULL;
}
// 购买
void* on_pb_black_shop_buy_req(client::black_shop_buy_req* req, void* ud, size_t* rsp_len)
{
	PB_FUNC_GUARD(req);
	client::black_shop_buy_rsp rsp_msg;

	if (!psu->is_can_add_props()){
		return NULL;
	}

	psu->on_pb_black_shop_buy_req(req,rsp_msg);

	psu->send_client(rsp_msg);

	amtsvr_billlog(ss->ctx, "%u|%s|%u", psu->get_role_id(), req->GetTypeName().c_str(), req->item_slot());

	return NULL;
}
