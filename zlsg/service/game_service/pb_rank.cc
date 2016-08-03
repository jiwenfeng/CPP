#include "game_interface.h"
#include "scene_user.h"
#include "scene_inst.h"

#if 0
void* on_game_pb_rank_vote_req(client::rank_vote_req* req, void* ud, size_t* rsp_len)
{
    PB_FUNC_GUARD(req);

    rank_vote_rsp rsp;
    psu->on_pb_rank_vote_req(req, rsp);
    game_send_client(ss, req->svr_head().client_id(), rsp);
    if (rsp.ret() == 0) {
        game_send_center(ss, *req);
    }
    return NULL;
}
#endif

