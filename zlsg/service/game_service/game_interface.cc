#include "client.offline.pb.h"
#include "client.scene_ready.pb.h"
#include "client.role_potential.pb.h"
#include "client.practice.pb.h"
#include "client.chat_msg.pb.h"
#include "client.mounts.pb.h"
#include "client.promo.pb.h"
#include "client.city.pb.h"
#include "client.target.pb.h"
#include "client.vip.pb.h"
#include "client.lottery.pb.h"
#include "client.team.pb.h"
#include "client.levels.pb.h"
#include "game_interface.h"
#include "msg_handler_pool.h"
#include "client.role_gem.pb.h"
#include "client.role_bows.pb.h"

#ifndef __LIB_MINI_SERVER__
#include "client.cycle.pb.h"
#include "client.devil.pb.h"
#include "inner.pay.pb.h"
#endif

// 进入场景
void* on_game_pb_scene_ready(client::scene_ready* p,  void* ud, size_t* rsp_len);
void game_reg_pb_scene(struct game_service_t* s);

// 跑
void* on_pb_role_move(client::role_move_msg* p,  void* ud, size_t* rsp_len);
void* on_pb_role_postion_msg(client::role_postion_msg* p, void* ud, size_t* rsp_len);
void game_reg_pb_move(struct game_service_t* s);

// 打
void* on_pb_attack(client::attack_req* p,  void* ud, size_t* rsp_len);
void* on_pb_nearby_npc_pos_req(client::nearby_npc_pos_req* req, void* ud, size_t* rsp_len);
void game_reg_pb_attack(struct game_service_t* s);

// 传送
void* on_pb_jump_scene(client::jump_scene* js, void* ud, size_t* rsp_len); // 客户端跳转请求
void* on_pb_jump_user(client::jump_user* ju, void* ud, size_t* rsp_len);  // 服务模块传递跳转指令
void game_reg_pb_jump(struct game_service_t* s);

// 竹蜻蜓
void* on_pb_use_props_req(client::use_props_req* req, void* ud, size_t* rsp_len);
void* on_pb_sell_props_req(client::sell_props_req* req, void* ud, size_t* rsp_len);
void* on_pb_move_props_req(client::move_props_req* req, void* ud, size_t* rsp_len);
void* on_pb_equip_props_req(client::equip_props_req* req, void* ud, size_t* rsp_len);
void* on_pb_baptize_req(client::baptize_req* req, void* ud, size_t* rsp_len);
void* on_pb_baptize_save_req(client::baptize_save_req* req, void* ud, size_t* rsp_len);
void* on_pb_upgrade_equip_req(client::upgrade_equip_req* req, void* ud, size_t* rsp_len);
void* on_pb_pick_up_props_req(client::pick_up_props_req* req, void* ud, size_t* rsp_len);
void* on_pb_melting_equip_req(client::melting_equip_req* req, void* ud, size_t* rsp_len);
void* on_pb_cast_equip_req(client::cast_equip_req* req, void* ud, size_t* rsp_len);
void* on_pb_treasure_req(client::treasure_req* req, void* ud, size_t* rsp_len);
void* on_pb_collect_explore_req(client::collect_explore_req* req, void* ud, size_t* rsp_len);
void* on_pb_black_shop_req(client::black_shop_req* req, void* ud, size_t* rsp_len);
void* on_pb_black_shop_reflesh_req(client::black_shop_reflesh_req* req, void* ud, size_t* rsp_len);
void* on_pb_black_shop_buy_req(client::black_shop_buy_req* req, void* ud, size_t* rsp_len);
void game_reg_pb_props(struct game_service_t* s);

// 伊妹儿
void* on_pb_mail_list_req(client::mail_list_req* mlr, void* ud, size_t* rsp_len);
void* on_pb_send_mail_req(client::send_mail_req* mlr, void* ud, size_t* rsp_len);
void* on_pb_mail_open_req(client::mail_open_req* mor, void* ud, size_t* rsp_len);
void* on_pb_delivery_mail_info(client::delivery_mail_info* dmi, void* ud, size_t* rsp_len);
void* on_pb_remove_mail_req(client::remove_mail_req* rmr, void* ud, size_t* rsp_len);
void* on_pb_extract_attachment_req(client::extract_attachment_req* ear, void* ud, size_t* rsp_len);
void* on_game_pb_send_mail_check_complete(client::send_mail_check_complete* req, void* ud, size_t* rsp_len);
void game_reg_pb_mail(struct game_service_t* s);

// 摆龙门阵
void* on_pb_scene_chat_msg(client::client_chat_msg* cm, void* ud, size_t* rsp_len);
void game_reg_pb_chat(struct game_service_t* s);

// 广交会
void* on_pb_buy_goods_req(client::buy_goods_req* b, void* ud, size_t* rsp_len);
void* on_pb_shop_item_list_req(client::shop_item_list_req* req, void* ud, size_t* rsp_len);
void* on_pb_sell_goods_req(client::sell_goods_req* s, void* ud, size_t* rsp_len);
void* on_pb_buy_mall_req(client::buy_mall_req* b, void* ud, size_t* rsp_len);
void* on_game_pb_order_tracking_req(client::order_tracking_req* req, void* ud, size_t* rsp_len);
void* on_game_pb_buy_tael_req(client::buy_tael_req* req, void* ud, size_t* rsp_len);
void game_reg_pb_mall(struct game_service_t* s);

// 工作
void* on_pb_ctrl_scene_req(client::ctrl_scene_req* req, void* ud, size_t* rsp_len);
void* on_pb_accept_task_req(client::accept_task_req* a, void* ud, size_t* rsp_len);
void* on_pb_submit_task_req(client::submit_task_req* s, void* ud, size_t* rsp_len);
void* on_pb_talk_task_req(client::talk_task_req* t, void* ud, size_t* rsp_len);
void* on_game_pb_story_end_req(client::story_end_req* t, void* ud, size_t* rsp_len);
void game_reg_pb_task(struct game_service_t* s);

// 百炼成精
void* on_pb_practice_req(client::practice_req* pr, void* ud, size_t* rsp_len);
void* on_pb_comm_practice_req(client::comm_practice_req* pr, void* ud, size_t* rsp_len);
void* on_pb_comm_practice_list_req(client::comm_practice_list_req* pr, void* ud, size_t* rsp_len);
void* on_pb_comm_practice_invite_result(client::comm_practice_invite_result* cpir, void* ud, size_t* rsp_len);
void* on_pb_auto_agree_comm_practice(client::auto_agree_comm_practice* cacp, void* ud, size_t* rsp_len);
void* on_game_pb_stop_comm_practice_req(client::stop_comm_practice_req* req, void* ud, size_t* rsp_len);
void* on_game_pb_activate_potential_req(client::act_potential_req* req, void* ud, size_t* rsp_len);
void* on_game_pb_potential_info_list_req(client::potential_info_list* req, void* ud, size_t* rsp_len);
void game_reg_pb_practice(struct game_service_t* s);

// 战场
void* on_pb_battle_enter_req(client::battle_enter_req* ber, void* ud, size_t* rsp_len);
void* on_pb_battle_leave_req(client::battle_leave_req* blr, void* ud, size_t* rsp_len);
void* on_game_pb_battle_honor_refund_req(client::battle_honor_refund_req* req, void* ud, size_t* rsp_len);
void* on_game_pb_battle_info_req(client::battle_info_req* req, void* ud, size_t* rsp_len);
void game_reg_pb_battle(struct game_service_t* s);

// 老板
void *on_pb_boss_refresh_req(client::boss_refresh_req *req, void *ud, size_t *rsp_len);
void game_reg_pb_boss(struct game_service_t* s);

// 头衔
void* on_pb_label_list_req(client::label_list_req* llr, void* ud, size_t* rsp_len);
void* on_pb_operate_label_req(client::operate_label_req* olr, void* ud, size_t* rsp_len);
void* on_game_pb_get_role_label_info_req(client::get_role_label_info_req* req, void* ud, size_t* rsp_len);
void game_reg_pb_label(struct game_service_t* s);

// VIP
void register_op_cb();
void *on_pb_vip_exp_req(client::vip_exp_req *req, void *ud, size_t *rsp_len);
void *on_pb_vip_op_times_req(client::vip_op_times_req *req, void *ud, size_t *rsp_len);
void *on_pb_vip_op_req(client::vip_op_req *req, void *ud, size_t *rsp_len);
void *on_pb_vip_buy_info_req(client::vip_buy_info_req *req, void *ud, size_t *rsp_len);
void game_reg_pb_vip(struct game_service_t* s);

// 爬塔登高望远
void* on_pb_tower_challenge_req(client::tower_challenge_req* tcr, void* ud, size_t* rsp_len);
void* on_pb_tower_auto_req(client::tower_auto_req* tar, void* ud, size_t* rsp_len);
void game_reg_pb_tower(struct game_service_t* s);

// 寻宝
void* on_pb_explore_secret_place_req(client::explore_secret_place_req* req, void* ud, size_t* rsp_len);
void* on_pb_last_explore_secret_place_req(client::last_explore_secret_place_req* req, void* ud, size_t* rsp_len);
void game_reg_pb_explore(struct game_service_t* s);

// 家族
void* on_game_pb_family_rsp(client::family_rsp* req, void* ud, size_t* rsp_len);
void* on_game_pb_create_family_req(client::create_family_req* req, void* ud, size_t* rsp_len);
void* on_pb_set_family_req(client::set_family_req* req, void *ud, size_t* rsp_len);
void* on_pb_set_create_famliy_consume(client::set_create_famliy_consume* req, void *ud, size_t* rsp_len);
void* on_pb_set_donate_consume(client::set_donate_consume* req, void *ud, size_t* rsp_len);
void* on_pb_update_player_power_to_smy(client::update_player_power_to_smy* req, void *ud, size_t* rsp_len);
void* on_game_pb_donate_family_req(client::donate_family_req* req, void* ud, size_t* rsp_len);
void* on_game_pb_secede_family_req(client::secede_family_req* req, void* ud, size_t* rsp_len);
void *on_pb_load_friend_family_rsp(client::load_friend_family_rsp *rsp, void *ud, size_t *rsp_len);
void *on_pb_update_friend_family_rsp(client::update_friend_family_rsp *rsp, void *ud, size_t *rsp_len);
void game_reg_pb_family(struct game_service_t* s);

// 活动
void* on_pb_activity_shop_c2g(client::activity_shop_c2g* req, void *ud, size_t* rsp_len);
void* on_pb_activity_reward_c2g(client::activity_reward_c2g* req, void *ud, size_t* rsp_len);
void* on_pb_activity_his_record_c2g(client::activity_his_record_c2g* req, void *ud, size_t* rsp_len);
void game_reg_pb_activity(struct game_service_t* s);
// 日常成就

void *on_pb_target_list_req(client::target_list_req *req, void *ud, size_t *rsp_len);
void *on_pb_target_prize_req(client::target_prize_req *req, void *ud, size_t *rsp_len);

// 技能
void* on_pb_role_skill_upgrade_req(client::role_skill_upgrade_req* req, void* ud, size_t* rsp_len);
void* on_pb_role_skill_effect_upgrade_req(client::role_skill_effect_upgrade_req* req, void* ud, size_t* rsp_len);
void* on_pb_role_skill_point_time_req(client::skill_point_time_req* req, void* ud, size_t* rsp_len);
void* on_pb_role_gem_skill_study_req(client::gem_skill_study_req* req, void* ud, size_t* rsp_len);
void* on_pb_role_gem_skill_replace_req(client::gem_skill_replace_req* req, void* ud, size_t* rsp_len);
void game_reg_pb_skill(struct game_service_t* s);

// 打架
void* on_pb_enter_raid_req(client::enter_raid_req* req, void* ud, size_t* rsp_len);
void* on_pb_raid_open_box_req(client::raid_open_box_req* req, void* ud, size_t* rsp_len);
void* on_pb_exit_raid_req(client::exit_raid_req* er, void* ud, size_t* rsp_len);
void* on_pb_auto_raid_req(client::auto_raid_req* req, void* ud, size_t* rsp_len);
void* on_pb_mini_server_req(client::mini_server_req* req, void* ud, size_t* rsp_len);
void* on_pb_reported_raid_req(client::reported_raid_req* req, void* ud, size_t* rsp_len);
void game_reg_pb_raid(struct game_service_t* s);

// 查询
void* on_pb_query_req(client::query_req* bir, void* ud, size_t* rsp_len);
void game_reg_pb_query(struct game_service_t* s);

// 统计
void* on_pb_role_statis_req(client::role_statis_req* req, void* ud, size_t* rsp_len);
void game_reg_pb_statis(struct game_service_t* s);
void* on_pb_sign_list_req(client::sign_list_req* req, void* ud, size_t* rsp_len);
void* on_pb_sign_in_req(client::sign_in_req* req, void* ud, size_t* rsp_len);
void* on_pb_land_reward_req(client::land_reward_req* req, void* ud, size_t* rsp_len);

// 状态
void* on_pb_change_status(client::change_status* p, void* ud, size_t* rsp_len);
void game_reg_pb_status(struct game_service_t* s);

// 设置
void* on_pb_shortcut_req(client::shortcut_req* req, void* ud, size_t* rsp_len);
void* on_pb_shortcut_set_req(client::shortcut_set_req* req, void* ud, size_t* rsp_len);
void* on_pb_handup_req(client::handup_req* req, void* ud, size_t* rsp_len);
void* on_pb_handup_set_req(client::handup_set_req* req, void* ud, size_t* rsp_len);
void game_reg_pb_set(struct game_service_t* s);

// 捡肥皂
void* on_pb_pick_req(client::pick_req* req, void* ud, size_t* rsp_len);
void game_reg_pb_pick(struct game_service_t* s);

// 组队
void *on_pb_create_team_req(client::create_team_req *req, void *ud, size_t *rsp_len);
void* on_pb_team_raid_enter_cmd(client::team_raid_enter_cmd* cmd, void* ud, size_t* rsp_len);
void *on_pb_team_add_robot(client::team_add_robot *req, void *ud, size_t *rsp_len);
void *on_pb_team_join_check(client::team_join_check *req, void *ud, size_t *rsp_len);
void game_reg_pb_team(struct game_service_t* s);

// 信春哥的几种方法
void* on_game_pb_select_relive_type_req(client::select_relive_type_req* req, void* ud, size_t* rsp_len);
void game_reg_pb_relive(struct game_service_t* s);

// 福利院
void* on_game_pb_welfare_online_req(client::welfare_online_req* req, void* ud, size_t* rsp_len);
void* on_game_pb_welfare_gifts_req(client::welfare_gifts_req* req, void* ud, size_t* rsp_len);
void* on_game_pb_welfare_yellow_vip_req(client::welfare_yellow_vip_req* req, void* ud, size_t* rsp_len);
void* on_game_pb_online_gifts_req(client::online_gifts_req* req, void* ud, size_t* rsp_len);
void* on_game_pb_get_gifts_req(client::get_gifts_req* req, void* ud, size_t* rsp_len);
void* on_game_pb_check_list_of_names_rsp(client::check_list_of_names_rsp* req, void* ud, size_t* rsp_len);
void game_reg_pb_welfare(struct game_service_t* s);

#ifndef __LIB_MINI_SERVER__
// 沙场百战
void *on_pb_cycle_challenge_req(client::cycle_challenge_req *req, void *ud, size_t *rsp_len);
void *on_pb_cycle_helpers_req(client::cycle_helpers_req *req, void *ud, size_t *rsp_len);
void *on_pb_cycle_mopup_req(client::cycle_mopup_req *req, void *ud, size_t *rsp_len);
void *on_pb_cycle_info_req(client::cycle_info_req *req, void *ud, size_t *rsp_len);
void *on_pb_cycle_update_record_rsp(client::cycle_update_record_rsp *req, void *ud, size_t *rsp_len);
#endif

// 好基友一被子
void* on_game_pb_friend_recommend_req(client::friend_recommend_req* req, void* ud, size_t* rsp_len);
void game_reg_pb_friend(struct game_service_t* s);

// 投资登陆宝
void* on_game_pb_invest_req(client::invest_req* req, void* ud, size_t* rsp_len);
void game_reg_pb_invest(struct game_service_t* s);

#ifndef __LIB_MINI_SERVER__

// 开宝箱
void *on_pb_lottery_time_req(client::lottery_time_req *req, void *ud, size_t *rsp_len);
void *on_pb_lottery_open_req(client::lottery_open_req *req, void *ud, size_t *rsp_len);


// 过关斩将
void *on_pb_levels_list_req(client::levels_list_req *req, void *ud, size_t *rsp_len);
void *on_pb_levels_reset_req(client::levels_reset_req *req, void *ud, size_t *rsp_len);

void game_reg_pb_levels(struct game_service_t *s);
// inner服务转发
void* on_game_pb_getBackpackGoodsList_req(inner::getBackpackGoodsList_req* req, void* ud, size_t* rsp_len);
void* on_game_pb_updateBackpackGoods_req(inner::updateBackpackGoods_req* req, void* ud, size_t* rsp_len);
void* on_game_pb_addPlayerRestict_req(inner::addPlayerRestict_req* req, void* ud, size_t* rsp_len);
void* on_game_pb_delPlayerRestict_req(inner::delPlayerRestict_req* req, void* ud, size_t* rsp_len);
void* on_game_pb_delivery_ingot_req(inner::delivery_ingot_req* req, void* ud, size_t* rsp_len);
void* on_game_pb_pay_req(inner::pay_req* req, void* ud, size_t* rsp_len);
void game_reg_pb_inner(struct game_service_t* s);

void *on_pb_game_devil_award_req(client::devil_award_req *req, void *ud, size_t *rsp_len);
void *on_pb_game_devil_fight_result(client::devil_fight_result *rsp, void *ud, size_t *rsp_len);
#endif // __LIB_MINI_SERVER__

// 鲜花礼仪
void* on_pb_send_flower(client::send_flower* req, void* ud, size_t* rsp_len);
void* on_pb_recive_flower(client::recive_flower* rf, void* ud, size_t* rsp_len);
void game_reg_pb_flower(struct game_service_t* s);

// 温泉
void* on_pb_enter_spa_req(client::enter_spa_req* req, void* ud, size_t* rsp_len);
void* on_pb_spa_action_req(client::spa_action_req* req, void* ud, size_t* rsp_len);
void game_reg_pb_spa(struct game_service_t* s);

// 竞技场
void *on_pb_arena_personal_info_req(client::arena_personal_info_req *req, void *ud, size_t *rsp_len);
void *on_pb_arena_challenge_req(client::arena_challenge_req *req, void *ud, size_t *rsp_len);
void *on_pb_arena_speed_up_req(client::arena_speed_up_req *req, void *ud, size_t *rsp_len);
void *on_pb_arena_load_target_data_rsp(client::load_target_data_rsp *rsp, void *ud, size_t *rsp_len);
void *on_pb_load_online_target_data_req(client::load_online_target_data_req *req, void *ud, size_t *rsp_len);
void *on_pb_arena_fight_begin(client::arena_fight_begin *req, void *ud, size_t *rsp_len);
void game_reg_pb_arena(struct game_service_t* s);

// 抢分
void* on_game_pb_filed_score_exchange_req(client::filed_score_exchange_req* req, void* ud, size_t* rsp_len);
void* on_game_pb_filed_score_find_req(client::filed_score_find_req* req, void* ud, size_t* rsp_len);
void game_reg_pb_filed(struct game_service_t* s);

// 搞基
void* on_pb_mate_create_msg(client::mate_create_msg* msg, void* ud, size_t* rsp_len);
void* on_pb_depth_mate_msg(client::depth_mate_msg* msg, void* ud, size_t* rsp_len);
void* on_pb_depth_mate_end_msg(client::depth_mate_end_msg* msg, void* ud, size_t* rsp_len);
void* on_pb_mate_remove_req(client::mate_remove_req* msg, void* ud, size_t* rsp_len);
void* on_game_pb_mate_award_req(client::mate_award_req* req, void* ud, size_t* rsp_len);
void* on_game_pb_mate_award_via(client::mate_award_via* via, void* ud, size_t* rsp_len);
void game_reg_pb_mate(struct game_service_t* s);

// center服务转发
void* on_pb_scene_player_info(client::trans_user* tr,  void* ud, size_t* rsp_len);
void* on_pb_offline(client::offline* p,  void* ud, size_t* rsp_len);
void* on_replace_scene_user(mysql::sp_login_auth* sla, void* ud, size_t* rsp_len);
void *on_pb_offline_timelong_req(client::offline_timelong_req *req, void *ud, size_t *len);
void *on_pb_offline_reward_req(client::offline_reward_req *req, void *ud, size_t *len);
void* on_pb_randnpc(client::randnpc* rn, void* ud, size_t* rsp_len);
void* on_pb_trans_back_user(server::trans_back_user* req, void* ud, size_t* rsp_len);
void game_reg_pb_center(struct game_service_t* s);

// 服务器时间
void* on_game_pb_server_time_req(client::server_time_req* req, void* ud, size_t* rsp_len);
void* on_pb_get_svr_start_time_req(client::get_svr_start_time_req* req, void* ud, size_t* rsp_len);
void game_reg_pb_time(struct game_service_t* s);

// 种田
void* on_pb_farm_create_req(client::farm_create_req* req, void* ud, size_t* rsp_len);
void* on_pb_farm_enter_req(client::farm_enter_req* req, void* ud, size_t* rsp_len);
void* on_pb_farm_sow_req(client::farm_sow_req* req, void* ud, size_t* rsp_len);
void* on_pb_update_farm_info(client::update_farm_info* ufi, void* ud, size_t* rsp_len);
void* on_pb_farm_steal_req(client::farm_steal_req* req, void* ud, size_t* rsp_len);
void* on_pb_farm_stolen_msg(client::farm_stolen_msg* msg, void* ud, size_t* rsp_len);
void* on_pb_farm_gain_msg(client::farm_gain_msg* msg, void* ud, size_t* rsp_len);
void* on_pb_farm_improve_req(client::farm_improve_req* req, void* ud, size_t* rsp_len);
void* on_pb_farm_gain_money_rsp(client::farm_gain_money_rsp* rsp, void* ud, size_t* rsp_len);
void game_reg_pb_farm(struct game_service_t* s);

// 骑马马
void* on_game_pb_receive_mounts_req(client::receive_mounts_req* req, void* ud, size_t* rsp_len);
void* on_game_pb_ride_mounts_req(client::ride_mounts_req* req, void* ud, size_t* rsp_len);
void* on_game_pb_upgrade_mounts_req(client::upgrade_mounts_req* req, void* ud, size_t* rsp_len);
void* on_game_pb_upgrade_mounts_skill_req(client::upgrade_mounts_skill_req* req, void* ud, size_t* rsp_len);
void game_reg_pb_mounts(struct game_service_t* s);
void* on_game_pb_mounts_bless_time_req(client::mounts_bless_time_req* req, void* ud, size_t* rsp_len);

// 弓箭系统
void* on_game_pb_upgrade_bows_req(client::upgrade_bows_req* req, void* ud, size_t* rsp_len);
void* on_game_pb_bows_bless_time_req(client::bows_bless_time_req* req, void* ud, size_t* rsp_len);
void game_reg_pb_bows(struct game_service_t* s);

// 美人
void* on_pb_pet_recruit_req(client::pet_recruit_req* req, void* ud, size_t* rsp_len);
void* on_pb_pet_lingering_req(client::pet_lingering_req* req, void* ud, size_t* rsp_len);
void* on_pb_pet_magic_upgrade_req(client::magic_upgrade_req* req, void* ud, size_t* rsp_len);
void* on_pb_pet_levelup_req(client::pet_levelup_req* req, void* ud, size_t* rsp_len);
void* on_pb_pet_list_req(client::pet_list_req* req, void* ud, size_t* rsp_len);
void* on_pb_pet_rest_or_fight_req(client::pet_rest_or_fight_req* req, void* ud, size_t* rsp_len);
void* on_game_pb_pet_change_mode_req(client::pet_change_mode_req* req, void* ud, size_t* rsp_len);
void* on_game_pb_pet_info_req(client::pet_info_req* req, void* ud, size_t* rsp_len);
void* on_game_pb_pet_fight_mode_change_req(client::pet_fight_mode_change_req* req, void* ud, size_t* rsp_len);
void* on_game_pb_pet_levels_set_req(client::pet_levels_set_req* req, void* ud, size_t* rsp_len);
void game_reg_pb_pets(struct game_service_t* s);

// 宝石
void* on_pb_gem_list_req(client::gem_list_req* req, void* ud, size_t* rsp_len);
void* on_pb_gem_level_up_req(client::gem_level_up_req* req, void* ud, size_t* rsp_len);
void game_reg_pb_gem(struct game_service_t* s);
// 攻城
void *on_pb_siege_enter_field(client::siege_enter_req *req, void *ud, size_t *rsp_len);
void *on_pb_siege_revive_req(client::siege_revive_req *req, void *ud, size_t *rsp_len);
void *on_pb_city_welfare_get_rsp(client::city_welfare_get_notify *rsp, void *ud, size_t *rsp_len);
void *on_pb_city_skill_modify(client::siege_skill_modify *req, void *ud, size_t *rsp_len);
void *on_pb_family_money_query_rsp(client::family_money_query_rsp *rsp, void *ud, size_t *rsp_len);
void *on_pb_siege_call_req(client::siege_call_req *req, void *ud, size_t *rsp_len);
void *on_pb_siege_call(client::siege_call *sc, void *ud, size_t *rsp_len);
void *on_pb_siege_personal_info_req(client::siege_personal_info_req *req, void *ud, size_t *rsp_len);
void *on_pb_siege_field_open(client::siege_field_open *sfo, void *ud, size_t *rsp_len);
void *on_pb_siege_start(client::siege_start *ss, void *ud, size_t *rsp_len);
void *on_pb_update_sculpture(client::update_sculpture *us, void *ud, size_t *rsp_len);
void game_reg_pb_city(struct game_service_t* s);

// test
void* on_pb_test(client::test* req, void* ud, size_t* rsp_len);

#ifdef __LIB_MINI_SERVER__
void* on_pb_miniserver(client::exit_miniserver* req, void* ud, size_t* rsp_len);
void* on_pb_update_role_attr_msg(client::update_role_attr_msg* msg, void* ud, size_t* rsp_len);
#endif // __LIB_MINI_SERVER__

int game_dispatcher_init(struct game_service_t* s)
{
    game_reg_pb_move(s);
    game_reg_pb_attack(s);
    game_reg_pb_scene(s);
    game_reg_pb_status(s);

#ifndef __LIB_MINI_SERVER__
    game_reg_pb_jump(s);
    game_reg_pb_props(s);
    game_reg_pb_mail(s);
    game_reg_pb_chat(s);
    game_reg_pb_mall(s);
    game_reg_pb_task(s);
    game_reg_pb_practice(s);
    game_reg_pb_battle(s);
    game_reg_pb_boss(s);
    game_reg_pb_label(s);
    game_reg_pb_vip(s);
    game_reg_pb_tower(s);
    game_reg_pb_explore(s);
    game_reg_pb_family(s);
    game_reg_pb_skill(s);
    game_reg_pb_raid(s);
    game_reg_pb_query(s);
    game_reg_pb_statis(s);
    game_reg_pb_set(s);
    game_reg_pb_pick(s);
    game_reg_pb_team(s);
    game_reg_pb_relive(s);
    game_reg_pb_welfare(s);
    game_reg_pb_friend(s);
    game_reg_pb_invest(s);
    game_reg_pb_inner(s);
    game_reg_pb_flower(s);
    game_reg_pb_spa(s);
    game_reg_pb_arena(s);
    game_reg_pb_filed(s);
    game_reg_pb_mate(s);
    game_reg_pb_center(s);
    game_reg_pb_time(s);
    game_reg_pb_farm(s);
    game_reg_pb_mounts(s);
	game_reg_pb_bows(s);
    game_reg_pb_pets(s);
    game_reg_pb_city(s);
	game_reg_pb_levels(s);
	game_reg_pb_activity(s);
	game_reg_pb_gem(s);
    // test
    s->pd.reg<client::test>("client.test", on_pb_test);

	// 日常成就
	s->pd.reg<client::target_list_req>("client.target_list_req", on_pb_target_list_req);
	s->pd.reg<client::target_prize_req>("client.target_prize_req", on_pb_target_prize_req);
	s->pd.reg<client::lottery_time_req>("client.lottery_time_req", on_pb_lottery_time_req);
	s->pd.reg<client::lottery_open_req>("client.lottery_open_req", on_pb_lottery_open_req);
	s->pd.reg<client::devil_award_req>("client.devil_award_req", on_pb_game_devil_award_req);
	s->pd.reg<client::devil_fight_result>("client.devil_fight_result", on_pb_game_devil_fight_result);

	scene_role_target::init_target_func();
#endif // __LIB_MINI_SERVER__


#ifdef __LIB_MINI_SERVER__
    s->pd.reg<client::exit_miniserver>("client.exit_miniserver", on_pb_miniserver);
    s->pd.reg<client::update_role_attr_msg>("client.update_role_attr_msg", on_pb_update_role_attr_msg);
    s->pd.reg<client::pet_fight_mode_change_req>("client.pet_fight_mode_change_req", on_game_pb_pet_fight_mode_change_req);
#endif // __LIB_MINI_SERVER__

#ifndef __LIB_MINI_SERVER__
    s->pd.reg<client::cycle_challenge_req>("client.cycle_challenge_req", on_pb_cycle_challenge_req);
    s->pd.reg<client::cycle_helpers_req>("client.cycle_helpers_req", on_pb_cycle_helpers_req);
    s->pd.reg<client::cycle_mopup_req>("client.cycle_mopup_req", on_pb_cycle_mopup_req);
    s->pd.reg<client::cycle_info_req>("client.cycle_info_req", on_pb_cycle_info_req);
    s->pd.reg<client::cycle_update_record_rsp>("client.cycle_update_record_rsp", on_pb_cycle_update_record_rsp);
#endif
    return 0;
}

void game_reg_pb_move(struct game_service_t *s)
{
    s->pd.reg<client::role_move_msg>("client.role_move_msg", on_pb_role_move);
    s->pd.reg<client::role_postion_msg>("client.role_postion_msg", on_pb_role_postion_msg);
}

void game_reg_pb_attack(struct game_service_t* s)
{
    s->pd.reg<client::attack_req>("client.attack_req", on_pb_attack);
    s->pd.reg<client::nearby_npc_pos_req>("client.nearby_npc_pos_req", on_pb_nearby_npc_pos_req);
}

void game_reg_pb_scene(struct game_service_t* s)
{
    s->pd.reg<client::scene_ready>("client.scene_ready", on_game_pb_scene_ready);
    s->pd.reg<client::ctrl_scene_req>("client.ctrl_scene_req", on_pb_ctrl_scene_req);
}

void game_reg_pb_status(struct game_service_t* s)
{
    s->pd.reg<client::change_status>("client.change_status", on_pb_change_status);
}

#ifndef __LIB_MINI_SERVER__

void game_reg_pb_jump(struct game_service_t* s)
{
    s->pd.reg<client::jump_scene>("client.jump_scene", on_pb_jump_scene);
    s->pd.reg<client::jump_user>("client.jump_user", on_pb_jump_user);
}

void game_reg_pb_props(struct game_service_t* s)
{
    s->pd.reg<client::use_props_req>("client.use_props_req", on_pb_use_props_req);
    s->pd.reg<client::sell_props_req>("client.sell_props_req", on_pb_sell_props_req);
    s->pd.reg<client::move_props_req>("client.move_props_req", on_pb_move_props_req);
    s->pd.reg<client::equip_props_req>("client.equip_props_req", on_pb_equip_props_req);
    s->pd.reg<client::baptize_req>("client.baptize_req", on_pb_baptize_req);
	s->pd.reg<client::baptize_save_req>("client.baptize_save_req", on_pb_baptize_save_req);
    s->pd.reg<client::upgrade_equip_req>("client.upgrade_equip_req", on_pb_upgrade_equip_req);
    s->pd.reg<client::pick_up_props_req>("client.pick_up_props_req", on_pb_pick_up_props_req);
    s->pd.reg<client::melting_equip_req>("client.melting_equip_req", on_pb_melting_equip_req);
    s->pd.reg<client::cast_equip_req>("client.cast_equip_req", on_pb_cast_equip_req);
    s->pd.reg<client::treasure_req>("client.treasure_req", on_pb_treasure_req);
	s->pd.reg<client::collect_explore_req>("client.collect_explore_req", on_pb_collect_explore_req);
	s->pd.reg<client::black_shop_req>("client.black_shop_req", on_pb_black_shop_req);
	s->pd.reg<client::black_shop_reflesh_req>("client.black_shop_reflesh_req", on_pb_black_shop_reflesh_req);
	s->pd.reg<client::black_shop_buy_req>("client.black_shop_buy_req", on_pb_black_shop_buy_req);
}

void game_reg_pb_levels(struct game_service_t *s)
{
	s->pd.reg<client::levels_list_req>("client.levels_list_req", on_pb_levels_list_req);
	s->pd.reg<client::levels_reset_req>("client.levels_reset_req", on_pb_levels_reset_req);
}

void game_reg_pb_mail(struct game_service_t* s)
{
    s->pd.reg<client::mail_list_req>("client.mail_list_req", on_pb_mail_list_req);
    s->pd.reg<client::send_mail_req>("client.send_mail_req", on_pb_send_mail_req);
    s->pd.reg<client::mail_open_req>("client.mail_open_req", on_pb_mail_open_req);
    s->pd.reg<client::delivery_mail_info>("client.delivery_mail_info", on_pb_delivery_mail_info);
    s->pd.reg<client::remove_mail_req>("client.remove_mail_req", on_pb_remove_mail_req);
    s->pd.reg<client::extract_attachment_req>("client.extract_attachment_req", on_pb_extract_attachment_req);
    s->pd.reg<client::send_mail_check_complete>("client.send_mail_check_complete", on_game_pb_send_mail_check_complete);
}

void game_reg_pb_chat(struct game_service_t* s)
{
    s->pd.reg<client::client_chat_msg>("client.client_chat_msg", on_pb_scene_chat_msg);
}

void game_reg_pb_mall(struct game_service_t* s)
{
    s->pd.reg<client::buy_goods_req>("client.buy_goods_req", on_pb_buy_goods_req);
	s->pd.reg<client::shop_item_list_req>("client.shop_item_list_req", on_pb_shop_item_list_req);
    s->pd.reg<client::sell_goods_req>("client.sell_goods_req", on_pb_sell_goods_req);
    s->pd.reg<client::buy_mall_req>("client.buy_mall_req", on_pb_buy_mall_req);
    s->pd.reg<client::order_tracking_req>("client.order_tracking_req", on_game_pb_order_tracking_req);
    s->pd.reg<client::buy_tael_req>("client.buy_tael_req", on_game_pb_buy_tael_req);
}

void game_reg_pb_task(struct game_service_t* s)
{
    s->pd.reg<client::accept_task_req>("client.accept_task_req", on_pb_accept_task_req);
    s->pd.reg<client::submit_task_req>("client.submit_task_req", on_pb_submit_task_req);
    s->pd.reg<client::talk_task_req>("client.talk_task_req", on_pb_talk_task_req);
    s->pd.reg<client::story_end_req>("client.story_end_req", on_game_pb_story_end_req);
}

void game_reg_pb_practice(struct game_service_t* s)
{
    s->pd.reg<client::practice_req>("client.practice_req", on_pb_practice_req);
    s->pd.reg<client::comm_practice_req>("client.comm_practice_req", on_pb_comm_practice_req);
    s->pd.reg<client::comm_practice_list_req>("client.comm_practice_list_req", on_pb_comm_practice_list_req);
    s->pd.reg<client::comm_practice_invite_result>("client.comm_practice_invite_result", on_pb_comm_practice_invite_result);
    s->pd.reg<client::auto_agree_comm_practice>("client.auto_agree_comm_practice", on_pb_auto_agree_comm_practice);
    s->pd.reg<client::stop_comm_practice_req>("client.stop_comm_practice_req", on_game_pb_stop_comm_practice_req);
    s->pd.reg<client::act_potential_req>("client.act_potential_req", on_game_pb_activate_potential_req);
    s->pd.reg<client::potential_info_list>("client.potential_info_list", on_game_pb_potential_info_list_req);
}

void game_reg_pb_battle(struct game_service_t* s)
{
    s->pd.reg<client::battle_enter_req>("client.battle_enter_req", on_pb_battle_enter_req);
    s->pd.reg<client::battle_leave_req>("client.battle_leave_req", on_pb_battle_leave_req);
    s->pd.reg<client::battle_honor_refund_req>("client.battle_honor_refund_req", on_game_pb_battle_honor_refund_req);
    s->pd.reg<client::battle_info_req>("client.battle_info_req", on_game_pb_battle_info_req);  
}

void game_reg_pb_boss(struct game_service_t* s)
{
    s->pd.reg<client::boss_refresh_req>("client.boss_refresh_req", on_pb_boss_refresh_req);
}

void game_reg_pb_label(struct game_service_t* s)
{
    s->pd.reg<client::label_list_req>("client.label_list_req", on_pb_label_list_req);
    s->pd.reg<client::operate_label_req>("client.operate_label_req", on_pb_operate_label_req);
    s->pd.reg<client::get_role_label_info_req>("client.get_role_label_info_req", on_game_pb_get_role_label_info_req);
}

void game_reg_pb_vip(struct game_service_t* s)
{
	register_op_cb();
	s->pd.reg<client::vip_exp_req>("client.vip_exp_req", on_pb_vip_exp_req);
	s->pd.reg<client::vip_op_times_req>("client.vip_op_times_req", on_pb_vip_op_times_req);
	s->pd.reg<client::vip_op_req>("client.vip_op_req", on_pb_vip_op_req);
	s->pd.reg<client::vip_buy_info_req>("client.vip_buy_info_req", on_pb_vip_buy_info_req);
}

void game_reg_pb_tower(struct game_service_t* s)
{
    s->pd.reg<client::tower_challenge_req>("client.tower_challenge_req", on_pb_tower_challenge_req);
    s->pd.reg<client::tower_auto_req>("client.tower_auto_req", on_pb_tower_auto_req);
}

void game_reg_pb_explore(struct game_service_t* s)
{
    s->pd.reg<client::explore_secret_place_req>("client.explore_secret_place_req", on_pb_explore_secret_place_req);
    s->pd.reg<client::last_explore_secret_place_req>("client.last_explore_secret_place_req", on_pb_last_explore_secret_place_req);
}

void game_reg_pb_family(struct game_service_t* s)
{
    s->pd.reg<client::family_rsp>("client.family_rsp", on_game_pb_family_rsp);
    s->pd.reg<client::create_family_req>("client.create_family_req", on_game_pb_create_family_req);
    //s->pd.reg<client::create_family_failure>("client.create_family_failure", on_pb_create_family_failure);
    s->pd.reg<client::set_family_req>("client.set_family_req", on_pb_set_family_req);
    s->pd.reg<client::set_create_famliy_consume>("client.set_create_famliy_consume", on_pb_set_create_famliy_consume);
    s->pd.reg<client::set_donate_consume>("client.set_donate_consume", on_pb_set_donate_consume);
	s->pd.reg<client::update_player_power_to_smy>("client.update_player_power_to_smy", on_pb_update_player_power_to_smy);
    s->pd.reg<client::donate_family_req>("client.donate_family_req", on_game_pb_donate_family_req);
	s->pd.reg<client::secede_family_req>("client.secede_family_req", on_game_pb_secede_family_req);
	s->pd.reg<client::load_friend_family_rsp>("client.load_friend_family_rsp", on_pb_load_friend_family_rsp);
	s->pd.reg<client::update_friend_family_rsp>("client.update_friend_family_rsp", on_pb_update_friend_family_rsp);
    //s->pd.reg<client::family_welfare_req>("client.family_welfare_req", on_game_pb_family_welfare_req);
    //s->pd.reg<client::jump_to_family_scene>("client.jump_to_family_scene", on_game_pb_jump_to_family_scene);
    //s->pd.reg<client::call_family_boss>("client.call_family_boss", on_game_pb_call_family_boss);
    //s->pd.reg<client::family_monster_boss_rsp>("client.family_monster_boss_rsp", on_game_pb_family_monster_boss_rsp);
    //s->pd.reg<client::inferno_struck_open_via>("client.inferno_struck_open_via", on_game_pb_inferno_struck_open_via);
    //s->pd.reg<client::inferno_struck_skill_req>("client.inferno_struck_skill_req", on_game_pb_inferno_struck_skill_req);
    //s->pd.reg<client::family_role_attr_req>("client.family_role_attr_req", on_game_pb_family_role_attr_req);
    //s->pd.reg<client::family_role_attr_upgrade_req>("client.family_role_attr_upgrade_req", on_game_pb_family_role_attr_upgrade_req);
    //s->pd.reg<client::family_role_attr_upgrade_via>("client.family_role_attr_upgrade_via", on_game_pb_family_role_attr_upgrade_via);
}

void game_reg_pb_activity(struct game_service_t* s)
{
	s->pd.reg<client::activity_shop_c2g>("client.activity_shop_c2g", on_pb_activity_shop_c2g);
	s->pd.reg<client::activity_reward_c2g>("client.activity_reward_c2g", on_pb_activity_reward_c2g);
	s->pd.reg<client::activity_his_record_c2g>("client.activity_his_record_c2g", on_pb_activity_his_record_c2g);
}

void game_reg_pb_skill(struct game_service_t* s)
{
    s->pd.reg<client::role_skill_upgrade_req>("client.role_skill_upgrade_req", on_pb_role_skill_upgrade_req);
    s->pd.reg<client::role_skill_effect_upgrade_req>("client.role_skill_effect_upgrade_req", on_pb_role_skill_effect_upgrade_req);
	s->pd.reg<client::skill_point_time_req>("client.skill_point_time_req", on_pb_role_skill_point_time_req);
	s->pd.reg<client::gem_skill_study_req>("client.gem_skill_study_req", on_pb_role_gem_skill_study_req);
	s->pd.reg<client::gem_skill_replace_req>("client.gem_skill_replace_req", on_pb_role_gem_skill_replace_req);
}

void game_reg_pb_raid(struct game_service_t* s)
{
    s->pd.reg<client::enter_raid_req>("client.enter_raid_req", on_pb_enter_raid_req);
    s->pd.reg<client::raid_open_box_req>("client.raid_open_box_req", on_pb_raid_open_box_req);
    s->pd.reg<client::exit_raid_req>("client.exit_raid_req", on_pb_exit_raid_req);
    s->pd.reg<client::auto_raid_req>("client.auto_raid_req", on_pb_auto_raid_req);
    s->pd.reg<client::mini_server_req>("client.mini_server_req", on_pb_mini_server_req);
    s->pd.reg<client::reported_raid_req>("client.reported_raid_req", on_pb_reported_raid_req);
}

void game_reg_pb_query(struct game_service_t* s)
{
    s->pd.reg<client::query_req>("client.query_req", on_pb_query_req);
}

void game_reg_pb_statis(struct game_service_t* s)
{
    s->pd.reg<client::role_statis_req>("client.role_statis_req", on_pb_role_statis_req);
	s->pd.reg<client::sign_in_req>("client.sign_in_req", on_pb_sign_in_req);
	s->pd.reg<client::sign_list_req>("client.sign_list_req", on_pb_sign_list_req);
	s->pd.reg<client::land_reward_req>("client.land_reward_req", on_pb_land_reward_req);
}

void game_reg_pb_set(struct game_service_t* s)
{
    s->pd.reg<client::shortcut_req>("client.shortcut_req", on_pb_shortcut_req);
    s->pd.reg<client::shortcut_set_req>("client.shortcut_set_req", on_pb_shortcut_set_req);
    s->pd.reg<client::handup_req>("client.handup_req", on_pb_handup_req);
    s->pd.reg<client::handup_set_req>("client.handup_set_req", on_pb_handup_set_req);
}

void game_reg_pb_pick(struct game_service_t* s)
{
    s->pd.reg<client::pick_req>("client.pick_req", on_pb_pick_req);
}

void game_reg_pb_team(struct game_service_t* s)
{
	s->pd.reg<client::create_team_req>("client.create_team_req", on_pb_create_team_req);
	s->pd.reg<client::team_raid_enter_cmd>("client.team_raid_enter_cmd", on_pb_team_raid_enter_cmd);
	s->pd.reg<client::team_add_robot>("client.team_add_robot", on_pb_team_add_robot);
	s->pd.reg<client::team_join_check>("client.team_join_check", on_pb_team_join_check);
}

void game_reg_pb_relive(struct game_service_t* s)
{
    s->pd.reg<client::select_relive_type_req>("client.select_relive_type_req", on_game_pb_select_relive_type_req);
}

void game_reg_pb_welfare(struct game_service_t* s)
{
    s->pd.reg<client::welfare_online_req>("client.welfare_online_req", on_game_pb_welfare_online_req);
    s->pd.reg<client::welfare_gifts_req>("client.welfare_gifts_req", on_game_pb_welfare_gifts_req);
    s->pd.reg<client::welfare_yellow_vip_req>("client.welfare_yellow_vip_req", on_game_pb_welfare_yellow_vip_req);
    s->pd.reg<client::online_gifts_req>("client.online_gifts_req", on_game_pb_online_gifts_req);
    s->pd.reg<client::get_gifts_req>("client.get_gifts_req", on_game_pb_get_gifts_req);
    s->pd.reg<client::check_list_of_names_rsp>("client.check_list_of_names_rsp", on_game_pb_check_list_of_names_rsp);
//    s->pd.reg<client::rank_check_rsp>("client.rank_check_rsp", on_game_pb_rank_check_rsp);
}

void game_reg_pb_friend(struct game_service_t* s)
{
    s->pd.reg<client::friend_recommend_req>("client.friend_recommend_req", on_game_pb_friend_recommend_req);
}

void game_reg_pb_invest(struct game_service_t* s)
{
    s->pd.reg<client::invest_req>("client.invest_req", on_game_pb_invest_req);
}

void game_reg_pb_inner(struct game_service_t* s)
{
    s->pd.reg<inner::getBackpackGoodsList_req>("inner.getBackpackGoodsList_req", on_game_pb_getBackpackGoodsList_req);
    s->pd.reg<inner::updateBackpackGoods_req>("inner.updateBackpackGoods_req", on_game_pb_updateBackpackGoods_req);
    s->pd.reg<inner::addPlayerRestict_req>("inner.addPlayerRestict_req", on_game_pb_addPlayerRestict_req);
    s->pd.reg<inner::delPlayerRestict_req>("inner.delPlayerRestict_req", on_game_pb_delPlayerRestict_req);
    s->pd.reg<inner::delivery_ingot_req>("inner.delivery_ingot_req", on_game_pb_delivery_ingot_req);
    s->pd.reg<inner::pay_req>("inner.pay_req", on_game_pb_pay_req);
}

void game_reg_pb_flower(struct game_service_t* s)
{
    s->pd.reg<client::send_flower>("client.send_flower", on_pb_send_flower);
    s->pd.reg<client::recive_flower>("client.recive_flower", on_pb_recive_flower);
}

void game_reg_pb_spa(struct game_service_t* s)
{
    s->pd.reg<client::enter_spa_req>("client.enter_spa_req", on_pb_enter_spa_req);
    s->pd.reg<client::spa_action_req>("client.spa_action_req", on_pb_spa_action_req);
}

void game_reg_pb_arena(struct game_service_t* s)
{
    s->pd.reg<client::arena_personal_info_req>("client.arena_personal_info_req", on_pb_arena_personal_info_req);
    s->pd.reg<client::arena_challenge_req>("client.arena_challenge_req", on_pb_arena_challenge_req);
    s->pd.reg<client::arena_speed_up_req>("client.arena_speed_up_req", on_pb_arena_speed_up_req);
    s->pd.reg<client::load_target_data_rsp>("client.load_target_data_rsp", on_pb_arena_load_target_data_rsp);
    s->pd.reg<client::load_online_target_data_req>("client.load_online_target_data_req", on_pb_load_online_target_data_req);
    s->pd.reg<client::arena_fight_begin>("client.arena_fight_begin", on_pb_arena_fight_begin);
}

void game_reg_pb_filed(struct game_service_t* s)
{
    s->pd.reg<client::filed_score_exchange_req>("client.filed_score_exchange_req", on_game_pb_filed_score_exchange_req);
    s->pd.reg<client::filed_score_find_req>("client.filed_score_find_req", on_game_pb_filed_score_find_req);
}

void game_reg_pb_mate(struct game_service_t* s)
{
    s->pd.reg<client::mate_create_msg>("client.mate_create_msg", on_pb_mate_create_msg);
    s->pd.reg<client::depth_mate_msg>("client.depth_mate_msg", on_pb_depth_mate_msg);
    s->pd.reg<client::depth_mate_end_msg>("client.depth_mate_end_msg", on_pb_depth_mate_end_msg);
    s->pd.reg<client::mate_remove_req>("client.mate_remove_req", on_pb_mate_remove_req);
    s->pd.reg<client::mate_award_req>("client.mate_award_req", on_game_pb_mate_award_req);
    s->pd.reg<client::mate_award_via>("client.mate_award_via", on_game_pb_mate_award_via);
}

void game_reg_pb_center(struct game_service_t* s)
{
    s->pd.reg<client::trans_user>("client.trans_user", on_pb_scene_player_info);
    s->pd.reg<client::offline>("client.offline", on_pb_offline);
	s->pd.reg<client::offline_reward_req>("client.offline_reward_req", on_pb_offline_reward_req);
	s->pd.reg<client::offline_timelong_req>("client.offline_timelong_req", on_pb_offline_timelong_req);
    s->pd.reg<mysql::sp_login_auth>("mysql.sp_login_auth", on_replace_scene_user);
    s->pd.reg<client::randnpc>("client.randnpc", on_pb_randnpc);
    s->pd.reg<server::trans_back_user>("server.trans_back_user", on_pb_trans_back_user);
}

void game_reg_pb_time(struct game_service_t* s)
{
    s->pd.reg<client::get_svr_start_time_req>("client.get_svr_start_time_req", on_pb_get_svr_start_time_req);
    s->pd.reg<client::server_time_req>("client.server_time_req", on_game_pb_server_time_req);
}

void game_reg_pb_farm(struct game_service_t* s)
{
    s->pd.reg<client::farm_create_req>("client.farm_create_req", on_pb_farm_create_req);
    s->pd.reg<client::farm_enter_req>("client.farm_enter_req", on_pb_farm_enter_req);
    s->pd.reg<client::farm_sow_req>("client.farm_sow_req", on_pb_farm_sow_req);
    s->pd.reg<client::update_farm_info>("client.update_farm_info", on_pb_update_farm_info);
    s->pd.reg<client::farm_steal_req>("client.farm_steal_req", on_pb_farm_steal_req);
    s->pd.reg<client::farm_stolen_msg>("client.farm_stolen_msg", on_pb_farm_stolen_msg);
    s->pd.reg<client::farm_gain_msg>("client.farm_gain_msg", on_pb_farm_gain_msg);
    s->pd.reg<client::farm_improve_req>("client.farm_improve_req", on_pb_farm_improve_req);
    s->pd.reg<client::farm_gain_money_rsp>("client.farm_gain_money_rsp", on_pb_farm_gain_money_rsp);
}

void game_reg_pb_mounts(struct game_service_t *s)
{
    s->pd.reg<client::receive_mounts_req>("client.receive_mounts_req", on_game_pb_receive_mounts_req);
    s->pd.reg<client::ride_mounts_req   >("client.ride_mounts_req"   , on_game_pb_ride_mounts_req   );
    s->pd.reg<client::upgrade_mounts_req>("client.upgrade_mounts_req", on_game_pb_upgrade_mounts_req);
    s->pd.reg<client::upgrade_mounts_skill_req>("client.upgrade_mounts_skill_req", on_game_pb_upgrade_mounts_skill_req);
	s->pd.reg<client::mounts_bless_time_req>("client.mounts_bless_time_req", on_game_pb_mounts_bless_time_req);
}

void game_reg_pb_bows(struct game_service_t* s)
{
	s->pd.reg<client::upgrade_bows_req>("client.upgrade_bows_req", on_game_pb_upgrade_bows_req);
	s->pd.reg<client::bows_bless_time_req>("client.bows_bless_time_req", on_game_pb_bows_bless_time_req);
}

void game_reg_pb_city(struct game_service_t *s)
{
	s->pd.reg<client::siege_enter_req>("client.siege_enter_req", on_pb_siege_enter_field);
	s->pd.reg<client::siege_revive_req>("client.siege_revive_req", on_pb_siege_revive_req);
	s->pd.reg<client::city_welfare_get_notify>("client.city_welfare_get_notify", on_pb_city_welfare_get_rsp);
	s->pd.reg<client::siege_skill_modify>("client.siege_skill_modify", on_pb_city_skill_modify);
	s->pd.reg<client::family_money_query_rsp>("client.family_money_query_rsp", on_pb_family_money_query_rsp);
	s->pd.reg<client::siege_call_req>("client.siege_call_req", on_pb_siege_call_req);
	s->pd.reg<client::siege_call>("client.siege_call", on_pb_siege_call);
	s->pd.reg<client::siege_personal_info_req>("client.siege_personal_info_req", on_pb_siege_personal_info_req);
	s->pd.reg<client::siege_field_open>("client.siege_field_open", on_pb_siege_field_open);
	s->pd.reg<client::siege_start>("client.siege_start", on_pb_siege_start);
	s->pd.reg<client::update_sculpture>("client.update_sculpture", on_pb_update_sculpture);
}

void game_reg_pb_pets(struct game_service_t* s)
{
   // 招募
   s->pd.reg<client::pet_recruit_req>("client.pet_recruit_req", on_pb_pet_recruit_req);
   // 缠绵
   s->pd.reg<client::pet_lingering_req>("client.pet_lingering_req", on_pb_pet_lingering_req);
   // 升级
   s->pd.reg<client::pet_levelup_req>("client.pet_levelup_req", on_pb_pet_levelup_req);
   // 宠物列表
   s->pd.reg<client::pet_list_req>("client.pet_list_req", on_pb_pet_list_req);
   // 宠物出战与休战
   s->pd.reg<client::pet_rest_or_fight_req>("client.pet_rest_or_fight_req", on_pb_pet_rest_or_fight_req);
   // 宠物神兵升级
   s->pd.reg<client::magic_upgrade_req>("client.magic_upgrade_req", on_pb_pet_magic_upgrade_req);
   // 宠物复活
   //s->pd.reg<client::pet_resurgence_req>("client.pet_resurgence_req", on_pb_pet_resurgence_req);
   // 宠物模式
   s->pd.reg<client::pet_change_mode_req>("client.pet_change_mode_req", on_game_pb_pet_change_mode_req);
   // 宠物单个信息
   s->pd.reg<client::pet_info_req>("client.pet_info_req", on_game_pb_pet_info_req);
   // 宠物战斗状态的切换
   s->pd.reg<client::pet_fight_mode_change_req>("client.pet_fight_mode_change_req", on_game_pb_pet_fight_mode_change_req);
   //  过关斩将设置美人出战
   s->pd.reg<client::pet_levels_set_req>("client.pet_levels_set_req", on_game_pb_pet_levels_set_req);
}

void game_reg_pb_gem(struct game_service_t* s)
{
	
	s->pd.reg<client::gem_list_req>("client.gem_list_req", on_pb_gem_list_req);
	s->pd.reg<client::gem_level_up_req>("client.gem_level_up_req", on_pb_gem_level_up_req);
}

#endif // #ifndef __LIB_MINI_SERVER__

int game_send_client(struct game_service_t* s, const client::server_head& head, google::protobuf::Message& msg)
{
    return game_send_client(s, head.client_id(), msg);
}

int game_send_client(struct game_service_t* s, uint32 client_id, google::protobuf::Message& msg)
{
    size_t msg_len = 0;
    void* buf = encode(msg, msg_len);
    if (buf == NULL) {
        return -1;
    }

    amtsvr_send(s->ctx, 0, client_id, PTYPE_CLIENT|PTYPE_TAG_DONTCOPY, 0, buf, msg_len);
    return 0;
}

int game_send_client(struct game_service_t* pgs, uint32 client_id, const char* name, const char* bin, uint32 len)
{
    void* buf = encode(name, bin, len);
    if (buf == NULL) {
        return -1;
    }

    amtsvr_send(pgs->ctx, 0, client_id, PTYPE_CLIENT | PTYPE_TAG_DONTCOPY, 0, buf, len);
    return 0;
}

int game_send_scene(struct game_service_t *s, uint32 svr_scene_id, google::protobuf::Message &msg)
{
	
    google::protobuf::Message* new_msg = create_msg(msg.GetTypeName());
    if (new_msg == NULL) {
        return -1;
    }
    new_msg->CopyFrom(msg);

    msg_handler* mh = MSG_HANDLER_POOL->pop();
    if (mh == NULL) {
        delete new_msg;
        return -2;
    }
    
    mh->msg = new_msg;

    return amtsvr_send(s->ctx, 0, svr_scene_id, PTYPE_CLIENT|PTYPE_TAG_DONTCOPY, 0, mh, sizeof(msg_handler));
}

int game_send_center(struct game_service_t* s, google::protobuf::Message& msg)
{
    google::protobuf::Message* new_msg = create_msg(msg.GetTypeName());
    if (new_msg == NULL) {
        return -1;
    }
    new_msg->CopyFrom(msg);

    msg_handler* mh = MSG_HANDLER_POOL->pop();
    if (mh == NULL) {
        delete new_msg;
        return -2;
    }
    
    mh->msg = new_msg;

    return amtsvr_sendname(s->ctx, CENTER_NAME, PTYPE_CLIENT|PTYPE_TAG_DONTCOPY, 0, mh, sizeof(msg_handler));
}

int game_broadcast_by_center(struct game_service_t* s,client::broadcast_area_to_center& rsp_msg)
{
	return game_send_center(s,rsp_msg);
}

int send_scene_area_msg(struct game_service_t* s,uint32 svr_scene_id,client::broadcast_area_rsp& area_msg)
{
	return game_send_scene(s,svr_scene_id,area_msg);
}

int game_send_chores(struct game_service_t* s, google::protobuf::Message& msg)
{
    google::protobuf::Message* new_msg = create_msg(msg.GetTypeName());
    if (new_msg == NULL) {
        return -1;
    }
    new_msg->CopyFrom(msg);

    msg_handler* mh = MSG_HANDLER_POOL->pop();
    if (mh == NULL) {
        delete new_msg;
        return -2;
    }
    
    mh->msg = new_msg;

    return amtsvr_sendname(s->ctx, CHORES_NAME, PTYPE_CLIENT|PTYPE_TAG_DONTCOPY, 0, mh, sizeof(msg_handler));
}

#ifndef __LIB_MINI_SERVER__
int game_send_inner(struct game_service_t* s, const inner::inner_head& head, google::protobuf::Message& msg)
{
    return game_send_client(s, head.client_id(), msg);
}
#endif // __LIB_MINI_SERVER__

int game_close_agent(struct game_service_t* s, uint32 agent_id)
{
    server::close_agent ca;
    ca.set_agent_id(agent_id);
    int ca_len = 0;
    void* ca_buf = svr_encode(ca, ca_len);
    if (ca_buf == NULL) {
        return -1;
    }

    return amtsvr_send(s->ctx, 0, agent_id, PTYPE_CLIENT|PTYPE_TAG_DONTCOPY, 0, ca_buf, (size_t)ca_len);
}

void* on_game_pb_server_time_req(client::server_time_req* req, void* ud, size_t* rsp_len)
{
    struct game_service_t* ss = (struct game_service_t*)ud;
    client::server_time_rsp rsp;
    rsp.set_unixtime((uint32)time(NULL));
    game_send_client(ss, req->svr_head(), rsp);
    
    return NULL;
}

