#include <math.h>
#include "config_map.h"
#include "config_raid.h"
#include "config_npc.h"
#include "config_props.h"
#include "config_upgrade.h"
#include "config_beauty.h"
#include "config_basic.h"
#include "config_treasure.h"
#include "config_open.h"
#include "config_mounts.h"
#include "config_legion.h"
#include "config_bows.h"
#include "config_gem_act.h"
#include "client.mounts.pb.h"
#include "client.open.pb.h"
#include "mysql.t_upgradelog.pb.h"
#include "mysql.t_moneylog.pb.h"
#include "game_map_mgr.h"
#include "raid_scene.h"
#include "player_role.h"
#include "player_role_auto_raid.h"
#include "player_role_props.h"
#include "operate_log.h"
#include "player_role_buffer.h"
#include "player_role_mail.h"
#include "player_role_mounts.h"
#include "player_role_raid_mgr.h"
#include "player_role_skill_mgr.h"
#include "player_role_statis_mgr.h"
#include "player_role_status.h"
#include "player_role_treasure.h"
#include "player_role_ext_attr.h"
#include "player_role_black_shop.h"
#include "player_role_treasure.h"
#include "player_role_cd_cooldown.h"
#include "player_role_bows.h"
#include "player_role_gem.h"

#ifndef __LIB_MINI_SERVER__
#include "player_role_mission.h"
#include "player_role_vip.h"
#include "player_role_lottery.h"
#include "player_role_cycle.h"
#include "player_role_devil.h"
#include "config_devil.h"
#include "client.devil.pb.h"
#endif

#include "scene_lose_mgr.h"
#include "scene_npc.h"
#include "scene_pet.h"
#include "scene_inst.h"
#include "scene_user.h"

void get_price(int id,ROLE_ATTRIBUTE& pay_type, bool& is_item);

scene_user::scene_user(player_role* p)
    : user(p)
    , scene_role((const uint64)(p->get_role_id()))
    , _task(p->get_tasks())
{
    assert(p != NULL);
    set_role_type(RTE_USER);
    _agent_id  = 0;
    _client_id = 0;

    _save_map_id   = 0;
    _save_scene_id = 0;

    _practice_time         = 0;
    _refresh_practice_time = 0;
    _comm_practice_time    = 0;
    _auto_comm_practice    = true;
    _comm_practice_user    = 0;
    _physical_tick         = 0;

    _pick_npc_inst_id = 0;
    set_team_id(p->get_team_id());
    _scene_ready      = false;

    _dead_time         = 0;
    _sub_infamy_timer  = 0;
    _raid_type         = client::enter_raid_req_type_normal;
	_store_weapon = 0;
	_king_weapon = 0;
#ifdef __LIB_MINI_SERVER__
    _raid_count        = 0;
#endif // __LIB_MINI_SERVER__

    _raid_id   = 0;
    _raid_time = 0;
}

scene_user::~scene_user()
{
}

int scene_user::init()
{
    return 0;
}

int scene_user::init(uint32 mid, uint32 sid)
{
    int ret = init_user_attr();
    if (ret != 0) {
        return -1;
    }

    player_role_props& props = get_player_role().get_role_props();
    //mutable_attr()->set_val(ARMOR_ID, props.get_suit_id());
#ifndef __LIB_MINI_SERVER__
    mutable_attr()->set_armor_id(props.get_suit_id());
    mutable_attr()->set_weapon_id(props.get_weapon_id());
    uint32 online_time  = get_attr().get_val(ONLINE_TIME);
    uint32 offline_time = get_attr().get_val(OFFLINE_TIME);
    uint32 space_time = online_time > offline_time ? online_time - offline_time : 0;
    if(get_attr().get_val(PHYSICAL_POWER) < CONFMGR->get_config_basic_mgr().get_physical_max()) {
        uint32 physical = min(get_attr().get_val(PHYSICAL_POWER) + space_time / (CONFMGR->get_config_basic_mgr().get_physical_tick() / 100), CONFMGR->get_config_basic_mgr().get_physical_max());
        mutable_attr()->set_val(PHYSICAL_POWER, physical);
    }
	modify_offline_timelong();
#endif // __LIB_MINI_SERVER__

    player_role_status& prs = _spr->get_role_status();
    ret = init_status(prs, mid, sid);
    if (ret != 0) {
#ifndef __LIB_MINI_SERVER__
        return -2;
#endif // __LIB_MINI_SERVER__
    }

    ret = init_user_skill();
    if (ret != 0) {
        return -3;
    }

    ret = init_user_buffer();
    if (ret != 0) {
        return -4;
    }

    init_day();

    calculate_attr();

    calculate_offline_data();

    return 0;
}

void scene_user::modify_offline_timelong()
{
	uint32 off = get_attr().get_val(OFFLINE_TIME);
	if(off == 0)
	{
		return ;
	}
	if(!has_func_open(OFFLINE_FUNC_ID))
	{
		return ;
	}
	player_role_statis_mgr &prsm = get_player_role().get_role_statis_mgr();
	uint32 max_tl = CONFMGR->get_basic_value(10156);
	if(prsm.get_total_offline_tl() >= max_tl)
	{
		return ;
	}
	uint32 now = (uint32)time(NULL);
	uint32 diff;
	if(!time_utils::is_same_day(now, off))
	{
		diff = now - time_utils::make_day_start_time();
	}
	else
	{
		diff = now - off;
	}
	uint32 limit = CONFMGR->get_basic_value(10152);
	uint32 tl = prsm.get_offline_timelong();
	if(tl + diff > limit)
	{
		diff = limit - tl;
	}
	uint32 total = prsm.get_total_offline_tl();
	if(total + diff > max_tl)
	{
		diff = max_tl - total;
	}
	prsm.add_offline_timelong(diff);
}

int scene_user::init_user_attr()
{
    mysql::tb_player_role* tpr = get_player_role().get_db_player_role();
    if (tpr == NULL) {
        amtsvr_log("mysql::tb_player_role:NULL\n");
        return -1;
    }

    set_role_id(tpr->role_id());
    snprintf(_role_name, sizeof(_role_name), "%s", tpr->role_name().c_str());
    get_player_role().init_role_attr(*mutable_attr());

    return 0;
}

int scene_user::init_user_skill()
{
#ifdef __INNER_DEBUG__PACKAGE_
    // 方便策划修改配置后重新登陆学会已满足学习条件的技能
    get_player_role().get_role_skill_mgr().check_skill(get_role_id(), get_attr(PROFESSION), get_attr(LEVEL));
#endif // __INNER_DEBUG__PACKAGE_

    std::vector<player_role_skill>& sk_list = get_player_role().get_role_skill_mgr().get_skills();
    for (size_t i = 0; i < sk_list.size(); ++i) {
        if (sk_list[i].get_db_role_skill() == NULL) {
            continue;
        }

        const config_skill* pcs = CONFMGR->get_config_skill(sk_list[i].get_db_role_skill()->skill_id());
        if (pcs == NULL) {
            amtsvr_log("not found user skill %u\n", sk_list[i].get_db_role_skill()->skill_id());
            get_player_role().get_role_skill_mgr().delete_skill(sk_list[i].get_db_role_skill()->skill_id());
            continue;
        }

        skill sk(*(sk_list[i].get_db_role_skill()), *pcs);
        mutable_skill()->add_skill(sk);
    }

    mutable_skill()->set_attack_speed(_attr.get_val(ATTACK_SPEED));
    mutable_skill()->init_max_anger();

    return 0;
}

int scene_user::init_user_buffer()
{
    mutable_buffer()->set_prb(&get_player_role().get_buffer());
    const std::multimap<uint32, client::buffer_info>& bimap = get_player_role().get_buffer().get_all_buffers();
    std::multimap<uint32, client::buffer_info>::const_iterator it;
    for (it = bimap.begin(); it != bimap.end(); ++it) {
        mutable_buffer()->init_buf(it->second);
    }

    return 0;
}

int scene_user::init_day()
{
#ifndef __LIB_MINI_SERVER__
    // 判断是否今天
    if (!get_player_role().get_role_statis_mgr().is_day_init()) {
        return -1;
    }
    //////////////////////////////////////////////////////////////
    ///////////////////////////重置相关//////////////////////////
    // 坐骑技能相关重置
    init_mount_skill_count();
    // 寻宝记录重置
    init_treasure_data();
    // 心法冲击波经验获得总量重置
    reset_potential_share_exp();
    //
    client::raid_info_msg rim;
    fill_raid_info_msg(rim);
    send_client(rim);

    get_player_role().get_role_statis_mgr().set_day_init();

    // 重置日常任务统计
    get_player_role().get_player_role_target().clear_daily_progress();

    // 重置VIP购买记录
    get_player_role().get_player_role_vip().clear_vip_op_info();

    // 重置宝箱
	get_player_role().get_player_role_lottery().clear_props();
	// 登陆天数累加1
	get_player_role().get_role_statis_mgr().add_land_day();

	CHECK_TARGET_PROGRESS(this, GET_INGOT);

	get_player_role().get_role_cycle().clear_helpers();
#endif
    return 0;
}

int scene_user::init_task(int32 day_ret, uint32 now)
{
    if (day_ret != 0) {
        // task_operator.init_visible_list(now, true);
    }

    return 0;
}

int scene_user::init_status(player_role_status& prs, uint32 mid, uint32 sid)
{
    mysql::tb_role_status* trs = prs.get_db_role_status();
    if (trs == NULL) {
        return -1;
    }

    if (mutable_status()->init(trs, this) != 0) {
        return -2;
    }

    if (mid == trs->tmp_map_id() && sid == trs->tmp_scene_id()) { // 进入的是临时场景
        _save_map_id   = trs->pos_map_id();
        _save_scene_id = trs->pos_scene_id();
        _save_pos.reset((int)trs->pos_x(), (int)trs->pos_y(), (int)trs->pos_z());
        _map_id   = mid;
        _scene_id = sid;
        _scene_pos.reset((int)trs->tmp_x(), (int)trs->tmp_y(), (int)trs->tmp_z());
    } else if (mid == trs->pos_map_id() && sid == trs->pos_scene_id()) { // 进入正常的场景
        _map_id   = mid;
        _scene_id = sid;
        _scene_pos.reset((int)trs->pos_x(), (int)trs->pos_y(), (int)trs->pos_z());
    } else {    // 所要进入的场景没有保存，这里为了容错，要将用户弹出到一个固定的场景坐标
        return -3;
    }

    return 0;
}

void fill_mounts_msg(const mysql::tb_role_mounts& mounts, client::mounts_msg& mmsg);

int scene_user::init_mount_skill_count()
{
    player_role_mounts& m = get_player_role().get_mounts();
    mysql::tb_role_mounts* pdb = m.mutable_db();
    if (pdb != NULL && pdb->mount_id() != 0) {
        pdb->set_skill_upgrade_num(10);
        const mysql::tb_role_mounts& mounts = get_player_role().get_mounts().get_db();
        client::mounts_msg mmsg;
#ifndef __LIB_MINI_SERVER__
        fill_mounts_msg(mounts, mmsg);
        send_client(mmsg);
#endif // __LIB_MINI_SERVER__
    }

    return 0;
}

int scene_user::init_treasure_data()
{
    get_player_role().get_role_treasures().remove_all();

    return 0;
}

int scene_user::init_vip()
{
    return 0;
}

int  scene_user::reset_potential_share_exp()
{
	mutable_attr()->set_val(POTENTIAL_ACCUMULATION,0);

    return 0;
}

void scene_user::fill_query_attr(client::role_attr& ra)
{
}

void scene_user::fill_user_msg(client::role_msg& rm)
{
    fill_role_data(*(rm.mutable_rd()));
    fill_role_attr(*rm.mutable_attr());
    fill_role_status(*(rm.mutable_status()));
}

void scene_user::fill_cur_date(client::day_info& data_info)
{
    time_t now = time(NULL);
    struct tm tm_temp;
    localtime_r(&now, &tm_temp);
    data_info.set_day(tm_temp.tm_mday);
    data_info.set_month(tm_temp.tm_mon +1);
    data_info.set_year(tm_temp.tm_year + 1900);
    if (tm_temp.tm_wday == 0){
        data_info.set_week_day(7);
    }else{
        data_info.set_week_day(tm_temp.tm_wday); // 1-6
    }

    uint32 cur_max_day = 0;
    // 判断润年，平年，判断月份，天数
    switch(data_info.month())
    {
    case 1:
    case 3:
    case 5:
    case 7:
    case 8:
    case 10:
    case 12:
        {
            cur_max_day = 31;
        }
        break;
    case 2:
        {
            // 判断润年，平年
            if (((data_info.year() % 4) == 0 && (data_info.year() % 100) != 0)
                || (data_info.year() % 400) == 0){
                    cur_max_day = 29;
            }else {
                cur_max_day = 28;
            }
        }
        break;
    default:
        {
            cur_max_day = 30;
        }
        break;
    }

    data_info.set_max_day(cur_max_day);
}

void scene_user::fill_props_msg(client::role_props_msg& rpm)
{
    fill_role_data(*(rpm.mutable_rd()));
    get_player_role().get_role_props().fill_all_props_info(rpm);
}

void scene_user::send_client_change_props_msg()
{
    client::role_props_msg rpm;
    get_player_role().get_role_props().fill_change_props_info(rpm);
    if (rpm.pi_size() > 0) {
        fill_role_data(*(rpm.mutable_rd()));
        send_client(rpm);
    }
}

void scene_user::fill_raid_info_msg(client::raid_info_msg& rim)
{
    get_player_role().get_raid_mgr().fill_raid_info_msg(rim);
}

int scene_user::send_agent(google::protobuf::Message& msg)
{
    if (get_scene() == NULL) {
        return -1;
    }

    return send_agent(get_scene()->get_ctx(), msg);
}

int scene_user::send_agent(struct amtsvr_context* ctx, google::protobuf::Message& msg)
{
    int len = 0;
    void* buf = svr_encode(msg, len);
    if (buf == NULL) {
        return -1;
    }

    amtsvr_send(ctx, 0, _agent_id, PTYPE_CLIENT|PTYPE_TAG_DONTCOPY, 0, buf, (size_t)len);
    return 0;
}

int scene_user::send_client(struct amtsvr_context* ctx, google::protobuf::Message& msg)
{
    size_t msg_len = 0;
    void* buf = encode(msg, msg_len);
    if (buf == NULL) {
        return -1;
    }

    amtsvr_send(ctx, 0, _client_id, PTYPE_CLIENT|PTYPE_TAG_DONTCOPY, 0, buf, msg_len);
    return 0;
}

int scene_user::send_client(google::protobuf::Message& msg)
{
    if (get_scene() == NULL) {
        return -1;
    }

    return send_client(get_scene()->get_ctx(), msg);
}

uint32 scene_user::get_mail_past_time(uint32 mail_type,bool has_adjunct)
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

int scene_user::on_jump_scene_by_scene(client::jump_scene* js, client::jump_user& ju)
{
    uint32 scene_id =  js->dst_id();
    if (scene_id == 0 || get_scene() == NULL) {
        return -1;
    }

    if (scene_id == get_scene()->get_scene_id() && (js->x() != 0 || js->y() != 0)) {
        ju.set_x(js->x());
        ju.set_y(js->y());
        ju.set_z(0);
        ju.set_same_scene(true);

        return 0;
    }

    const config_map* cm = CONFMGR->get_config_map(scene_id);
    if (cm == NULL) {
        return -2;
    }

    uintptr_t ptr = (uintptr_t)this;
    ju.set_ptr((uint64)ptr);
    ju.set_role_id(get_role_id());;
    ju.set_map_id(scene_id);
    ju.set_scene_id(scene_id);
    ju.set_trans_id(scene_id);
    ju.set_x(cm->get_birth_point_x());
    ju.set_y(cm->get_birth_point_y());
    ju.set_z(0);
    ju.set_same_scene(false);
    if (js->x() != 0 && js->y() != 0) {
        ju.set_x(js->x());
        ju.set_y(js->y());
    }

    return 0;
}

bool scene_user::on_update(uint32 tick, uint32 user_num, bool is_one_sec, bool is_fiv_sec, bool is_ten_sec, bool is_thirty_sec, bool is_one_minute)
{
    if (get_scene() == NULL || !get_scene_ready()) {
        return true;
    }

    on_update_status(tick);

    on_update_move(tick);

    return scene_role::on_update(tick, user_num, is_one_sec, is_fiv_sec, is_ten_sec, is_thirty_sec, is_one_minute);
}

bool scene_user::on_update_one_second(uint32 tick, uint32 user_num)
{
	on_update_mount_bless_time();
	on_update_bow_bless_time();
    on_update_skill_point();
    on_update_black_shop();
	on_update_props_date();

	CHECK_TARGET_PROGRESS(this, GET_PHYSIC_POWER);

    return scene_role::on_update_one_second(tick, user_num);
}

bool scene_user::on_update_fiv_second(uint32 tick, uint32 user_num)
{
    return scene_role::on_update_fiv_second(tick, user_num);
}

bool scene_user::on_update_ten_second(uint32 tick, uint32 user_num)
{
    //if (get_status().is_meditate()) {
    //    uint32 cur_level = mutable_attr()->get_val(LEVEL);
    //    //uint32 cur_job = mutable_attr()->get_val(PROFESSION);
    //    /*const config_upgrade* upgrade_info = CONFMGR->get_config_upgrade_mgr().get_config_upgrade(cur_job,cur_level);*/
    //    if (_practice_time >= (uint32)time(NULL) + 20) {
    //        add_experience(cur_level);
    //        // 测试
    //        if (cur_level >= MEDITATE_STRATEGY_LEVEL_LIMIT) {
    //            mutable_attr()->add_val(MILITARY_STRATEGY, cur_level);
    //        }
    //    }
    //}

    return true;
}

bool scene_user::on_update_thirty_second(uint32 tick, uint32 user_num)
{
    return scene_role::on_update_thirty_second(tick, user_num);
}

bool scene_user::on_update_one_minute(uint32 tick, uint32 user_nmu)
{
    get_player_role().get_role_statis_mgr().add_online_time();
    recover_physical_power(tick);


    // 重置相关
    init_day();

    return scene_role::on_update_one_minute(tick, user_nmu);
}

int scene_user::fill_back_trans_jump(client::jump_user& ju)
{
    const game_map* back_gm = GAME_MAP_MGR->get_map(_save_map_id);
    if (back_gm == NULL || !(back_gm->get_type() == MAP_TYPE_NORMAL) || _save_map_id == 0 || _save_scene_id == 0 || (_save_pos.x == 0 && _save_pos.y == 0)) {
        return fill_default_trans_jump(ju);
    }

    uintptr_t ptr = (uintptr_t)this;
    ju.set_ptr((uint64)ptr);
    ju.set_map_id(_save_map_id);
    ju.set_scene_id(_save_scene_id);
    ju.set_trans_id(0);
    ju.set_x(_save_pos.x);
    ju.set_y(_save_pos.y);
    ju.set_z(_save_pos.z);
    ju.set_same_scene(is_same_map_and_scene(ju.map_id(), ju.scene_id()));

    return 0;
}

int scene_user::fill_default_trans_jump(client::jump_user& ju)
{
    const config_map* cm = CONFMGR->get_config_map_default();
    if (cm == NULL) {
        return -1;
    }

    uintptr_t ptr = (uintptr_t)this;
    ju.set_ptr((uint64)ptr);
    ju.set_map_id(cm->id);
    ju.set_scene_id(cm->id);
    ju.set_trans_id(0);
    ju.set_x(cm->birthPointX);
    ju.set_y(cm->birthPointY);
    ju.set_z(0);
    ju.set_same_scene(is_same_map_and_scene(ju.map_id(), ju.scene_id()));

    return 0;
}

int scene_user::trans_back_user()
{
    if (get_scene() == NULL) {
        return -1;
    }

    server::trans_back_user tbu;
    tbu.set_game_handle(get_scene()->get_game_handle());
    tbu.add_role_id(get_role_id());
    get_scene()->send_center(tbu);

    return 0;
}

int scene_user::trans_back_user_at_once()
{
    if (get_scene() == NULL) {
        return -1;
    }

    client::jump_user ju;
    ju.set_role_id(get_role_id());
    if (fill_back_trans_jump(ju) != 0) {
        return -2;
    }

    struct amtsvr_context* c = get_scene()->get_ctx();
    get_scene()->leave_scene(this);
    detach_scene();
    send_center(c, ju);

    return 0;
}

bool scene_user::on_update_status(uint32 tick)
{
#if 0
    if (get_status().is_dead()) {
        if (get_dead_time() == 0) {
            set_dead_time(tick);
        }

        scene_inst *si = get_scene();
        if(si == NULL) {
            return true;
        }

        uint32 tl = si->get_relive_timelong();
        if (tick >= get_dead_time() + tl) {
            relive();
        }
    }
#endif
    return true;
}

bool scene_user::on_update_move(uint32 tick)
{
    if (get_move_count() <= 1) {
        return true;
    }

    return move_by_path(tick);
}

bool scene_user::on_update_practice(uint32 tick)
{
    return true;
}

int scene_user::on_killed(scene_role* psr)
{
    //     if (get_scene() != NULL && get_scene()->is_raid_scene()) {
    //         relive();
    //         send_client_change_attr();
    //         trans_back_user(); // 通过消息，让center来将用户跳转回去
    //         in_scene->trans_back_all_user();// 这里不能trans back user因为外围有用户会调用Attack事件，会有inscene的广播,不能在战斗回调事件中这样与场景分离
    //     }

    if (psr == NULL) {
        return scene_role::on_killed(psr);
    }

    /*client::notify_kill_event nke;
    nke.set_dead_id(get_role_id());
    nke.set_lose_factor(1);
    nke.set_killer_type(src_role->get_role_type());
    nke.set_killer_id(src_role->get_role_id());
    nke.set_got_factor(1);
    send_client(nke);*/

    // 被某个人干掉广播给你兄弟伙报仇
    if (psr->get_role_type() == RTE_USER) {
        sys_msg_be_kill(psr);
    }

    // 在运镖过程中
    if (get_status().guard() > 0) {
        uint32 convoy_quality = 0;
        on_convoy_task_end(true, &convoy_quality);
        psr->on_rob_convoy(convoy_quality);
        if (convoy_quality == 6) {
            sys_msg_convoy(psr);
        }
    }

    std::map<uint64, scene_role*>& pets = get_pets();
    std::map<uint64, scene_role*>::iterator begin = pets.begin();
    std::map<uint64, scene_role*>::iterator end   = pets.end();
	client::multi_role_msg mrm;
    for ( ; begin != end; ++begin) {
        if (begin->second == NULL || begin->second->get_status().is_dead()) {
            continue;
        }

        begin->second->mutable_attr()->set_val(CUR_HP, 0);
        begin->second->mutable_status()->set_dead();
		begin->second->fill_role_broadcast_msg(*mrm.add_rm());
		if (begin->second->get_scene() != NULL){
			begin->second->get_scene()->screen_broadcast(*begin->second,mrm,false);
		}
        psr->on_kill(begin->second);
        begin->second->on_killed(psr);
    }

    return scene_role::on_killed(psr);
}

void scene_user::on_arena_result(bool win)
{
}

void scene_user::add_filed_score(uint32 score)
{
    get_player_role().get_role_statis_mgr().add_filed_score(score);
}

void scene_user::sub_filed_score(uint32 score)
{
    get_player_role().get_role_statis_mgr().sub_filed_score(score);
}

uint32 scene_user::get_filed_score()
{
    return get_player_role().get_role_statis_mgr().get_filed_score();
}

void scene_user::set_offline_time(uint32 t)
{
	mutable_attr()->set_val(OFFLINE_TIME, t);
}

void scene_user::set_skill_point_time(uint32 t)
{
    get_player_role().set_skill_point_time(t);
}

bool scene_user::on_watch(scene_role* sr)
{
    std::map<uint64, scene_role*>::iterator i = _pets.begin();
    for ( ; i != _pets.end(); ++i) {
        i->second->on_watch(sr);
    }

    return true;
}

void scene_user::on_dead(uint32 tick)
{
    scene_role::on_dead(tick);
}

int scene_user::on_picked(scene_npc* n)
{
    if (n == NULL) {
        return -1;
    }

    get_scene()->on_pick_npc(this, n);

	std::vector<client::props_info> pis;
    uint32 ret = get_scene()->get_scene().single_npc_lose(n->get_origin_npc(), pis);
	if(ret > 0)
	{
		add_props(pis);
	}
    return 0;
}

int scene_user::on_kill(scene_role* psr)
{
    if (psr == NULL) {
        return -1;
    }

    add_anger(1);

    switch (psr->get_role_type()) {
    case RTE_USER: {
        break;
                   }
    case RTE_NPC:
    case RTE_PET: {
        uint32 exp = cal_kill_npc_exp(static_cast<scene_npc*>(psr));
        add_exp(exp);

        if (get_team_id() != 0 && get_scene() != NULL) {
            get_scene()->team_add_exp(this, exp);
        }

        break;
                  }
    default: break;
    }

    return scene_role::on_kill(psr);
}

uint32 scene_user::cal_kill_npc_exp(scene_npc* npc)
{
    if (npc == NULL) {
        return 0;
    }

    const config_npc* pcn = npc->get_origin_npc().get_config_npc();
    if (pcn == NULL) {
        return 0;
    }

    int lv     = this->get_attr().get_val(LEVEL);
    int npc_lv = pcn->get_monster_level();
    int ret    = abs(lv - npc_lv);
    uint32 exp = pcn->get_experience();

    if (ret > 5 && ret < 10) {
        return exp / 2;
    } else if (ret <= 5) {
        return exp;
    }

    return 1;
}

void scene_user::save_user()
{
    save_user_attr();
    save_user_status();
    mutable_buffer()->save_buffer();
}

void scene_user::reset_user_weapon()
{
	if(get_store_weapon() != 0 && get_attr().get_val(WEAPON_ID) != 0)
	{
		mutable_attr()->set_val(WEAPON_ID, get_store_weapon());
		set_store_weapon(0);
	}
}

void scene_user::save_user_attr()
{
    uint32 count = get_attr().field_count();
    mysql::tb_player_role* tpr = get_player_role().get_db_player_role();
    const google::protobuf::Reflection* ref = tpr->GetReflection();
    const google::protobuf::Descriptor* des = tpr->GetDescriptor();
    if (ref == NULL || des == NULL) {
        return ;
    }

#ifdef __INNER_DEBUG__PACKAGE_
    if (tpr->level() < get_attr().level()) {
        amtsvr_log("level error\n");
        //assert(false);
    }
#endif // __INNER_DEBUG__PACKAGE_

    uint32 index = 0;
    for (uint32 i = 0; i < count; ++i) {
        index = i + ATTR_CODE_TO_DB;
        switch (des->field(index)->type()) {
        case google::protobuf::FieldDescriptor::TYPE_UINT32: {
            ref->SetUInt32(tpr, des->field(index), get_attr().get_val((ROLE_ATTRIBUTE)i));
            break;
                                                             }
        case google::protobuf::FieldDescriptor::TYPE_UINT64: {
            ref->SetUInt64(tpr, des->field(index), get_attr().get_val_64((ROLE_ATTRIBUTE)i));
            break;
                                                             }
        default: break;
        }
    }
}

void scene_user::save_user_status()
{
    if (get_scene() == NULL) {
        return;
    }

    player_role_status& prs = _spr->get_role_status();
    if (get_scene()->get_scene_type() != MAP_TYPE_NORMAL) {
        prs.set_pos(_save_map_id, _save_scene_id, _save_pos.x, _save_pos.y, _save_pos.z);
        prs.set_tmp_pos(get_scene()->get_map_id(), get_scene()->get_scene_id(), _scene_pos.x, _scene_pos.y, _scene_pos.z, get_scene()->get_inst_id());
    } else {
        prs.set_pos(get_scene()->get_map_id(), get_scene()->get_scene_id(), _scene_pos.x, _scene_pos.y, _scene_pos.z);
        prs.set_tmp_pos(0, 0, 0, 0, 0, 0);
    }
}

void scene_user::calculate_attr()
{
    scene_role::calculate_attr();

    uint32 fight_power = calculate_fight_power();
    update_power(fight_power);
	modify_family_power(fight_power);
    if (fight_power != get_attr().get_val(FIGHTING_POWER)) {
        mutable_attr()->set_val(FIGHTING_POWER, fight_power);
        update_power_rank();
    }
}

void scene_user::calculate_status()
{
    scene_role::calculate_status();
}

uint32 scene_user::calculate_fight_power()
{
    uint32 fight_power = (uint32)(
        get_attr().get_val(MAX_HP) * 0.12f +
        get_attr().get_val(ATTACK_MAX) * 3 +
        get_attr().get_val(DEFENSE_MAX) * 4.5f +
        get_attr().get_val(HIT_POINT) * 2 +
        get_attr().get_val(DODGE) * 2 +
        get_attr().get_val(RECOVER) * 0.6 +
        get_attr().get_val(STABILITY) * 10 +
        get_attr().get_val(ATTACK_SPEED) * 50 +
        get_attr().get_val(MOVE_SPEED) * 30);
    return fight_power;
}

int scene_user::modify_family_power(uint32 power)
{
    uint32 family_id = _attr.get_val(FAMILY_ID);
    if (family_id == 0) {
        return 0;
    }

	//uint32 power = get_attr().get_val(FIGHTING_POWER);

    client::modify_family_power req;
    req.set_role_id(get_role_id());
    req.set_family_id(family_id);
    req.set_new_power(power);

    return send_center(req);
}

void scene_user::reset_pack_size(uint32 pack_size,uint32 depot_size)
{
    get_player_role().get_role_props().resize(pack_size, depot_size);
    mutable_attr()->set_backpack_size(pack_size);
    mutable_attr()->set_warehouse_size(depot_size);
    send_client_change_attr();
}

bool scene_user::reflesh_shop()
{
    player_role_black_shop & player_shop = get_player_role().get_player_role_black_shop();
    uint32 base_denominator = 0;
    uint32 player_level = mutable_attr()->get_val(LEVEL);
    player_role_treasure& treasure_obj = get_player_role().get_role_treasures();
    const std::map<uint32, config_treasure*>& config_treasure_map_ = CONFMGR->get_config_treasure_mgr().get_config_treasure_map();
    std::map<uint32, config_treasure*>::const_iterator itr = config_treasure_map_.begin();
    std::map<uint32,client::shop_info> new_black_shop_map_;
    new_black_shop_map_.clear();
    for (;itr != config_treasure_map_.end(); ++itr){
        // 检测次数是否达到限制，算出分母
        client::treasure_db_info tr_info;
        if (!treasure_obj.get_treasure_info(itr->second->get_item_idx(),tr_info)){
            treasure_obj.create_treasure_info(itr->second->get_item_idx(),tr_info);
        }

        if (player_level >= itr->second->get_level_min() && player_level <= itr->second->get_level_max()){
            if (tr_info.item_count() < itr->second->get_amount_max_today()){
                base_denominator += itr->second->get_probability();
                uint32 item_count = mtrandom::rand_mid32(itr->second->get_min_quantity(),itr->second->get_max_quantity());
                client::shop_info info;
                info.set_item_id(itr->second->get_item_idx());
                info.set_item_count(item_count);
                info.set_probability(itr->second->get_probability());
                info.set_key_id(itr->second->get_key_id());
                new_black_shop_map_.insert(std::pair<uint32, client::shop_info>(info.item_id(),info));
            }
        }
    }

    return player_shop.reflesh_shop(base_denominator,new_black_shop_map_);
}

void scene_user::update_mount_info(uint32 mount_level,uint32 mount_power)
{
#ifndef __LIB_MINI_SERVER__
    update_online_smy_info(client::SMY_MOUNT_DATA, mount_level, mount_power);
#endif // __LIB_MINI_SERVER__
    //	client::update_smy_mount_data update_msg;
    //
    //	update_msg.set_role_id(get_role_id());
    //	update_msg.set_mount_level(mount_level);
    //	update_msg.set_mount_power(mount_power);
    //	send_center(update_msg);
}

int scene_user::update_power(uint32 fight_power)
{
    //uint32 family_id = attr.get_val(FAMILY_ID);
    //if (family_id == 0) {
    //	return 0;
    //}

    //	client::update_smy_role_fight_power update_msg;
    //	update_msg.set_role_id(get_role_id());
    //	update_msg.set_fight_power(fight_power);
    //	send_center(update_msg);
#ifndef __LIB_MINI_SERVER__
    update_online_smy_info(client::SMY_ROLE_FIGHT_POWER, fight_power);
    get_player_role().update_role_levels_info(get_role_id(), get_attr().get_val(LEVEL), fight_power);
#endif // __LIB_MINI_SERVER__

    return 0;
}
#ifdef __LIB_MINI_SERVER__
void scene_user::add_equip(uint32 role_id, std::vector<client::props_info>& pis)
{
    props_parser& equip_pack = this->get_player_role().get_role_props().get_equip();
    equip_pack.set_props(pis);

    scene_role_attr sra;
    for (size_t i = 0; i < pis.size(); ++i) {
        for (int32 j = 0; j < pis[i].attr_size(); ++j) {
            sra.set_val(ATTR_CFG_TO_CODE[pis[i].attr(j).type()], pis[i].attr(j).value());
        }
    }

    get_player_role().get_player_role_ext_attr_mgr().set_ext_attr(role_id, REASTE_MINI_SERVER_EQUIP, sra);
}
#endif // __LIB_MINI_SERVER__

bool scene_user::add_props(std::vector<client::props_info>& pis,bool is_send)
{
    // 这里需要区分不同的物品类型放到不同的背包中,检测背包格子
    for (size_t i = 0; i< pis.size(); ++i){
        const config_props* cg = CONFMGR->get_config_props(pis[i].props_id());
        if (cg == NULL) {
            return false;
        }

        if (!get_player_role().get_role_props().is_can_add_prop()){
            return false;
        }

    }

	super_add_props(pis,is_send);

    return true;
}

void scene_user::super_add_props(std::vector<client::props_info>& pis,bool is_send)
{
#ifndef __LIB_MINI_SERVER__
	vector<client::props_info> pack_item;
	vector<client::props_info> depot_item;
	pack_item.clear();
	depot_item.clear();
	bool ad_attr = false; 
	// 分类
	for (size_t j = 0; j< pis.size(); ++j){
		const config_props* base_info = CONFMGR->get_config_props(pis[j].props_id());
		if (base_info == NULL) {
			return;
		}

		switch (base_info->get_item_type()) {
		case PT_MONEY:
			{
				ROLE_ATTRIBUTE pay_type;
				bool is_item = false;
				get_price(pis[j].props_id(),pay_type,is_item);
				if (!is_item){
					add_money(pay_type, pis[j].count()); 
					ad_attr = true;
				}
			}
			break;
		case PT_EQUIP:
		case PT_PET_EQUIP:
			depot_item.push_back(pis[j]);
			break;
		default:
			pack_item.push_back(pis[j]);
			break;
		}
	}

	uint32 pack_count = 0;
	uint32 depot_count = 0;
	props_parser& gp = _spr->get_role_props().get_pack();
	props_parser& detop = _spr->get_role_props().get_depot();
	bool is_change = false;
	if (pack_item.size() != 0){
		vector<client::props_info> vec_explore_1;
		if (gp.add_props(pack_item,pack_count,vec_explore_1) >= 0){
			is_change = true;
		}

		if (vec_explore_1.size() != 0){
			_spr->get_role_props().add_prop_to_explore(vec_explore_1);
		}
	}

	if (depot_item.size() != 0){
		vector<client::props_info> vec_explore_2;
		if (detop.add_props(depot_item,depot_count,vec_explore_2) >= 0){
			is_change = true;
		}

		if (vec_explore_2.size() != 0){
			_spr->get_role_props().add_prop_to_explore(vec_explore_2);
		}
	}

	if (is_send){
		if (is_change){
			send_client_change_props_msg();
		}
	}

	if (ad_attr){
		send_client_change_attr();
	}
#endif // __LIB_MINI_SERVER__
}

uint32 scene_user::get_free_space()
{
    return get_player_role().get_role_props().get_depot().get_free_space();
}

bool scene_user::check_pack_space(uint32 props_id)
{
    const config_props* cg = CONFMGR->get_config_props(props_id);
    if (cg == NULL) {
        return false;
    }

    if (!get_player_role().get_role_props().is_can_add_prop()){
        return false;
    }

    return true;
}

int scene_user::set_money(ROLE_ATTRIBUTE type, uint32 num)
{
#ifdef __INNER_DEBUG__PACKAGE_
    amtsvr_log("role_id:%u type:%u num:%u cur:%u\n", get_role_id(), type, num, get_attr().get_val(type));
#endif // __INNER_DEBUG__PACKAGE_
    mutable_attr()->set_val(type, num);
    return 0;
}

int scene_user::add_money(ROLE_ATTRIBUTE type, uint32 num)
{
#ifdef __INNER_DEBUG__PACKAGE_
    amtsvr_log("role_id:%u type:%u num:%u cur:%u\n", get_role_id(), type, num, get_attr().get_val(type));
#endif // __INNER_DEBUG__PACKAGE_
    mutable_attr()->add_val(type, num);
    return 0;
}

int scene_user::sub_money(ROLE_ATTRIBUTE type, uint32 num)
{
    int ret = 0;
    /*switch (type)
    {
    case TAEL:
    break;
    case INGOT:
    break;
    case GIFT_MONEY:
    break;
    case MILITARY_STRATEGY:
    break;
    case SIEGE_REPUTATION:
    break;
    case CROSS_SVR_WAR_REPUTATION:
    break;
    case MELTING:
    break;
    default:
    break;
    }*/

    uint32 val = get_attr().get_val(type);
    if (num > val) {
        ret = -1;
#ifdef __INNER_DEBUG__PACKAGE_
        amtsvr_log("error role_id:%u type:%u num:%u cur:%u\n", get_role_id(), type, num, get_attr().get_val(type));
#endif // __INNER_DEBUG__PACKAGE_
        return ret;
    }

#ifdef __INNER_DEBUG__PACKAGE_
    amtsvr_log("role_id:%u type:%u, num:%u cur:%u\n", get_role_id(), type, num, get_attr().get_val(type));
#endif // __INNER_DEBUG__PACKAGE_

    mutable_attr()->sub_val(type, num);

    return ret;
}

void scene_user::sub_money_log(uint32 root_id, uint32 type_id, uint32 old_value, uint32 new_value, uint32 goods_id/* = 0*/, uint32 num/* = 0*/)
{
    mysql::t_moneylog log;
    log.set_id(0);
    log.set_sid(get_player_role().sid());
    log.set_did("");
    log.set_accountname(get_player_role().account());
    log.set_playername(get_role_name());
    log.set_rootid(root_id);
    log.set_typeid_(type_id);
    log.set_oldvalue(old_value);
    log.set_newvalue(new_value);
    log.set_gold(old_value - new_value);
    log.set_goldticke(0);
    log.set_goodsid(goods_id);
    log.set_num(0);
    log.set_createdate((uint32)time(NULL));
    log.set_roleid(get_role_id());
    send_log(log);
}

#ifndef __LIB_MINI_SERVER__
void scene_user::update_online_smy_info(client::UPDATE_TYPE_E type, uint32 val1, uint32 val2 /* = 0 */ )
{
    client::update_smy_info usi;
    usi.set_type(type);
    usi.set_role_id(get_role_id());
    usi.set_val1(val1);
    if(val2 != 0)
    {
        usi.set_val2(val2);
    }
    send_center(usi);
}
#endif // __LIB_MINI_SERVER__

int scene_user::on_level_up(uint32 old_level, uint32 now_level)
{
    set_level(now_level);

    on_level_up_skill(old_level, now_level);
    on_level_up_attr(old_level, now_level);
    on_level_up_rank(old_level, now_level);
    on_level_up_open(old_level, now_level);
	on_level_up_gem(now_level);
	check_bow_open();

	CHECK_TARGET_PROGRESS(this, ROLE_UPGRADE, now_level);
#ifndef __LIB_MINI_SERVER__
    update_online_smy_info(client::SMY_ROLE_LEVEL, now_level);
    get_player_role().update_role_levels_info(get_role_id(), now_level, get_attr().get_val(FIGHTING_POWER));
#endif // __LIB_MINI_SERVER__
    mysql::t_upgradelog log;
    log.set_id(0);
    log.set_sid(get_player_role().sid());
    log.set_did("");
    log.set_accountname(get_player_role().account());
    log.set_playername(get_role_name());
    log.set_oldlevel(old_level);
    log.set_newlevel(now_level);
    log.set_sucess(1);
    log.set_upgradetype(1);
    log.set_params("");
    log.set_createdate((uint32)time(NULL));
    send_log(log);

    return 0;
}

int scene_user::on_level_up_skill(uint32 old_level, uint32 now_level)
{
    config_skill_mgr* pcsm = MUTABLE_CONFIG_MGR(config_skill_mgr);
    if (pcsm == NULL) {
        return -1;
    }

    std::vector<const config_skill*> vcs;
    pcsm->get_config_skill_by_job(get_attr().profession(), now_level, vcs);
    for (size_t i = 0; i < vcs.size(); ++i) {
        if (vcs[i] == NULL) {
            continue;
        }

        get_player_role().get_role_skill_mgr().add_skill(get_role_id(), vcs[i]->get_skill_id());

        skill new_skill(*vcs[i]);
        mutable_skill()->add_skill(new_skill);
    }

    if (!vcs.empty()) {
        client::role_skill_msg rsm;
        fill_role_skill_msg(rsm);
        send_client(rsm);
    }

    return 0;
}

int scene_user::on_level_up_vip(mysql::tb_player_role& tpr, uint32 old_level, uint32 now_level)
{
    return 0;
}

int scene_user::on_level_up_rank(uint32 old_level, uint32 now_level)
{
    // 更新排行榜数据
    update_lv_rank();

    return 0;
}

int scene_user::on_level_up_attr(uint32 old_level, uint32 now_level)
{
    // 升级属性
    calculate_attr();

    // 升级满血满气
    mutable_attr()->set_val(CUR_HP, get_attr().get_val(MAX_HP));

    return 0;
}

int scene_user::on_level_up_task(mysql::tb_player_role& tpr, uint32 old_level, uint32 now_level)
{
    for (uint32 i = old_level; i < now_level; ++i) {
        // task_operator.on_level_up(i + 1);
    }

    // task_operator.send_role_opt_tmsg();

    return 0;
}

bool scene_user::has_func_open(uint32 func_id)
{
    const std::map<uint32, client::open_info>& opens = get_player_role().get_tasks().get_opens();
	return opens.find(func_id) != opens.end();
}

int scene_user::on_level_up_open(uint32 old_level, uint32 now_level)
{
    const config_open_mgr* mgr = GET_CONFIG_MGR(config_open_mgr);
    if (mgr == NULL) {
        return -1;
    }

    std::map<uint32, client::open_info>* opens = this->get_player_role().get_tasks().mutable_opens();
    const std::map<uint32, config_open*>& cfg = mgr->get_cfg();
    std::map<uint32, config_open*>::const_iterator begin = cfg.begin();
    std::map<uint32, config_open*>::const_iterator end = cfg.end();
    for (; begin != end; ++begin) {
        const config_open* pco = begin->second;
        if (pco == NULL || pco->get_open_type() != config_open::OPEN_TYPE_LEVEL) {
            continue;
        }
        if (now_level == (uint32)pco->get_open_condition()) {
            (*opens)[pco->get_func_id()].set_id(pco->get_func_id());
			switch(pco->get_func_id())
			{
				case LEVEL_FUNC_ID:
					get_player_role().init_role_levels();
					break;
#ifndef __LIB_MINI_SERVER__
				case CYCLE_FUNC_ID:
					get_player_role().init_role_cycle();
					break;
				case DEVIL_FUNC_ID:
				{
					get_player_role().init_role_devil();
					client::devil_stage_list list;
					get_player_role().get_role_devil().fill_devil_raids(list);
					send_client(list);
				}
					break;
#endif
				default:
					break;
			}
            client::open_msg om;
            om.set_t(client::open_msg_type_add);
            om.add_id()->CopyFrom((*opens)[pco->get_func_id()]);
            this->send_client(om);
        }
    }

    return 0;
}

int scene_user::on_level_up_gem(uint32 now_level)
{
	uint32 hole_count = CONFMGR->get_config_gem_act_mgr().get_config_gem_act(2,now_level);
	for (uint32 i = EQUIP_GRID_BEG; i <=  EQUIP_SHOE; ++i){
		get_player_role().get_player_role_gems().create_gem_info(i,hole_count);
	}
	
	return 0;
}

int scene_user::check_bow_open()
{
	const config_open_mgr* mgr = GET_CONFIG_MGR(config_open_mgr);
	const config_open* bow_info = mgr->get_config_open(19);
	if (bow_info != NULL){
		if (get_attr(LEVEL)>= static_cast<uint32>(bow_info->get_open_condition())){
			const mysql::tb_role_bows& bows = get_player_role().get_player_role_bows().get_db();

			if (bows.bow_id() == 0) {
				mysql::tb_role_bows* pm = get_player_role().get_player_role_bows().mutable_db();
				pm->set_role_id(get_role_id());
				pm->set_bow_id(1);
				pm->set_advance_num(0);
				pm->set_advance_time(0);
				pm->set_advance_bless(0);
				pm->set_bow_level(1);
				pm->set_bow_exp(0);
				uint32 bows_power = static_cast<uint32>(ceil(GET_CONFIG_MGR(config_bows_mgr)->calculate_bow_power(pm->bow_id(),pm->bow_level())));
				pm->set_bow_power(bows_power);

				client::bows_msg mmsg;
				client::bows_info* pmi = mmsg.add_mi();
				pmi->set_bow_id(bows.bow_id());
				pmi->set_advance_time(bows.advance_time());
				pmi->set_advance_bless(bows.advance_bless());
				pmi->set_bow_level(bows.bow_level());
				pmi->set_bow_exp(bows.bow_exp());
				pmi->set_bow_power(bows.bow_power());
				send_client(mmsg);
			}
		}
	}

	return 0;
}

int scene_user::add_exp(uint64 exp)
{
    // TODO: 修正老号系数为0问题 测试用
    if (get_attr().get_val(EXP_COEFFICIENT) < TEN_THOUSAND) {
        //amtsvr_log("exp_coefficient:%u fix:10000\n", get_attr().get_val(EXP_COEFFICIENT));
        mutable_attr()->set_val(EXP_COEFFICIENT, TEN_THOUSAND);
        get_player_role().get_db_player_role()->set_exp_coefficient(TEN_THOUSAND);
    }

    uint64 last_exp = static_cast<uint64>(exp * (get_attr().get_val(EXP_COEFFICIENT) / 10000.0f));
    add_normal_exp(last_exp);

    return 0;
}

uint32 scene_user::add_family_exp(uint32 origin_exp)
{
	// 军团等级加成副本经验
	if (get_attr().get_val(FAMILY_LEVEL) != 0){
		const config_legion* legion_info = CONFMGR->get_config_legion_mgr().get_config_legion(get_attr().get_val(FAMILY_LEVEL));
		if (legion_info != NULL){
			return (uint32)(ceil(origin_exp * legion_info->get_add_experience() / 100.0f));
		}
	}

	return 0;
}

int scene_user::add_normal_exp(uint64 exp)
{
#ifdef __LIB_MINI_SERVER__
    return 0;
#else

#ifdef __INNER_DEBUG__PACKAGE_
    amtsvr_log("role_id:%u add_exp:%lu\n", get_role_id(), exp);
#endif // __INNER_DEBUG__PACKAGE_

    add_experience(exp);

    while (true) {
        const config_upgrade* pcu = CONFMGR->get_config_upgrade(get_job(), get_level());
        if (pcu == NULL || pcu->get_need_exp() == 0 || pcu->get_need_exp() > get_experience()) {
            send_client_change_attr();
            return 0;
        }

        sub_experience(pcu->get_need_exp());
        on_level_up(get_level(), get_level() + 1);

        client::multi_role_msg mrm;
        fill_user_msg(*mrm.add_rm());
        get_scene()->screen_broadcast(*this, mrm, false);
    }

    return 0;
#endif // __LIB_MINI_SERVER__
}

void scene_user::add_anger(uint32 anger)
{
    //     static const uint32 MAX_ANGER = 20;
    uint32 MAX_ANGER = get_skill().get_max_anger();
    mutable_attr()->add_val(ANGER, anger);
    if (get_attr().get_val(ANGER) > MAX_ANGER) {
        mutable_attr()->set_val(ANGER, MAX_ANGER);
    }
}

void scene_user::add_physical_power(uint32 num)
{
    mutable_attr()->set_val(PHYSICAL_POWER, get_attr().get_val(PHYSICAL_POWER) + num);
    send_client_change_attr();
}

int scene_user::recover_physical_power(uint32 tick)
{
    if (_physical_tick == 0) {
        _physical_tick = tick;
    }

    if (tick - _physical_tick > CONFMGR->get_config_basic_mgr().get_physical_tick()) {
        _physical_tick = tick;
    } else {
        return -1;
    }

    if (get_attr().get_val(PHYSICAL_POWER) < CONFMGR->get_config_basic_mgr().get_physical_max()) {
        mutable_attr()->set_val(PHYSICAL_POWER, get_attr().get_val(PHYSICAL_POWER) + CONFMGR->get_config_basic_mgr().get_physical_add());
        send_client_change_attr();
    }

    return 0;
}

int scene_user::attach_scene(scene_inst* s)
{
    assert(s != NULL);
    return scene_role::attach_scene(s);
}

int scene_user::detach_scene()
{
    set_scene_ready(false);
    return scene_role::detach_scene();
}

void scene_user::practice()
{
    _practice_time = (uint32)time(NULL);
    _comm_practice_time = 0;
    mutable_status()->set_val(client::RSE_STATUS_MEDITATE, true);
    mutable_status()->set_val(client::RSE_STATUS_COMM_PRACTICE, false);

    if (get_scene() != NULL) {
        get_scene()->screen_broadcast_modify_role_msg(*this, false);
    }
}

void scene_user::stop_practice()
{
    if (!get_status().meditate() && !get_status().comm_practice()) {
        return;
    }

    _practice_time      = 0;
    _comm_practice_time = 0;
    mutable_status()->set_val(client::RSE_STATUS_MEDITATE, false);
    mutable_status()->set_val(client::RSE_STATUS_COMM_PRACTICE, false);

    if (get_scene() != NULL) {
        get_scene()->screen_broadcast_modify_role_msg(*this, false);
    }
}

bool scene_user::can_comm_practice(bool* agree)
{
    bool ret = false;
    if (can_practice() && !get_status().comm_practice()) {
        ret = true;
    }

    if (agree != NULL) {
        if (ret && _auto_comm_practice) {
            *agree = true;
        } else {
            *agree = false;
        }
    }

    return ret;
}

bool scene_user::comm_practice(uint64 user_inst_id)
{
    _comm_practice_user = user_inst_id;
    _practice_time      = 0;
    _comm_practice_time = (uint32)time(NULL);
    mutable_status()->set_val(client::RSE_STATUS_MEDITATE, false);
    mutable_status()->set_val(client::RSE_STATUS_COMM_PRACTICE, true);

    if (get_scene() != NULL) {
        get_scene()->screen_broadcast_modify_role_msg(*this, false);
    }

    return true;
}

bool scene_user::can_get_comm_practice_list()
{
    uint32 now = (uint32)time(NULL);
    if (_refresh_practice_time + PRACTICE_LIST_INTERAL > now) {
        return false;
    }

    _refresh_practice_time = now;

    return true;
}

uint32 scene_user::get_props_num(uint32 prop_id)
{
    props_parser& pack_obj = get_player_role().get_role_props().get_pack();
    return pack_obj.get_props_num(prop_id);
}

bool scene_user::is_can_add_props()
{
    if (!get_player_role().get_role_props().is_can_add_prop()){
        return false;
    }

    return true;
}

bool scene_user::add_props(const client::props_info& pi,bool notify /* = true */)
{
#ifndef __LIB_MINI_SERVER__
    int ret = 1;
    const config_props* cg = CONFMGR->get_config_props(pi.props_id());
    if (cg == NULL) {
        return false;
    }

    if (!get_player_role().get_role_props().is_can_add_prop()){
        return false;
    }

    switch (cg->get_item_type()) {
    case PT_MONEY:
		{
			ROLE_ATTRIBUTE pay_type;
			bool is_item = false;
			get_price(pi.props_id(),pay_type,is_item);
			if (!is_item){
				ret = add_money(pay_type, pi.count()); 
				send_client_change_attr();
				return true;
			}
		}
		break;
    case PT_EQUIP:
    case PT_PET_EQUIP:
        ret = get_player_role().get_role_props().get_depot().add_props(pi);
        break;
    default:
        ret = get_player_role().get_role_props().get_pack().add_props(pi);
        break;
    }

    if (ret == 0 && notify) {
        send_client_change_props_msg();
    }

    return ret >= 0;
#else
    return false;
#endif // __LIB_MINI_SERVER__
}

int scene_user::add_props(const vector<uint32> &v, client::raid_result_info *rri)
{
	for(size_t i = 0; i < v.size(); ++i)
	{
		add_drop_props(v[i], rri);
	}
	return 0;
}

int scene_user::add_props(const vector<int> &v, client::raid_result_info *rri)
{
	for(size_t i = 0; i < v.size(); ++i)
	{
		add_drop_props((uint32)v[i], rri);
	}
	return 0;
}

int scene_user::add_drop_props(uint32 drop_id, client::raid_result_info *rri, uint32 timeout)
{
	vector<client::props_info> v;
	make_props(drop_id, v, get_job(), timeout);
	super_add_props(v);

	for(size_t i = 0; i < v.size(); ++i){
		if(rri != NULL){
			client::props_info *pi = rri->add_props();
			pi->CopyFrom(v[i]);
		}
	}

	return 0;
}
// 这接口要手动发送物品改变协议
int scene_user::add_drop_props(uint32 drop_id,vector<client::props_info>& item_vec,uint32 time_out)
{
	make_props(drop_id, item_vec, get_job(), time_out);

	super_add_props(item_vec,false);

	return 0;
}

int scene_user::add_props(uint32 id, uint32 num, uint32 bind/* = 1*/, uint32 quality/* = 0*/, bool notify/* = true*/)
{
#ifndef __LIB_MINI_SERVER__
    const config_props* cg = CONFMGR->get_config_props(id);
    if (cg == NULL) {
        return -1;
    }


    if (!is_can_add_props()){
        return -1;
    }


    if (quality == 0) {
        quality = cg->get_name_color_index();
    }

    if (bind == 1){
        bind = cg->get_bind_type();
    }

    vector<client::props_info> vec_explore;
    vec_explore.clear();
    int ret = 0;
    switch (cg->get_item_type()) {
    case PT_MONEY:
		{
			ROLE_ATTRIBUTE pay_type;
			bool is_item = false;
			get_price(id,pay_type,is_item);
			if (!is_item){
				ret = add_money(pay_type,num); 
				send_client_change_attr();
				return ret;
			}
		}
		break;
        break;
    case PT_EQUIP:
    case PT_PET_EQUIP:
        amtsvr_log("role_id:%u add props:%u\n", get_role_id(), id);
        ret = get_player_role().get_role_props().get_depot().add_props(id, num, vec_explore,quality, bind);
        break;
    default:
        ret = get_player_role().get_role_props().get_pack().add_props(id, num, vec_explore,quality, bind);
        break;
    }

    if (vec_explore.size() != 0){
        get_player_role().get_role_props().add_prop_to_explore(vec_explore);
    }

    if (ret != 0 && notify) {
        send_client_change_props_msg();
    }

    return ret;
#else 
    return -1;
#endif // __LIB_MINI_SERVER__
}

int  scene_user::add_melting_prop(uint32 id, uint32 num)
{
    const config_props* cg = CONFMGR->get_config_props(id);
    if (cg == NULL) {
        return -1;
    }

    uint32 quality = cg->get_quality();
    uint32 bind = cg->get_bind_type();

    vector<client::props_info> vec_explore;
    vec_explore.clear();
    int ret = 0;
    switch (cg->get_item_type()) {
    case PT_MONEY:
        ret = add_money(TAEL, num); send_client_change_attr();
        return ret;
        break;
    case PT_EQUIP:
    case PT_PET_EQUIP:
        amtsvr_log("role_id:%u add props:%u\n", get_role_id(), id);
        ret = get_player_role().get_role_props().get_depot().add_props(id, num, vec_explore,quality, bind);
        break;
    default:
        ret = get_player_role().get_role_props().get_pack().add_props(id, num, vec_explore,quality, bind);
        break;
    }

    if (vec_explore.size() != 0){
        get_player_role().get_role_props().add_prop_to_explore(vec_explore);
    }

    if (ret != 0) {
        send_client_change_props_msg();
    }

    return ret;
}

int scene_user::sub_props(uint32 id, uint32 num)
{
    const config_props* cg = CONFMGR->get_config_props(id);
    if (cg == NULL) {
        return -1;
    }

    int ret = 0;
    switch (cg->get_item_type()) {
    case PT_PET_EQUIP:
    case PT_EQUIP:
        ret = get_player_role().get_role_props().get_depot().remove_props(id, num);
        break;
    default:
        ret = get_player_role().get_role_props().get_pack().remove_props(id, num);
        break;
    }

    if (ret == 0) {
        send_client_change_props_msg();
    }

    return ret;
}

bool scene_user::sub_props(std::map<uint32 ,uint32>& expend_map_)
{
	if (expend_map_.size() != 0){
		props_parser& pack_obj = get_player_role().get_role_props().get_pack();
		std::map<uint32 ,uint32>::iterator it = expend_map_.begin();
		uint32 prop_id = 0;
		for (;it != expend_map_.end(); ++it){
			pack_obj.remove_props(it->first,it->second,prop_id,NULL);
		}
	}

	return true;
}
void cal_expend(uint32 id,uint32 count,std::map<uint32,uint32>& total_expend)
{
	std::map<uint32,uint32>::iterator itr = total_expend.find(id);
	if (itr != total_expend.end()){
		itr->second += count;
	}else{
		total_expend.insert(pair<uint32,uint32>(id,count));
	}
}
bool scene_user::sub_super_props(const std::vector<int>& sub_item_id,uint32 need_count,std::map<uint32,uint32>& total_expend)
{
	std::map<uint32,uint32> expend_map_;
	expend_map_.clear();
	// 根据规则算好每个格子的物品扣除顺序与数量：
	props_parser& pack_obj = get_player_role().get_role_props().get_pack();
	pack_obj.compare_props_time(need_count,sub_item_id,expend_map_);
	// 限时的：扣除哪一个格子多少数量
	sub_props(expend_map_);
	std::map<uint32,uint32>::iterator it = expend_map_.begin();
	for (; it != expend_map_.end();++it){
		cal_expend(it->first,it->second,total_expend);
	}
	// 扣完限时的再扣普通的！
	uint32 cur_count = 0;
	for (uint32 j = 0; j < sub_item_id.size(); ++j){
		cur_count = pack_obj.get_props_num(sub_item_id[j]);
		if (cur_count == 0){
			continue;
		}

		if (cur_count >= need_count){
			if (pack_obj.remove_props(static_cast<uint32>(sub_item_id[j]),need_count) == 0){
				cal_expend(static_cast<uint32>(sub_item_id[j]),need_count,total_expend);
				break;
			}

		}else{
			pack_obj.remove_props(static_cast<uint32>(sub_item_id[j]),cur_count);
			cal_expend(static_cast<uint32>(sub_item_id[j]),cur_count,total_expend);
			need_count -= cur_count;
		}
	}

	return true;
}

bool scene_user::calc_grindex_sub_count(uint32& need_count,const std::vector<int>& sub_item_id,std::map<uint32 ,uint32>& expend_map_)
{
	//props_parser& pack_obj = get_player_role().get_role_props().get_pack();
	//处理消耗序列排序：
	//uint32 cur_count = 0;
	//for (uint32 j = 0; j < need_item_vec.size(); ++j){
	//	cur_count = pack_obj.get_props_num(need_item_vec[j]);
	//	if (cur_count >= need_count){
	//		if (pack_obj.remove_props(static_cast<uint32>(need_item_vec[j]),need_count) == 0){
	//			is_enough_item = true;
	//			break;
	//		}

	//	}else{
	//		pack_obj.remove_props(static_cast<uint32>(need_item_vec[j]),cur_count);
	//		need_count -= cur_count;
	//	}
	//}

	//pack_obj.compare_props_time(need_count,sub_item_id,expend_map_);

	return true;
}

int scene_user::on_attacked(scene_role* psr, skill& sk, effect_result& atk_rsp, bool& trigger)
{
    if (atk_rsp.fd_size() > 0) {
        uint32 damage = atk_rsp.fd(atk_rsp.fd_size() - 1).has_data() ? atk_rsp.fd(atk_rsp.fd_size() - 1).data() : 0;
        add_hate_role(psr->get_inst_id(), sk.get_hate_val(), damage, get_scene_pos().get_distance(psr->get_scene_pos()));
		add_counter_role(psr, sk.get_hate_val());
    }

    int ret = scene_role::on_attacked(psr, sk, atk_rsp, trigger);
    if (ret != 0) {
        return ret;
    }

    if (!is_same_role(*psr) && psr->get_role_type() == RTE_USER) {
        scene_user* su = static_cast<scene_user*>(psr);
        justifiable_defense(su);
    }

    screen_broadcast_change_buf();

    return ret;
}

void scene_user::justifiable_defense(scene_user* su)
{
    if (su->in_justifiable_defense(this)) {
        return ;
    }

    _defenses[su->get_role_id()] = 0;
}

bool scene_user::in_justifiable_defense(scene_role* sr)
{
    std::map<uint32, uint32>::iterator i = _defenses.find(sr->get_role_id());
    return i != _defenses.end();
}

bool scene_user::on_update_pet_fight_status()
{
    // 如果不是主城则不复活
    player_role_pet& player_pet = get_player_role().get_pets();
    client::pet_db_info pet_info;
    const std::map<uint32, client::pet_db_info>& pet_map_ = player_pet.get_all_pet_db_info();
    std::map<uint32, client::pet_db_info>::const_iterator itr = pet_map_.begin();
    for (;itr != pet_map_.end();++itr){
        //if (itr->second.status() != PET_STATUS_DEAD){
        //	continue;
        //}
        if (itr->second.status() != PET_STATUS_FIGHTING){
            continue;
        }

        const config_beauty* beauty_info = CONFMGR->get_config_beauty_mgr().get_config_beauty(itr->first,itr->second.pet_level());
        if (beauty_info == NULL){
            continue;
        }

		call_pet(itr->first,player_pet);

        break;
    }

    return true;
}

bool scene_user::change_pre_pet_attr()
{
	player_role_pet& player_pet = get_player_role().get_pets();
	client::pet_db_info pet_info;
	const std::map<uint32, client::pet_db_info>& pet_map_ = player_pet.get_all_pet_db_info();
	std::map<uint32, client::pet_db_info>::const_iterator itr = pet_map_.begin();
	for (;itr != pet_map_.end();++itr){
		if (itr->second.status() == PET_STATUS_FIGHTING){
			continue;
		}

		const config_beauty* beauty_info = CONFMGR->get_config_beauty_mgr().get_config_beauty(itr->first,itr->second.pet_level());
		if (beauty_info == NULL){
			continue;
		}

		client::pet_db_info* mut_info = player_pet.mutable_pet_info(itr->first);
		if (mut_info != NULL){
			mut_info->set_cur_hp(0);
		}
	}

	return true;
}

bool scene_user::on_update_skill_point()
{
    // 检测技能点恢复
    if (get_role_type() == RTE_USER) {
        uint32 cur_time = (uint32)time(NULL);
        uint32 skil_point_max = static_cast<uint32>(CONFMGR->get_config_basic_mgr().get_basic_value(10086));
        uint32 cur_skill_point = mutable_attr()->get_val(ROLE_SKILL_POINT);
        if (cur_skill_point < skil_point_max){
            uint32 interval_time = 0;
            uint32 add_skil_point = 0;
            uint32 remain_time = 0; // 客户端倒计时使用的时间

            interval_time = static_cast<uint32>(CONFMGR->get_config_basic_mgr().get_basic_value(10085));
            uint32 difference_time = cur_time - mutable_attr()->skill_point_time();
            if (difference_time >= interval_time/* || cur_skill_point_time == 0*/) {
                // 取商值
                add_skil_point = static_cast<uint32>(difference_time / interval_time);
                // 取剩下时间 = 配置间隔时间 - 取出余数
                remain_time = interval_time - static_cast<uint32>(difference_time % interval_time);

                mutable_attr()->set_val(ROLE_SKILL_POINT_TIME,cur_time);
                uint32 last_skill_point = cur_skill_point + add_skil_point;

                if (last_skill_point >= skil_point_max){
                    last_skill_point = skil_point_max;
                    remain_time = 0;
                }

                // 技能点满了~就不发剩余时间给前端，前端根据点数满做显示
                if (last_skill_point != skil_point_max){
                    client::skill_point_time_rsp rsp_msg;
                    rsp_msg.set_remain_time(remain_time);
                    send_client(rsp_msg);
                }

                mutable_attr()->set_val(ROLE_SKILL_POINT,last_skill_point);
                send_client_change_attr();
            }
        }
    }

    return true;
}

bool scene_user::on_update_mount_bless_time()
{
	player_role_mounts& m = get_player_role().get_mounts();
	mysql::tb_role_mounts* pdb = m.mutable_db();
	if (pdb != NULL && pdb->mount_id() != 0) {
		if (pdb->advance_bless() != 0){
			const config_mounts* mount_info = CONFMGR->get_config_mounts(pdb->mount_id());
			if (mount_info != NULL){
				uint32 clear_time = mount_info->get_base()->get_advance_bless_time();
				if (clear_time != 0){
					uint32 valid_time = pdb->advance_time() + clear_time;
					uint32 system_time = (uint32)time(NULL);
					// 时间到了
					if (valid_time <= system_time) {
						pdb->set_advance_bless(0);
						pdb->set_advance_num(0);
						const mysql::tb_role_mounts& mounts = get_player_role().get_mounts().get_db();
						client::mounts_msg mmsg;
#ifndef __LIB_MINI_SERVER__
						fill_mounts_msg(mounts, mmsg);
						send_client(mmsg);
#endif // __LIB_MINI_SERVER__
					}
				}
			}
		}
	}

	return true;
}
void fill_bows_info(const mysql::tb_role_bows& bows, client::bows_info& pmi);
bool scene_user::on_update_bow_bless_time()
{
	player_role_bows& m = get_player_role().get_player_role_bows();
	mysql::tb_role_bows* pdb = m.mutable_db();
	if (pdb != NULL && pdb->bow_id() != 0) {
		if (pdb->advance_bless() != 0){
			const config_bows* bow_info = CONFMGR->get_config_bows(pdb->bow_id());
			if (bow_info != NULL){
				uint32 clear_time = bow_info->get_base()->get_advance_bless_time();
				if (clear_time != 0){
					uint32 valid_time = pdb->advance_time() + clear_time;
					uint32 system_time = (uint32)time(NULL);
					// 时间到了
					if (valid_time <= system_time) {
						pdb->set_advance_bless(0);
						pdb->set_advance_num(0);
						const mysql::tb_role_bows& bows = get_player_role().get_player_role_bows().get_db();
						client::bows_msg mmsg;
#ifndef __LIB_MINI_SERVER__
						fill_bows_info(bows, *(mmsg.add_mi()));
						send_client(mmsg);
#endif // __LIB_MINI_SERVER__
					}
				}
			}
		}
	}

	return true;
}

bool scene_user::on_update_props_date()
{
	player_role_props& player_prop = get_player_role().get_role_props();

	player_prop.get_pack().check_props_time();
	//player_prop.get_equip().check_props_time();

	return true;
}

bool scene_user::on_update_black_shop()
{
#ifndef __LIB_MINI_SERVER__
    uint32 is_frist_reflesh = 0;
    get_player_role().get_role_statis_mgr().get(PRSTE_BLACK_FRIST_REFRESH)->get_val(is_frist_reflesh);
    if (is_frist_reflesh == 1){
        // 已经打开过的就根据时间给他刷新！
        if (get_player_role().get_role_cds().is_can_operate_by_cd(CD_COOLDOW_TYPE_ID_BLACK_SHOP)){
            client::black_shop_reflesh_rsp rsp_msg;
            //刷新
            reflesh_shop();
            // 更新刷新时间
            get_player_role().get_role_cds().add_cooldown_by_id(CD_COOLDOW_TYPE_ID_BLACK_SHOP);
            const std::map<uint32, client::black_shop_db_info>& shop_list = get_player_role().get_player_role_black_shop().get_black_shop_item_list();
            std::map<uint32, client::black_shop_db_info>::const_iterator it = shop_list.begin();
            for (;it != shop_list.end(); ++it){
                client::black_shop_db_info* info = rsp_msg.add_info();
                info->CopyFrom(it->second);
            }
            uint32 cur_time = (uint32)time(NULL);
            uint32 cd_time = get_player_role().get_role_cds().get_cooldown_time(CD_COOLDOW_TYPE_ID_BLACK_SHOP);
            uint32 remain_time = cd_time - cur_time;
            rsp_msg.set_time(remain_time);
            rsp_msg.set_result_type(BLACK_SHOP_RESULT_TYPE_REFLESH_SUCCESS);
            uint32 reflesh_count = 0;
            get_player_role().get_role_statis_mgr().get(PRSTE_BLACK_REFRESH_COUNT)->get_val(reflesh_count);
            rsp_msg.set_reflesh_count(reflesh_count);
            send_client(rsp_msg);
        }
    }
#endif
    return true;
}

void scene_user::calculate_offline_data()
{
    calculate_skill_point();
}

void scene_user::calculate_skill_point()
{
    // 离线处理
    uint32 cur_time = (uint32)time(NULL);
    uint32 skil_point_max = static_cast<uint32>(CONFMGR->get_config_basic_mgr().get_basic_value(10086));
    uint32 cur_skill_point = mutable_attr()->get_val(ROLE_SKILL_POINT);
    if (cur_skill_point < skil_point_max){
        uint32 interval_time = 0;
        uint32 add_skil_point = 0;
        uint32 remain_time = 0; // 客户端倒计时使用的时间

        interval_time = static_cast<uint32>(CONFMGR->get_config_basic_mgr().get_basic_value(10085));
        uint32 difference_time = cur_time - mutable_attr()->skill_point_time();
        if (difference_time >= interval_time/* || cur_skill_point_time == 0*/) {
            // 用时间计算所增加的技能点(离线处理)
            // 取商值
            add_skil_point = static_cast<uint32>(difference_time / interval_time);
            // 取剩下时间 = 配置间隔时间 - 取出余数
            remain_time = interval_time - static_cast<uint32>(difference_time % interval_time);

            mutable_attr()->set_val(ROLE_SKILL_POINT_TIME,cur_time);
            uint32 last_skill_point = cur_skill_point + add_skil_point;

            if (last_skill_point >= skil_point_max){
                last_skill_point = skil_point_max;
                remain_time = 0;
            }

            // 技能点满了~就不发剩余时间给前端，前端根据点数满做显示
            if (last_skill_point != skil_point_max){
                client::skill_point_time_rsp rsp_msg;
                rsp_msg.set_remain_time(remain_time);
                send_client(rsp_msg);
            }

            mutable_attr()->set_val(ROLE_SKILL_POINT,last_skill_point);
            send_client_change_attr();
        }
    }
}

uint32 scene_user::calculate_role_bow_speed(uint32 bow_id,uint32 bow_level)
{
	uint32 cur_speed = 0;
	const config_bows_attr* bows_attr = CONFMGR->get_config_bows_mgr().get_config_bows_attr(bow_id,bow_level);

	if (bows_attr != NULL){
		uint32 skill_count = get_bow_skill_count();
		uint32 base_value = CONFMGR->get_basic_value(10006);
		cur_speed = static_cast<uint32>(bows_attr->get_bow_speed()) - skill_count* base_value;
	}


	return cur_speed;
}

int scene_user::set_robot(const bool s)
{
    if (get_status().is_dead()) {
        return -1;
    }

    mutable_status()->set_robot(s);

    return 0;
}

int scene_user::set_riding(const client::role_status_riding_status s)
{
    if (s == get_status().riding()) {
        return -1;
    }

    switch (s) {
    case  client::role_status_riding_status_status_walk: {
        mutable_status()->set_val(client::RSE_STATUS_RIDING, s);
        break;
                                                         }
    case  client::role_status_riding_status_status_riding: {
        mutable_status()->set_val(client::RSE_STATUS_RIDING, s);
        break;
                                                           }
    case  client::role_status_riding_status_status_fly: {
        mutable_status()->set_val(client::RSE_STATUS_RIDING, s);
        break;
                                                        }
    default: break;
    }

    if (get_scene() != NULL) {
        get_scene()->screen_broadcast_modify_role_msg(*this);
    }

    return 0;
}

int scene_user::set_atk_mode(const client::role_status_attack_mode m)
{
    if (get_scene() == NULL) {
        return -1;
    }

    // const region_info* ri = in_scene->get_region_info(get_scene_pos());
    // if (ri != NULL){
    //     if (status.client_set_attack_mode(ri->attk_type, ri->attk_mode, m)){
    //         in_scene->screen_broadcast_modify_role_msg(*this, false);
    //     }
    // } else {
    //     status.set_attack_mode(m);
    //     in_scene->screen_broadcast_modify_role_msg(*this, false);
    // }

    return 0;
}

void scene_user::relive()
{ 
    scene_inst *si = get_scene();
    if(NULL != si) {
        si->get_cell().enter_cell(this, si->to_cell_pos(get_scene_pos()));
		if(!si->relive(this))
		{
			return;
		}
    }

    calculate_attr();
    calculate_status();

    mutable_attr()->set_val(CUR_HP, get_attr().max_hp());
    mutable_status()->set_relive();
    set_dead_time(0);
	set_role_status(false);

    send_client_change_attr();
	change_pre_pet_attr();
	on_update_pet_fight_status();
}

void scene_user::stand(bool broadcast)
{
    scene_role::stand(broadcast);
}

bool scene_user::is_same_map_and_scene(uint32 m_id, uint32 s_id)
{
    return (get_scene() == NULL ? false : get_scene()->get_scene_id() == s_id);
}

void scene_user::check_equip_timeout(uint32 sec)
{
    player_role_props& prg = get_player_role().get_role_props();
    bool sub   = prg.sub_active_second(sec);
    bool check = prg.check_equip_timeout();
    if (sub || check) {
        send_client_change_props_msg();
    }

    if (check) {
        calculate_attr();
        send_client_change_attr();
    }
}

void scene_user::pick_remove()
{
    if (_pick_npc_inst_id == 0 || get_scene() == NULL) {
        return;
    }

    scene_npc* n = get_scene()->get_scene_npc(_pick_npc_inst_id);
    if (n == NULL) {
        return;
    }

    n->pick_remove(_role_id);
}

client::PICK_RET_E scene_user::pick(uint64 sid)
{
    if (sid == 0) {
        return client::PIRE_PARAMETER_ERROR;
    }

    pick_remove();

    scene_npc* n = get_scene()->get_scene_npc(sid);
    if (n == NULL) {
        return client::PIRE_TYPE_NOT_EXIST;
    }

    client::PICK_RET_E ret = n->pick(this);
    if (ret == client::PIRE_SUCCESS) {
        _pick_npc_inst_id = sid;
    }

    return ret;
}

client::PICK_RET_E scene_user::pick_commit()
{
    if (_pick_npc_inst_id == 0) {
        return client::PIRE_TYPE_NOT_EXIST;
    }

    scene_npc* n = get_scene()->get_scene_pick(_pick_npc_inst_id);
    if (n == NULL) {
        return client::PIRE_TYPE_NOT_EXIST;
    }

    client::PICK_RET_E ret = n->pick_commit(this);
    if (ret != client::PIRE_TIME) {
        _pick_npc_inst_id = 0;
    }

    return ret;
}

int scene_user::on_region_check(const region_data& r)
{
    return 0;
}

int scene_user::on_leave_region(const region_data& r)
{
    if (scene_role::on_leave_region(r) != 0) {
        return -1;
    }

    return 0;
}

int scene_user::on_enter_region(const region_data& r)
{
    if (scene_role::on_enter_region(r) != 0) {
        return -1;
    }

    return 0;
}

void scene_user::add_mail(client::delivery_mail_info& dmi)
{
    uint32 mail_id = get_player_role().get_role_mails().add_mail(get_role_id(), dmi.mi());

    client::new_mail_recv new_mail_rsp;
    client::mail_head* mh = new_mail_rsp.add_mh();
    mh->CopyFrom(dmi.mi().mh());
    mh->set_mail_id(mail_id);
    send_client(new_mail_rsp);
}

void scene_user::add_activity(uint32 count)
{
}

void scene_user::set_ban(uint32 type, uint32 end)
{
    get_player_role().set_ban(type, end);
}

void scene_user::save_logs()
{
    if (get_scene() != NULL) {
        get_player_role().get_logs().save(get_scene()->get_ctx());
    }
}

void scene_user::on_convoy_task_start(uint32 quality, bool notify)
{
    if (quality == 0) {
        return;
    }

    mutable_status()->set_guard_start(quality);
    if (notify) {
        send_client_change_attr();
    }

    if (quality == 6) {
        sys_msg_convoy();
    }
}

void scene_user::on_convoy_task_end(bool active_end, uint32* quality)
{
    mutable_status()->set_guard_end();
    if (active_end) { // 被杀，主动放弃了运镖，要有相应的加成
        //task_operator.on_convoy_stop(quality);
    }

    send_client_change_attr();
}

void scene_user::save_formal_pos()
{
    if (get_scene() == NULL) { //出现错误重置初生点
        init_formal_pos();
        return ;
    }

    if (get_scene()->get_scene_type() != MAP_TYPE_NORMAL) {
        init_formal_pos();
    } else {
        _save_map_id   = get_scene()->get_map_id();
        _save_scene_id = get_scene()->get_scene_id();
        _save_pos      = get_scene_pos();
    }
}

void scene_user::save_formal_pos(uint32 scene_id, uint32 x, uint32 y)
{
    _save_map_id   = scene_id;
    _save_scene_id = scene_id;
    _save_pos.x    = x;
    _save_pos.y    = y;
}

void scene_user::init_formal_pos()
{
    const config_map* cm = CONFMGR->get_config_map_default();
    if (cm != NULL) {
        _save_map_id   = cm->get_id();
        _save_scene_id = cm->get_id();
        _save_pos.reset(cm->get_birth_point_x(), cm->get_birth_point_y(), 0);
    }
}

void scene_user::on_raid_complete(uint32 raid_id, uint32 type, uint32 star)
{
    if (mutable_task()->add_raid(raid_id)) {
        send_client(mutable_task()->get_update());
        mutable_task()->get_update().Clear();
    }
}


void scene_user::auto_raid()
{
#ifndef __LIB_MINI_SERVER__
    if (get_scene() == NULL) {
        return ;
    }
    player_role_auto_raid* auto_raid = get_player_role().get_auto_raid();
    if (auto_raid == NULL || auto_raid->get().id() == 0) {
        return ;
    }
    client::auto_raid_info& info = auto_raid->get();
    uint32 count = 1;
    const game_map* map = GAME_MAP_MGR->get_map(info.id());
    if (map == NULL) {
        return ;
    }
    const scene* sc = map->get_scene(info.id());
    if (sc == NULL) {
        return ;
    }
    const config_raid* pcr = sc->get_config_raid();
    uint32 all_npc_count = sc->get_dynamic_npcs().size() + sc->get_npc_list().size();
    count = all_npc_count;
    for (uint32 i = 0; i < count; ++i) 
	{
        const origin_npc* npc = NULL;
        if (info.monster() < sc->get_npc_list().size()) 
		{
            npc = sc->get_npc_list().at(info.monster());
            info.set_monster(info.monster() + 1);
        } else if ((info.monster() - sc->get_npc_list().size()) < sc->get_dynamic_npcs().size()) {
            npc = sc->get_dynamic_npcs().at(info.monster() - sc->get_npc_list().size());
            info.set_monster(info.monster() + 1);
        }
		if(NULL == npc)
		{
			break;
		}
    }
	const std::vector<int>* reward_item = NULL;
	vector<int> v;
	if(pcr->get_map_type() == MAP_TYPE_DEVIL)
	{
		const config_devil *cd = CONFMGR->get_config_devil(info.id());
		if(cd == NULL)
		{
			return ;
		}
		v.push_back(cd->get_again_reward());
		reward_item = &v;
	}
	else
	{
		if (info.type() == 0) {
			info.set_exp(pcr->get_reward_experience());
			info.set_coin(pcr->get_reward_money());
			info.set_military_strategy(pcr->get_reward_tao_lue());
			reward_item       = &pcr->get_reward_item();
		} else  {
			info.set_exp(pcr->get_reward_experience_2());
			info.set_coin(pcr->get_reward_money_2());
			info.set_military_strategy(pcr->get_reward_tao_lue());
			reward_item       = &pcr->get_reward_item_2();
		}
		uint32 family_add = add_family_exp(info.exp());
		info.set_exp(info.exp() + family_add);
	}
	vector<client::props_info> props;
	for(size_t i = 0; i < reward_item->size(); ++i)
	{
		make_props(reward_item->at(i), props, get_job());
	}
	for(size_t i = 0; i < props.size(); ++i)
	{
		client::props_info *pi = info.add_props();
		pi->CopyFrom(props[i]);
	}
    client::auto_raid_msg msg;
    msg.mutable_info()->CopyFrom(info);
    send_client(msg);
	if (info.count() != 0) {
		info.set_frozen_silver(info.frozen_silver() > 1 ? info.frozen_silver() - 1 : 0);
		info.set_frozen_physical_power(info.frozen_physical_power() > 1 ? info.frozen_physical_power() - 5 : 0);
		info.set_start_time(info.start_time());
		info.set_count(count - 1);
	} else {
		info.set_id(0);
		mutable_status()->set_raid(false);
	}
	add_props(props);
	add_exp(info.exp());
	add_money(TAEL, info.coin());
	add_money(MILITARY_STRATEGY, info.military_strategy());
	info.clear_exp();
	info.clear_coin();
	info.clear_military_strategy();
	info.clear_props();
	send_client_change_attr();
	send_client_change_props_msg();
#endif
}

uint32 scene_user::get_raid_complete_count(uint32 id)
{
#ifdef __LIB_MINI_SERVER__
    if (_raid_count != 0) {
        uint32 temp = _raid_count;
        _raid_count = 0;
        return temp;
    }
#endif // __LIB_MINI_SERVER__

    player_role_raid* p = get_player_role().get_raid_mgr().mutable_raid(id);
    if (p == NULL) {
        return 0;
    }

    return p->get_complete_count(get_raid_type());
}

#ifdef __LIB_MINI_SERVER__
void scene_user::set_raid_complete_count(uint32 count)
{
    _raid_count = count;
}
#endif // __LIB_MINI_SERVER__

bool scene_user::call_pet(uint32 pet_id, player_role_pet& prp)
{
    scene_inst* psi = get_scene();
    if (psi == NULL) {
        return false;
    }

    std::map<uint64, scene_role*>::iterator it = _pets.begin();
    for (;it != _pets.end();++it) {
        if (it->second->get_role_id() == pet_id) {
            if (it->second->get_scene() != NULL && !it->second->get_status().is_dead()){
				return false;
            }else{
				scene_inst* master_inst = this->get_scene();
				if (master_inst == NULL){
					return false;
				}

				it->second->attach_scene(master_inst);
				it->second->set_born_pos(get_scene_pos());
				it->second->set_scene_pos(get_scene_pos());
				it->second->relive();
				it->second->set_attr(CUR_HP,it->second->get_attr(MAX_HP));
				master_inst->enter_scene(it->second);
				it->second->send_client_change_attr();
				return true;
			}
        }
    }

    const origin_npc* n = GAME_MAP_MGR->get_npc(pet_id);
    if (n == NULL) {
        return false;
    }

    map_postion mp = get_scene_pos();
    get_scene()->calc_role_cell_to_map_postion(this, 1, mp.x, mp.y);

    scene_pet* sp = new scene_pet(*n, prp.mutable_pet_info(pet_id), psi->generate_pet_id(), this);
    add_pet(sp);
    sp->set_role_id(n->get_role_id());
    sp->set_born_pos(get_scene_pos());
    sp->set_scene_pos(mp);
	sp->relive();

    return true;
}

bool scene_user::ectype_call_pet(uint32 pet_id,player_role_pet& prp,client::pet_fight_mode_change_rsp& rsp_msg,bool is_frist)
{
    scene_inst* psi = get_scene();
    if (psi == NULL) {
        return false;
    }

    const origin_npc* n = GAME_MAP_MGR->get_npc(pet_id);
    if (n == NULL) {
        return false;
    }

    std::map<uint64, scene_role*>::iterator it = _pets.begin();
    for ( ; it != _pets.end(); ++it) {
        if (it->second->get_role_id() == pet_id) {
			if (it->second->get_scene() != NULL){
				rest_pet(pet_id, true);
				client::pet_fight_info* info =  rsp_msg.add_info();
				info->set_pet_id(pet_id);
				info->set_pet_status(2);
				// 战斗副本美人切换召回
				call_back_pet(pet_id, prp, true, rsp_msg);
				return true;
			}else{
				scene_inst* master_inst = this->get_scene();
				if (master_inst == NULL){
					return false;
				}
				it->second->attach_scene(master_inst);
				it->second->set_born_pos(get_scene_pos());
				it->second->set_scene_pos(get_scene_pos());
				it->second->relive();
				it->second->set_attr(CUR_HP,it->second->get_attr(MAX_HP));
				master_inst->enter_scene(it->second);
				it->second->send_client_change_attr();
				client::pet_fight_info* info = rsp_msg.add_info();
				info->set_pet_id(n->get_role_id());
				info->set_pet_status(1);
				// 战斗副本美人切换召回
				call_back_pet(pet_id, prp, true, rsp_msg);
				return true;
			}
        }
    }
	// 新的
    map_postion mp = get_scene_pos();
    get_scene()->calc_role_cell_to_map_postion(this, 1, mp.x, mp.y);

    client::pet_db_info* pet_info = prp.mutable_pet_info(pet_id);
    if (pet_info == NULL) {
        return false;
    }


    // 副本中的切换

	scene_pet* sp = new scene_pet(*n, pet_info, psi->generate_pet_id(), this, is_frist);


    add_pet(sp);
    sp->set_role_id(n->get_role_id());
    sp->set_born_pos(get_scene_pos());
    sp->set_scene_pos(mp);
	sp->relive();


    client::pet_fight_info* info = rsp_msg.add_info();
    info->set_pet_id(n->get_role_id());
    info->set_pet_status(1);

    // 战斗副本美人切换召回
    call_back_pet(pet_id, prp, true, rsp_msg);

    return true;
}

uint32 scene_user::get_current_pet()
{
    player_role_pet &player_pet = get_player_role().get_pets();
    const std::map<uint32, client::pet_db_info>& pet_db_map_ = player_pet.get_all_pet_db_info();
    std::map<uint32, client::pet_db_info>::const_iterator itr = pet_db_map_.begin();
    for (;itr != pet_db_map_.end(); ++itr){
        if (itr->second.status() == PET_STATUS_FIGHTING){
            return itr->first;
        }
    }
    return 0;
}

bool scene_user::has_pet(uint32 id)
{
    player_role_pet &prp = get_player_role().get_pets();
    const std::map<uint32, client::pet_db_info>& pet_db_info_map_ = prp.get_all_pet_db_info();
    return pet_db_info_map_.find(id) != pet_db_info_map_.end();
}

bool scene_user::call_back_pet(uint32 pet_id, player_role_pet& prp, bool is_ectype, client::pet_fight_mode_change_rsp& rsp_msg)
{
    if (!is_ectype) {
        return true;
    }

    const std::map<uint32, client::pet_db_info>& pet_db_info_map_ = prp.get_all_pet_db_info();
    std::map<uint32, client::pet_db_info>::const_iterator it_pet = pet_db_info_map_.begin();
    uint32 pet_status = 0;
    for (; it_pet != pet_db_info_map_.end(); ++it_pet) {
        if (prp.get_pet_status(it_pet->first, pet_status) != 0) {
            continue;
        }

        if (pet_status != PET_STATUS_FIGHTING && pet_status != PET_STATUS_PREPARE) {
            continue;
        }

        if (it_pet->first == pet_id) {
            continue;
        }

        std::map<uint64, scene_role*>::iterator it_sc = _pets.begin();
        for ( ; it_sc != _pets.end(); ++it_sc) {
            if (it_sc->second != NULL && it_sc->second->get_role_id() == it_pet->first && it_sc->second->get_role_type() == RTE_PET) {
                scene_pet* fight_obj = static_cast<scene_pet*>(it_sc->second);
                if (fight_obj == NULL) {
                    return false;
                }

                rest_pet(it_pet->first, true);
                client::pet_fight_info* info = rsp_msg.add_info();
                info->set_pet_id(it_pet->first);
                info->set_pet_status(2);
                break;
            }
        }
    }

    return true;
}

bool  scene_user::levels_call_pet(client::pet_fight_mode_change_req* req,player_role_pet& prp,client::pet_fight_mode_change_rsp& rsp_msg,bool is_frist)
{
	scene_inst* psi = get_scene();
	if (psi == NULL) {
		rsp_msg.set_result_type(PET_FIGHT_MODE_CHANGE_FAILED);
		return false;
	}

	client::pet_db_info pet_info;
	if (!prp.get_pet_info(req->pet_id(),pet_info)){
		rsp_msg.set_result_type(PET_FIGHT_MODE_CHANGE_FAILED);
		return false;
	}

	const origin_npc* n = GAME_MAP_MGR->get_npc(req->pet_id());
	if (n == NULL) {
		rsp_msg.set_result_type(PET_FIGHT_MODE_CHANGE_FAILED);
		return false;
	}

	bool is_new = true;
	std::map<uint64, scene_role*>::iterator it = _pets.begin();
	for ( ; it != _pets.end(); ++it) {
		if (it->second->get_role_id() == req->pet_id()) {
			if (it->second->get_scene() != NULL){
				rest_pet(req->pet_id(), true);
				client::pet_fight_info* info =  rsp_msg.add_info();
				info->set_pet_id(req->pet_id());
				info->set_pet_status(2);
				rsp_msg.set_result_type(PET_FIGHT_MODE_CHANGE_SUCCEED);
				return true;
			}else{
				scene_inst* master_inst = this->get_scene();
				if (master_inst == NULL){
					rsp_msg.set_result_type(PET_FIGHT_MODE_CHANGE_FAILED);
					return false;
				}

				it->second->attach_scene(master_inst);
				it->second->set_born_pos(get_scene_pos());
				it->second->set_scene_pos(get_scene_pos());
				it->second->relive();
				it->second->set_attr(CUR_HP,it->second->get_attr(MAX_HP));
				master_inst->enter_scene(it->second);
				it->second->send_client_change_attr();
				client::pet_fight_info* info = rsp_msg.add_info();
				info->set_pet_id(n->get_role_id());
				info->set_pet_status(1);
			}

			is_new = false;
		}
	}

	if (is_new){
		// 新的
		map_postion mp = get_scene_pos();
		get_scene()->calc_role_cell_to_map_postion(this, 1, mp.x, mp.y);
		client::pet_db_info* pet_db = prp.mutable_pet_info(req->pet_id());
		if (pet_db == NULL) {
			rsp_msg.set_result_type(PET_FIGHT_MODE_CHANGE_FAILED);
			return false;
		}
		// 副本中的切换
		scene_pet* sp = new scene_pet(*n, pet_db, psi->generate_pet_id(), this, is_frist);
		add_pet(sp);
		sp->set_role_id(n->get_role_id());
		sp->set_born_pos(get_scene_pos());
		sp->set_scene_pos(mp);
		sp->relive();

		client::pet_fight_info* info = rsp_msg.add_info();
		info->set_pet_id(n->get_role_id());
		info->set_pet_status(1);
	}

	rsp_msg.set_result_type(PET_FIGHT_MODE_CHANGE_SUCCEED);

	return true;
}

bool scene_user::pet_levels_set_req(uint32 pet_id, uint32 levels_status,client::pet_levels_set_rsp& rsp_msg)
{
	player_role_pet& player_pet = get_player_role().get_pets();

	client::pet_db_info* pet_info = player_pet.mutable_pet_info(pet_id);
	if (pet_info == NULL){
		rsp_msg.set_result_type(1);
		rsp_msg.set_pet_id(pet_id);
		rsp_msg.set_levels_status(levels_status);
		return false;
	}


	pet_info->set_levels_status(levels_status);
	rsp_msg.set_result_type(0);
	rsp_msg.set_pet_id(pet_id);
	rsp_msg.set_levels_status(levels_status);

	return true;
}

void scene_user::reset_original_attr()
{
    get_player_role().init_original_attr(*mutable_attr());
}

void scene_user::fall_from_mounts(uint32 tick)
{
    scene_role::fall_from_mounts(tick);
    get_player_role().get_query_attr().set_mounts_id(0);
    calculate_attr();
    set_attr(MOUNTS_ID, 0);
    send_client_change_attr();
}

void scene_user::add_friend_family(uint32 id)
{
	if(_ffl.find(id) == _ffl.end())
	{
		_ffl.insert(make_pair(id, 1));
	}
}

void scene_user::remove_friend_family(uint32 id)
{
	std::map<uint32, int>::iterator it = _ffl.find(id);
	if(it != _ffl.end())
	{
		_ffl.erase(it);
	}
}

void scene_user::clear_friend_family()
{
	_ffl.clear();
}

bool scene_user::is_friend_family(uint32 family_id)
{
	return _ffl.find(family_id) != _ffl.end();
}

void scene_user::find_replace_bow_skill(uint32& need_replace_id,uint32& rand_ser_index)
{
	// key: ser_index ,value: id
	std::map<uint32,uint32> bow_skill_map_;
	bow_skill_map_.clear();

	get_bow_skill(bow_skill_map_);

	uint32 rand_size = static_cast<uint32>(bow_skill_map_.size());
	// 随机找一个替换
	rand_ser_index = mtrandom::rand_mid32(1,rand_size);
	std::map<uint32,uint32>::iterator it = bow_skill_map_.begin();
	for (; it != bow_skill_map_.end(); ++it){
		if (it->first == rand_ser_index){
			need_replace_id = it->second;
			break;
		}
	}
}

void scene_user::enter_mini_server(uint32 raid_id, uint32 enter_time)
{
    _raid_id = raid_id;
    _raid_time = enter_time;
}

bool scene_user::check_mini_server(uint32 raid_id, uint32 leave_time)
{
    if (_raid_id != raid_id) {
        return false;
    }

    if (_raid_time >= leave_time) {
        return false;
    }

    return true;
}

uint32 scene_user::cal_equip_reward_power()
{
	float totoal_power = 0.0f;
	props_parser& equip = get_player_role().get_role_props().get_equip();
	// 算人物的
	for (uint32 i = EQUIP_GRID_BEG; i <= EQUIP_SHOE; ++i){
		totoal_power += equip.calcula_depot_power(i);
	}

	// 算美人的
	const std::map<uint32, client::pet_db_info>& pet_map_ = get_player_role().get_pets().get_all_pet_db_info();
	std::map<uint32, client::pet_db_info>::const_iterator it = pet_map_.begin();
	uint32 begin_index = 0;
	uint32 end_grindex = 0;
	for (; it != pet_map_.end(); ++it){
		begin_index = 1 + it->first* 10;
		end_grindex = 6 + it->first* 10;
		for (uint32 k = begin_index; k <= end_grindex; ++ k){
			totoal_power += equip.calcula_depot_power(k);
		}
	}

	return static_cast<uint32>(ceil(totoal_power));
}

bool scene_user::is_add_attack_count()
{
	std::vector<skill*> vs_ar;
	vs_ar.clear();
	mutable_skill()->get_skill_by_belong(SB_ARCHER, vs_ar);
	if (vs_ar.size() != 0){
		return true;
	}


	return false;
}

void scene_user::update_activity_record(uint32 act_limit_type,uint32 value_count)
{
	client::update_activity_record_g2c center_rsp;
	center_rsp.set_limit_type(act_limit_type);
	center_rsp.set_value_count(value_count);
	center_rsp.set_role_id(get_role_id());
	send_center(center_rsp);
}

size_t scene_user::get_pet_count() 
{
	return get_player_role().get_pets().get_all_pet_db_info().size(); 
}
