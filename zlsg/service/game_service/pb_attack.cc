#include "config_npc.h"
#include "game_interface.h"
#include "game_map_mgr.h"
#include "scene_npc.h"
#include "scene_user.h"
#include "scene_inst.h"
#include "effect_result.h"

// void use_attack_tirgger_skill(scene_user* psu, client::attack_req* req, void* ud, size_t* rsp_len)
// {
//     role_skill* rsk = psu->mutable_skill();
//     std::vector<skill*> vs;
//     rsk->get_skill_by_type(ST_ATK_TIRGGER, vs);
//     for (size_t i = 0; i < vs.size(); ++i) {
//         const config_skill* pcs = vs[i]->get_config_skill();
//         if (pcs == NULL || !psu->get_skill().check_mount_skill(psu->get_attr(MOUNTS_ID), pcs)) {
//             continue;
//         }
// 
//         if (req->skill_id() == vs[i]->get_skill_id()) {
//             continue;
//         }
// 
//         if (pcs->get_attack_time() != 0) {
//             if (psu->get_attack_count() == 0 || 
//                 psu->get_attack_count() % pcs->get_attack_time() != 0) {
//                     continue;
//             }
//         }
// 
//         uint32 rand = mtrandom::rand_int32(10000u);
//         if (rand > pcs->get_trigger_probability()) {
//             continue;
//         }
// 
//         client::attack_req r(*req);
//         r.set_skill_id(vs[i]->get_skill_id());
//         on_pb_attack(&r, ud, rsp_len);
//         if (pcs->get_attack_time() != 0) {
//             psu->sub_attack_count();
//         }
//     }
// }

void* on_pb_attack(client::attack_req* req, void* ud, size_t* rsp_len)
{
    PB_FUNC_GUARD(req);

    scene_inst* psi = psu->get_scene();
    if (psi == NULL) {
        return NULL;
    }

	if(psu->get_status().is_dead()) {
		return NULL;
	}

    // 验证技能
    role_skill* rsk = psu->mutable_skill();
    if (rsk == NULL) {
        return NULL;
    }

    skill* sk = rsk->use_skill(req->skill_id(), psu->get_attr(ATTACK_SPEED));
    if (sk == NULL) {
        amtsvr_log("error skill %u\n", req->skill_id());
        return NULL;
    }

    psu->skill_cost(sk);
    std::vector<skill*> vs;
    rsk->get_skill_by_type(ST_ATK_TIRGGER, vs);
    for (size_t i = 0; i < vs.size(); ++i) {
        const config_skill* pcs = vs[i]->get_config_skill();
        if (pcs == NULL || !psu->get_skill().check_mount_skill(psu->get_attr(MOUNTS_ID), pcs)) {
            continue;
        }

        if (req->skill_id() == vs[i]->get_skill_id()) {
            continue;
        }

        if (pcs->get_attack_time() != 0) {
            if (psu->get_attack_count() == 0 || 
                psu->get_attack_count() % pcs->get_attack_time() != 0) {
                    continue;
            }
        }

        uint32 rand = mtrandom::rand_int32(10000u);
        if (rand > pcs->get_trigger_probability()) {
            continue;
        }

        //amtsvr_log("ATK_TIRGGER %u\n", vs[i]->get_skill_id());
        client::attack_req r(*req);
        r.set_skill_id(vs[i]->get_skill_id());
        on_pb_attack(&r, ud, rsp_len);
        if (pcs->get_attack_time() != 0) {
            psu->sub_attack_count();
        }
    }

    psu->stop_practice();
    if (req->has_ac()) {
        psu->set_facing(req->ac().x(), req->ac().y());
    }

    // 获取受击的对象
    std::vector<scene_role*> vsr;
    std::vector<scene_role*> friends;
    std::vector<map_postion> miss;
    psi->select_target(psu, sk, vsr, friends, miss);
    cell_pos attack_cell = psi->to_cell_pos(psu->get_scene_pos());
    std::vector<client::fight_data> fds;
    for (int32 i = 0; i < req->rd_size(); ++i) {
        if (vsr.size() >= sk->get_max_target()) {
            break;
        }

        const role_data& rd = req->rd(i);
        ROLE_TYPE_E rt = (ROLE_TYPE_E)rd.role_typ();
        scene_role* psr = NULL;
        if (rt == RTE_USER) {
            psr = psi->get_scene_user(rd.inst_id());
        } else if (rt == RTE_NPC) {
            psr = psi->get_scene_npc(rd.inst_id());
        } else if (rt == RTE_PET) {
            psr = psi->get_scene_npc(rd.inst_id());
        }

        if (psr == NULL) {
            amtsvr_log("target error %u, %u\n", rd.role_id(), rd.role_typ());
            continue;
        }

        if (psi->is_friend(psu, psr)) {
            amtsvr_log("target error %u, %u\n friend", rd.role_id(), rd.role_typ());
            continue;
        }

        std::vector<scene_role*>::iterator f = find(vsr.begin(), vsr.end(), psr);
        if (f != vsr.end()) {
            continue;
        }

        bool is_far = sk->get_distance() + 1 < attack_cell.get_distance(psi->to_cell_pos(psr->get_scene_pos()));
        if (psr != NULL && !is_far) {
            vsr.push_back(psr);
        } else {
            vsr.push_back(psr);
            amtsvr_log("a.x = %d a.y = %d, d.x = %d, d.y = %d\n", psu->get_scene_pos().x, psu->get_scene_pos().y, psr->get_scene_pos().x, psr->get_scene_pos().y);
//             client::fight_data d;
//             psr->fill_role_data(d.mutable_dst_rd());
//             d.set_fight_type(client::fight_data_TYPE_attk_far);
//             fds.push_back(d);
        }
    }

    // 计算伤害
    effect_result rsp;
    rsp.set_skill_id(req->skill_id());
    psu->attack_damage(sk, vsr, rsp);
    psu->attack_effect(sk, vsr, friends, rsp);
    psu->add_attack_count();

	if (psu->is_add_attack_count()){
		psu->bow_attack_skill();
	}

    rsp.set_id(req->id());
    if (req->has_ac()) {
        rsp.mutable_ac()->CopyFrom(req->ac());
    }

    psu->fill_miss(sk, miss, rsp);

    for (size_t i = 0; i < fds.size(); ++i) {
        rsp.add_fd()->CopyFrom(fds[i]);
    }

    // 下发通知
    psi->screen_broadcast(*psu, rsp, false);

    // 攻击者可能不在另一个可见受击者屏幕范围需要以受击者为中心广播消息
    client::attack_rsp def;
    def.CopyFrom(rsp);
    def.set_def(true);
    for (int32 i = 0; i < rsp.fd_size(); ++i) {
        scene_role* psr = psi->get_scene_role(rsp.fd(i).dst_rd());
        if (psr == NULL) {
            psr = psi->get_scene_role(rsp.fd(i).dst_rd().inst_id());
        }

        if (psr == NULL) {
            continue;
        }

        def.clear_fd();
        def.add_fd()->CopyFrom(rsp.fd(i));
        psi->screen_broadcast(*psr, def, false);
    }

    psi->multi_broadcast_modify_role_msg(*psu, vsr);
    psu->send_client_change_attr();

    client::notify_kill_event nke;
    psu->fill_role_data(nke.mutable_killer());
    for (size_t i = 0; i < vsr.size(); ++i) {
        if (vsr[i]->get_status().is_dead()) {
            vsr[i]->fill_role_data(nke.mutable_dead());
			if(!vsr[i]->has_dead())
			{
				vsr[i]->on_killed(psu);
	            psu->on_kill(vsr[i]);
			}
        }
    }

    return NULL;
}

/*float ccpAngle(const CCPoint& a, const CCPoint& b)
  {
  float angle = acosf(ccpDot(ccpNormalize(a), ccpNormalize(b)));
  if( fabs(angle) < kCCPointEpsilon ) return 0.f;
  return angle;
  }*/

void* on_pb_nearby_npc_pos_req(client::nearby_npc_pos_req* req, void* ud, size_t* rsp_len)
{
    PB_FUNC_GUARD(req);

    scene_inst* psi = psu->get_scene();
    if (psi == NULL) {
        return NULL;
    }

    std::vector<scene_npc*> ns;
    psi->get_alive_scene_npcs(ns);
    client::nearby_npc_pos_rsp rsp;
    bool is_break = false;
    for (size_t i = 0; i < ns.size(); ++i) {
        if (ns[i]->get_master() == psu) {
            continue;
        }

        if (ns[i]->get_origin_npc().get_monster_type() == MT_NPC ||
            ns[i]->get_origin_npc().get_monster_type() == MT_TEMP_PET ||
            ns[i]->get_origin_npc().get_monster_type() == MT_USER_FRIEND ||
            ns[i]->get_origin_npc().get_monster_type() == MT_TASK_NPC ) {
            continue;
        }

        if (ns[i]->get_region() == psu->get_region()) {
            rsp.clear_pos();
            is_break = true;
        }

        client::client_map_pos* cmp = rsp.add_pos();
        cmp->set_x(ns[i]->get_scene_pos().x);
        cmp->set_y(ns[i]->get_scene_pos().y);
        cmp->set_z(ns[i]->get_scene_pos().z);

        if (is_break) {
            break;
        }
    }

    rsp.set_ret(0);
    psu->send_client(rsp);

    return NULL;
}
