#include "client.mounts.pb.h"
#include "client.scene_ready.pb.h"
#include "client.city.pb.h"
#include "client.open.pb.h"
#include "client.target.pb.h"
#include "client.role_login.pb.h"
#include "client.chat_msg.pb.h"
#include "config_mounts.h"
#include "config_birth.h"
#include "config_treasure.h"
#include "config_gem_act.h"
#include "config_bows.h"
#include "config_open.h"
#include "player_role.h"
#include "player_role_mounts.h"
#include "player_role_raid_mgr.h"
#include "player_role_skill_mgr.h"
#include "player_role_statis_mgr.h"
#include "player_role_treasure.h"
#include "player_role_mail.h"
#include "player_role_gem.h"
#include "player_role_props.h"
#include "player_role_bows.h"
#include "scene_lose_mgr.h"
#include "game_interface.h"
#include "config_mail.h"
#include "config_raid.h"
#include "levels_scene_inst.h"
#include "raid_scene_inst.h"
#include "client.offline.pb.h"
#ifndef __LIB_MINI_SERVER__
#include "client.devil.pb.h"
#include "player_role_devil.h"
#endif

void fill_task_msg(client::task_msg_type t, const std::map<uint32, client::task_accepted>& accepteds, const std::map<uint32, client::task_completed>& completeds, client::task_msg& msg);

void send_task_msg(struct game_service_t* ss, scene_user* psu, scene_inst* si)
{
    // 如果没有任务记录初始化主线任务
    std::map<uint32, client::task_accepted>*  pa = psu->get_player_role().get_tasks().mutable_accepted_tasks() ;
    std::map<uint32, client::task_completed>* pc = psu->get_player_role().get_tasks().mutable_completed_tasks();
    if (pa->empty() && pc->empty()) {
        const config_birth_mgr* mgr = GET_CONFIG_MGR(config_birth_mgr);
        if (mgr == NULL) {
            return ;
        }

        const config_birth* pcb = mgr->get_config_birth(psu->get_attr(PROFESSION));
        if (pcb == NULL) {
            return ;
        }

        client::task_accepted& a = (*pa)[pcb->get_quest_id()];
        a.set_task_id(pcb->get_quest_id());
        a.mutable_progress();
    }

    client::task_msg tmsg;
    fill_task_msg(client::task_msg_type_insert,
        psu->get_player_role().get_tasks().get_accepted_tasks(),
        psu->get_player_role().get_tasks().get_completed_tasks(),
        tmsg);
    psu->send_client(ss->ctx, tmsg);
}

void send_buffer_list(struct game_service_t* ss, scene_user* su, scene_inst* si)
{
    client::buffer_list bl;
    if (su->mutable_buffer()->fill_all_buffer_list(bl) > 0) {
        su->fill_role_data(bl.mutable_rd());
        if (si != NULL) {
            si->screen_broadcast(*su, bl, false);
        }
    }
}

void send_mounts(struct game_service_t* ss, scene_user* su, scene_inst* si)
{
    // TODO: 测试代码 如果没有坐骑初始化一个
    const mysql::tb_role_mounts& mounts = su->get_player_role().get_mounts().get_db();
    if (mounts.mount_id() == 0) {
        mysql::tb_role_mounts* pm = su->get_player_role().get_mounts().mutable_db();
        pm->set_role_id(su->get_role_id());
        pm->set_mount_id(1);
        pm->set_advance_num(0);
        pm->set_advance_time(0);
        pm->set_advance_bless(0);
        pm->set_skill_upgrade_num(10);
        pm->set_mount_level(1);
        pm->set_mount_exp(0);
        uint32 mount_power = static_cast<uint32>(ceil(GET_CONFIG_MGR(config_mounts_mgr)->calculate_mount_power(pm->mount_id(),pm->mount_level())));
        pm->set_mount_power(mount_power);
        const config_mounts* p = CONFMGR->get_config_mounts_min();
        if (p != NULL) {
            const config_mounts_base* base = p->get_base();
            const std::vector<int>& skills = base->get_talent_skill();
            for (size_t i = 0; i < skills.size(); ++i) {
                su->get_player_role().get_role_skill_mgr().add_skill(su->get_role_id(), skills[i]);
            }
        }

        su->calculate_attr();
        su->send_client_change_attr();
    }

    //////////////////////////////////////
    if (mounts.mount_id() != 0) {
        client::mounts_msg mmsg;
        client::mounts_info* pmi = mmsg.add_mi();
        pmi->set_mount_id(mounts.mount_id());
        pmi->set_advance_time(mounts.advance_time());
        pmi->set_advance_bless(mounts.advance_bless());
        pmi->set_skill_upgrade_num(mounts.skill_upgrade_num());
        pmi->set_mount_level(mounts.mount_level());
        pmi->set_mount_exp(mounts.mount_exp());
        pmi->set_mount_power(mounts.mount_power());

        su->send_client(ss->ctx, mmsg);
    }
}

void send_bows_info(struct game_service_t* ss, scene_user* su, scene_inst* si)
{
	const mysql::tb_role_bows& bows = su->get_player_role().get_player_role_bows().get_db();

	const config_open_mgr* mgr = GET_CONFIG_MGR(config_open_mgr);
	const config_open* bow_info = mgr->get_config_open(19);
	if (bow_info != NULL){
		if (su->get_attr(LEVEL) >= static_cast<uint32>(bow_info->get_open_condition())){
			const mysql::tb_role_bows& bows = su->get_player_role().get_player_role_bows().get_db();
			if (bows.bow_id() == 0) {
				mysql::tb_role_bows* pm = su->get_player_role().get_player_role_bows().mutable_db();
				pm->set_role_id(su->get_role_id());
				pm->set_bow_id(1);
				pm->set_advance_num(0);
				pm->set_advance_time(0);
				pm->set_advance_bless(0);
				pm->set_bow_level(1);
				pm->set_bow_exp(0);
				uint32 bows_power = static_cast<uint32>(ceil(GET_CONFIG_MGR(config_bows_mgr)->calculate_bow_power(pm->bow_id(),pm->bow_level())));
				pm->set_bow_power(bows_power);
			}
		}
	}

	if (bows.bow_id() != 0) {
		client::bows_msg mmsg;
		client::bows_info* pmi = mmsg.add_mi();
		pmi->set_bow_id(bows.bow_id());
		pmi->set_advance_time(bows.advance_time());
		pmi->set_advance_bless(bows.advance_bless());
		pmi->set_bow_level(bows.bow_level());
		pmi->set_bow_exp(bows.bow_exp());
		pmi->set_bow_power(bows.bow_power());
		su->send_client(ss->ctx, mmsg);
	}
}

void send_pet(struct game_service_t* ss, scene_user* su, scene_inst* si)
{
    player_role_pet& player_pet = su->get_player_role().get_pets();

    // 下发美人列表
    client::pet_list_rsp list_rsp;
    client::role_data* role_data = 	list_rsp.mutable_rd();
    role_data->set_role_id(su->get_role_id());
    role_data->set_inst_id(su->get_inst_id());
    role_data->set_role_typ((client::role_data_TYPE)su->get_role_type());
    const std::map<uint32, client::pet_db_info>& pet_info_map_ = player_pet.get_all_pet_db_info();
    std::map<uint32, client::pet_db_info>::const_iterator itr_1 = pet_info_map_.begin();
    for (; itr_1 != pet_info_map_.end();++itr_1){
        client::pet_db_info* info = list_rsp.add_pet_info();
        info->CopyFrom(itr_1->second);
    }

    su->send_client(list_rsp);

    client::role_skill_msg rsm;
    su->fill_all_pets_skill_msg(rsm);
    su->send_client(rsm);

    // 下发当前出战的宠物数据与场景中创建已经出战状态的宠物
    //const std::map<uint32, client::pet_db_info>& pet_db_map_ = player_pet.get_all_pet_db_info();
    //std::map<uint32, client::pet_db_info>::const_iterator itr = pet_db_map_.begin();
    //for (;itr != pet_db_map_.end(); ++itr){
    //    if (itr->second.status() == PET_STATUS_FIGHTING){
    //        su->call_pet(itr->first,player_pet);
    //        break;
    //    }
    //}
}

void send_mail_list(struct game_service_t* ss, scene_user* su, scene_inst* si)
{
	client::mail_list_rsp mail_rsp;
	su->get_player_role().get_role_mails().proc_mail_list_req(mail_rsp);
	su->send_client(mail_rsp);
}

void send_target_list(struct game_service_t *ss, scene_user *psu, scene_inst *si)
{
#ifndef __LIB_MINI_SERVER__
	client::target_list_rsp rsp;
	rsp.set_type(client::ACHIEVEMENT);
	fill_target_list(rsp, psu, client::ACHIEVEMENT);
	psu->send_client(rsp);
	rsp.Clear();
	rsp.set_type(client::DAILY_MISSION);
	fill_target_list(rsp, psu, client::DAILY_MISSION);
	psu->send_client(rsp);
#endif // __LIB_MINI_SERVER__
}

void send_offline_timelong(struct game_service_t *ss, scene_user *psu, scene_inst *si)
{
#ifndef __LIB_MINI_SERVER__
	client::offline_timelong_rsp rsp;
	player_role_statis_mgr &mgr = psu->get_player_role().get_role_statis_mgr();
	rsp.set_time(mgr.get_offline_timelong());
	rsp.set_total(mgr.get_total_offline_tl());
	psu->send_client(rsp);
#endif
}

void send_sign_in_list(struct game_service_t* ss, scene_user* su, scene_inst* si)
{
	client::sign_list_rsp rsp_msg;

	su->on_pb_sign_list_req(rsp_msg);

	su->send_client(rsp_msg);
}

void send_land_reward_list(struct game_service_t* ss, scene_user* su, scene_inst* si)
{
	client::land_reward_list_rsp rsp_msg;

	su->on_pb_land_reward_list_req(rsp_msg);

	su->send_client(rsp_msg);
}

void update_family_data(struct game_service_t* ss, scene_user* su, scene_inst* si)
{
    uint32 family_id = 0;
	family_id = su->mutable_attr()->get_val(FAMILY_ID);

    client::update_family_data_from_center center_msg;
    center_msg.set_family_id(family_id);
    center_msg.set_role_id(su->get_role_id());
    game_send_center(ss,center_msg);

	if(family_id != 0)
	{
		client::load_friend_family_req req;
		req.set_role_id(su->get_role_id());
		req.set_family_id(family_id);
		game_send_center(ss, req);
	}
}

void update_activity_status(struct game_service_t* ss, scene_user* su, scene_inst* si)
{
	client::load_activity_list_center req;
	req.set_role_id(su->get_role_id());
	game_send_center(ss, req);
}

void update_title_info(struct game_service_t* ss, scene_user* su, scene_inst* si)
{
	client::update_title_info_req req_msg;
	
	req_msg.set_role_id(su->get_role_id());
	game_send_center(ss, req_msg);

}

void check_king_skill(struct game_service_t *ss, scene_user *su)
{
#ifndef __LIB_MINI_SERVER__
	uint32 family_id = su->get_family_id();
	if(family_id == 0)
	{
		return;
	}
	client::king_skill_add ksa;
	ksa.set_role_id(su->get_role_id());
	ksa.set_family_id(family_id);
	game_send_center(ss, ksa);
#endif
}

void check_gem_info(struct game_service_t* ss, scene_user* su, scene_inst* si)
{
	uint32 hole_count = CONFMGR->get_config_gem_act_mgr().get_config_gem_act(2,su->get_level());
	if (hole_count == 0){
		return;
	}

	for (uint32 i = EQUIP_GRID_BEG; i <=  EQUIP_SHOE; ++i){
		for (uint32 j = 1; j <= hole_count; ++j){
			su->get_player_role().get_player_role_gems().create_gem_info(i,j);
		}
	}

}

void check_skill_info(struct game_service_t* ss, scene_user* su, scene_inst* si)
{
	su->get_player_role().get_role_skill_mgr().check_old_skill();
	su->mutable_skill()->check_old_skill();
}

void update_siege_status(struct game_service_t *ss, scene_user *su)
{
#ifndef __LIB_MINI_SERVER__
    client::siege_status center_siege;
    center_siege.set_role_id(su->get_role_id());
    game_send_center(ss, center_siege);
#endif // __LIB_MINI_SERVER__
}

void send_sys_mail_test(struct game_service_t* ss, scene_user* su, scene_inst* si)
{
    string title = "email_title";
    string content = "email_content";
    std::vector<client::mail_attr> attr;
    std::vector<client::content_data> content_data;
    content_data.clear();
    attr.clear();
    std::vector<props_info> ma;
    ma.clear();

    uint32 att_type[3] = {2,4,17};
    uint32 att_value[3] = {200,200,2000};
    uint32 item_id[4] = {50015001,50105001,50125003,50085001};
    uint32 item_count[4] = {3,2,1,4};
    attr.resize(3);
    for (uint32 i = 0; i < 3; ++i){
        attr[i].set_attr_type(att_type[i]);
        attr[i].set_attr_value(att_value[i]); 
    }
    ma.resize(4);
    for (uint32 j = 0; j < 4; ++j){
        ma[j].set_props_id(item_id[j]);
        ma[j].set_count(item_count[j]);
        ma[j].set_pos(0);
        ma[j].set_grid_index(0);
        ma[j].set_bind(0);
        ma[j].set_quality(1);
    }

    si->sys_mail(su,title,content,attr,ma,content_data);
}

void send_raid(scene_user* psu)
{
    client::raid_info_msg rim;
    psu->get_player_role().get_raid_mgr().fill_raid_info_msg(rim);
    psu->send_client(rim);
}

void send_devil_raids(scene_user *psu)
{
#ifndef __LIB_MINI_SERVER__
	client::devil_stage_list list;
	psu->get_player_role().get_role_devil().fill_devil_raids(list);
	psu->send_client(list);
#endif
}

void send_open(scene_user* psu)
{
    client::open_msg om;
    om.set_t(client::open_msg_type_now);
    const std::map<uint32, client::open_info>& opens = psu->get_player_role().get_tasks().get_opens();
    std::map<uint32, client::open_info>::const_iterator begin = opens.begin();
    std::map<uint32, client::open_info>::const_iterator end   = opens.end();
    for ( ; begin != end; ++begin) {
        om.add_id()->CopyFrom(begin->second);
    }

    psu->send_client(om);
}

void game_send_personal_info(struct game_service_t* ss, scene_user* psu, scene_inst* si)
{
	check_skill_info(ss, psu, si);

    client::role_skill_msg rsm;
    psu->fill_role_skill_msg(rsm);
    psu->send_client(ss->ctx, rsm);

    client::role_props_msg rpm;
    psu->fill_props_msg(rpm);
    psu->send_client(ss->ctx, rpm);

    client::day_info data_info;
    psu->fill_cur_date(data_info);

    send_task_msg(ss, psu, si);
    send_buffer_list(ss, psu, si);
    send_mounts(ss, psu, si);
    send_pet(ss, psu, si);
    //send_sys_mail_test(ss, su, si);
    // update军团的数据，更新
    update_family_data(ss, psu, si);
    // 下发攻城状态
    update_siege_status(ss, psu);
	// 下发邮件列表
	send_mail_list(ss, psu, si);
	// 下发签到列表
	send_sign_in_list(ss, psu, si);
	// 下发开服奖励列表
	send_land_reward_list(ss, psu, si);
	// 下发日常成就列表
	send_target_list(ss, psu, si);
	// 向center 请求下发活动数据
	update_activity_status(ss, psu, si);
	// 向center 请求下发聊天相关头衔数据
	update_title_info(ss, psu, si);

	send_offline_timelong(ss, psu, si);
    send_raid(psu);
    send_open(psu);

	check_king_skill(ss, psu);

	check_gem_info(ss, psu, si);

	send_bows_info(ss, psu, si);

	send_devil_raids(psu);
}

static void send_personal_info_success(scene_user* psu)
{
    client::personal_info_success pis;
    pis.set_ret(true);
    psu->send_client(pis);
}

void* on_game_pb_scene_ready(client::scene_ready* p, void* ud, size_t* rsp_len)
{
    PB_FUNC_GUARD(p);

    scene_inst* psi = psu->get_scene();
    if (psi == NULL) {
        return NULL;
    }

    psu->set_scene_ready(true);
    //psu->get_player_role().write_all(ss->ctx);
    //su->on_region_change(NULL, si->get_region_info(su->get_scene_pos()));

    client::multi_role_msg mrm;
    psu->fill_user_msg(*mrm.add_rm());
    psu->send_client(ss->ctx, mrm);

    psi->screen_push_user(psu);

    client::terrain_list tl;
    psi->get_terrain_mgr().fill_terrain_list(tl);
    if (tl.toi_size() > 0) {
        psu->send_client(ss->ctx, tl);
    }

    if (!psi->get_scene_lose_mgr()->get_all_lost().empty()) {
        client::drop_props_msg dpm;
        if (psi->get_scene_lose_mgr()->fill_loses(dpm)) {
            psu->send_client(ss->ctx, dpm);
        }
    }


	if(psi->get_scene_type() == MAP_TYPE_NORMAL) {
		psi->send_scene_sculpture(psu);
	}
	psi->set_scene_ready(true, psu);

    if (p->has_ro() && p->ro() == client::scene_ready_READY_OCCASION_JUMP) { // 只有登录场景确认才会发个人信息
        psu->save_user();
        return NULL;
    }

    game_send_personal_info(ss, psu, psi);
    send_personal_info_success(psu);

    return NULL;
}
