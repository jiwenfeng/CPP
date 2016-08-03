#include "client.offline.pb.h"
#include "client.scene_ready.pb.h"
#include "player_role.h"
#include "player_role_status.h"
#include "game_interface.h"

void* on_game_pb_scene_ready(client::scene_ready* p, void* ud, size_t* rsp_len);

void* on_replace_scene_user(mysql::sp_login_auth* sla, void* ud, size_t* rsp_len)
{
    struct game_service_t* ss = (struct game_service_t*)ud;
    scene_user* psu = ss->sim->find_user(sla->v_role_id());
    if (psu == NULL) {
        client::user_lose ul;
        ul.set_role_id(sla->v_role_id());
        game_send_center(ss, ul);
        return NULL;
    }

    psu->set_agent_id(sla->p_agent_id());
    psu->set_client_id(sla->p_client_id());
    if (ss->sim->get_space_user(sla->v_role_id()) != NULL) {
        client::role_pos_msg rpm;
        rpm.set_type(2);
        client::server_head* svr_head = rpm.mutable_svr_head();
        svr_head->set_role_id(psu->get_role_id());
        svr_head->set_scenesvr_id(ss->self);
        psu->fill_role_pos_msg(rpm);
        psu->send_agent(ss->ctx, rpm);
        return NULL;
    }

    client::multi_role_msg mrm;
    psu->fill_user_msg(*mrm.add_rm());
    psu->send_agent(ss->ctx, mrm); 

    client::role_name_msg rnm;
    psu->fill_role_name_msg(rnm);
    psu->send_agent(ss->ctx, rnm);

    client::role_pos_msg rpm;
    client::server_head* svr_head = rpm.mutable_svr_head();
    svr_head->set_role_id(psu->get_role_id());
    svr_head->set_scenesvr_id(ss->self);
    psu->fill_role_pos_msg(rpm);
    psu->send_agent(ss->ctx, rpm);

    client::scene_ready sr;
    sr.set_ro(client::scene_ready_READY_OCCASION_LOGIN);
    sr.set_time_now(0);
    sr.mutable_svr_head()->CopyFrom(*svr_head);
    on_game_pb_scene_ready(&sr, ud, rsp_len);

    return NULL;
}

static int switch_scene(struct game_service_t* ss, client::trans_user* tr, player_role* p, uint32 role_id)
{
    if (tr->scene_inst_id() == 0) {
        return -1;
    }

    uint32 map_id   = 0;
    uint32 scene_id = 0;
    p->get_role_status().clear_tmp_pos(map_id, scene_id);
    if (map_id == 0 || scene_id == 0) {
        return -2;
    }

    client::trans_user wtr;
    uintptr_t ptr = (uintptr_t)p;
    wtr.set_ptr((uint64)ptr);
    wtr.set_map_id(map_id);
    wtr.set_scene_id(scene_id);
    wtr.set_scene_inst_id(0);
    client::server_head* svr_head = wtr.mutable_svr_head();
    svr_head->set_agent_id(tr->svr_head().agent_id());
    svr_head->set_client_id(tr->svr_head().client_id());

    game_send_center(ss, wtr);

    return 0;
}

#ifdef TEST_MEMCHECK  // 这里主要监测scene_user对象是否存在内存泄露的问题20130703检测通过
static void test_memcheck_scene_user(struct game_service_t* ss, client::trans_user* tr, player_role* p, uint32 role_id)
{
    return;
    uint32 map_id = tr->map_id();
    uint32 scene_id = tr->scene_id();
    for (uint32 i = 0; i < 100000; ++i) {
        scene_user* su = new scene_user(p);
        if (su->init(map_id, scene_id) != 0) {
            game_user_offline(ss, su);
        }
        su->detach_player_role();
        delete su;
    }
}
#endif

static scene_user* scene_create_role(struct game_service_t* ss, client::trans_user* tr, player_role* p, uint32 role_id)
{
    uint32      map_id   = tr->map_id();
    uint32      scene_id = tr->scene_id();
    scene_user* su       = new scene_user(p);
    if (su->init(map_id, scene_id) != 0) {
        game_user_offline(ss, su);
        return NULL;
    }

#ifdef TEST_MEMCHECK
    test_memcheck_scene_user(ss, tr, p, role_id);
#endif

    if (map_id == 0 || scene_id == 0) {
        game_user_offline(ss, su);
        return NULL;
    }

    su->set_agent_id(tr->svr_head().agent_id());
    su->set_client_id(tr->svr_head().client_id());
    int ret = ss->sim->login_scene(su, scene_id, tr->scene_inst_id());
    if (ret != 0) {
        if (switch_scene(ss, tr, p, role_id) == 0) {
            su->detach_player_role();
            delete su;
            return NULL;
        }

        game_user_offline(ss, su);
        return NULL;
    }

    return su;
}

#ifdef TEST_MEMCHECK  // 这里主要监测scene_user登录和离开场景是否会造成内存泄露，20130703检测通过
static scene_user* test_memcheck_scene_user_login(struct game_service_t* ss, client::trans_user* tr, player_role* p, uint32 role_id)
{
    for (uint32 i = 0; i < 1024000; ++i) {
        scene_user* su = scene_create_role(ss, tr, p, role_id);
        if (su == NULL) {
            continue;
        }

        scene_inst* si = su->get_scene();
        if (si != NULL) {
            ss->sim->logout_scene(su, si);
        }

        su->detach_player_role();
        delete su;
    }

    return scene_create_role(ss, tr, p, role_id);
}
#endif

void* on_pb_scene_player_info(client::trans_user* tr, void* ud, size_t* rsp_len)
{
    struct game_service_t* ss = (struct game_service_t*)ud;
    uintptr_t    ptr = (uintptr_t)tr->ptr();
    player_role* p   = (player_role*)(ptr);
    if (p == NULL) {
        return NULL;
    }

    uint32 role_id = p->get_role_id();
    scene_user* psu = ss->sim->get_user(role_id);
    bool auto_ready = false;
    if (psu == NULL) {
#ifdef TEST_MEMCHECK
        psu = test_memcheck_scene_user_login(ss, tr, p, role_id);//scene_create_role(ss, tr, p, role_id);
#endif
        psu = scene_create_role(ss, tr, p, role_id);
        if (psu == NULL) {
            return NULL;
        }
    } else {
        psu->set_agent_id(tr->svr_head().agent_id());
        psu->set_client_id(tr->svr_head().client_id());

        auto_ready = true;
    }

    client::multi_role_msg mrm;
    psu->fill_user_msg(*mrm.add_rm());
    psu->send_agent(ss->ctx, mrm); 

    client::role_name_msg rnm;
    psu->fill_role_name_msg(rnm);
    psu->send_agent(ss->ctx, rnm);

    client::role_pos_msg rpm;
    client::server_head* svr_head = rpm.mutable_svr_head();
    svr_head->set_scenesvr_id(ss->self);
    psu->fill_role_pos_msg(rpm);
    psu->send_agent(ss->ctx, rpm);

    if (auto_ready) {
        client::scene_ready sr;
        sr.set_time_now(0);
        sr.mutable_svr_head()->CopyFrom(tr->svr_head());
        on_game_pb_scene_ready(&sr, ud, rsp_len);
    }

    return NULL;
}
