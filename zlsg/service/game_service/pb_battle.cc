#include "config_raid.h"
#include "scene_user.h"
#include "scene_inst.h"
#include "battle_scene_inst.h"
#include "game_interface.h"

void* on_game_pb_battle_info_req(client::battle_info_req* req, void* ud, size_t* rsp_len)
{
// 	struct game_service_t* ss = (struct game_service_t*)ud;
//     std::vector<scene_inst*> vsi;
//     battle_info_rsp rsp;
//     uint32 battle_id = req->time(); // 6 三阵营战场ID
//     const config_battle* cb = CONFMGR->get_config_battle_mgr().get_config_battle(battle_id);
//     if (cb == NULL) {
//         return NULL;
//     }
//     
//     const mysql::tb_battle& tb = cb->get_tb();
//     const vector<const mysql::tb_battle_actvt_time*>& at = cb->get_tbat();
//     uint32 now = (uint32)time(NULL);
//     for (uint32 i = 0; i < at.size(); ++i) {
//         uint32 open_time = time_utils::make_time(at[i]->time_type(), at[i]->from_hh(), at[i]->from_mm(), at[i]->from_ss());
//         if (open_time > now) {
//             rsp.set_open_seconds(open_time - now);
//             rsp.mutable_bi()->set_index(battle_id);
//             rsp.mutable_bi()->set_close_seconds(1800);
//             rsp.mutable_bi()->set_angel_num(0);
//             rsp.mutable_bi()->set_nether_num(0);
//             rsp.mutable_bi()->set_witch_num(0);
//         } else if (open_time + tb.duration() > now) {
//             rsp.set_open_seconds(0);
//         } else {
//             rsp.set_open_seconds(open_time + 86400 - now);
//         }
//         
//         break;
//     }
// 
//     uint32 scene_id = tb.scene_id();
//     ss->sim->get_scene_by_scene_id(scene_id, vsi);
//     for (uint32 i = 0; i < vsi.size(); ++i) {
//         if (!vsi[i]->is_battle_scene()) {
//             continue ;
//         }
//     
//         battle_scene_inst *bsi = static_cast<battle_scene_inst*>(vsi[i]);
//         if (bsi->is_full() && (vsi.size() != (i + 1))) {
//             continue ;
//         }
// 
//         rsp.mutable_bi()->set_index(battle_id);
//         rsp.mutable_bi()->set_close_seconds(bsi->close_seconds());
//         rsp.mutable_bi()->set_angel_num(bsi->camp_user_num(5));
//         rsp.mutable_bi()->set_witch_num(bsi->camp_user_num(6));
//         rsp.mutable_bi()->set_nether_num(bsi->camp_user_num(7));
//         break;
//     }
// 
//     //rsp.set_open_seconds(0);
//     game_send_client(ss, req->svr_head().client_id(), rsp);

    return NULL;
}

void* on_pb_battle_enter_req(client::battle_enter_req* ber, void* ud, size_t* rsp_len)
{
	struct game_service_t* ss = (struct game_service_t*)ud;
	scene_user* su = ss->sim->get_user(ber->svr_head().role_id());
	if (su == NULL) {
		return NULL;
	}

	bool leave_scene = false;
	client::jump_user ju;
    ju.set_role_id(su->get_role_id());;
    client::BATTLE_OPERATE_RET_E ret = client::BORE_SUCCESS;
	//ret = su->on_pb_battle_enter_req(ber, leave_scene, ju);

    client::battle_enter_rsp rsp;
    rsp.set_ret(ret);
    game_send_client(ss, ber->svr_head().client_id(), rsp);
    amtsvr_billlog(ss->ctx, "%u|%s|%u|%u|%u", su->get_role_id(), ber->GetTypeName().c_str(), ber->time(), ber->id(), (uint32)rsp.ret());

	if (leave_scene) {
		ss->sim->logout_scene(su, su->get_scene());
		su->send_center(ss->ctx, ju);
	}
    
	return NULL;
}

void* on_pb_battle_leave_req(client::battle_leave_req* req, void* ud, size_t* rsp_len)
{
    PB_FUNC_GUARD(req);

    bool leave_scene = false;
    client::battle_leave_rsp rsp;
    rsp.set_ret(client::BORE_SUCCESS);
    client::jump_user ju;
    ju.set_role_id(psu->get_role_id());
    if (psu->get_scene() == NULL) {
        rsp.set_ret(client::BORE_SYS_ERR);
    }

    if (psu->get_scene()->get_scene_type() != MAP_TYPE_BATTLE) {
        rsp.set_ret(client::BORE_SYS_ERR);
    }

    if (rsp.ret() == client::BORE_SUCCESS) {
        psu->fill_back_trans_jump(ju);
        leave_scene = true;
    }

    game_send_client(ss, req->svr_head(), rsp);
    amtsvr_billlog(ss->ctx, "%u|%s|%u|%u", psu->get_role_id(), req->GetTypeName().c_str(), req->time(), (uint32)rsp.ret());

    if (leave_scene) {
        ss->sim->logout_scene(psu, psu->get_scene());
        psu->send_center(ss->ctx, ju);
    }

    return NULL;
}

void* on_game_pb_battle_honor_refund_req(client::battle_honor_refund_req* req, void* ud, size_t* rsp_len)
{
    return NULL;
}

void* on_game_pb_battle_status_req(client::battle_status_req* req, void* ud, size_t* rsp_len)
{
    struct game_service_t* ss = (struct game_service_t*)ud;
    client::battle_status_rsp rsp;
    for (int32 i = 0; i < req->battle_id_size(); ++i) {
        client::battle_status* bs = rsp.add_bs();
        bs->set_battle_id(req->battle_id(i));
    }
    game_send_client(ss, req->svr_head().client_id(), rsp);
    return NULL;
}

