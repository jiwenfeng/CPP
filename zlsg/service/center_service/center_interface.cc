#include "offline_role_mgr.h"
#include "config_mail.h"
#include "family_mgr.h"
#include "raid_center_mgr.h"
#include "randnpc_mgr.h"
#include "role_key.h"
#include "db_log.h"
#include "client.role_login.pb.h"
#include "client.trans_user.pb.h"
#include "client.jump_scene.pb.h"
#include "client.offline.pb.h"
#include "client.create_role.pb.h"
#include "client.city.pb.h"
#include "center_interface.h"
#include "config_edreward.h"
#include "player_role_skill_mgr.h"
#include "config_pet_skill.h"
#include "config_robot.h"
#include "config_activity_item.h"
#include "config_npc.h"
#include "config_open.h"
#include "config_mgr.h"
#include "client.promo.pb.h"
#include "client.open.pb.h"
#include "client.poll_activity.pb.h"
#include "mysql.t_upgradelog.pb.h"
#include "mysql.t_newtask.pb.h"
#include "mysql.t_login.pb.h"
#include "mysql.t_offline.pb.h"
#include "mysql.t_online.pb.h"
#include "mysql.t_recharge.pb.h"
#include "mysql.t_moneylog.pb.h"
#include "client.node.pb.h"
#include "inner.email.pb.h"
#include "inner.pay.pb.h"
#include "inner.online.pb.h"
#include "inner.bulletin.pb.h"
#include "inner.promotions.pb.h"
#include "inner.server.pb.h"
#include "client.devil.pb.h"

static void generate_arena_robot(center_service_t *w);

// 服务组件间发送过来的消息处理，可信任
void* on_pb_init_data_req(server::init_data_req* idr, void* ud, size_t* rsp_len);

void* on_pb_center_player_info(client::trans_user* tr, void* ud, size_t* rsp_len);
void* on_pb_center_jump_scene(client::jump_user* ju, void* ud, size_t* rsp_len);
void* on_pb_broadcast_msg_req(client::broadcast_msg_req* req, void* ud, size_t* rsp_len);
void* on_pb_delivery_mail(client::delivery_mail_info* dmi, void* ud, size_t* rsp_len);
void* on_center_pb_send_mail_check_role(client::send_mail_check_role* req, void* ud, size_t* rsp_len);

void* on_pb_center_offline(client::offline* p, void* ud, size_t* rsp_len);
void* on_pb_center_user_lose(client::user_lose* ul, void* ud, size_t* rsp_len);
void* on_pb_record_offline(server::record_offline_user* ro, void* ud, size_t* rsp_len);
void *on_pb_center_offline_load_image(client::offline_load_image *req, void *ud, size_t *len);

void* center_load_role_ret(server::load_role_ret* ret, void* ud, size_t* rsp_len);

// 客户端发送过来的消息处理
void* on_pb_center_chat_msg(client::client_chat_msg* cm, void* ud, size_t* rsp_len);

void* on_pb_center_update_title_info_req(client::update_title_info_req* req, void* ud, size_t* rsp_len);
// game to center
void* on_pb_center_broadcast_area_to_center(client::broadcast_area_to_center* req, void* ud, size_t* rsp_len);

void *on_pb_center_boss_info(client::boss_info_req *req, void *ud, size_t *rsp_len);

void *on_pb_center_refresh_msg(client::boss_refresh_msg_rsp *rsp, void *ud, size_t *rsp_len);
void *on_pb_center_boss_status(client::boss_status *bs, void *ud, size_t *rsp_len);

void* on_pb_center_query_req(client::query_req* bir, void* ud, size_t* rsp_len);
void *on_center_pb_update_smy_info(client::update_smy_info *req, void *ud, size_t *rsp);
void* on_pb_gamet_to_center_query_req(client::game_to_center_query* bir, void* ud, size_t* rsp_len);

void* on_pb_friend_list_req(client::friend_list_req* flr, void* ud, size_t* rsp_len);
void* on_pb_temporary_friend_list_req(client::temporary_friend_list_req* flr, void* ud, size_t* rsp_len);
void* on_pb_friend_add_req(client::friend_add_req* req, void* ud, size_t* rsp_len);
void* on_pb_friend_del_req(client::friend_del_req* fdr, void* ud, size_t* rsp_len);
void* on_pb_friend_blk_req(client::friend_blk_req* fdr, void* ud, size_t* rsp_len);
void* on_pb_friend_status_req(client::friend_status_req* req, void* ud, size_t* rsp_len);
void* on_pb_friend_degree_add(client::friend_degree_add* req, void* ud, size_t* rsp_len);

void *on_pb_rank_list_req(client::rank_list_req *req, void *ud, size_t *rsp_len);
void *on_pb_rank_add_data(client::rank_add_data *raa, void *ud, size_t *rsp_len);
void *on_pb_act_rank_add_data(client::act_rank_add_data *raa, void *ud, size_t *rsp_len);
void* on_after_role_login(mysql::sp_login_auth* sla, void* ud, size_t* rsp_len);

void *on_pb_center_create_team_req(client::create_team_req *req, void *ud, size_t *rsp_len);
void *on_pb_center_fire_member_req(client::fire_member_req *req, void *ud, size_t *rsp_len);
void *on_pb_center_join_team_req(client::join_team_req *req, void *ud, size_t *rsp_len);
void *on_pb_center_team_ready_req(client::team_ready_req *req, void *ud, size_t *rsp_len);
void *on_pb_center_team_notify(client::team_notify *req, void *ud, size_t *rsp_len);
void *on_pb_center_team_enter_raid_req(client::team_enter_raid_req *req, void *ud, size_t *rsp_len);
void *on_pb_center_team_enter_prepare_req(client::team_enter_prepare_req *req, void *ud, size_t *rsp_len);
void *on_pb_center_team_join_check(client::team_join_check *req, void *ud, size_t *rsp_len);

void* on_pb_honor_label_list_req(client::honor_label_list_req* req, void* ud, size_t* rsp_len);
void* on_center_pb_get_role_label_info_req(client::get_role_label_info_req* req, void* ud, size_t* rsp_len);

void *on_pb_center_open_server_time_req(client::open_server_time_req *req, void *ud, size_t *rsp_len);

void * on_pb_center_open_func_req(client::open_func_req *req, void *ud, size_t *rsp_len);


// 军团
void  on_center_pb_reg_family(struct center_service_t* w);
void* on_center_pb_family_list_req(client::family_list_req* req, void* ud, size_t* rsp_len);
void* on_center_pb_family_req(client::family_req* req, void* ud, size_t* rsp_len);
void* on_center_pb_create_family_req(client::create_family_req* req, void* ud, size_t* rsp_len);
void* on_center_pb_secede_family_to_center(client::secede_family_to_center* req, void* ud, size_t* rsp_len);
void* on_center_pb_update_family_data_from_center(client::update_family_data_from_center* req, void* ud, size_t* rsp_len);
void* on_center_pb_notive_center_donate_sucess(client::notive_center_donate_sucess* req, void* ud, size_t* rsp_len);
void* on_center_pb_apply_family_req(client::apply_family_req* req, void* ud, size_t* rsp_len);
void* on_center_pb_family_set_req(client::family_set_req* req, void* ud, size_t* rsp_len);
void* on_center_pb_apply_family_list_req(client::apply_family_list_req* req, void* ud, size_t* rsp_len);
void* on_center_pb_apply_family_list_self_req(client::apply_family_list_self_req* req, void* ud, size_t* rsp_len);
void* on_center_pb_approval_family_req(client::approval_family_req* req, void* ud, size_t* rsp_len);
void* on_center_pb_invite_family_req(client::invite_family_req* req, void* ud, size_t* rsp_len);
void* on_center_pb_invite_family_list_req(client::invite_family_list_req* req, void* ud, size_t* rsp_len);
void* on_center_pb_agree_family_req(client::agree_family_req* req, void* ud, size_t* rsp_len);
void* on_center_pb_modify_family_post_req(client::modify_family_post_req* req, void* ud, size_t* rsp_len);
void* on_center_pb_family_member_list_req(client::family_member_list_req* req, void* ud, size_t* rsp_len);
void* on_center_pb_modify_family_member_req(client::modify_family_member_req* req, void* ud, size_t* rsp_len);
void* on_center_pb_expel_member_req(client::expel_member_req* req, void* ud, size_t* rsp_len);
void* on_center_pb_disband_family_req(client::disband_family_req* req, void* ud, size_t* rsp_len);
void* on_center_pb_donate_family_req(client::set_family_money* req, void* ud, size_t* rsp_len);
void* on_center_pb_upgrade_family_req(client::upgrade_family_req* req, void* ud, size_t* rsp_len);
void* on_center_pb_family_self_req(client::family_self_req* req, void* ud, size_t* rsp_len);
void* on_center_pb_family_log_req(client::family_log_req* req, void* ud, size_t* rsp_len);
void* on_center_pb_family_record_req(client::family_record_req* req, void* ud, size_t* rsp_len);
void* on_center_pb_modify_family_power(client::modify_family_power* req, void* ud, size_t* rsp_len);
void* on_center_pb_family_record_update(client::family_record_update* req, void* ud, size_t* rsp_len);
void* on_center_pb_family_log_update(client::family_log_update* req, void* ud, size_t* rsp_len);
void *on_center_pb_family_money_mod_req(client::family_money_mod_req *req, void *ud, size_t *rsp_len);
void* on_center_pb_query_family_money_req(client::family_money_query_req *req, void *ud, size_t *rsp_len);
void* on_center_pb_change_family_flag_id_req(client::change_family_flag_id_req *req, void *ud, size_t *rsp_len);
void* on_center_pb_change_family_flag_name_req(client::change_family_flag_name_req *req, void *ud, size_t *rsp_len);
void* on_center_pb_impeach_chief_req(client::impeach_chief_req *req, void *ud, size_t *rsp_len);
void* on_center_pb_donate_list_req(client::donate_list_req *req, void *ud, size_t *rsp_len);
void* on_center_pb_ally_family_req(client::ally_family_req *req, void *ud, size_t *rsp_len);
void* on_center_pb_remove_foreign_req(client::remove_foreign_req *req, void *ud, size_t *rsp_len);
void* on_center_pb_ally_family_list_req(client::ally_family_list_req *req, void *ud, size_t *rsp_len);
void *on_center_pb_load_friend_family_req(client::load_friend_family_req *rqe, void *ud, size_t *rsp_len);
// 活动
void  on_center_pb_reg_activity(struct center_service_t* w);
void* on_center_pb_activity_info_req(client::activity_info_req* req, void* ud, size_t* rsp_len);
void* on_center_pb_activity_shop_req(client::activity_shop_req* req, void* ud, size_t* rsp_len);
void* on_center_pb_activity_reward_req(client::activity_reward_req* req, void* ud, size_t* rsp_len);
void* on_center_pb_activity_shop_g2c(client::activity_shop_g2c* req, void* ud, size_t* rsp_len);
void* on_center_pb_activity_reward_g2c(client::activity_reward_g2c* req, void* ud, size_t* rsp_len);
void* on_center_pb_activity_his_record_g2c(client::activity_his_record_g2c* req, void* ud, size_t* rsp_len);
void* on_center_pb_update_activity_record_g2c(client::update_activity_record_g2c* req, void* ud, size_t* rsp_len);
void* on_center_pb_load_activity_list_center(client::load_activity_list_center* req, void* ud, size_t* rsp_len);


void* on_pb_broadcast_explore_list_req(client::broadcast_explore_list_req* req, void* ud, size_t* rsp_len);
void* on_pb_explore_update_list(client::explore_update_list* req, void* ud, size_t* rsp_len);

void* on_center_pb_battle_info_req(client::battle_info_req* req, void* ud, size_t* rsp_len);

void* on_center_pb_yellow_vip_info_req(client::yellow_vip_info_req* req, void* ud, size_t* rsp_len);

void* on_pb_center_create_role_check_msg(client::create_role_check_msg* msg, void* ud, size_t* rsp_len);

void* on_center_pb_send_flower(client::send_flower* req, void* ud, size_t* rsp_len);
void* on_center_pb_recive_flower(client::recive_flower* rf, void* ud, size_t* rsp_len);

void* on_pb_center_trans_back_user(server::trans_back_user* req, void* ud, size_t* rsp_len);

void *on_pb_center_arena_reward_req(client::arena_reward_req *req, void *ud, size_t *rsp_len);
void *on_pb_center_arena_rank_list_req(client::arena_rank_list_req *req, void *ud, size_t *rsp_len);
void *on_pb_arena_info_req(client::arena_info_req *api, void *ud, size_t *rsp_len);
void *on_pb_load_target_data_req(client::load_target_data_req *req, void *ud, size_t *rsp_len);
void *on_pb_arena_fight_ret(client::arena_fight_ret *ret, void *ud, size_t *rsp_len);
void *on_pb_arena_record_req(client::arena_record_req *req, void *ud, size_t *rsp_len);

void* on_center_pb_battle_reward_via(client::battle_reward_via* req, void* ud, size_t* rsp_len);

void* on_center_pb_mate_info_req(client::mate_info_req* req, void* ud, size_t* rsp_len);
void* on_center_pb_mate_invite_req(client::mate_invite_req* req, void* ud, size_t* rsp_len);
void* on_center_pb_mate_invite_rsp(client::mate_invite_rsp* rsp, void* ud, size_t* rsp_len);
void* on_center_pb_mate_create_msg(client::mate_create_msg* msg, void* ud, size_t* rsp_len);
void* on_center_pb_depth_mate_req(client::depth_mate_req* req, void* ud, size_t* rsp_len);
void* on_center_pb_depth_mate_rsp(client::depth_mate_rsp* rsp, void* ud, size_t* rsp_len);
void* on_center_pb_depth_mate_msg(client::depth_mate_msg* msg, void* ud, size_t* rsp_len);
void* on_center_pb_depth_mate_end_req(client::depth_mate_end_req* msg, void* ud, size_t* rsp_len);
void* on_center_pb_mate_remove_msg(client::mate_remove_msg* msg, void* ud, size_t* rsp_len);
void* on_center_pb_mate_remove_req(client::mate_remove_req* req, void* ud, size_t* rsp_len);
void* on_center_pb_mate_award_req(client::mate_award_req* msg, void* ud, size_t* rsp_len);

void* on_center_pb_farm_entered_msg(client::farm_entered_msg* req, void* ud, size_t* rsp_len);
void* on_center_pb_farm_stolen_msg(client::farm_stolen_msg* req, void* ud, size_t* rsp_len);

//王城争霸
void on_center_pb_reg_city(struct center_service_t *w);
void* on_pb_center_city_info_req(client::city_info_req* req, void* ud, size_t* rsp_len);
void *on_pb_center_city_mod_title_req(client::city_mod_title_req* req, void* ud, size_t* rsp_len);
void *on_pb_center_city_welfare_info_req(client::city_welfare_info_req *req, void *ud, size_t *rsp_len);
void *on_pb_center_city_welfare_get_req(client::city_welfare_get_req *req, void *ud, size_t *rsp_len);
void *on_pb_center_siege_over(client::siege_over* rsp, void* ud, size_t* rsp_len);
void *on_pb_center_siege_status(client::siege_status *req, void *ud, size_t *rsp_len);
void *on_pb_center_siege_flag_occupy(client::siege_flag_occupy *req, void *ud, size_t *rsp_len);
void *on_pb_center_city_welfare_get_confirm(client::city_welfare_get_confirm *req, void *ud, size_t *rsp_len);
void *on_pb_siege_call_accept_req(client::siege_call_accept_req *req, void *ud, size_t *rsp_len);
void *on_pb_center_broadcast_siege_status(client::siege_status_rsp *rsp, void *ud, size_t *rsp_len);
void *on_pb_center_king_skill_add(client::king_skill_add *ksa, void *ud, size_t *rsp_len);
void *on_pb_center_siege_start(client::siege_start *req, void *ud, size_t *rsp_len);

// 登陆相关
void on_center_pb_reg_login(struct center_service_t* w);
void* on_center_pb_auto_login_req(client::auto_login_req* req, void* ud, size_t* rsp_len);
void* on_center_pb_update_auto_login_key(client::update_auto_login_key* via, void* ud, size_t* rsp_len);

// 数据中心日志
void on_pb_center_log(struct center_service_t* w);
void* on_pb_center_t_upgradelog(mysql::t_upgradelog* req, void* ud, size_t* rsp_len);
void* on_pb_center_t_newtask(mysql::t_newtask* req, void* ud, size_t* rsp_len);
void* on_pb_center_t_login(mysql::t_login* req, void* ud, size_t* rsp_len);
void* on_pb_center_t_offline(mysql::t_offline* req, void* ud, size_t* rsp_len);
void* on_pb_center_t_online_data(mysql::t_online* req, void* ud, size_t* rsp_len);
void* on_pb_center_t_recharge(mysql::t_recharge* req, void* ud, size_t* rsp_len);
void* on_pb_center_t_moneylog(mysql::t_moneylog* req, void* ud, size_t* rsp_len);

// 沙场百战
void *on_pb_center_cycle_challenge_req(client::cycle_challenge_req *req, void *ud, size_t *rsp_len);
void *on_pb_center_cycle_fetch_helpers(client::cycle_fetch_helpers *req, void *ud, size_t *rsp_len);
void *on_pb_center_cycle_add_helped_count(client::cycle_add_helped_count *req, void *ud, size_t *rsp);
void *on_pb_center_cycle_info_rsp(client::cycle_info_rsp *rsp, void *ud, size_t *rsp_len);
void *on_pb_center_cycle_update_record_req(client::cycle_update_record_req *req, void *ud, size_t *rsp_len);

// 魔化神将
void *on_pb_center_devil_fight_over(client::devil_fight_over *req, void *ud, size_t *rsp_len);
void *on_pb_center_devil_info_req(client::devil_info_req *req, void *ud, size_t *rsp_len);

// 跨服相关
void on_pb_center_node(struct center_service_t* w);
void* on_center_pb_user_data(client::user_data* req, void* ud, size_t* rsp_len);

// 后台接口相关
void on_pb_center_backdoor(struct center_service_t* w);
void* on_center_pb_addNotice_req(inner::addNotice_req* req, void* ud, size_t* rsp_len);
void* on_center_pb_close_server(inner::close_server* req, void* ud, size_t* rsp_len);
void* on_center_pb_getNoticeList_req(inner::getNoticeList_req* req, void* ud, size_t* rsp_len);
void* on_center_pb_updateNotice_req(inner::updateNotice_req* req, void* ud, size_t* rsp_len);
void* on_center_pb_delNotice_req(inner::delNotice_req* req, void* ud, size_t* rsp_len);
void* on_center_pb_getPlayerList_req(inner::getPlayerList_req* req, void* ud, size_t* rsp_len);
void* on_center_pb_t_online_req(inner::t_online_req* req, void* ud, size_t* rsp_len);
void* on_center_pb_getBackpackGoodsList_req(inner::getBackpackGoodsList_req* req, void* ud, size_t* rsp_len);
void* on_center_pb_updateBackpackGoods_req(inner::updateBackpackGoods_req* req, void* ud, size_t* rsp_len);
void* on_center_pb_sendGoods_req(inner::sendGoods_req* req, void* ud, size_t* rsp_len);
void* on_center_pb_sendEmail_req(inner::sendEmail_req* req, void* ud, size_t* rsp_len);
void* on_center_pb_getPlayerRestictList_req(inner::getPlayerRestictList_req* req, void* ud, size_t* rsp_len);
void* on_center_pb_addPlayerRestict_req(inner::addPlayerRestict_req* req, void* ud, size_t* rsp_len);
void* on_center_pb_delPlayerRestict_req(inner::delPlayerRestict_req* req, void* ud, size_t* rsp_len);
void* on_center_pb_delivery_ingot_req(inner::delivery_ingot_req* req, void* ud, size_t* rsp_len);
void* on_center_pb_pay_req(inner::pay_req* req, void* ud, size_t* rsp_len);
void* on_pb_center_online_player_req(inner::online_player_req* req, void* ud, size_t* rsp_len);
void* on_pb_center_set_attr_req(inner::set_attr_req* req, void* ud, size_t* rsp_len);
void* on_pb_center_set_vip_req(inner::set_vip_req* req, void* ud, size_t* rsp_len);
void* on_pb_center_bulletin_req(inner::bulletin_req* req, void* ud, size_t* rsp_len);
void* on_pb_center_revolving_req(inner::revolving_req* req, void* ud, size_t* rsp_len);
void* on_pb_center_get_revolving_req(inner::get_revolving_req* req, void* ud, size_t* rsp_len);
void* on_pb_center_del_revolving_req(inner::del_revolving_req* req, void* ud, size_t* rsp_len);
void* on_pb_center_stop_revolving_req(inner::stop_revolving_req* req, void* ud, size_t* rsp_len);
void* on_pb_center_start_time_req(inner::start_time_req* req, void* ud, size_t* rsp_len);
void* on_pb_center_open_gm_req(inner::open_gm_req* req, void* ud, size_t* rsp_len);
void* on_pb_center_get_promotions_req(inner::get_promotions_req* req, void* ud, size_t* rsp_len);
void* on_pb_center_set_promotions_req(inner::set_promotions_req* req, void* ud, size_t* rsp_len);
void* on_pb_center_del_promotions_req(inner::del_promotions_req* req, void* ud, size_t* rsp_len);

int center_dispatcher_init(struct center_service_t* w)
{
	w->pd.reg<server::init_data_req>("server.init_data_req", on_pb_init_data_req);

	on_center_pb_reg_login(w);
    on_pb_center_log(w);

	w->pd.reg<client::trans_user>("client.trans_user", on_pb_center_player_info);
	w->pd.reg<client::jump_user>("client.jump_user", on_pb_center_jump_scene);
	w->pd.reg<client::delivery_mail_info>("client.delivery_mail_info", on_pb_delivery_mail);
	w->pd.reg<client::broadcast_msg_req>("client.broadcast_msg_req", on_pb_broadcast_msg_req);
	w->pd.reg<client::send_mail_check_role>("client.send_mail_check_role", on_center_pb_send_mail_check_role);

	w->pd.reg<client::offline>("client.offline", on_pb_center_offline);
	w->pd.reg<client::user_lose>("client.user_lose", on_pb_center_user_lose);
	w->pd.reg<client::offline_load_image>("client.offline_load_image", on_pb_center_offline_load_image);
	w->pd.reg<server::record_offline_user>("server.record_offline_user", on_pb_record_offline);

	w->pd.reg<client::client_chat_msg>("client.client_chat_msg", on_pb_center_chat_msg);

	w->pd.reg<client::update_title_info_req>("client.update_title_info_req", on_pb_center_update_title_info_req);

	w->pd.reg<client::broadcast_area_to_center>("client.broadcast_area_to_center", on_pb_center_broadcast_area_to_center);

	w->pd.reg<client::boss_info_req>("client.boss_info_req", on_pb_center_boss_info);

	w->pd.reg<client::boss_refresh_msg_rsp>("client.boss_refresh_msg_rsp", on_pb_center_refresh_msg);
	w->pd.reg<client::boss_status>("client.boss_status", on_pb_center_boss_status);

	w->pd.reg<client::query_req>("client.query_req", on_pb_center_query_req);
	w->pd.reg<client::update_smy_info>("client.update_smy_info", on_center_pb_update_smy_info);
	w->pd.reg<client::game_to_center_query>("client.game_to_center_query", on_pb_gamet_to_center_query_req);
	w->pd.reg<client::friend_list_req>("client.friend_list_req", on_pb_friend_list_req);
	w->pd.reg<client::temporary_friend_list_req>("client.temporary_friend_list_req", on_pb_temporary_friend_list_req);
	w->pd.reg<client::friend_add_req>("client.friend_add_req", on_pb_friend_add_req);
	w->pd.reg<client::friend_del_req>("client.friend_del_req", on_pb_friend_del_req);
	w->pd.reg<client::friend_blk_req>("client.friend_blk_req", on_pb_friend_blk_req);
	w->pd.reg<client::friend_status_req>("client.friend_status_req", on_pb_friend_status_req);
	w->pd.reg<client::friend_degree_add>("client.friend_degree_add", on_pb_friend_degree_add);
	w->pd.reg<client::rank_list_req>("client.rank_list_req", on_pb_rank_list_req);
	w->pd.reg<client::rank_add_data>("client.rank_add_data", on_pb_rank_add_data);
	w->pd.reg<client::act_rank_add_data>("client.act_rank_add_data", on_pb_act_rank_add_data);
	w->pd.reg<server::load_role_ret>("server.load_role_ret", center_load_role_ret);
	w->pd.reg<mysql::sp_login_auth>("mysql.sp_login_auth", on_after_role_login);
	w->pd.reg<client::honor_label_list_req>("client.honor_label_list_req", on_pb_honor_label_list_req);
	w->pd.reg<client::get_role_label_info_req>("client.get_role_label_info_req", on_center_pb_get_role_label_info_req);

	// 客户端查询服务器开启时间
	w->pd.reg<client::open_server_time_req>("client.open_server_time_req", on_pb_center_open_server_time_req);
	// 军团相关
	on_center_pb_reg_family(w);
	on_center_pb_reg_city(w);

	on_center_pb_reg_activity(w);

	w->pd.reg<client::open_func_req>("client.open_func_req", on_pb_center_open_func_req);

	w->pd.reg<client::broadcast_explore_list_req>("client.broadcast_explore_list_req", on_pb_broadcast_explore_list_req);
	w->pd.reg<client::explore_update_list>("client.explore_update_list", on_pb_explore_update_list);

	w->pd.reg<client::battle_info_req>("client.battle_info_req", on_center_pb_battle_info_req);

	w->pd.reg<client::yellow_vip_info_req>("client.yellow_vip_info_req", on_center_pb_yellow_vip_info_req);

	w->pd.reg<client::create_role_check_msg>("client.create_role_check_msg", on_pb_center_create_role_check_msg);

	w->pd.reg<client::send_flower>("client.send_flower", on_center_pb_send_flower);
	w->pd.reg<client::recive_flower>("client.recive_flower", on_center_pb_recive_flower);

	w->pd.reg<client::arena_rank_list_req>("client.arena_rank_list_req", on_pb_center_arena_rank_list_req);
	w->pd.reg<client::arena_reward_req>("client.arena_reward_req", on_pb_center_arena_reward_req);
	w->pd.reg<client::arena_info_req>("client.arena_info_req", on_pb_arena_info_req);
	w->pd.reg<client::load_target_data_req>("client.load_target_data_req", on_pb_load_target_data_req);
	w->pd.reg<client::arena_fight_ret>("client.arena_fight_ret", on_pb_arena_fight_ret);
	w->pd.reg<client::arena_record_req>("client.arena_record_req", on_pb_arena_record_req);

	w->pd.reg<client::mate_info_req>("client.mate_info_req", on_center_pb_mate_info_req);
	w->pd.reg<client::mate_invite_req>("client.mate_invite_req", on_center_pb_mate_invite_req);
	w->pd.reg<client::mate_invite_rsp>("client.mate_invite_rsp", on_center_pb_mate_invite_rsp);
	w->pd.reg<client::mate_create_msg>("client.mate_create_msg", on_center_pb_mate_create_msg);
	w->pd.reg<client::depth_mate_req>("client.depth_mate_req", on_center_pb_depth_mate_req);
	w->pd.reg<client::depth_mate_rsp>("client.depth_mate_rsp", on_center_pb_depth_mate_rsp);
	w->pd.reg<client::depth_mate_msg>("client.depth_mate_msg", on_center_pb_depth_mate_msg);
	w->pd.reg<client::depth_mate_end_req>("client.depth_mate_end_req", on_center_pb_depth_mate_end_req);
	w->pd.reg<client::mate_remove_msg>("client.mate_remove_msg", on_center_pb_mate_remove_msg);
	w->pd.reg<client::mate_remove_req>("client.mate_remove_req", on_center_pb_mate_remove_req);
	w->pd.reg<client::mate_award_req>("client.mate_award_req", on_center_pb_mate_award_req);

	w->pd.reg<client::farm_entered_msg>("client.farm_entered_msg", on_center_pb_farm_entered_msg);
	w->pd.reg<client::farm_stolen_msg>("client.farm_stolen_msg", on_center_pb_farm_stolen_msg);

    on_pb_center_backdoor(w);

	w->pd.reg<server::trans_back_user>("server.trans_back_user", on_pb_center_trans_back_user);

	// 组队副本
	w->pd.reg<client::create_team_req>("client.create_team_req", on_pb_center_create_team_req);
	w->pd.reg<client::fire_member_req>("client.fire_member_req", on_pb_center_fire_member_req);
	w->pd.reg<client::join_team_req>("client.join_team_req", on_pb_center_join_team_req);
	w->pd.reg<client::team_ready_req>("client.team_ready_req", on_pb_center_team_ready_req);
	w->pd.reg<client::team_notify>("client.team_notify", on_pb_center_team_notify);
	w->pd.reg<client::team_enter_raid_req>("client.team_enter_raid_req", on_pb_center_team_enter_raid_req);
	w->pd.reg<client::team_enter_prepare_req>("client.team_enter_prepare_req", on_pb_center_team_enter_prepare_req);
	w->pd.reg<client::team_join_check>("client.team_join_check", on_pb_center_team_join_check);

	w->pd.reg<client::cycle_challenge_req>("client.cycle_challenge_req", on_pb_center_cycle_challenge_req);
	w->pd.reg<client::cycle_fetch_helpers>("client.cycle_fetch_helpers", on_pb_center_cycle_fetch_helpers);
	w->pd.reg<client::cycle_add_helped_count>("client.cycle_add_helped_count", on_pb_center_cycle_add_helped_count);
	w->pd.reg<client::cycle_info_rsp>("client.cycle_info_rsp", on_pb_center_cycle_info_rsp);
	w->pd.reg<client::cycle_update_record_req>("client.cycle_update_record_req", on_pb_center_cycle_update_record_req);

	w->pd.reg<client::devil_fight_over>("client.devil_fight_over", on_pb_center_devil_fight_over);
	w->pd.reg<client::devil_info_req>("client.devil_info_req", on_pb_center_devil_info_req);
	return 0;
}

void on_pb_center_backdoor(struct center_service_t* w)
{
    w->pd.reg<inner::addNotice_req>("inner.addNotice_req", on_center_pb_addNotice_req);
    w->pd.reg<inner::close_server>("inner.close_server", on_center_pb_close_server);
    w->pd.reg<inner::getNoticeList_req>("inner.getNoticeList_req", on_center_pb_getNoticeList_req);
    w->pd.reg<inner::updateNotice_req>("inner.updateNotice_req", on_center_pb_updateNotice_req);
    w->pd.reg<inner::delNotice_req>("inner.delNotice_req", on_center_pb_delNotice_req);
    w->pd.reg<inner::getPlayerList_req>("inner.getPlayerList_req", on_center_pb_getPlayerList_req);
    w->pd.reg<inner::t_online_req>("inner.t_online_req", on_center_pb_t_online_req);
    w->pd.reg<inner::getBackpackGoodsList_req>("inner.getBackpackGoodsList_req", on_center_pb_getBackpackGoodsList_req);
    w->pd.reg<inner::updateBackpackGoods_req>("inner.updateBackpackGoods_req", on_center_pb_updateBackpackGoods_req);
    w->pd.reg<inner::sendGoods_req>("inner.sendGoods_req", on_center_pb_sendGoods_req);
    w->pd.reg<inner::sendEmail_req>("inner.sendEmail_req", on_center_pb_sendEmail_req);
    w->pd.reg<inner::getPlayerRestictList_req>("inner.getPlayerRestictList_req", on_center_pb_getPlayerRestictList_req);
    w->pd.reg<inner::addPlayerRestict_req>("inner.addPlayerRestict_req", on_center_pb_addPlayerRestict_req);
    w->pd.reg<inner::delPlayerRestict_req>("inner.delPlayerRestict_req", on_center_pb_delPlayerRestict_req);
    w->pd.reg<inner::delivery_ingot_req>("inner.delivery_ingot_req", on_center_pb_delivery_ingot_req);
    w->pd.reg<inner::pay_req>("inner.pay_req", on_center_pb_pay_req);
    w->pd.reg<inner::set_attr_req>("inner.set_attr_req", on_pb_center_set_attr_req);
	w->pd.reg<inner::set_vip_req>("inner.set_vip_req", on_pb_center_set_vip_req);

    w->pd.reg<inner::online_player_req>("inner.online_player_req", on_pb_center_online_player_req);
    w->pd.reg<inner::bulletin_req>("inner.bulletin_req", on_pb_center_bulletin_req);
    w->pd.reg<inner::revolving_req>("inner.revolving_req", on_pb_center_revolving_req);
	
	w->pd.reg<inner::get_revolving_req>("inner.get_revolving_req", on_pb_center_get_revolving_req);
	w->pd.reg<inner::del_revolving_req>("inner.del_revolving_req", on_pb_center_del_revolving_req);
	w->pd.reg<inner::stop_revolving_req>("inner.stop_revolving_req", on_pb_center_stop_revolving_req);

    w->pd.reg<inner::start_time_req>("inner.start_time_req", on_pb_center_start_time_req);
    w->pd.reg<inner::open_gm_req>("inner.open_gm_req", on_pb_center_open_gm_req);
	w->pd.reg<inner::get_promotions_req>("inner.get_promotions_req", on_pb_center_get_promotions_req);
	w->pd.reg<inner::set_promotions_req>("inner.set_promotions_req", on_pb_center_set_promotions_req);
	w->pd.reg<inner::del_promotions_req>("inner.del_promotions_req", on_pb_center_del_promotions_req);
}

void on_center_pb_reg_login(struct center_service_t* w)
{
	w->pd.reg<client::auto_login_req>("client.auto_login_req", on_center_pb_auto_login_req);
	w->pd.reg<client::update_auto_login_key>("client.update_auto_login_key", on_center_pb_update_auto_login_key);
}

void on_pb_center_log(struct center_service_t* w)
{
    w->pd.reg<mysql::t_upgradelog>("mysql.t_upgradelog", on_pb_center_t_upgradelog);
    w->pd.reg<mysql::t_newtask>("mysql.t_newtask", on_pb_center_t_newtask);
    w->pd.reg<mysql::t_login>("mysql.t_login", on_pb_center_t_login);
    w->pd.reg<mysql::t_offline>("mysql.t_offline", on_pb_center_t_offline);
    w->pd.reg<mysql::t_online>("mysql.t_online", on_pb_center_t_online_data);
    w->pd.reg<mysql::t_recharge>("mysql.t_recharge", on_pb_center_t_recharge);
    w->pd.reg<mysql::t_moneylog>("mysql.t_moneylog", on_pb_center_t_moneylog);
}

void on_pb_center_node(struct center_service_t* w)
{
    w->pd.reg<client::user_data>("client.user_data", on_center_pb_user_data);
}

void on_center_pb_reg_city(struct center_service_t* w)
{
	w->pd.reg<client::city_info_req>("client.city_info_req", on_pb_center_city_info_req);
	w->pd.reg<client::city_mod_title_req>("client.city_mod_title_req", on_pb_center_city_mod_title_req);
	w->pd.reg<client::city_welfare_info_req>("client.city_welfare_info_req", on_pb_center_city_welfare_info_req);
	w->pd.reg<client::city_welfare_get_req>("client.city_welfare_get_req", on_pb_center_city_welfare_get_req);
	w->pd.reg<client::siege_over>("client.siege_over", on_pb_center_siege_over);
	w->pd.reg<client::siege_status>("client.siege_status", on_pb_center_siege_status);
	w->pd.reg<client::siege_flag_occupy>("client.siege_flag_occupy", on_pb_center_siege_flag_occupy);
	w->pd.reg<client::city_welfare_get_confirm>("client.city_welfare_get_confirm", on_pb_center_city_welfare_get_confirm);
	w->pd.reg<client::siege_call_accept_req>("client.siege_call_accept_req", on_pb_siege_call_accept_req);
	w->pd.reg<client::siege_status_rsp>("client.siege_status_rsp", on_pb_center_broadcast_siege_status);
	w->pd.reg<client::king_skill_add>("client.king_skill_add", on_pb_center_king_skill_add);
	w->pd.reg<client::siege_start>("client.siege_start", on_pb_center_siege_start);
}

void on_center_pb_reg_family(struct center_service_t* w)
{
	w->pd.reg<client::family_list_req                  >("client.family_list_req"                  , on_center_pb_family_list_req                    );
	w->pd.reg<client::family_req                       >("client.family_req"                       , on_center_pb_family_req                         );
	w->pd.reg<client::create_family_req                >("client.create_family_req"                , on_center_pb_create_family_req                  );
	w->pd.reg<client::secede_family_to_center          >("client.secede_family_to_center"          , on_center_pb_secede_family_to_center            );
	w->pd.reg<client::apply_family_req                 >("client.apply_family_req"                 , on_center_pb_apply_family_req                   );
	w->pd.reg<client::apply_family_list_req            >("client.apply_family_list_req"            , on_center_pb_apply_family_list_req              );
	w->pd.reg<client::apply_family_list_self_req       >("client.apply_family_list_self_req"       , on_center_pb_apply_family_list_self_req         );
	w->pd.reg<client::approval_family_req              >("client.approval_family_req"              , on_center_pb_approval_family_req                );
	w->pd.reg<client::invite_family_req                >("client.invite_family_req"                , on_center_pb_invite_family_req                  );
	w->pd.reg<client::invite_family_list_req           >("client.invite_family_list_req"           , on_center_pb_invite_family_list_req             );
	w->pd.reg<client::agree_family_req                 >("client.agree_family_req"                 , on_center_pb_agree_family_req                   );
	w->pd.reg<client::modify_family_post_req           >("client.modify_family_post_req"           , on_center_pb_modify_family_post_req             );
	w->pd.reg<client::family_member_list_req           >("client.family_member_list_req"           , on_center_pb_family_member_list_req             );
	w->pd.reg<client::modify_family_member_req         >("client.modify_family_member_req"         , on_center_pb_modify_family_member_req           );
	w->pd.reg<client::expel_member_req				   >("client.expel_member_req"				   , on_center_pb_expel_member_req					 );
	w->pd.reg<client::update_family_data_from_center   >("client.update_family_data_from_center"   , on_center_pb_update_family_data_from_center	 );
	w->pd.reg<client::disband_family_req               >("client.disband_family_req"               , on_center_pb_disband_family_req                 );
	w->pd.reg<client::set_family_money                 >("client.set_family_money"                 , on_center_pb_donate_family_req                  );
	w->pd.reg<client::upgrade_family_req               >("client.upgrade_family_req"               , on_center_pb_upgrade_family_req                 );
	w->pd.reg<client::family_self_req                  >("client.family_self_req"                  , on_center_pb_family_self_req                    );
	w->pd.reg<client::family_log_req                   >("client.family_log_req"                   , on_center_pb_family_log_req                     );
	w->pd.reg<client::family_record_req                >("client.family_record_req"                , on_center_pb_family_record_req                  );
	w->pd.reg<client::modify_family_power              >("client.modify_family_power"              , on_center_pb_modify_family_power                );
	w->pd.reg<client::family_record_update             >("client.family_record_update"             , on_center_pb_family_record_update               );
	w->pd.reg<client::family_log_update                >("client.family_log_update"                , on_center_pb_family_log_update                  );
	w->pd.reg<client::family_set_req			       >("client.family_set_req"				   , on_center_pb_family_set_req					 );
	w->pd.reg<client::family_money_query_req>("client.family_money_query_req", on_center_pb_query_family_money_req);
	w->pd.reg<client::family_money_mod_req>("client.family_money_mod_req", on_center_pb_family_money_mod_req);
	w->pd.reg<client::change_family_flag_id_req>("client.change_family_flag_id_req", on_center_pb_change_family_flag_id_req);
	w->pd.reg<client::change_family_flag_name_req>("client.change_family_flag_name_req", on_center_pb_change_family_flag_name_req);
	w->pd.reg<client::impeach_chief_req>("client.impeach_chief_req", on_center_pb_impeach_chief_req);
	w->pd.reg<client::donate_list_req>("client.donate_list_req", on_center_pb_donate_list_req);
	w->pd.reg<client::ally_family_req>("client.ally_family_req", on_center_pb_ally_family_req);
	w->pd.reg<client::remove_foreign_req>("client.remove_foreign_req", on_center_pb_remove_foreign_req);
	w->pd.reg<client::ally_family_list_req>("client.ally_family_list_req", on_center_pb_ally_family_list_req);
	w->pd.reg<client::notive_center_donate_sucess>("client.notive_center_donate_sucess", on_center_pb_notive_center_donate_sucess);
	w->pd.reg<client::load_friend_family_req>("client.load_friend_family_req", on_center_pb_load_friend_family_req);
}

void   on_center_pb_reg_activity(struct center_service_t *w)
{
	w->pd.reg<client::activity_info_req>("client.activity_info_req", on_center_pb_activity_info_req);
	w->pd.reg<client::activity_shop_req>("client.activity_shop_req", on_center_pb_activity_shop_req);
	w->pd.reg<client::activity_reward_req>("client.activity_reward_req", on_center_pb_activity_reward_req);
	w->pd.reg<client::activity_shop_g2c>("client.activity_shop_g2c", on_center_pb_activity_shop_g2c);
	w->pd.reg<client::activity_reward_g2c>("client.activity_reward_g2c", on_center_pb_activity_reward_g2c);
	w->pd.reg<client::activity_his_record_g2c>("client.activity_his_record_g2c", on_center_pb_activity_his_record_g2c);
	w->pd.reg<client::update_activity_record_g2c>("client.update_activity_record_g2c", on_center_pb_update_activity_record_g2c);
	w->pd.reg<client::load_activity_list_center>("client.load_activity_list_center", on_center_pb_load_activity_list_center);
}

//static void send_rank_reward(center_service_t *w)
void send_rank_reward(center_service_t *w)
{
	const vector<uint32> &v = w->ar_mgr.get_arena_list();
	time_t now = time(NULL);
	struct tm tm;
	localtime_r(&now, &tm);
	for(size_t i = 0; i < v.size(); ++i)
	{
		if(i >= 30000)		// 只发前3W名
		{
			return;
		}
		uint32 role_id = v[i];
		if(w->ar_mgr.get_role_type(role_id) == ARENA_ROBOT)
		{
			continue;
		}
		const config_edreward *reward = CONFMGR->get_edreward(i + 1);
		if(!reward)
		{
			continue;
		}
		client::mail_info mi;
		client::mail_head *mh = mi.mutable_mh();
		client::mail_body *mb = mi.mutable_mb();
		mh->set_mail_id(0);
		mh->set_mail_type(MAIL_TYPE_SYSTEM);
		mh->set_recv_role_id(role_id);
		mh->set_send_role_id(0);
		mh->set_send_role_name("system");
		mh->set_title("battle_title");
		mb->set_content("battle_content");
		mh->set_send_time((uint32)now);
		mh->set_past_time(get_mail_past_time(MAIL_TYPE_SYSTEM,true));
		mh->set_opened(false);
		mh->set_is_take(false);
		mh->set_attachment_num(reward->_reward.size());
		string str;
		for(size_t j = 0; j < reward->_reward.size(); ++j)
		{
			char tmp[256] = {0};
			client::mail_attr *attr = mi.add_attr();
			attr->set_attr_type((reward->_reward[j]).first);
			attr->set_attr_value((reward->_reward[j]).second);
			const char *type = "";
			switch(reward->_reward[j].first)
			{
				case INGOT:
					type = "元宝";
					break;
				case TAEL:
					type = "铜钱";
					break;
				case MILITARY_STRATEGY:
					type = "韬略";
					break;
				case PHYSICAL_POWER:
					type = "体力";
					break;
				case EXPERIENCE:
					type = "经验";
					break;
				case POPULARITY:
					type = "威望";
					break;
				case SIEGE_REPUTATION:
					type = "军功";
					break;
				default:
					break;
			}
			snprintf(tmp, 256, " %s:%u", type, (reward->_reward[j]).second);
			str += tmp;
		}
		client::content_data* rank_info = mb->add_data();
		rank_info->set_n(i + 1);
		client::content_data* mon_info = mb->add_data();
		mon_info->set_n(tm.tm_mon + 1);
		client::content_data* day_info = mb->add_data();
		day_info->set_n(tm.tm_mday);
		send_new_mail(w, role_id, mi);
		amtsvr_billlog(w->ctx, "%u|arena_daily_reward|%u|%s", role_id, i + 1, str.c_str());
	}
}

int center_save_data(struct center_service_t* w)
{
	w->wg_mcm.save_mail(w->ctx);
	w->fm_mgr.save(w->ctx);
	w->cf_mgr->save(w->ctx);
	w->rcm_mgr->save(w->ctx);
	w->ar_mgr.save(w->ctx);
	w->poa_mgr->save(w->ctx);
	w->cc_mgr.save(w->ctx);
	w->rm_mgr.save(w->ctx);
	w->dm_mgr.save(w->ctx);
	w->notic_mgr.save(w->ctx);

	return 0;
}

int center_save_all_data(struct center_service_t* w)
{
	center_save_data(w);
	w->rm_mgr.save(w->ctx);
	w->cwm_mgr->save(w->ctx);

	return 0;
}

int center_one_second_timer(struct center_service_t* w)
{
	w->ct_mgr->update(w->ctx);
	w->bm_mgr.update(w->ctx);
	w->rm_mgr.update(w->ctx);
	poll_activity_on_process(w);
	//w->poa_mgr->on_process(w);
	// 发送竞技场奖励
	if((uint32)time(NULL) >= w->ar_mgr.get_reward_time())
	{
		send_rank_reward(w);
		w->ar_mgr.reset_reward_time();
	}

	on_area_brocast(w);

	return 0;
}

int center_ten_second_timer(struct center_service_t* w)
{
	//w->cf_mgr->clear_famliy_apply_relation();
    //GAME_EXIT = true;

	return 0;
}

bool hour_and_min(struct tm& mt, int h, int m)
{
	return mt.tm_hour == h && mt.tm_min == m;
}

int center_half_hour_timer(struct center_service_t* w, struct tm& mt)
{
	return 0;
}

int center_one_minute_timer(struct center_service_t* w)
{
    if (w == NULL) {
        return -1;
    }

    ++(w->five_minute_timer);

    if (w->five_minute_timer > 4) {
        w->five_minute_timer = 0;

        const std::map<uint32, online_role>& ors = w->oi_mgr.get_all_online_info();
        mysql::t_online req;
        req.set_id(0);
        req.set_sid("");
        req.set_allcount(ors.size());
        std::map<uint32, online_role>::const_iterator i = ors.begin();
        int count = 0;
        while (i != ors.end()) {
            if (i->second.vip_exp() > 0) {
                ++count;
            }

            ++i;
        }

        req.set_cashcount(count);
        req.set_createdate((uint32)time(NULL));
        on_pb_center_t_online_data(&req, w, NULL);
    }

	return 0;
}

int center_one_hour_timer(struct center_service_t* w)
{
	// 更新军团外交关系：只处理申请关系
	w->cf_mgr->clear_famliy_apply_relation();
    w->_role_key->update();
    w->_log->write();
	// 一个小时处理一遍邮件中心过期的邮件

	return 0;
}

void* on_pb_init_data_req(server::init_data_req* idr, void* ud, size_t* rsp_len)
{
	center_service_t* w = (center_service_t*)ud;
	w->wg_mcm.init(NULL);
	w->fm_mgr.init();
	w->rm_mgr.init();
	w->cf_mgr->init(NULL);
	w->ar_mgr.init();
	w->rbt_mgr.init();
	w->poa_mgr->init(NULL);
	w->notic_mgr.init(NULL);
	if(w->rbt_mgr.need_grenerate_robot())
	{
		generate_arena_robot(w);
	}
	w->cs_mgr.init();
	w->rcm_mgr->init(NULL);
	w->mm_mgr.init();
	w->bm_mgr.init();

	w->tm_mgr.attach_ctx(w->ctx);
	//通过地图ID获取scene_service_id
	
	w->ct_mgr->init(w->ctx);
	w->ct_mgr->set_scene_svr_id(w->wm_mgr.get_scene_service_id(101));

	w->wm_mgr.get_boss_scene_service_id(w->bm_mgr.m_boss_scene);

	w->cwm_mgr->init(w->ctx);

	w->cc_mgr.init(w->ctx);

	w->bm_mgr.refresh_world_boss(w->ctx);

	w->dm_mgr.init();

	init_scene_sculpture(w);

	return NULL;
}

int center_load_role_req(struct center_service_t* w, uint32 role_id, google::protobuf::Message& msg)
{
	msg_handler* mh = MSG_HANDLER_POOL->pop();
	if (mh == NULL) {
		return -1;
	}

	google::protobuf::Message* new_msg = create_msg(msg.GetTypeName());
	if (new_msg == NULL) {
		MSG_HANDLER_POOL->push(mh);
		return -2;
	}

	new_msg->CopyFrom(msg);
	mh->ext1 = role_id;
	mh->msg = new_msg;

	amtsvr_sendname(w->ctx, LOADER_NAME, PTYPE_CLIENT|PTYPE_TAG_DONTCOPY, 0, mh, sizeof(msg_handler));

	return 0;
}

void* center_load_role_ret(server::load_role_ret* ret, void* ud, size_t* rsp_len)
{
	center_service_t* w   = (center_service_t*)ud;
	uintptr_t         ptr = (uintptr_t)ret->ptr();
	player_role*      pr  = (player_role*)(ptr);
	if (pr == NULL) {
		return NULL;
	}

	uintptr_t    cptr = (uintptr_t)ret->cptr();
	center_role* cr   = (center_role*)(cptr);
	if (cr == NULL) {
		delete pr;
		return NULL;
	}

#ifdef TEST_MEMCHECK
	test_memcheck_load_role_ret(w, pr, cr);
	return NULL;
#endif

	w->ou_mgr->push(pr->get_role_id(), pr, cr);

	return NULL;
}

int center_send_inner(struct center_service_t* w, const inner::inner_head& head, google::protobuf::Message& msg)
{
	size_t msg_len = 0;
	void* buf = encode(msg, msg_len);
	if (buf == NULL) {
		return -1;
	}

	return amtsvr_send(w->ctx, 0, head.client_id(), PTYPE_CLIENT|PTYPE_TAG_DONTCOPY, 0, buf, msg_len);
}

int center_send_scene(struct center_service_t* w, uint32 role_id, google::protobuf::Message& msg)
{
	const online_role* oi = w->oi_mgr.get_online_info(role_id);
	if (oi == NULL) {
		return -1;
	}

	return center_send_scene(w, *oi, msg);
}

int center_send_scene(struct center_service_t* w, const online_role& oi, google::protobuf::Message& msg)
{
	return center_send_scene_ex(w, oi.scene_svr_id, msg);
}

int center_send_scene_ex(struct center_service_t* w,  uint32 scene_svr_id, google::protobuf::Message& msg)
{
	if (scene_svr_id == 0) {
		return -2;
	}

	google::protobuf::Message* new_msg = create_msg(msg.GetTypeName());
	if (new_msg == NULL) {
		return -3;
	}

	new_msg->CopyFrom(msg);

	msg_handler* mh = MSG_HANDLER_POOL->pop();
	if (mh == NULL) {
		delete new_msg;
		return -4;
	}

	mh->msg = new_msg;

	return amtsvr_send(w->ctx, 0, scene_svr_id, PTYPE_CLIENT|PTYPE_TAG_DONTCOPY, 0, mh, sizeof(msg_handler));
}

int center_send_scene(struct center_service_t* w, const client::server_head& head, google::protobuf::Message& msg)
{
	return center_send_scene(w, head.role_id(), msg);
}

int center_send_scene_by_scene_id(struct center_service_t* w, uint32 scene_id, google::protobuf::Message& msg)
{
	uint32 scene_svr_id =  w->wm_mgr.get_scene_service_id(scene_id);
	if (scene_svr_id == 0) {
		// 启动场景服务
		return -2;
	}

	google::protobuf::Message* new_msg = create_msg(msg.GetTypeName());
	if (new_msg == NULL) {
		return -3;
	}

	new_msg->CopyFrom(msg);

	msg_handler* mh = MSG_HANDLER_POOL->pop();
	if (mh == NULL) {
		delete new_msg;
		return -4;
	}

	mh->msg = new_msg;

	return amtsvr_send(w->ctx, 0, scene_svr_id, PTYPE_CLIENT|PTYPE_TAG_DONTCOPY, 0, mh, sizeof(msg_handler));
}

int center_send_scene_by_service_id(struct center_service_t* w, uint32 service_id, google::protobuf::Message& msg)
{
	if (service_id == 0) {
		return -1;
	}

	google::protobuf::Message* new_msg = create_msg(msg.GetTypeName());
	if (new_msg == NULL) {
		return -3;
	}

	new_msg->CopyFrom(msg);

	msg_handler* mh = MSG_HANDLER_POOL->pop();
	if (mh == NULL) {
		delete new_msg;
		return -4;
	}

	mh->msg = new_msg;

	return amtsvr_send(w->ctx, 0, service_id, PTYPE_CLIENT|PTYPE_TAG_DONTCOPY, 0, mh, sizeof(msg_handler));
}

int center_send_client_by_role_id(center_service_t* w, uint32 role_id, google::protobuf::Message& msg)
{
	const online_role* oi = w->oi_mgr.get_online_info(role_id);
	if (oi == NULL) {
		return -1;
	}

	return center_send_client(w, *oi, msg);
}

int center_send_client(center_service_t* w, const online_role& oi, google::protobuf::Message& msg)
{
	return center_send_client(w, oi.client_id, msg);
}

int center_send_client(center_service_t* w, const client::server_head& head, google::protobuf::Message& msg)
{
	return center_send_client(w, head.client_id(), msg);
}

int center_send_client(center_service_t* w, uint32 client_id, google::protobuf::Message& msg)
{
	size_t msg_len = 0;
	void* buf = encode(msg, msg_len);
	if (buf == NULL) {
		return -1;
	}

	return amtsvr_send(w->ctx, 0, client_id, PTYPE_CLIENT|PTYPE_TAG_DONTCOPY, 0, buf, msg_len);
}

int center_send_role(center_service_t* w, google::protobuf::Message& msg)
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

	return amtsvr_sendname(w->ctx, ROLE_NAME, PTYPE_CLIENT|PTYPE_TAG_DONTCOPY, 0, mh, sizeof(msg_handler));
}

int ceneter_send_chores(center_service_t* w, const google::protobuf::Message& msg)
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

	return amtsvr_sendname(w->ctx, CHORES_NAME, PTYPE_CLIENT|PTYPE_TAG_DONTCOPY, 0, mh, sizeof(msg_handler));
}

player_role* center_get_player_role(center_service_t* w, google::protobuf::Message& msg, uint32 role_id)
{
	const online_role* oi = w->oi_mgr.get_online_info(role_id);
	if (oi != NULL) {
		center_send_scene(w, *oi, msg);
		return NULL;
	}

	const offline_role* ou = w->ou_mgr->get(role_id);
	if (ou == NULL) {
		center_load_role_req(w, role_id, msg);
		return NULL;
	}

	player_role* pr = ou->get();
	if (pr == NULL) {
		return NULL;
	}

	return pr;
}

int center_send_agent(center_service_t* w, uint32 agent_id, google::protobuf::Message& msg)
{
	//google::protobuf::Message* new_msg = create_msg(msg.GetTypeName());
	//if (new_msg == NULL) {
	//	return -1;
	//}

	//new_msg->CopyFrom(msg);

	//msg_handler* mh = MSG_HANDLER_POOL->pop();
	//if (mh == NULL) {
	//	delete new_msg;
	//	return -2;
	//}

	//mh->msg = new_msg;

	//return amtsvr_send(w->ctx, 0, agent_id, PTYPE_CLIENT|PTYPE_TAG_DONTCOPY, 0, mh, sizeof(msg_handler));

    int   ca_len = 0;
    void* ca_buf = svr_encode(msg, ca_len);
    if (ca_buf == NULL) {
        return -1;
    }

    return amtsvr_send(w->ctx, 0, agent_id, PTYPE_CLIENT|PTYPE_TAG_DONTCOPY, 0, ca_buf, (size_t)ca_len);
}

int center_close_agent(center_service_t* w, const online_role& oi)
{
	server::close_agent ca;
	ca.set_agent_id(oi.agent_id);
	int   ca_len = 0;
	void* ca_buf = svr_encode(ca, ca_len);
	if (ca_buf == NULL) {
		return -1;
	}

	return amtsvr_send(w->ctx, 0, oi.agent_id, PTYPE_CLIENT|PTYPE_TAG_DONTCOPY, 0, ca_buf, (size_t)ca_len);
}

uint32 get_mail_past_time(uint32 mail_type,bool has_adjunct)
{
	int past_time = 0;
	uint32 cur_time = (uint32)time(NULL);
	uint32 adjunct_time = 0;
	uint32 total_time = 0;
	if (has_adjunct){// 有附件所增加的时间
		adjunct_time = CONFMGR->get_basic_value(10107);
	}
	switch(mail_type)
	{
		case MAIL_TYPE_SYSTEM:// 系统
			{
				past_time = CONFMGR->get_basic_value(10102);
			}
			break;
		case MAIL_TYPE_PLAYER: // 玩家邮件
			{
				past_time = CONFMGR->get_basic_value(10103);
			}
			break;
		case MAIL_TYPE_ADMIN: // 管理员邮件
			{
				past_time = CONFMGR->get_basic_value(10104);
			}
			break;
		case MAIL_TYPE_FAMILY: // 同盟邮件邀请
			{
				past_time = CONFMGR->get_basic_value(10105);
			}
			break;
		case MAIL_TYPE_ARMY: // 其余军团邮件（同盟拒绝通知，敌盟通知）
			{
				past_time = CONFMGR->get_basic_value(10106);
			}
			break;
		default:
			{
				past_time = CONFMGR->get_basic_value(10102);
			}
			break;
	}

	total_time = cur_time + (uint32)past_time + adjunct_time;

	return total_time;
}

void send_broadcast_area_msg(center_service_t *w, uint32 msg_id, const vector<client::content_data> &data)
{
	const config_mail *cm = CONFMGR->get_config_mail_mgr().get_config_mail(msg_id);
	if(cm == NULL){
		return ;
	}

	client::broadcast_area_rsp rsp;
	rsp.set_area_id(msg_id);
	client::mail_body *mb = rsp.mutable_area_msg();
	mb->set_content(cm->get_content_value());
	for(size_t i = 0; i < data.size(); ++i){
		client::content_data *cd = mb->add_data();
		cd->CopyFrom(data[i]);
	}

	rsp.set_broadcast_count(cm->get_brod_times());
	send_broadcast_area_msg(w, rsp);
}

void send_broadcast_area_msg(center_service_t *w,client::broadcast_area_rsp& area_msg)
{
	w->oi_mgr.brodcast(w->ctx, area_msg);
}

static uint32 generate_pet_id(uint32 except)
{
	vector<uint32> v;
	for(uint32 i = 1; i < 9; ++i)
	{
		if(i == except)
		{
			continue;
		}
		v.push_back(i);
	}
	return v[mtrandom::rand_mid32(0u, v.size() - 1)];
}

static void generate_pet_attr(uint32 id, uint32 lv, float r, map<int, int> &attr)
{
	const config_pet_skill *cps = CONFMGR->get_config_pet_skill(id, lv);
	if(cps == NULL)
	{
		return ;
	}
	const map<int, int> &p_attr = cps->get_pet_attribute();
	map<int, int>::const_iterator it;
	for(it = p_attr.begin(); it != p_attr.end(); ++it)
	{
		attr.insert(make_pair(it->first, (int)ceil(it->second * r)));
	}
}

static void generate_arena_robot(center_service_t *w)
{
	std::map<uint32, config_robot *> robots = CONFMGR->get_config_robots();
	uint32 index = 1000000;
	std::map<uint32, config_robot *>::const_iterator it;
	const std::vector<string>& first_name = CONFMGR->get_config_first_name();
	const std::vector<string>& f_second_name = CONFMGR->get_config_female_second_name();
	const std::vector<string>& m_second_name = CONFMGR->get_config_male_second_name();
	vector<mysql::tb_arena_robot *> v;
	for(it = robots.begin(); it != robots.end(); ++it)
	{
		if(it->first == 1110 || it->first == 1112)
		{
			continue;
		}
		config_robot *cr = it->second;
		for(int i = 0; i < (int)cr->get_robot_percent(); ++i)
		{
			float r = mtrandom::rand_mid32(cr->get_attr_float_min(), cr->get_attr_float_max()) / 10000.0f;
			mysql::tb_arena_robot *tar = new mysql::tb_arena_robot;
			tar->set_id(index++);
			while(1)
			{
				string name = first_name[mtrandom::rand_mid32(0u, first_name.size() - 1)];
				if(cr->get_robot_job() == 0)
				{
					name += m_second_name[mtrandom::rand_mid32(0u, m_second_name.size() - 1)];
				}
				else
				{
					name += f_second_name[mtrandom::rand_mid32(0u, f_second_name.size() - 1)];
				}
				if(w->aus_mgr.mutable_user_smy(name) == NULL && !w->rbt_mgr.robot_name_exist(name))
				{
					tar->set_name(name);
					break;
				}
			}
			tar->set_config_id(cr->get_robot_config_id());
			tar->set_fighting_power((uint32)ceil((float)cr->_robot_combat * r + cr->_skill_combat));
			map<int, int> robot_attr, g_attr, b_attr;
			map<int, int>::const_iterator it;
			const map<int, int> &r_attr = cr->get_robot_attr();
			for(it = r_attr.begin(); it != r_attr.end(); ++it)
			{
				robot_attr.insert(make_pair(it->first, (int)ceil(it->second * r)));
			}
			uint32 pet_id = cr->get_general_id();
			if(pet_id == 999)
			{
				tar->set_pet_id(0);
			}
			else
			{
				if(pet_id == 0)
				{
					pet_id = generate_pet_id(0);
				}
				tar->set_pet_id(pet_id);
				generate_pet_attr(pet_id, cr->get_general_level(), r, g_attr);
			}
			uint32 standby_id = cr->get_standby_pet_id();
			if(standby_id == 999)
			{
				tar->set_standby(0);
			}
			else
			{
				if(standby_id == 0)
				{
					standby_id = generate_pet_id(pet_id);
				}
				tar->set_standby(standby_id);
				generate_pet_attr(standby_id, cr->get_standby_pet_level(), r, b_attr);
			}
			robot *robot = w->rbt_mgr.add_robot(tar);
			if(NULL == robot)
			{
				delete tar;
				continue;
			}
			robot->set_robot_attr(robot_attr);
			robot->set_pet_attr(g_attr);
			robot->set_standby_attr(b_attr);
			v.push_back(tar);
		}
	}
	w->rbt_mgr.save(w->ctx);
	sort(v.begin(), v.end(), cmp());
	for(size_t i = 0; i < v.size(); ++i)
	{
		w->ar_mgr.add_role(v[i]->id(), ARENA_ROBOT);
	}
}

void send_activity_mail(center_service_t *w,uint32 role_id,uint32 drop_id,uint32 rank_num,uint32 limit_type,uint32 second_value)
{
	const mysql::sp_load_all_user_smy* req_smy = w->aus_mgr.get_user_smy(role_id);
	if (req_smy == NULL){
		return;
	}

	time_t now = time(NULL);
	struct tm tm;
	localtime_r(&now, &tm);

	vector<client::props_info> item_vec;
	item_vec.clear();
	make_props(drop_id,item_vec,req_smy->profession());
	client::mail_info mi;
	client::mail_head *mh = mi.mutable_mh();
	client::mail_body *mb = mi.mutable_mb();
	mh->set_mail_id(0);
	mh->set_mail_type(MAIL_TYPE_SYSTEM);
	mh->set_recv_role_id(role_id);
	mh->set_send_role_id(0);
	mh->set_send_role_name("system");
	mh->set_send_time((uint32)now);
	mh->set_past_time(get_mail_past_time(MAIL_TYPE_SYSTEM,true));
	mh->set_opened(false);
	mh->set_is_take(false);
	mh->set_attachment_num(item_vec.size());
	std::vector<client::content_data> data_vec;
	data_vec.clear();
	if (limit_type == REWARD_TYPE_LEVEL){
		mh->set_title("activity_level1");
		mb->set_content("activity_level2");
		// 填充人物等级
		client::content_data data_1;
		data_1.set_n(req_smy->level());
		data_vec.push_back(data_1);

	}else if (limit_type == REWARD_TYPE_POWER){
		mh->set_title("activity_equip1");
		mb->set_content("activity_equip2");
		// 填充战力值
		client::content_data data_1;
		data_1.set_n(second_value);
		data_vec.push_back(data_1);
	}else if (limit_type == REWARD_TYPE_GEM_LEVEL){
		mh->set_title("activity_gem1");
		mb->set_content("activity_gem2");
		// 填充积分值
		client::content_data data_1;
		data_1.set_n(second_value);
		data_vec.push_back(data_1);
	}
	else{
		return;
	}
	// 排名
	client::content_data data_2;
	data_2.set_n(rank_num);
	data_vec.push_back(data_2);
	// 填充月份
	client::content_data data_3;
	data_3.set_n(tm.tm_mon + 1);
	data_vec.push_back(data_3);
	// 填充号数
	client::content_data data_4;
	data_4.set_n(tm.tm_mday);
	data_vec.push_back(data_4);

	for (uint32 i = 0; i < data_vec.size(); ++i){
		client::content_data* data_info = mb->add_data();
		data_info->set_n(data_vec[i].n());
		data_info->set_s(data_vec[i].s());
		data_info->set_f(data_vec[i].f());
	}

	for (uint32 j = 0; j < item_vec.size(); ++j){
		client::props_info* item_info = mi.add_ma();
		item_info->CopyFrom(item_vec[j]);
	}

	send_new_mail(w,role_id,mi);
}

void record_online_activity(center_service_t *w,uint32 activity_limit_type,uint32 activity_id)
{
	if (activity_limit_type == REWARD_TYPE_POWER || activity_limit_type == REWARD_TYPE_GEM_LEVEL){
		const map<uint32, online_role>& ol_map_ = w->oi_mgr.get_all_online_info();
		map<uint32, online_role>::const_iterator it = ol_map_.begin();
		for (; it != ol_map_.end(); ++it){
			if (activity_limit_type == REWARD_TYPE_POWER){
				// 记录进去:向game_server 请求更新相关数据到center记录

				// 优化：现在center找~有木有没有就下发update 到center

				client::activity_his_record_c2g rsp_msg;
				rsp_msg.set_role_id(it->first);
				rsp_msg.set_activity_id(activity_id);
				rsp_msg.set_limit_type(activity_limit_type);
				center_send_scene(w,it->first,rsp_msg);
			}else if(activity_limit_type == REWARD_TYPE_GEM_LEVEL){
				// 宝石记录数据初始化
				w->poa_mgr->get_record_mgr().create_limit_record(activity_id,it->first,activity_limit_type);
			}
		}
	}
}
// 锻造达人奖励发放
void activity_rank_reward(center_service_t *w,uint32 act_id,client::RANK_TYPE rank_type,std::vector<config_activity_item*> item_vec)
{
	uint32 limit_value = 0;
	uint32 rank_num = 0;
	const vector<client::rank_rule> & rank_vec = w->rm_mgr.get_real_rank_list(rank_type);
	uint32 true_num = 0;
	for (uint32 j = 0; j < rank_vec.size(); ++j){
		rank_num = j + 1;
		if (true_num >= 10){
			for (uint32 k = 0; k < item_vec.size(); ++k){
				if (item_vec[k]->get_equip_order() != 0){
					continue;
				}

				const std::map<int, int>& limit_map_ = item_vec[k]->get_a_conditon();
				std::map<int, int>::const_iterator it_1 = limit_map_.begin();
				if (it_1 != limit_map_.end()){
					limit_value = it_1->second;
				}

				if (rank_vec[j].first() >= limit_value){
					true_num += 1;
					send_activity_mail(w,rank_vec[j].role_id(),item_vec[k]->get_get_item(),true_num,it_1->first,rank_vec[j].first());
					break;
				}
			}
		}else{
			for (uint32 i = true_num; i < item_vec.size(); ++i){
				const std::map<int, int>& limit_map_1 = item_vec[i]->get_a_conditon();
				std::map<int, int>::const_iterator it_1 = limit_map_1.begin();
				if (it_1 != limit_map_1.end()){
					limit_value = it_1->second;
				}

				if (rank_vec[j].first() >= limit_value){
					true_num = item_vec[i]->get_equip_order();
					if (true_num == 0){
						true_num = item_vec.size();
						send_activity_mail(w,rank_vec[j].role_id(),item_vec[i]->get_get_item(),true_num,it_1->first,rank_vec[j].first());
						break;
					}

					send_activity_mail(w,rank_vec[j].role_id(),item_vec[i]->get_get_item(),true_num,it_1->first,rank_vec[j].first());

					break;
				}
			}
		}

		
	}

	//发完奖励广播一遍最新的锻造榜 or 宝石积分榜
	client::rank_list_rsp rsp;
	rsp.set_rt(rank_type);
	uint32 cur_rank_num = 0;
	for (size_t i= 0; i < rank_vec.size(); ++i){
		const mysql::sp_load_all_user_smy *smy = w->aus_mgr.get_user_smy(rank_vec[i].role_id());
		if(smy == NULL){
			continue;
		}

		cur_rank_num = i + 1;
		if (cur_rank_num >= 100){
			break;
		}

		client::rank_item *ri = rsp.add_ri();
		ri->set_role_id(rank_vec[i].role_id());
		ri->set_value(rank_vec[i].first());
		ri->set_role_name(smy->role_name());
		ri->set_vip_lv(smy->vip_level());
		ri->set_profession(smy->profession());
	}

	w->oi_mgr.brodcast(w->ctx,rsp);

	w->poa_mgr->add_reward_flag(act_id);
	w->rm_mgr.clear_rank(static_cast<uint32>(rank_type));
}
// 冲级达人奖励发放
void activity_level_reward(center_service_t *w,uint32 act_id,std::vector<config_activity_item*> item_vec)
{
	uint32 limit_value = 0;
	uint32 rank_num = 0;
	// 给奖励（只处理通过邮件发放奖励的活动）
	const vector<client::rank_rule> & rank_vec = w->rm_mgr.get_real_rank_list(client::RANK_PERSONAL_LEVEL);
	for (uint32 j = 0; j < rank_vec.size(); ++j){
		rank_num = j + 1;
		for (uint32 i = 0; i < item_vec.size(); ++i){
			const std::map<int, int>& limit_map_1 = item_vec[i]->get_a_conditon();
			std::map<int, int>::const_iterator it_1 = limit_map_1.begin();
			if (it_1 != limit_map_1.end()){
				limit_value = it_1->second;
			}

			if (limit_value == rank_num){
				// 发邮件：
				send_activity_mail(w,rank_vec[j].role_id(),item_vec[i]->get_get_item(),rank_num,it_1->first,0);
			}

		}
	}

	if (rank_vec.size() != 0){
		w->poa_mgr->add_reward_flag(act_id);
	}
}

void copy_activity_data(center_service_t *w)
{
	std::map<uint32,activity_promotions*>& promotions_map_ = w->poa_mgr->get_promotions_mgr().mutable_promotions_list();
	std::map<uint32,activity_promotions*>::iterator it_prom = promotions_map_.begin();
	for (;it_prom != promotions_map_.end(); ++it_prom){
		const mysql::tb_promotions* info = it_prom->second->get_db_info();
		if (info != NULL){
			w->poa_mgr->reset_activity_open_data(info->id(),info->type(),info->start_time(),info->duration_time(),info->loop_time());
		}
	}

	w->poa_mgr->get_promotions_mgr().set_is_copy(true);
}

void poll_activity_on_process(center_service_t *w)
{
	if (!w->poa_mgr->get_is_load()){
		const std::map<uint32, config_activity*>& config_activity_map_ = CONFMGR->get_config_activity_list();
		if (config_activity_map_.size() != 0){
			std::map<uint32, config_activity*>::const_iterator it = config_activity_map_.begin();
			std::map<uint32,poll_activity*>& _activity_map = w->poa_mgr->get_poll_activity_list();
			for (;it != config_activity_map_.end(); ++it){
				std::map<uint32,poll_activity*>::iterator itr = _activity_map.find(it->first);
				if (itr == _activity_map.end()){
					activity_info info;
					info.operator=(*it->second);
					poll_activity* poll_obj = new poll_activity(info);
					_activity_map.insert(std::pair<uint32,poll_activity*>(it->first,poll_obj));
				}else{
					itr->second->mut_activity_info(*it->second);
				}
			}

			w->poa_mgr->set_load(true);
		}
	}

	if (!w->poa_mgr->get_promotions_mgr().get_is_copy()){
		copy_activity_data(w);
	}

	client::poll_activity_rsp rsp_msg;
	bool need_send = false;
	std::map<uint32,poll_activity*>& activity_map_ = w->poa_mgr->get_poll_activity_list();
	// 每一秒检测一次活动的状态
	std::map<uint32,poll_activity*>::iterator it_act = activity_map_.begin();
	for (;it_act != activity_map_.end();++it_act){
		bool need_brocast = false;
		uint32 back_door_type = w->poa_mgr->get_promotions_mgr().get_activity_status(it_act->first);
		it_act->second->on_process(need_brocast,back_door_type);
		uint32 cur_ba_status = w->poa_mgr->get_promotions_mgr().get_activity_status(it_act->first);
		// 后台活动状态更新
		if (cur_ba_status != BACKGROUND_ACTIVITY_OPEN_TYPE_STOP){
			if (cur_ba_status != back_door_type){
				w->poa_mgr->get_promotions_mgr().set_activity_status(it_act->first,back_door_type);
			}
		}

		if (need_brocast){
			if (!need_send){
				need_send = true;
			}
			rsp_msg.set_uixtime(0);
			client::act_open_info* info = rsp_msg.add_info();
			info->set_activity_id(it_act->first);
			info->set_is_activity(it_act->second->is_activiting());
			if (it_act->second->get_is_change_time()){
				// update到后台存储
				w->poa_mgr->get_promotions_mgr().update_activity_time(it_act->first,it_act->second->get_open_time(),it_act->second->get_close_time());
				it_act->second->set_is_change_time(false);
			}
			if (it_act->second->is_activiting()){
				w->poa_mgr->get_record_mgr().add_record(it_act->first);
				uint32 act_limit_type = CONFMGR->get_config_activity_item_mgr().get_act_limit_type(it_act->first);
				// 活动开启
				info->set_remain_time(it_act->second->get_remain_time());
				if (!it_act->second->is_same_day()){
					w->poa_mgr->get_record_mgr().is_need_reset(it_act->first);
				}

				if (!w->poa_mgr->get_record_mgr().is_record(it_act->first)){
					// 记录
					record_online_activity(w,act_limit_type,it_act->first);
					w->poa_mgr->get_record_mgr().set_record(it_act->first,true);
				}
				// 活动开启：重置领取标记
				w->poa_mgr->reset_reward_flag(it_act->first);
			}else{
				// 关闭了要移除记录的数据
				if (w->poa_mgr->get_promotions_mgr().get_activity_status(it_act->first) != BACKGROUND_ACTIVITY_OPEN_TYPE_STOP){
					if (!w->poa_mgr->is_reward(it_act->first)){
						uint32 limit_type = CONFMGR->get_config_activity_item_mgr().get_act_limit_type(it_act->first);
						// 奖励的东西
						if (limit_type == REWARD_TYPE_LEVEL ||limit_type == REWARD_TYPE_POWER || limit_type == REWARD_TYPE_GEM_LEVEL){
							std::vector<config_activity_item*> item_vec;
							item_vec.clear();
							CONFMGR->get_config_activity_item_mgr().get_act_vec(it_act->first,item_vec);
							if (item_vec.size() != 0){
								if (limit_type == REWARD_TYPE_LEVEL){
									activity_level_reward(w,it_act->first,item_vec);
								}else if (limit_type == REWARD_TYPE_POWER){
									activity_rank_reward(w,it_act->first,RANK_EQUIP_POWER,item_vec);
								}else if (limit_type == REWARD_TYPE_GEM_LEVEL){
									activity_rank_reward(w,it_act->first,RANK_GEMS_INTEGRAL,item_vec);
								}
							}
						}else {
							w->poa_mgr->add_reward_flag(it_act->first);
						}
					}

					if (w->poa_mgr->is_reward(it_act->first)){
						w->poa_mgr->get_record_mgr().remove(it_act->first);
					}
				}
			}
		}
	}

	if (need_send){
		w->oi_mgr.brodcast(w->ctx,rsp_msg);
	}
}

void init_scene_sculpture(center_service_t *w)
{
	uint32 family_id = w->ct_mgr->get_family_id();
	if(0 == family_id)
	{
		return;
	}
	uint32 chief = w->cf_mgr->get_chief(family_id);
	const mysql::sp_load_all_user_smy *smy = w->aus_mgr.get_user_smy(chief);
	if(NULL == smy)
	{
		return;
	}
	uint32 id = 0;
	switch(smy->profession())
	{
		case 0:
			id = CONFMGR->get_basic_value(10134);
			break;
		case 2:
			id = CONFMGR->get_basic_value(10135);
			break;
		default:
			id = 0;
			break;
	}
	client::update_sculpture us;
	us.set_role_id(id);
	us.set_king(w->cf_mgr->get_chief_name(family_id));
	us.set_title(w->ct_mgr->get_title());
	us.set_family_name(w->cf_mgr->get_family_name(family_id));
	center_send_scene_by_scene_id(w, 10100, us);
}

void on_area_brocast(center_service_t *w)
{
	uint32 now = (uint32)time(NULL);
	std::map<uint32, mysql::tb_center_notice*>& notic_map_ = w->notic_mgr.get_notice_map(); 

	std::map<uint32, mysql::tb_center_notice*>::iterator it = notic_map_.begin();
	client::broadcast_area_rsp rsp_msg;
	for (;it != notic_map_.end(); ++it){
		if (it->second->is_over() == 1){
			continue;
		}

		if (now < it->second->start_time()){
			continue;
		}

		if (now > it->second->close_time()){
			w->notic_mgr.set_over(it->first);
		}

		if (now > it->second->start_time() && now < it->second->close_time()){
			// 判断间隔
			if (now - it->second->brocast_time() >= it->second->interval_time()){
				// 广播
				rsp_msg.set_area_id(0);
				client::mail_body *mb = rsp_msg.mutable_area_msg();
				mb->set_content(it->second->content());
				rsp_msg.set_broadcast_count(1);
				send_broadcast_area_msg(w, rsp_msg);
				w->notic_mgr.set_broast_time(it->first);
			}
		}
	}
}
