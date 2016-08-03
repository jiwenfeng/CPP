#include "scene_user.h"
#include "offline_role_mgr.h"
#include "center_interface.h"

void center_on_user_offline(center_service_t* w, uint32 role_id)
{

}

void destroy_user(center_service_t* w, scene_user* su)
{
    if (su == NULL) {
        return;
    }

    uint32 role_id = su->get_role_id();
    w->oi_mgr.remove(role_id);
    center_on_user_offline(w, role_id);
    //su->get_player_role().write_all(w->ctx);
    
    player_role* pr = su->detach_player_role();
    delete su;
    if (pr != NULL) {
        center_role* cr = NULL;
        online_role* oi = w->oi_mgr.mutable_online_info(role_id);
        if (oi != NULL) {
            cr = oi->pop_center_role();
        }
        
        w->ou_mgr->push(role_id, pr, cr);
        w->oi_mgr.remove(role_id);
        center_on_user_offline(w, role_id);
    }
}

void* on_pb_center_jump_scene(client::jump_user* ju, void* ud, size_t* rsp_len)
{
    center_service_t* w   = (center_service_t*)ud;
    uintptr_t         ptr = (uintptr_t)ju->ptr();
    scene_user*        su = (scene_user*)(ptr);
    if (su == NULL) {
        return NULL;
    }

    uint32 scene_service_id = w->wm_mgr.get_scene_service_id(ju->scene_id());
    if (scene_service_id == 0) {
        destroy_user(w, su);// 跳转场景不存在，要销毁场景用户对象
        return NULL;
    }

    msg_handler* mh = MSG_HANDLER_POOL->pop();
    if (mh == NULL) {
        destroy_user(w, su);
        return NULL;
    }

    mh->msg = new client::jump_user(*ju);
    if (amtsvr_send(w->ctx, 0, scene_service_id, PTYPE_CLIENT|PTYPE_TAG_DONTCOPY, 0, mh, sizeof(msg_handler)) < 0) {
        destroy_user(w, su);
        return NULL;// 跳转场景不存在，要销毁场景用户对象
    }

    w->oi_mgr.update_scene(ju->role_id(), ju->scene_id(), scene_service_id);

    return NULL;
}
