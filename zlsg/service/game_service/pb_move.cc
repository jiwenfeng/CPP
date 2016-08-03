#include "game_interface.h"
#include "scene_user.h"
#include "scene_inst.h"

void* on_pb_role_move(client::role_move_msg* req, void* ud, size_t* rsp_len)
{
    struct game_service_t* ss = (struct game_service_t*)ud;
    scene_user* su = ss->sim->get_user(req->svr_head().role_id());
    if (su == NULL) {
        return NULL;
    }

    scene_inst* psi = su->get_scene();
    if (psi == NULL) {
        return NULL;
    }

    if (req->move_type() != client::role_move_msg_MOVE_TYPE_CLIENT_NOTIFY || req->mi_size() <= 0 ) {
        return NULL;
    }

    const client::move_info& mi = req->mi(0);
    if (mi.mps_size() <= 1) {
        return NULL;
    }
    
    if (!su->get_scene_ready()) {
        su->stand(false);
        return NULL;
    }

    //printf("%s\n", req->DebugString().c_str());

    su->stop_practice();

    uint32 move_count = (mi.mps_size() < MAX_MOVE_NODE ? mi.mps_size() : MAX_MOVE_NODE);
    su->set_move_count(move_count);

    std::vector<move_map_postion>* move_node = su->mutable_move_node();
    for (uint8 i = 0; i < move_count; ++i) {
        const client_map_pos& cmp = mi.mps(i);
        if (cmp.has_t()) {
            move_node->at(move_count - 1 - i).reset(cmp.x(), cmp.y(), cmp.z(), cmp.t());
        } else {
            move_node->at(move_count - 1 - i).reset(cmp.x(), cmp.y(), cmp.z());
        }
    }

    psi->screen_broadcast_move_pos(*su);

    su->set_last_move_tick(time_utils::tenms());
    su->change_cur_pos(move_node->at(move_count - 1));

    return NULL;
}

void* on_pb_role_postion_msg(client::role_postion_msg* p, void* ud, size_t* rsp_len)
{
    struct game_service_t* ss = (struct game_service_t*)ud;
    scene_user* su = ss->sim->get_user(p->svr_head().role_id());
    if (su == NULL) {
        return NULL;
    }

    scene_inst* si = su->get_scene();
    if (si == NULL) {
        return NULL;
    }

    p->set_z(0);
    su->stand();
    static const uint32 allowed_range = 1000;
    map_postion mp(p->x(), p->y(), p->z());
    if (su->get_scene_pos() == mp) {
        return NULL;
    }

    if (su->is_in_range(mp, allowed_range)) { // 误差范围内只作位置修正，不需要同步广播
        su->change_cur_pos(mp);
    }

    return NULL;
}
