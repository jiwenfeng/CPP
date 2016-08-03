#include "game_interface.h"
#include "operate_log.h"
#include "player_role.h"
#include "player_role_statis_mgr.h"
#include "scene_user.h"
#include "scene_inst.h"

void* on_game_pb_invest_req(client::invest_req* req, void* ud, size_t* rsp_len)
{
    PB_FUNC_GUARD(req);

    uint32 old_value = psu->get_attr().ingot();
    int ret = psu->sub_money(INGOT, 200);
    uint32 new_value = psu->get_attr().ingot();
    psu->sub_money_log(operate_log::ROOT_INVEST, operate_log::TYPE_INVEST, old_value, new_value);

    invest_rsp rsp;
    if (ret == 0 && psu->get_player_role().get_role_statis_mgr().join_invest()) {
        uint32 now = (uint32)time(NULL);
        psu->get_player_role().get_role_statis_mgr().set_invest_join_time(now);
        rsp.set_ret(0);
    } else {
        rsp.set_ret(1);
    }

    game_send_client(ss, req->svr_head().client_id(), rsp);

    return NULL;
}

