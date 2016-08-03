#include "config_map.h"
#include "arena_scene_inst.h"
#include "team_scene_inst.h"
#include "cycle_scene_inst.h"
#include "player_role.h"
#include "player_role_skill_mgr.h"
#include "game_interface.h"
#include "siege_scene_inst.h"
#include "client.raid_operate.pb.h"

#define ARENA_MAP_ID 50101

static void load_target_data(game_service_t *ss, scene_user *su, uint32 target, uint32 scene_id)
{
    client::load_target_data_req req;
    req.set_target(target);
    req.set_role_id(su->get_role_id());
    req.set_scene_id(scene_id);
    su->send_center(ss->ctx, req);
}

static void load_user_data(game_service_t *ss, client::load_online_target_data_req req, scene_user *su)
{
    client::load_target_data_rsp rsp;
    rsp.set_role_id(req.role_id());
    client::robot_attr *ra = rsp.mutable_attr();
    ra->set_role_id(su->get_role_id());
    ra->set_role_name(su->get_role_name());
    ra->set_level(su->get_attr().get_val(LEVEL));
    ra->set_profession(su->get_attr().profession());
    ra->set_config_id(1110 + ra->profession());
    ra->set_sex(su->get_attr().sex());
    ra->set_fighting_power(su->get_attr().get_val(FIGHTING_POWER));
    ra->set_hp_max(su->get_attr().get_val(MAX_HP));
    ra->set_attack_min(su->get_attr().get_val(ATTACK_MIN));
    ra->set_attack_max(su->get_attr().get_val(ATTACK_MAX));
    ra->set_defense_min(su->get_attr().get_val(DEFENSE_MIN));
    ra->set_defense_max(su->get_attr().get_val(DEFENSE_MAX));
    ra->set_recover(su->get_attr().get_val(RECOVER));
    ra->set_hit(su->get_attr().get_val(HIT_POINT));
    ra->set_dodge(su->get_attr().get_val(DODGE));
    ra->set_move_speed(su->get_attr().get_val(MOVE_SPEED));
    ra->set_weapon_id(su->get_attr().weapon_id());
    ra->set_armor_id(su->get_attr().armor_id());
    ra->set_mount_id(su->get_attr().mounts_id());
	ra->set_vip_level(su->get_attr().get_val(ROLE_VIP_LEVEL));
    player_role_pet& player_pet = su->get_player_role().get_pets();
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
    su->get_player_role().get_role_skill_mgr().fill_role_skill(ra->mutable_sk());

    game_send_scene(ss, req.svr_scene_id(), rsp);
}

//挑战次数信息
void *on_pb_arena_personal_info_req(client::arena_personal_info_req *req, void *ud, size_t *rsp_len)
{
    PB_FUNC_GUARD(req);
    if(psu->get_level() < 1)
    {
        return NULL;
    }
    client::arena_personal_info_rsp rsp;
    rsp.set_used(psu->get_arena_used_times());
    rsp.set_total(CONFMGR->get_basic_value(10019));
	rsp.set_left(psu->get_challenge_cd());
	rsp.set_timelong(psu->get_arena_left_fresh_time());
    psu->send_client(rsp);

    amtsvr_billlog(ss->ctx, "%u|%s|%u|%u|%u|%u", psu->get_role_id(), req->GetTypeName().c_str(), rsp.used(), rsp.total(), rsp.left(), rsp.timelong());
    return NULL;
}

//挑战
void *on_pb_arena_challenge_req(client::arena_challenge_req *req, void *ud, size_t *rsp_len)
{
    struct game_service_t *ss = (struct game_service_t *)ud;
    if (req->svr_head().role_id() == req->target()) {
        return NULL;
    }

    scene_user *su = ss->sim->get_user(req->svr_head().role_id());
    if (su == NULL) {
        return NULL;
    }

    scene_inst *si = su->get_scene();
    if (si == NULL || si->get_scene_type() == MAP_TYPE_ARENA) {
        return NULL;
    }
    // 未冷却
	if(su->get_challenge_cd() != 0)
	{
		return NULL;
	}
    // 次数用完
    if ((int)su->get_arena_used_times() >= CONFMGR->get_basic_value(10019)) 
	{
        return NULL;
    }

	su->set_old_challenge_count(su->get_arena_used_times());

    // 正常挑战
    if (req->type() == 0) {
        su->add_target_id(req->target());
		client::enter_raid_req err;
		err.set_raid_id(50101);
		err.set_t(client::enter_raid_req_type_normal);
		client::server_head *svr_head = err.mutable_svr_head();
		svr_head->CopyFrom(req->svr_head());
		game_send_scene(ss, ss->self, err);
    } else {
        load_target_data(ss, su, req->target(), su->get_scene()->get_scene_id());
    }

    amtsvr_billlog(ss->ctx, "%u|%s", su->get_role_id(), req->GetTypeName().c_str());
    return NULL;
}

//加速冷却
void *on_pb_arena_speed_up_req(client::arena_speed_up_req *req, void *ud, size_t *rsp_len)
{
    PB_FUNC_GUARD(req);

    if (psu->get_level() < 1) {
        return NULL;
    }

    // 元宝不足
    uint32 need = CONFMGR->get_basic_value(10007);
    if (psu->get_attr().get_val(INGOT) < need) {
        return NULL;
    }
	uint32 old_money = psu->get_attr().get_val(INGOT);
    psu->sub_money(INGOT, need);
	uint32 new_money = psu->get_attr().get_val(INGOT);
	psu->sub_money_log(MC_ARENA_SPEND, MC_CLEAR_ARENA_CD, old_money, new_money);
	psu->clear_arena_cd_time();

    client::arena_speed_up_rsp rsp;
    rsp.set_time(0);
    psu->send_client(rsp);
	psu->send_client_change_attr();

    amtsvr_billlog(ss->ctx, "%u|%s|%u|%u|%u", psu->get_role_id(), req->GetTypeName().c_str(), need, old_money, new_money);
    return NULL;
}

void *on_pb_arena_load_target_data_rsp(client::load_target_data_rsp *rsp, void *ud, size_t *rsp_len)
{
	game_service_t *ss = (game_service_t *)ud;
	scene_user *psu = NULL;
	scene_inst *psi = NULL;
	if(rsp->role_id() != 0)
	{
		psu = ss->sim->get_user(rsp->role_id());
		psi = psu->get_scene();
	}
	else
	{
		psi = ss->sim->get_normal_scene();		// 主城
	}
    if (psi == NULL) {
        amtsvr_log("role:%u not in arena\n", psu->get_role_id());
        return NULL;
    }
	switch(psi->get_scene_type())
	{
		case MAP_TYPE_ARENA:
			psu->set_arena_used_times(1);
			static_cast<arena_scene_inst *>(psi)->push(rsp->mutable_attr());
			CHECK_TARGET_PROGRESS(psu, ARENA_CHALLENGE);
			return NULL;
		case MAP_TYPE_TEAM:
			static_cast<team_scene_inst *>(psi)->push(rsp->mutable_attr());
			return NULL;
		case MAP_TYPE_NORMAL:
			psi->push(rsp->mutable_attr());
			return NULL;
		case MAP_TYPE_BATTLE:
			static_cast<siege_scene_inst *>(psi)->push(rsp->mutable_attr());
			return NULL;
		case MAP_TYPE_CYCLE:
			static_cast<cycle_scene_inst *>(psi)->push(rsp->mutable_attr());
		default:
			break;
	}
    return NULL;
}

void *on_pb_load_online_target_data_req(client::load_online_target_data_req *req, void *ud, size_t *rsp_len)
{
    PB_FUNC_GUARD_FIND(req->target());

    load_user_data(ss, *req, psu);

    return NULL;
}

void *on_pb_arena_fight_begin(client::arena_fight_begin *req, void *ud, size_t *rsp_len)
{
	PB_FUNC_GUARD(req);
	scene_inst *psi = psu->get_scene();
	if(NULL == psi || psi->get_scene_type() != MAP_TYPE_ARENA)
	{
		return NULL;
	}
	arena_scene_inst *asi = static_cast<arena_scene_inst *>(psi);
	asi->fight_begin(psu);
    amtsvr_billlog(ss->ctx, "%u|%s", psu->get_role_id(), req->GetTypeName().c_str());
	return NULL;
}
