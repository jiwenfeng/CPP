#include <math.h>
#include "client.promo.pb.h"
#include "player_role.h"
#include "player_role_statis_mgr.h"
#include "game_interface.h"
#include "scene_user.h"
#include "scene_inst.h"

void send_reward_by_code(scene_user* psu)
{
//     const config_sys_reward_mgr& csrm = CONFMGR->get_sys_reward_mgr();
//     const std::vector<config_sys_reward*>* vcsr = csrm.get_csr(1); // ÂÒÐ´µÄ1 ´ýÅäÖÃ
//     if (vcsr == NULL) {
//         return ;
//     }
// 
//     size_t size = vcsr->size();
//     for (size_t i = 0; i < size; ++i) {
//         const std::vector<const mysql::tb_sys_reward_detail*>& vtsrd = vcsr->at(i)->get_tsrd();
//         size_t detail_size = vtsrd.size();
//         for (size_t j = 0; j < detail_size; ++j) {
//             if (vtsrd[j] == NULL) {
//                 continue;
//             }
//                 
//             const mysql::tb_sys_reward_detail* tsrd = vtsrd[j];
//             if (tsrd->reward_type() == REWARD_TYPE_GOODS) {
//                 psu->add_props(tsrd->reward_enum(), tsrd->reward_val(), tsrd->is_binding(), tsrd->quality(), false);
//             } else if (tsrd->reward_type() == REWARD_TYPE_ATTR) {
//                 psu->operate_attr(CALCULATION_ADD, (ROLE_ATTR_E)tsrd->reward_enum(), tsrd->reward_val());
//             }
//         }
// 
//         psu->send_client_change_props_msg();
//     }
}

void send_reward_by_id(scene_user* psu, uint32 reward_id)
{
//     const config_sys_reward_mgr& csrm = CONFMGR->get_sys_reward_mgr();
//     const config_sys_reward* csr = csrm.get_csr_by_id(reward_id);
//     if (csr == NULL || reward_id == 0) {
//         psu->add_props(reward_id, 1);
//         return ;
//     }
// 
//     const std::vector<const mysql::tb_sys_reward_detail*>& vtsrd = csr->get_tsrd();
//     size_t detail_size = vtsrd.size();
//     for (size_t j = 0; j < detail_size; ++j) {
//         const mysql::tb_sys_reward_detail* tsrd = vtsrd[j];
//         if (tsrd == NULL) {
//             continue;
//         }
//                 
//         psu->add_props(tsrd->reward_enum(), tsrd->reward_val(), tsrd->is_binding(), tsrd->quality(), false);
//     }
// 
//     psu->send_client_change_props_msg();
}

bool online_day_reward(scene_user* psu, uint32 day , bool& success, uint32& reward_id, uint32& add_id)
{
    bool ret = false;
    player_role_statis_mgr& prs = psu->get_player_role().get_role_statis_mgr();
    ret = day <= prs.online_day_count();
    if (ret) {
        success = prs.online_day_award(day);
    } else {
        return false;
    }

    switch (day) {
    case 2: {
        reward_id = 13;
        add_id    = 22;
        break;
    }
    case 5: {
        reward_id = 14;
        add_id    = 23;
        break;
    }
    case 10: {
        reward_id = 15;
        add_id    = 24;
        break;
    }
    case 17: {
        reward_id = 16;
        add_id    = 25;
        break;
    }
    case 26: {
        reward_id = 17;
        add_id    = 26;
        break;
    }
    default:
        success = false;
    }

    return success;
}

void* on_game_pb_welfare_online_req(client::welfare_online_req* req, void* ud, size_t* rsp_len)
{
    PB_FUNC_GUARD(req);

    return NULL;
}

bool pack_type(scene_user* psu, uint32 index, bool& success, uint32& reward_id)
{
    success = false;
    static const uint32 reward_ids[] = {41, 401, 402, 403, 404, 405, 523, 524};
    if (index >= (sizeof(reward_ids) / sizeof(uint32))) {
        return false;
    }

    player_role_statis_mgr& prs = psu->get_player_role().get_role_statis_mgr();
    bool recharge = false;
    switch (index) {
    case 0: recharge = prs.get_recharge() > 0;     break;
    case 1: recharge = prs.get_recharge() >= 1000;  break;
    case 2: recharge = prs.get_recharge() >= 2500;  break;
    case 3: recharge = prs.get_recharge() >= 5000;  break;
    case 4: recharge = prs.get_recharge() >= 10000; break;
    case 5: recharge = prs.get_recharge() >= 20000; break;
    case 6: recharge = prs.get_recharge() >= 50000; break;
    case 7: recharge = prs.get_recharge() >= 100000; break;
    }

    if (recharge) {
        success   = prs.welfare_gifts(3 + index);
        reward_id = reward_ids[index];
    }

    return success && recharge;
}

void* on_game_pb_welfare_gifts_req(client::welfare_gifts_req* req, void* ud, size_t* rsp_len)
{
    PB_FUNC_GUARD(req);
    
    welfare_gifts_rsp rsp;
    rsp.set_ret(0);
    player_role_statis_mgr& prs = psu->get_player_role().get_role_statis_mgr();
    bool success = false;
    uint32 reward_id = 0;
    uint32 first_0 = 0, pt = 0;
    std::string nc = "", sc = "";
    if (req->has_first_0()) {
        first_0 = 1;
        success = prs.welfare_gifts(0);
        reward_id = 41;
        rsp.set_first_0(req->first_0());
    } else if (req->has_novice_code() && req->novice_code().size() < 512) {
        nc = req->novice_code();
        success = prs.welfare_gifts(1);
        reward_id = 40;
        rsp.set_novice_code(req->novice_code());
    } else if (req->has_spread_code() && req->novice_code().size() < 512) {
        sc = req->spread_code();
        success = prs.welfare_gifts(2);
        reward_id = 494;
        rsp.set_spread_code(req->spread_code());
    } else if (req->has_pack_type()) {
        pt = req->pack_type();
        pack_type(psu, req->pack_type(), success, reward_id);
        rsp.set_pack_type(req->pack_type());
    }

    if (success) {
        send_reward_by_id(psu, reward_id);
    } else {
        rsp.set_ret(1);
    }
    
    game_send_client(ss, req->svr_head().client_id(), rsp);
    amtsvr_billlog(ss->ctx, "%u|%s|%u|%s|%s|%u|%u", psu->get_role_id(), req->GetTypeName().c_str(), first_0, nc.c_str(), sc.c_str(), pt, rsp.ret());

    return NULL;
}

void* on_game_pb_welfare_yellow_vip_req(client::welfare_yellow_vip_req* req, void* ud, size_t* rsp_len)
{
    PB_FUNC_GUARD(req);

    player_role_statis_mgr& prs = psu->get_player_role().get_role_statis_mgr();
    bool success = false;
    uint32 reward_id = 0;
    welfare_yellow_vip_rsp rsp;
    rsp.set_ret(0);
    uint32 every_day = 0, grow_up = 0, every_day_year = 0, novice = 0;
    if (req->has_every_day()) {
        every_day = req->every_day();
        success = prs.yellow_vip_every_day();
        reward_id = 367;        
        rsp.set_every_day(req->every_day());
    } else if (req->has_grow_up()) {
        grow_up = req->grow_up();
        if (req->grow_up() + 1 > 6) {
            rsp.set_ret(3);
            return NULL;
        }
        
        success = prs.yellow_vip_gifts(req->grow_up() + 1);
        reward_id = 377 + req->grow_up();
        reward_id = reward_id < 382 ? reward_id : 382;
        rsp.set_grow_up(req->has_grow_up());
    } else if (req->has_ever_day_year()) {
        every_day_year = 1;
        success = prs.yellow_vip_year_every_day();
        reward_id = 375;
        rsp.set_ever_day_year(req->has_ever_day_year());
    } else if (req->has_novice()) {
        novice = 1;
        success = prs.yellow_vip_gifts(0);
        reward_id = 376;
        rsp.set_novice(req->has_novice());
    }

    if (success) {
        send_reward_by_id(psu, reward_id);
    } else {
        rsp.set_ret(1);
    }
    
    game_send_client(ss, req->svr_head().client_id(), rsp);
    amtsvr_billlog(ss->ctx, "%u|%s|%u|%u|%u|%u|%u", psu->get_role_id(), req->GetTypeName().c_str(), every_day, grow_up, every_day_year, novice, rsp.ret());

    return NULL;
}

void* on_game_pb_online_gifts_req(client::online_gifts_req* req, void* ud, size_t* rsp_len)
{
    PB_FUNC_GUARD(req);

    online_gifts_rsp rsp;
    rsp.set_online_time(req->online_time());
    rsp.set_ret(0);
    uint32 index = 0;
    uint32 reward_id = 0;
    switch (req->online_time()) {
    case 1:  index = 0; reward_id = 383; break;
    case 5:  index = 1; reward_id = 384; break;
    case 15: index = 2; reward_id = 385; break;
    case 30: index = 3; reward_id = 386; break;
    case 60: index = 4; reward_id = 387; break;
    default: index = 0; reward_id = 388; break;
    }
    
    player_role_statis_mgr& prs = psu->get_player_role().get_role_statis_mgr();
    if (prs.online_gifts_record(index)) {
        send_reward_by_id(psu, reward_id);
    } else {
        rsp.set_ret(1);
    }
    
    game_send_client(ss, req->svr_head().client_id(), rsp);
    amtsvr_billlog(ss->ctx, "%u|%s|%u|%u", psu->get_role_id(), req->GetTypeName().c_str(), req->online_time(), rsp.ret());

    return NULL;
}

bool test_gifts(scene_user* psu, uint32 id) // ·â²âÔª±¦Àñ°ü
{
    /*
    static const uint32 ids[] = {389, 390, 391};
    static const uint32 size  = 3;
    for (uint32 i = 0; i < size; ++i) {
        if (ids[i] != id) {
            continue;
        }

        return psu->get_player_role().get_role_statis_mgr().test_gifts_record(i);
        }*/

    return false;
}

bool activity_gifts(scene_user* psu, uint32 id) 
{
    static const uint32 ids[] = {395, 396, 397, 398, 399, 400};
    static const uint32 val[] = {15 , 50 , 100, 150, 200, 260};
    static const uint32 size  = 6;
    for (uint32 i = 0; i < size; ++i) {
        if (ids[i] != id) {
            continue;
        }

        uint32 activity = psu->get_player_role().get_role_statis_mgr().get_activity();
        if (activity < val[i]) {
            return false;
        }

        return psu->get_player_role().get_role_statis_mgr().activity_gifts(i);
    }

    return false;
}

// bool check_target(scene_user* psu, const mysql::tb_target* tt) {
//     bool ret = false;
//     player_role_target& target = psu->get_player_role().get_role_statis_mgr().get_target();
//     uint32 count = tt->count();
//     switch (tt->type()) {
//     case  1: ret = target.get_pet_fight()          >= count; break;
//     // case  2: ret = psu->get_attr().strength()       >= count; break;
//     case  3: ret = target.get_kill_monster()       >= count; break;
//     case  4: ret = psu->get_attr().level()          >= count; break;
//     case  5: ret = target.get_use_aoe_skill()      >= count; break;
//     case  6: ret = target.get_open_pulse()         >= count; break;
//     case  7: ret = target.get_equip_temp_wing()    >= count; break;
//     case  8: ret = target.get_add_friend()         >= count; break;
//     case  9: ret = target.get_use_unique_skill()   >= count; break;
//     case 10: ret = target.get_join_family()        >= count; break;
//     case 11: ret = target.get_enter_pet_raid()     >= count; break;
//     case 12: ret = target.get_use_trans()          >= count; break;
//     case 13: ret = psu->raid_start(tt->id())        >= count; break;
//     case 14: ret = target.get_equip_ring()         >= count; break;
//     case 15: ret = target.get_enter_family_scene() >= count; break;
//     case 16: ret = target.get_learn_skill()        >= count; break;
//     case 17: ret = target.get_enter_money_raid()   >= count; break;
//     case 18: ret = target.get_join_battle()        >= count; break;
//     case 19: ret = target.get_equip_wing()         >= count; break;
//     case 20: ret = target.get_enter_team_tower()   >= count; break;
//     case 21: ret = target.get_kill_boss() & 1 ? true : false; break;
//     case 22: ret = target.get_kill_role()          >= count; break;
//     case 23: ret = target.get_equip_quality()      >= count; break;
//     case 24: ret = target.get_explore()            >= count; break;
//     case 25: ret = target.get_equip_on()           >= count; break;
//     case 26: ret = target.get_equip_enhance()      >= count; break;
//     case 27: ret = target.get_wing_level()         >= count; break;
//     case 28: ret = target.get_pet_advanced()       >= count; break;
//     case 29: ret = target.get_join_siege()         >= count; break;
//     case 30: ret = target.get_join_domain()        >= count; break;
//     case 31: ret = target.get_kill_boss() & 2 ? true : false; break;
//     case 32: ret = target.get_kill_elite()         >= count; break;
//     case 33: ret = target.get_pet_quality()        >= count; break;
//     case 34: ret = psu->raid_start(tt->id())        >= count; break;
//     case 35: ret = target.get_equip_sublime()      >= count; break;
//     case 36: ret = target.get_explore()       >= count; break;
//         //case 36: ret = target.get_high_explore()       >= count; break;
//     case 37: ret = target.get_kill_boss() & 4 ? true : false; break;
//     case 38: ret = target.get_equip_reset()        >= count; break;
//     case 39: ret = target.get_lnlaid_stone()       >= count; break;
//     default: ret = false;
//     }
// 
//     return ret;
// }

bool target_gifts(scene_user* psu, uint32 id)
{
//     vector<const mysql::tb_target*> vt;
//     const mysql::tb_target* tt = NULL;
//     size_t size = vt.size();
//     if (tt == NULL && size == 0) {
//         return false;
//     }
// 
//     bool ret = false;
//     if (size > 0 && vt[0] != NULL) {
//         uint32 lvl = vt[0]->lvl() / 10;
//         if (!psu->get_player_role().get_role_statis_mgr().target_add_gifts(lvl)) {
//             return false;
//         }
// 
//         for (size_t i = 0; i < size; ++i) {
//             if (!check_target(psu, vt[i])) {
//                 return false;
//             }
//         }
// 
//         ret = true;
//         /*static const SYS_MSG_ID[] = {44, 45, 46, 47, 56, 57, 58, 59};
//           if (lvl < sizeof(SYS_MSG_ID)) {
//           psu->sys_msg_target(SYS_MSG_ID[lvl]);
//           }*/
//     } else if (tt != NULL) {
//         if (!psu->get_player_role().get_role_statis_mgr().target_gifts(tt->id())) {
//             return false;
//         }
// 
//         ret = check_target(psu, tt);
//     }

//     return ret;
    return false;
}

bool promo_wing_and_aircraft(struct game_service_t* ss, scene_user* psu, uint32 type, uint32 id, uint32 lv)
{
    uint32 now = (uint32)time(NULL);
    uint32 day = amtsvr_svr_start_day(now);
    if (day > 8) {
        return false;
    }
    
    uint32 lvl  = 0;
    bool reward = false;
    //if (type == RANK_WING_POWER) {
    //    reward = psu->get_player_role().get_role_statis_mgr().get_promo_wing(id);
    //    lvl = 0;
    //} else if (type == RANK_AIRCRAFT) {
    //    reward = psu->get_player_role().get_role_statis_mgr().get_promo_aircraft(id);
    //}

    if (!reward) {
        return reward;
    }

    client::get_gifts_rsp rsp;
    rsp.set_reward_id(id);
    if (lvl == lv) {
        psu->add_props(id, 1);
        rsp.set_ret(0);
    } else {
        rsp.set_ret(1);
    }
    
    psu->send_client(rsp);
    return reward;
}

bool promo_gifts(struct game_service_t* ss, scene_user* su, uint32 id)
{
    return false;
}

bool invest_ret(scene_user* psu, uint32 id)
{
    if (!psu->get_player_role().get_role_statis_mgr().is_join_invest()) {
        return false;
    }
    
    static const uint32 ids[] = {501, 502, 503, 504, 505, 506, 507, 508, 509, 510};
    static const uint32 day[] = {  0,   1,   2,   4,   6,   9,  13,  17,  22,  30};
    static const uint32 size  = 10;
    for (uint32 i = 0; i < size; ++i) {
        if (ids[i] != id) {
            continue;
        }

        uint32 join = psu->get_player_role().get_role_statis_mgr().get_invest_join_time();
        uint32 now  = (uint32)time(NULL);
        uint32 join_day = (join + (8 * 3600)) / 86400;
        uint32 now_day  = (now  + (8 * 3600)) / 86400;
        if (join_day > now_day || (now_day - join_day) < day[i]) {
            break;
        }

        return psu->get_player_role().get_role_statis_mgr().invest_ret(i + 1);
    }
    
    return false;
}

bool mooncake_reward(struct game_service_t* ss, scene_user* psu, uint32 id)
{
	return false;

    //if (id < 513 || id > 522) {
    //    return false;
    //}

    //time_t now = time(NULL);
    //struct tm t;
    //localtime_r(&now, &t);
    //if (t.tm_year <= 113 && t.tm_mon <= 8 && t.tm_mday < 23) {
    //    return false;
    //}

    //if (!psu->get_player_role().get_role_statis_mgr().get_mooncake_reward()) {
    //    return false;
    //}

    //uint32 type = (uint32)client::RANK_MOON;
   // uint32 rank = id - 512;
   // client::rank_check_req req;
    //req.set_type(type);
    //req.set_role_id(psu->get_role_id());	
    //req.set_rank(rank);
    //req.set_reward(id);
    //game_send_center(ss, req);

    return true;
}

void* on_game_pb_get_gifts_req(client::get_gifts_req* req, void* ud, size_t* rsp_len)
{
    PB_FUNC_GUARD(req);

    bool ret = false;
    /*if (!ret) {
        ret = test_gifts(psu, req->reward_id());
        }*/
 
    if (!ret) {
        ret = activity_gifts(psu, req->reward_id());
    }

    if (!ret) {
        ret = target_gifts(psu, req->reward_id());
    }

    if (!ret) {
        if (promo_gifts(ss, psu, req->reward_id())) {
            return NULL;
        }
    }

    if (!ret) {
        if (mooncake_reward(ss, psu, req->reward_id())) {
            return NULL;
        }
    }

    if (!ret) {
        ret = invest_ret(psu, req->reward_id());
    }
    
    client::get_gifts_rsp rsp;
    rsp.set_reward_id(req->reward_id());
    if (ret) {
        send_reward_by_id(psu, req->reward_id());
        rsp.set_ret(0);
    } else {
        rsp.set_ret(1);
    }
    
    game_send_client(ss, req->svr_head().client_id(), rsp);
    amtsvr_billlog(ss->ctx, "%u|%s|%u|%u", psu->get_role_id(), req->GetTypeName().c_str(), req->reward_id(), rsp.ret());

    return NULL;
}

void* on_game_pb_check_list_of_names_rsp(client::check_list_of_names_rsp* rsp, void* ud, size_t* rsp_len)
{
    struct game_service_t* ss = NULL;                               
    ss = (struct game_service_t*)ud;                                
    scene_user* psu = ss->sim->get_user(rsp->role_id());  
    if (psu == NULL) {                                                
        return NULL;                                                
    }

    client::get_gifts_rsp ret;
    ret.set_reward_id(rsp->reward());
    if (rsp->check())  {
        send_reward_by_id(psu, rsp->reward());
        ret.set_ret(0);
    } else {
        ret.set_ret(1);
    }

    psu->send_client(ret);
    amtsvr_billlog(ss->ctx, "%u|%s|%u|%u", psu->get_role_id(), "client.get_gifts_req", rsp->reward(), ret.ret());

    return NULL;
}

#if 0
void* on_game_pb_rank_check_rsp(client::rank_check_rsp* rsp, void* ud, size_t* rsp_len)
{
    struct game_service_t* ss = NULL;                               
    ss = (struct game_service_t*)ud;                                
    scene_user* psu = ss->sim->get_user(rsp->role_id());  
    if (psu == NULL) {                                                
        return NULL;                                                
    }

    client::get_gifts_rsp ret;
    ret.set_reward_id(rsp->reward());
    if (rsp->check())  {
        send_reward_by_id(psu, rsp->reward());
        ret.set_ret(0);
    } else {
        ret.set_ret(1);
    }

    psu->send_client(ret);
    amtsvr_billlog(ss->ctx, "%u|%s|%u|%u", psu->get_role_id(), "client.get_gifts_req", rsp->reward(), ret.ret());

    return NULL;
}
#endif
