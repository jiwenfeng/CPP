#include <math.h>
#include "config_skill.h"
#include "config_effect.h"
#include "player_role.h"
#include "player_role_skill_mgr.h"
#include "game_interface.h"
#include "scene_user.h"
#include "scene_inst.h"
#include "player_role_props.h"
#include "player_role_gem.h"
#include "config_basic.h"
#include "config_props.h"
#include "config_gem_act.h"
#include "config_beauty.h"

void fill_skill_data(player_role_skill* prs, client::skill_data* sd)
{
    if (prs == NULL || sd == NULL) {
        return ;
    }
    
    sd->set_id    (prs->get_db_role_skill()->skill_id());
    sd->set_cd    (prs->get_db_role_skill()->end_cd_time());
    sd->set_mas   (prs->get_db_role_skill()->skillful());
    sd->set_serial(prs->get_db_role_skill()->skill_grid_index());
    sd->set_level (prs->get_db_role_skill()->skill_level());
	sd->set_effect_id(prs->get_db_role_skill()->effect_id());
}

// 技能点满的时候才会修正时间，避免每一秒去修正这个时间的消耗
void update_skill_point_time(scene_user* psu,bool is_set)
{
	uint32 cur_time = (uint32)time(NULL);
	uint32 interval_time = static_cast<uint32>(CONFMGR->get_config_basic_mgr().get_basic_value(10085));
	uint32 skil_point_max = static_cast<uint32>(CONFMGR->get_config_basic_mgr().get_basic_value(10086));
	if (is_set){
		psu->mutable_attr()->set_val(ROLE_SKILL_POINT_TIME,cur_time);
	}
	uint32 cur_skill_point_time = psu->mutable_attr()->get_val(ROLE_SKILL_POINT_TIME);
	uint32 difference_time = cur_time - cur_skill_point_time;

	// 取剩下时间 = 配置间隔时间 - 取出余数
	uint32 remain_time = interval_time - static_cast<uint32>(difference_time % interval_time);
	uint32 cur_skill_point = psu->mutable_attr()->get_val(ROLE_SKILL_POINT);
	if (cur_skill_point != skil_point_max){
		client::skill_point_time_rsp rsp_msg;
		rsp_msg.set_remain_time(remain_time);
		psu->send_client(rsp_msg);
	}
}

void* on_pb_role_skill_upgrade_req(client::role_skill_upgrade_req* req, void* ud, size_t* rsp_len)
{
    PB_FUNC_GUARD(req);
	uint32 ser_index = 0;
	if (req->has_ser_index()){
		ser_index = req->ser_index();
	}

    client::role_skill_upgrade_rsp rsp;
    player_role_skill* prs = psu->get_player_role().get_role_skill_mgr().get_skill(req->skill_id(),ser_index);
    if (prs == NULL || !prs->studied()) {
        rsp.set_ret(client::role_skill_upgrade_rsp_RET_LIMIT_SKILL);
        psu->send_client(rsp);
        return NULL;
    }

    if (prs->max_lvl()) {
        rsp.set_ret(client::role_skill_upgrade_rsp_RET_LIMIT_MAX);
        psu->send_client(rsp);
        return NULL;
    }

    skill* ps = psu->mutable_skill()->get_skill(req->skill_id(),ser_index);
    if (ps == NULL) {
		rsp.set_ret(role_skill_upgrade_rsp_RET_LIMIT_SKILL);
		psu->send_client(rsp);
        return NULL;
    }

    const config_skill* cs = CONFMGR->get_config_skill_mgr().get_config_skill(req->skill_id());
    if (cs == NULL) {
		rsp.set_ret(role_skill_upgrade_rsp_RET_LIMIT_CONF);
		psu->send_client(rsp);
        return NULL;
    }

	// 判断相关消耗
	uint32 cur_strategy = psu->mutable_attr()->get_val(MILITARY_STRATEGY);
	uint32 expend_money = cs->get_upgrade_need_yuan_li();
	if (cur_strategy < expend_money){
		rsp.set_ret(role_skill_upgrade_rsp_RET_LIMIT_YUNLI);
		psu->send_client(rsp);
		return NULL;
	}
	uint32 expend_id = cs->get_upgrade_need_item_id();
	uint32 expend_count = cs->get_upgrade_need_item_num();
	if (psu->get_props_num(expend_id) < expend_count){
		rsp.set_ret(role_skill_upgrade_rsp_RET_LIMIT_ITEM);
		psu->send_client(rsp);
		return NULL;
	}

    const config_skill* upgrade_cs = CONFMGR->get_config_skill_mgr().get_config_skill(cs->get_upgrade_skill_id());
    if (upgrade_cs == NULL) {
		rsp.set_ret(role_skill_upgrade_rsp_RET_LIMIT_CONF);
		psu->send_client(rsp);
        return NULL;
    }

    fill_skill_data(prs, rsp.mutable_old_skill());
	uint32 effect_id = prs->get_db_role_skill()->effect_id();
	uint32 skill_level = prs->get_db_role_skill()->skill_level();
    skill new_skill = *ps;
    new_skill.init(upgrade_cs);
	new_skill.set_ser_index(ser_index);

    psu->mutable_skill()->delete_skill(prs->get_skill_id(),ser_index);
    psu->mutable_skill()->add_skill(new_skill,ser_index);
	psu->mutable_skill()->set_skill_effect(upgrade_cs->get_skill_id(),effect_id,skill_level,ser_index);

    psu->get_player_role().get_role_skill_mgr().delete_skill(prs->get_skill_id(),ser_index);
    psu->get_player_role().get_role_skill_mgr().add_skill(psu->get_role_id(), upgrade_cs,ser_index);
	psu->get_player_role().get_role_skill_mgr().set_skill_effect(upgrade_cs->get_skill_id(),effect_id,skill_level,ser_index);

    player_role_skill* new_prs = psu->get_player_role().get_role_skill_mgr().get_skill(upgrade_cs->get_skill_id(),ser_index);
	if (new_prs == NULL){
		rsp.set_ret(role_skill_upgrade_rsp_RET_LIMIT_CONF);
		psu->send_client(rsp);
		return NULL;
	}

    fill_skill_data(new_prs, rsp.mutable_new_skill());

	CHECK_TARGET_PROGRESS(psu, ROLE_SKILL_UPGRADE_STAR, upgrade_cs->get_skill_id());

    rsp.set_ret(client::role_skill_upgrade_rsp_RET_SUCCESS);
	// 扣除相关东西
	if (expend_count != 0){
		psu->sub_props(expend_id,expend_count);
		psu->send_client_change_props_msg();
	}

	if (expend_money != 0){
		psu->sub_money(MILITARY_STRATEGY,expend_money);
		psu->send_client_change_attr();
	}

    psu->send_client(rsp);

    amtsvr_billlog(ss->ctx, "%u|%s|%u", psu->get_role_id(), req->GetTypeName().c_str(), req->skill_id());

    return NULL;
}

// 升级技能效果
void* on_pb_role_skill_effect_upgrade_req(client::role_skill_effect_upgrade_req* req, void* ud, size_t* rsp_len)
{
	PB_FUNC_GUARD(req);
	role_skill_effect_upgrade_rsp rsp_msg;
	const uint32 skill_id = req->skill_id();
	player_role_skill* prs = psu->get_player_role().get_role_skill_mgr().get_skill(skill_id);
	if (prs == NULL || !prs->studied()) {
		rsp_msg.set_result_type(2);
		psu->send_client(rsp_msg);
		return NULL;
	}
	
	skill* ps = psu->mutable_skill()->get_skill(skill_id);
	if (ps == NULL) {
		rsp_msg.set_result_type(3);
		psu->send_client(rsp_msg);
		return NULL;
	}

	// 改变效果的等级与效果的effect_id
	fill_skill_data(prs, rsp_msg.mutable_old_skill());
	uint32 cur_money = psu->mutable_attr()->get_val(TAEL);
	uint32 player_level = psu->mutable_attr()->get_val(LEVEL);
	uint32 skill_point = psu->mutable_attr()->get_val(ROLE_SKILL_POINT);
	const config_skill* skill_base = CONFMGR->get_config_skill(skill_id);
	if (skill_base == NULL) {
		rsp_msg.set_result_type(4);
		psu->send_client(rsp_msg);
		return NULL;
	}

	
	skill* cur_skill = psu->mutable_skill()->get_skill(skill_id);
	if (cur_skill == NULL){
		rsp_msg.set_result_type(3);
		psu->send_client(rsp_msg);
		return NULL;
	}

	// 当前技能的效果数据
	const config_effect* cur_base_info = CONFMGR->get_config_effect_mgr().get_config_effect_by_key(skill_base->get_key(),cur_skill->level);
	if (cur_base_info == NULL){
		rsp_msg.set_result_type(4);
		psu->send_client(rsp_msg);
		return NULL;
	}
	// 判断技能点消耗
	if (skill_point < 1){
		rsp_msg.set_result_type(5);
		psu->send_client(rsp_msg);
		return NULL;
	}

	uint32 pet_id = 0;
	client::pet_db_info pet_info;
	// 判断等级限制
	if (req->has_pet_id()){
		pet_id = req->pet_id();
		player_role_pet& player_pet = psu->get_player_role().get_pets();
		if (!player_pet.get_pet_info(pet_id,pet_info)){
			rsp_msg.set_result_type(6);
			psu->send_client(rsp_msg);
			return NULL;
		}

		if (cur_base_info->get_upgrade_need_level() > pet_info.pet_level()){
			rsp_msg.set_result_type(7);
			psu->send_client(rsp_msg);
			return NULL;
		}
	}
	else{
		if (cur_base_info->get_upgrade_need_level() > player_level){
			rsp_msg.set_result_type(8);
			psu->send_client(rsp_msg);
			return NULL;
		}
	}
	// 判断消耗的金钱
	uint32 need_pay  = cur_base_info->get_upgrade_need_money();
	if (need_pay > cur_money){
		rsp_msg.set_result_type(9);
		psu->send_client(rsp_msg);
		return NULL;
	}

	uint32 old_effect_level = psu->get_player_role().get_role_skill_mgr().get_skill_effect_level(skill_id);
	if (!psu->mutable_skill()->upgrade_skill_effect(skill_id)
		|| !psu->get_player_role().get_role_skill_mgr().upgrade_skill_effect(skill_id)){
		rsp_msg.set_result_type(1);
		psu->send_client(rsp_msg);
		return NULL;
	}
	uint32 new_effect_level = psu->get_player_role().get_role_skill_mgr().get_skill_effect_level(skill_id);
	if (req->has_pet_id()){
		psu->pet_skill_effect_up(pet_info.pet_id(),skill_id);
		CHECK_TARGET_PROGRESS(psu, PET_SKILL_UPGRADE);
	}
	else{
		CHECK_TARGET_PROGRESS(psu, ROLE_SKILL_UPGRADE);
	}
	// 扣钱 扣技能点
	psu->sub_money(TAEL,need_pay);
	uint32 new_money = cur_money - need_pay;
	psu->sub_money(ROLE_SKILL_POINT,1);
	psu->send_client_change_attr();
	uint32 skil_point_max = static_cast<uint32>(CONFMGR->get_config_basic_mgr().get_basic_value(10086));
	// 当上一次技能点是满的时候设置技能点时间
	if (skill_point == skil_point_max){
		update_skill_point_time(psu,true);
	}

	prs = psu->get_player_role().get_role_skill_mgr().get_skill(skill_id);
	fill_skill_data(prs, rsp_msg.mutable_new_skill());
	rsp_msg.set_result_type(0);
	psu->send_client(rsp_msg);
	if (req->has_pet_id()){
		// 美人XX|技能ID（技能名字）|消耗技能点1|剩余技能点前值XX|剩余技能点后值XX；铜钱XX，前值XX/后值XX；技能等级XX，前值XX/后值XX；
		const config_skill* skill_log = CONFMGR->get_config_skill(skill_id);
		if (skill_log != NULL){
			const config_beauty* pet_config = CONFMGR->get_config_beauty(pet_info.pet_id(),pet_info.pet_level());
			if (pet_config != NULL){
				amtsvr_billlog(ss->ctx, "%u|%s|%s|%s|%u|%u|%u|%u|%u|%u|%u", psu->get_role_id(), req->GetTypeName().c_str(), pet_config->get_general_name().c_str(),
					skill_log->get_skill_name().c_str(),1,skill_point,skill_point -1,cur_money,new_money,old_effect_level,new_effect_level);
			}
		}
	}else{
		amtsvr_billlog(ss->ctx, "%u|%s|%u", psu->get_role_id(), req->GetTypeName().c_str(), req->skill_id());
	}

	return NULL;
}

void* on_pb_role_skill_point_time_req(client::skill_point_time_req* req, void* ud, size_t* rsp_len)
{
	PB_FUNC_GUARD(req);

	update_skill_point_time(psu,false);

	amtsvr_billlog(ss->ctx, "%u|%s|%u", psu->get_role_id(), req->GetTypeName().c_str(), req->unixtime());

	return NULL;
}
// 宝石技能学习
void* on_pb_role_gem_skill_study_req(client::gem_skill_study_req* req, void* ud, size_t* rsp_len)
{
	PB_FUNC_GUARD(req);
	client::gem_skill_study_rsp rsp_msg;
	// 判断是否已经开了当前索引
	player_role_gem& player_gem = psu->get_player_role().get_player_role_gems();
	uint32 open_index = 0;
	open_index = CONFMGR->get_config_gem_act_mgr().get_config_gem_act(3,player_gem.get_gem_total_level());
	// 判断品阶能否学
	if (req->ser_index() > open_index){
		rsp_msg.set_result_type(3);
		psu->send_client(rsp_msg);
		return NULL;
	}

	// 判断是否能学习，检测现在有多少个宝石技能 
	uint32 skill_coun_max = CONFMGR->get_config_gem_act_mgr().get_gem_skill_max_count();

	if (psu->get_gem_skill_count()>= skill_coun_max){
		rsp_msg.set_result_type(4);
		psu->send_client(rsp_msg);
		return NULL;
	}

	const config_props* item_info = CONFMGR->get_config_props(req->item_id());
	if (item_info == NULL){
		rsp_msg.set_result_type(2);
		psu->send_client(rsp_msg);
		return NULL;
	}

	const std::vector<int>& trigger_list = 	item_info->get_trigger_list();
	const uint32 skill_id = trigger_list[0];
	player_role_skill* prs = psu->get_player_role().get_role_skill_mgr().get_skill(skill_id,req->ser_index());
	if (prs != NULL) {
		rsp_msg.set_result_type(1);
		psu->send_client(rsp_msg);
		return NULL;
	}

	skill* ps = psu->mutable_skill()->get_skill(skill_id,req->ser_index());
	if (ps != NULL) {
		rsp_msg.set_result_type(1);
		psu->send_client(rsp_msg);
		return NULL;
	}

	const config_skill* new_skill_base = CONFMGR->get_config_skill(skill_id);
	if (new_skill_base == NULL){
		rsp_msg.set_result_type(2);
		psu->send_client(rsp_msg);
		return NULL;
	}

	// 判断消耗
	uint32 cur_strategy = psu->mutable_attr()->get_val(MILITARY_STRATEGY);
	uint32 expend_money = new_skill_base->get_upgrade_need_yuan_li();
	if (cur_strategy < expend_money){
		rsp_msg.set_result_type(5);
		psu->send_client(rsp_msg);
		return NULL;
	}

	uint32 expend_id = new_skill_base->get_upgrade_need_item_id();
	uint32 expend_count = new_skill_base->get_upgrade_need_item_num();
	if (psu->get_props_num(expend_id) < expend_count){
		rsp_msg.set_result_type(5);
		psu->send_client(rsp_msg);
		return NULL;
	}

	skill* new_skill = new skill(*new_skill_base);
	new_skill->init(new_skill_base);
	new_skill->set_ser_index(req->ser_index());
	// 学了
	psu->mutable_skill()->add_skill(*new_skill,req->ser_index());

	psu->get_player_role().get_role_skill_mgr().add_skill(psu->get_role_id(), new_skill_base,req->ser_index());

	player_role_skill* new_prs = psu->get_player_role().get_role_skill_mgr().get_skill(new_skill_base->get_skill_id(),req->ser_index());
	if (new_prs == NULL){
		rsp_msg.set_result_type(2);
		psu->send_client(rsp_msg);
		return NULL;
	}

	// 扣除物品
	if (expend_count != 0){
		psu->sub_props(expend_id,expend_count);
		psu->send_client_change_props_msg();
	}

	if (expend_money != 0){
		psu->sub_money(MILITARY_STRATEGY,expend_money);
		psu->send_client_change_attr();
	}

	client::role_skill_msg skill_msg;
	psu->fill_role_data(skill_msg.mutable_rd());
	fill_skill_data(new_prs,skill_msg.add_sk());
	psu->send_client(skill_msg);

	rsp_msg.set_result_type(0);
	rsp_msg.set_ser_index(req->ser_index());

	psu->send_client(rsp_msg);

	return NULL;
}

// 宝石技能替换
void* on_pb_role_gem_skill_replace_req(client::gem_skill_replace_req* req, void* ud, size_t* rsp_len)
{
	PB_FUNC_GUARD(req);

	client::gem_skill_replace_rsp rsp_msg;
	const config_props* item_info = CONFMGR->get_config_props(req->item_id());
	if (item_info == NULL){
		rsp_msg.set_result_type(2);
		psu->send_client(rsp_msg);
		return NULL;
	}

	
	const std::vector<int>& trigger_list = 	item_info->get_trigger_list();
	const uint32 need_study_skill_id = trigger_list[0];
	player_role_skill* prs = psu->get_player_role().get_role_skill_mgr().get_skill(req->skill_id(),req->ser_index());
	if (prs == NULL) {
		return NULL;
	}

	skill* ps = psu->mutable_skill()->get_skill(req->skill_id(),req->ser_index());
	if (ps == NULL) {
		return NULL;
	}

	fill_skill_data(prs, rsp_msg.mutable_old_skill());

	const config_skill* upgrade_cs = CONFMGR->get_config_skill_mgr().get_config_skill(need_study_skill_id);
	if (upgrade_cs == NULL) {
		rsp_msg.set_result_type(2);
		psu->send_client(rsp_msg);
		return NULL;
	}

	// 判断消耗：
	uint32 cur_strategy = psu->mutable_attr()->get_val(MILITARY_STRATEGY);
	uint32 expend_money = upgrade_cs->get_upgrade_need_yuan_li();
	if (cur_strategy < expend_money){
		rsp_msg.set_result_type(3);
		psu->send_client(rsp_msg);
		return NULL;
	}

	uint32 expend_id = upgrade_cs->get_upgrade_need_item_id();
	uint32 expend_count = upgrade_cs->get_upgrade_need_item_num();
	if (psu->get_props_num(expend_id) < expend_count){
		rsp_msg.set_result_type(3);
		psu->send_client(rsp_msg);
		return NULL;
	}

	///////////////////////////////////////
	skill* new_skill = new skill(*upgrade_cs);
	new_skill->init(upgrade_cs);
	new_skill->set_ser_index(req->ser_index());

	psu->mutable_skill()->delete_skill(prs->get_skill_id(),prs->get_skill_ser_index());
	psu->mutable_skill()->add_skill(*new_skill,req->ser_index());

	psu->get_player_role().get_role_skill_mgr().delete_skill(prs->get_skill_id(),prs->get_skill_ser_index());
	psu->get_player_role().get_role_skill_mgr().add_skill(psu->get_role_id(), upgrade_cs,req->ser_index());

	player_role_skill* new_prs = psu->get_player_role().get_role_skill_mgr().get_skill(upgrade_cs->get_skill_id(),req->ser_index());
	if (new_prs == NULL){
		return NULL;
	}

	// 扣除物品
	if (expend_count != 0){
		psu->sub_props(expend_id,expend_count);
		psu->send_client_change_props_msg();
	}
	if (expend_money != 0){
		psu->sub_money(MILITARY_STRATEGY,expend_money);
		psu->send_client_change_attr();
	}

	fill_skill_data(new_prs, rsp_msg.mutable_new_skill());

	rsp_msg.set_result_type(0);

	psu->send_client(rsp_msg);


	return NULL;
}
