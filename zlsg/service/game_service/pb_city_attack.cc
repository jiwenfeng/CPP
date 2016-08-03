#include "config_raid.h"
#include "game_interface.h"
#include "halo.h"
#include "scene_user.h"
#include "siege_scene_inst.h"
#include "config_map.h"
#include "config_skill.h"

// op 1 添加 
static void change_king_weap(game_service_t *ss, scene_user *su, uint32 op)
{
	if(op == 1)
	{
		uint32 weapon_id = su->get_attr().get_val(WEAPON_ID);
		uint32 king_weapon = 0;
		if(su->get_attr().get_val(PROFESSION) == 0)
		{
			king_weapon = CONFMGR->get_basic_value(10130);
		}
		else
		{
			king_weapon = CONFMGR->get_basic_value(10131);
		}
		if(weapon_id != 0)
		{
			su->mutable_attr()->set_val(WEAPON_ID, king_weapon);
			su->set_store_weapon(weapon_id);
		}
		su->set_king_weapon(king_weapon);
	}
	else
	{
		su->reset_user_weapon();
		su->set_king_weapon(0);
	}
	su->send_client_change_attr();
#ifndef __LIB_MINI_SERVER__
	su->update_online_smy_info(client::SMY_ROLE_WEAPON_ID, su->get_attr().get_val(WEAPON_ID));
#endif
}

//进入攻城场景
void *on_pb_siege_enter_field(client::siege_enter_req *req, void *ud, size_t *rsp_len)
{
	PB_FUNC_GUARD(req);
	scene_inst *si = psu->get_scene();
	if(!si || si->get_scene_type() != MAP_TYPE_NORMAL)
	{
		return NULL;
	}
    client::jump_user ju;
#define SIEGE_MAP_ID 101
	psu->save_formal_pos();
	uintptr_t ptr = (uintptr_t)psu;
	ju.set_ptr((uint64)ptr);
	ju.set_map_id(SIEGE_MAP_ID);
	ju.set_scene_id(SIEGE_MAP_ID);
	ju.set_role_id(psu->get_role_id());
    const config_map* cm = CONFMGR->get_config_map(SIEGE_MAP_ID);
	ju.set_trans_id(SIEGE_MAP_ID);
	ju.set_x(cm->get_birth_point_x());
	ju.set_y(cm->get_birth_point_y());
	ju.set_z(0);
	ju.set_same_scene(psu->is_same_map_and_scene(ju.map_id(), ju.scene_id()));
    ss->sim->logout_scene(psu, si);
    psu->send_center(ss->ctx, ju);
    amtsvr_billlog(ss->ctx, "%u|%s", psu->get_role_id(), req->GetTypeName().c_str());
	return NULL;
}

//复活
void *on_pb_siege_revive_req(client::siege_revive_req *req, void *ud, size_t *rsp_len)
{
    PB_FUNC_GUARD(req);
	scene_inst *p = psu->get_scene();
	if(p == NULL || p->get_scene_type() != MAP_TYPE_BATTLE)
	{
		return NULL;
	}
	if(!psu->get_status().is_dead())
	{
		return NULL;
	}
	psu->relive();
    amtsvr_billlog(ss->ctx, "%u|%s", psu->get_role_id(), req->GetTypeName().c_str());
	return NULL;
}

void *on_pb_city_welfare_get_rsp(client::city_welfare_get_notify *rsp, void *ud, size_t *rsp_len)
{
	 struct game_service_t *ss = (struct game_service_t *)ud;
	 scene_user *psu = ss->sim->get_user(rsp->role_id());
	 if(psu == NULL)
	 {
		 return NULL;
	 }
	 client::city_welfare_get_rsp ret;
	 if(psu->add_props(rsp->props_id(), rsp->num()) < 0)
	 {
		 ret.set_err_no(BAG_LIMIT);
		 psu->send_client(ret);
		 return NULL;
	 }
	 ret.set_err_no(SUCCESS);
	 psu->send_client(ret);
	 client::city_welfare_get_confirm confirm;
	 confirm.set_role_id(rsp->role_id());
	 game_send_center(ss, confirm);
	 return NULL;
}

void *on_pb_city_skill_modify(client::siege_skill_modify *req, void *ud, size_t *rsp_len)
{
	struct game_service_t *ss = (struct game_service_t *)ud;
	scene_user *psu = ss->sim->get_user(req->role_id());
	if(NULL == psu)
	{
		return NULL;
	}

    config_skill_mgr* pcsm = MUTABLE_CONFIG_MGR(config_skill_mgr);
    if (pcsm == NULL) {
        return NULL;
    }

	switch(req->op())
	{
		case 1:
			for(int i = 0; i < req->id_size(); ++i)
			{
				const config_skill* pcs = pcsm->get_config_skill(req->id(i));
				if (pcs == NULL) 
				{
					continue;
				}
				const std::vector<const config_effect*>& effects = pcs->get_effects();
				for (size_t effects_index = 0; effects_index < effects.size(); ++effects_index) 
				{
					if (effects[effects_index] != NULL && effects[effects_index]->is_halo()) 
					{
						psu->add_halo(psu, pcs, effects[effects_index]);
					}
				}
			}

			break;
		case 2:

			for(int i = 0; i < req->id_size(); ++i)
			{
				const config_skill* pcs = pcsm->get_config_skill(req->id(i));
				if (pcs == NULL) 
				{
					continue;
				}
				const std::vector<const config_effect*>& effects = pcs->get_effects();
				for (size_t effects_index = 0; effects_index < effects.size(); ++effects_index) 
				{
					if (effects[effects_index] != NULL && effects[effects_index]->is_halo()) 
					{
						psu->del_halo(psu, effects[effects_index]);
					}
				}
			}
			break;
		default:
			break;
	}

	change_king_weap(ss, psu, req->op());
	return NULL;
}

void *on_pb_family_money_query_rsp(client::family_money_query_rsp *rsp, void *ud, size_t *rsp_len)
{
	PB_FUNC_GUARD_VIA(rsp);
	scene_inst *p = psu->get_scene();
	if(p == NULL || p->get_scene_type() != MAP_TYPE_BATTLE)
	{
		return NULL;
	}
	siege_scene_inst *ssi = static_cast<siege_scene_inst *>(p);
	ssi->do_pick(rsp);
	return NULL;
}

void *on_pb_siege_call_req(client::siege_call_req *req, void *ud, size_t *rsp_len)
{
	PB_FUNC_GUARD(req);
	scene_inst *si = psu->get_scene();
	client::siege_call_rsp rsp;
	if(!si || si->get_scene_type() != MAP_TYPE_BATTLE)
	{
		return NULL;
	}
	uint32 family_id = psu->get_family_id();
	if(0 == family_id)
	{
		rsp.set_err_no(FAMILY_LIMIT);
		psu->send_client(rsp);
		return NULL;
	}
	battle_scene_inst *bsi = static_cast<battle_scene_inst *>(si);
	client::CITY_OPT_ERR_NO_E ret = bsi->call(psu, family_id);
	rsp.set_err_no(ret);
	psu->send_client(rsp);
    amtsvr_billlog(ss->ctx, "%u|%s|%d", psu->get_role_id(), req->GetTypeName().c_str(), (int)ret);
	return NULL;
}

void *on_pb_siege_call(client::siege_call *sc, void *ud, size_t *rsp_len)
{
	PB_FUNC_GUARD_VIA(sc);
	scene_inst *si = psu->get_scene();
	client::siege_call_accept_rsp rsp;
	if(!si || si->get_scene_type() != MAP_TYPE_BATTLE)
	{
		rsp.set_ret(client::siege_call_accept_rsp_RET_OUTSIDE);
		psu->send_client(rsp);
		return NULL;
	}
	if(psu->get_status().is_dead())
	{
		rsp.set_ret(client::siege_call_accept_rsp_RET_DEAD);
		psu->send_client(rsp);
		return NULL;
	}
	uint32 family_id = psu->get_family_id();
	battle_scene_inst *bsi = static_cast<battle_scene_inst *>(si);
	if(bsi->is_call_expired(family_id) != 0)
	{
		rsp.set_ret(client::siege_call_accept_rsp_RET_TIMEOUT);
		psu->send_client(rsp);
		return NULL;
	}
	scene_user *chief = bsi->get_scene_user_by_role_id(sc->chief());
	if(!chief)
	{
		rsp.set_ret(client::siege_call_accept_rsp_RET_KING_OUTSIDE);
		psu->send_client(rsp);
		return NULL;
	}
	rsp.set_ret(client::siege_call_accept_rsp_RET_SUCCESS);
	psu->send_client(rsp);
	bsi->response_call(psu, chief);
	return NULL;
}

void *on_pb_siege_personal_info_req(client::siege_personal_info_req *req, void *ud, size_t *rsp_len)
{
	PB_FUNC_GUARD(req);
	scene_inst *si = psu->get_scene();
	if(si == NULL || si->get_scene_type() != MAP_TYPE_BATTLE)
	{
		return NULL;
	}
	siege_scene_inst *ssi = static_cast<siege_scene_inst *>(si);
	if(ssi == NULL)
	{
		return NULL;
	}
	client::siege_personal_info_rsp rsp;
	if(-1 == ssi->fill_personal_info(psu, rsp))
	{
		return NULL;
	}
	psu->send_client(rsp);
    amtsvr_billlog(ss->ctx, "%u|%s|%u|%u|%u", psu->get_role_id(), req->GetTypeName().c_str(), rsp.region(), rsp.exp(), rsp.time());
	return NULL;
}

void *on_pb_siege_field_open(client::siege_field_open *sfo, void *ud, size_t *rsp_len)
{
	struct game_service_t *ss = (struct game_service_t *)ud;
	scene_inst *si = ss->sim->get_siege_scene();
	if(si == NULL)
	{
		si = ss->sim->create_siege_scene();
	}
	if(si == NULL)
	{
		amtsvr_log("create siege field failed\n");
	}
	return NULL ;
}

void *on_pb_siege_start(client::siege_start *req, void *ud, size_t *rsp_len)
{
	struct game_service_t *ss = (struct game_service_t *)ud;
	scene_inst *si = ss->sim->get_siege_scene();
	if(si == NULL)
	{
		si = ss->sim->create_siege_scene();
	}
	if(si == NULL)
	{
		amtsvr_log("create siege field failed\n");
		return NULL;
	}
	siege_scene_inst *ssi = static_cast<siege_scene_inst *>(si);
	ssi->siege_start(req);
	return NULL;
}

void *on_pb_update_sculpture(client::update_sculpture *us, void *ud, size_t *rsp_len)
{
	struct game_service_t *ss = (struct game_service_t *)ud;
	scene_inst *si = ss->sim->get_normal_scene();
	if(NULL == si)
	{
		return NULL;
	}
	if(si->get_scene_type() != MAP_TYPE_NORMAL)
	{
		return NULL;
	}
	si->update_sculpture(us->role_id(), us->king(), us->title(), us->family_name());
	return NULL;
}
