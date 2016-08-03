#include "md5_encrypt.h"
#include "config_npc.h"
#include "config_raid.h"
#include "config_map.h"
#include "player_role.h"
#include "player_role_auto_raid.h"
#include "player_role_raid_mgr.h"
#include "player_role_statis_mgr.h"
#include "client.operate_mail.pb.h"
#include "scene_user.h"
#include "scene_inst.h"
#include "game_interface.h"
#include "player_role_levels.h"
#ifndef __LIB_MINI_SERVER__
#include "player_role_devil.h"
#include "config_devil.h"
#include "client.devil.pb.h"
#include "config_vip.h"
#endif

#define ELITE_RAID_COUNT(V) (V >> 16)
#define NORMAL_RAID_COUNT(V) (V & 0x0000FFFF)

#define BASIC_ELITE_RAID_ENTER_MAX 10144
#define BASIC_TEAM_RAID_ENTER_MAX 10182

#define BASIC_PET_RAID_ENTER_MAX 10184

uint32 get_pet_raid_enter_times(scene_user *psu, const config_raid *pcr)
{
    uint32 father_id = pcr->get_father_map_id();
    const std::map<uint32, player_role_raid>& raids = psu->get_player_role().get_raid_mgr().get_all_raids();
    map<uint32, player_role_raid>::const_iterator it = raids.begin();
    uint32 num = 0;
    for(; it != raids.end(); ++it)
    {
        const config_raid *cr = CONFMGR->get_config_raid(it->first);
        if(cr == NULL)
        {
            continue;
        }
        if(cr->get_map_type() != MAP_TYPE_PET)
        {
            continue;
        }
        if(cr->get_father_map_id() != father_id)
        {
            continue;
        }
        num += it->second.get_elite_raid_enter_count();
    }
    return num;
}

int check_devil_raid(uint32 raid_id, scene_user *psu)
{
#ifndef __LIB_MINI_SERVER__
	if(!psu->has_func_open(DEVIL_FUNC_ID))
	{
		return (int)client::func_closed;
	}
	const config_devil *cd = CONFMGR->get_config_devil(raid_id);
	if(NULL == cd)
	{
		return (int)client::invalid_id;
	}
	return 0;
#else
    return (int)client::failed;
#endif
}

int check_enter_raid(uint32 raid_id, uint32 raid_type, scene_user* psu, bool enter /* = false */)
{
#ifndef __LIB_MINI_SERVER__
    const config_raid* cr = CONFMGR->get_config_raid(raid_id);
    if (cr == NULL) 
	{
        return 1;
    }
    uint32 need_energy = 0;
	uint32 map_type = cr->get_map_type();
	player_role &pr = psu->get_player_role();
    if(map_type == MAP_TYPE_DEVIL)
    {
        int ret = check_devil_raid(raid_id, psu);
        if(ret != 0)
        {
            return ret;
        }
    }
	else if(map_type == MAP_TYPE_LEVELS)
    {
        if(!enter)
        {
            player_role_levels& prl = pr.get_player_role_levels();
            if(prl.levels_need_reset())
            {
                return 8;
            }
            need_energy = cr->get_need_energy();
        }
    }
    else
    {
        if (raid_type == 0) 
		{
            need_energy = cr->get_need_energy() - cr->get_fail_need_energy_1();
        } 
		else
		{
            need_energy = cr->get_d_2_need_energy() - cr->get_fail_need_energy_2();
        }
        if (enter) 
		{
            if (raid_type == 0) 
			{
                need_energy -= cr->get_fail_need_energy_1();
            }
			else 
			{
                need_energy -= cr->get_fail_need_energy_2();
            }
        }
    }
    if (psu->get_attr(PHYSICAL_POWER) < need_energy) 
	{
        return 2;
    }
    if (psu->get_attr(LEVEL) < cr->get_enter_lv()) 
	{
        return 3;
    }
	if(map_type == MAP_TYPE_PET)
	{
        uint32 need_pet = cr->get_map_monster_id();
        if(!psu->has_pet(need_pet))  // 必须要招募到该美人
		{
            return 4;
        }
    }
	uint32 ex_raid = cr->get_ex_map_id();
	if(ex_raid != 0)
	{
		if(map_type == MAP_TYPE_DEVIL)
		{
			player_role_devil &prd = pr.get_role_devil();
			if(!prd.raid_is_complete(ex_raid))
			{
				return 5;
			}
		}
		else
		{
			player_role_raid *prr = pr.get_raid_mgr().mutable_raid(ex_raid);
			if(prr == NULL) 
			{
				return 5;
			}
		}
	}
    if (raid_type == client::enter_raid_req_type_elite) 
	{
        player_role_raid* prr = pr.get_raid_mgr().mutable_raid(raid_id);
        if(prr == NULL || prr->mutable_raid() == NULL) 
		{
			if(map_type != MAP_TYPE_PET && map_type != MAP_TYPE_TEAM)
			{
				return 7;
			}
		}
		uint32 count = 0;
		uint32 used = 0;
		switch(map_type)
		{
		case MAP_TYPE_PET:
			count = CONFMGR->get_basic_value(BASIC_PET_RAID_ENTER_MAX);
			used  = get_pet_raid_enter_times(psu, cr);
			break;
		case MAP_TYPE_TEAM:
			count = CONFMGR->get_basic_value(BASIC_TEAM_RAID_ENTER_MAX);
			used = prr == NULL ? 0 : prr->get_elite_raid_enter_count();
			break;
		case MAP_TYPE_RAID:
			count = CONFMGR->get_basic_value(BASIC_ELITE_RAID_ENTER_MAX);
			used = prr->get_elite_raid_enter_count();
			break;
		default:
			return 6;
		}
		if(count <= used)
		{
			return 6;
		}
	}
    return 0;
#else
	return 1;
#endif
}

void* on_pb_enter_raid_req(client::enter_raid_req* req, void* ud, size_t* rsp_len)
{
    struct game_service_t* ss = (struct game_service_t*)ud;
    scene_user* su = ss->sim->get_user(req->svr_head().role_id());
    if (su == NULL) {
        return NULL;
    }

    scene_inst* psi = su->get_scene();
    if (psi == NULL || psi->get_scene_type() != MAP_TYPE_NORMAL) {
        return NULL;
    }

    uint32 raid_id  = req->raid_id();
    const config_map* cm = CONFMGR->get_config_map(raid_id);
    if (cm == NULL) {
        return NULL;
    }

    const config_raid* cr = CONFMGR->get_config_raid(raid_id);
    if (cr == NULL) {
        return NULL;
    }

    if(su->get_attr().get_val(LEVEL) < cr->get_enter_lv()){
        return NULL;
    }

    if(cr->get_map_type() == MAP_TYPE_PET)			// 美人关
    {
        uint32 need_pet = cr->get_map_monster_id();
        if(!su->has_pet(need_pet))				// 必须要招募到该美人
        {
            return NULL;
        }
        uint32 cur_pet = su->get_current_pet();
        if(cur_pet != 0 && cur_pet != need_pet)		// 单人或者携带该美人
        {
            return NULL;
        }
    }

    uint32 type = 0;
    if (req->has_t()) {
        type = req->t();
        su->set_raid_type(type);
    }

    uint32 need_energy = 0;
    if (type == 0) {
        need_energy = cr->get_need_energy();
    } else {
        need_energy = cr->get_d_2_need_energy();
    }

    if (su->get_attr().get_val(PHYSICAL_POWER) < need_energy) {
        return NULL;
    }

    su->mutable_attr()->sub_val(PHYSICAL_POWER, need_energy);
    su->send_client_change_attr();

    // 保存跳转副本前的地图位置
    su->save_formal_pos();

    client::jump_user ju;
    uintptr_t ptr = (uintptr_t)su;
    ju.set_ptr((uint64)ptr);
    ju.set_role_id(su->get_role_id());
    ju.set_map_id(raid_id);
    ju.set_scene_id(raid_id);
    ju.set_trans_id(raid_id);
    ju.set_x(cm->get_birth_point_x());
    ju.set_y(cm->get_birth_point_y());
    ju.set_z(0);
    ju.set_same_scene(su->is_same_map_and_scene(ju.map_id(), ju.scene_id()));

    ss->sim->logout_scene(su, su->get_scene());
    su->send_center(ss->ctx, ju);

    amtsvr_billlog(ss->ctx, "%u|%s|%u", su->get_role_id(), req->GetTypeName().c_str(), req->raid_id());

    return NULL;
}

void game_exit_raid_cmd(struct game_service_t* ss, uint32 role_id)
{
    scene_user* psu = ss->sim->get_user(role_id);
    if (psu == NULL) {
        return;
    }

    bool leave_scene = false;
    client::jump_user ju;
    ju.set_role_id(psu->get_role_id());;
    scene_inst* psi = psu->get_scene();
    if (psi == NULL || psi->get_scene_type() == MAP_TYPE_NORMAL) {
        return ;
    }

    psu->fill_back_trans_jump(ju);
    leave_scene = true;

    if (leave_scene) {
        ss->sim->logout_scene(psu, psi);
        psu->send_center(ss->ctx, ju);
    }
}

void* on_pb_exit_raid_req(client::exit_raid_req* req, void* ud, size_t* rsp_len)
{
    struct game_service_t* ss = (struct game_service_t*)ud;
    uint32 role_id = req->svr_head().role_id();

    game_exit_raid_cmd(ss, role_id);

    amtsvr_billlog(ss->ctx, "%u|%s|%u", role_id, req->GetTypeName().c_str(), req->raid_id());

    return NULL;
}

void* on_pb_raid_open_box_req(client::raid_open_box_req* req, void* ud, size_t* rsp_len)
{
    PB_FUNC_GUARD(req);

    scene_inst* psi = psu->get_scene();
    if (psi == NULL || psi->get_scene_type() != MAP_TYPE_RAID) {
        return NULL;
    }

    const config_raid* cr = psi->get_map().get_config_raid();
    if (cr == NULL) {
        return NULL;
    }

    const std::vector<int> rewards = cr->get_reward_item();
    if (rewards.empty()) {
        return NULL;
    }

    client::raid_open_box_rsp rsp;
    rsp.add_open_props_ids(rewards[0]);
    for (size_t i = 1; i < rewards.size(); ++i) {
        psu->add_props(rewards[0], 1, 1, 0, false);
        rsp.add_close_props_ids(rewards[i]);
    }
    psu->send_client(rsp);
    return NULL;
}


void* on_pb_team_raid_enter_cmd(client::team_raid_enter_cmd* cmd, void* ud, size_t* rsp_len)
{
    struct game_service_t* ss = (struct game_service_t*)ud;
    uint32 role_id = cmd->role_id();
    scene_user* su = ss->sim->get_user(role_id);
    if (su == NULL) {
        return NULL;
    }

    const config_map* cm = CONFMGR->get_config_map(cmd->raid_id());
    if (cm == NULL) {
        return NULL;
    }
    su->set_team_id(cmd->team_id());
    su->set_raid_type(client::enter_raid_req_type_elite);
    bool leave_scene = true;
    su->save_formal_pos();
    client::jump_user ju;
    ju.set_role_id(su->get_role_id());
    uintptr_t ptr = (uintptr_t)su;
    ju.set_ptr((uint64)ptr);
    ju.set_map_id(cmd->raid_id());
    ju.set_scene_id(cmd->raid_id());
    ju.set_trans_id(cmd->raid_id());
    ju.set_x(cm->get_birth_point_x());
    ju.set_y(cm->get_birth_point_y());
    ju.set_z(0);
    ju.set_same_scene(su->is_same_map_and_scene(ju.map_id(), ju.scene_id()));
    amtsvr_billlog(ss->ctx, "%u|%s|%u|%u|%u", su->get_role_id(), cmd->GetTypeName().c_str(), cmd->role_id(), cmd->team_id(), cmd->raid_id());
    if (leave_scene) {
        ss->sim->logout_scene(su, su->get_scene());
        su->send_center(ss->ctx, ju);
    }
    return NULL;
}

bool set_auto_raid(client::auto_raid_req* req, scene_user* psu)
{
#ifndef __LIB_MINI_SERVER__
    if (req == NULL || psu == NULL) 
	{
        return false;
    }
    if (!psu->is_can_add_props())
	{
        return false;
    }
	player_role &pr = psu->get_player_role();
    psu->mutable_status()->set_raid(true);
    const config_raid* pcr = CONFMGR->get_config_raid(req->id());
    if (pcr == NULL) 
	{
        return false;
    }
	uint32 map_type = pcr->get_map_type();
    uint32 type = req->has_t() ? req->t(): 0;
	player_role_raid *prr = pr.get_raid_mgr().mutable_raid(req->id());
	if(map_type == MAP_TYPE_DEVIL)
	{
		player_role_devil &prd = pr.get_role_devil();
		if(prd.get_reward_type(req->id()) != 1)
		{
			return false;
		}
		if(!prd.raid_is_perfect(req->id()))
		{
			return false;
		}
	}
	else
	{
		if (prr == NULL || prr->mutable_raid() == NULL) 
		{
			return false;
		}
		uint32 star = type == 1 ? prr->get_elite_star_num() : prr->get_star_num();
		if(star != 1)
		{
			return false;
		}
	}
	uint32 need_energy = type == 0 ? pcr->get_need_energy() : pcr->get_d_2_need_energy();
	if (psu->get_attr().get_val(PHYSICAL_POWER) < need_energy) 
	{
		return false;
	}
	if (type == client::enter_raid_req_type_elite)
	{
		uint32 count = 0;
		uint32 used = 0;
		switch(map_type)
		{
		case MAP_TYPE_PET:
			count = CONFMGR->get_basic_value(BASIC_PET_RAID_ENTER_MAX);
			used = get_pet_raid_enter_times(psu, pcr);
			break;
		case MAP_TYPE_RAID:
			count = CONFMGR->get_basic_value(BASIC_ELITE_RAID_ENTER_MAX);
			used = prr->get_elite_raid_enter_count();
			break;
		default:
			return false;
		}
		if(count <= used)
		{
			return false;
		}
	}
	if(map_type == MAP_TYPE_RAID || map_type == MAP_TYPE_PET)
	{
		uint32 auto_count = 0;
		pr.get_role_statis_mgr().get(PRSTE_AUTO_RAID_COUNT)->get_val(auto_count);
		const config_vip *cv = CONFMGR->get_config_vip(psu->get_vip_lv());
		uint32 add = 0;
		if(cv != NULL)
		{
			add = (uint32)cv->_sweep;
		}
		if(add <= (ELITE_RAID_COUNT(auto_count) + NORMAL_RAID_COUNT(auto_count)))
		{
			return false;
		}
		prr->set_auto(type);
	}
	pr.get_auto_raid()->set_auto_raid(req->id(), 0, req->num(), type);
    psu->mutable_attr()->sub_val(PHYSICAL_POWER, need_energy);
    switch(pcr->get_map_type())
    {
    case MAP_TYPE_RAID:
		CHECK_TARGET_PROGRESS(psu, PASS_COPY, req->id(), 3, type, req->num());
        break;
    case MAP_TYPE_PET:
		CHECK_TARGET_PROGRESS(psu, PASS_PET_RAID, req->id(), req->num());
        break;
    default:
        break;
    }
    psu->send_client_change_attr();
	return true;
#else
	return false;
#endif
}

bool stop_auto_raid(scene_user* psu)
{
#ifndef __LIB_MINI_SERVER__
    if (psu == NULL) {
        return false;
    }

    player_role_auto_raid* auto_raid = psu->get_player_role().get_auto_raid();
    if (auto_raid == NULL || auto_raid->get().id() == 0) {
        return false;
    }

    psu->mutable_status()->set_raid(false);

    client::auto_raid_info& info = auto_raid->get();
    psu->add_exp(info.exp() + psu->add_family_exp(info.exp()));
    psu->add_money(TAEL, info.coin());
    psu->add_money(MILITARY_STRATEGY, info.military_strategy());
    psu->add_money(PHYSICAL_POWER, info.frozen_physical_power());
    psu->add_props(0, info.frozen_silver());
    vector<client::props_info> vec_prop;
    vec_prop.clear();
    for (int32 props_index = 0; props_index < info.props_size(); ++props_index) {
        vec_prop.push_back(info.props(props_index));
    }

    psu->add_props(vec_prop);

    info.Clear();
    info.set_id(0);

    psu->send_client_change_attr();
    psu->send_client_change_props_msg();
#endif
    return true;
}

void get_raid_reward(const config_raid* cr,  uint32 type, scene_user* psu)
{
#ifndef __LIB_MINI_SERVER__
    uint32 need_energy       = 0;
    uint32 reward_money      = 0;
    uint32 reward_experience = 0;
    uint32 reward_tao_lue    = 0;
    const std::vector<int>* reward_item = NULL;
    if (type == 0) {
        need_energy       = cr->get_need_energy();
        reward_money      = cr->get_reward_money();
        reward_experience = cr->get_reward_experience();
        reward_tao_lue    = cr->get_reward_tao_lue();
        reward_item       = &cr->get_reward_item();
    } else {
        need_energy       = cr->get_d_2_need_energy();
        reward_money      = cr->get_reward_money_2();
        reward_experience = cr->get_reward_experience_2();
        reward_tao_lue    = cr->get_reward_tao_lue();
        reward_item       = &cr->get_reward_item_2();
    }
	std::vector<client::props_info> v;
	for(size_t i = 0; i < reward_item->size(); ++i)
	{
		make_props(reward_item->at(i), v, psu->get_job());
	}
	psu->add_props(v);
    psu->mutable_attr()->sub_val(PHYSICAL_POWER, need_energy);
    psu->add_money(TAEL, reward_money);
    psu->add_exp(reward_experience + psu->add_family_exp(reward_experience));
    psu->add_money(MILITARY_STRATEGY, reward_tao_lue);
    psu->send_client_change_attr();
#endif
}

void* on_pb_auto_raid_req(client::auto_raid_req* req, void* ud, size_t* rsp_len)
{
#ifndef __LIB_MINI_SERVER__
	PB_FUNC_GUARD(req);
	if (req->id() == 0) 
	{
		return NULL;
	}
	client::auto_raid_rsp rsp;
	bool ret = set_auto_raid(req, psu);
	if (ret) 
	{
		rsp.set_id(req->id());
		psu->send_client(rsp);
		psu->auto_raid();
		const config_raid* pcr = CONFMGR->get_config_raid(req->id());
		if(pcr->get_map_type() == MAP_TYPE_DEVIL)
		{
			player_role_devil &prd = psu->get_player_role().get_role_devil();
			prd.add_raid_info(req->id(), 3, -1, 0);
			client::devil_raid_detail drd;
			prd.fill_devil_raid_info(req->id(), drd);
			psu->send_client(drd);
		}
		else
		{
			if(pcr->get_map_type() == MAP_TYPE_RAID)
			{
				uint32 auto_count = 0;
				psu->get_player_role().get_role_statis_mgr().get(PRSTE_AUTO_RAID_COUNT)->get_val(auto_count);
				uint32 enter_count = 0;
				psu->get_player_role().get_role_statis_mgr().get(PRSTE_ENTER_RAID_COUNT)->get_val(enter_count);
				if (req->has_t() && req->t()) 
				{
					auto_count  += 0x00010000;
					enter_count += 0x00010000;
				}
				else
				{
					req->set_t(0);
					auto_count  += 1;
					enter_count += 1;
				}
				psu->get_player_role().get_role_statis_mgr().get(PRSTE_AUTO_RAID_COUNT)->set_val(auto_count);
				psu->get_player_role().get_role_statis_mgr().get(PRSTE_ENTER_RAID_COUNT)->set_val(enter_count);
				client::role_statis_rsp statis_rsp;
				psu->get_player_role().get_role_statis_mgr().get(PRSTE_AUTO_RAID_COUNT)->fill(statis_rsp);
				psu->get_player_role().get_role_statis_mgr().get(PRSTE_ENTER_RAID_COUNT)->fill(statis_rsp);
				psu->send_client(statis_rsp);
			}
			psu->get_player_role().get_raid_mgr().push(psu->get_role_id(), req->id(), 3, -1, (uint32)time(NULL), req->t());
			client::raid_info_msg rim;
			psu->get_player_role().get_raid_mgr().fill_raid_info_msg(rim, req->id());
			psu->send_client(rim);
		}
	}
	else
	{
		rsp.set_id(0);
		psu->send_client(rsp);
	}

	amtsvr_billlog(ss->ctx, "%u|%s|%u|%u|%u", req->svr_head().role_id(), req->GetTypeName().c_str(), req->id(), req->t(), rsp.id());
#endif
    return NULL;
}

#define OP_TYPE_ENTER 1

void* on_pb_mini_server_req(client::mini_server_req* req, void* ud, size_t* rsp_len)
{
#ifndef __LIB_MINI_SERVER__
    struct game_service_t* ss = (struct game_service_t*)ud;
    if (ss == NULL) {
        return NULL;
    }

    amtsvr_billlog(ss->ctx, "%u|%s|%u|%u|%u", req->svr_head().role_id(), req->GetTypeName().c_str(), req->op(), req->raid_id(), req->raid_type());

    scene_user* psu = NULL;
    uint32 ret = 0;
    if (req->op() == OP_TYPE_ENTER) {
        psu = ss->sim->get_user(req->svr_head().role_id());
        if (psu == NULL) {
            return NULL;
        }

        if (psu->get_scene()->get_scene_type() != MAP_TYPE_NORMAL) {
            return NULL;
        }

        ret = check_enter_raid(req->raid_id(), req->raid_type(), psu, false);
        if (ret == 0) {

            const config_raid* pcr = CONFMGR->get_config_raid(req->raid_id());
            if (pcr == NULL) {
                return NULL;
            }
            if(pcr->get_map_type() == MAP_TYPE_LEVELS)
            {
                psu->mutable_attr()->sub_val(PHYSICAL_POWER, pcr->get_need_energy());
            }
            else
            {
                if (req->raid_type() == client::enter_raid_req_type_normal) {
                    psu->mutable_attr()->sub_val(PHYSICAL_POWER, pcr->get_fail_need_energy_1());
                } else {
                    psu->mutable_attr()->sub_val(PHYSICAL_POWER, pcr->get_fail_need_energy_2());
                }
            }

            psu->send_client_change_attr();

            if(pcr->get_map_type() == MAP_TYPE_PET) {
				CHECK_TARGET_PROGRESS(psu, PASS_PET_RAID, req->raid_id(), 1);
            }
            psu->enter_mini_server(req->raid_id(), (uint32)time(NULL));
            ss->sim->go_to_space(psu);
        }
        client::mini_server_rsp rsp;
        rsp.set_op(req->op());
        rsp.set_raid_id(req->raid_id());
        if (req->has_raid_type()) {
            rsp.set_raid_type(req->raid_type());
        }
        rsp.set_ret(ret);
        rsp.set_time((uint32)time(NULL));
        psu->send_client(rsp);
    } else {
        psu = ss->sim->get_space_user(req->svr_head().role_id());
        if (psu == NULL) {
            return NULL;
        }

        ss->sim->go_to_scene(psu);
        psu->set_scene_ready(true);
        if (psu->get_scene() != NULL) {
            psu->get_scene()->screen_push_user(psu);
        }
    }
#endif
    return NULL;
}

bool check_key(const reported_raid_info& rri)
{
	char str_buf[128]= {0};
	snprintf(str_buf, sizeof(str_buf), "%d%d%d%d%d%d%s%d%d%d", rri.id(), rri.star(), rri.used_tick(), rri.complete_time(), rri.type(), rri.time(), "chebazi", rri.verify().max_hp(), rri.verify().max_atk(), rri.verify().max_def());

	unsigned char digest[17] = {0};
	MD5_CTX md5;
	MD5Init(&md5);
	MD5Update(&md5, (unsigned char*)str_buf, strlen(str_buf));
	MD5Final(&md5, digest);

	char ret[64] = {0};
	//     int result = 0;
	for (int i = 0, j = 0; i < 32; i += 2) {
		sprintf(ret + i, "%02x", digest[j++]);
	}

	//printf("md5 [%s]->[%s] req_sign:%s\n", str_buf, ret, req_sign);
	if (memcmp(ret, rri.key().c_str(), 32) != 0) {
		return false;
	}

	return true;
}

#include "config_giantraid.h"
#include "config_upgrade.h"

void devil_complete(game_service_t *s, const reported_raid_info &rri, scene_user *psu, client::raid_result_msg &msg)
{
#ifndef __LIB_MINI_SERVER__
	player_role_devil &prd = psu->get_player_role().get_role_devil();
	client::devil_fight_over req;
	req.set_role_id(psu->get_role_id());
	req.set_raid_id(rri.id());
	req.set_star(rri.star());
	req.set_tick(rri.used_tick());
	req.set_champion(prd.raid_is_champion(rri.id()));
	if(rri.has_success())
	{
		client::raid_success_info *success = req.mutable_success();
		success->CopyFrom(rri.success());
	}
	if(rri.has_failure())
	{
		client::raid_failure_info *failure = req.mutable_failure();
		failure->CopyFrom(rri.failure());
	}
	game_send_center(s, req);
#endif
}

#ifndef __LIB_MINI_SERVER__
void *on_pb_game_devil_fight_result(client::devil_fight_result *rsp, void *ud, size_t *rsp_len)
{

	struct game_service_t* ss = (struct game_service_t*)ud;
	scene_user *psu = ss->sim->get_space_user(rsp->role_id());
	if(NULL == psu)
	{
		psu = ss->sim->get_user(rsp->role_id());
		if(psu == NULL)
		{
			return NULL;
		}
	}
    player_role_devil &prd = psu->get_player_role().get_role_devil();
	const config_devil *cd = CONFMGR->get_config_devil(rsp->raid_id());
	if(NULL == cd)
	{
		return NULL;
	}
	const config_raid *cr = CONFMGR->get_config_raid(rsp->raid_id());
	if(NULL == cr)
	{
		return NULL;
	}
	uint32 reward_type = prd.get_reward_type(rsp->raid_id());
	client::raid_result_msg msg;
	msg.Clear();
	msg.set_raid_id(rsp->raid_id());
	msg.set_star(rsp->star());
	msg.set_tick(rsp->used_tick());
	if(rsp->has_role_name())
	{
		msg.set_value1(rsp->role_name());
	}
	if(rsp->has_tick())
	{
		msg.set_value2(rsp->tick());
	}
	client::raid_result_info *info = NULL;
#if 0
	std::multimap<uint32, drop_prop> drops;
#endif
	std::vector<client::props_info> v;
	switch(reward_type)
	{
	case 0:		// 首次通关奖励
		make_props(cd->get_1st_reward(), v, psu->get_job());
		info = msg.add_reward1();
		psu->fill_role_data(info->mutable_rd());
		break;
	case 1:		// 再次通关奖励
		make_props(cd->get_again_reward(), v, psu->get_job());
		info = msg.add_info();
		psu->fill_role_data(info->mutable_rd());
		break;
	default:
		break;
	}
	for(size_t i = 0; i < v.size(); ++i)
	{
		client::props_info *pi = info->add_props();
		pi->CopyFrom(v[i]);
	}
	psu->add_props(v);
	v.clear();
	if(rsp->champion() == 1)	// 最快通关奖励
	{
		uint32 id = cd->get_fast_reward();
		if(0 != id)
		{
			client::raid_result_info* fast = msg.add_reward2();
			psu->fill_role_data(fast->mutable_rd());
			make_props(id, v, psu->get_job());
			for(size_t i = 0; i < v.size(); ++i)
			{
				client::props_info *pi = fast->add_props();
				pi->CopyFrom(v[i]);
			}
			psu->add_props(v);
		}
	}
    if (rsp->has_success()) {
        msg.mutable_success()->CopyFrom(rsp->success());
    }
	if (rsp->has_failure()) {
		msg.mutable_failure()->CopyFrom(rsp->failure());
	}
	prd.add_raid_info(rsp->raid_id(), rsp->star(), rsp->tick(), rsp->champion());
	psu->send_client(msg);

	uint32 need_energy = cr->get_need_energy() - cr->get_fail_need_energy_1();
	psu->mutable_attr()->sub_val(PHYSICAL_POWER, need_energy);

	client::devil_raid_detail drd;
	prd.fill_devil_raid_info(rsp->raid_id(), drd);
	psu->send_client(drd);
	return NULL;
}
#endif

void levels_complete(game_service_t *s, const reported_raid_info& rri, scene_user* psu, client::raid_result_msg& msg, client::raid_info_msg& rim)
{
#ifndef __LIB_MINI_SERVER__
    msg.Clear();
    msg.set_raid_id(rri.id());
    msg.set_star(rri.star());
    msg.set_tick(rri.used_tick());
    client::raid_result_info* info = msg.add_info();
    psu->fill_role_data(info->mutable_rd());
    player_role_levels& prl = psu->get_player_role().get_player_role_levels();
    const config_giantraid *cg = CONFMGR->get_config_giantraid(prl.get_cur_level());
    if(cg == NULL) {
        return ;
    }
    const config_upgrade *cu = CONFMGR->get_config_upgrade(psu->get_attr().get_val(PROFESSION), psu->get_attr().get_val(LEVEL));
    if(cu == NULL) {
        return ;
    }
    uint32 money = cg->get_reward_money() * cu->get_bz_obtain() / 10000;
    info->set_coin(money);
    info->set_strategy(cg->get_reward_resource());

	psu->add_props(cg->get_reward_item(), info);

    if (rri.has_success()) {
        msg.mutable_success()->CopyFrom(rri.success());
    }

    if (rri.has_failure()) {
        msg.mutable_failure()->CopyFrom(rri.failure());
    }
    psu->add_money(TAEL, money);
    psu->add_money(STRATEGY, cg->get_reward_resource());
    psu->send_client_change_attr();
    uint32 idx = cg->get_giant_index();
	CHECK_TARGET_PROGRESS(psu, PASS_LEVELS_RAID, idx);
    prl.change_cur_levels();
    if(idx >= prl.get_levels_count())
    {
        client::broadcast_msg_req req;
        req.set_area_id(41);
        client::content_data *cd_s = req.add_data();
        cd_s->set_s(psu->get_role_name());
        client::content_data *cd_n = req.add_data();
        cd_n->set_n(idx);
        game_send_center(s, req);
    }
#endif
}

void normal_complete(const reported_raid_info& rri, const config_raid* cr, scene_user* psu, client::raid_result_msg& msg, client::raid_info_msg& rim,bool is_add_family_exp = false)
{
#ifndef __LIB_MINI_SERVER__
    msg.Clear();
    msg.set_raid_id(rri.id());
    msg.set_star(rri.star());
    msg.set_tick(rri.used_tick());
    uint32 need_energy       = 0;
    uint32 reward_money      = 0;
    uint32 reward_experience = 0;
    uint32 reward_tao_lue    = 0;
    const std::vector<int>* reward_item = NULL;
    if (rri.type() == client::enter_raid_req_type_normal) {
        need_energy       = cr->get_need_energy() - cr->get_fail_need_energy_1();
        reward_money      = cr->get_reward_money();
        reward_experience = cr->get_reward_experience();
        reward_tao_lue    = cr->get_reward_tao_lue();
        reward_item       = &cr->get_reward_item();
        uint32 val = 0;
        psu->get_player_role().get_role_statis_mgr().get(PRSTE_ENTER_RAID_COUNT)->get_val(val);
        psu->get_player_role().get_role_statis_mgr().get(PRSTE_ENTER_RAID_COUNT)->set_val(val + 1);
    } else {
        need_energy       = cr->get_d_2_need_energy() - cr->get_fail_need_energy_2();
        reward_money      = cr->get_reward_money_2();
        reward_experience = cr->get_reward_experience_2();
        reward_tao_lue    = cr->get_reward_tao_lue();
        reward_item       = &cr->get_reward_item_2();
        uint32 val = 0;
        psu->get_player_role().get_role_statis_mgr().get(PRSTE_ENTER_RAID_COUNT)->get_val(val);
        psu->get_player_role().get_role_statis_mgr().get(PRSTE_ENTER_RAID_COUNT)->set_val(val + 0x00010000);
    }
    client::role_statis_rsp rsp;
    psu->get_player_role().get_role_statis_mgr().get(PRSTE_ENTER_RAID_COUNT)->fill(rsp);
    psu->send_client(rsp);
#if 0
    std::multimap<uint32, drop_prop> drops;
    for(size_t i = 0; i < reward_item->size(); ++i)
    {
        make_drops(reward_item->at(i), drops);
    }
#endif
    client::raid_result_info* info = msg.add_info();
    psu->fill_role_data(info->mutable_rd());
    info->set_coin(reward_money);
    if (is_add_family_exp){
        reward_experience = reward_experience + psu->add_family_exp(reward_experience);
    }

    info->set_exp(reward_experience);
    info->set_tao_lue(reward_tao_lue);

	psu->add_props(*reward_item, info);

    if (rri.has_success()) {
        msg.mutable_success()->CopyFrom(rri.success());
    }
    if (rri.has_failure()) {
        msg.mutable_failure()->CopyFrom(rri.failure());
    }
    psu->mutable_attr()->sub_val(PHYSICAL_POWER, need_energy);
    psu->add_money(TAEL, reward_money);
    psu->add_exp(info->exp());
    psu->add_money(MILITARY_STRATEGY, reward_tao_lue);
    psu->send_client_change_attr();
    if(cr->get_map_type() == MAP_TYPE_NORMAL)
    {
        psu->get_player_role().get_raid_mgr().push(psu->get_role_id(), rri.id(), rri.star(), rri.used_tick(), rri.complete_time(), rri.type());
    }
    else 
    {
        psu->get_player_role().get_raid_mgr().push(psu->get_role_id(), rri.id(), rri.star(), rri.used_tick(), rri.complete_time(), rri.type(), 1);
    }
    psu->get_player_role().get_raid_mgr().fill_raid_info_msg(rim, rri.id());
    psu->on_raid_complete(rri.id(), rri.type(), rri.star());
#endif
}

bool check_raid_verify(const raid_verify& verify, scene_user* psu) 
{
    static const uint32 MULTI = 3;
    if (psu->get_attr(MAX_HP) * MULTI < verify.max_hp()) {
        return false;
    }

    if (psu->get_attr(ATTACK_MAX) * MULTI < verify.max_atk()) {
        return false;
    }

    if (psu->get_attr(DEFENSE_MAX) * MULTI < verify.max_def()) {
        return false;
    }

	psu->mutable_attr()->set_val(BOW_INDEX,verify.bow_index());
	psu->send_bow_attack_count();

    return true;
}

void* on_pb_reported_raid_req(client::reported_raid_req* req, void* ud, size_t* rsp_len)
{
#ifndef __LIB_MINI_SERVER__
	struct game_service_t* ss = (struct game_service_t*)ud;
	scene_user *psu = ss->sim->get_space_user(req->svr_head().role_id());
	if(NULL == psu)
	{
		return NULL;
	}
	uint32 leave_time = (uint32)time(NULL);
	client::raid_result_msg msg;
	client::raid_info_msg   rim;
	for (int32 i = 0; i < req->rri_size(); ++i) {
		const reported_raid_info& rri = req->rri(i);
		if (!check_key(rri)) {
			continue;
		}

		if (!check_raid_verify(rri.verify(), psu)) {
			continue;
		}

		uint32 ret = check_enter_raid(rri.id(), rri.type(), psu, true);
		if (ret != 0) {
			continue;
		}

		const config_raid* pcr = CONFMGR->get_config_raid(rri.id());
		if (pcr == NULL) {
			continue;
		}

		if (!psu->check_mini_server(rri.id(), leave_time)) {
			continue;
		}

		switch(pcr->get_map_type())
		{
		case MAP_TYPE_RAID:
			normal_complete(rri, pcr, psu, msg, rim,true);
			CHECK_TARGET_PROGRESS(psu, PASS_COPY, rri.id(), rri.star(), rri.type(), 1);
			break;
		case MAP_TYPE_PET:
			normal_complete(rri, pcr, psu, msg, rim);
			break;
		case MAP_TYPE_LEVELS:
			levels_complete(ss, rri, psu, msg, rim);
			break;
		case MAP_TYPE_DEVIL:
			devil_complete(ss, rri, psu, msg);
			return NULL;
		}
		psu->send_client(msg);
		msg.Clear();
	}

	if (rim.ri_size() > 0) {
		psu->send_client(rim);
	}
#endif
	return NULL;
}
