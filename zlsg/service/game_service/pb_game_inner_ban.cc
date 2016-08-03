#include "client.offline.pb.h"
#include "player_role.h"
#include "scene_user.h"
#include "scene_inst.h"
#include "game_interface.h"

void fill_tail(inner::inner_tail* tail);
void* on_pb_offline(client::offline* p, void* ud, size_t* rsp_len);

void* on_game_pb_addPlayerRestict_req(inner::addPlayerRestict_req* req, void* ud, size_t* rsp_len)
{
    struct game_service_t* ss = NULL;
    ss = (struct game_service_t*)ud;
    int32 size = req->bi_size();
    client::offline off;
    off.set_active(true);
    for (int32 i = 0; i < size; ++i) {
        scene_user* su = ss->sim->get_user(req->bi(i).user().id());
        if (su == NULL) {
            continue;
        }

        su->set_ban(req->bi(i).type(), req->bi(i).time());
        
        if (req->bi(i).type() == player_role::ROLE_BAN_CHAT) {
            continue;
        }
        
        off.set_role_id(su->get_role_id());
        off.mutable_svr_head()->set_role_id(su->get_role_id());
        off.mutable_svr_head()->set_agent_id(su->get_agent_id());
        on_pb_offline(&off, ud, rsp_len);
    }

    return NULL;
}

void* on_game_pb_delPlayerRestict_req(inner::delPlayerRestict_req* req, void* ud, size_t* rsp_len)
{
    struct game_service_t* ss = NULL;
    ss = (struct game_service_t*)ud;
    int32 size = req->bi_size();
    for (int32 i = 0; i < size; ++i) {
        scene_user* su = ss->sim->get_user(req->bi(i).user().id());
        if (su == NULL) {
            continue;
        }
        
        su->set_ban(0, 0);
        su->save_user();
        su->get_player_role().write_all(ss->ctx);
    }

    return NULL;
}
