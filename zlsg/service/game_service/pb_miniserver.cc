#include "player_role.h"
#include "scene_inst.h"
#include "game_interface.h"

#ifdef __LIB_MINI_SERVER__
#include "player_role_ext_attr.h"

void* on_pb_miniserver(client::exit_miniserver* req, void* ud, size_t* rsp_len)
{
    struct game_service_t* ss = NULL;
    ss = (struct game_service_t*)ud;
    scene_user* psu = ss->sim->get_user(req->svr_head().role_id());
    if (psu == NULL) {
        return NULL;
    }

    if (psu->mutable_buffer()->del_raid_buf()) {
        uint32 count = 0;
        psu->mutable_buffer()->update(time_utils::tenms(), count);
        client::buffer_list bl;
        psu->mutable_buffer()->fill_change_buffer_list(bl);
        psu->fill_role_data(bl.mutable_rd());
        psu->get_scene()->screen_broadcast(*psu, bl, false);
    }

    psu->calculate_attr();
    psu->calculate_status();

    if (psu->get_status().is_dead()) {
        psu->relive();
    } else if (psu->get_attr(MAX_HP) > psu->get_attr((CUR_HP))) {
        psu->mutable_attr()->set_val(CUR_HP, psu->get_attr(MAX_HP));
        psu->send_client_change_attr();
    }

    return (void*)"end";
}

void* on_pb_update_role_attr_msg(client::update_role_attr_msg* msg, void* ud, size_t* rsp_len)
{
    if (msg->rm_size() == 0) {
        return NULL;
    }

    struct game_service_t* ss = (struct game_service_t*)ud;
    scene_user* psu = ss->sim->get_user(msg->rm(0).rd().role_id());
    if (psu == NULL) {
        return NULL;
    }

    psu->get_player_role().get_player_role_ext_attr_mgr().set_ext_attr(psu->get_role_id(), REASTE_MINI_SERVER_SET, msg->rm(0).attr());
    psu->calculate_attr();

    return NULL;
}

#endif // __LIB_MINI_SERVER__
