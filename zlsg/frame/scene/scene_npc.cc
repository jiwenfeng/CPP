#include <limits.h>
#include <math.h>
#include "random.h"
#include "config_map.h"
#include "config_npc.h"
#include "config_fallitem.h"
#include "config_props.h"
#include "client.chat_msg.pb.h"
#include "player_role.h"
#include "player_role_statis_mgr.h"
#include "scene.h"
#include "game_map.h"
#include "origin_npc.h"
#include "scene_user.h"
#include "scene_lose_mgr.h"
#include "scene_inst.h"
#include "scene_npc.h"

const uint32 scene_npc::BACK_LIMIT = 2;

scene_npc::scene_npc(const origin_npc& n, const uint64 id) 
    : scene_role(id)
    , origin_self(n)
{
    operator=(n);

    _killer         = 0;
    _leave_time     = 0;
    _reative_atk    = true;
    _atk_id         = 0;
    _d              = 0;
    _attacked       = false;
    _rand_move_tick = 0;
    _last_ai_tick   = 0;
    _elite          = false;
    _disappear_tick = time_utils::tenms();
    _stop_tick = 0;
    _hide_tick = 0;
    _master_tick = 0;
	_ai_expire = 0;
    set_role_type(RTE_NPC);
    set_npc_status(NPC_STS_STAND);
    set_scene_pos(get_born_pos());
	_cp.clear();
    _target_inst_id = 0;
    _is_fiv_sec     = false;
}

scene_npc::~scene_npc()
{
	_cp.clear();
}

scene_npc& scene_npc::operator=(const origin_npc& rhs)
{
    role::operator=(rhs);

    return *this;
}

int scene_npc::init()
{
    return 0;
}

bool scene_npc::on_update(uint32 tick, uint32 user_num, bool is_one_sec, bool is_fiv_sec, bool is_ten_sec, bool is_thirty_sec, bool is_one_minute)
{
    if (user_num == 0) {
        return true;
    }

    _is_fiv_sec = is_fiv_sec;
    //if (get_role_type() == RTE_PET) {
    //    static const char* NPC_STS_STRING[] = {"null", "stand", "move", "fight", "dead", "force_move", "rand_move", "back_move", "path_move", "idle", "in_block"};
    //    amtsvr_log("%llu %u %s\n", get_inst_id(), get_role_id(), NPC_STS_STRING[get_npc_status()]);
    //}

    bool ret = true;
    switch (get_npc_status()) {
    case NPC_STS_STAND: {
        ret = on_status_stand(tick);
        break;
                        }
    case NPC_STS_MOVE: {
        ret = on_status_move(tick);
        break;
                       }
    case NPC_STS_FIGHT: {
        ret = on_status_fight(tick);
        break;
                        }
    case NPC_STS_DEAD: {
        ret = on_status_dead(tick);
		if(is_one_sec)
		{
			send_whip_reward();
		}
        break;
                       }
    case NPC_STS_FORCE_MOVE: {
        ret = on_status_force_move(tick);
        break;
                             }
    case NPC_STS_RAND_MOVE: {
        ret = on_status_rand_move(tick);
        break;
                            }
    case NPC_STS_BACK_MOVE: {
        ret = on_status_back_move(tick);
        break;
                            }
    case NPC_STS_PATH_MOVE: {
        ret = on_status_path_move(tick);
        break;
                            }
    case NPC_STS_IDLE: {
        ret = on_status_idle(tick);
        break;
                       }
    case NPC_STS_IN_BLOCK: {
        ret = on_status_in_block(tick);
        break;
                           }
    case NPC_STS_HIDE: {
        ret = on_status_hide(tick);
        break;
                       }
    default: break;
    }

//     if (!ret) {
//         return scene_role::on_update(tick, user_num, is_one_sec, is_fiv_sec, is_ten_sec, is_thirty_sec, is_one_minute);
//     }

    return scene_role::on_update(tick, user_num, is_one_sec, is_fiv_sec, is_ten_sec, is_thirty_sec, is_one_minute);
}

bool scene_npc::on_update_100ms(uint32 tick, uint32 user_num)
{
    run_ai(tick);

    return scene_role::on_update_100ms(tick, user_num);
}

bool scene_npc::on_update_disappear(uint32 tick, uint32 user_num)
{
	if(get_status().is_dead()) {
		return false;
	}

    if (get_origin_npc().get_disappear() > 0) {
        if (_disappear_tick == 0 || _disappear_tick > tick) {
            _disappear_tick = tick;
        }

        uint32 space_tick = tick - _disappear_tick;
        _disappear_tick = tick;
        if (get_origin_npc().get_disappear() > space_tick) {
            get_origin_npc().set_disappear(get_origin_npc().get_disappear() - space_tick);
        } else {
            get_origin_npc().set_disappear(1);
        }

        if (get_origin_npc().get_disappear() == 1) {
            get_origin_npc().set_disappear(0);
            set_disappear();
        }
    }

	return scene_role::on_update_disappear(tick, user_num);
}

bool scene_npc::on_update_one_second(uint32 tick, uint32 user_num)
{
    ai_master(tick);
	ai_expire(tick);
    return scene_role::on_update_one_second(tick, user_num);
}

const char *scene_npc::get_role_name()const
{
	const config_npc *cn = const_cast<scene_npc *>(this)->get_origin_npc().get_config_npc();
	if(NULL == cn) {
		return "";
	}

	return cn->get_npc_name().c_str();
}

scene_role *scene_npc::get_hatest_role(int limit_level)
{
	if(get_scene() == NULL || get_origin_npc().get_attk_type() == NATE_DEFENCE) {
		return NULL;
	}

    uint64      hatest_role = 0;
    scene_role* sr          = NULL;
	uint32 lv1 = get_attr().get_val(LEVEL);
    while ((hatest_role = hate_man::get_hate_role()) != 0) {
        sr = get_scene()->get_scene_role(hatest_role);
        if (sr != NULL
            && !sr->get_status().is_dead()
            && (_attacked || is_in_chase_range_cell(get_scene()->to_cell_pos(sr->get_scene_pos()), get_origin_npc().get_chase_range()))
            && get_scene_pos().is_in_same_flat(sr->get_scene_pos())
			&& abs((int)(lv1) - (int)(sr->get_attr().get_val(LEVEL))) <= limit_level) { // 角色在场景内, 没有死亡, 已经被攻击或者在追击范围内, 在同一平面内
                break;
        }

        del_hate_role(hatest_role);
        sr = NULL;
    }

    return sr;
}

scene_role* scene_npc::get_hatest_role()
{
    if (get_scene() == NULL || get_origin_npc().get_attk_type() == NATE_DEFENCE) {
        return NULL;
    }

    uint64      hatest_role = 0;
    scene_role* sr          = NULL;
    while ((hatest_role = hate_man::get_hate_role()) != 0) {
        sr = get_scene()->get_scene_role(hatest_role);
        if (sr != NULL
            && !sr->get_status().is_dead()
            && (_attacked || is_in_chase_range_cell(get_scene()->to_cell_pos(sr->get_scene_pos()), get_origin_npc().get_chase_range()))
            && get_scene_pos().is_in_same_flat(sr->get_scene_pos())) { // 角色在场景内, 没有死亡, 已经被攻击或者在追击范围内, 在同一平面内
                break;
        }

        del_hate_role(hatest_role);
        sr = NULL;
    }

    return sr;
}

bool scene_npc::chase_role_by_cell(uint32 tick)
{
    scene_role* target = get_hatest_role();
    if (target == NULL || get_scene() == NULL) {
        return false;
    }

    if (!can_attack()) {
        return true;
    }

    uint32 ar = 0;
    skill* sk = NULL;
    if (tick - get_last_attack_tick() >= get_origin_npc().get_attack_cool()) {
        sk = mutable_skill()->get_skill();
    }

    if (sk != NULL) {
        ar = sk->get_distance();
    } else {
        ar = get_skill().get_min_atk_range();
    }

    cell_pos cp = get_scene()->to_cell_pos(target->get_scene_pos());
    if (is_in_range_cell(&cp, ar)) {
        if (get_move_count() > 1) {
            stand();
        }

        _d  = 0;
        set_npc_status(NPC_STS_FIGHT);
        broadcast_hate_list(target);
        return attack(sk, target, tick);
    }

    move_to_cell(cp);

    return true;
}

bool scene_npc::attack(skill* sk, scene_role* dst_role, uint32 tick)
{
    if (sk == NULL || dst_role == NULL || get_scene() == NULL || get_scene()->get_scene_type() == MAP_TYPE_NORMAL) {
        return true;
    }

    //if (get_role_type() != RTE_PET) {
    //    amtsvr_log("target:%llu\n list:\n", dst_role->get_inst_id());
    //    std::map<uint64, hate>::iterator hate_iter = _hate_map.begin();
    //    while (hate_iter != _hate_map.end()) {
    //        printf("%llu, %d \n", hate_iter->first, hate_iter->second.get_val());
    //        ++hate_iter;
    //    }
    //    printf("\n");
    //}

    use_attack_tirgger_skill(sk->get_skill_id(), dst_role, tick);

    sk->set_cd(tick);

    client::attack_req a;
    dst_role->fill_role_data(*(a.add_rd()));
    a.set_skill_id(sk->id);
    a.mutable_ac()->set_x((float)_scene_pos.x);
    a.mutable_ac()->set_y((float)_scene_pos.y);
    std::vector<scene_role*> dst_role_list;
    std::vector<scene_role*> friends;
    std::vector<map_postion> miss;
    get_scene()->select_target(this, sk, dst_role_list, friends, miss);
    if (dst_role_list.empty()) {
        dst_role_list.push_back(dst_role);
    }

    if(dst_role_list.empty()) {
        return true;
    }

    effect_result atk_rsp;
    fill_role_data(*atk_rsp.mutable_src_rd());
    atk_rsp.set_id(_atk_id++);
    atk_rsp.set_skill_id(sk->id);
    attack_damage(sk, dst_role_list, atk_rsp);
    attack_effect(sk, dst_role_list, friends, atk_rsp);
    add_attack_count();

	std::vector<skill*> vs_ar;
	vs_ar.clear();
	mutable_skill()->get_skill_by_belong(SB_ARCHER, vs_ar);
	if (vs_ar.size() != 0){
		use_bow_skill();
	}

    get_scene()->screen_broadcast(*this, atk_rsp, false);

    // 攻击者可能不在另一个可见受击者屏幕范围需要以受击者为中心广播消息
    client::attack_rsp def;
    def.CopyFrom(atk_rsp);
    def.set_def(true);
    for (int32 i = 0; i < atk_rsp.fd_size(); ++i) {
        scene_role* psr = get_scene()->get_scene_role(atk_rsp.fd(i).dst_rd());
        if (psr == NULL) {
            psr = get_scene()->get_scene_role(atk_rsp.fd(i).dst_rd().inst_id());
        }

        if (psr == NULL) {
            continue;
        }

        def.clear_fd();
        def.add_fd()->CopyFrom(atk_rsp.fd(i));
        get_scene()->screen_broadcast(*psr, def, false);
    }

    get_scene()->multi_broadcast_modify_role_msg(*this, dst_role_list);
    set_last_attack_tick(tick);

    for (size_t i = 0; i < dst_role_list.size(); ++i) {
        if (dst_role_list[i]->get_status().is_dead() && !dst_role_list[i]->has_dead()) {
            dst_role_list[i]->on_killed(this);
            this->on_kill(dst_role_list[i]);
            del_hate_role(dst_role_list[i]->get_role_id());
			del_counter_role(dst_role_list[i]);
			dst_role_list[i]->del_counter_role(this);
        }
    }

    return true;
}

void scene_npc::use_attack_tirgger_skill(uint32 skill_id, scene_role* dst_role, uint32 tick)
{
    std::vector<skill*> vs;
    mutable_skill()->get_skill_by_type(ST_ATK_TIRGGER, vs);
    for (size_t i = 0; i < vs.size(); ++i) {
        const config_skill* pcs = vs[i]->get_config_skill();
        if (pcs == NULL || skill_id == pcs->get_skill_id()) {
            continue;
        }

        if (!vs[i]->cd_pass(tick)) {
            continue;
        }

        if (pcs->get_attack_time() != 0) {
            if (get_attack_count() == 0 || get_attack_count() % pcs->get_attack_time() != 0) {
                    continue;
            }
        }

        uint32 rand = mtrandom::rand_int32(10000u);
        if (rand > pcs->get_trigger_probability()) {
            continue;
        }

        attack(vs[i], dst_role, tick);
    }
}

void scene_npc::broadcast_hate_list(scene_role* dst_role)
{
    if (get_origin_npc().is_boss() && get_scene()->get_scene_type() == MAP_TYPE_WORLD_BOSS) {
        if (dst_role->get_inst_id() != _target_inst_id || _is_fiv_sec) {
            client::hate_list req;
            client::role_name* target = req.add_target();
            dst_role->fill_role_data(target->mutable_rd());
            target->set_name(dst_role->get_role_name());
            get_scene()->screen_broadcast(*this, req);
        }
    }
}

bool scene_npc::go_rand()
{
    if (get_scene() == NULL) {
        return false;
    }

    const config_npc* cn = get_origin_npc().get_config_npc();
    if (cn == NULL) {
        return false;
    }

    cell_pos now = get_scene()->to_cell_pos(get_scene_pos());
    if (get_rand_cell(now, cn->get_random_move_range(), get_move_cell())) {
        move_to_cell(get_move_cell());
        set_npc_status(NPC_STS_RAND_MOVE);
    }

    return true;
}

bool scene_npc::go_back()
{
    if (!can_move()) {
        set_npc_status(NPC_STS_STAND);
        return true;
    }

    if (get_scene() == NULL) {
        return false;
    }

    if (!get_move_node().empty()) {
        if (get_born_pos() == get_move_node()[0]) {
            return true;
        }
    }

    cell_pos cp  = get_scene()->to_cell_pos(get_born_pos());
    set_move_cell(cp);
    move_to_cell(cp);
    //move_to_pos(get_born_pos());
    set_npc_status(NPC_STS_BACK_MOVE);

    return true;
}

bool scene_npc::go_path()
{
    if (_path_cell.empty()) {
        stand();
        return false;
    }

    if (get_move_count() > 1) {
        return false;
    }

    std::list<cell_pos>::iterator first = _path_cell.begin();
    set_born_pos(get_scene()->to_map_postion(*first));
    cell_pos cp  = get_scene()->to_cell_pos(get_scene_pos());
    //printf("cur(%d, %d), dst(%d, %d)\n", cp._x, cp._y, first->_x, first->_y);
    if (*first == cp) {
        _path_cell.erase(_path_cell.begin());
        return go_path();
    }

    set_move_cell(*first);
    move_to_cell(get_move_cell(), true);
    set_npc_status(NPC_STS_PATH_MOVE);

    return true;
}

bool scene_npc::out_block()
{
    if (get_scene() == NULL) {
        return false;
    }

    scene_role* sr = get_hatest_role();
    if (sr != NULL) {
        move_to_pos(sr->get_scene_pos(), MT_FORCE_AND_NOTIFY);
    } else {
        const config_map* pcm = CONFMGR->get_config_map(get_scene()->get_scene_id());
        if (pcm == NULL) {
            return false;
        }

        move_to_pos(map_postion(pcm->get_birth_point_x(), pcm->get_birth_point_y(), 0u), MT_FORCE_AND_NOTIFY);
    }

    set_npc_status(NPC_STS_IN_BLOCK);

    return true;
}

bool scene_npc::on_status_stand(uint32 tick)
{
    if (get_status().is_dead()) {
        set_npc_status(NPC_STS_DEAD);
        return true;
    }

    if (!_path_cell.empty()) {
        return go_path();
    }

    if (get_scene()->get_cell().is_block(get_scene()->to_cell_pos(get_scene_pos()))) {
        on_status_in_block(tick);
        return true;
    }

    if (chase_role_by_cell(tick)) {
        return true;
    }

    if (get_status().giddy()) {
        return true;
    }

    if (!get_scene()->to_cell_pos(get_scene_pos()).around(get_scene()->to_cell_pos(get_born_pos()), scene_npc::BACK_LIMIT)) {
        return go_back();
    }

    if (get_origin_npc().is_rand_move() && _rand_move_tick + get_origin_npc().get_rand_move_space() < tick) {
        _rand_move_tick = tick;
        return go_rand();
    }

    return true;
}

bool scene_npc::on_status_move(uint32 tick)
{
    if (!can_move()) {
        if (is_keep_move()) {
            stand();
        }

        return chase_role_by_cell(tick);
    }

    trans_to_master_pos();

    bool move = move_by_cell(tick);
    if (!move) {
        stand();
        return chase_role_by_cell(tick);
    }

    if (!is_keep_move()) {
        on_move_end();
    }

    return chase_role_by_cell(tick);
}

bool scene_npc::on_status_fight(uint32 tick)
{
    if (!can_attack()) {
        return false;
    }

    scene_role* target = get_hatest_role();
    if (target == NULL) {
        if (is_keep_move()) {
            set_npc_status(NPC_STS_MOVE);
        } else {
            set_npc_status(NPC_STS_STAND);
        }

        return true;
    }

    return chase_role_by_cell(tick);
}

bool scene_npc::can_watch()
{
    if (get_status().is_dead()) {
        return false;
    }

    if (get_reative_atk()) {
        return true;
    }

    if (!get_origin_npc().is_active_attack() || !get_origin_npc().is_initiative_attack() || !get_origin_npc().is_picker()) {
        return false;
    }

    return true;
}

bool scene_npc::on_watch(scene_role* psr)
{
    if (!can_watch() || get_scene() == NULL) {
        return false;
    }

    if (get_scene()->is_friend(this, psr)) {
        return false;
    }

    if (!can_attack(psr)) {
        return false;
    }
    uint32 cp_range = get_origin_npc().get_attack_range();
    uint32 mp_range = get_scene()->calc_cell_length(cp_range);
    if (psr->get_status().is_dead() || !is_in_range(psr->get_scene_pos(), mp_range)) {
        return false;
    }

    add_hate_role(psr->get_inst_id(), 1, 0, get_scene_pos().get_distance(psr->get_scene_pos()));

    return true;
}

//鞭尸奖励
void scene_npc::send_whip_reward()
{
    //世界boss死亡后发放
    if (get_scene() == NULL || !get_status().is_dead() || !get_origin_npc().is_world_boss() || get_scene()->get_scene_type() != MAP_TYPE_WORLD_BOSS) {
        return;
    }

    scene_role *sr = get_hatest_role(CONFMGR->get_basic_value(10252));
    if(sr == NULL) {
        return;
    }
    on_whip_drop_down(sr);
    clean_hate_map();
}

bool scene_npc::on_status_dead(uint32 tick)
{
    remove_body(tick);
    if (!get_origin_npc().can_relive() || (tick - get_dead_time()) / 100 < get_origin_npc().get_revi_time()) {
        return false;
    }

    relive();

    return true;
}

bool scene_npc::on_status_force_move(uint32 tick)
{
    if (!move_by_path(tick, _force_move_notify) || !is_keep_move()) {
        stand();
    }

    //     if (_force_move_notify) {
    //         get_scene()->screen_broadcast_move_pos(*this);
    //     }

    return true;
}

bool scene_npc::on_status_rand_move(uint32 tick)
{
    bool move = move_by_cell(tick);
    if (!move || !is_keep_move()) {
        go_back();
        return true;
    }

    //     if (get_last_move_tick() == tick) {
    //         get_scene()->screen_broadcast_move_pos(*this);
    //     }

    cell_pos now = get_scene()->to_cell_pos(get_scene_pos());
    if (now == get_move_cell()) {
        go_back();
    } else {
        move_to_cell(get_move_cell());
    }

    return chase_role_by_cell(tick);
}

bool scene_npc::on_status_back_move(uint32 tick)
{
    cell_pos now = get_scene()->to_cell_pos(get_scene_pos());
    if (now.around(get_scene()->to_cell_pos(get_born_pos()), scene_npc::BACK_LIMIT)) {
        stand();
        on_move_end();
        return chase_role_by_cell(tick);
    } else {
        // amtsvr_log("%d, %d\n", get_born_pos().x, get_born_pos().y);
        go_back();
    }

    if (!move_by_cell(tick)) {
        stand();
        return chase_role_by_cell(tick);
    }

    now = get_scene()->to_cell_pos(get_scene_pos());
    if (now.around(get_scene()->to_cell_pos(get_born_pos()), scene_npc::BACK_LIMIT)) {
        stand();
        on_move_end();
    }

    //     if (get_last_move_tick() == tick) {
    //         get_scene()->screen_broadcast_move_pos(*this);
    //     }

    return chase_role_by_cell(tick);
}

bool scene_npc::on_status_path_move(uint32 tick)
{
    if (get_status().is_dead()) {
        set_npc_status(NPC_STS_DEAD);
        return true;
    }

    move_ignore_block(tick);

    //     if (get_last_move_tick() == tick) {
    //         get_scene()->screen_broadcast_move_pos(*this);
    //     }

    go_path();

    return chase_role_by_cell(tick);
}

bool scene_npc::on_status_idle(uint32 tick)
{
    if (_stop_tick > 0) {
        if (get_last_update_tick() > tick) {
             if (_stop_tick > (get_last_update_tick() - tick)) {
                 _stop_tick -= (get_last_update_tick() - tick);
             } else {
                 _stop_tick = 0;
                 set_npc_status(NPC_STS_STAND);
             }
        }
    }

    return true;
}

bool scene_npc::on_status_in_block(uint32 tick)
{
    if (!can_move()) {
        return true;
    }

    if (get_status().is_dead()) {
        set_npc_status(NPC_STS_DEAD);
        return true;
    }

    out_block();

    move_ignore_block(tick);

    //     if (get_last_move_tick() == tick) {
    //         get_scene()->screen_broadcast_move_pos(*this);
    //     }

    if (!get_scene()->get_cell().is_block(get_scene()->to_cell_pos(get_scene_pos()))) {
        set_npc_status(NPC_STS_STAND);
        return chase_role_by_cell(tick);
    }

    return true;
}

bool scene_npc::on_status_hide(uint32 tick)
{
    if (_hide_tick > 0) {
        if (get_last_update_tick() > tick) {
            if (_hide_tick > (get_last_update_tick() - tick)) {
                _hide_tick -= (get_last_update_tick() - tick);
            } else {
                _hide_tick = 0;
                set_npc_status(NPC_STS_STAND);
                show();
            }
        }
    }

    return true;
}

void scene_npc::on_reset(bool reborn /* = true */)
{
    reset_npc();                   // 重置NPC属性
    reset_scene_role();            // 重置场景角色
    clean_hate_map();              // 重置仇恨列表
    set_npc_status(NPC_STS_STAND); // 重置NPC状态
}

void scene_npc::relive()
{
    on_reset(true);

	scene_role *sr = get_master();
	if(NULL != sr)
	{
		set_camp(sr->get_camp());
		set_team_id(sr->get_team_id());
	}

    if (get_scene() == NULL || get_scene()->enter_scene(this) != 0) {
        return ;
    }

    get_scene()->screen_push_npc(this);
}

bool scene_npc::move_to_cell(const cell_pos& p, bool force /*= false*/)
{
    if (get_scene() == NULL) {
        return false;
    }

    if (!get_move_node().empty() && get_scene()->to_map_postion(p) == get_move_node()[0]) {
        return true;
    }

    cell_mgr& c  = get_scene()->get_cell();
    cell_pos now = get_scene()->to_cell_pos(get_scene_pos());
    cell_pos cp  = c.step(now, p);

    if (c.is_block(cp)) {
        mutable_move_node()->clear();
        if (get_scene()->find_path(get_scene_pos(), get_scene()->to_map_postion(p), *mutable_move_node()) == 0) {
            set_move_count(get_move_node().size());
            set_npc_status(NPC_STS_MOVE);
            if (get_last_move_tick() == 0) {
                set_last_move_tick(time_utils::tenms());
            }
        }

        return true;
    } else if (!force && !c.is_empty(cp)) {
        cp = c.path(now, cp, _d);
    }

    return move_to_pos(get_scene()->to_map_postion(cp));
}

bool scene_npc::move_to_pos(const map_postion& p, MOVE_TYPE mt /* = MT_NORMAL */)
{
    if (mt == MT_FORCE || mt == MT_FORCE_AND_NOTIFY) {
        set_npc_status(NPC_STS_FORCE_MOVE);
        _force_move_notify = mt == MT_FORCE_AND_NOTIFY;
    } else {
        set_npc_status(NPC_STS_MOVE);
    }

    return scene_role::move_to_pos(p, mt);
}

void scene_npc::follow(const map_postion& p)
{
    if ((get_npc_status() != NPC_STS_STAND &&
        get_npc_status() != NPC_STS_MOVE &&
        get_npc_status() != NPC_STS_RAND_MOVE &&
        get_npc_status() != NPC_STS_BACK_MOVE)
         ||
        get_scene() == NULL) {
            return ;
    }

    cell_pos now = get_scene()->to_cell_pos(get_scene_pos());
    if (now.around(get_scene()->to_cell_pos(get_born_pos()), scene_npc::BACK_LIMIT)) {
        return ;
    }

    move_to_pos(p);
    set_npc_status(NPC_STS_BACK_MOVE);
}

void scene_npc::init_to_update()
{
    add_to_update();
}

void scene_npc::add_to_update()
{
    if (get_scene() == NULL) {
        return;
    }

    get_scene()->add_tick_npc(this);
}

void scene_npc::remove_to_update()
{
    if (get_scene() == NULL) {
        return;
    }

    get_scene()->remove_tick_npc(this);
}

int scene_npc::on_attacked(scene_role* psr, skill& sk, effect_result& atk_rsp, bool& trigger)
{
    if (psr == NULL) {
        return -1;
    }

    _attacked = true;
    screen_broadcast_change_buf();
    int atk_sum_data = atk_rsp.get_result_sum();
    if (atk_sum_data <= 0) {
        atk_sum_data = 1;
    }
	add_hate_role(psr->get_inst_id(), sk.get_hate_val(), atk_sum_data, get_scene_pos().get_distance(psr->get_scene_pos()));
	add_counter_role(psr, sk.get_hate_val());
    return scene_role::on_attacked(psr, sk, atk_rsp, trigger);
}

uint64 scene_npc::get_max_damage_role()
{
    std::map<uint32, uint64> rank;
    get_real_damage_rank_list(rank);
    std::map<uint32, uint64>::reverse_iterator i = rank.rbegin();
    if (i == rank.rend()) {
        return 0;
    }

    return i->second;
}

void scene_npc::on_dead(uint32 tick)
{
    run_ai_dead(tick);
    set_npc_status(NPC_STS_DEAD);
    scene_role::on_dead(tick);

    remove_body(tick);

    if (!get_origin_npc().is_world_boss() || get_scene()->get_scene_type() != MAP_TYPE_WORLD_BOSS) {
        clean_hate_map();
    }
}

void scene_npc::set_force_speed(int speed)
{
    scene_role::set_force_speed(speed);
    if (speed != 0 )
	{
        set_npc_status(NPC_STS_FORCE_MOVE);
    }
}

int scene_npc::on_move_end()
{
    if (_move_node.empty()) {
        _move_node.resize(1);
    }

    _move_node[0] = get_scene_pos();
    get_scene()->screen_broadcast_move_pos(*this);

    scene_role::on_move_end();

    set_npc_status(NPC_STS_STAND);
    set_last_move_tick(0);

    return 0;
}

void scene_npc::calculate_attr()
{
    if (get_status().is_dead()) {
        return ;
    }

    scene_role::calculate_attr();
}

void scene_npc::fill_role_move_msg(client::role_move_msg& rmm)
{
    rmm.set_move_type(client::role_move_msg_MOVE_TYPE_MOVE_CONFIRM);
    client::move_info* mi = rmm.add_mi();
    fill_role_data(mi->mutable_rds());
    if (get_move_count() > 1 && get_move_count() <= _move_node.size()) {
        client::client_map_pos* cmp = mi->add_mps();
        cmp->set_x(_move_node[get_move_count() - 1].x);
        cmp->set_y(_move_node[get_move_count() - 1].y);
        cmp->set_z(_move_node[get_move_count() - 1].z);
        cmp->set_t(_move_node[get_move_count() - 1].t);
    } else {
        client::client_map_pos* cmp = mi->add_mps();
        cmp->set_x(_scene_pos.x);
        cmp->set_y(_scene_pos.y);
        cmp->set_z(_scene_pos.z);
        cmp->set_t(2);
    }
}

int scene_npc::on_killed(scene_role* psr)
{
    if(get_npc_status() == NPC_STS_DEAD) {
        return 0;
    }

    if(psr == NULL) {
        psr = get_hatest_role(CONFMGR->get_basic_value(10252));
    }

	if(psr == NULL) {
		return 0;
	}
	if(psr != NULL)
	{
		_killer = psr->get_role_id();
		on_normal_drop_down(psr);
	}
    if(!get_origin_npc().is_world_boss() || get_scene()->get_scene_type() != MAP_TYPE_WORLD_BOSS) { // 世界boss要鞭尸
		return scene_role::on_killed(psr);
    }

	set_role_status(true);
    clean_hate_map();
	uint32 tick = time_utils::tenms();
	on_dead(tick);

	return 0;
}

void scene_npc::stand(bool broadcast /* = true */)
{
    if (get_force_speed() != 0) {
        set_force_speed(0);
    }

    set_last_move_tick(0);
    set_npc_status(NPC_STS_STAND);

    return scene_role::stand(broadcast);
}

int scene_npc::add_lose_props(client::drop_props_msg &dpm)
{
    std::vector<cell_pos> cps;
	get_scene()->get_cell().get_not_block(get_scene()->to_cell_pos(get_scene_pos()), cps, dpm.dpi_size(), _cp);
    if (!cps.empty()) {
        client::drop_props_info* dpi = NULL;
        map_postion pos;
        for (int32 i = 0; i < dpm.dpi_size(); ++i) {
            dpi = dpm.mutable_dpi(i);
            pos = get_scene()->to_map_postion(cps[i % cps.size()]);
            dpi->set_x(pos.x);
            dpi->set_y(pos.y);
            dpi->set_z(pos.z);
            get_scene()->get_scene_lose_mgr()->add_lose(dpi);
        }
    }
    return 0;
}

int scene_npc::broadcast_drop_props(client::drop_props_msg &dpm)
{
    if (dpm.dpi_size() > 0) {
        get_scene()->screen_broadcast(*this, dpm);
    }

    return 0;
}

int scene_npc::on_whip_drop_down(scene_role *sr)
{
    if(get_scene() == NULL || get_origin_npc().is_picker()) {
        return -1;
    }
    const config_npc *cn = get_origin_npc().get_config_npc();
    if(cn == NULL) {
        return -2;
    }
    const std::vector<int> &cds = cn->get_zombie_drop_list();
    if(cds.size() == 0) {
        return 0;
    }
    client::drop_props_msg dpm;
    dpm.set_m(client::drop_props_msg_METHOD_ADD);
	vector<client::props_info> props;
	for(size_t i = 0; i < cds.size(); ++i)
	{
		make_props(cds[i], props, sr->get_job(), 0, 0, false);
	}
	uint32 now = (uint32)time(NULL);
	for(size_t i = 0; i < props.size(); ++i)
	{
		client::drop_props_info *dpi = dpm.add_dpi();
		dpi->add_belong(sr->get_role_id());
		dpi->set_index(get_scene()->get_scene_lose_mgr()->generate_index());
		dpi->set_end_time(now + 120);
		dpi->set_protect_time(now + 20);
		client::props_info *pi = dpi->mutable_pi();
		pi->CopyFrom(props[i]);
		pi->set_grid_index(dpi->index());
		pi->set_pos(client::props_info_POS_SCENE);
	}
    add_lose_props(dpm);
    broadcast_drop_props(dpm);
	//amtsvr_syslog(get_scene()->get_ctx(), "%d:%s\n",(int)time(NULL), dpm.DebugString().c_str());
    return 0;
}

int scene_npc::on_normal_drop_down(scene_role *sr)
{
    if (get_scene() == NULL || get_origin_npc().is_picker()) {
        return -1;
    }

    const config_npc* cn = get_origin_npc().get_config_npc();
    if (cn == NULL) {
        return -2;
    }
	if(!get_origin_npc().is_world_boss())
	{
		return 0;
	}
    client::drop_props_msg dpm;
    const std::vector<config_drop*> cds = cn->get_config_drops();
    dpm.set_m(client::drop_props_msg_METHOD_ADD);
	int diff = abs((int)sr->get_attr().get_val(LEVEL) - (int)get_attr().get_val(LEVEL));
	vector<client::props_info> props;
	for(size_t i = 0; i < cds.size(); ++i)
	{
		make_props(cds[i]->get_drop_id(), props, sr->get_job(), 0, diff, false);
	}
	uint32 now = (uint32)time(NULL);
	for(size_t i = 0; i < props.size(); ++i)
	{
		client::drop_props_info *dpi = dpm.add_dpi();
		dpi->add_belong(sr->get_role_id());
		dpi->set_index(get_scene()->get_scene_lose_mgr()->generate_index());
		dpi->set_end_time(now + 120);
		dpi->set_protect_time(now + 20);
		client::props_info *pi = dpi->mutable_pi();
		pi->CopyFrom(props[i]);
		pi->set_grid_index(dpi->index());
		pi->set_pos(client::props_info_POS_SCENE);
	}
    add_lose_props(dpm);
    broadcast_drop_props(dpm);
	//amtsvr_syslog(get_scene()->get_ctx(), "%d:%s\n",(int)time(NULL), dpm.DebugString().c_str());
    return 0;
}

client::PICK_RET_E scene_npc::pick(scene_user* su)
{
    if (get_scene() == NULL || su == NULL || !get_origin_npc().is_picker()) {
        return client::PIRE_TYPE_NOT_EXIST;
    }

    uint32 role_id = su->get_role_id();
    if (su->get_scene() != get_scene() || !is_in_range(su->get_scene_pos(), PICK_MAX_DISTANCE)) {
        _pick_roles.erase(role_id);
        return client::PIRE_RANGE;
    }

    uint32 now = (uint32)time(NULL);
    std::map<uint32, uint32>::iterator it = _pick_roles.find(role_id);
    if (it != _pick_roles.end()) {
        it->second = now;
        return client::PIRE_SUCCESS;
    }

    _pick_roles.insert(pair<uint32,uint32>(role_id, now));

    return client::PIRE_SUCCESS;
}

client::PICK_RET_E scene_npc::pick_commit(scene_user* psu)
{
    if (get_scene() == NULL || psu == NULL || !get_origin_npc().is_picker()) {
        return client::PIRE_TYPE_NOT_EXIST;
    }

    uint32 role_id = psu->get_role_id();
    if (psu->get_scene() != get_scene() || !is_in_range(psu->get_scene_pos(), PICK_MAX_DISTANCE)) {
        _pick_roles.erase(role_id);
        return client::PIRE_RANGE;
    }

    std::map<uint32, uint32>::iterator it = _pick_roles.find(role_id);
    if (it == _pick_roles.end()) {
        return client::PIRE_EFFECT;
    }

    uint32 now = (uint32)time(NULL);
    const config_npc* cn = get_origin_npc().get_config_npc();
    uint32 need_time = cn->get_collect_need_time();

    if(it->second + need_time / 100 > now) {
        return client::PIRE_TIME;
    }

    _pick_roles.erase(it);
    psu->on_picked(this);
    pick_complete();

    //    psu->on_kill_evt(this);
    //    on_killed_evt(psu);

    return client::PIRE_SUCCESS;
}

void scene_npc::pick_remove(uint32 role_id)
{
    _pick_roles.erase(role_id);
}

void scene_npc::pick_complete()
{
    mutable_status()->set_dead();
    set_npc_status(NPC_STS_DEAD);
}

bool scene_npc::remove_body(uint32 tick)
{
    if (!get_status().is_dead()) {
        return true;
    }

    const config_npc* cn = get_origin_npc().get_config_npc();
    if (get_scene() != NULL && (cn == NULL || (max(300u, cn->get_corpse_remain_time())) < (tick - get_dead_time()))) {
        get_scene()->remove_style_box_pos(*this);
        if (!get_origin_npc().can_relive() && is_pet_all_dead()) {
            get_scene()->remove_role(this);
            get_scene()->leave_scene(this);
			_cp.clear();
        }

        return true;
    }

    return false;
}

void scene_npc::run_ai(uint32 tick)
{
    if (get_status().is_dead()) {
        return ;
    }

    scene_role* sr = get_hatest_role();
    const server::ai_info& ai = get_origin_npc().get_ai().get_ai();
    if (get_last_update_tick() == 0) {
        set_last_update_tick(tick);
    }

    for (int32 i = 0; i < ai.ai_size(); ++i) {
        if (ai.ai(i).has_dead() && ai.ai(i).dead().dead()) {
            continue;
        }

        std::map<uint32, uint32>::iterator it = _ai_alarm.find(i);
        if (it == _ai_alarm.end()) {
            _ai_alarm[i] = 1;
        }

        if (tick > get_last_update_tick()) {
            _ai_timers[i] += tick - get_last_update_tick();
            if (_ai_alarm[i] != 0) {
                _ai_alarm[i] += tick - get_last_update_tick();
            }
        }

        bool timer = get_origin_npc().get_ai().check_timer(ai.ai(i), _ai_timers[i], _ai_alarm[i]);
        bool attr  = get_origin_npc().get_ai().check_attr(ai.ai(i), get_attr());
        bool distance = true;
        if (sr != NULL) {
            distance = get_origin_npc().get_ai().check_distance(ai.ai(i), get_scene_pos(), sr->get_scene_pos());
        }

        bool attack = get_origin_npc().get_ai().check_attack(ai.ai(i), get_attack_count());
        bool defense = get_origin_npc().get_ai().check_defense(ai.ai(i), get_defense_count());

        if (timer && attr && distance && attack && defense) {
            //printf("%s\n", ai.ai(i).DebugString().c_str());
            server::ai_run ar;
            if (ai.ai(i).has_msg()) {
                ar.mutable_msg()->CopyFrom(ai.ai(i).msg());
            }

            for (int32 j = 0; j < ai.ai(i).method_size(); ++j) {
                if ( ai.ai(i).method(j).count() != 0 && _ai_count[i][j] > ai.ai(i).method(j).count()) {
                    continue;
                }

                if (_ai_do_method[i][j]) {
                    continue;
                }

                ar.mutable_method()->CopyFrom(ai.ai(i).method(j));
                _ai_run[i][j].CopyFrom(ar);
                ar.clear_method();

                //if (run_ai(ai.ai(i).method(j), tick)) {
                //    amtsvr_log("npc:%u %s\n", get_role_id(), ai.ai(i).DebugString().c_str());
                //}
                _ai_do_method[i][j] = true;
                ++_ai_count[i][j];
            }

            for (int32 j = 0; j < ai.ai(i).move_size(); ++j) {
                //if (run_ai(ai.ai(i).move(j), tick)) {
                //    amtsvr_log("npc:%u %s\n", get_role_id(), ai.ai(i).DebugString().c_str());
                //}

                if (_ai_do_move[i][j]) {
                    continue;
                }

                ar.mutable_move()->CopyFrom(ai.ai(i).move(j));
                _ai_run[i][j].CopyFrom(ar);
                ar.clear_move();

                _ai_do_move[i][j] = true;
            }

            for (int32 j = 0; j < ai.ai(i).fix_move_size(); ++j) {
                if (get_npc_status() != NPC_STS_FORCE_MOVE) {
                    //if (run_ai(ai.ai(i).fix_move(j), tick)) {
                    //    amtsvr_log("npc:%u %s\n", get_role_id(), ai.ai(i).DebugString().c_str());
                    //}
                    if (_ai_do_fix_move[i][j]) {
                        continue;
                    }

                    ar.mutable_fix_move()->CopyFrom(ai.ai(i).fix_move(j));
                    _ai_run[i][j].CopyFrom(ar);
                    ar.clear_fix_move();

                    _ai_do_fix_move[i][j] = true;
                }
            }

            for (int32 j = 0; j < ai.ai(i).target_size(); ++j) {
                //if (run_ai(ai.ai(i).target(j), tick)) {
                //    amtsvr_log("npc:%u %s\n", get_role_id(), ai.ai(i).DebugString().c_str());
                //}
                if (_ai_do_target[i][j]) {
                    continue;
                }

                ar.mutable_target()->CopyFrom(ai.ai(i).target(j));
                _ai_run[i][j].CopyFrom(ar);
                ar.clear_target();

                _ai_do_target[i][j] = true;
            }

            if (ai.ai(i).has_stop()) {
                if (_ai_do_stop[i][0]) {
                    continue;
                }

                ar.mutable_stop()->CopyFrom(ai.ai(i).stop());
                _ai_run[i][0].CopyFrom(ar);
                ar.clear_stop();
            }

            if (ai.ai(i).has_hide()) {
                if (_ai_do_hide[i][0]) {
                    continue;
                }

                ar.mutable_hide()->CopyFrom(ai.ai(i).hide());
                _ai_run[i][0].CopyFrom(ar);
                ar.clear_hide();
            }

            if (ai.ai(i).has_master()) {
                if (_ai_do_master[i][0]) {
                    continue;
                }

                ar.mutable_master()->CopyFrom(ai.ai(i).master());
                _ai_run[i][0].CopyFrom(ar);
                ar.clear_master();
            }
        }
    }

    do_ai(tick);
}

bool scene_npc::run_ai(const server::ai_method& method, uint32 tick)
{
    if (mtrandom::rand_int32(100) > method.rand()) {
        return false;
    }

    skill* ps = mutable_skill()->get_ai_skill(method.skill_id());
    if (ps == NULL) {
        //printf("npc %d, not found ai skill %d\n", get_role_id(), method.skill_id());
        return false;
    }

    if (method.target() == server::ATT_ENEMY && has_hate_role()) {
        use_skill(ps);
    } else if (method.target() != server::ATT_ENEMY) {
        use_skill(ps);
    }

    return true;
}

bool scene_npc::run_ai(const server::ai_move& move, uint32 tick)
{
    if (mtrandom::rand_int32(100) > move.rand()) {
        return false;
    }

    if (!_path_cell.empty()) {
        return false;
    }

    for (int32 i = 0; i < move.x_size() && i < move.y_size(); ++i) {
        _path_cell.push_back(cell_pos(move.x(i), move.y(i)));
    }

    return true;
}

bool scene_npc::run_ai(const server::ai_fix_move& fix_move, uint32 tick)
{
    if (mtrandom::rand_int32(100) > fix_move.rand()) {
        return false;
    }

    uint32 fix_grid = 0;
    for (int32 i = 0; i < fix_move.grid_size(); ++i) {
        fix_grid = fix_move.grid(i);
        break;
    }

    if (fix_grid == 0) {
        return false;
    }

    scene_role* psr = get_hatest_role();
    scene_inst* psi = get_scene();
    if (psr == NULL || psi == NULL) {
        return false;
    }

    map_postion s = psr->get_scene_pos();
    map_postion e = get_scene_pos();
    map_postion t;
    //t.x = e.x - s.x;
    //t.y = e.y - s.y;
    //double length = sqrt((double)(t.x * t.x + t.y * t.y));
    //t.x = (int)((t.x / length) * get_scene()->calc_cell_length(fix_grid));
    //t.y = (int)((t.y / length) * get_scene()->calc_cell_length(fix_grid));
    //t.x += s.x;
    //t.y += s.y;
    float x = 0.0f, y = 0.0f;
    std::vector<map_postion> ts;
    uint32 max_d = 0;
    map_postion d;
    for (uint32 i = NORTH; i < MAX_ROLE_FACING; ++i) {
        psi->get_normalize_d((ROLE_FACING)i, x, y);
        t.x = (int)(x * psi->calc_cell_length(fix_grid)) + s.x;
        t.y = (int)(y * psi->calc_cell_length(fix_grid)) + s.y;
        if (psi->get_scene().is_out_scene_range(t)) {
            continue;
        }

        if (psi->get_cell().is_block(psi->to_cell_pos(t))) {
            continue;
        }

        if (check_path_block(t)) {
            ts.push_back(t);
        } else {
            uint32 cur_d = t.get_distance(s);
            if (max_d < cur_d) {
                max_d = cur_d;
                d = t;
            }
        }
    }

    if (!ts.empty()) {
        std::random_shuffle(ts.begin(), ts.end());
        d = ts[0];
    }

    move_to_pos(psi->to_map_postion(psi->to_cell_pos(d)), MT_FORCE_AND_NOTIFY);
    fix_grid = 0;

    return true;
}

bool scene_npc::run_ai(const server::ai_attack& target, uint32 tick)
{
    if (get_scene() == NULL) {
        return false;
    }

    scene_role* psr = NULL;
    if (target.attack() != 0) {
        psr = get_scene()->get_scene_npc_by_role_id(target.attack());
    } else {
        psr = get_scene()->get_first_user();
    }

    if (psr != NULL) {
        add_hate_role(psr->get_inst_id(), INT_MAX, 1, 0);
    }

    return true;
}

bool scene_npc::run_ai(const server::ai_stop& target , uint32 tick)
{
    if (mtrandom::rand_int32(100) > target.rand()) {
        return false;
    }

    if (target.time() > 0) {
        _stop_tick = target.time();
        stop();
    }

    return true;
}

bool scene_npc::run_ai(const server::ai_hide& target , uint32 tick)
{
    if (mtrandom::rand_int32(100) > target.rand()) {
        return false;
    }

    if (target.time() > 0) {
        _hide_tick = target.time();
        hide(target.pet());
    }

    return true;
}

bool scene_npc::run_ai(const server::ai_master& target , uint32 tick)
{
    if (_master_tick > 0 || get_scene() == NULL) {
        return false;
    }

    if (mtrandom::rand_int32(100) > target.rand()) {
        return false;
    }


    std::vector<scene_user*> sus;
    get_scene()->get_scene_users(sus);
    scene_user* psu = NULL;
    if (target.type() == 1) {
        for (size_t i = 0; i < sus.size(); ++i) {
            if (sus[i]->get_camp() == this->get_camp()) {
                psu = sus[i];
                break;
            }
        }
    } else if (target.type() == 2) {
        for (size_t i = 0; i < sus.size(); ++i) {
            if (sus[i]->get_family_id() == this->get_family_id()) {
                psu = sus[i];
                break;
            }
        }
    } else if (target.type() == 3) {
        for (size_t i = 0; i < sus.size(); ++i) {
            if (sus[i]->get_family_position_id() == 0x00000040 && sus[i]->get_family_id() == this->get_family_id()) {
                psu = sus[i];
                break;
            }
        }
    }

    if (psu == NULL) {
        return false;
    }

    if (get_master() != NULL) {
        get_master()->remove_pet(this);
    }

    psu->add_pet(this);
    _master_tick = target.time();
    set_master(psu);

    return true;
}

void scene_npc::do_ai(uint32 tick)
{
    if (_ai_run.empty()) {
        return ;
    }

    uint32 space_tick = 0;
    std::map<uint32, std::map<uint32, server::ai_run> >::iterator begin = _ai_run.begin();
    for (; begin != _ai_run.end(); ++begin) {
        std::map<uint32, server::ai_run>::iterator iter = begin->second.begin();

        while (iter != begin->second.end()) {
            if (_last_ai_tick < tick) {
                if (_last_ai_tick == 0) {
                    _last_ai_tick = tick;
                }

                space_tick = tick - _last_ai_tick;
            }

            if (iter->second.has_msg()) {
                if (!iter->second.msg().send() && get_scene() != NULL) {
                    iter->second.mutable_msg()->set_send(true);
                    client::create_npc_msg msg;
                    msg.set_content(iter->second.msg().text());
                    msg.set_delay  (0);
                    msg.set_show   (iter->second.msg().show() / 100);
                    get_scene()->scene_broadcast(msg);
                }

                if (iter->second.msg().delay() > space_tick) {
                    iter->second.mutable_msg()->set_delay(iter->second.msg().delay() - space_tick);
                    ++iter;
                    continue;
                }
            }

            if (iter->second.has_method()) {
                _ai_do_method[begin->first][iter->first] = false;
                if (run_ai(iter->second.method(), tick)) {
                    //amtsvr_log("npc:%u %s\n", get_role_id(), iter->second.DebugString().c_str());
                }
            }

            if (iter->second.has_move()) {
                _ai_do_move[begin->first][iter->first] = false;
                if (run_ai(iter->second.move(), tick)) {
                    //amtsvr_log("npc:%u %s\n", get_role_id(), iter->second.DebugString().c_str());
                }
            }

            if (iter->second.has_fix_move() && get_npc_status() != NPC_STS_FORCE_MOVE) {
                _ai_do_fix_move[begin->first][iter->first] = false;
                if (run_ai(iter->second.fix_move(), tick)) {
                    //amtsvr_log("npc:%u %s\n", get_role_id(), iter->second.DebugString().c_str());
                }
            }

            if (iter->second.has_target()) {
                _ai_do_target[begin->first][iter->first] = false;
                if (run_ai(iter->second.target(), tick)) {
                    //amtsvr_log("npc:%u %s\n", get_role_id(), iter->second.DebugString().c_str());
                }
            }

            if (iter->second.has_stop()) {
                _ai_do_stop[begin->first][iter->first] = false;
                if (run_ai(iter->second.stop(), tick)) {
                    //amtsvr_log("npc:%u %s\n", get_role_id(), iter->second.DebugString().c_str());
                }
            }

            if (iter->second.has_hide()) {
                _ai_do_hide[begin->first][iter->first] = false;
                if (run_ai(iter->second.hide(), tick)) {
                    //amtsvr_log("npc:%u %s\n", get_role_id(), iter->second.DebugString().c_str());
                }
            }

            if (iter->second.has_master()) {
                _ai_do_master[begin->first][iter->first] = false;
                if (run_ai(iter->second.master(), tick)) {
                    //amtsvr_log("npc:%u %s\n", get_role_id(), iter->second.DebugString().c_str());
                }
            }

            begin->second.erase(iter++);
            _last_ai_tick = tick;
        }
    }
}

void scene_npc::run_ai_dead(uint32 tick)
{
    const server::ai_info& ai = get_origin_npc().get_ai().get_ai();
    for (int32 i = 0; i < ai.ai_size(); ++i) {
        if (ai.ai(i).has_dead() && ai.ai(i).dead().dead()) {
            for (int32 j = 0; j < ai.ai(i).method_size(); ++j) {
                if (ai.ai(i).has_msg() && get_scene() != NULL) {
                    client::create_npc_msg msg;
                    msg.set_content(ai.ai(i).msg().text());
                    msg.set_delay  (0);
                    msg.set_show   (ai.ai(i).msg().show() / 100);
                    get_scene()->scene_broadcast(msg);
                }

                run_ai(ai.ai(i).method(j), tick);
            }
        }
    }
}

bool scene_npc::get_rand_cell(const cell_pos& source, uint32 range, cell_pos& target)
{
    if (get_scene() == NULL) {
        return false;
    }

    range = mtrandom::rand_int32(range);
    if (range == 0) {
        return false;
    }

    cell_mgr& c = get_scene()->get_cell();
    if (mtrandom::rand_int32(100) < 50) {
        target._x = source._x > range ? source._x - range : 0;
    } else {
        target._x = source._x + range;
        target._x = target._x < c.get_width() ? target._x : c.get_width();
    }

    if (mtrandom::rand_int32(100) < 50) {
        target._y = source._y > range ? source._y - range : 0;
    } else {
        target._y = source._y + range;
        target._y = target._y < c.get_height() ? target._y : c.get_height();
    }

    return true;
}

void scene_npc::change_mode(client::PET_MODE type)
{
    switch (type) {
    case client::PM_FOLLOW: set_npc_status(NPC_STS_STAND); break;
    case client::PM_STAY  : set_npc_status(NPC_STS_IDLE ); break;
    default: break;
    }
}

void scene_npc::multi_attr()
{
    const config_npc* p = get_origin_npc().get_config_npc();
    if (p == NULL || p->get_monster_difficulty_1() < 1) {
        return ;
    }

    _elite = true;

    for (uint32 i = MAX_HP; i < MOVE_SPEED; ++i) {
        origin_self.mutable_attr()->multi_val((ROLE_ATTRIBUTE)i, p->get_monster_difficulty_1() / 10000.0f);
        mutable_attr()->multi_val((ROLE_ATTRIBUTE)i, p->get_monster_difficulty_1() / 10000.0f);
    }
}

void scene_npc::get_drop_down_postion(const map_postion& src, uint32 num, map_postion& dst)
{
    float x = 0.0f, y = 0.0f;
    get_scene()->get_normalize_d((ROLE_FACING)(num % MAX_ROLE_FACING), x, y);
    int length = num / MAX_ROLE_FACING + 1;
    dst.x = src.x + (int)(x * length * get_scene()->get_map().get_config_map()->get_unit_grid_width());
    dst.y = src.y + (int)(y * length * get_scene()->get_map().get_config_map()->get_unit_grid_height());
}

void scene_npc::trans_to_master_pos()
{
    if (get_scene() == NULL || get_master() == NULL || get_status().giddy() || get_status().faint() || get_status().is_dead()) {
        return ;
    }

    if (get_master()->get_status().is_dead()) {
        return ;
    }

    //if (get_scene() != get_master()->get_scene()) {
    //    amtsvr_log("pet scene error %u, %u\n", get_scene()->get_scene_id(), get_master()->get_scene()->get_scene_id());
    //    attach_scene(get_master()->get_scene());
    //}

    uint32 d = get_scene()->to_cell_pos(get_scene_pos()).get_distance(get_scene()->to_cell_pos(get_born_pos()));
    if (d > scene_npc::get_origin_npc().get_chase_range() * 2) {
        stand();
        get_scene()->trans_pos(this, get_born_pos());
    } else if (d > scene_npc::get_origin_npc().get_chase_range()) {
        go_back();
    }
}

void scene_npc::reset_original_attr()
{
    uint32 map_id    = get_map_id();
    uint32 cur_hp = get_attr(CUR_HP);
    uint32 camp_id   = 0;
    uint32 team_id   = 0;
    uint32 family_id = 0;
    if (get_master() == NULL) {
        camp_id   = get_camp();
        team_id   = get_team_id();
        family_id = get_family_id();
    } else {
        camp_id   = get_master()->get_camp();
        team_id   = get_master()->get_team_id();
        family_id = get_master()->get_family_id();
    }

    reset_npc();

    set_attr(MAP_ID, map_id);
    set_attr(CUR_HP, cur_hp);
    set_camp     (camp_id  );
    set_team_id  (team_id  );
    set_family_id(family_id);
}

void scene_npc::reset_npc()
{
    mutable_attr()->CopyFrom(get_origin_npc().get_attr());
}

void scene_npc::set_npc_status(NPC_STS_E nse)
{
	if (get_npc_status() == NPC_STS_DEAD && nse == NPC_STS_FORCE_MOVE) {
		return;
	}

    //if (get_npc_status() == NPC_STS_BACK_MOVE && nse != NPC_STS_BACK_MOVE) {
    //    set_attr(MOVE_SPEED, get_origin_npc().get_attr(MOVE_SPEED));
    //    send_client_change_attr();
    //} else if (get_npc_status() != NPC_STS_BACK_MOVE && nse == NPC_STS_BACK_MOVE) {
    //    set_attr(MOVE_SPEED, get_origin_npc().get_attr(MOVE_SPEED) * 2);
    //    send_client_change_attr();
    //}

    _nse = nse;
}

bool scene_npc::is_pet_all_dead()
{
    std::map<uint64, scene_role*>& pets = get_pets();
    if (pets.empty()) {
        return true;
    }

    std::map<uint64, scene_role*>::iterator i = pets.begin();
    for ( ; i != pets.end(); ++i) {
        if (i->second->get_role_type() == RTE_PET) {
            scene_npc* pet = static_cast<scene_npc*>(i->second);
            if (pet->get_origin_npc().get_config_npc()->get_is_remove_pet() > 0) {
                continue;
            }
        }

        if (!i->second->get_status().is_dead()) {
            return false;
        }
    }

    return true;
}

void scene_npc::stop()
{
    set_npc_status(NPC_STS_IDLE);
}

void scene_npc::show()
{
    set_npc_status(NPC_STS_STAND);
    if (get_scene() == NULL) {
        return ;
    }

    get_scene()->screen_push_npc(this);
}

void scene_npc::hide(uint32 pet)
{
    set_npc_status(NPC_STS_HIDE);
    if (get_scene() == NULL) {
        return ;
    }

    if (pet > 0) {
        for (std::map<uint64, scene_role*>::iterator i = get_pets().begin(); i != get_pets().end(); ++i) {
            get_scene()->remove_style_box_pos(*(i->second));
        }
    }

    get_scene()->remove_style_box_pos(*this);
}

void scene_npc::ai_expire(uint32 tick)
{
	if(get_status().is_dead())
	{
		return ;
	}
	uint32 expire = get_ai_expire_tick();
	if(expire == 0)
	{
		return ;
	}
	if(expire >= tick)
	{
		scene_inst *si = get_scene();
		if(NULL == si)
		{
			return ;
		}
		si->reset_ai(this);
	}
}

void scene_npc::ai_master(uint32 tick)
{
    if (get_last_update_tick() <= tick || _master_tick == 0) {
        return ;
    }

    if (_master_tick > (get_last_update_tick() - tick)) {
        _master_tick -= (get_last_update_tick() - tick);
    } else {
        _master_tick = 0;
    }

    if (_master_tick == 0 && get_master() != NULL) {
        get_master()->remove_pet(this);
    }
}

void scene_npc::set_disappear_tick(uint32 tick)
{
    get_origin_npc().set_disappear(tick);
}

void scene_npc::set_ai_expire_tick(uint32 tick)
{
	_ai_expire = time_utils::tenms() + tick / 10;
}

int scene_npc::add_effect(const config_effect* effect, client::role_data* caster /*= NULL*/)
{
    if (check_immunity_effect(effect)) {
        return role_buffer::ADD_RET_IMMUNE;
    }

    return scene_role::add_effect(effect, caster);
}

bool scene_npc::check_immunity_effect(const config_effect* effect)
{
    const config_npc* pcn = get_origin_npc().get_config_npc();
    if (pcn == NULL) {
        return false;
    }

    const std::vector<int>& ies = pcn->get_immunity_effect();
    const std::vector<int>& tis = effect->get_effect_type_id();
    for (size_t i = 0; i < ies.size(); ++i) {
        for (size_t j = 0; j < tis.size(); ++j) {
            if (ies[i] == tis[j]) {
                return true;
            }
        }
    }

    return false;
}

bool scene_npc::check_immunity_skill(const config_skill* skill)
{
    const config_npc* pcn = get_origin_npc().get_config_npc();
    if (pcn == NULL) {
        return false;
    }

    const std::vector<int>& iss = pcn->get_immunity_skill();
    for (size_t i = 0; i < iss.size(); ++i) {
        if ((uint32)iss[i] == skill->get_skill_id()) {
            return true;
        }
    }

    return false;
}
