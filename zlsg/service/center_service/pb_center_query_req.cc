#include "player_role.h"
#include "player_role_props.h"
#include "player_role_skill_mgr.h"
#include "center_interface.h"
#include "player_role_potential.h"
#include "player_role_pet.h"
#include "scene_user.h"
#include "player_role_mounts.h"
#include "player_role_statis_mgr.h"
#include "config_open.h"
#define QUERY_SMY_MAX_NUM 100
#define QUERY_ATTR_MAX_NUM 1
#define QUERY_EQUIP_MAX_NUM 1
#define QUERY_SKILL_MAX_NUM 1

static void* center_query_smy(client::query_req* bir, center_service_t* w, size_t* rsp_len)
{
	client::query_rsp rsp;
	rsp.set_type(bir->type());
	int size = bir->role_ids_size();
    int name_size = bir->role_names_size();
    if (size > QUERY_SMY_MAX_NUM){
        size = QUERY_SMY_MAX_NUM;
    }

	for (int i = 0; i < size; ++i) {
		const mysql::sp_load_all_user_smy* smy = w->aus_mgr.get_user_smy(bir->role_ids(i));
		if (smy == NULL){
			continue;
		}
		client::role_smy_info* rsi_info = rsp.add_rsi();
		const online_role* oi = w->oi_mgr.get_online_info(bir->role_ids(i));
		if (oi == NULL){
			rsi_info->set_online(false);
		}else{
			rsi_info->set_online(true);
		}

		w->aus_mgr.pack_role_smy_info(smy, *rsi_info);
		w->cf_mgr->fill_smy_family_base_data(smy,*rsi_info);
	}

    for (int i = 0; i < name_size; ++i) {
        const mysql::sp_load_all_user_smy* smy = w->aus_mgr.get_user_smy(bir->role_names(i));
        if (smy == NULL) {
            continue ;
        }
		client::role_smy_info* info = rsp.add_rsi();
        w->aus_mgr.pack_role_smy_info(smy, *info);
		w->cf_mgr->fill_smy_family_base_data(smy,*info);
    }

	if (bir->has_opt_flag()){
		rsp.set_opt_flag(bir->opt_flag());
	}

	center_send_client(w, bir->svr_head(), rsp);

	return NULL;
}

static void* center_role_query_info(client::query_req* bir, center_service_t* w, size_t* rsp_len)
{
    int size = bir->role_ids_size();
    if (size <= 0){
        return NULL;
    }

    uint32 role_id = bir->role_ids(0);
    player_role* pr = center_get_player_role(w, *bir, role_id);
    if (pr == NULL){
        return NULL;
    }

    client::query_rsp rsp;
    rsp.set_type(client::CQTE_ROLE_ATTR);
	uint32 open_flag = 0;
    client::role_query_info* role_info = rsp.add_role_info();
	const mysql::sp_load_all_user_smy* smy = w->aus_mgr.get_user_smy(role_id);
	if (smy == NULL){
		return NULL;
	}

	role_info->set_role_id(role_id);
	role_info->set_role_name(smy->role_name());
	w->cf_mgr->fill_query_family_base(role_id,*role_info);
	role_info->set_fight_power(smy->fighting_power());
	role_info->set_profession(smy->profession());
	role_info->set_level(smy->level());
	role_info->set_weapon_id(smy->weapon_id());
	role_info->set_armor_id(smy->armor_id());
	role_info->set_vip_level(smy->vip_level());

    pr->get_query_role_info(*role_info);
	client::pet_db_info pet_info;
	if (pr->get_pets().get_query_fighting_pet(pet_info)){
		INSERT_FLAG(open_flag,ROLE_PET);
	}

	if (pr->get_potentials().get_all_potential_info().size() != 0){
		INSERT_FLAG(open_flag,ROLE_POTENTIAL);
	}
	uint32 is_frist_ride = 0;
	pr->get_role_statis_mgr().get(PRSTE_MOUNT_FRIST_RIDE)->get_val(is_frist_ride);
	if (is_frist_ride == 1){
		INSERT_FLAG(open_flag,ROLE_MOUNT);
	}

	const config_open_mgr* mgr = GET_CONFIG_MGR(config_open_mgr);
	if (mgr != NULL) {
		const config_open* bow_info = mgr->get_config_open(19);
		if (bow_info != NULL){
			if (smy->level() >= static_cast<uint32>(bow_info->get_open_condition())){
				INSERT_FLAG(open_flag,ROLE_BOW);
			}
		}

		const config_open* gem_info = mgr->get_config_open(18);
		if (gem_info != NULL){
			if (smy->level() > static_cast<uint32>(gem_info->get_open_condition())){
				INSERT_FLAG(open_flag,ROLE_GEM);
			}
		}
	}

	role_info->set_open_type(open_flag);

	if (bir->has_opt_flag()){
		rsp.set_opt_flag(bir->opt_flag());
	}

    center_send_client(w, bir->svr_head(), rsp);

    return NULL;
}

static void* center_mount_query_info(client::query_req* bir, center_service_t* w, size_t* rsp_len)
{
    int size = bir->role_ids_size();
    if (size <= 0){
        return NULL;
    }

    uint32 role_id = bir->role_ids(0);
    player_role* pr = center_get_player_role(w, *bir, role_id);
    if (pr == NULL){
        return NULL;
    }

    client::query_rsp rsp;
    rsp.set_type(client::CQTE_ROLE_MOUNT);
	rsp.set_role_id(role_id);
    client::mount_query_info* mount_info = rsp.mutable_mount_info();
	mount_info->set_mount_id(pr->get_mounts().get_db().mount_id());
	mount_info->set_mount_level(pr->get_mounts().get_db().mount_level());
	mount_info->set_mount_power(pr->get_mounts().get_db().mount_power());

	if (bir->has_opt_flag()){
		rsp.set_opt_flag(bir->opt_flag());
	}

    center_send_client(w, bir->svr_head(), rsp);
    return NULL;
}

static void* center_potential_query_info(client::query_req* bir, center_service_t* w, size_t* rsp_len)
{
    int size = bir->role_ids_size();
    if (size <= 0){
        return NULL;
    }

    uint32 role_id = bir->role_ids(0);
    player_role* pr = center_get_player_role(w, *bir, role_id);
    if (pr == NULL){
        return NULL;
    }

    client::query_rsp rsp;
    rsp.set_type(client::CQTE_ROLE_POTENTIAL);
    rsp.set_role_id(role_id);
	client::potential_query_info* potential_info = rsp.add_potential_info();
	const std::map<uint32, client::acupoint_info>& potential_map_ = pr->get_potentials().get_all_potential_info();
	std::map<uint32, client::acupoint_info>::const_iterator it = potential_map_.begin();
	for (;it != potential_map_.end();++it){
		if (it->second.status() != 1){
			continue;
		}

		client::client_point_info* info = potential_info->add_point_info();
		info->set_index(it->second.index());
		info->set_status(it->second.status());
	}

	if (bir->has_opt_flag()){
		rsp.set_opt_flag(bir->opt_flag());
	}

    center_send_client(w, bir->svr_head(), rsp);

    return NULL;
}

static void* center_pet_query_info(client::query_req* bir, center_service_t* w, size_t* rsp_len)
{
    int size = bir->role_ids_size();
    if (size <= 0) {
        return NULL;
    }

    uint32 role_id = bir->role_ids(0);
    player_role* pr = center_get_player_role(w, *bir, role_id);
    if (pr == NULL) {
        return NULL;
    }

    client::query_rsp rsp;
    rsp.set_type(client::CQTE_ROLE_PET);
	client::pet_query_info* pet_info = rsp.mutable_pet_info();
	pr->get_query_pet_info(*pet_info);

    center_send_client(w, bir->svr_head(), rsp);
    
    return NULL;
}

static void* center_bow_query_info(client::query_req* bir, center_service_t* w, size_t* rsp_len)
{
	int size = bir->role_ids_size();
	if (size <= 0) {
		return NULL;
	}

	uint32 role_id = bir->role_ids(0);
	player_role* pr = center_get_player_role(w, *bir, role_id);
	if (pr == NULL) {
		return NULL;
	}

	client::query_rsp rsp;
	rsp.set_type(client::CQTE_ROLE_BOW);
	client::bows_query_info* bow_info = rsp.mutable_bow_info();
	pr->get_query_bow_info(*bow_info);

	center_send_client(w, bir->svr_head(), rsp);

	return NULL;
}

static void* center_gem_query_info(client::query_req* bir, center_service_t* w, size_t* rsp_len)
{
	int size = bir->role_ids_size();
	if (size <= 0) {
		return NULL;
	}

	uint32 role_id = bir->role_ids(0);
	player_role* pr = center_get_player_role(w, *bir, role_id);
	if (pr == NULL) {
		return NULL;
	}

	client::query_rsp rsp;
	rsp.set_type(client::CQTE_ROLE_GEM);
	client::gem_query_info* gem_info = rsp.mutable_gem_info();
	pr->get_query_gem_info(*gem_info);

	center_send_client(w, bir->svr_head(), rsp);

	return NULL;
}

void* on_pb_center_query_req(client::query_req* bir, void* ud, size_t* rsp_len)
{
	switch (bir->type()){
	case client::CQTE_SMY:
    case client::CQTE_SMY_BY_NAME:
		{
			center_query_smy(bir, (center_service_t*)ud, rsp_len);
		}
		break;
    case client::CQTE_ROLE_ATTR:// 查询人物
        {
            center_role_query_info(bir, (center_service_t*)ud, rsp_len);
        }
        break;
    case client::CQTE_ROLE_MOUNT:// 查询坐骑
        {
             center_mount_query_info(bir, (center_service_t*)ud, rsp_len);
        }
        break;
    case client::CQTE_ROLE_POTENTIAL:// 查询心法
        {
            center_potential_query_info(bir, (center_service_t*)ud, rsp_len);
        }
        break;
    case client::CQTE_ROLE_PET:// 查询美人(当前出战那个)
        {
            center_pet_query_info(bir, (center_service_t*)ud, rsp_len);
        }
        break;
	case client::CQTE_ROLE_BOW: // 查询弓箭信息
		{
			center_bow_query_info(bir, (center_service_t*)ud, rsp_len);
		}
		break;
	case client::CQTE_ROLE_GEM: // 查询宝石系统
		{
			center_gem_query_info(bir, (center_service_t*)ud, rsp_len);
		}
	default:
		break;
	}

	return NULL;
}

#if 0
void* on_center_pb_update_smy_role_lvl(client::update_smy_role_lvl* req, void* ud, size_t* rsp_len)
{
    center_service_t* w = (center_service_t*)ud;
    w->aus_mgr.update_lvl(req->role_id(), req->level());
    if (req->level() >= ARENA_MIN_LVL &&  w->ar_mgr.is_not_add(req->role_id())){
		w->ar_mgr.add_role(req->role_id(), ARENA_PLAYER);
    }
    return NULL;
}

void *on_center_pb_update_smy_role_fight_power(client::update_smy_role_fight_power *req, void *ud, size_t *rsp)
{
	center_service_t *w = (center_service_t *)ud;
	w->aus_mgr.update_fight_power(req->role_id(), req->fight_power());
	return NULL;
}

void *on_center_pb_update_smy_mount_data(client::update_smy_mount_data *req, void *ud, size_t *rsp)
{
	center_service_t *w = (center_service_t *)ud;

	w->aus_mgr.update_mount_info(req->role_id(),req->mount_level(),req->mount_power());

	return NULL;
}
#endif

void *on_center_pb_update_smy_info(client::update_smy_info *req, void *ud, size_t *rsp)
{
	center_service_t *w = (center_service_t *)ud;
	switch(req->type())
	{
		case client::SMY_ROLE_LEVEL:
			w->aus_mgr.update_lvl(req->role_id(), req->val1());
			break;
		case client::SMY_ROLE_FIGHT_POWER:
			w->aus_mgr.update_fight_power(req->role_id(), req->val1());
			break;
		case client::SMY_MOUNT_DATA:
			w->aus_mgr.update_mount_info(req->role_id(), req->val1(), req->val2());
			break;
		case client::SMY_ROLE_VIP:
			w->aus_mgr.update_vip_level(req->role_id(), req->val1());
			break;
		case client::SMY_ROLE_WEAPON_ID:
			w->aus_mgr.update_weapon_id(req->role_id(), req->val1());
			break;
		case client::SMY_ROLE_ARMOR_ID:
			w->aus_mgr.update_armor_id(req->role_id(), req->val1());
			break;
		default:
			break;
	}
	return NULL;
}

void* on_pb_gamet_to_center_query_req(client::game_to_center_query* bir, void* ud, size_t* rsp_len)
{
	center_service_t *w = (center_service_t *)ud;
	client::query_rsp rsp_msg;

	client::role_query_info* info = rsp_msg.add_role_info();
	const client::query_rsp& qr = bir->query_info();
	for (int i = 0; i < qr.role_info_size(); ++i){
		info->CopyFrom(qr.role_info(i));
	}

	rsp_msg.set_type(qr.type());
	w->cf_mgr->fill_query_family_base(bir->query_id(),*info);

	center_send_client(w, bir->role_id(), rsp_msg);

	return NULL;
}