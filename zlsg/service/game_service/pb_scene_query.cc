#include "game_interface.h"
#include "player_role.h"
#include "player_role_props.h"
#include "player_role_skill_mgr.h"
#include "scene_user.h"
#include "scene_inst.h"
#include "player_role_mounts.h"
#include "player_role_potential.h"
#include "player_role_statis_mgr.h"

static scene_user* get_query_scene_user(game_service_t* ss, client::query_req* bir)
{
    int size = bir->role_ids_size();
    if (size <= 0){
        return NULL;
    }

    uint32 role_id = bir->role_ids(0);
    return ss->sim->find_user(role_id);
}

static void* game_role_query_info(scene_user* su, client::query_req* bir, game_service_t* s, size_t* rsp_len)
{
    client::game_to_center_query rsp;
    //player_role& pr = su->get_player_role();
	rsp.set_role_id(bir->svr_head().client_id());
	rsp.set_query_id(su->get_role_id());
    client::query_rsp* query_info = rsp.mutable_query_info();
    query_info->set_type(client::CQTE_ROLE_ATTR);
	player_role& pr = su->get_player_role();
	client::role_query_info* role_info = query_info->add_role_info();
	role_info->set_role_id(su->get_role_id());
	role_info->set_role_name(su->get_role_name());

	role_info->set_fight_power(su->get_attr(FIGHTING_POWER));
	role_info->set_profession(su->get_attr(PROFESSION));
	role_info->set_level(su->get_attr(LEVEL));
	role_info->set_weapon_id(su->get_attr(WEAPON_ID));
	role_info->set_armor_id(su->get_attr(ARMOR_ID));
	role_info->set_vip_level(su->get_attr(ROLE_VIP_LEVEL));
	pr.get_query_role_info(*role_info);
	uint32 open_flag = 0;
	client::pet_db_info pet_info;
	if (pr.get_pets().get_query_fighting_pet(pet_info)){
		INSERT_FLAG(open_flag,ROLE_PET);
	}

	if (pr.get_potentials().get_all_potential_info().size() != 0){
		INSERT_FLAG(open_flag,ROLE_POTENTIAL);
	}

	uint32 is_frist_ride = 0;
	pr.get_role_statis_mgr().get(PRSTE_MOUNT_FRIST_RIDE)->get_val(is_frist_ride);

	if (is_frist_ride == 1){
		INSERT_FLAG(open_flag,ROLE_MOUNT);
	}

	role_info->set_open_type(open_flag);

	if (bir->has_opt_flag()){
		query_info->set_opt_flag(bir->opt_flag());
	}

	game_send_center(s,rsp);

    return NULL;
}

static void* game_mount_query_info(scene_user* su, client::query_req* bir, game_service_t* s, size_t* rsp_len)
{
	client::query_rsp rsp;
	player_role& pr = su->get_player_role();

	rsp.set_type(client::CQTE_ROLE_MOUNT);
	rsp.set_role_id(su->get_role_id());
	client::mount_query_info* mount_info = rsp.mutable_mount_info();
	mount_info->set_mount_id(pr.get_mounts().get_db().mount_id());
	mount_info->set_mount_level(pr.get_mounts().get_db().mount_level());
	mount_info->set_mount_power(pr.get_mounts().get_db().mount_power());

	if (bir->has_opt_flag()){
		rsp.set_opt_flag(bir->opt_flag());
	}

	game_send_client(s, bir->svr_head().client_id(), rsp);

	return NULL;
}

static void* game_potential_query_info(scene_user* su, client::query_req* bir, game_service_t* s, size_t* rsp_len)
{
	client::query_rsp rsp;
	player_role& pr = su->get_player_role();

	rsp.set_type(client::CQTE_ROLE_POTENTIAL);
	rsp.set_role_id(su->get_role_id());
	client::potential_query_info* potential_info = rsp.add_potential_info();
	const std::map<uint32, client::acupoint_info>& potential_map_ = pr.get_potentials().get_all_potential_info();
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

	game_send_client(s, bir->svr_head().client_id(), rsp);

	return NULL;
}

static void* game_pet_query_info(scene_user* su, client::query_req* bir, game_service_t* s, size_t* rsp_len)
{
	int size = bir->role_ids_size();
	if (size < 1){
		return NULL;
	}

	player_role& pr = su->get_player_role();

	client::query_rsp rsp;
	rsp.set_type(client::CQTE_ROLE_PET);

	uint32 role_id = bir->role_ids(0);
	rsp.set_role_id(role_id);

	client::pet_query_info* pet_info = rsp.mutable_pet_info();
	pr.get_query_pet_info(*pet_info);

	if (bir->has_opt_flag()){
		rsp.set_opt_flag(bir->opt_flag());
	}

	game_send_client(s, bir->svr_head().client_id(), rsp);

	return NULL;
}

static void* game_bow_query_info(scene_user* su, client::query_req* bir, game_service_t* s, size_t* rsp_len)
{

	int size = bir->role_ids_size();
	if (size < 1){
		return NULL;
	}

	player_role& pr = su->get_player_role();

	client::query_rsp rsp;
	rsp.set_type(client::CQTE_ROLE_PET);

	uint32 role_id = bir->role_ids(0);
	rsp.set_role_id(role_id);

	client::bows_query_info* bow_info = rsp.mutable_bow_info();
	pr.get_query_bow_info(*bow_info);

	if (bir->has_opt_flag()){
		rsp.set_opt_flag(bir->opt_flag());
	}

	game_send_client(s, bir->svr_head().client_id(), rsp);

	return NULL;
}

static void* game_gem_query_info(scene_user* su, client::query_req* bir, game_service_t* s, size_t* rsp_len)
{
	int size = bir->role_ids_size();
	if (size < 1){
		return NULL;
	}

	player_role& pr = su->get_player_role();

	client::query_rsp rsp;
	rsp.set_type(client::CQTE_ROLE_GEM);

	uint32 role_id = bir->role_ids(0);
	rsp.set_role_id(role_id);

	client::gem_query_info* gem_info = rsp.mutable_gem_info();
	pr.get_query_gem_info(*gem_info);

	if (bir->has_opt_flag()){
		rsp.set_opt_flag(bir->opt_flag());
	}

	game_send_client(s, bir->svr_head().client_id(), rsp);

	return NULL;
}

void* on_pb_query_req(client::query_req* bir, void* ud, size_t* rsp_len)
{
    scene_user* su = get_query_scene_user((game_service_t*)ud, bir);
    if (su == NULL){
        return NULL;
    }

	struct game_service_t* ss = (struct game_service_t*)ud;

	switch (bir->type()){
	case client::CQTE_ROLE_ATTR:
		{
			game_role_query_info(su, bir, (game_service_t*)ud, rsp_len);
		}
		break;
	case client::CQTE_ROLE_MOUNT:
		{
			game_mount_query_info(su, bir, (game_service_t*)ud, rsp_len);
		}
		break;
	case client::CQTE_ROLE_POTENTIAL:
		{
			game_potential_query_info(su, bir, (game_service_t*)ud, rsp_len);
		}
		break;
	case client::CQTE_ROLE_PET:
		{
			game_pet_query_info(su, bir, (game_service_t*)ud, rsp_len);
		}
		break;
	case client::CQTE_ROLE_BOW:
		{
			game_bow_query_info(su, bir, (game_service_t*)ud, rsp_len);
		}
		break;
	case client::CQTE_ROLE_GEM: // 查询宝石系统
		{
			game_gem_query_info(su, bir, (game_service_t*)ud, rsp_len);
		}
		break;
	default:
		break;
	}

	amtsvr_billlog(ss->ctx, "%u|%s|%u", su->get_role_id(), bir->GetTypeName().c_str(), bir->type());

    return NULL;
}


