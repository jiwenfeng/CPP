#include "config_raid.h"
#include "game_interface.h"
#include "scene_user.h"
#include "scene_inst.h"

void* on_pb_change_status(client::change_status* req, void* ud, size_t* rsp_len)
{
    PB_FUNC_GUARD(req);

    scene_inst* psi = psu->get_scene();
    if (psi == NULL) {
        return NULL;
    }

    // 客户端主动状态更新支持以下几种模式
    //if (req->rs().has_robot()) {
    //    psu->set_robot(req->rs().robot());
    //} 

    //if (req->rs().has_riding()) {
    //    psu->set_riding(req->rs().riding());
    //}

    if (req->rs().has_atk_mode()) {
        scene_inst* psi = psu->get_scene();
        if (psi == NULL) {
            return NULL;
        }
		if(psu->get_status().atk_mode() == req->rs().atk_mode())
		{
			return NULL;
		}
		psu->set_attack_mode(req->rs().atk_mode());
        std::map<uint64, scene_role*>& pets = psu->get_pets();
        for (std::map<uint64, scene_role*>::iterator i = pets.begin(); i != pets.end(); ++i) {
	    	i->second->clean_hate_map();
        }
		psu->clean_hate_map();
		psu->clear_counter_list();
        psu->send_client_change_attr();
    }

    return NULL;
}

void* on_game_pb_select_relive_type_req(client::select_relive_type_req* req, void* ud, size_t* rsp_len)
{
    PB_FUNC_GUARD(req);

    client::select_relive_type_rsp rsp;
    client::select_relive_type_rsp_RET ret = client::select_relive_type_rsp_RET_SUCCESS;
    if (req->type() != client::select_relive_type_req_TYPE_SITU) {
        // TODO: 获取复活点位置 然后跳转
        client::jump_user ju;
        ju.set_role_id(psu->get_role_id());
        if (ju.has_same_scene() && ju.same_scene()) {
            scene_inst* si = psu->get_scene();
            if (si == NULL) {
                return NULL;
            }

            map_postion now_pos(ju.x(), ju.y(), ju.z());
            si->trans_pos(psu, now_pos);
        } else {
            ss->sim->logout_scene(psu, psu->get_scene());
            psu->send_center(ss->ctx, ju);
        }
    } else {
        psu->relive();
    }

    rsp.set_ret(ret);
    game_send_client(ss, req->svr_head().client_id(), rsp);

    return NULL;
}
