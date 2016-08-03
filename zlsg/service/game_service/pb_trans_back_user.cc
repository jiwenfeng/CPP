#include "game_interface.h"
#include "scene_user.h"
#include "scene_inst.h"

void* on_pb_trans_back_user(server::trans_back_user* req, void* ud, size_t* rsp_len)
{
    struct game_service_t* ss = (struct game_service_t*)ud;
    int size = req->role_id_size();
    if (size <= 0) {
        return NULL;
    }

    client::jump_user ju;
    for (int i = 0; i < size; ++i) {
        scene_user* su = ss->sim->get_user(req->role_id(i));
        if (su == NULL) {
            continue;
        }

        scene_inst* si = su->get_scene();
        if (si == NULL) {
            continue;
        }

        ju.set_role_id(su->get_role_id());
        if (su->fill_back_trans_jump(ju) != 0) {
            su->fill_default_trans_jump(ju);
        }
        
        ss->sim->logout_scene(su, si);
        su->send_center(ss->ctx, ju);
    }

    return NULL;
}
