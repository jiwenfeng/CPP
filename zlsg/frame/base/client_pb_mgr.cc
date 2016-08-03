#include "client_pb_mgr.h"

client_pb_mgr::client_pb_mgr()
{
    init();
}

client_pb_mgr::~client_pb_mgr()
{

}

int client_pb_mgr::init()
{
    // 创建角色
    string type_name = "client.create_role_req";
    string dst_name = ROLE_NAME;
    cpbs.insert(type_name, dst_name);

    type_name = "client.role_info_rsp";
    dst_name = GAME_NAME;
    cpbs.insert(type_name, dst_name);

    type_name = "client.role_attr";
    dst_name = GAME_NAME;
    cpbs.insert(type_name, dst_name);

    type_name = "client.scene_ready";
    dst_name = GAME_NAME;
    cpbs.insert(type_name, dst_name);

    type_name = "client.role_move_msg";
    dst_name = GAME_NAME;
    cpbs.insert(type_name, dst_name);

    type_name = "client.attack_req";
    dst_name = GAME_NAME;
    cpbs.insert(type_name, dst_name);

    type_name = "client.nearby_npc_pos_req";
    dst_name = GAME_NAME;
    cpbs.insert(type_name, dst_name);

    // 技能相关
	// 升级技能
    type_name = "client.role_skill_upgrade_req";
    dst_name = GAME_NAME;
    cpbs.insert(type_name, dst_name);
	// 升级技能效果
	type_name = "client.role_skill_effect_upgrade_req";
	dst_name = GAME_NAME;
	cpbs.insert(type_name, dst_name);

	type_name = "client.skill_point_time_req";
	dst_name = GAME_NAME;
	cpbs.insert(type_name, dst_name);

	// 宝石技能学习 
	type_name = "client.gem_skill_study_req";
	dst_name = GAME_NAME;
	cpbs.insert(type_name, dst_name);

	// 宝石技能替换
	type_name = "client.gem_skill_replace_req";
	dst_name = GAME_NAME;
	cpbs.insert(type_name, dst_name);

    // 场景相关
    type_name = "client.jump_scene";
    dst_name = GAME_NAME;
    cpbs.insert(type_name, dst_name);

    // 道具相关
    type_name = "client.use_props_req";
    dst_name = GAME_NAME;
    cpbs.insert(type_name, dst_name);
    
	type_name = "client.sell_props_req";
	dst_name = GAME_NAME;
	cpbs.insert(type_name, dst_name);

    type_name = "client.move_props_req";
    dst_name = GAME_NAME;
    cpbs.insert(type_name, dst_name);
    
    type_name = "client.equip_props_req";
    dst_name = GAME_NAME;
    cpbs.insert(type_name, dst_name);

	type_name = "client.baptize_req";
	dst_name = GAME_NAME;
	cpbs.insert(type_name, dst_name);
	
	type_name = "client.baptize_save_req";
	dst_name = GAME_NAME;
	cpbs.insert(type_name, dst_name);

	type_name = "client.collect_explore_req";
	dst_name = GAME_NAME;
	cpbs.insert(type_name, dst_name);

	type_name = "client.black_shop_req";
	dst_name = GAME_NAME;
	cpbs.insert(type_name, dst_name);

	type_name = "client.black_shop_reflesh_req";
	dst_name = GAME_NAME;
	cpbs.insert(type_name, dst_name);

	type_name = "client.black_shop_buy_req";
	dst_name = GAME_NAME;
	cpbs.insert(type_name, dst_name);

    type_name = "client.pick_up_props_req";
    dst_name = GAME_NAME;
    cpbs.insert(type_name, dst_name);

    type_name = "client.upgrade_equip_req";
    dst_name = GAME_NAME;
    cpbs.insert(type_name, dst_name);

    type_name = "client.melting_equip_req";
    dst_name = GAME_NAME;
    cpbs.insert(type_name, dst_name);

    type_name = "client.cast_equip_req";
    dst_name = GAME_NAME;
    cpbs.insert(type_name, dst_name);

	type_name = "client.treasure_req";
	dst_name = GAME_NAME;
	cpbs.insert(type_name, dst_name);

    type_name = "client.drop_props_msg";
    dst_name = GAME_NAME;
    cpbs.insert(type_name, dst_name);

    type_name = "client.sort_pack";
    dst_name = GAME_NAME;
    cpbs.insert(type_name, dst_name);

    type_name = "client.lottery_req";
    dst_name = GAME_NAME;
    cpbs.insert(type_name, dst_name);

    type_name = "client.open_grid_req";
    dst_name = GAME_NAME;
    cpbs.insert(type_name, dst_name);

    type_name = "client.pet_skill_seal_req";
    dst_name = GAME_NAME;
    cpbs.insert(type_name, dst_name);

    type_name = "client.mail_list_req";
    dst_name = GAME_NAME;
    cpbs.insert(type_name, dst_name);

    type_name = "client.mail_open_req";
    dst_name = GAME_NAME;
    cpbs.insert(type_name, dst_name);

    type_name = "client.send_mail_req";
    dst_name = GAME_NAME;
    cpbs.insert(type_name, dst_name);

    type_name = "client.extract_attachment_req";
    dst_name = GAME_NAME;
    cpbs.insert(type_name, dst_name);

    type_name = "client.remove_mail_req";
    dst_name = GAME_NAME;
    cpbs.insert(type_name, dst_name);

    type_name = "client.client_chat_msg";
    dst_name = CENTER_NAME;
    cpbs.insert(type_name, dst_name);

    type_name = "client.buy_goods_req";
    dst_name = GAME_NAME;
    cpbs.insert(type_name, dst_name);

	type_name = "client.shop_item_list_req";
	dst_name = GAME_NAME;
	cpbs.insert(type_name, dst_name);

    type_name = "client.sell_goods_req";
    dst_name = GAME_NAME;
    cpbs.insert(type_name, dst_name);

    type_name = "client.buy_mall_req";
    dst_name = GAME_NAME;
    cpbs.insert(type_name, dst_name);

    type_name = "client.order_tracking_req";
    dst_name = GAME_NAME;
    cpbs.insert(type_name, dst_name);

    type_name = "client.buy_tael_req";
    dst_name = GAME_NAME;
    cpbs.insert(type_name, dst_name);

    // 任务
    type_name = "client.accept_task_req";
    dst_name = GAME_NAME;
    cpbs.insert(type_name, dst_name);

    type_name = "client.submit_task_req";
    dst_name = GAME_NAME;
    cpbs.insert(type_name, dst_name);

    type_name = "client.talk_task_req";
    dst_name = GAME_NAME;
    cpbs.insert(type_name, dst_name);

    type_name = "client.story_end_req";
    dst_name = GAME_NAME;
    cpbs.insert(type_name, dst_name);

    // 宠物
	///////////////////////////////////////
	type_name = "client.pet_recruit_req";
	dst_name = GAME_NAME;
	cpbs.insert(type_name, dst_name);

	type_name = "client.pet_lingering_req";
	dst_name = GAME_NAME;
	cpbs.insert(type_name, dst_name);

	type_name = "client.magic_upgrade_req";
	dst_name = GAME_NAME;
	cpbs.insert(type_name, dst_name);

	type_name = "client.pet_levelup_req";
	dst_name = GAME_NAME;
	cpbs.insert(type_name, dst_name);

	type_name = "client.pet_list_req";
	dst_name = GAME_NAME;
	cpbs.insert(type_name, dst_name);

	type_name = "client.pet_rest_or_fight_req";
	dst_name = GAME_NAME;
	cpbs.insert(type_name, dst_name);

	//type_name = "client.pet_resurgence_req";
	//dst_name = GAME_NAME;
	//cpbs.insert(type_name, dst_name);

	type_name = "client.pet_change_mode_req";
	dst_name = GAME_NAME;
	cpbs.insert(type_name, dst_name);

	type_name = "client.pet_info_req";
	dst_name = GAME_NAME;
	cpbs.insert(type_name, dst_name);
	// 美人副本战斗状态切换请求
	type_name = "client.pet_fight_mode_change_req";
	dst_name = GAME_NAME;
	cpbs.insert(type_name, dst_name);

	type_name = "client.pet_levels_set_req";
	dst_name = GAME_NAME;
	cpbs.insert(type_name, dst_name);

	////////////////////////////////////////
	// 宝石
	type_name = "client.gem_list_req";
	dst_name = GAME_NAME;
	cpbs.insert(type_name, dst_name);

	type_name = "client.gem_level_up_req";
	dst_name = GAME_NAME;
	cpbs.insert(type_name, dst_name);
	/////////////////////////////////////
    type_name = "client.practice_req";
    dst_name = GAME_NAME;
    cpbs.insert(type_name, dst_name);

    type_name = "client.comm_practice_req";
    dst_name = GAME_NAME;
    cpbs.insert(type_name, dst_name);

    type_name = "client.comm_practice_list_req";
    dst_name = GAME_NAME;
    cpbs.insert(type_name, dst_name);

    type_name = "client.comm_practice_invite_result";
    dst_name = GAME_NAME;
    cpbs.insert(type_name, dst_name);

    type_name = "client.auto_agree_comm_practice";
    dst_name = GAME_NAME;
    cpbs.insert(type_name, dst_name);

    type_name = "client.receive_wing_req";
    dst_name = GAME_NAME;
    cpbs.insert(type_name, dst_name);

    type_name = "client.wing_info_req";
    dst_name = GAME_NAME;
    cpbs.insert(type_name, dst_name);

    type_name = "client.wing_train_req";
    dst_name = GAME_NAME;
    cpbs.insert(type_name, dst_name);

    type_name = "client.wing_up_grade_req";
    dst_name = GAME_NAME;
    cpbs.insert(type_name, dst_name);

    type_name = "client.wing_skill_upgrade_req";
    dst_name = GAME_NAME;
    cpbs.insert(type_name, dst_name);

    type_name = "client.boss_info_req";
    dst_name = CENTER_NAME;
    cpbs.insert(type_name, dst_name);

    type_name = "client.jump_to_boss_req";
    dst_name = GAME_NAME;
    cpbs.insert(type_name, dst_name);

    type_name = "client.battle_status_req";
    dst_name = GAME_NAME;
    cpbs.insert(type_name, dst_name);
    
    type_name = "client.battle_info_req";
    dst_name = CENTER_NAME;
    cpbs.insert(type_name, dst_name);

    type_name = "client.battle_enter_req";
    dst_name = GAME_NAME;
    cpbs.insert(type_name, dst_name);

    type_name = "client.battle_leave_req";
    dst_name = GAME_NAME;
    cpbs.insert(type_name, dst_name);

    type_name = "client.battle_honor_refund_req";
    dst_name = GAME_NAME;
    cpbs.insert(type_name, dst_name);

    type_name = "client.query_req";
    dst_name = CENTER_NAME;
    cpbs.insert(type_name, dst_name);

    type_name = "client.friend_list_req";
    dst_name = CENTER_NAME;
    cpbs.insert(type_name, dst_name);

	type_name = "client.temporary_friend_list_req";
	dst_name = CENTER_NAME;
	cpbs.insert(type_name, dst_name);

	
    type_name = "client.friend_add_req";
    dst_name = CENTER_NAME;
    cpbs.insert(type_name, dst_name);

    type_name = "client.friend_add_ret";
    dst_name = CENTER_NAME;
    cpbs.insert(type_name, dst_name);

    type_name = "client.friend_del_req";
    dst_name = CENTER_NAME;
    cpbs.insert(type_name, dst_name);

    type_name = "client.friend_blk_req";
    dst_name = CENTER_NAME;
    cpbs.insert(type_name, dst_name);

    type_name = "client.friend_status_req";
    dst_name = CENTER_NAME;
    cpbs.insert(type_name, dst_name);

    type_name = "client.friend_recommend_req";
    dst_name = GAME_NAME;
    cpbs.insert(type_name, dst_name);

	type_name = "client.rank_list_req";
	dst_name = CENTER_NAME;
	cpbs.insert(type_name, dst_name);

    type_name = "client.label_list_req";
    dst_name = GAME_NAME;
    cpbs.insert(type_name, dst_name);

    type_name = "client.operate_label_req";
    dst_name = GAME_NAME;
    cpbs.insert(type_name, dst_name);

    type_name = "client.get_role_label_info_req";
    dst_name = CENTER_NAME;
    cpbs.insert(type_name, dst_name);

    type_name = "client.honor_label_list_req";
    dst_name = CENTER_NAME;
    cpbs.insert(type_name, dst_name);






    type_name = "client.vip_exp_req";
    dst_name = GAME_NAME;
    cpbs.insert(type_name, dst_name);

    type_name = "client.vip_op_times_req";
    dst_name = GAME_NAME;
    cpbs.insert(type_name, dst_name);

    type_name = "client.vip_op_req";
    dst_name = GAME_NAME;
    cpbs.insert(type_name, dst_name);

    type_name = "client.vip_buy_info_req";
    dst_name = GAME_NAME;
    cpbs.insert(type_name, dst_name);

    // 军团
    type_name = "client.family_list_req";
    dst_name = CENTER_NAME;
    cpbs.insert(type_name, dst_name);
    type_name = "client.family_req";
    dst_name = CENTER_NAME;
    cpbs.insert(type_name, dst_name);
    type_name = "client.create_family_req";
    dst_name = GAME_NAME;
    cpbs.insert(type_name, dst_name);
    type_name = "client.apply_family_req";
    dst_name = CENTER_NAME;
    cpbs.insert(type_name, dst_name);

	type_name = "client.family_set_req";
	dst_name = CENTER_NAME;
	cpbs.insert(type_name, dst_name);

	type_name = "client.secede_family_req";
	dst_name = GAME_NAME;
	cpbs.insert(type_name, dst_name);

	type_name = "client.expel_member_req";
	dst_name = CENTER_NAME;
	cpbs.insert(type_name, dst_name);

    type_name = "client.apply_family_list_req";
    dst_name = CENTER_NAME;
    cpbs.insert(type_name, dst_name);
    type_name = "client.apply_family_list_self_req";
    dst_name = CENTER_NAME;
    cpbs.insert(type_name, dst_name);
    type_name = "client.approval_family_req";
    dst_name = CENTER_NAME;
    cpbs.insert(type_name, dst_name);
    type_name = "client.invite_family_req";
    dst_name = CENTER_NAME;
    cpbs.insert(type_name, dst_name);
    type_name = "client.invite_family_list_req";
    dst_name = CENTER_NAME;
    cpbs.insert(type_name, dst_name);
    type_name = "client.agree_family_req";
    dst_name = CENTER_NAME;
    cpbs.insert(type_name, dst_name);
    type_name = "client.modify_family_post_req";
    dst_name = CENTER_NAME;
    cpbs.insert(type_name, dst_name);
    type_name = "client.family_member_list_req";
    dst_name = CENTER_NAME;
    cpbs.insert(type_name, dst_name);
    type_name = "client.modify_family_member_req";
    dst_name = CENTER_NAME;
    cpbs.insert(type_name, dst_name);
    type_name = "client.disband_family_req";
    dst_name = CENTER_NAME;
    cpbs.insert(type_name, dst_name);
    type_name = "client.donate_family_req";
    dst_name = GAME_NAME;
    cpbs.insert(type_name, dst_name);
    type_name = "client.upgrade_family_req";
    dst_name = CENTER_NAME;
    cpbs.insert(type_name, dst_name);
    type_name = "client.family_self_req";
    dst_name = CENTER_NAME;
    cpbs.insert(type_name, dst_name);
    type_name = "client.family_log_req";
    dst_name = CENTER_NAME;
    cpbs.insert(type_name, dst_name);
    //type_name = "client.family_skills_req";
    //dst_name = CENTER_NAME;
    //cpbs.insert(type_name, dst_name);
    //type_name = "client.family_skill_upgrade_req";
    //dst_name = CENTER_NAME;
    //cpbs.insert(type_name, dst_name);
    type_name = "client.family_record_req";
    dst_name = CENTER_NAME;
    cpbs.insert(type_name, dst_name);
    //type_name = "client.family_monster_boss_req";
    //dst_name = CENTER_NAME;
    //cpbs.insert(type_name, dst_name);
    //type_name = "client.family_monster_boss_set_req";
    //dst_name = CENTER_NAME;
    //cpbs.insert(type_name, dst_name);
    //type_name = "client.family_monster_boss_call_jump_req";
    //dst_name = CENTER_NAME;
    //cpbs.insert(type_name, dst_name);
    //type_name = "client.family_god_bless_req";
    //dst_name = CENTER_NAME;
    //cpbs.insert(type_name, dst_name);
    //type_name = "client.family_welfare_req";
    //dst_name = GAME_NAME;
    //cpbs.insert(type_name, dst_name);
	// 更换战旗形象
	type_name = "client.change_family_flag_id_req";
	dst_name = CENTER_NAME;
	cpbs.insert(type_name, dst_name);
	// 更换战旗名字
	type_name = "client.change_family_flag_name_req";
	dst_name = CENTER_NAME;
	cpbs.insert(type_name, dst_name);
	// 弹劾军团长
	type_name = "client.impeach_chief_req";
	dst_name = CENTER_NAME;
	cpbs.insert(type_name, dst_name);
	// 请求捐献列表
	type_name = "client.donate_list_req";
	dst_name = CENTER_NAME;
	cpbs.insert(type_name, dst_name);
	// 结盟 or 敌对
	type_name = "client.ally_family_req";
	dst_name = CENTER_NAME;
	cpbs.insert(type_name, dst_name);
	// 请求解除结盟 or 敌对
	type_name = "client.remove_foreign_req";
	dst_name = CENTER_NAME;
	cpbs.insert(type_name, dst_name);
	// 请求外交列表
	type_name = "client.ally_family_list_req";
	dst_name = CENTER_NAME;
	cpbs.insert(type_name, dst_name);
	///////////////////////////////////////////////
	// 活动协议
	// 活动购买请求
	type_name = "client.activity_info_req";
	dst_name = CENTER_NAME;
	cpbs.insert(type_name, dst_name);

	type_name = "client.activity_shop_req";
	dst_name = CENTER_NAME;
	cpbs.insert(type_name, dst_name);
	// 活动奖励领取请求
	type_name = "client.activity_reward_req";
	dst_name = CENTER_NAME;
	cpbs.insert(type_name, dst_name);

	/////////////////////////////////////////////
	// 弓箭系统：
	// 弓箭进阶
	type_name = "client.upgrade_bows_req";
	dst_name  = GAME_NAME;
	cpbs.insert(type_name, dst_name);

	// 弓箭进阶时间请求
	type_name = "client.bows_bless_time_req";
	dst_name  = GAME_NAME;
	cpbs.insert(type_name, dst_name);

	////////////////////////////////////////////
    type_name = "client.explore_secret_place_req";
    dst_name = GAME_NAME;
    cpbs.insert(type_name, dst_name);

    type_name = "client.last_explore_secret_place_req";
    dst_name = GAME_NAME;
    cpbs.insert(type_name, dst_name);

    type_name = "client.broadcast_explore_list_req";
    dst_name = CENTER_NAME;
    cpbs.insert(type_name, dst_name);

    type_name = "client.tower_challenge_req";
    dst_name = GAME_NAME;
    cpbs.insert(type_name, dst_name);

    type_name = "client.tower_leave_req";
    dst_name = GAME_NAME;
    cpbs.insert(type_name, dst_name);

    type_name = "client.tower_auto_req";
    dst_name = GAME_NAME;
    cpbs.insert(type_name, dst_name);

    type_name = "client.role_statis_req";
    dst_name = GAME_NAME;
    cpbs.insert(type_name, dst_name);

	type_name = "client.sign_list_req";
	dst_name = GAME_NAME;
	cpbs.insert(type_name, dst_name);

	type_name = "client.sign_in_req";
	dst_name = GAME_NAME;
	cpbs.insert(type_name, dst_name);

	type_name = "client.land_reward_req";
	dst_name = GAME_NAME;
	cpbs.insert(type_name, dst_name);

    type_name = "client.change_status";
    dst_name = GAME_NAME;
    cpbs.insert(type_name, dst_name);

    type_name = "client.shortcut_req";
    dst_name = GAME_NAME;
    cpbs.insert(type_name, dst_name);

    type_name = "client.shortcut_set_req";
    dst_name = GAME_NAME;
    cpbs.insert(type_name, dst_name);

    type_name = "client.handup_req";
    dst_name = GAME_NAME;
    cpbs.insert(type_name, dst_name);

    type_name = "client.handup_set_req";
    dst_name = GAME_NAME;
    cpbs.insert(type_name, dst_name);

    type_name = "client.pick_req";
    dst_name = GAME_NAME;
    cpbs.insert(type_name, dst_name);

    type_name = "client.select_relive_type_req";
    dst_name = GAME_NAME;
    cpbs.insert(type_name, dst_name);

    type_name = "client.welfare_online_req";
    dst_name = GAME_NAME;
    cpbs.insert(type_name, dst_name);

    type_name = "client.welfare_gifts_req";
    dst_name = GAME_NAME;
    cpbs.insert(type_name, dst_name);

    type_name = "client.welfare_yellow_vip_req";
    dst_name = GAME_NAME;
    cpbs.insert(type_name, dst_name);

    type_name = "client.online_gifts_req";
    dst_name = GAME_NAME;
    cpbs.insert(type_name, dst_name);

    type_name = "client.get_gifts_req";
    dst_name = GAME_NAME;
    cpbs.insert(type_name, dst_name);

    type_name = "client.invest_req";
    dst_name = GAME_NAME;
    cpbs.insert(type_name, dst_name);
    
    type_name = "client.yellow_vip_info_req";
    dst_name = CENTER_NAME;
    cpbs.insert(type_name, dst_name);

    type_name = "client.heart";
    dst_name = CENTER_NAME;
    cpbs.insert(type_name, dst_name);

    type_name = "client.auth_req";
    dst_name = ROLE_NAME;
    cpbs.insert(type_name, dst_name);

    type_name = "client.promo_list_of_names_req";
    dst_name = CENTER_NAME;
    cpbs.insert(type_name, dst_name);

    type_name = "client.open_server_time_req";
    dst_name = CENTER_NAME;
    cpbs.insert(type_name, dst_name);

    type_name = "client.role_postion_msg";
    dst_name = GAME_NAME;
    cpbs.insert(type_name, dst_name);

    type_name = "client.send_flower";
    dst_name = CENTER_NAME;
    cpbs.insert(type_name, dst_name);

    type_name = "client.enter_spa_req";
    dst_name = GAME_NAME;
    cpbs.insert(type_name, dst_name);

    type_name = "client.leave_spa_req";
    dst_name = GAME_NAME;
    cpbs.insert(type_name, dst_name);

    type_name = "client.spa_action_req";
    dst_name = GAME_NAME;
    cpbs.insert(type_name, dst_name);

    type_name = "client.arena_rank_info_req";
    dst_name = CENTER_NAME;
    cpbs.insert(type_name, dst_name);

    type_name = "client.arena_top_list_req";
    dst_name = CENTER_NAME;
    cpbs.insert(type_name, dst_name);
    
    type_name = "client.arena_chall_req";
    dst_name = CENTER_NAME;
    cpbs.insert(type_name, dst_name);

    type_name = "client.buy_arena_times_req";
    dst_name = GAME_NAME;
    cpbs.insert(type_name, dst_name);

    type_name = "client.get_svr_start_time_req";
    dst_name = GAME_NAME;
    cpbs.insert(type_name, dst_name);

    type_name = "client.arena_reward_req";
    dst_name = CENTER_NAME;
    cpbs.insert(type_name, dst_name);

	type_name = "client.arena_fight_begin";
	dst_name = GAME_NAME;
	cpbs.insert(type_name, dst_name);

    type_name = "client.get_land_tax_req";
    dst_name = CENTER_NAME;
    cpbs.insert(type_name, dst_name);

    type_name = "client.set_land_tax_req";
    dst_name = CENTER_NAME;
    cpbs.insert(type_name, dst_name);

    type_name = "client.land_lucky_star_req";
    dst_name = CENTER_NAME;
    cpbs.insert(type_name, dst_name);

    type_name = "client.get_land_tax_reward_req";
    dst_name = GAME_NAME;
    cpbs.insert(type_name, dst_name);

//    type_name = "client.siege_reward_ring_req";
//    dst_name = CENTER_NAME;
//    cpbs.insert(type_name, dst_name);

    type_name = "client.filed_score_exchange_req";
    dst_name = GAME_NAME;
    cpbs.insert(type_name, dst_name);

    type_name = "client.filed_score_find_req";
    dst_name = GAME_NAME;
    cpbs.insert(type_name, dst_name);

    type_name = "client.mate_info_req";
    dst_name = CENTER_NAME;
    cpbs.insert(type_name, dst_name);

    type_name = "client.mate_invite_req";
    dst_name = CENTER_NAME;
    cpbs.insert(type_name, dst_name);

    type_name = "client.depth_mate_req";
    dst_name = CENTER_NAME;
    cpbs.insert(type_name, dst_name);

    type_name = "client.mate_remove_req";
    dst_name = CENTER_NAME;
    cpbs.insert(type_name, dst_name);

    type_name = "client.mate_invite_rsp";
    dst_name = CENTER_NAME;
    cpbs.insert(type_name, dst_name);

    type_name = "client.depth_mate_req";
    dst_name = CENTER_NAME;
    cpbs.insert(type_name, dst_name);

    type_name = "client.depth_mate_rsp";
    dst_name = CENTER_NAME;
    cpbs.insert(type_name, dst_name);

    type_name = "client.depth_mate_end_req";
    dst_name = CENTER_NAME;
    cpbs.insert(type_name, dst_name);

    type_name = "client.mate_award_req";
    dst_name = GAME_NAME;
    cpbs.insert(type_name, dst_name);

    type_name = "client.aircraft_info_req";
    dst_name = GAME_NAME;
    cpbs.insert(type_name, dst_name);

    type_name = "client.aircraft_upgrade_req";
    dst_name = GAME_NAME;
    cpbs.insert(type_name, dst_name);

    type_name = "client.aircraft_use_req";
    dst_name = GAME_NAME;
    cpbs.insert(type_name, dst_name);

    type_name = "client.aircraft_receive_req";
    dst_name = GAME_NAME;
    cpbs.insert(type_name, dst_name);

    type_name = "client.inferno_struck_open_req";
    dst_name = CENTER_NAME;
    cpbs.insert(type_name, dst_name);

    type_name = "client.inferno_struck_skill_req";
    dst_name = GAME_NAME;
    cpbs.insert(type_name, dst_name);

    type_name = "client.inferno_struck_skill_req";
    dst_name = GAME_NAME;
    cpbs.insert(type_name, dst_name);

    type_name = "client.family_role_attr_req";
    dst_name = GAME_NAME;
    cpbs.insert(type_name, dst_name);

    type_name = "client.family_role_attr_upgrade_req";
    dst_name = GAME_NAME;
    cpbs.insert(type_name, dst_name);

    type_name = "client.sect_info_req";
    dst_name = CENTER_NAME;
    cpbs.insert(type_name, dst_name);

    type_name = "client.sect_battle_enter_req";
    dst_name = CENTER_NAME;
    cpbs.insert(type_name, dst_name);

    type_name = "client.sect_meter_reward_req";
    dst_name = CENTER_NAME;
    cpbs.insert(type_name, dst_name);

    type_name = "client.sect_meter_reward_req";
    dst_name = CENTER_NAME;
    cpbs.insert(type_name, dst_name);

    type_name = "client.server_time_req";
    dst_name = GAME_NAME;
    cpbs.insert(type_name, dst_name);

    type_name = "client.farm_create_req";
    dst_name = GAME_NAME;
    cpbs.insert(type_name, dst_name);

    type_name = "client.farm_info_req";
    dst_name = CHORES_NAME;
    cpbs.insert(type_name, dst_name);

    type_name = "client.farm_enter_req";
    dst_name = CHORES_NAME;
    cpbs.insert(type_name, dst_name);

    type_name = "client.farm_sow_req";
    dst_name = GAME_NAME;
    cpbs.insert(type_name, dst_name);
    
    type_name = "client.farm_steal_req";
    dst_name = GAME_NAME;
    cpbs.insert(type_name, dst_name);

    type_name = "client.farm_gain_req";
    dst_name = CHORES_NAME;
    cpbs.insert(type_name, dst_name);
    
    type_name = "client.farm_improve_req";
    dst_name = GAME_NAME;
    cpbs.insert(type_name, dst_name);

    type_name = "client.farm_gain_money_req";
    dst_name = CHORES_NAME;
    cpbs.insert(type_name, dst_name);

    type_name = "client.farm_record_req";
    dst_name = CHORES_NAME;
    cpbs.insert(type_name, dst_name);

    // 副本
    type_name = "client.enter_raid_req";
    dst_name = GAME_NAME;
    cpbs.insert(type_name, dst_name);

    type_name = "client.exit_raid_req";
    dst_name = GAME_NAME;
    cpbs.insert(type_name, dst_name);

    type_name = "client.raid_open_box_req";
    dst_name  = GAME_NAME;
    cpbs.insert(type_name, dst_name);

    type_name = "client.auto_raid_req";
    dst_name  = GAME_NAME;
    cpbs.insert(type_name, dst_name);

    type_name = "client.mini_server_req";
    dst_name  = GAME_NAME;
    cpbs.insert(type_name, dst_name);

    type_name = "client.reported_raid_req";
    dst_name  = GAME_NAME;
    cpbs.insert(type_name, dst_name);

    // 任务
    type_name = "client.ctrl_scene_req";
    dst_name  = GAME_NAME;
    cpbs.insert(type_name, dst_name);

    // 坐骑
    type_name = "client.receive_mounts_req";
    dst_name  = GAME_NAME;
    cpbs.insert(type_name, dst_name);

    type_name = "client.ride_mounts_req";
    dst_name  = GAME_NAME;
    cpbs.insert(type_name, dst_name);

    type_name = "client.upgrade_mounts_req";
    dst_name  = GAME_NAME;
    cpbs.insert(type_name, dst_name);

	type_name = "client.mounts_bless_time_req";
	dst_name  = GAME_NAME;
	cpbs.insert(type_name, dst_name);


    type_name = "client.upgrade_mounts_skill_req";
    dst_name  = GAME_NAME;
    cpbs.insert(type_name, dst_name);

	type_name = "client.act_potential_req";
	dst_name  = GAME_NAME;
	cpbs.insert(type_name, dst_name);

	type_name = "client.potential_info_list";
	dst_name  = GAME_NAME;
	cpbs.insert(type_name, dst_name);

	//type_name = "client.saparatism_region_info_req";
	//dst_name = CENTER_NAME;
	//cpbs.insert(type_name, dst_name);

	//type_name = "client.saparatism_region_award_req";
	//dst_name = GAME_NAME;
	//cpbs.insert(type_name, dst_name);

	type_name = "client.sa_battle_enter_req";
	dst_name = GAME_NAME;
	cpbs.insert(type_name, dst_name);

	type_name = "client.sa_capture_flag_req";
	dst_name = GAME_NAME;
	cpbs.insert(type_name, dst_name);

	//王城争霸相关
	type_name = "client.siege_revive_req";
	dst_name = GAME_NAME;
	cpbs.insert(type_name, dst_name);

	type_name = "client.siege_personal_info_req";
	dst_name = GAME_NAME;
	cpbs.insert(type_name, dst_name);

	type_name = "client.siege_enter_req";
	dst_name = GAME_NAME;
	cpbs.insert(type_name, dst_name);

    type_name = "client.city_info_req";
    dst_name = CENTER_NAME;
    cpbs.insert(type_name, dst_name);

    type_name = "client.city_mod_title_req";
    dst_name = CENTER_NAME;
    cpbs.insert(type_name, dst_name);

    type_name = "client.city_welfare_info_req";
    dst_name = CENTER_NAME;
    cpbs.insert(type_name, dst_name);

    type_name = "client.city_welfare_get_req";
    dst_name = CENTER_NAME;
    cpbs.insert(type_name, dst_name);

    type_name = "client.siege_call_accept_req";
    dst_name = CENTER_NAME;
    cpbs.insert(type_name, dst_name);

	type_name = "client.siege_call_req";
	dst_name = GAME_NAME;
	cpbs.insert(type_name, dst_name);

	type_name = "client.siege_scene_camp_req";
	dst_name = GAME_NAME;
	cpbs.insert(type_name, dst_name);

	type_name = "client.arena_personal_info_req";
	dst_name = GAME_NAME;
	cpbs.insert(type_name, dst_name);


	type_name = "client.arena_refresh_req";
	dst_name = GAME_NAME;
	cpbs.insert(type_name, dst_name);


	type_name = "client.arena_challenge_req";
	dst_name = GAME_NAME;
	cpbs.insert(type_name, dst_name);


	type_name = "client.arena_speed_up_req";
	dst_name = GAME_NAME;
	cpbs.insert(type_name, dst_name);

	type_name = "client.arena_speed_up_req";
	dst_name = GAME_NAME;
	cpbs.insert(type_name, dst_name);

	type_name = "client.arena_info_req";
	dst_name = CENTER_NAME;
	cpbs.insert(type_name, dst_name);

	type_name = "client.arena_record_req";
	dst_name = CENTER_NAME;
	cpbs.insert(type_name, dst_name);

	type_name = "client.arena_rank_list_req";
	dst_name = CENTER_NAME;
	cpbs.insert(type_name, dst_name);

	type_name = "client.arena_reward_req";
	dst_name = CENTER_NAME;
	cpbs.insert(type_name, dst_name);

	type_name = "client.target_list_req";
	dst_name = GAME_NAME;
	cpbs.insert(type_name, dst_name);

	type_name = "client.target_prize_req";
	dst_name = GAME_NAME;
	cpbs.insert(type_name, dst_name);

	type_name = "client.lottery_time_req";
	dst_name = GAME_NAME;
	cpbs.insert(type_name, dst_name);

	type_name = "client.lottery_open_req";
	dst_name = GAME_NAME;
	cpbs.insert(type_name, dst_name);

	type_name = "client.create_team_req";
	dst_name = GAME_NAME;
	cpbs.insert(type_name, dst_name);

	type_name = "client.join_team_req";
	dst_name = CENTER_NAME;
	cpbs.insert(type_name, dst_name);

	type_name = "client.fire_member_req";
	dst_name = CENTER_NAME;
	cpbs.insert(type_name, dst_name);

	type_name = "client.team_ready_req";
	dst_name = CENTER_NAME;
	cpbs.insert(type_name, dst_name);

	type_name = "client.team_enter_raid_req";
	dst_name = CENTER_NAME;
	cpbs.insert(type_name, dst_name);

	type_name = "client.team_enter_prepare_req";
	dst_name = CENTER_NAME;
	cpbs.insert(type_name, dst_name);

	type_name = "client.levels_list_req";
	dst_name = GAME_NAME;
	cpbs.insert(type_name, dst_name);

	type_name = "client.levels_reset_req";
	dst_name = GAME_NAME;
	cpbs.insert(type_name, dst_name);

	type_name = "client.offline_reward_req";
	dst_name = GAME_NAME;
	cpbs.insert(type_name, dst_name);

	type_name = "client.cycle_info_req";
	dst_name = GAME_NAME;
	cpbs.insert(type_name, dst_name);


	type_name = "client.cycle_challenge_req";
	dst_name = CENTER_NAME;
	cpbs.insert(type_name, dst_name);


	type_name = "client.cycle_helpers_req";
	dst_name = GAME_NAME;
	cpbs.insert(type_name, dst_name);


	type_name = "client.cycle_mopup_req";
	dst_name = GAME_NAME;
	cpbs.insert(type_name, dst_name);

	type_name = "client.devil_info_req";
	dst_name = CENTER_NAME;
	cpbs.insert(type_name, dst_name);

	type_name = "client.devil_award_req";
	dst_name = GAME_NAME;
	cpbs.insert(type_name, dst_name);
    return 0;
}

const char* client_pb_mgr::get_dst_name(const char* name) const
{
    std::string type_name = name;
    const std::string* ret = cpbs.find(type_name);
    if (ret == NULL) {
        return NULL;
    }

    return ret->c_str();
}

void client_pb_mgr::insert(const char* type_name, const char* dst_name)
{
    std::string tn = type_name;
    std::string dn = dst_name;
    cpbs.insert(tn, dn);
    script.insert(std::pair<std::string, std::string>(tn, dn));
}

bool client_pb_mgr::is_script(const char* type_name)
{
    return script.find(type_name) != script.end();
}
