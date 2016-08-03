#include "config_historyReward.h"
#include "player_role.h"
#include "player_role_pet.h"
#include "player_role_skill_mgr.h"
#include "offline_role_mgr.h"
#include "center_interface.h"
#include "config_edreward.h"
#include "config_robot.h"
#include "config_map.h"

static void send_mail(center_service_t *w, uint32 role_id, uint32 history, uint32 rank, uint32 num)
{
    client::mail_info mi;
    client::mail_head *mh = mi.mutable_mh();
    client::mail_body* mb = mi.mutable_mb();
    mh->set_mail_id(0);
    mh->set_mail_type(MAIL_TYPE_SYSTEM);
    mh->set_recv_role_id(role_id);
    mh->set_send_role_id(0);
    mh->set_send_role_name("system");
    mh->set_title("arena_history_title");
    mb->set_content("arena_history_content");
    mh->set_send_time((uint32)time(NULL));
	mh->set_past_time(get_mail_past_time(MAIL_TYPE_SYSTEM,true));
    mh->set_attachment_num(2);
    mh->set_is_take(false);
    mh->set_opened(false);
    client::mail_attr *attr = mi.add_attr();
    attr->set_attr_type(INGOT);
    attr->set_attr_value(num);


    client::content_data* cd_history = mb->add_data();
    cd_history->set_n(history);

    client::content_data* cd_rank = mb->add_data();
    cd_rank->set_n(rank);

    send_new_mail(w, role_id, mi);
}

static void pack_robot_attr_from_db(player_role* pr, client::robot_attr* ra)
{
    if(NULL == pr) {
        return ;
    }

    mysql::tb_player_role *tpr = pr->get_db_player_role();
    if(NULL == tpr) {
        return ;
    }
    ra->set_config_id(1110 + tpr->profession());
    ra->set_role_id(tpr->role_id());
    ra->set_role_name(tpr->role_name());
    ra->set_level(tpr->level());
    ra->set_profession(tpr->profession());
    ra->set_sex(tpr->sex());
    ra->set_fighting_power(tpr->fighting_power());
    ra->set_hp_max(tpr->max_hp());
    ra->set_attack_min(tpr->attack_min());
    ra->set_attack_max(tpr->attack_max());
    ra->set_defense_min(tpr->defense_min());
    ra->set_defense_max(tpr->defense_max());
    ra->set_hit(tpr->hit_point());
    ra->set_dodge(tpr->dodge());
    ra->set_recover(tpr->recover());
    ra->set_move_speed(tpr->move_speed());
    ra->set_mount_id(tpr->mounts_id());
    ra->set_weapon_id(tpr->weapon_id());
    ra->set_armor_id(tpr->armor_id());
	ra->set_vip_level(tpr->vip_level());
	if(tpr->map_id() == 0)
	{
		const config_map *cm = CONFMGR->get_config_map_default();
		if(cm == NULL)
		{
			return;
		}
		ra->set_x(cm->birthPointX);
		ra->set_y(cm->birthPointY);
	}
	else
	{
		ra->set_x(tpr->x_pos());
		ra->set_y(tpr->y_pos());
	}
	ra->set_family_id(tpr->family_id());
    player_role_pet& player_pet = pr->get_pets();
    const std::map<uint32, client::pet_db_info>& pet_db_map_ = player_pet.get_all_pet_db_info();
    std::map<uint32, client::pet_db_info>::const_iterator itr = pet_db_map_.begin();
    for (;itr != pet_db_map_.end(); ++itr) {
        if (itr->second.status() == PET_STATUS_FIGHTING) {
            ra->mutable_pet()->CopyFrom(itr->second);
        }
		if (itr->second.status() == PET_STATUS_PREPARE) {
			ra->mutable_backup()->CopyFrom(itr->second);
		}
    }

    pr->get_role_skill_mgr().fill_role_skill(ra->mutable_sk());
}

static void fill_pet_db_info(client::pet_db_info *pet, uint32 pet_id, uint32 pet_level, const map<int, int> &m)
{
	pet->set_pet_id(pet_id);
	pet->set_pet_level(pet_level);
	pet->set_pet_exp(0);
	pet->set_cur_hp(0);
	pet->set_status(0);
	pet->set_lingering_count(0);
	pet->set_magic_star(0);
	pet->set_debris_exp(0);
	map<int, int>::const_iterator it;
	for(it = m.begin(); it != m.end(); ++it)
	{
		client::pet_fight_attr *pfa = pet->add_pfa();
		pfa->set_id(it->first);
		pfa->set_num(it->second);
	}
}

static client::robot_attr *pack_robot_attr_from_cfg(const robot *r, client::robot_attr *ra)
{
    if(r == NULL || ra == NULL) {
        return NULL;
    }
    const config_robot *cr = CONFMGR->get_config_robot(r->get_config_id());
	if(cr == NULL)
	{
		return NULL;
	}
	ra->set_role_id(r->get_role_id());
    ra->set_role_name(r->get_role_name());
    ra->set_level(cr->get_robot_level());
    ra->set_profession(cr->get_robot_job());
    ra->set_fighting_power(r->get_fight_power());
	const map<int, int> &robot_attr = r->get_robot_attr();
	map<int, int>::const_iterator it;
	const google::protobuf::Reflection *ref = ra->GetReflection();
	const google::protobuf::Descriptor *des = ra->GetDescriptor();
	if(ref == NULL || des == NULL)
	{
		return NULL;
	}
	int i = 0;
	for(it = robot_attr.begin(); it != robot_attr.end(); ++it)
	{
		ref->SetUInt32(ra, des->field(i + 6), (uint32)it->second);
		i++;
	}
    ra->set_mount_id(cr->get_mount_id());
    ra->set_weapon_id(cr->get_weapon_id());
    ra->set_armor_id(cr->get_armor_id());
    ra->set_config_id(r->get_config_id());
	if(r->get_pet_id() != 0)
	{
		client::pet_db_info* pet = ra->mutable_pet();
		fill_pet_db_info(pet, r->get_pet_id(), cr->get_general_level(), r->get_pet_attr());
	}
	if(r->get_standby_id() != 0)
	{
		client::pet_db_info *pet = ra->mutable_backup();
		fill_pet_db_info(pet, r->get_standby_id(), cr->get_standby_pet_level(), r->get_standby_attr());
	}
	return ra;
}

// 从数据库中拉取数据
void load_user_data(center_service_t *w, client::load_target_data_req req)
{
    player_role *pr = NULL;
    const offline_role* por = w->ou_mgr->get(req.target());
    if (por == NULL) {
        center_load_role_req(w, req.target(), req);
        return;
    }
    pr = por->get();
    if (pr == NULL) {
        center_load_role_req(w, req.target(), req);
        return;
    }
    client::load_target_data_rsp rsp;	
    rsp.set_role_id(req.role_id());
    pack_robot_attr_from_db(pr, rsp.mutable_attr());
	center_send_scene_by_scene_id(w, req.scene_id(), rsp);
}

// 读取配置表
static void load_npc_data(center_service_t *w, client::load_target_data_req req)
{
    const robot *r = w->rbt_mgr.get_robot(req.target());
    if(r == NULL)
    {
        return;
    }
    const online_role *oi = w->oi_mgr.get_online_info(req.role_id());
    if(!oi)
    {
        return;
    }

    client::load_target_data_rsp rsp;
    rsp.set_role_id(req.role_id());
    pack_robot_attr_from_cfg(r, rsp.mutable_attr());
    center_send_scene(w, *oi, rsp);
}

static const string& query_role_name(center_service_t *w, uint32 role_id)
{
    const mysql::sp_load_all_user_smy *smy = w->aus_mgr.get_user_smy(role_id);
    if(!smy)
    {
        static string str = "";
        return str;
    }
    return smy->role_name();
}

static const string& query_robot_name(center_service_t *w, uint32 role_id)
{
	const robot *r = w->rbt_mgr.get_robot(role_id);
	if(r == NULL)
    {
        static string str = "";
        return str;
    }
	return r->get_role_name();
}

void *on_pb_arena_info_req(client::arena_info_req *req, void *ud, size_t *rsp_len)
{
    center_service_t *w = (struct center_service_t *)ud;
    uint32 role_id = req->svr_head().role_id();
    const arena *mine = w->ar_mgr.get_arena_info(role_id);
    if(NULL == mine)
    {
        return NULL;
    }
    client::arena_info_rsp rsp;
    rsp.set_history(mine->get_history_rank());	//历史记录
    rsp.set_rank(mine->get_rank());	//个人当前排名
    rsp.set_status(w->ar_mgr.get_record_status(role_id));
    vector<int> v;
    w->ar_mgr.get_challenge_role_list(role_id, v);
    for(size_t i = 0; i < v.size(); ++i)
    {
        const arena *a = w->ar_mgr.get_arena_info(v[i]);
        if(!a)
        {
            continue;
        }
        client::role_info *ri = rsp.add_ri();
        ri->set_role_id(a->get_role_id());
        ri->set_rank(a->get_rank());
        ri->set_type(a->get_role_type());
        if(a->get_role_type() == ARENA_ROBOT)
        {
			const robot *r = w->rbt_mgr.get_robot(a->get_role_id());
            const config_robot *cr = CONFMGR->get_config_robot(r->get_config_id());
            if (cr == NULL) {
                continue;
            }
            ri->set_name(r->get_role_name());
            ri->set_profession(cr->get_robot_job());
            ri->set_sex(cr->get_robot_job());
            ri->set_weapon_id(cr->get_weapon_id());
            ri->set_armor_id(cr->get_armor_id());
            ri->set_fighting_power(r->get_fight_power());
        }
    }
    center_send_client(w, req->svr_head(), rsp);
    return NULL;
}

void *on_pb_load_target_data_req(client::load_target_data_req *req, void *ud, size_t *rsp_len)
{
    center_service_t *w = (center_service_t *)ud;
    int type = w->ar_mgr.get_role_type(req->target());
    if(type == ARENA_ROBOT)	//NPC
    {
        load_npc_data(w, *req);
    }
    else
    {
        const online_role *oi = w->oi_mgr.get_online_info(req->target());
        const online_role *mine = w->oi_mgr.get_online_info(req->role_id());
        if(oi != NULL)
        {
            client::load_online_target_data_req lotd;
            lotd.set_role_id(req->role_id());
            lotd.set_target(req->target());
            lotd.set_svr_scene_id(mine->scene_svr_id);
            center_send_scene(w, *oi, lotd);
        }
        else
        {
            load_user_data(w, *req);
        }
    }
    return NULL;
}

// 发送战斗结果
// a1战斗发起方
void *on_pb_arena_fight_ret(client::arena_fight_ret *ret, void *ud, size_t *rsp_len)
{
    center_service_t *w = (center_service_t *)ud;
    const arena *a1 = w->ar_mgr.get_arena_info(ret->role_id());
    const arena *a2 = w->ar_mgr.get_arena_info(ret->target());
    if(!a1 || !a2)
    {
        return NULL;
    }
    int history = a1->get_history_rank();
    int current = a2->get_rank();
    ret->set_r_name(query_role_name(w, ret->role_id()));
    if(a2->get_role_type() == ARENA_PLAYER)
    {
        ret->set_t_name(query_role_name(w, ret->target()));
    }
    else
    {
        ret->set_t_name(query_robot_name(w, ret->target()));
    }
    w->ar_mgr.challenge_over(ret);
    const online_role *oi = w->oi_mgr.get_online_info(ret->role_id());
    if(!oi)
    {
        return NULL;
    }
    // 发送战斗结果
    client::arena_challenge_result rsp;	
    rsp.set_num(CONFMGR->get_basic_value(10040 + (int)ret->ret()));
    rsp.set_ret(ret->ret());
    if(ret->ret() == client::FIGHT_WIN)
    {
        rsp.set_history(a1->get_history_rank());
        rsp.set_current(a1->get_rank());
        rsp.set_status(history > current);
    }
    center_send_client(w, *oi, rsp);
    // 发送最高历史排名奖励
    if(ret->ret() == client::FIGHT_WIN && history > current)
    {
        uint32 n = (uint32)CONFMGR->get_config_history_reward_mgr().get_reward(history, current);
		if(n > 0)
		{
			send_mail(w, ret->role_id(), history, current, n);
		}
    }
    amtsvr_billlog(w->ctx, "%u|%s|%s|%u|%u|%u|%u", ret->role_id(), ret->GetTypeName().c_str(), ret->t_name().c_str(), ret->ret(), rsp.num(), history, current);
    return NULL;
}

void *on_pb_arena_record_req(client::arena_record_req *req, void *ud, size_t *rsp_len)
{
    center_service_t *w = (center_service_t *)ud;
    uint32 role_id = req->svr_head().role_id();
    const arena *mine = w->ar_mgr.get_arena_info(role_id);
    if(NULL == mine)
    {
        return NULL;
    }
    client::arena_record_rsp rsp;
    vector<client::arena_record_info> vari;
    w->ar_mgr.get_record(role_id, vari);
    for(size_t i = 0; i < vari.size(); ++i)
    {
        client::arena_record_info *ari = rsp.add_ar();
        ari->CopyFrom(vari[i]);
    }
    center_send_client(w, req->svr_head(), rsp);
    return NULL;
}

void *on_pb_center_arena_reward_req(client::arena_reward_req *req, void *ud, size_t *rsp_len)
{
    center_service_t * w = (center_service_t *)ud;

    const std::vector<config_edreward *> &reward_list = CONFMGR->get_all_edreward();
    client::arena_reward_rsp rsp;
    for(size_t i = 0; i < reward_list.size(); ++i)
    {
        client::arena_reward_item *ari = rsp.add_ari();
        ari->set_min_rank(reward_list[i]->min_rank);  
        ari->set_max_rank(reward_list[i]->max_rank);
        client::reward_info *ri = ari->add_ri();
        for(size_t j = 0; j < reward_list[i]->_reward.size(); ++j)
        {
            ri->set_type(reward_list[i]->_reward[j].first);
            ri->set_value(reward_list[i]->_reward[j].second);
        }
    }
    center_send_client(w, req->svr_head(), rsp);
    return NULL;
}

void *on_pb_center_arena_rank_list_req(client::arena_rank_list_req *req, void *ud, size_t *rsp_len)
{
    center_service_t *w = (center_service_t *)ud;

    uint32 role_id = req->svr_head().role_id();

    client::arena_rank_list_rsp rsp;

    const arena *a = w->ar_mgr.get_arena_info(role_id);
    if(!a)
    {
        rsp.set_rank(-1);
    }
    else
    {
        rsp.set_rank(a->get_rank());
    }
    const vector<uint32>& v = w->ar_mgr.get_arena_list();
    for(size_t i = 0; i < v.size(); ++i)
    {
        if(i >= 100)
        {
            break;
        }
        
        uint32 r = v[i];
        if(w->ar_mgr.get_role_type(r) == ARENA_PLAYER)
        {
            const mysql::sp_load_all_user_smy *smy = w->aus_mgr.get_user_smy(r);
            if(!smy)
            {
                continue;
            }

			client::rank_list_item *rli = rsp.add_rli();
            rli->set_name(smy->role_name());
            rli->set_level(smy->level());
            rli->set_power(smy->fighting_power());
        }
        else
        {
			const robot * robot = w->rbt_mgr.get_robot(r);
			if(robot == NULL)
			{
				continue;
			}
            const config_robot *cr = CONFMGR->get_config_robot(robot->get_config_id());
            if (cr == NULL) {
                continue;
            }
			client::rank_list_item *rli = rsp.add_rli();
            rli->set_name(robot->get_role_name());
            rli->set_level(cr->get_robot_level());
            rli->set_power(robot->get_fight_power());
        }
    }
    center_send_client(w, req->svr_head(), rsp);
    return NULL;
}
