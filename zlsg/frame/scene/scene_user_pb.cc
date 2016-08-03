#include <math.h>
#include "config_map.h"
#include "config_props.h"
#include "config_mall.h"
#include "config_gift.h"
#ifndef __LIB_MINI_SERVER__
#include "config_vip.h"
#include "config_recharge.h"
#endif
#include "config_gem.h"
#include "config_beauty.h"
#include "client.chat_msg.pb.h"
#include "client.mounts.pb.h"
#include "client.rank.pb.h"
#include "player_role_mounts.h"
#include "player_role_props.h"
#include "player_role_set.h"
#include "player_role_statis_mgr.h"
#include "player_role_status.h"
#ifndef __LIB_MINI_SERVER__
#include "player_role_mission.h"
#include "player_role_vip.h"
#endif
#include "player_role_black_shop.h"
#include "player_role_cd_cooldown.h"
#include "player_role_treasure.h"
#include "player_role_gem.h"
#include "player_role_bows.h"
#include "player_role.h"
#include "game_map_mgr.h"
#include "scene_pet.h"
#include "scene_inst.h"
#include "player_role_skill_mgr.h"
#ifndef __LIB_MINI_SERVER__
#include "tower_scene_inst.h"
#endif // __LIB_MINI_SERVER__
#include "config_material.h"
#include "scene_user.h"
#include "config_cd_time.h"
#include "config_treasure.h"
#include "config_sign.h"
#include "config_land.h"
#include "config_bows.h"

#define MONTH_VIP 2

bool check_pay(scene_user* su,uint32 shop_id,uint32 item_id,uint32 count);
bool check_limit(scene_user* su,const config_mall* mall_info);
int scene_user::on_pb_buy_goods_npc_shop(client::buy_goods_req* b,client::buy_goods_rsp& rsp)
{
#ifndef __LIB_MINI_SERVER__
	rsp.set_goods_id(b->goods_id());
	rsp.set_goods_count(0);
	const config_props* base_info = CONFMGR->get_config_props_mgr().get_config_props(b->goods_id());
	if (base_info == NULL){
		rsp.set_ret(SBRE_CANT_BUY);
		return -1;
	}

	const config_mall* mall_info = CONFMGR->get_config_mall_mgr().get_config_mall(b->shop_id(),b->goods_id());
	if (mall_info == NULL){
		rsp.set_ret(SBRE_CANT_BUY);
		return -1;
	}

	uint32 l_old_count = get_props_num(b->goods_id());

	if (b->goods_id() == 53295001){
		uint32 now = (uint32)time(NULL);
		if (now < get_player_role().get_player_role_vip().get_month_timeout()){
			rsp.set_ret(SBRE_CANT_BUY);
			rsp.set_vip_day(get_player_role().get_player_role_vip().get_month_timeout()- now);
			return -1;
		}
	}
	// 检测是否已经达到购买上限了
	bool need_record = false;
	uint32 config_limit = mall_info->get_daily_quota();
	if (config_limit != 0){
		need_record = true;
	}

	player_role_treasure& player_treasure = get_player_role().get_role_treasures();
	client::treasure_db_info info;
	if (need_record){
		if (!player_treasure.get_treasure_info(b->goods_id(),info,b->shop_id())){
			player_treasure.create_treasure_info(b->goods_id(),info,mall_info->get_mall_id());
		}

		if (config_limit <= info.item_count()){
			rsp.set_ret(SBRE_LIMIT);
			return -1;
		}
	}

	if (!check_limit(this,mall_info)){
		rsp.set_ret(SBRE_CANT_BUY);
		return -1;
	}

	// 检测购买力
	if (!check_pay(this,b->shop_id(),b->goods_id(),b->num())){
		rsp.set_ret(SBRE_NOMONEY);
		return -1;
	}

	ROLE_ATTRIBUTE pay_type;
	// 给东西
	switch(mall_info->get_pay_type())
	{
	case SHOP_PAY_TYPE_INGOT: pay_type = INGOT; break;
	case SHOP_PAY_TYPE_FAMILY_CON: pay_type = CAN_JOIN_FAMILY; break;
	case SHOP_PAY_TYPE_ARENA: pay_type = POPULARITY; break;
	case SHOP_PAY_TYPE_ARMS: pay_type = STRATEGY;break;
	case SHOP_PAY_TYPE_COIN: pay_type = TAEL; break;
	default: 
		{
			rsp.set_ret(SBRE_LIMIT);
			return -1;
		}
		break;
	}

	uint32 old_value = 0;

	if (pay_type ==  INGOT){
		old_value = get_attr(INGOT);
	}

	if (add_props(b->goods_id(),b->num()) < 0 ){
		rsp.set_ret(SBRE_CANT_BUY);
		return -1;
	}

	uint32 pay_value = mall_info->get_price_value() * b->num(); 
	rsp.set_goods_count(b->num());
	uint32 l_pay_old = get_attr(pay_type);
	sub_money(pay_type, pay_value);
	uint32 l_pay_new = get_attr(pay_type);
	if (pay_type == INGOT){
		sub_money_log(MC_STORE_SPEND, MC_BUY_PROP, old_value,get_attr(INGOT), b->goods_id(), b->num());
	}

	send_client_change_props_msg();
	send_client_change_attr();
	rsp.set_ret(SBRE_SUCCESS);
	// 记录购买
	if (need_record){
		info.set_item_count(info.item_count() + b->num());
		player_treasure.change_treasure_info(info,b->shop_id());
		rsp.set_last_count(config_limit - info.item_count());
	}
	uint32 l_new_count = get_props_num(b->goods_id());
	// 购买道具ID（道具名）|数量XX|购买前值XX|后值XX|消耗XX货币XX数量|前值XX/后值XX；
	amtsvr_context* ctx = this->get_scene()->get_ctx();
	amtsvr_billlog(ctx, "%u|client.buy_goods_req|%s|%u|%u|%u|%u|%u|%u|%u", get_role_id(),base_info->get_item_name().c_str(),
	b->num(),l_old_count,l_new_count,(uint32)pay_type,pay_value,l_pay_old,l_pay_new);
#endif
    return 0;
}

bool check_pay(scene_user* su,uint32 shop_id,uint32 item_id,uint32 count)
{
	// 获取商品数据
	const config_mall* mall_info = CONFMGR->get_config_mall_mgr().get_config_mall(shop_id,item_id);
	if (mall_info == NULL){
		return false;
	}
	
	uint32 need_pay = mall_info->get_price_value() * count;
	ROLE_ATTRIBUTE pay_type;
	switch(mall_info->get_pay_type()){
	case SHOP_PAY_TYPE_INGOT: pay_type = INGOT; break;// 元宝
	case SHOP_PAY_TYPE_FAMILY_CON: pay_type = CAN_JOIN_FAMILY; break;// 帮贡
	case SHOP_PAY_TYPE_ARENA: pay_type = POPULARITY; break;// 竞技场声望
	case SHOP_PAY_TYPE_COIN: pay_type = TAEL; break; // 铜币
	case SHOP_PAY_TYPE_ARMS: pay_type = STRATEGY; break; // 谋略
	default:return false; break;
	}

	uint32 cur_contr = su->mutable_attr()->get_val(pay_type);
	if (cur_contr < need_pay){
		return false;
	}

	return true;
}

bool check_limit(scene_user* su,const config_mall* mall_info)
{
	const std::map<int, int>& limit_map_ = mall_info->get_extra_limit();
	if (limit_map_.size() == 0){
		return true;
	}
	uint32 limit_type = 0;
	uint32 limit_value =0;
	std::map<int, int>::const_iterator it = limit_map_.begin();
	for (;it != limit_map_.end(); ++it){
		limit_type = static_cast<uint32>(it->first);
		limit_value = static_cast<uint32>(it->second);
	}

	switch(mall_info->get_mall_id())
	{
	case SHOP_PAY_TYPE_INGOT:
		{
			

		}
		break;
	case SHOP_PAY_TYPE_FAMILY_CON:
		{
			if (su->get_attr(FAMILY_ID) == 0){
				return false;
			}

			if (limit_type == SHOP_LIMIT_TYPE_FAMILY){
				uint32 family_level = su->get_attr(FAMILY_LEVEL);
				if (family_level < limit_value || family_level == 0){
					return false;
				}
			}
		}
		break;
	case SHOP_PAY_TYPE_ARENA:
		{

		}
		break;
	case SHOP_PAY_TYPE_ARMS:
		{

		}
		break;
	case SHOP_PAY_TYPE_COIN:
		{

		}
		break;
	default:
		return false;
		break;
	}

	return true;
}

int scene_user::fill_spa_trans_jump(client::jump_user& ju)
{
    if (_in_scene == NULL){
        return -1;
    }

//     const mysql::tb_hot* th = NULL;
//     if (th == NULL){
//         return -2;
//     }
// 
//     uint32 trans_id = th->trans_id();
//     const mysql::tb_trans* trans_cfg = CONFMGR->get_trans(trans_id);
//     if (trans_cfg == NULL){
//         return -3;
//     }
// 
//     // 保存跳转副本前的地图位置
//     save_formal_pos();
// 
//     uintptr_t ptr = (uintptr_t)this;
//     ju.set_ptr((uint64)ptr);
//     ju.set_map_id(trans_cfg->trans_map_id());
//     ju.set_scene_id(trans_cfg->trans_scene_id());
//     ju.set_trans_id(trans_id);
//     ju.set_x(trans_cfg->trans_x());
//     ju.set_y(trans_cfg->trans_y());
//     ju.set_z(trans_cfg->trans_z());
//     ju.set_same_scene(is_same_map_and_scene(ju.map_id(), ju.scene_id()));
    return 0;
}

void scene_user::send_sys_msg(uint32 msg_id, char* cmd, uint32 type/* = 0*/)
{
    client::client_chat_msg ccm;
    client::chat_msg* cm = ccm.mutable_cm();
    cm->set_channel(client::chat_msg_CHANNEL_SYSTEM);
    cm->set_src_rold_id(msg_id);
    cm->set_content(cmd);
    cm->set_dst_role_id(type);
    send_center(ccm);
}
                                
void scene_user::sys_msg_enhance(uint32 goods_id, uint32 enhance_lvl)
{
    if (goods_id == 0 || enhance_lvl < 30) {
        return ;
    } else if (enhance_lvl < 50 && enhance_lvl % 10 != 0) {
        return ;
    } else if (enhance_lvl % 5 != 0) {
        return ;
    }
    
    char cmd[128] = {0};
    snprintf(cmd, sizeof(cmd), "%s|%u|%u|%u", get_role_name(), get_role_id(), goods_id, enhance_lvl);
    send_sys_msg(60, cmd);
}

void scene_user::sys_msg_stone(uint32 goods_id, uint32 num)
{
    if (goods_id == 0 || num == 0) {
        return ;
    }
    
    char cmd[128] = {0};
    snprintf(cmd, sizeof(cmd), "%s|%u|%u|%u", get_role_name(), get_role_id(), num, goods_id);
    send_sys_msg(65, cmd);
}

void scene_user::sys_msg_imquality(uint32 material_id, uint32 goods_id)
{
    if (material_id == 0 || goods_id == 0) {
        return ;
    }
    
    char cmd[128] = {0};
    snprintf(cmd, sizeof(cmd), "%s|%u|%u|%u", get_role_name(), get_role_id(), material_id, goods_id);
    send_sys_msg(92, cmd);
}

void scene_user::sys_msg_be_kill(scene_role* src_role)
{
    if (src_role == NULL || _in_scene == NULL) {
        return ;
    }

    uint32 scene_id = _in_scene->get_scene_id();
    uint32 x = _scene_pos.x;
    uint32 y = _scene_pos.y;
    char cmd[128] = {0};
    snprintf(cmd, sizeof(cmd), "%s|%u|%u|%u|%u|%s|%u", get_role_name(), get_role_id(), scene_id, x, y, src_role->get_role_name(), src_role->get_role_id());
    send_sys_msg(68, cmd, SYS_MSG_TO_FRIEND);
}

void scene_user::sys_msg_kill()
{
    if (_in_scene == NULL){
        return;
    }

    uint32 scene_id = _in_scene->get_scene_id();
    uint32 x = _scene_pos.x;
    uint32 y = _scene_pos.y;
    char cmd[128] = {0};
    snprintf(cmd, sizeof(cmd), "%s|%u|%u|%u|%u", get_role_name(), get_role_id(), scene_id, x, y);
    send_sys_msg(70, cmd);
}

void scene_user::sys_msg_explore(uint32 place_id, uint32 goods_id)
{
    char cmd[128] = {0};
    snprintf(cmd, sizeof(cmd), "%s|%u|%u|%u", get_role_name(), get_role_id(), place_id, goods_id);
    send_sys_msg(69, cmd);
}

void scene_user::sys_msg_pick(uint32 npc_id, uint32 goods_id)
{
    char cmd[128] = {0};
    snprintf(cmd, sizeof(cmd), "%s|%u|%u|%u",  get_role_name(), get_role_id(), npc_id, goods_id);
    send_sys_msg(90, cmd);
}

void scene_user::sys_msg_pet(uint32 pet_id)
{
    char cmd[128] = {0};
    snprintf(cmd, sizeof(cmd), "%s|%u|%u", get_role_name(), get_role_id(), pet_id);
    send_sys_msg(84, cmd);
}

void scene_user::sys_msg_advance_pet(uint32 pet_id, uint32 lvl)
{
    char cmd[128] = {0};
    snprintf(cmd, sizeof(cmd), "%s|%u|%u|%u",  get_role_name(), get_role_id(), pet_id, lvl);
    send_sys_msg(61, cmd);
}

void scene_user::sys_msg_half_year_vip()
{
    char cmd[128] = {0};
    snprintf(cmd, sizeof(cmd), "%s|%u",  get_role_name(), get_role_id());
    send_sys_msg(104, cmd);
}

void scene_user::sys_msg_hour_vip()
{
    char cmd[128] = {0};
    snprintf(cmd, sizeof(cmd), "%s|%u",  get_role_name(), get_role_id());
    send_sys_msg(105, cmd);
}

void scene_user::sys_msg_wing(uint32 wing_id)
{
    char cmd[128] = {0};
    snprintf(cmd, sizeof(cmd), "%s|%u|%u", get_role_name(), get_role_id(), wing_id);
    send_sys_msg(107, cmd);
}

void scene_user::sys_msg_upgrade_wing(uint32 wing_id)
{
    char cmd[128] = {0};
    snprintf(cmd, sizeof(cmd), "%s|%u|%u",  get_role_name(), get_role_id(), wing_id);
    send_sys_msg(108, cmd);
}

void scene_user::sys_msg_target(uint32 id)
{
    char cmd[128] = {0};
    snprintf(cmd, sizeof(cmd), "%s|%u",  get_role_name(), get_role_id());
    send_sys_msg(id, cmd);
}

void scene_user::sys_msg_label(uint32 raid_id, uint32 label)
{
    char cmd[128] = {0};
    snprintf(cmd, sizeof(cmd), "%s|%u|%u|%u",  get_role_name(), get_role_id(), raid_id, label);
    send_sys_msg(306, cmd);
}

void scene_user::sys_msg_shop(uint32 num, uint32 goods_id)
{
    char cmd[128] = {0};
    snprintf(cmd, sizeof(cmd), "%s|%u|%u|%u",  get_role_name(), get_role_id(), num, goods_id);
    send_sys_msg(321, cmd);
}

void scene_user::sys_msg_convoy(scene_role* sr/* = NULL*/)
{
    uint32 msg_id = 0;
    char cmd[128] = {0};
    if (sr == NULL) {
        snprintf(cmd, sizeof(cmd), "%s|%u",  get_role_name(), get_role_id());
        msg_id = 117;
    } else if (sr->get_role_type() == RTE_USER) {
        msg_id = 119;
        snprintf(cmd, sizeof(cmd), "%s|%u|%s|%u",  sr->get_role_name(), sr->get_role_id(), get_role_name(), get_role_id());
    } else {
        msg_id = 120;
        snprintf(cmd, sizeof(cmd), "%s|%u|%u",  get_role_name(), get_role_id(), sr->get_role_id());
    }

    send_sys_msg(msg_id, cmd);
}

void scene_user::sys_msg_aircraft(uint32 id)
{
    char cmd[128] = {0};
    snprintf(cmd, sizeof(cmd), "%s|%u|%u",  get_role_name(), get_role_id(), id);
    send_sys_msg(322, cmd);
}

void scene_user::sys_msg_score(uint32 num)
{
    char cmd[128] = {0};
    snprintf(cmd, sizeof(cmd), "%s|%u|%u",  get_role_name(), get_role_id(), num);
    send_sys_msg(323, cmd);
}


uint32 scene_user::get_spa_action_times()
{
    return _spr->get_role_status().get_spa_action_times();
}

client::SPA_ACTION_RET_E scene_user::spa_action_req(uint32& action_times, bool& add_other_exp)
{
    if (_in_scene == NULL){
        return client::SARE_OUT;
    }

    action_times = get_spa_action_times();
    if (action_times < 1){
        return client::SARE_TIMES_LIMIT;
    }

    add_other_exp = true;
    --action_times;
    //add_exp(200 + attr.level() * 80);
    _spr->get_role_status().sub_action_times(1);
    send_client_change_attr();

    return client::SARE_SUCCESS;
}

void scene_user::spa_action_by_other()
{
    add_exp(200 + _attr.level() * 80);
    send_client_change_attr();
}

void scene_user::on_rob_convoy(uint32 quality)
{
}


ITEM_OPERATE_TYPE scene_user::props_use_effect(uint32 grid_index,uint32 props_count,client::role_data rd)
{
	props_parser* pp = get_player_role().get_role_props().get_pos_parser(grid_index);
	if (pp == NULL){
		return ITEM_OPERATE_TYPE_OBJECT_NULL;
	}
	// 获得当前格子物品的数据
	grid_role_props* grp = pp->mutable_props(grid_index);

	if (grp == NULL){
		return ITEM_OPERATE_TYPE_OBJECT_NULL;
	}
	const config_props* base_info = CONFMGR->get_config_props_mgr().get_config_props(grp->props_id());
	if (base_info == NULL){
		return ITEM_OPERATE_TYPE_CONFIG_ERRO;
	}
	// 先区分主要类型，再区分子类型去确定不同物品的使用操作
	switch(base_info->get_item_type())
	{
	case PT_MEDICINE:
		return change_role_attr(base_info,props_count);
		break;
	case PT_USE:
		return use_item(base_info,props_count,rd);
		break;
	case PT_COMPOUND:
		return compound_item(base_info,props_count);
		break;
	default:
		return ITEM_OPERATE_TYPE_USE_LIMIT;
		break;
	}
	
	return ITEM_OPERATE_TYPE_SUCCESS;
}
// 传送卷轴
ITEM_OPERATE_TYPE scene_user::chang_pos(const config_props* base_info,uint32 count)	
{
	
	



	return ITEM_OPERATE_TYPE_SUCCESS;
}
// 礼包类物品
ITEM_OPERATE_TYPE scene_user::use_gift(const config_props* base_info,uint32 count)
{
	const config_gift* gift_info = CONFMGR->get_config_gift_mgr().get_config_gift(base_info->get_item_id());
	if (gift_info == NULL){
		return ITEM_OPERATE_TYPE_CONFIG_ERRO;
	}
	uint32 drop_id = gift_info->get_item_list();
	client::use_props_show_rsp rsp_msg;
	rsp_msg.set_rsp_type(1);
	vector<client::props_info> item_vec;
	item_vec.clear();
	std::multimap<uint32,client::props_info> show_map_;
	show_map_.clear();
	for (uint32 i = 0; i < count; ++i){
		add_drop_props(drop_id,item_vec);
		for (uint32 j = 0; j < item_vec.size(); ++j){
			std::multimap<uint32,client::props_info>::iterator it = show_map_.find(item_vec[j].props_id());
			if (it != show_map_.end()){
				const config_props* base_info = CONFMGR->get_config_props(item_vec[j].props_id());
				if (base_info != NULL){
					if (base_info->get_item_type() != PT_EQUIP && base_info->get_item_type() != PT_PET_EQUIP){
						it->second.set_count(it->second.count() + item_vec[j].count());
					}else{
						// 装备
						show_map_.insert(pair<uint32,client::props_info>(item_vec[j].props_id(),item_vec[j]));
					}
				}
			}else{
				show_map_.insert(pair<uint32,client::props_info>(item_vec[j].props_id(),item_vec[j]));
			}
		}

		item_vec.clear();
	}

	std::multimap<uint32,client::props_info>::iterator itr = show_map_.begin();
	for (; itr != show_map_.end();++itr){
		client::props_info* info = rsp_msg.add_show_info();
		info->CopyFrom(itr->second);
	}

	send_client(rsp_msg);
	return ITEM_OPERATE_TYPE_SUCCESS;
}

void fill_mounts_msg(const mysql::tb_role_mounts& mounts, client::mounts_msg& mmsg);
void fill_bows_info(const mysql::tb_role_bows& bows, client::bows_info& pmi);
// BUFF类物品
ITEM_OPERATE_TYPE scene_user::use_item(const config_props* base_info, uint32 count, client::role_data rd)
{
#ifndef __LIB_MINI_SERVER__
	//ITEM_OPERATE_TYPE erro_type = ITEM_OPERATE_TYPE_SUCCESS;
	// 日志记录参数：
	uint32 old_level = 0;
	uint32 old_count = 0;
	uint32 new_count = 0;
	uint32 old_exp = 0;
	uint32 add_exp = 0;
	amtsvr_context* ctx = this->get_scene()->get_ctx();
	switch(base_info->get_item_sub_type())
	{
	case SUB_TYPE_ACQUIRE_BUFF:
		{
			const std::vector<int>&  trigger_vec = base_info->get_trigger_list();
			if (trigger_vec.size() != 0){
				for (size_t i = 0; i < trigger_vec.size(); i++){
					if (add_effect(static_cast<uint32>(trigger_vec[i])) != 0){
						return ITEM_OPERATE_TYPE_CONFIG_ERRO;// 配置表错误
					}
				}
			}
		}
		break;
	case SUB_TYPE_OPEN_GIFT:
		{
			return use_gift(base_info,count);
		}
		break;
	case SUB_TYPE_MOUNT_UPGRATE:// 坐骑升级使用效果
		{
			player_role_mounts& mount_obj = get_player_role().get_mounts();
			mysql::tb_role_mounts* pdb = mount_obj.mutable_db();
			if (pdb == NULL || pdb->mount_id() == 0) {
				return ITEM_OPERATE_TYPE_OBJECT_NULL;
			}
			const std::vector<int>& tri_vec = base_info->get_trigger_list();
			if (tri_vec.size() == 0){
				return ITEM_OPERATE_TYPE_CONFIG_ERRO;
			}

			old_level = pdb->mount_level();
			old_count = get_props_num(base_info->get_item_id());
			new_count = old_count - count;
			old_exp = pdb->mount_exp();
			uint32 player_level = mutable_attr()->get_val(LEVEL);
			add_exp = static_cast<uint32>(tri_vec[0] * count);
			mount_obj.upgrate_mount_level(player_level,add_exp);
		
			const mysql::tb_role_mounts& db_info = mount_obj.get_db();
			client::mounts_level_rsp rsp_msg;
			client::mounts_info* pmi = rsp_msg.add_mi();
			pmi->set_mount_id(db_info.mount_id());
			pmi->set_advance_time(db_info.advance_time());
			pmi->set_advance_bless(db_info.advance_bless());
			pmi->set_skill_upgrade_num(db_info.skill_upgrade_num());
			pmi->set_mount_level(db_info.mount_level());
			pmi->set_mount_exp(db_info.mount_exp());
			pmi->set_mount_power(db_info.mount_power());
			send_client(rsp_msg);
			// 查询数据更新
			update_mount_info(db_info.mount_level(),db_info.mount_power());

			calculate_attr();
			// 统计坐骑等级
			CHECK_TARGET_PROGRESS(this, MOUNT_UPGRADE, db_info.mount_level());
			send_client_change_attr();
			// 消耗物品ID（物品名字），数量XX（道具前值XX/道具后值XX）；获得经验XX，经验前值XX/经验后值XX；坐骑等级X，前值XX/后值XX；
			amtsvr_billlog(ctx, "%u|client.use_props_req|%s|%u|%u|%u|%u|%u|%u|%u|%u|%u|%u",get_role_id(),base_info->get_item_name().c_str(),
			old_count, new_count,add_exp,old_exp,db_info.mount_exp(),db_info.mount_level(),old_level,db_info.mount_level(),PT_USE,SUB_TYPE_MOUNT_UPGRATE);
		}
		break;
	case SUB_TYPE_BOW_UPGRATE:// 弓箭升级符
		{
			player_role_bows& player_bow = get_player_role().get_player_role_bows();
			mysql::tb_role_bows* pdb = player_bow.mutable_db();
			if (pdb == NULL || pdb->bow_id() == 0) {
				return ITEM_OPERATE_TYPE_OBJECT_NULL;
			}
			const std::vector<int>& tri_vec = base_info->get_trigger_list();
			if (tri_vec.size() == 0){
				return ITEM_OPERATE_TYPE_CONFIG_ERRO;
			}

			uint32 player_level = mutable_attr()->get_val(LEVEL);
			uint32 add_exp = static_cast<uint32>(tri_vec[0] * count);
			player_bow.upgrate_bow_level(player_level,add_exp);

			const mysql::tb_role_bows& db_info = player_bow.get_db();
			client::bows_level_rsp rsp_msg;
			client::bows_info* pmi = rsp_msg.add_mi();
			fill_bows_info(db_info,*pmi);
			send_client(rsp_msg);

			calculate_attr();
			// 更新到排行榜
			update_bows_rank(db_info.bow_id(), db_info.bow_level());

			send_client_change_attr();
		}
		break;
	case SUB_TYPE_PET_UPGRATE: // 美人经验药水
		{
			if (rd.pet_id() == 0){
				return ITEM_OPERATE_TYPE_OBJECT_NULL;
			}
			player_role_pet& player_pet = get_player_role().get_pets();
			const std::vector<int>& tri_vec = base_info->get_trigger_list();
			if (tri_vec.size() == 0){
				return ITEM_OPERATE_TYPE_CONFIG_ERRO;
			}
			uint32 player_level = mutable_attr()->get_val(LEVEL);
			client::pet_db_info cur_pet_info;
			if (!player_pet.get_pet_info(static_cast<uint32>(rd.pet_id()),cur_pet_info)){
				return ITEM_OPERATE_TYPE_OBJECT_NULL;
			}
			
			if (cur_pet_info.pet_level() >= 100){
				return ITEM_OPERATE_TYPE_PET_LEVEL_HAS_MAX;
			}

			old_level = cur_pet_info.pet_level();
			old_count = get_props_num(base_info->get_item_id());
			new_count = old_count - count;
			old_exp = cur_pet_info.pet_exp();
			add_exp = static_cast<uint32>(tri_vec[0] * count);
			std::vector<uint32> act_skill_id_vec;
			act_skill_id_vec.clear();
			if (player_pet.upgrate_pet_level(player_level,add_exp,static_cast<uint32>(rd.pet_id()),act_skill_id_vec) != 0){
				return ITEM_OPERATE_TYPE_OBJECT_NULL;
			}

			if (act_skill_id_vec.size() != 0){
				for (size_t i = 0; i < act_skill_id_vec.size(); ++i){
					get_player_role().get_role_skill_mgr().add_skill(get_role_id(),act_skill_id_vec[i]);
					const config_skill* skill_info = CONFMGR->get_config_skill(act_skill_id_vec[i]);
					if (skill_info != NULL){
						skill sk(*skill_info);
						mutable_skill()->add_skill(sk);
					}
				}

			}

			client::pet_db_info pet_info;
			if (!player_pet.get_pet_info(static_cast<uint32>(rd.pet_id()),pet_info)){
				return ITEM_OPERATE_TYPE_OBJECT_NULL;
			}
			// 如果当前宠物是出战状态，需要更新场景中宠物的技能,属性相关
			if (act_skill_id_vec.size() != 0 && pet_info.status() == PET_STATUS_FIGHTING){
				//等级更新，技能更新
				pet_attr_change(pet_info.pet_id(),PET_ATTR_CHANGE_TYPE_LEVEL);
			}
			// 统计美人等级
			CHECK_TARGET_PROGRESS(this, PET_UPGRADE, pet_info.pet_level());

			client::pet_info_rsp rsp_msg;
			client::role_data* role_data = 	rsp_msg.mutable_rd();
			role_data->set_role_id(get_role_id());
			role_data->set_inst_id(get_inst_id());
			role_data->set_role_typ((client::role_data_TYPE)get_role_type());
			client::pet_db_info* info = rsp_msg.add_pet_info();
            info->CopyFrom(pet_info);
            send_client(rsp_msg);

			client::role_skill_msg rsm;
			fill_pets_skill_msg(rsm,pet_info.pet_id(),pet_info.pet_level());
			send_client(rsm);
			const config_beauty* pet_base = CONFMGR->get_config_beauty(pet_info.pet_id(),pet_info.pet_level());
			if (pet_base != NULL){
				// 美人(名字)|消耗物品(物品名字)|前值XX|后值XX|获得经验XX|经验前值XX|后值XX|美人等级X|前值XX|后值XX；
				amtsvr_billlog(ctx, "%u|client.use_props_req|%s|%s|%u|%u|%u|%u|%u|%u|%u|%u|%u|%u",get_role_id(),pet_base->get_general_name().c_str(),base_info->get_item_name().c_str(),
					old_count, new_count,add_exp,old_exp,pet_info.pet_exp(),pet_info.pet_level(),old_level,pet_info.pet_level(),PT_USE,SUB_TYPE_PET_UPGRATE);
			}
		}
		break;
	case SUB_TYPE_BOW_SKILL_STUDY:// 弓箭技能升级
		{
			return bow_skill_study(base_info,count);
		}
		break;
	case SUB_TYPE_DROP_PACK:// 活动礼包
		{
			
			const std::vector<int>& tri_vec = base_info->get_trigger_list();
			if (tri_vec.size() != 0){
				uint32 drop_id = tri_vec[0];
				client::use_props_show_rsp rsp_msg;
				rsp_msg.set_rsp_type(1);
				vector<client::props_info> item_vec;
				item_vec.clear();
				std::multimap<uint32,client::props_info> show_map_;
				show_map_.clear();
				for (uint32 i = 0; i < count; ++i){
					add_drop_props(drop_id,item_vec);
					for (uint32 j = 0; j < item_vec.size(); ++j){
						std::multimap<uint32,client::props_info>::iterator it = show_map_.find(item_vec[j].props_id());
						if (it != show_map_.end()){
							const config_props* base_info = CONFMGR->get_config_props(item_vec[j].props_id());
							if (base_info != NULL){
								if (base_info->get_item_type() != PT_EQUIP && base_info->get_item_type() != PT_PET_EQUIP){
									it->second.set_count(it->second.count() + item_vec[j].count());
								}else{
									// 装备
									show_map_.insert(pair<uint32,client::props_info>(item_vec[j].props_id(),item_vec[j]));
								}
							}
						}else{
							show_map_.insert(pair<uint32,client::props_info>(item_vec[j].props_id(),item_vec[j]));
						}
					}

					item_vec.clear();
				}

				std::multimap<uint32,client::props_info>::iterator itr = show_map_.begin();
				for (; itr != show_map_.end();++itr){
					client::props_info* info = rsp_msg.add_show_info();
					info->CopyFrom(itr->second);
				}

				send_client(rsp_msg);
			}else{
				return ITEM_OPERATE_TYPE_CONFIG_ERRO; 
			}
		}
		break;
	case SUB_TYPE_MONTH: // 月卡
		{
			const std::vector<int>& tri_vec = base_info->get_trigger_list();
			if (tri_vec.size() == 0){
				return ITEM_OPERATE_TYPE_CONFIG_ERRO;
			}

			if (recharge(tri_vec[0]) != 0){
				return ITEM_OPERATE_TYPE_USE_LIMIT;
			}
		}
		break;
	default:
		return ITEM_OPERATE_TYPE_USE_LIMIT;
		break;
	}
#endif // __LIB_MINI_SERVER__
	return ITEM_OPERATE_TYPE_SUCCESS;
}
// 加属性类型物品（药水等）复活呢？
ITEM_OPERATE_TYPE scene_user::change_role_attr(const config_props* base_info,uint32 count)
{
	uint32 add_value = 0;
	if (base_info->get_item_sub_type() != SUB_TYPE_CHANGE_HP_PRE){
		const std::vector<int>& trigger_list = base_info->get_trigger_list();
		if (trigger_list.size() == 0 || trigger_list[0]== 0){
			return ITEM_OPERATE_TYPE_CONFIG_ERRO;
		}

		add_value = static_cast<uint32>(trigger_list[0]);
	}

	switch(base_info->get_item_sub_type())
	{
	case SUB_TYPE_CHANGE_HP_PRE:// 血
		{
			uint32 cur_hp = mutable_attr()->get_val(CUR_HP);
			uint32 max_hp = mutable_attr()->get_val(MAX_HP);
			if (cur_hp >= max_hp){
				return ITEM_OPERATE_TYPE_HP_HAS_MAX;
			}
			else{
				uint32 cal_hp = static_cast<uint32>(cur_hp * (float)((base_info->get_affect_hp() / 100.0f)));
				uint32 add_hp = cur_hp + cal_hp;
				if (add_hp >= max_hp){
					if (base_info->get_exceed_limit() == 1){
						set_money(CUR_HP,max_hp);
						send_client_change_attr();
						return ITEM_OPERATE_TYPE_SUCCESS;
					}
					else{
						return ITEM_OPERATE_TYPE_HP_HAS_MAX;
					}
				}
				else{
					set_money(CUR_HP,add_hp);
					send_client_change_attr();
				}
			}
		}
		break;
	case SUB_TYPE_CHANGE_EXP:// 经验
		{
			add_exp(static_cast<uint64>(add_value));
		}
		break;
	case SUB_TYPE_CHANGE_TAEL: // 铜钱
		{
			add_money(TAEL,add_value);
		}
		break;
	case SUB_TYPE_CHANGE_STRATEGY: // 韬略
		{

			add_money(MILITARY_STRATEGY,add_value * count);
		}
		break;
	case SUB_TYPE_CHANGE_INGOT: // 元宝
		{
			add_money(INGOT,add_value);
		}
		break;
	default:
		return ITEM_OPERATE_TYPE_USE_LIMIT;
		break;
	}

	send_client_change_attr();

	return ITEM_OPERATE_TYPE_SUCCESS;
}
// 装备合成
ITEM_OPERATE_TYPE scene_user::compound_item(const config_props* base_info,uint32 count)
{
	const std::vector<int>&  trigger_vec = base_info->get_trigger_list();
	const config_material* material_info = CONFMGR->get_config_material_mgr().get_config_material(trigger_vec[0]);
	if (material_info == NULL){
		return ITEM_OPERATE_TYPE_CONFIG_ERRO;
	}
	if (trigger_vec.size() != 0){
		const std::map<int,int>& material_list = material_info->get_material_list();
		if (material_list.size() == 0){
			return ITEM_OPERATE_TYPE_CONFIG_ERRO;
		}
		//uint32 real_del_count = 0;
		// 扣物品
		std::map<int,int>::const_iterator it = material_list.begin();
		for (;it != material_list.end(); ++it){
			if (it->first != base_info->get_item_id()){
				return ITEM_OPERATE_TYPE_CONFIG_ERRO;
			}

			if (count < static_cast<uint32>(it->second)){
				return ITEM_OPERATE_TYPE_ITEM_NOT_ENOUGH;
			}

			//real_del_count = it->second;
		}
		
		if (add_props(trigger_vec[0],1) < 0){
			return ITEM_OPERATE_TYPE_SPACE_NOT_ENOUGH;
		}
	}
	else{
		return ITEM_OPERATE_TYPE_CONFIG_ERRO;
	}

	return ITEM_OPERATE_TYPE_SUCCESS;
}

void scene_user::update_lv_rank()
{
	int lv = get_attr().get_val(LEVEL);
	if(lv >= CONFMGR->get_basic_value(10112))
	{
		client::rank_add_data rad;
		rad.set_rt(client::RANK_PERSONAL_LEVEL);
		client::rank_rule *ru = rad.mutable_ru();
		ru->set_role_id(get_role_id());
		ru->set_first(lv);
		ru->set_forth((uint32)time(NULL));
		send_center(rad);
	}
}

void scene_user::update_power_rank()
{
	int power = get_attr().get_val(FIGHTING_POWER);
	if(power >= CONFMGR->get_basic_value(10113))
	{
		client::rank_add_data rad;
		rad.set_rt(client::RANK_PERSONAL_POWER);
		client::rank_rule *ru = rad.mutable_ru();
		ru->set_role_id(get_role_id());
		ru->set_first(power);
		ru->set_forth((uint32)time(NULL));
		send_center(rad);
	}
}

void scene_user::update_mounts_rank(int id, int lv)
{
	if(id >= CONFMGR->get_basic_value(10114))
	{
		client::rank_add_data rad;
		rad.set_rt(client::RANK_MOUNT);
		client::rank_rule *ru = rad.mutable_ru();
		ru->set_role_id(get_role_id());
		ru->set_first(id);
		ru->set_second(lv);
		ru->set_forth((uint32)time(NULL));
		send_center(rad);
	}
}

void scene_user::update_bows_rank(int id, int lv)
{
	if(id >= CONFMGR->get_basic_value(10139))
	{
		client::rank_add_data rad;
		rad.set_rt(client::RANK_BOWS);
		client::rank_rule *ru = rad.mutable_ru();
		ru->set_role_id(get_role_id());
		ru->set_first(id);
		ru->set_second(lv);
		ru->set_forth((uint32)time(NULL));
		send_center(rad);
	}

}

void scene_user::update_potential_rank(int lv)
{
	if(lv >= CONFMGR->get_basic_value(10115))
	{
		client::rank_add_data rad;
		rad.set_rt(client::RANK_POTENTIAL);
		client::rank_rule *ru = rad.mutable_ru();
		ru->set_role_id(get_role_id());
		ru->set_first(lv);
		ru->set_forth((uint32)time(NULL));
		send_center(rad);
	}
}

void scene_user::update_gem_rank(int lv)
{
	if(lv >= CONFMGR->get_basic_value(10138))
	{
		client::rank_add_data rad;
		rad.set_rt(client::RANK_GEMS);
		client::rank_rule *ru = rad.mutable_ru();
		ru->set_role_id(get_role_id());
		ru->set_first(lv);
		ru->set_forth((uint32)time(NULL));
		send_center(rad);
	}
}

void scene_user::update_equip_power_rank(uint32 limit_type,int new_power)
{
	client::act_rank_add_data rad;
	rad.set_rt(client::RANK_EQUIP_POWER);
	rad.set_limit_value(limit_type);
	client::rank_rule *ru = rad.mutable_ru();
	ru->set_role_id(get_role_id());
	ru->set_first(new_power);
	ru->set_forth((uint32)time(NULL));
	send_center(rad);
}

void scene_user::update_gem_integral(uint32 limit_type,int add_integral)
{
	client::act_rank_add_data rad;
	rad.set_rt(client::RANK_GEMS_INTEGRAL);
	rad.set_limit_value(limit_type);
	client::rank_rule *ru = rad.mutable_ru();
	ru->set_role_id(get_role_id());
	ru->set_first(add_integral);
	ru->set_forth((uint32)time(NULL));
	send_center(rad);
}

int scene_user::set_vip_lv(uint32 lv, bool month)
{
#ifndef __LIB_MINI_SERVER__
	int ret = get_player_role().set_vip_lv(lv, month);
	if(ret != 0)
	{
		const config_vip *cv = CONFMGR->get_config_vip(ret);
		if(cv != NULL)
		{
			mutable_attr()->set_val(ROLE_VIP_LEVEL, ret);
			mutable_attr()->set_backpack_size(cv->_grid);
			mutable_attr()->set_warehouse_size(cv->_grid);
			send_client_change_attr();
		}
	}
	return ret;
#else
	return 0;
#endif
}

int scene_user::recharge(int n)
{
#ifndef __LIB_MINI_SERVER__
	if(n <= 0)
	{
		return -1;
	}
	player_role_vip &prv = get_player_role().get_player_role_vip();
	bool is_month = prv.get_month_timeout() > (uint32)time(NULL);
	uint32 old_lv = prv.get_vip_lv();
	uint32 new_lv = old_lv;
	const std::map<uint32, config_recharge *> &v = CONFMGR->get_config_recharge_list();
	std::map<uint32, config_recharge *>::const_reverse_iterator i;
	uint32 min_recharge = CONFMGR->get_min_recharge();
	while(n > 0)
	{
		if((uint32)n < min_recharge)
		{
			new_lv = prv.add_vip_exp(get_role_id(), n, false);
			mutable_attr()->add_val(INGOT, n);
			break;
		}
		for(i = v.rbegin(); i != v.rend() && n > 0; ++i)
		{
			const config_recharge *cr = i->second;
			if((int)cr->get_ingot() > n)
			{
				continue;
			}
			if(cr->get_type() == MONTH_VIP && is_month)
			{
				continue;
			}
			new_lv = prv.add_vip_exp(get_role_id(), cr->get_ingot(), cr->get_type() == MONTH_VIP);
			if(cr->get_type() == MONTH_VIP)
			{
				CHECK_TARGET_PROGRESS(this, GET_INGOT);
			}
			uint32 rate = prv.get_recharge_times(cr->get_id()) == 0 ? cr->get_times() : 1;
			prv.add_recharge_times(cr->get_id());
			mutable_attr()->add_val(INGOT, cr->get_ingot() * rate);
			n -= cr->get_ingot();
		}
	}
	if(old_lv != new_lv)
	{
		const config_vip *vip = CONFMGR->get_config_vip(new_lv);
		if(vip)
		{
			reset_pack_size(vip->_grid,vip->_grid);
		}
		mutable_attr()->set_val(ROLE_VIP_LEVEL, get_vip_lv());
		update_online_smy_info(client::SMY_ROLE_VIP, new_lv);
	}
#endif
	return 0;
}

uint32 scene_user::get_vip_lv()
{
#ifndef __LIB_MINI_SERVER__
	player_role_vip &v = get_player_role().get_player_role_vip();
	return v.get_vip_lv();
#else
	return 0;
#endif
}

int scene_user::on_pb_black_shop_req(client::black_shop_req* req,client::black_shop_rsp& rsp_msg)
{
	uint32 now_time = (uint32)time(NULL);
	uint32 cd = get_player_role().get_role_cds().get_cooldown_time(CD_COOLDOW_TYPE_ID_BLACK_SHOP);
	uint32 remain = cd - now_time;
	rsp_msg.set_time(remain);
	uint32 reflesh_count = 0;
	get_player_role().get_role_statis_mgr().get(PRSTE_BLACK_REFRESH_COUNT)->get_val(reflesh_count);
	rsp_msg.set_reflesh_count(reflesh_count);
	player_role_black_shop & player_shop = get_player_role().get_player_role_black_shop();

	const std::map<uint32, client::black_shop_db_info>&  shop_list = player_shop.get_black_shop_item_list();

	uint32 is_frist_reflesh = 0;
	get_player_role().get_role_statis_mgr().get(PRSTE_BLACK_FRIST_REFRESH)->get_val(is_frist_reflesh);

	if (is_frist_reflesh == 0){
		const config_cd_time* cd_base = CONFMGR->get_config_cd_time_mgr().get_config_cd_time(CD_COOLDOW_TYPE_ID_BLACK_SHOP);
		if (cd_base == NULL){
			return -1;
		}
		// 同时开始倒计时
		if (get_player_role().get_role_cds().add_cooldown_time(CD_COOLDOW_TYPE_ID_BLACK_SHOP)){
			reflesh_shop();
			get_player_role().get_role_statis_mgr().get(PRSTE_BLACK_FRIST_REFRESH)->set_val(1);
			uint32 test_count = 0;
			get_player_role().get_role_statis_mgr().get(PRSTE_BLACK_FRIST_REFRESH)->get_val(test_count);
		}
	}

	if (shop_list.size() == 0){
		rsp_msg.set_result_type(BLACK_SHOP_RESULT_TYPE_REFLESH_FAILD);
		return -1;
	}

	if (get_player_role().get_role_cds().is_can_operate_by_cd(CD_COOLDOW_TYPE_ID_BLACK_SHOP)){
		reflesh_shop();
		get_player_role().get_role_cds().add_cooldown_time(CD_COOLDOW_TYPE_ID_BLACK_SHOP);
	}

	std::map<uint32, client::black_shop_db_info>::const_iterator it = shop_list.begin();
	for (; it != shop_list.end(); ++it){
		client::black_shop_db_info* info = rsp_msg.add_info();
		info->CopyFrom(it->second);
	}

	uint32 cur_time = (uint32)time(NULL);
	uint32 cd_time = get_player_role().get_role_cds().get_cooldown_time(CD_COOLDOW_TYPE_ID_BLACK_SHOP);
	uint32 remain_time = cd_time - cur_time;
	rsp_msg.set_time(remain_time);
	rsp_msg.set_result_type(BLACK_SHOP_RESULT_TYPE_REFLESH_SUCCESS);

	return 0;
}

int scene_user::on_pb_black_shop_reflesh_req(client::black_shop_reflesh_req* req,client::black_shop_reflesh_rsp& rsp_msg)
{
#ifndef __LIB_MINI_SERVER__
	uint32 cur_itme = (uint32)time(NULL);
	uint32 cd_time = get_player_role().get_role_cds().get_cooldown_time(CD_COOLDOW_TYPE_ID_BLACK_SHOP);
	uint32 remain_time =  cd_time - cur_itme;
	uint32 reflesh_count = 0;
	get_player_role().get_role_statis_mgr().get(PRSTE_BLACK_REFRESH_COUNT)->get_val(reflesh_count);
	rsp_msg.set_reflesh_count(reflesh_count);
	rsp_msg.set_time(remain_time);
	// 检测相关够不够
	uint32 cur_melting = get_attr().get_val(MELTING);
	uint32 need_melting = CONFMGR->get_basic_value(10229);
	if (cur_melting < need_melting){
		rsp_msg.set_result_type(BLACK_SHOP_RESULT_TYPE_MELTING_NOT_ENOUGH);
	}
	bool is_need_item = false;

	player_role_vip &v = get_player_role().get_player_role_vip();
	uint32 vip_level = v.get_vip_lv();
	const config_vip *vip = CONFMGR->get_config_vip(vip_level);
	if (vip == NULL){
		rsp_msg.set_result_type(BLACK_SHOP_RESULT_TYPE_CONFIG_ERRO);
		return -1;
	}

	if (reflesh_count >= static_cast<uint32>(vip->_refresh_count)){
		rsp_msg.set_result_type(BLACK_SHOP_RESULT_TYPE_REFLESH_COUNT_NOT_ENGOUH);
		is_need_item = true;
	}

	uint32 item_id = CONFMGR->get_basic_value(10008);
	if (is_need_item){
		props_parser& main_pack = get_player_role().get_role_props().get_pack();
		if (main_pack.get_props_num(item_id) < 1){
			rsp_msg.set_result_type(BLACK_SHOP_RESULT_TYPE_ITEM_NOT_ENOUGH);
			return -1;
		}
	} else {
		if (reflesh_count >= static_cast<uint32>(vip->_refresh_count)){
			rsp_msg.set_result_type(BLACK_SHOP_RESULT_TYPE_REFLESH_COUNT_NOT_ENGOUH);
			return -1;
		}
	}

	// 刷新：数据处理
	if (!reflesh_shop()){
		rsp_msg.set_result_type(BLACK_SHOP_RESULT_TYPE_REFLESH_FAILD);
		return -1;
	}
	
	// 扣除次数或者物品
	if (is_need_item){
		sub_props(item_id,1);
	}else {
		get_player_role().get_role_statis_mgr().get(PRSTE_BLACK_REFRESH_COUNT)->set_val(reflesh_count +1);
		rsp_msg.set_reflesh_count(reflesh_count +1);
	}
	// 扣熔炼值
	mutable_attr()->set_val(MELTING,cur_melting - need_melting);
	player_role_black_shop & player_shop = get_player_role().get_player_role_black_shop();

	const std::map<uint32, client::black_shop_db_info>&  shop_list = player_shop.get_black_shop_item_list();
	std::map<uint32, client::black_shop_db_info>::const_iterator it = shop_list.begin();
	for (; it != shop_list.end(); ++it){
		client::black_shop_db_info* info = rsp_msg.add_info();
		info->CopyFrom(it->second);
	}
	
	rsp_msg.set_result_type(BLACK_SHOP_RESULT_TYPE_REFLESH_SUCCESS);
	//rsp_msg.set_time(remain_time);

	send_client_change_attr();
	if (is_need_item){
		send_client_change_props_msg();
	}
#endif	
	return 0;
}

int scene_user::on_pb_black_shop_buy_req(client::black_shop_buy_req* req,client::black_shop_buy_rsp& rsp_msg)
{
	rsp_msg.set_item_slot(req->item_slot());

	// 检测是否足够

	player_role_black_shop & player_shop = get_player_role().get_player_role_black_shop();

	const std::map<uint32, client::black_shop_db_info>&  shop_list = player_shop.get_black_shop_item_list();

	std::map<uint32, client::black_shop_db_info>::const_iterator it = shop_list.find(req->item_slot());
	if (it == shop_list.end()){
		rsp_msg.set_result_type(1);
		return -1;
	}

	if (it->second.item_count() == 0 || it->second.is_buy()){
		rsp_msg.set_result_type(1);
		return -1;
	}

	const config_treasure* base_info = CONFMGR->get_config_treasure_mgr().get_config_treasure(it->second.key_id());
	if (base_info == NULL){
		rsp_msg.set_result_type(1);
		return -1;
	}

	uint32 total_melting = base_info->get_single_price() * it->second.item_count();
	uint32 cur_melting = get_attr().get_val(MELTING);
	if (cur_melting < total_melting){
		rsp_msg.set_result_type(1);
		return -1;
	}

	add_props(it->second.item_id(),it->second.item_count());

	// 记录相关的次数
	player_role_treasure& player_treasure = get_player_role().get_role_treasures();
	client::treasure_db_info info;
	if (!player_treasure.get_treasure_info(it->second.item_id(),info)){
		player_treasure.create_treasure_info(it->second.item_id(),info);
	}
	info.set_item_id(it->second.item_id());
	info.set_item_count(info.item_count() + it->second.item_count());
	player_treasure.change_treasure_info(info);
	// 扣除当前格子的东西
	//player_shop.remove_black_shop_data(it->first);
	client::black_shop_db_info change_info;
	change_info.set_item_count(it->second.item_count());
	change_info.set_item_id(it->second.item_id());
	change_info.set_item_slot(it->second.item_slot());
	change_info.set_key_id(it->second.key_id());
	change_info.set_is_buy(true);
	player_shop.add_blacl_shop_data(change_info);
	std::map<uint32, client::black_shop_db_info>::const_iterator it_black = shop_list.begin();
	for (;it_black != shop_list.end(); ++it_black){
		client::black_shop_db_info* info = rsp_msg.add_info();
		info->CopyFrom(it_black->second);
	}
	
	// 扣除熔炼值
	mutable_attr()->set_val(MELTING,cur_melting - total_melting);
	send_client_change_attr();
	send_client_change_props_msg();
	rsp_msg.set_result_type(0);

	return 0;
}

uint32 scene_user::get_challenge_cd()
{
	player_role_statis_mgr &mgr = get_player_role().get_role_statis_mgr();
	uint32 now = (uint32)time(NULL);
	uint32 end = mgr.get_challenge_cd() + (uint32)CONFMGR->get_basic_value(10020);
	if(now > end)
	{
		return 0;
	}
	return end - now;
}

uint32 scene_user::get_arena_used_times()
{
	player_role_statis_mgr &mgr = get_player_role().get_role_statis_mgr();
	if(mgr.get_challenge_times() > 0)
	{
		uint32 val = (uint32)time(NULL) - mgr.get_last_recover_time();
		uint32 timelong = (uint32)CONFMGR->get_basic_value(10021);
		uint32 n = val / timelong;
		if(n > 0)
		{
			mgr.recover_challenge_times(n, n * timelong);
		}
	}
	return mgr.get_challenge_times();
}

uint32 scene_user::get_arena_left_fresh_time()
{
	player_role_statis_mgr &mgr = get_player_role().get_role_statis_mgr();
	uint32 val = (uint32)time(NULL) - mgr.get_last_recover_time();
	uint32 timelong = (uint32)CONFMGR->get_basic_value(10021);
	if(val >= timelong)
	{
		return 0;
	}
	return timelong - val;
}

void scene_user::set_arena_used_times(uint32 n)
{
	get_player_role().get_role_statis_mgr().add_challenge_times();
}

void scene_user::reset_arena_used_times()
{
	get_player_role().get_role_statis_mgr().reset_challenge_times();
}

uint32 scene_user::get_arena_reset_times()
{
	return get_player_role().get_role_statis_mgr().get_reset_challenge_times();
}

void scene_user::clear_arena_cd_time()
{
	get_player_role().get_role_statis_mgr().clear_challenge_cd();
}

uint32 scene_user::get_arena_chall_time()
{
	return get_player_role().get_role_statis_mgr().get_challenge_time();
}

int scene_user::on_pb_sign_list_req(client::sign_list_rsp& rsp_msg)
{
	client::day_info data_info;	
	fill_cur_date(data_info);

	client::day_info* info = rsp_msg.add_info();

	info->CopyFrom(data_info);

	uint32 draw_flag = 0;
	get_player_role().get_role_statis_mgr().get(PRSTE_SIGN_IN)->get_val(draw_flag);

	rsp_msg.set_sign_value(draw_flag);

	return 0;
}

int scene_user::on_pb_land_reward_list_req(client::land_reward_list_rsp& rsp_msg)
{

	uint32 land_day = 0;
	get_player_role().get_role_statis_mgr().get(PRSTE_LAND_DAY)->get_val(land_day);

	uint32 reward_flag = 0;
	get_player_role().get_role_statis_mgr().get(PRSTE_LAND_DAY_REWARD)->get_val(reward_flag);


	rsp_msg.set_land_day(land_day);
	rsp_msg.set_reward_flag(reward_flag);

	return 0;
}

uint32 get_sign_count(uint32 i)
{
	 uint32 count = 0;

	 while (i){
		 ++ count;
		 i = (i - 1) & i;
	 }

	 return count;

}

int scene_user::on_pb_sign_in_req(client::sign_in_req* req,client::sign_in_rsp& rsp_msg)
{
	client::day_info data_info;	
	fill_cur_date(data_info);


	uint32 cur_flag = sign_vale[data_info.day()];
	uint32 draw_flag = 0;
	get_player_role().get_role_statis_mgr().get(PRSTE_SIGN_IN)->get_val(draw_flag);
	if (HAS_FLAG(draw_flag,cur_flag)){
		// 已经签到过
		rsp_msg.set_result_type(SIGN_IN_RESULT_ALREADY_SIGN);
		rsp_msg.set_sign_value(draw_flag);
		return -1;
	}

	INSERT_FLAG(draw_flag,cur_flag);
	uint32 sign_count = get_sign_count(draw_flag);
	const config_sign* sign_info = CONFMGR->get_config_sign_mgr().get_config_sign(sign_count);
	if (sign_info == NULL){
		rsp_msg.set_result_type(SIGN_IN_RESULT_CONFIG_ERRO);
		rsp_msg.set_sign_value(draw_flag);
		return -1;
	}

	// 记录
	get_player_role().get_role_statis_mgr().get(PRSTE_SIGN_IN)->set_val(draw_flag);

	// 直接给奖励
	const std::map<int32,int32>& sign_reward_item_map_ = sign_info->get_reward_item();
	std::map<int32,int32>::const_iterator it = sign_reward_item_map_.begin();
	for (;it != sign_reward_item_map_.end(); ++it){
		add_props((uint32)it->first,(uint32)it->second);
	}

	if (sign_reward_item_map_.size() != 0){
		send_client_change_props_msg();
	}

	uint32 reward_money = 0;
	reward_money = sign_info->get_reward_tael();
	uint32 reward_ingot = 0;
	reward_ingot = sign_info->get_reward_ingot();
	if (reward_money != 0){
		add_money(TAEL,reward_money);
	}

	if (reward_ingot != 0){
		add_money(INGOT,reward_ingot);
	}
	
	if (reward_money != 0 || reward_ingot != 0){
		send_client_change_attr();
	}
	
	rsp_msg.set_result_type(SIGN_IN_RESULT_SUCCESS);
	rsp_msg.set_sign_value(draw_flag);

	return 0;
}

bool is_can_opt(scene_user* psu,const config_land* land_info)
{
	const std::map<int, int>& limit_map_ = land_info->get_land_condition();
	std::map<int, int>::const_iterator it = limit_map_.begin();
	for (;it != limit_map_.end(); ++it){
		if (it->first == 1){
			if (psu->get_attr(LEVEL) < static_cast<uint32>(it->second)){
				return false;
			}
		}
	}
	
	return true;
}

int scene_user::on_pb_land_reward_req(client::land_reward_req* req,client::land_reward_rsp& rsp_msg)
{
	// 获取登陆的天数
	uint32 land_day = 0;
	get_player_role().get_role_statis_mgr().get(PRSTE_LAND_DAY)->get_val(land_day);
	// 获取奖励领取的标记
	uint32 reward_flag = 0;
	get_player_role().get_role_statis_mgr().get(PRSTE_LAND_DAY_REWARD)->get_val(reward_flag);

	if (req->land_day() > land_day || req->land_day() > 15){
		rsp_msg.set_result_type(1);
		return -1;
	}


	// 获取奖励的标记
	uint32 cur_flag = sign_vale[req->land_day()];
	// 检测是否已经领取过
	if (HAS_FLAG(reward_flag,cur_flag)){
		// 已经领过了~
		rsp_msg.set_result_type(2);
		return -1;
	}

	const config_land* land_info = CONFMGR->get_config_land_mgr().get_config_land(req->land_day());

	if (land_info == NULL){
		rsp_msg.set_result_type(3);
		return -1;
	}

	if (!is_can_opt(this,land_info)){
		rsp_msg.set_result_type(4);
		return -1;
	}
	// 设置标记
	INSERT_FLAG(reward_flag,cur_flag);

	get_player_role().get_role_statis_mgr().get(PRSTE_LAND_DAY_REWARD)->set_val(reward_flag);
	// 给奖励
	const std::map<int, succinct_info>& reward_map_1 = land_info->get_item_1();
	std::map<int, succinct_info>::const_iterator it_1 = reward_map_1.begin();
	for (;it_1 != reward_map_1.end(); ++it_1){
		if (static_cast<uint32>(it_1->first) == get_attr(PROFESSION) || it_1->first == 5){
			add_props(it_1->second.item_id,it_1->second.item_count);
		}
	}

	const std::map<int, succinct_info>& reward_map_2 = land_info->get_item_2();
	std::map<int, succinct_info>::const_iterator it_2 = reward_map_2.begin();
	for (;it_2 != reward_map_2.end(); ++it_2){
		if (static_cast<uint32>(it_2->first) == get_attr(PROFESSION) || it_2->first == 5){
			add_props(it_2->second.item_id,it_2->second.item_count);
		}
	}


	const std::map<int, succinct_info>& reward_map_3 = land_info->get_item_3();
	std::map<int, succinct_info>::const_iterator it_3 = reward_map_3.begin();
	for (;it_3 != reward_map_3.end(); ++it_3){
		if (static_cast<uint32>(it_3->first) == get_attr(PROFESSION) || it_3->first == 5){
			add_props(it_3->second.item_id,it_3->second.item_count);
		}
	}

	send_client_change_props_msg();

	uint32 reward_money = 0;
	reward_money = land_info->get_land_money();
	uint32 reward_ingot = 0;
	reward_ingot = land_info->get_yuan_bao();
	if (reward_money != 0){
		add_money(TAEL,reward_money);
	}

	if (reward_ingot != 0){
		add_money(INGOT,reward_ingot);
	}

	if (reward_money != 0 || reward_ingot != 0){
		send_client_change_attr();
	}

	rsp_msg.set_result_type(0);
	rsp_msg.set_reward_flag(reward_flag);

	return 0;
}

int scene_user::on_pb_gem_list_req(client::gem_list_req* req,client::gem_list_rsp& rsp_msg)
{
	rsp_msg.set_list_type(0);

	player_role_gem& player_gem = get_player_role().get_player_role_gems();


	const std::map<uint32, client::gem_info>& gem_map_ = player_gem.get_all_gem_info();
	std::map<uint32, client::gem_info>::const_iterator it = gem_map_.begin();

	for (;it != gem_map_.end();++ it){
		client::gem_info* info = rsp_msg.add_info();
		info->CopyFrom(it->second);
	}

	return 0;
}

uint32 calcul_add_exp(const std::map<int, succinct_info> exp_map,uint32& cir_count)
{
	int denominator = 0;
	std::map<int, succinct_info>::const_iterator it = exp_map.begin();
	for (; it != exp_map.end(); ++it){
		denominator += it->second.item_id;// 获得分母
	}

	uint32 rand_value = mtrandom::rand_int32(static_cast<uint32>(denominator));
	uint32 attenuation = 0; 
	std::map<int, succinct_info>::const_iterator itr = exp_map.begin();
	for (;itr != exp_map.end(); ++itr){
		attenuation += static_cast<uint32>(itr->second.item_id);

		if (rand_value >= attenuation){
			continue;
		}

		cir_count = itr->second.item_count;

		return static_cast<uint32>(itr->first);
	}

	return 0;
}

int scene_user::on_pb_gem_level_up_req(client::gem_level_up_req* req,client::gem_level_up_rsp& rsp_msg)
{
	rsp_msg.set_gem_pos(req->gem_pos());

	// 升级或者激活成1级
	player_role_gem& player_gem = get_player_role().get_player_role_gems();
	
	if (player_gem.check_is_max(req->gem_pos())){
		rsp_msg.set_result_type(3);
		return -1;
	}

	uint32 gem_type = player_gem.get_gem_type(req->gem_pos());
	// 获取宝石基础数据
	const config_gem* gem_base_info = CONFMGR->get_config_gem(gem_type,0);

	if (gem_base_info == NULL){
		rsp_msg.set_result_type(1);
		return -1;
	}
	
	const std::vector<int>& use_vec = gem_base_info->get_using_component();
	bool is_match = false;

	for (uint32 i = 0; i < use_vec.size(); ++i){
		if (use_vec[i] == static_cast<int>(req->gem_pos())){
			is_match = true;
		}
	}

	if (!is_match){
		rsp_msg.set_result_type(2);
		return -1;
	}

	uint32 upgrade_hole_id = 0;
	player_gem.find_gem_upgrade_id(req->gem_pos(),upgrade_hole_id);

	const config_gem* gem_base = CONFMGR->get_config_gem(gem_type,player_gem.get_hole_gem_level(upgrade_hole_id));
	if (gem_base == NULL){
		rsp_msg.set_result_type(1);
		return -1;
	}

	const config_gem* next_gem_info = CONFMGR->get_config_gem(gem_type,player_gem.get_hole_gem_level(upgrade_hole_id) + 1);
	if (next_gem_info == NULL){
		rsp_msg.set_result_type(3);
		return -1;
	}

	// 判断升级所需的物品是否足够
	uint32 need_count = gem_base->get_need_count();
	uint32 has_count = 0;
	const std::vector<int>& need_vec = gem_base->get_upgrade_goods();
	for (size_t k = 0; k < need_vec.size(); ++k){
		has_count += get_props_num(static_cast<uint32>(need_vec[k]));
	}

	if (has_count < need_count){
		rsp_msg.set_result_type(1);
		return -1;
	}
	// 暴击倍数
	uint32 cir_count = 0;
	// 算经验
	uint32 add_exp = calcul_add_exp(gem_base->get_add_exp_list(),cir_count);

	bool level_is_change = false;
	while(true){
		if (add_exp == 0){
			break;
		}

		player_gem.add_gem_exp(req->gem_pos(),add_exp,level_is_change);

	}

	// 扣除消耗：
	for (size_t j = 0; j < need_vec.size(); ++j){
		uint32 count = 0;
		count = get_props_num(static_cast<uint32>(need_vec[j]));
		if (count >= need_count){
			sub_props(static_cast<uint32>(need_vec[j]),need_count);
			break;
		}else{
			sub_props(static_cast<uint32>(need_vec[j]),count);
			need_count -= count;
		}
	}
	// 更新到活动：宝石积分榜
	update_gem_integral(REWARD_TYPE_GEM_LEVEL,static_cast<int>(need_count));

	send_client_change_props_msg();
	
	if (level_is_change){
		calculate_attr();
		send_client_change_attr();
		// 排行榜更新
		uint32 total_level = player_gem.get_gem_total_level();
		update_gem_rank(total_level);
		CHECK_TARGET_PROGRESS(this, GEM_LEVEL, total_level);
	}
	CHECK_TARGET_PROGRESS(this, GEM_UPGRADE);
	rsp_msg.set_result_type(0);
	rsp_msg.set_cir_count(cir_count);
	player_gem.fill_gem_pos_info(req->gem_pos(),rsp_msg);

	return 0;
}

void fill_skill_data(player_role_skill* prs, client::skill_data* sd);
ITEM_OPERATE_TYPE scene_user::bow_skill_study(const config_props* base_info, uint32 count)
{
#ifndef __LIB_MINI_SERVER__
	client::bows_skill_studt_rsp studt_rsp;
	const std::vector<int>& tri_vec = base_info->get_trigger_list();
	if (tri_vec.size() == 0){
		studt_rsp.set_result_type(2);
		send_client(studt_rsp);
		return ITEM_OPERATE_TYPE_CONFIG_ERRO;
	}

	uint32 study_skill_id = static_cast<uint32>(tri_vec[0]);
	const config_skill* skill_base = CONFMGR->get_config_skill(study_skill_id);
	if (skill_base == NULL){
		studt_rsp.set_result_type(2);
		send_client(studt_rsp);
		return ITEM_OPERATE_TYPE_CONFIG_ERRO;
	}
	
	player_role_bows& player_bow = get_player_role().get_player_role_bows();
	if (player_bow.get_db().bow_id() == 0){
		studt_rsp.set_result_type(3);
		send_client(studt_rsp);
		return ITEM_OPERATE_TYPE_OBJECT_NULL;
	}

	const config_bows*  bows_base = CONFMGR->get_config_bows_mgr().get_config_bows(player_bow.get_db().bow_id());
	if (bows_base == NULL){
		studt_rsp.set_result_type(2);
		send_client(studt_rsp);
		return ITEM_OPERATE_TYPE_CONFIG_ERRO;
	}

	if (bows_base->get_base() == NULL){
		studt_rsp.set_result_type(2);
		send_client(studt_rsp);
		return ITEM_OPERATE_TYPE_CONFIG_ERRO;
	}
	// 获取阶数所开启的技能格子数
	uint32 open_slot = bows_base->get_base()->get_max_skill_level();

	if (open_slot == 0){
		studt_rsp.set_result_type(4);
		send_client(studt_rsp);
		return ITEM_OPERATE_TYPE_USE_LIMIT;
	}
	// 获取当前所拥有的弓箭技能数
	uint32 cur_skill_count = get_bow_skill_count();

	// 没有空位了
	if (open_slot == cur_skill_count){
		uint32 need_replace_id = 0;
		uint32 rand_ser_index = 0;
		find_replace_bow_skill(need_replace_id,rand_ser_index);

		player_role_skill* prs = get_player_role().get_role_skill_mgr().get_skill(need_replace_id,rand_ser_index);
		if (prs == NULL) {
			studt_rsp.set_result_type(4);
			send_client(studt_rsp);
			return ITEM_OPERATE_TYPE_USE_LIMIT;
		}

		fill_skill_data(prs, studt_rsp.mutable_old_skill());

		skill* new_skill = new skill(*skill_base);
		new_skill->init(skill_base);
		new_skill->set_ser_index(rand_ser_index);

		mutable_skill()->delete_skill(need_replace_id,rand_ser_index);
		mutable_skill()->add_skill(*new_skill,rand_ser_index);

		get_player_role().get_role_skill_mgr().delete_skill(need_replace_id,rand_ser_index);
		get_player_role().get_role_skill_mgr().add_skill(get_role_id(), skill_base,rand_ser_index);

		player_role_skill* new_prs = get_player_role().get_role_skill_mgr().get_skill(study_skill_id,rand_ser_index);
		if (new_prs == NULL) {
			studt_rsp.set_result_type(4);
			send_client(studt_rsp);
			return ITEM_OPERATE_TYPE_USE_LIMIT;
		}

		fill_skill_data(new_prs, studt_rsp.mutable_new_skill());
		studt_rsp.set_result_type(0);
		send_client(studt_rsp);

	}else if (open_slot > cur_skill_count){

		uint32 study_param = CONFMGR->get_basic_value(10012);
		uint32 denominator = cur_skill_count * study_param + 1;
		//uint32 amp_denominator = denominator * 10000;
		// 放大一万倍
		//uint32 member = static_cast<uint32>((1.0f / static_cast<float>(denominator)) * 10000.0f);

		uint32 rand_value = mtrandom::rand_mid32(1,denominator);

		if (rand_value ==  1){
			// 中了，打到空格去
			uint32 new_ser_index = cur_skill_count + 1;
			skill* new_skill = new skill(*skill_base);
			new_skill->init(skill_base);
			new_skill->set_ser_index(new_ser_index);

			mutable_skill()->add_skill(*new_skill,new_ser_index);

			get_player_role().get_role_skill_mgr().add_skill(get_role_id(), skill_base,new_ser_index);

			uint32 cur_speed = calculate_role_bow_speed(player_bow.get_db().bow_id(),player_bow.get_db().bow_level());

			mutable_attr()->set_val(BOW_SPEED,cur_speed);
			get_player_role().get_query_attr().set_bow_speed(get_attr(BOW_SPEED));
			calculate_attr();
			mutable_attr()->set_val(BOW_SPEED,cur_speed);
			send_client_change_attr();

			player_role_skill* new_prs = get_player_role().get_role_skill_mgr().get_skill(study_skill_id,new_ser_index);
			if (new_prs == NULL) {
				studt_rsp.set_result_type(4);
				send_client(studt_rsp);
				return ITEM_OPERATE_TYPE_USE_LIMIT;
			}

			calculate_attr();
			send_client_change_attr();

			studt_rsp.set_result_type(0);
			fill_skill_data(new_prs,studt_rsp.mutable_new_skill());
			send_client(studt_rsp);
		}else{
			// 覆盖
			uint32 need_replace_id = 0;
			uint32 rand_ser_index = 0;
			find_replace_bow_skill(need_replace_id,rand_ser_index);

			player_role_skill* old_prs = get_player_role().get_role_skill_mgr().get_skill(need_replace_id,rand_ser_index);
			if (old_prs == NULL) {
				studt_rsp.set_result_type(4);
				send_client(studt_rsp);
				return ITEM_OPERATE_TYPE_USE_LIMIT;
			}

			fill_skill_data(old_prs,studt_rsp.mutable_old_skill());

			skill* new_skill = new skill(*skill_base);
			new_skill->init(skill_base);
			new_skill->set_ser_index(rand_ser_index);

			mutable_skill()->delete_skill(need_replace_id,rand_ser_index);
			mutable_skill()->add_skill(*new_skill,rand_ser_index);

			get_player_role().get_role_skill_mgr().delete_skill(need_replace_id,rand_ser_index);
			get_player_role().get_role_skill_mgr().add_skill(get_role_id(), skill_base,rand_ser_index);


			player_role_skill* new_prs = get_player_role().get_role_skill_mgr().get_skill(study_skill_id,rand_ser_index);
			if (new_prs == NULL) {
				studt_rsp.set_result_type(4);
				send_client(studt_rsp);
				return ITEM_OPERATE_TYPE_USE_LIMIT;
			}

			fill_skill_data(new_prs,studt_rsp.mutable_new_skill());
			studt_rsp.set_result_type(0);
			send_client(studt_rsp);
		}
	}
	else{
		studt_rsp.set_result_type(1);
		send_client(studt_rsp);
		return ITEM_OPERATE_TYPE_USE_LIMIT;
	}
#endif // __LIB_MINI_SERVER__
	return ITEM_OPERATE_TYPE_SUCCESS;
}
