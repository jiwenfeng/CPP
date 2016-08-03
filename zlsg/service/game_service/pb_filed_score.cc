#include "game_interface.h"
#include "player_role.h"
#include "player_role_statis_mgr.h"
#include "scene_user.h"
#include "scene_inst.h"

void* on_game_pb_filed_score_exchange_req(client::filed_score_exchange_req* req, void* ud, size_t* rsp_len)
{
    PB_FUNC_GUARD(req);

    client::filed_score_exchange_rsp rsp;
//     uint32 score_to_force = 0;
//     uint32 score_to_exp = 0;
    uint32 score = req->score();
    if (psu->get_filed_score() < score) {
        rsp.set_ret(1);
        game_send_client(ss, req->svr_head().client_id(), rsp);
        return NULL;
    }

    //uint32 num = psu->get_player_role().get_role_statis_mgr().filed_score_exchange();
    //if (num >= 3) {
    //    rsp.set_ret(2);
    //    game_send_client(ss, req->svr_head().client_id(), rsp);
    //    return NULL;
    //}

    //psu->get_player_role().get_role_statis_mgr().add_filed_score_exchange();
    //psu->sub_filed_score(score);
//     switch (score) {
//     case 100:  score_to_exp = 20000; score_to_force = 200; break;
//     case 200:  score_to_exp = 40000; score_to_force = 400; break;
//     case 300:  score_to_exp = 60000; score_to_force = 600; break;
//     case 400:  score_to_exp = 80000; score_to_force = 800; break;
//     case 500:  score_to_exp = 100000; score_to_force = 1000; break;
//     case 600:  score_to_exp = 120000; score_to_force = 1200; break;
//     case 700:  score_to_exp = 140000; score_to_force = 1400; break;
//     case 800:  score_to_exp = 160000; score_to_force = 1600; break;
//     case 900:  score_to_exp = 180000; score_to_force = 1800; break;
//     case 1000: score_to_exp = 200000; score_to_force = 2000; break;
//     default: score_to_exp = 0;  score_to_force = 0; break;
//     }

    if (score == 1000) {
        psu->sys_msg_score(score);
    }
    
    //psu->add_exp(score_to_exp);
//     psu->add_force(score_to_force);
    psu->send_client_change_attr();
    rsp.set_ret(0);
    game_send_client(ss, req->svr_head().client_id(), rsp);

    scene_inst* si = psu->get_scene();
    if (si == NULL){
        return NULL;
    }
    if (si->get_scene().get_scene_id() == 458 && si->get_map().get_id() == 328) {
        client::field_score_msg su_msg;
        client::field_score_info* fsi = su_msg.add_fsi();
        fsi->set_role_id(psu->get_role_id());
        fsi->set_score(psu->get_filed_score());
        fsi->set_type(1);
        si->scene_broadcast(su_msg);
    }
    
    return NULL;
}


void* on_game_pb_filed_score_find_req(client::filed_score_find_req* req, void* ud, size_t* rsp_len)
{
    PB_FUNC_GUARD(req);

    scene_inst* si = psu->get_scene();
    if (si == NULL) {
        return NULL;
    }
    
    client::filed_score_find_rsp rsp;
    std::vector<scene_user*> sus;
    si->get_scene_users(sus);
    size_t size = sus.size();
    for (size_t i = 0; i < size; ++i) {
        for (int32 j = 0; j < req->role_id_size() && j < 3; ++j) {
            if (req->role_id(j) != sus[i]->get_role_id()) {
                continue;
            }
            
            client::filed_score_role_pos* fsrp = rsp.add_fsrp();
            fsrp->set_role_id(sus[i]->get_role_id());
            fsrp->set_x(sus[i]->get_scene_pos().x);
            fsrp->set_y(sus[i]->get_scene_pos().y);
            break;
        }
    }
    
    game_send_client(ss, req->svr_head().client_id(), rsp);

    return NULL;
}

