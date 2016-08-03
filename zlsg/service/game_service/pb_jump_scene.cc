#include "client.scene_ready.pb.h"
#include "game_interface.h"
#include "scene_user.h"
#include "scene_inst.h"

#define JSE_SUC  0 // 成功
#define JSE_PROP 1 // 没有道具
#define JSE_CLR  2 // 没有清除怪物
#define JSE_TYPE 3 // 错误的跳转类型
#define JSE_TRS  4 // 错误跳转点
#define JSE_SYS  5 // 找不到自己当前场景
#define JSE_BOSS 6 // 找不到BOSS

int set_jump_user(scene_user* psu, client::jump_scene* pjs, bool& leave_scene, client::jump_user& ju)
{
    leave_scene = false;
    client::jump_scene_rsp rsp;
    scene_inst* psi = psu->get_scene();
    if (psi == NULL) {
        rsp.set_ret(JSE_SYS);
        psu->send_client(rsp);
        return -1;
    }

    // 副本之间跳转则需要清除当前场景所有怪物
//     if (psu->check_raid_monster(pjs) != 0) {
//         rsp.set_ret(JSE_CLR);
//         psu->send_client(rsp);
//         return -2;
//     }

    int ret = 0;
    switch (pjs->dst_type()) {
    case client::jump_scene_DST_TYPE_SCENE:
    case client::jump_scene_DST_TYPE_ROLE:        
    case client::jump_scene_DST_TYPE_DTASK:
    case client::jump_scene_DST_TYPE_CLUMP:
    case client::jump_scene_DST_TYPE_KILL:
    case client::jump_scene_DST_TYPE_TRANS:
        {
            ret = psu->on_jump_scene_by_scene(pjs, ju);
            rsp.set_ret(JSE_TRS);
        }
        break;
        /*{
            ret = psu->on_jump_scene_by_trans(pjs, ju);
            rsp.set_ret(JSE_TRS);
        }
        break;*/
    default:
        {
            rsp.set_ret(JSE_TYPE);
            ret = -3;
        }
        break;        
    }

    if (ret != 0) {
        rsp.set_ret(JSE_TRS);
        psu->send_client(rsp);
        return -4;
    }

//     if (psu->sub_teleport_rune(pjs->dst_type(), pjs->jump_code()) != 0) {
//         rsp.set_ret(JSE_PROP);
//         psu->send_client(rsp);
//         return -5;
//     }

    rsp.set_ret(JSE_SUC);
    leave_scene = true;
    psu->send_client(rsp);

    return ret;
}

void* on_pb_jump_scene(client::jump_scene* js, void* ud, size_t* rsp_len)
{
    struct game_service_t* ss = (struct game_service_t*)ud;
    scene_user* su = ss->sim->get_user(js->svr_head().role_id());
    if (su == NULL) {
        return NULL;
    }
    
    bool leave_scene = false;
    client::jump_user ju;
    ju.set_role_id(su->get_role_id());
    int ret = set_jump_user(su, js, leave_scene, ju);
    if (ret != 0) {
        return NULL;
    }

    /*amtsvr_billlog(ss->ctx, "%d|%s|%d|%d|%d|%d|%d|%d|%d|%d|%d|%d|%d", 
        su->get_role_id(), js->GetTypeName().c_str(), js->dst_type(), js->dst_id(), js->dst_index(), js->jump_index(), js->jump_code(), ju.x(), ju.y(), ju.z(), js->param(), ju.map_id(), ju.scene_id());*/
    if (leave_scene) {
        if (ju.has_same_scene() && ju.same_scene()) {
            scene_inst* si = su->get_scene();
            if (si == NULL) {
                return NULL;
            }

            map_postion now_pos(ju.x(), ju.y(), ju.z());
            si->trans_pos(su, now_pos);
        } else {
            ss->sim->logout_scene(su, su->get_scene());
            su->send_center(ss->ctx, ju);
        }
    }

    return NULL;
}

void* on_game_pb_scene_ready(client::scene_ready* p, void* ud, size_t* rsp_len);

void* on_pb_jump_user(client::jump_user* ju, void* ud, size_t* rsp_len)
{
    struct game_service_t* ss = (struct game_service_t*)ud;
    uintptr_t ptr = (uintptr_t)ju->ptr();
    scene_user* su = (scene_user*)(ptr);
    uint32 map_id = ju->map_id();
    uint32 scene_id = ju->scene_id();
    if (map_id == 0 || scene_id == 0) {
        game_user_offline(ss, su);
        return NULL;
    }

    map_postion now_pos(ju->x(), ju->y(), ju->z());
    su->set_scene_pos(now_pos);
    int ret = ss->sim->login_scene(su, scene_id, 0);
    if (ret != 0) {
        game_user_offline(ss, su);
        return NULL;
    }

    client::role_pos_msg rpm;
    client::server_head* svr_head = rpm.mutable_svr_head();
    svr_head->set_scenesvr_id(ss->self);
    su->fill_role_pos_msg(rpm);
    su->send_agent(ss->ctx, rpm);

    if (ju->has_same_scene() && ju->same_scene()) {
        client::scene_ready sr;
        sr.set_time_now((uint32)time(NULL));
        sr.set_ro(client::scene_ready_READY_OCCASION_JUMP);
        sr.mutable_svr_head()->CopyFrom(*svr_head);
        sr.mutable_svr_head()->set_role_id(su->get_role_id());
        on_game_pb_scene_ready(&sr, ud, rsp_len);
    }

    return NULL;
}

