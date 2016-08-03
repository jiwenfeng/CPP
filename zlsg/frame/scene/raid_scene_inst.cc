#include "config_raid.h"
#include "config_npc.h"
#include "player_role.h"
#include "player_role_raid_mgr.h"
#include "game_map_mgr.h"
#include "scene_user.h"
#include "scene_npc.h"
#include "create_npc_rule.h"
#include "raid_over.h"
#include "region_over.h"
#include "raid_scene_inst.h"

raid_scene_inst::raid_scene_inst(uint32 id, const game_map* p, const raid_scene* s, struct amtsvr_context* c, const int m)
    : scene_inst(id, p, s, c, m), _rsc(s)
{
    _dead_npc_count  = 0;
    _complete        = false;
    _complete_time   = 0;
    _close_raid_time = 0;

    _family_id    = 0;
    _team_id      = 0;
    _farmer_id    = 0;
    _type         = client::enter_raid_req_type_normal;
    _first        = false;

//    set_start_tick(time_utils::tenms());
    set_raid_over_tick(0);
    _ro = NULL;
}

raid_scene_inst::~raid_scene_inst()
{
    if (_ro != NULL) {
        delete _ro;
        _ro = NULL;
    }
}

int raid_scene_inst::init()
{
    _ro = new raid_over(this);

    return scene_inst::init();
}

int raid_scene_inst::on_pick_props(scene_role* psr, client::props_info* ppi)
{
    get_raid_over()->mark_items(ppi);
    get_region_over()->mark_items(ppi);
    get_check_create_npc_rule()->on_get_item(ppi);

    return scene_inst::on_pick_props(psr, ppi);
}

int raid_scene_inst::on_pick_npc(scene_user* psu, scene_npc* psn)
{
    get_check_create_npc_rule()->on_touch_object(psn);
    get_raid_over()->mark_touch(psn);
    get_region_over()->mark_touch(psn);

    return scene_inst::on_pick_npc(psu, psn);
}

int raid_scene_inst::on_role_move(scene_role* r, const map_postion& from, const map_postion& to)
{
    get_raid_over()->mark_role_postion(r, from, to);
    get_region_over()->mark_role_postion(r, from, to);

    return scene_inst::on_role_move(r, from, to);
}

int raid_scene_inst::on_attack_role(scene_role* atk, scene_role* dst_role, skill& sk, effect_result& atk_rsp)
{
    return scene_inst::on_attack_role(atk, dst_role, sk, atk_rsp);
}

int raid_scene_inst::on_kill_role(scene_role* atk, scene_role* def)
{
    get_raid_over()->mark_dead_role(def);
    get_region_over()->mark_dead_role(def);

    if (atk == NULL) {
        // buff µ¼ÖÂËÀÍö»áÓÐatk == NULL
        return scene_inst::on_kill_role(atk, def);
    }

    return scene_inst::on_kill_role(atk, def);
}

int raid_scene_inst::on_role_attr_change(scene_role* psr)
{
    get_raid_over()->mark_role_attribute(psr);

    return scene_inst::on_role_attr_change(psr);
}

int raid_scene_inst::on_story_end(scene_user* psu, uint32 id)
{
    get_check_create_npc_rule()->on_after_story(id);

    return scene_inst::on_story_end(psu, id);
}

void get_drops(uint32 drop_id, std::vector<client::props_info>& drops);

int raid_scene_inst::on_raid_complete(bool failure /*= false*/)
{
    _complete_time = (uint32)time(NULL);
    settlement(SETTLE_TYPE_COMPLETE);

    uint32 used_tick = time_utils::tenms() - get_start_tick();
    client::raid_result_msg msg;
    msg.set_raid_id(get_raid_id());
    msg.set_star(get_raid_scene().get_evaluation(used_tick));
    msg.set_tick(used_tick);

    std::vector<scene_user*> sus;
    get_scene_users(sus);
    const config_raid* cr = get_map().get_config_raid();
    for (size_t i = 0; i < sus.size(); ++i) {
        client::raid_result_info* info = msg.add_info();
        sus[i]->fill_role_data(info->mutable_rd());
        if (cr != NULL) {
            info->set_coin(cr->get_reward_money());
            info->set_exp(cr->get_reward_experience());
            info->set_tao_lue(cr->get_reward_tao_lue());
			std::vector<client::props_info> props;
			for(size_t i = 0; i < cr->get_reward_item().size(); ++i)
			{
				make_props(cr->get_reward_item().at(i), props, sus[i]->get_job());
			}
			for(size_t i = 0; i < props.size(); ++i)
			{
                client::props_info* ppi = info->add_props();
				ppi->CopyFrom(props[i]);
			}
        }
    }

    if (failure) {
        get_raid_over()->fill_failure(msg.mutable_failure());
    } else {
        get_raid_over()->fill_success(msg.mutable_success());
    }

    for (size_t i = 0; i < sus.size(); ++i) {
#ifndef __LIB_MINI_SERVER__
        sus[i]->send_client(msg);
#endif // __LIB_MINI_SERVER__
        if (!failure) {
            client::raid_info_msg rim;
            sus[i]->get_player_role().get_raid_mgr().push(sus[i]->get_role_id(), get_raid_id(), msg.star(), used_tick, _complete_time, get_type());
            sus[i]->get_player_role().get_raid_mgr().fill_raid_info_msg(rim, get_raid_id());
            sus[i]->on_raid_complete(get_raid_id(), get_type(), msg.star());
#ifndef __LIB_MINI_SERVER__
            if (rim.ri_size() > 0) {
                sus[i]->send_client(rim);
            }
#endif // __LIB_MINI_SERVER__
        }

        if (sus[i]->mutable_buffer()->del_raid_buf()) {
            uint32 count = 0;
            sus[i]->mutable_buffer()->update(time_utils::tenms(), count);
            client::buffer_list bl;
            sus[i]->mutable_buffer()->fill_change_buffer_list(bl);
            sus[i]->fill_role_data(bl.mutable_rd());
            screen_broadcast(*sus[i], bl, false);
            sus[i]->calculate_attr();
            sus[i]->calculate_status();
            sus[i]->send_client_change_attr();
        }

#ifdef __LIB_MINI_SERVER__
        msg.mutable_verify()->set_max_hp(sus[i]->get_attr(MAX_HP));
        msg.mutable_verify()->set_max_atk(sus[i]->get_attr(ATTACK_MAX));
        msg.mutable_verify()->set_max_def(sus[i]->get_attr(DEFENSE_MAX));
		msg.mutable_verify()->set_bow_index(sus[i]->get_attr(BOW_INDEX));

        if (failure) {
            sus[i]->send_client(msg);
        } else {
            client::mini_server_end_msg end;
            end.mutable_msg()->CopyFrom(msg);
            sus[i]->send_client(end);
        }

        if (get_terrain_mgr().has_terrain()) {
            get_terrain_mgr().stop_add();
            client::terrain_list remove;
            if (get_terrain_mgr().remove_terrain_list(remove)) {
                sus[i]->send_client(remove);
            }
        }
#endif // __LIB_MINI_SERVER__
    }



    return 0;
}

int raid_scene_inst::raid_lose_normal(vector<client::props_info>& vpi)
{
    return 0;
}

int raid_scene_inst::raid_lose_flop(vector<client::props_info>& vpi)
{
    return 0;
}

int raid_scene_inst::raid_lose_box(uint32 index, vector<client::props_info>& vpi)
{
    return 0;
}

void raid_scene_inst::on_role_enter(scene_role *sr)
{
	scene_inst::on_role_enter(sr);
}

void raid_scene_inst::on_role_leave(scene_role *sr)
{
    if (_user_map.obj_num() == 0) {
        _close_raid_time = time_utils::tenms() + RAID_SCENE_CLOSE_INTEVAL;
    }
	scene_inst::on_role_leave(sr);
}

int raid_scene_inst::enter_scene(scene_npc* psn)
{
    if (_type == client::enter_raid_req_type_elite) {
        psn->multi_attr();
    }

    int ret = scene_inst::enter_scene(psn);
    if (ret != 0) {
        return ret;
    }
    temp_pet_enter(psn);

    return 0;
}

int raid_scene_inst::enter_scene(scene_user* u)
{
    int ret = scene_inst::enter_scene(u);
    if (ret != 0) {
        return ret;
    }
    return 0;
}

int raid_scene_inst::leave_scene(scene_user* psu)
{
    if (_user_map.obj_num() == 1) {
        settlement(SETTLE_TYPE_LEAVE);
    }

    temp_pet_clear(psu);
    if (psu->get_status().is_dead()) {
        psu->relive();
    }

	psu->clear_target();
	psu->mutable_attr()->set_val(ROLE_TEAM_ID, 0);

    return scene_inst::leave_scene(psu);
}

int raid_scene_inst::on_raid_timeout()
{
    bool trans_back_user = (_rsc->get_type() == RAID_FAMILY_LAYER);
    std::vector<scene_user*> sus;
    get_scene_users(sus);
    uint32 size = sus.size();
    for (uint32 i = 0; i < size; ++i) {
        if (trans_back_user) {
            if (sus[i]->trans_back_user_at_once() != 0) {
                destroy_user(sus[i]);
            }
        }
    }

    settlement(SETTLE_TYPE_TIMEOUT);

    return 0;
}

void raid_scene_inst::all_npc_dead()
{
    client::multi_role_msg mrm;
    //uint32 monster_count = npc_map.obj_num();
    shobj* p = _npc_map.get_all_objs_head();
    scene_user* psu = get_first_user();
    while (p != NULL) {
        scene_npc* n = static_cast<scene_npc*>(p->sr);
        if ((psu != NULL && is_friend(psu, n)) || n->get_status().is_dead()) {
            p = p->next[SLPE_LIST];
            continue;
        }

        //n->change_mode(client::PM_STAY);
        n->mutable_attr()->set_val(CUR_HP, 0u);
        n->mutable_status()->set_dead();
        n->set_npc_status(NPC_STS_DEAD);
        n->fill_role_broadcast_msg(*mrm.add_rm(), false);

        p = p->next[SLPE_LIST];
    }

    scene_broadcast(mrm);
}

int raid_scene_inst::update(uint32 tick, bool is_one_sec, bool is_fiv_sec, bool is_ten_sec, bool is_thirty_sec, bool is_one_minute)
{
	if(!is_scene_ready())
	{
		return 0;
	}
    scene_inst::update(tick, is_one_sec, is_fiv_sec, is_ten_sec, is_thirty_sec, is_one_minute);

    if (_close_raid_time > 0) {
        if (tick > _close_raid_time) {
            on_raid_timeout();
            _close_inst = true;
            trans_back_all_user();
        }

        std::vector<uint32> us;
        get_scene_users(us);
        if (us.size() == 0) {
            _close_inst = true;
        }

        return 0;
    }

    if (is_fiv_sec && is_time_out()) {
        on_time_reach();
        _close_raid_time = tick + RAID_SCENE_CLOSE_INTEVAL;
    }

    if (is_one_sec && !_complete && tick > get_raid_over_tick()) {
        if (get_raid_over_tick() == 0) {
            set_raid_over_tick(tick);
        }

        int ret = get_raid_over()->on_update(tick - get_raid_over_tick());
        if (ret != 0) {
            _complete = true;
            bool failure = ret < 0;
            on_raid_complete(failure);
            all_npc_dead();
            _close_raid_time = tick + RAID_SCENE_CLOSE_INTEVAL;
        }

        set_raid_over_tick(tick);
    }

    return 0;
}

bool raid_scene_inst::is_time_out() const
{
    if (_rsc->get_standard_time() == 0) {
        return false;
    }

    uint32 now = (uint32)time(NULL);
    return ((now - _start_time) > (_rsc->get_standard_time()));
}

void raid_scene_inst::settlement(uint32 settle_type)
{

}

bool raid_scene_inst::check_kill_role_region(uint32 region_id)
{
    bool c = true;
    std::vector<scene_npc*> sns;
    get_alive_scene_npcs(sns);
    for (size_t i = 0; i < sns.size(); ++i) {
        if (sns[i]->get_region() != region_id) {
            continue;
        }

        if (sns[i]->get_real_role_type() == RTE_USER) {
            continue;
        }

        if (sns[i]->get_origin_npc().get_monster_type() == MT_USER_FRIEND ||
            sns[i]->get_origin_npc().get_monster_type() == MT_TEMP_PET ||
            sns[i]->get_origin_npc().get_monster_type() == MT_PICK) {
                continue;
        }

        c = false;
        break;
    }

    return c;
}

bool raid_scene_inst::check_kill_role(scene_role* atk, scene_role* sr)
{
    /*    if (atk == NULL) {
    printf("buff dead inst_id = %lu, role_id = %d\n", sr->get_inst_id(), sr->get_role_id());
    } else {
    printf("atk inst_id = %lu, role_id = %d\n", atk->get_inst_id(), atk->get_role_id());
    }
    */

    /*
    ++_dead_times[sr->get_inst_id()];
    if (_dead_times[sr->get_inst_id()] > 1) {
    printf("def inst_id = %lu, role_id = %d, dead times error\n", sr->get_inst_id(), sr->get_role_id());
    }
    */

    if (!_ccnr->is_all_create()) {
        return false;
    }

    bool c = is_scene_npc_all_dead();
    //std::vector<scene_npc*> sns;
    //get_alive_scene_npcs(sns);
    //for (size_t i = 0; i < sns.size(); ++i) {
    //    if (sns[i]->get_real_role_type() == RTE_USER) {
    //        continue;
    //    }

    //    if (sns[i]->get_origin_npc().get_monster_type() == MT_USER_FRIEND ||
    //        sns[i]->get_origin_npc().get_monster_type() == MT_TEMP_PET ||
    //        sns[i]->get_origin_npc().get_monster_type() == MT_PICK ||
    //        sns[i]->get_origin_npc().get_monster_type() == MT_NPC || 
    //        sns[i]->get_origin_npc().get_monster_type() == MT_TASK_NPC) {
    //        continue;
    //    }

    //    //printf("alive inst_id = %lu, role_id = %d (%d, %d)\n", sns[i]->get_inst_id(), sns[i]->get_role_id(), sns[i]->get_scene_pos().x, sns[i]->get_scene_pos().y);
    //    c = false;
    //    break;
    //}
    //printf("-------\n");

    return c;
}

void raid_scene_inst::temp_pet_enter(scene_npc* psn)
{
    if (psn == NULL || psn->get_origin_npc().get_monster_type() != MT_TEMP_PET) {
        return ;
    }

    scene_user* psu = get_first_user();
    if (psu != NULL) {
        psu->add_pet(psn);
    }
}

void raid_scene_inst::temp_pet_clear(scene_role* psr)
{
    if (psr == NULL) {
        return ;
    }

    std::map<uint64, scene_role*>& pets = psr->get_pets();
    std::map<uint64, scene_role*>::iterator i = pets.begin();
    while (i != pets.end()) {
        if (i->second->get_role_type() == RTE_NPC) {
            pets.erase(i++);
            continue;
        }

        ++i;
    }
}

void raid_scene_inst::set_scene_ready(bool ready, scene_user *psu)
{
	set_start_tick(time_utils::tenms());
	scene_inst::set_scene_ready(ready, psu);
}
