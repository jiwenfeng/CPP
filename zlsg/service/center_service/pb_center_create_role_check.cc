#include "client.create_role.pb.h"
#include "center_interface.h"

void* on_pb_center_create_role_check_msg(client::create_role_check_msg* msg, void* ud, size_t* rsp_len)
{
    center_service_t* w = (center_service_t*)ud;
    if (w->aus_mgr.mutable_user_smy(msg->crr().name()) != NULL || w->rbt_mgr.robot_name_exist(msg->crr().name()) || w->iw_mgr.has_illegal_word(msg->crr().name())){
        msg->set_pass(false);
    } else {
        msg->set_pass(true);
    }
    center_send_role(w, *msg);

    return NULL;
}
