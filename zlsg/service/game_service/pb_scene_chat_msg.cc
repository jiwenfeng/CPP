#include "client.chat_msg.pb.h"
#include "game_interface.h"
#include "scene_user.h"
#include "scene_inst.h"

void* on_pb_scene_chat_msg(client::client_chat_msg* cm, void* ud, size_t* rsp_len)
{
    struct game_service_t* ss = (struct game_service_t*)ud;
    scene_user* su = ss->sim->get_user(cm->svr_head().role_id());
    if (su == NULL){
        return NULL;
    }

    switch (cm->cm().channel()) {
    case client::chat_msg_CHANNEL_SCENE:
        {
            scene_inst* si = su->get_scene();
            if (si == NULL){
                return NULL;
            }

            si->scene_broadcast(*su, *(cm->mutable_cm()), false);
        }
        break;
    case client::chat_msg_CHANNEL_HORN:
        {
            // ¿Û³ı´«Òô·û(id:268)
            if (su->sub_props(268, 1) != 0) {
                return NULL;
            }
            
            cm->clear_svr_head();
            cm->mutable_cm()->set_channel(client::chat_msg_CHANNEL_HORN_BROADCAST);
            su->send_center(*cm);    
        }
        break;
    default:
        break;
    }

    return NULL;
}

