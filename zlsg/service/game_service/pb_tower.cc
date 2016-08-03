#include "scene_user.h"
#include "tower_scene_inst.h"
#include "game_interface.h"

void* on_pb_tower_challenge_req(client::tower_challenge_req* tcr, void* ud, size_t* rsp_len)
{
    PB_FUNC_GUARD(tcr);

    bool leave_scene = false;
    client::jump_user ju;
    ju.set_role_id(psu->get_role_id());

    amtsvr_billlog(ss->ctx, "%u|%s|%u", psu->get_role_id(), tcr->GetTypeName().c_str(), tcr->raid_id());

    if (leave_scene) {
        ss->sim->logout_scene(psu, psu->get_scene());
        psu->send_center(ss->ctx, ju);
    }

    return NULL;
}

void* on_pb_tower_auto_req(client::tower_auto_req* tar, void* ud, size_t* rsp_len)
{
    PB_FUNC_GUARD(tar);
    
    bool leave_scene = false;
    client::jump_user ju;
    ju.set_role_id(psu->get_role_id());

    amtsvr_billlog(ss->ctx, "%u|%s|%u", psu->get_role_id(), tar->GetTypeName().c_str(), tar->lvl());

    if (leave_scene) {
        ss->sim->logout_scene(psu, psu->get_scene());
        psu->send_center(ss->ctx, ju);
    }

    return NULL;
}

void* on_pb_tower_leave_req(client::tower_leave_req* tlr, void* ud, size_t* rsp_len)
{
    PB_FUNC_GUARD(tlr);

    bool leave_scene = false;
    client::jump_user ju;
    ju.set_role_id(psu->get_role_id());
    if (psu->get_scene() == NULL) {
        return NULL;
    }


    tower_scene_inst* tmp = static_cast<tower_scene_inst*>(psu->get_scene());
    client::tower_leave_notify  cln;
    cln.set_lvl(tmp->get_raid_scene().get_battle_ui_id());
    cln.set_req_leave(false);
    psu->send_client(cln);

    psu->fill_back_trans_jump(ju);
    leave_scene = true;

    amtsvr_billlog(ss->ctx, "%u|%s|%u", psu->get_role_id(), tlr->GetTypeName().c_str(), tlr->svr_time());
    
    if (leave_scene) {
        ss->sim->logout_scene(psu, psu->get_scene());
        psu->send_center(ss->ctx, ju);
    }

    return NULL;
}
