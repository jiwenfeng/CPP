#include "origin_npc.h"
#include "player_role.h"
#include "player_role_statis_mgr.h"
#include "scene_npc.h"
#include "scene_user.h"
#include "family_scene_inst.h"

family_scene_inst::family_scene_inst(uint32 id, const game_map* p, const raid_scene* s, struct amtsvr_context* c, const int m)
    : raid_scene_inst(id, p, s, c, m), _rsc(s)
{
    call_boss  = false;
    _close_inst = false;
    boss_time = 0;

    _inferno_struck = false;
    _chief_id = 0;
    _frost_freeze = family_scene_inst::MAX_FROST_FREEZE;
    _space_broken.clear();
    _leave_monster = 0;
    _leave_boss    = 0;
    _cur_monster   = 0;
    _cur_boss      = 0;
    _kill_monster.clear();
    _kill_boss.clear();
    _is_monster_score_update = false;
    _countdown     = 30;
    _round         = 1;
    _round_dead_npc_count = 0;
    _leave_npc.clear();
    _move_speed_recover_time = 0;
    _old_move_speed = 0;
}

family_scene_inst::~family_scene_inst()
{

}

int family_scene_inst::init()
{
    //client::family_monster_boss_req req;
    //req.set_family_id(_family_id);
    //send_center(req);

    client::family_req via;
    via.add_family_id(_family_id);
    via.set_scene_id(FAMILY_SCENE_ID);
    send_center(via);
    
    return scene_inst::init();
}

int family_scene_inst::enter_scene(scene_user* u)
{
    if (u != NULL) {
        player_role& pr = u->get_player_role();
        if (pr.get_role_statis_mgr().family_raid()) {
            u->add_activity(ACTIVITY_FAMILY_RAID);
        }
    }

    return scene_inst::enter_scene(u);
}

void family_scene_inst::on_role_leave(scene_role* psu)
{
    return scene_inst::on_role_leave(psu);
}

int family_scene_inst::update(uint32 tick, bool is_one_sec, bool is_fiv_sec, bool is_ten_sec, bool is_thirty_sec, bool is_one_minute)
{
    if (is_one_minute){
        uint32 now = (uint32)time(NULL);
        vector<scene_npc*> npc_list;
        shobj* cur = _npc_map.get_tick_objs_head();
        while (cur != NULL){
            scene_npc* sn = static_cast<scene_npc*>(cur->sr);
            if (sn->is_must_leave(now)){
                npc_list.push_back(sn);
            }   
            cur = cur->next[SLPE_TICK];
        }

        uint32 size = npc_list.size();
        for (uint32 i = 0; i < size; ++i){
            scene_inst::leave_scene(npc_list[i]);
            npc_list[i]->detach_scene();
            delete npc_list[i];
        }
    }
    
    if (_inferno_struck) {
        leave_npc();
        if (is_one_sec) {
            monster_score_msg();
            monster_countdown_msg();
            recover_move_speed();
        }
    }
   
    scene_inst::update(tick, is_one_sec, is_fiv_sec, is_ten_sec, is_thirty_sec, is_one_minute);
    if (is_one_minute)  {
        call_boss_msg();
    }
    
    return 0;
}

int family_scene_inst::on_kill_npc(scene_user* atk, scene_npc* npc)
{
    if (atk == NULL || npc == NULL) {
        return -1;
    }

    remove_tick_npc(npc);
    
    uint32 role_id = atk->get_role_id();
    if (npc->get_origin_npc().is_boss()) {
        ++_kill_boss[role_id];
        --_cur_boss;
    } else {
        ++_kill_monster[role_id];
        --_cur_monster;
    }

    result_msg(atk);
    ++_round_dead_npc_count;

    next_round();

    return 0;
}

int family_scene_inst::on_role_move(scene_role* r, const map_postion& from, const map_postion& to)
{
    scene_inst::on_role_move(r, from, to);
    if (r->get_role_type() != RTE_NPC) {
        return 0;
    }

    scene_npc* sn = static_cast<scene_npc*>(r);
    if (to.x >= 800 && to.x <= 1000 && to.y >= 600 && to.y <= 700) {
        remove_tick_npc(sn);
        _leave_npc.push_back(r->get_inst_id());
    }

    return 0;
}

void family_scene_inst::set_boss_time(uint32 h, uint32 m, uint32 s)
{
    boss_time_h = h;
    boss_time_m = m;
    boss_time_s = s;
}

void family_scene_inst::call_boss_msg()
{
    //if (call_boss) {
    //    uint32 now = (uint32)time(NULL);
    //    if (now >= boss_time) {
    //        call_boss = false;
    //        client::family_monster_boss_call_jump_req req;
    //        req.set_family_id(_family_id);
    //        req.set_call(0);
    //        send_center(req);
    //    }

    //    return ;
    //}
    //
    //uint32 mt  = time_utils::make_time(0, boss_time_h, boss_time_m, boss_time_s);
    //if (boss_time < mt) {
    //    boss_time = mt;
    //    call_boss = true;
    //} 
}

uint32 family_scene_inst::family_power_to_boss_id(uint32 family_power)
{
    uint32 boss_id = 1557;
    if (family_power < 200000) {
        boss_id = 1557;//1323;
    } else if (family_power < 4800000) {
        boss_id = 1558;//1324;
    } else if (family_power < 1500000) {
        boss_id = 1559;//1325;
    } else if (family_power < 3500000) {
        boss_id = 1560;//1326;
    } else if (family_power < 6000000) {
        boss_id = 1561;//1327;
    } else {//if (family_power <= 6000000) {
        boss_id = 1562;//1328;
    }

    return boss_id;
}

//int family_scene_inst::open(client::inferno_struck_open_via* via)
//{
//    if (via == NULL) {
//        return -1;
//    }
//    
//    _inferno_struck = true;
//    _chief_id = via->role_id();
//    _frost_freeze = family_scene_inst::MAX_FROST_FREEZE;
//    _space_broken.clear();
//    _leave_monster = 0;
//    _leave_boss    = 0;
//    _cur_monster   = 0;
//    _cur_boss      = 0;
//    _kill_monster.clear();
//    _kill_boss.clear();
//    _is_monster_score_update = false;
//    _countdown     = 30;
//    /*#ifdef __INNER_DEBUG__PACKAGE_
//    _countdown     = 10;
//    #endif*/
//    _round         = 1;
//    _round_dead_npc_count = 0;
//    _leave_npc.clear();
//    _move_speed_recover_time = 0;
//    _old_move_speed = 0;
//
//    return 0;
//}

//int family_scene_inst::skill(scene_user* su, client::inferno_struck_skill_req* req, client::inferno_struck_skill_rsp& rsp)
//{
//    if (su == NULL || req == NULL) {
//        return -1;
//    }
//
//    uint32 role_id = req->svr_head().role_id();
//    rsp.set_op(req->op());
//    rsp.set_role_id(role_id);
//    if (req->op() == 0) { // 查询
//        rsp.set_ret(0);
//        if (req->type() == 0) {
//            rsp.set_num(get_space_broken(role_id));
//        } else if (req->type() == 1) {
//            if (role_id == _chief_id) {
//                rsp.set_num(_frost_freeze);
//            } else {
//                rsp.set_num(0);
//            }
//        }
//
//        return 0;
//    } else { // 使用
//        ROLE_ATTRIBUTE money_type = TAEL;
//        uint32       money      = 0;
//        uint32       num        = 0;
//        uint32       op         = req->op();
//        if (req->type() == 0) { // 空间碎裂
//            num = get_space_broken(role_id);
//
//            if (op == 1) { 
//                money_type = TAEL;
//                money      = 50000;
//            } else if (op == 2) {
//                money_type = INGOT;
//                money      = 20;
//            }
//        } else if (req->type() == 1) { // 冰霜冻结
//            num = _frost_freeze;
//
//            if (op == 1) { 
//                money_type = TAEL;
//                money      = 100000;
//            } else if (op == 2) {
//                money_type = INGOT;
//                money      = 50;
//            }
//        }
//
//        if (num == 0) { // 次数不足
//            rsp.set_ret(1);
//            rsp.set_num(0);
//            return -2;
//        } 
//
//        if (su->sub_money(money_type, money) == 0) {
//            if (req->type() == 0) {
//                set_space_broken(role_id, --num);
//                use_space_broken(op);
//            } else {
//                _frost_freeze = --num;
//                use_frost_freeze(op);
//            }
//
//            rsp.set_ret(0);
//            rsp.set_num(num);
//
//            return 1;
//        } else {
//            rsp.set_ret(op == 1 ? 2 : 3);
//            rsp.set_num(num);
//        }
//
//        return 0;
//    }
//    
//    return 0;
//}

void family_scene_inst::monster_score_msg()
{
    //inferno_struck_monster_score_msg msg;
    //msg.set_score(get_monster_score());
    //msg.set_leave_monster(_leave_monster);
    //msg.set_leave_boss(_leave_boss);
    //msg.set_cur_monster(_cur_monster);
    //msg.set_cur_boss(_cur_boss);
    //uint32 kill_monster = get_kill_monster();
    //msg.set_kill_monster(kill_monster);
    //uint32 kill_boss = get_kill_boss();
    //msg.set_kill_boss(kill_boss);
    //scene_broadcast(msg);
}

void family_scene_inst::monster_countdown_msg()
{
    //if (_countdown == 0) {
    //    return ;
    //}
    //
    //inferno_struck_monster_countdown_msg msg;
    //msg.set_countdown(--_countdown);
    //msg.set_round(_round);
    //scene_broadcast(msg);
    //if (_countdown == 0) {
    //    refresh_npc();
    //}
}

void family_scene_inst::result_msg(scene_user* su)
{
    //if (su == NULL) {
    //    return ;
    //}
    //
    //inferno_struck_result_msg msg;
    //uint32 kill_monster = get_kill_monster(su->get_role_id());
    //uint32 kill_boss    = get_kill_boss(su->get_role_id());
    //msg.set_kill_monster(kill_monster);
    //msg.set_kill_boss(kill_boss);
    //msg.set_contribution(get_contribution(kill_monster, kill_boss));
    //su->send_client(msg);
}

uint32 family_scene_inst::get_space_broken(uint32 role_id)
{
    std::map<uint32, uint32>::iterator i = _space_broken.find(role_id);
    if (i == _space_broken.end()) {
        uint32 num = family_scene_inst::MAX_SPACE_BROKEN;
        _space_broken.insert(std::pair<uint32, uint32>(role_id, num));
        return get_space_broken(role_id);
    }

    return i->second;
}

void family_scene_inst::set_space_broken(uint32 role_id, uint32 num) 
{
    _space_broken[role_id] = num;
}

uint32 family_scene_inst::get_monster_score()
{
    return (_leave_monster * 1) + (_leave_boss * 1);
}

uint32 family_scene_inst::get_kill_monster(uint32 role_id/* = 0*/)
{
    uint32 num = 0;
    std::map<uint32, uint32>::iterator i = _kill_monster.end();
    if (role_id == 0) {
        i = _kill_monster.begin();
        for (; i != _kill_monster.end(); ++i) {
            num += i->second;
        }
    } else {
        i = _kill_monster.find(role_id);
        if (i != _kill_monster.end()) {
            num += i->second;
        }
    }
    
    return num;
}

uint32 family_scene_inst::get_kill_boss(uint32 role_id/* = 0*/)
{
    uint32 num = 0;
    std::map<uint32, uint32>::iterator i = _kill_boss.end();
    if (role_id == 0) {
        i = _kill_boss.begin();
        for (; i != _kill_boss.end(); ++i) {
            num += i->second;
        }
    } else {
        i = _kill_boss.find(role_id);
        if (i != _kill_boss.end()) {
            num += i->second;
        }
    }
    
    return num;
}

uint32 family_scene_inst::get_contribution(uint32 kill_monster, uint32 kill_boss)
{
    return (kill_monster * 1) + (kill_boss * 20);
}

void family_scene_inst::refresh_npc()
{
    clear_monster();
}

void family_scene_inst::result(uint32 win)
{
    //if (!_inferno_struck) {
    //    return ;
    //}

    //monster_score_msg();

    //client::inferno_struck_over_msg msg;
    //msg.set_ret(win);
    //scene_broadcast(msg);

    //_inferno_struck = false;

    //uint32 kill_monster = get_kill_monster();
    //uint32 kill_boss    = get_kill_boss();
    //client::inferno_struck_family_money fm;
    //fm.set_family_id(_family_id);
    //fm.set_money(kill_monster * 1 + kill_boss * 10);
    //send_center(fm);
}

void family_scene_inst::leave_npc()
{
    for (size_t i = 0; i < _leave_npc.size(); ++i) {
        scene_npc* sn = get_scene_npc(_leave_npc[i]);
        if (sn == NULL){
            continue;
        }

        if (!sn->get_status().is_dead()) {
            ++_round_dead_npc_count;
            if (sn->get_origin_npc().is_boss()) {
                ++_leave_boss;
                --_cur_boss;
            } else {
                ++_leave_monster;
                --_cur_monster;
            }
        }
        
        scene_inst::leave_scene(sn);
        delete sn;
    }

    _leave_npc.clear();
    
    if (get_monster_score() >= MAX_MONSTER_SCORE) {
        result(0);
        return ;
    }
        
    next_round();
}

void family_scene_inst::use_space_broken(uint32 type)
{
    shobj* cur = _npc_map.get_all_objs_head();
    for (; cur != NULL; cur = cur->next[SLPE_LIST]) {
        if (cur->sr == NULL) {
            continue;
        }

//         scene_npc* sn = static_cast<scene_npc*>(cur->sr);
        if (type == 1) {
//             sn->set_hp((uint32)(sn->get_attr().get_val(ATTR_CUR_LIFE) * 0.95f));
        } else {
//             sn->set_hp((uint32)(sn->get_attr().get_val(ATTR_CUR_LIFE) * 0.90f));
        }
    }
}

void family_scene_inst::use_frost_freeze(uint32 type)
{
    shobj* cur = _npc_map.get_all_objs_head();
    for (; cur != NULL; cur = cur->next[SLPE_LIST]) {
        if (cur->sr == NULL) {
            continue;
        }

        scene_npc* sn = static_cast<scene_npc*>(cur->sr);
        _old_move_speed = sn->get_attr(MOVE_SPEED);
//         sn->set_move_speed(1);
        if (type == 1) {
            _move_speed_recover_time += 30;
        } else {
            _move_speed_recover_time += 90;
        }
    }
}

void family_scene_inst::recover_move_speed()
{
    if (_move_speed_recover_time == 0) {
        return ;
    }

    --_move_speed_recover_time;
    if (_move_speed_recover_time != 0) {
        return ;
    }
    
    shobj* cur = _npc_map.get_all_objs_head();
    for (; cur != NULL; cur = cur->next[SLPE_LIST]) {
        if (cur->sr == NULL) {
            continue;
        }

//         scene_npc* sn = static_cast<scene_npc*>(cur->sr);
//         sn->set_move_speed(_old_move_speed);
    }
}

void family_scene_inst::next_round()
{
//     uint32 size = rsc->get_batch_npc_num(_round);
//     if (_round_dead_npc_count < size){
//         return ;
//     }
// 
//     if (_round == rsc->get_batch_count()) {
//         sys_msg_inferno_struck_win();
//         result(1);
//         return ;
//     }

    if (_round % 10 == 0) {
        sys_msg_inferno_struck_round();
    }

    _round_dead_npc_count = 0;
    _countdown = 20;
    ++_round;
}
    
void family_scene_inst::sys_msg_inferno_struck_round()
{
    char cmd[128] = {0};
    snprintf(cmd, sizeof(cmd), "%s|%u|%u",  family_name.c_str(), _family_id, _round);
    send_sys_msg(336, cmd);
}

void family_scene_inst::sys_msg_inferno_struck_win()
{
    char cmd[128] = {0};
    snprintf(cmd, sizeof(cmd), "%s|%u",  family_name.c_str(), _family_id);
    send_sys_msg(337, cmd);
}
