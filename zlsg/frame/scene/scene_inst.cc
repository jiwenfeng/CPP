#include "config_raid.h"
#include "config_npc.h"
#include "config_map.h"
#include "config_effect.h"
#include "client.chat_msg.pb.h"
#include "player_role.h"
#include "player_role_statis_mgr.h"
#include "player_role_mail.h"
#include "scene.h"
#include "game_map_mgr.h"
#include "scene_user.h"
#include "scene_pet.h"
#include "farm_scene_npc.h"
#include "scene_lose_mgr.h"
#include "create_npc_rule.h"
#include "region_over.h"
#include "filter_target.h"
#include "scene_inst.h"
#include "config_mail.h"

scene_inst::scene_inst(uint32 id, const game_map* p, const scene* s, struct amtsvr_context* c, const int m)
        : _inst_id(id)
        , _gm(p)
        , _sc(s)
        , _ctx(c)
        , _max_user(m)
{
    assert(p != NULL);
    assert(s != NULL);
    _cur_user          = 0;
    _start_time        = (uint32)time(NULL);
    _alloc_npc_inst_id = 0;
    _alloc_pet_inst_id = 0;
    _close_inst        = false;
    _suspend_tick      = 0;
    _last_tick         = 0;
    _space_tick        = 0;

    _slm = new scene_lose_mgr(this);
    get_cell().init(_gm->get_config_map());
    _ccnr = new check_create_npc_rule(this);
    _region_over = new region_over(this);
	_robot_num = 0;
	_ready = false;
}

scene_inst::~scene_inst()
{
    if (_region_over != NULL) {
        delete _region_over;
        _region_over = NULL;
    }

    if (_slm != NULL) {
        delete _slm;
        _slm = NULL;
    }

    if (_ccnr != NULL) {
        delete _ccnr;
        _ccnr = NULL;
    }

    clear_all();
}

int scene_inst::init()
{
    if (_gm->get_type() == MAP_TYPE_NORMAL || _gm->get_type() == MAP_TYPE_RAID || _gm->get_type() == MAP_TYPE_PET || _gm->get_type() == MAP_TYPE_TEAM || _gm->get_type() == MAP_TYPE_LEVELS) {
        space_style_box::init(_sc->get_scene_ret());
        //printf("%s:%d, %d, %d\n", __FILE__, __LINE__, sc->get_scene_ret().width(), sc->get_scene_ret().height());
    } else {
        space_style_box::init(_sc->get_scene_ret(), DEFAULT_OTHER_SCREEN_WIDTH, DEFAULT_OTHER_SCREEN_HEIGHT);
    }

    if (generate_npc_inst() != 0) {
        return -1;
    }

    return 0;
}

uint32 scene_inst::get_map_id() const
{
    return _gm->get_id();
}

uint32 scene_inst::get_scene_id() const
{
    return _sc->get_scene_id();
}

uint32 scene_inst::get_scene_type() const
{
    return _gm->get_type();
}

int scene_inst::generate_npc_inst()
{
    const std::vector<const origin_npc*>& nl = _sc->get_npc_list();
    size_t count = nl.size();
    //printf("npc count %u\n", (unsigned int)count);
    uint32 now = (uint32)time(NULL);
    for (size_t i = 0; i < count; ++i) {
        if (!nl[i]->can_create(now)) {
            printf("can not create npc %u\n", nl[i]->get_role_id());
            continue;
        }
        generate_npc_inst(*nl[i], now);
    }

    return 0;
}

scene_npc* scene_inst::generate_npc_inst(uint32 npc_id)
{
    const origin_npc* n = GAME_MAP_MGR->get_npc(npc_id);
    if (n == NULL) {
        amtsvr_log("scene:%u npc:%u not found\n", get_scene_id(), npc_id);
        return NULL;
    }

    uint32 now = (uint32)time(NULL);
    return generate_npc_inst(*n, now);
}

scene_npc *scene_inst::generate_npc_inst(uint32 npc_id, uint32 x, uint32 y)
{
	const origin_npc *n = GAME_MAP_MGR->get_npc(npc_id);
	if(NULL == n)
	{
		amtsvr_log("scene:%u npc:%u not found\n", get_scene_id(), npc_id);
		return NULL;
	}
	return generate_npc_inst(*n, (uint32)time(NULL), map_postion(x, y, (uint32)0));
}

uint64 scene_inst::generate_pet_id()
{
    if (++_alloc_pet_inst_id == 0) {
        ++_alloc_pet_inst_id;
    }

    uint32 high = get_scene_id();
    high = ((high << 2) | 2);
    uint64 pet_id = UINT64_MAKE(high, _alloc_pet_inst_id);

    return pet_id;
}

scene_npc* scene_inst::generate_npc_inst(const origin_npc& p, uint32 now)
{
    if (++_alloc_npc_inst_id == 0) {
        ++_alloc_npc_inst_id;
    }

    uint64 npc_inst_id = UINT64_MAKE(RTE_NPC, _alloc_npc_inst_id);//UINT64_MAKE(alloc_npc_inst_id, sc->get_scene_id());
    scene_npc* sn = new scene_npc(p, npc_inst_id);
    if (enter_scene(sn) != 0) {
        delete sn;
        return NULL;
    }

    return sn;
}

int scene_inst::create_randnpc(client::randnpc* rn)
{
    map_postion mp(rn->born_pos().x(), rn->born_pos().y(), rn->born_pos().z());
    const origin_npc* p = GAME_MAP_MGR->get_npc(rn->role_id());
    if (p == NULL) {
        return -1;
    }

    scene_npc* asn = static_cast<scene_npc*>(generate_npc_inst(*p, (uint32)time(NULL), mp, GNTE_AWARD));
    if (asn == NULL) {
        return -2;
    }

    screen_push_npc(asn);

    return 0;
}

uint64 scene_inst::generate_npc_inst_id()
{
    if (++_alloc_npc_inst_id == 0) {
        ++_alloc_npc_inst_id;
    }
	return UINT64_MAKE(RTE_NPC, _alloc_npc_inst_id);//UINT64_MAKE(alloc_npc_inst_id, sc->get_scene_id());
}

scene_npc* scene_inst::generate_npc_inst(const origin_npc& p, uint32 now, const map_postion& mp, GENERATE_NPC_TYPE_E gntype /* = GNTE_NORMAL */, bool enter /* = true */)
{
    if (++_alloc_npc_inst_id == 0) {
        ++_alloc_npc_inst_id;
    }

    uint64 npc_inst_id = UINT64_MAKE(RTE_NPC, _alloc_npc_inst_id);//UINT64_MAKE(alloc_npc_inst_id, sc->get_scene_id());
    scene_npc* sn;
    switch (gntype) {
    case GNTE_NORMAL:
        sn = new scene_npc(p, npc_inst_id);
        break;
    case GNTE_AWARD:
        sn = new scene_npc(p, npc_inst_id);
        break;
    case GNTE_FARM:
        sn = new farm_scene_npc(p, npc_inst_id);
        break;
    case GNTE_SPACE:
        sn = new scene_npc(p, npc_inst_id);
        break;
	case GNTE_ROBOT:
		sn = new scene_robot(p, npc_inst_id);
		break;
    default:
        sn = new scene_npc(p, npc_inst_id);
        break;
    }

    sn->set_born_pos(mp);
    sn->set_scene_pos(mp);

	if(enter) {
		if (enter_scene(sn) != 0) {
			delete sn;
			return NULL;
		}
    }

    return sn;
}

int scene_inst::update_role_dispear(uint32 tick, uint32 user_num)
{
    shobj* cur = NULL;
    cur = _npc_map.get_tick_objs_head();
    while (cur != NULL) {
		scene_role *sr = cur->sr;
		sr->on_update_disappear(tick, user_num);
		if(sr->is_disappear()) {
			on_role_disappear(sr);
			remove_role(sr);
		}
        cur = cur->next[SLPE_TICK];
    }
	return 0;
}

int scene_inst::update(uint32 tick, bool is_one_sec, bool is_fiv_sec, bool is_ten_sec, bool is_thirty_sec, bool is_one_minute)
{
    if (get_last_tick() == 0 || get_last_tick() > tick) {
        set_last_tick(tick);
    }

    set_space_tick(tick - get_last_tick());

    if (get_suspend_tick() != 0) {
        uint32 st = tick - get_last_tick();
        if (get_suspend_tick() > st) {
            set_suspend_tick(get_suspend_tick() - st);
            set_last_tick(tick);
            return 0;
        } else {
            set_suspend_tick(0);
        }
    }

    if (is_thirty_sec) {
        update_thirty_seconds(tick);
    }

    uint32 user_num = _user_map.obj_num(); // 无任何用户，场景不做更新
	if(is_one_sec) {
		update_role_dispear(tick, user_num);
	}
	clear_dead_npc();
    if (user_num == 0) {
        set_last_tick(tick);
        return 0;
    }

//    clear_dead_npc();
    get_terrain_mgr().check_terrain_timeout(tick);
    get_check_create_npc_rule()->on_update(get_space_tick());
    get_region_over()->on_update(get_space_tick());

    std::vector<scene_role*> role_list;
    shobj* cur = NULL;
    cur = _npc_map.get_tick_objs_head();
    while (cur != NULL) {
        role_list.push_back(cur->sr);
        cur = cur->next[SLPE_TICK];
    }

    cur = _user_map.get_tick_objs_head();
    while (cur != NULL) {
        role_list.push_back(cur->sr);
        cur = cur->next[SLPE_TICK];
    }

    if (!get_check_create_npc_rule()->mutable_msg()->empty()) {
        std::list<server::create_msg>::iterator cur = get_check_create_npc_rule()->mutable_msg()->begin();
        std::list<server::create_msg>::iterator end = get_check_create_npc_rule()->mutable_msg()->end();
        for ( ; cur != end; ++cur) {
            client::create_npc_msg msg;
            msg.set_content(cur->content());
            msg.set_delay  (cur->delay());
            msg.set_show   (cur->show());
            scene_broadcast(msg);
        }

        get_check_create_npc_rule()->mutable_msg()->clear();
    }

    uint32 count = role_list.size();
    for (uint32 i = 0; i < count; ++i) {
        role_list[i]->on_update(tick, user_num, is_one_sec, is_fiv_sec, is_ten_sec, is_thirty_sec, is_one_minute);
    }
#ifndef __LIB_MINI_SERVER__
	if((tick % (uint32)CONFMGR->get_basic_value(10208)) <= 10)
	{
		if(get_scene_type() == MAP_TYPE_NORMAL)
		{
			load_ghost();
		}
	}
#endif
	if(is_thirty_sec) {
	}
    if (is_one_minute) {
        get_scene_lose_mgr()->on_update(tick);
    }

    set_last_tick(tick);

    return 0;
}

int scene_inst::update_thirty_seconds(uint32 tick)
{
    return 0;
}

int scene_inst::add_tick_role(scene_role* sr)
{
    switch (sr->get_role_type()) {
    case RTE_USER: return add_tick_user(static_cast<scene_user*>(sr));
    case RTE_NPC :
    case RTE_PET : return add_tick_npc(static_cast<scene_npc*>(sr));
    default: break;
    }

    return -1;
}

int scene_inst::remove_tick_role(scene_role* sr)
{
    switch (sr->get_role_type()) {
    case RTE_USER: return remove_tick_user(static_cast<scene_user*>(sr));
    case RTE_NPC :
    case RTE_PET : return remove_tick_npc(static_cast<scene_npc*>(sr));
    default: break;
    }

    return -1;
}

int scene_inst::add_tick_npc(scene_npc* n)
{
    if (n->get_status().is_dead()) {
        return 0;
    }

    uint64 id = n->get_inst_id();
    _npc_map.push(id, n);
    _npc_map.push_to_tick(id);

    return 0;
}

int scene_inst::remove_tick_npc(scene_npc* n)
{
    return _npc_map.remove_from_tick(n->get_inst_id());
}

int scene_inst::add_tick_user(scene_user* u)
{
    return _user_map.push_to_tick(u->get_inst_id());
}

int scene_inst::remove_tick_user(scene_user* u)
{
    return _user_map.remove_from_tick(u->get_inst_id());
}

int scene_inst::clear_all()
{
    clear_npc();
    clear_dead_npc();
    _user_map.init();

    return 0;
}

int scene_inst::clear_npc(bool notify_remove /* = false */)
{
    client::role_remove_msg rrm;
    std::vector<scene_role*> del;
    shobj* cur = _npc_map.get_all_objs_head();
    for (; cur != NULL; cur = cur->next[SLPE_LIST]) {
        style_box* box = get_style_box(cur->sr->get_scene_pos());
        if (box != NULL) {
            box->remove_all_scene_npc();
        }

        if (notify_remove) {
            cur->sr->fill_role_remove_msg(rrm);
        }

        // 不能直接删除因为NPC下可能有管理PET 导致npc_map会有PET野指针
        if (cur->sr->get_role_type() != RTE_PET && cur->sr->get_master() == NULL) {
            del.push_back(cur->sr);
        }
    }

    for (size_t i = 0; i < del.size(); ++i) {
        delete del[i];
		del[i] = NULL;
    }

    del.clear();

    // 重置npc_map清理野指针
    _npc_map.init();

    if (notify_remove) {
        scene_broadcast(rrm);
    }

    return 0;
}

int scene_inst::clear_npc(std::vector<uint64>& tmns)
{
    uint32 size = tmns.size();
    if (size == 0) {
        return 0;
    }

    std::vector<scene_role*> del;
    for (uint32 i = 0; i < size; ++i) {
        scene_npc* sn = get_scene_npc(tmns[i]);
        if (sn == NULL || sn->get_role_type() == RTE_PET) {
            continue;
        }

        leave_scene(sn);

        // 不能直接删除因为NPC下可能有管理PET 导致npc_map会有PET野指针
        if (sn->get_role_type() != RTE_PET && sn->get_master() == NULL) {
            del.push_back(sn);
        }
    }

    for (size_t i = 0; i < del.size(); ++i) {
        delete del[i];
    }

    del.clear();
    tmns.clear();

    return 0;
}

int scene_inst::clear_monster()
{
    shobj* cur = _npc_map.get_all_objs_head();
    std::vector<scene_npc*> npcs;
    for (; cur != NULL; cur = cur->next[SLPE_LIST]) {
        scene_npc* npc = static_cast<scene_npc*>(cur->sr);
        if (npc->get_origin_npc().is_monster() && !npc->get_origin_npc().is_picker()) {
            npcs.push_back(npc);
        }
    }

    std::vector<scene_role*> del;
    for (size_t i = 0; i < npcs.size(); ++i) {
        scene_npc* sn = npcs[i];
        if (sn == NULL || sn->get_role_type() == RTE_PET) {
            continue;
        }

        leave_scene(sn);
        // 不能直接删除因为NPC下可能有管理PET 导致npc_map会有PET野指针
        if (sn->get_role_type() != RTE_PET) {
            del.push_back(sn);
        }
    }

    for (size_t i = 0; i < del.size(); ++i) {
        delete del[i];
    }

    del.clear();
    npcs.clear();

    return 0;
}

int scene_inst::clear_dead_npc()
{
    if (_remove_role.empty()) {
        return 0;
    }

    std::map<uint64, scene_role*>::iterator i = _remove_role.begin();
    for ( ; i != _remove_role.end(); ++i) {
        leave_scene(i->second);

        std::map<uint64, scene_role*>& pets = i->second->get_pets();
        for (std::map<uint64, scene_role*>::iterator iter = pets.begin(); iter != pets.end(); ++iter) {
            if (iter->second == NULL) {
                continue;
            }

            iter->second->set_master(NULL);
        }

        delete i->second;
    }

    _remove_role.clear();

    return 0;
}

int scene_inst::enter_scene(scene_role* psr)
{
    if (psr == NULL) {
        return -1;
    }
    switch (psr->get_role_type()) {
    case RTE_USER: return enter_scene(static_cast<scene_user*>(psr));
    case RTE_NPC:
    case RTE_PET: return enter_scene(static_cast<scene_npc*>(psr));
    default: break;
    }

    psr->set_attack_count(0);
    psr->set_defense_count(0);

    return -1;
}

int scene_inst::leave_scene(scene_role* psr)
{
    if (psr == NULL) {
        return -1;
    }

    switch (psr->get_role_type()) {
    case RTE_USER: return leave_scene(static_cast<scene_user*>(psr));
    case RTE_NPC:
    case RTE_PET: return leave_scene(static_cast<scene_npc*>(psr));
    default: break;
    }

    return -1;
}

int scene_inst::enter_scene(scene_npc* psn)
{
    assert(_remove_role.find(psn->get_inst_id()) == _remove_role.end());
	if(psn->is_robot())
	{
		_robot_num++;
	}
    uint64 id = psn->get_inst_id();
    scene_npc* tmp_npc = static_cast<scene_npc*>(_npc_map.mut(id));
    if (tmp_npc != NULL) {
        if (psn != tmp_npc) {
            assert(false);
            leave_scene(tmp_npc);
            _npc_map.push(id, psn);
        }
    } else {
        _npc_map.push(id, psn);
    }

    psn->attach_scene(this);
    psn->init_to_update();
    psn->set_attack_count(0);
    psn->set_defense_count(0);
	if (psn->get_role_type() == RTE_USER){
		psn->set_bow_attack_count(0);
		psn->send_bow_attack_count();
	}

    style_box* box = get_style_box(psn->get_scene_pos());
    if (box != NULL) {
        box->add_scene_npc(id, psn->get_role_id());
        cell_pos cp = to_cell_pos(psn->get_scene_pos());
        get_cell().enter_cell(psn, cp);
        enter_region(psn, cp, cp);
    } else {
        amtsvr_log("error scene_id:%u npc_id:%u x:%u y:%u\n", get_scene_id(), psn->get_role_id(), psn->get_scene_pos().x, psn->get_scene_pos().y);
        return -1;
    }

    std::vector<style_box*> in_boxs;
    get_style_boxs(psn->get_scene_pos(), in_boxs);
    npc_watch_user(psn, in_boxs);

    return 0;
}

int scene_inst::leave_scene(scene_npc* n)
{
    if (n == NULL) {
        return -1;
    }
	if(n->is_robot())
	{
		--_robot_num;
	}

    std::map<uint64, scene_role*>& pets = n->get_pets();
    for (std::map<uint64, scene_role*>::iterator i = pets.begin(); i != pets.end(); ++i) {
        if (i->second == NULL || i->second->get_status().is_dead()) {
            continue;
        }

        leave_scene(i->second);
    }

    screen_broadcast_remove(*n);

	vector<scene_role *> vsr;
    get_screen_role(n->get_scene_pos(), vsr);
	for(size_t i = 0; i < vsr.size(); ++i)
	{
		vsr[i]->del_counter_role(n);
		n->del_counter_role(vsr[i]);
	}

    uint64 id = (uint64)n->get_inst_id();
    style_box* box = get_style_box(n->get_scene_pos());
    if (box != NULL) {
        box->remove_scene_npc(id);
    }

    _npc_map.remove(id);

    get_cell().leave_cell(n);

    n->detach_scene();


    return 0;
}

int scene_inst::enter_scene(scene_user* psu)
{
	if(get_scene_type() == MAP_TYPE_NORMAL)
	{
		psu->mutable_status()->set_val(client::RSE_STATUS_ATTKMODE, role_status_attack_mode_attack_mod_peace);
	}
	erase_ghost(psu->get_role_id());
    uint64 id = psu->get_inst_id();
    scene_user* tmp_user = static_cast<scene_user*>(_user_map.mut(id));
    if (tmp_user != NULL) {
        if (psu != tmp_user) {
            leave_scene(tmp_user);
            delete tmp_user;
            _user_map.push(id, psu);
        }

        amtsvr_log("Re entering the scene role_id:%d name:%s\n", psu->get_role_id(), psu->get_role_name());
        return 0;
    } else {
        _user_map.push(id, psu);
    }

	psu->set_bow_attack_count(0);
	psu->send_bow_attack_count();

    std::map<uint64, scene_role*>& pets = psu->get_pets();
    std::map<uint64, scene_role*> tps;
    for (std::map<uint64, scene_role*>::iterator i = pets.begin(); i != pets.end(); ++i) {
        if (i->second == NULL) {
            continue;
        }

        uint64 pet_id = generate_pet_id();
        i->second->set_inst_id(pet_id);
        i->second->set_scene_pos(psu->get_scene_pos());
        i->second->set_born_pos(psu->get_scene_pos());
        switch (i->second->get_role_type()) {
        case RTE_USER: {
            assert(false);
            scene_user* su = static_cast<scene_user*>(i->second);
            enter_scene(su);
            break;
        }
        case RTE_NPC:
            assert(false);
        case RTE_PET: {
            scene_npc* sn = static_cast<scene_npc*>(i->second);
            if (RTE_PET == i->second->get_role_type()) {
                scene_pet* sp = static_cast<scene_pet*>(i->second);
                if (sp->get_master() != NULL && sp->get_pet() != NULL && sp->get_pet()->status() != PET_STATUS_FIGHTING) {
                    delete sp;
                    continue;
                }

                if (sp->get_status().is_dead()) {
                    sp->relive();
                } else {
                    sp->mutable_attr()->set_val(CUR_HP, sp->get_attr(MAX_HP));
                }
            }

			sn->set_camp(psu->get_camp());
			sn->set_team_id(psu->get_team_id());
            sn->set_family_id(psu->get_family_id());
            enter_scene(sn);

            break;
        }
        default: break;
        }

        tps.insert(std::pair<uint64, scene_role*>(pet_id, i->second));
    }

    pets.clear();
    pets = tps;

    //if (tps.empty()) {
    //    psu->on_update_pet_fight_status();
    //}

    return 0;
}

void scene_inst::on_role_enter(scene_role* psr)
{
    if (MAP_TYPE_NORMAL == get_scene_type() && psr->get_role_type() == RTE_USER) {
        psr->set_camp(0);
		scene_user* psu = static_cast<scene_user*>(psr);
        if (psr->get_status().is_dead()) {
            psr->relive();
        } else {
            psr->mutable_attr()->set_val(CUR_HP, psr->get_attr(MAX_HP));
            psr->calculate_attr();
            psr->calculate_status();
			psu->on_update_pet_fight_status();
        }
		psu->change_pre_pet_attr();
    }

	if (psr->get_role_type() == RTE_USER){
		scene_user* psu = static_cast<scene_user*>(psr);
		psu->change_pre_pet_attr();
	}
	
}

void scene_inst::del_scene_buffer(scene_role *psr)
{
	if(psr->get_role_type() != RTE_USER) {
		return ;
	}

	if (psr->mutable_buffer()->del_raid_buf()) {
		client::buffer_list bl;
		psr->mutable_buffer()->fill_change_buffer_list(bl);
		psr->fill_role_data(bl.mutable_rd());
		screen_broadcast(*psr, bl, false);
	}

	psr->calculate_attr();
	psr->calculate_status();
	psr->mutable_attr()->set_val(CUR_HP, psr->get_attr(MAX_HP));
	psr->send_client_change_attr();

#ifndef __LIB_MINI_SERVER__
	scene_user* psu = static_cast<scene_user*>(psr);
	psu->save_user();
	psu->get_player_role().write_all(_ctx);
#endif
}

void scene_inst::on_role_leave(scene_role* psr)
{
	if (psr->get_role_type() == RTE_USER) {
		scene_user* psu = static_cast<scene_user*>(psr);
		std::map<uint64, scene_role*>& pet_map_ = psu->get_pets();
		std::map<uint64, scene_role*>::iterator it = pet_map_.begin();
		for (; it != pet_map_.end(); ++it){
			it->second->mutable_buffer()->del_raid_buf();
			client::buffer_list bl;
			it->second->mutable_buffer()->fill_change_buffer_list(bl);
			it->second->fill_role_data(bl.mutable_rd());
			screen_broadcast(*it->second, bl, false);
			it->second->calculate_attr();
			it->second->calculate_status();
			it->second->mutable_attr()->set_val(CUR_HP, psr->get_attr(MAX_HP));
			it->second->send_client_change_attr();
		}

        if (get_terrain_mgr().has_terrain()) {
            client::terrain_list remove;
            if (get_terrain_mgr().remove_terrain_list(remove)) {
                psr->send_client(remove);
            }
        }
	}
}

bool scene_inst::is_scene_empty()
{
    return _user_map.obj_num() == 0;
}

int scene_inst::leave_scene(scene_user* u)
{
    std::map<uint64, scene_role*>& pets = u->get_pets();
    for (std::map<uint64, scene_role*>::iterator i = pets.begin(); i != pets.end(); ++i) {
        if (i->second == NULL) {
            continue;
        }

        switch (i->second->get_role_type()) {
        case RTE_USER: {
            scene_user* su = static_cast<scene_user*>(i->second);
            leave_scene(su);
            break;
        }
        case RTE_NPC: {
            i->second->set_master(NULL);
            break;
        }
        case RTE_PET: {
            scene_npc* sn = static_cast<scene_npc*>(i->second);
            leave_scene(sn);
            break;
        }
        default: break;
        }
    }

	del_scene_buffer(u);

    screen_broadcast_remove(*u);

	vector<scene_role *> vsr;
    get_screen_role(u->get_scene_pos(), vsr);
	for(size_t i = 0; i < vsr.size(); ++i)
	{
		vsr[i]->del_counter_role(u);
		u->del_counter_role(vsr[i]);
	}
	
    uint64 id = (uint64)u->get_inst_id();
    style_box* box = get_style_box(u->get_scene_pos());
    if (box != NULL) {
        box->remove_scene_user(id);
    }

    get_cell().leave_cell(u);

    _user_map.remove(id);
    on_role_leave(u);

    return 0;
}

void scene_inst::screen_role_leave(scene_role* sr)
{
    screen_broadcast_remove(*sr, false);
    client::role_remove_msg rrm;
    if (screen_fill_role_remove_msg(*sr, rrm) > 0) {
        sr->send_client(_ctx, rrm);
    }
}

int scene_inst::trans_pos(scene_role* sr, const map_postion& to)
{
    if (sr == NULL) {
        return -1;
    }

    map_postion pos(to);
    fix_pos(pos);
    if (get_style_box(pos) == NULL) {
        return -2;
    }

    style_box* box = get_style_box(sr->get_scene_pos());
    if (box == NULL) {
        return -3;
    }

    screen_role_leave(sr);
    remove_scene_role(box, sr);

    sr->change_cur_pos(pos);

    if (sr->get_role_type() == RTE_USER) {
        scene_user* su = static_cast<scene_user*>(sr);
        client::multi_role_msg mrm;
        su->fill_user_msg(*mrm.add_rm());
        su->send_client(mrm);

        client::role_pos_msg rpm;
        su->fill_role_pos_msg(rpm);
        su->send_client(rpm);

        screen_push_user(su);
    } else {
        push_role_to_users(*sr);
    }

    return 0;
}

int scene_inst::trans_postion(scene_role* sr, const map_postion& to)
{
    if (sr == NULL) {
        return -1;
    }

    map_postion pos(to);
    fix_pos(pos);
    sr->change_cur_pos(pos);
    client::role_pos_msg rpm;
    sr->fill_role_pos_msg(rpm);
    screen_broadcast(*sr, rpm, false);

    return 0;
}

void scene_inst::screen_broadcast_modify_role_msg(scene_role& r, bool except_me)
{
    client::multi_role_msg mrm;
    client::role_msg* rm = mrm.add_rm();
    client::role_attr broadcast_attr;
    bool need_broadcast = false;
    bool change_master = r.fill_role_modify_msg(*rm, broadcast_attr, need_broadcast);
    if (!change_master) {
        return;
    }

    if (r.get_role_type() == RTE_USER) {
        r.send_client(mrm);
    }

    if (!need_broadcast) {
        return;
    }

    rm->mutable_attr()->Clear();
    rm->mutable_attr()->CopyFrom(broadcast_attr);
    screen_broadcast(r, mrm, except_me);
}

int scene_inst::multicast_role_remove(vector<scene_user*>& users, scene_role& r)
{
    if (users.size() == 0) {
        return 0;
    }

    client::role_remove_msg rrm;
    r.fill_role_remove_msg(rrm);

    return multicast(r, users, rrm);
}

int scene_inst::multi_fill_role_remove_msg(scene_role& r, vector<scene_user*>& users, vector<scene_npc*>& npcs, client::role_remove_msg& rrm)
{
    uint32 size = users.size();
    for (uint32 i = 0; i < size; ++i) {
        if (users[i]->is_same_role(r)) {
            continue;
        }

        users[i]->fill_role_remove_msg(rrm);
    }

    size = npcs.size();
    for (uint32 i = 0; i < size; ++i) {
        npcs[i]->fill_role_remove_msg(rrm);
    }

    return 0;
}

int scene_inst::broadcast_scene_area_msg(amtsvr_context* ctx,uint32 svr_scene_id,client::broadcast_area_rsp& area_msg)
{
	return broadcast_scene_msg(ctx,svr_scene_id,area_msg);
}

int scene_inst::broadcast_scene_msg(amtsvr_context* ctx,uint32 svr_scene_id, google::protobuf::Message &msg)
{
	google::protobuf::Message* new_msg = create_msg(msg.GetTypeName());
	if (new_msg == NULL) {
		return -1;
	}
	new_msg->CopyFrom(msg);

	msg_handler* mh = MSG_HANDLER_POOL->pop();
	if (mh == NULL) {
		delete new_msg;
		return -2;
	}

	mh->msg = new_msg;


	return amtsvr_send(ctx, 0, svr_scene_id, PTYPE_CLIENT|PTYPE_TAG_DONTCOPY, 0, mh, sizeof(msg_handler));;
}

int scene_inst::screen_fill_role_remove_msg(scene_role& r, client::role_remove_msg& rrm)
{
    std::vector<scene_role*> vsr;
    get_screen_role(r.get_scene_pos(), vsr, true, true);
    uint32 size = vsr.size();
    uint32 count = 0;
    for (uint32 i = 0; i < size; ++i) {
        if (vsr[i]->is_same_role(r)) {
            continue;
        }

        ++count;
        vsr[i]->fill_role_remove_msg(rrm);
    }

    return count;
}

int scene_inst::screen_broadcast_remove(scene_role& r, bool except_me)
{
    client::role_remove_msg rrm ;
    r.fill_role_remove_msg(rrm);
    return screen_broadcast(r, rrm, except_me);
}

int scene_inst::screen_broadcast_move_pos(scene_role& r)
{
    client::role_move_msg rmm ;
    r.fill_role_move_msg(rmm);
    return screen_broadcast(r, rmm);
}

int scene_inst::multi_fill_move_role_msg(scene_role& r, vector<scene_user*>& users, vector<scene_npc*>& npcs, client::role_move_msg& rmm, client::multi_role_msg& mrm, client::role_name_msg& rnm, client::buffer_list& bl)
{
    int    num       = 0;
    uint32 count     = users.size();
    bool   send_name = is_need_broadcast_npc_name();

    for (uint32 i = 0; i < count; ++i) {
        if (users[i]->is_same_role(r)) {
            continue;
        }

        users[i]->fill_role_move_msg(rmm);
        users[i]->fill_role_broadcast_msg(*mrm.add_rm());
        users[i]->fill_role_name(*(rnm.add_rn()));
        users[i]->fill_buffer(bl);
        ++num;
    }

    count = npcs.size();
    for (uint32 i = 0; i < count; ++i) {
        npcs[i]->fill_role_move_msg(rmm);
        npcs[i]->fill_role_broadcast_msg(*mrm.add_rm());

        if ((npcs[i]->get_role_type() == RTE_PET && npcs[i]->get_master() != NULL) || send_name) {
            npcs[i]->fill_role_name(*(rnm.add_rn()));
        }

        npcs[i]->fill_buffer(bl);
        ++num;
    }

    return num;
}

int scene_inst::screen_broadcast(scene_role& sr, google::protobuf::Message& msg, bool except_me /* = true */)
{
    uint32 ids[MAX_BROADCAST_AGENT_NUM];
    int id_count = 0;

    std::vector<style_box*> boxs;
    get_style_boxs(sr.get_scene_pos().get_point(), boxs);

    std::vector<scene_user*> role_list;
    get_screen_user(boxs, role_list);

    uint32 role_num = role_list.size();
    for (uint32 i = 0; i < role_num; ++i) {
        scene_user* su = role_list[i];
        if (su->is_same_role(sr)) {
            continue;
        }

        ids[id_count++] = su->get_client_id();
    }

    if (!except_me && sr.get_role_type() == RTE_USER) {
        scene_user* su = static_cast<scene_user*>(&sr);
        ids[id_count++] = su->get_client_id();
    }

    if (id_count == 0) {
        return 0;
    }

    size_t msg_len = 0;
    void* buf = encode(msg, msg_len);
    if (buf == NULL) {
        return 0;
    }

    amtsvr_broadcast(_ctx, ids, id_count, buf, msg_len);

    return 0;
}

int scene_inst::screen_broadcast(scene_role& sr, vector<google::protobuf::Message*>& msgs, bool except_me)
{
    uint32 ids[MAX_BROADCAST_AGENT_NUM];
    int id_count = 0;

    std::vector<style_box*> boxs;
    get_style_boxs(sr.get_scene_pos().get_point(), boxs);

    std::vector<scene_user*> role_list;
    get_screen_user(boxs, role_list);

    uint32 role_num = role_list.size();
    for (uint32 i = 0; i < role_num; ++i) {
        scene_user* su = role_list[i];
        if (except_me && su->is_same_role(sr)) {
            continue;
        }

        ids[id_count++] = su->get_client_id();
    }

    if (id_count == 0) {
        return 0;
    }

    uint32 msg_size = msgs.size();
    for (uint32 i = 0; i < msg_size; ++i) {
        size_t msg_len = 0;
        void* buf = encode(*msgs[i], msg_len);
        if (buf == NULL) {
            continue;
        }

        amtsvr_broadcast(_ctx, ids, id_count, buf, msg_len);
    }

    return 0;
}

int scene_inst::multicast(scene_role& r, vector<scene_user*>& users, google::protobuf::Message& msg, bool except_me)
{
    if (users.size() == 0) {
        return 0;
    }

    uint32 ids[MAX_BROADCAST_AGENT_NUM];
    int id_count = 0;

    size_t count = users.size();
    for (size_t i = 0; i < count; ++i) {
        if (except_me && users[i]->is_same_role(r)) {
            continue;
        }

        ids[id_count++] = users[i]->get_client_id();
    }

    if (id_count == 0) {
        return 0;
    }

    size_t msg_len = 0;
    void* buf = encode(msg, msg_len);
    if (buf == NULL) {
        return 0;
    }

    amtsvr_broadcast(_ctx, ids, id_count, buf, msg_len);

    return 0;
}

int scene_inst::multicast(scene_role& r, std::vector<scene_user*>& users, std::vector<google::protobuf::Message*>& msgs, bool except_me)
{
    if (users.size() == 0) {
        return 0;
    }

    uint32 ids[MAX_BROADCAST_AGENT_NUM];
    int id_count = 0;

    size_t count = users.size();
    for (size_t i = 0; i < count; ++i) {
        if (except_me && users[i]->is_same_role(r)) {
            continue;
        }

        ids[id_count++] = users[i]->get_client_id();
    }

    if (id_count == 0) {
        return 0;
    }

    uint32 msg_size = msgs.size();
    for (uint32 i = 0; i < msg_size; ++i) {
        size_t msg_len = 0;
        void* buf = encode(*msgs[i], msg_len);
        if (buf == NULL) {
            return 0;
        }
        amtsvr_broadcast(_ctx, ids, id_count, buf, msg_len);
    }

    return 0;
}

void scene_inst::npc_watched_user(scene_user* su)
{
    if (su == NULL) {
        return;
    }

    std::vector<scene_role*> vsr;
    get_screen_role(su->get_scene_pos(), vsr, false, true);

    uint32 size = vsr.size();
    for (uint32 i = 0; i < size; ++i){
        vsr[i]->on_watch(su);
        su->on_watch(vsr[i]);
    }
}

int scene_inst::send_center(google::protobuf::Message& msg)
{
    msg_handler* mh = MSG_HANDLER_POOL->pop();
    if (mh == NULL) {
        return -1;
    }

    google::protobuf::Message* new_msg = create_msg(msg.GetTypeName());
    if (new_msg == NULL) {
        MSG_HANDLER_POOL->push(mh);
        return -2;
    }

    new_msg->CopyFrom(msg);
    mh->msg = new_msg;

    return amtsvr_sendname(this->get_ctx(), CENTER_NAME, PTYPE_CLIENT|PTYPE_TAG_DONTCOPY, 0, mh, sizeof(msg_handler));
}

int scene_inst::send_client(uint32 role_id, google::protobuf::Message& msg)
{
	size_t msg_len = 0;
	void* buf = encode(msg, msg_len);
	if (buf == NULL) {
		return -1;
	}
	amtsvr_send(_ctx, 0, get_scene_user(role_id)->get_client_id(), PTYPE_CLIENT|PTYPE_TAG_DONTCOPY, 0, buf, msg_len);

	return 0;
}

bool scene_inst::suspend(uint32 ut)
{
    if (ut > 300 || ut == 0) {
        ut = 300;
    }

    set_suspend_tick(ut * 100);
    return true;
}

bool scene_inst::resume()
{
    set_suspend_tick(0);
    return true;
}

void scene_inst::npc_watched_user(scene_user* su, std::vector<scene_npc*>& push_npcs)
{
    uint32 size = push_npcs.size();
    if (su == NULL || size == 0) {
        return;
    }

    for (uint32 i = 0; i < size; ++i) {
        push_npcs[i]->on_watch(su);
        get_check_create_npc_rule()->on_create_npc_find_monster(push_npcs[i]->get_role_id());
    }
}

void scene_inst::npc_watch_user(scene_role* sr, std::vector<style_box*>& move_box)
{
    if (sr == NULL || (sr->get_role_type() != RTE_NPC && sr->get_role_type() != RTE_PET)) {
        return;
    }

    scene_npc* n = static_cast<scene_npc*>(sr);
    if (!n->can_watch()) {
        return;
    }

    std::vector<scene_user*> push_users;
    get_screen_user(move_box, push_users);
    npc_watch_user(*n, push_users);

    if (sr->get_role_type() == RTE_PET || n->get_origin_npc().get_monster_type() == MT_USER_FRIEND || n->get_origin_npc().get_monster_type() == MT_TEMP_PET || n->get_role_type() == RTE_NPC) {
        std::vector<scene_npc*>  push_npcs;
        get_screen_npc(move_box, push_npcs);
        pet_watch_npc(sr, push_npcs);
    }
}

void scene_inst::npc_watch_user(scene_role* sr, std::vector<scene_user*>& push_users)
{
    if (sr == NULL || (sr->get_role_type() != RTE_NPC && sr->get_role_type() != RTE_PET)) {
        return;
    }

    scene_npc* n = static_cast<scene_npc*>(sr);
    if (!n->can_watch()) {
        return;
    }

    npc_watch_user(*n, push_users);
}

void scene_inst::npc_watch_user(scene_npc& n, vector<scene_user*>& push_users)
{
    uint32 size = push_users.size();
    for (uint32 i = 0; i < size; ++i) {
        n.on_watch(push_users[i]);
    }
}

void scene_inst::pet_watch_npc(scene_role* sr, vector<scene_npc*>& push_npcs)
{
    if (sr == NULL || (sr->get_role_type() != RTE_NPC && sr->get_role_type() != RTE_PET)) {
        return;
    }

    scene_npc* n = static_cast<scene_npc*>(sr);
    if (!n->can_watch()) {
        return;
    }

    uint32 size = push_npcs.size();
    for (uint32 i = 0; i < size; ++i) {
//		if(n->get_feature() == MT_USER_FRIEND || n->get_role_type() == RTE_PET)
//		{
			n->on_watch(push_npcs[i]);
//		}
//		if(push_npcs[i]->get_feature() == MT_USER_FRIEND || push_npcs[i]->get_role_type() == RTE_PET)
//		{
			push_npcs[i]->on_watch(n);
//		}
    }
}

cell_pos scene_inst::to_cell_pos(const map_postion& pos)
{
    const config_map* mc = _gm->get_config_map();
    cell_pos cp;
    uint32 width  = 0;
    uint32 height = 0;
    if (mc != NULL) {
        width  = mc->get_unit_grid_width();
        height = mc->get_unit_grid_height();
    } else {
        width  = DEFAULT_GRID_WIDTH;
        height = DEFAULT_GRID_WIDTH;
    }

    cp.set(pos.x / width, pos.y / height);

    return cp;
}

map_postion scene_inst::to_map_postion(const cell_pos& pos)
{
    const config_map* mc = _gm->get_config_map();
    map_postion mp;
    uint32 width  = 0;
    uint32 height = 0;
    if (mc != NULL) {
        width  = mc->get_unit_grid_width();
        height = mc->get_unit_grid_height();
    } else {
        width  = DEFAULT_GRID_WIDTH;
        height = DEFAULT_GRID_WIDTH;
    }

    mp.reset(pos._x * width + width / 2, pos._y * height + height / 2, 0);

    return mp;
}

bool scene_inst::cell_empty(const cell_pos& pos)
{
    return get_cell().get_role_in_cell(pos) == 0;
}

scene_role* scene_inst::get_role_in_cell(const cell_pos& pos)
{
    uint64 inst_id = get_cell().get_role_in_cell(pos);
    return get_scene_role(inst_id);
}

void scene_inst::role_move_cell(scene_role* sr, const map_postion& from, const map_postion& to)
{
    if (sr == NULL) {
        return ;
    }

    cell_pos from_cp = to_cell_pos(from);
    cell_pos to_cp   = to_cell_pos(to);

    get_cell().enter_cell(sr, to_cp);

    enter_region(sr, from_cp, to_cp);
}

void scene_inst::calc_role_cell_to_map_postion(scene_role* sr, int length, int& ux, int& uy)
{
    if (sr == NULL) {
        return ;
    }

    const map_postion& pos = sr->get_scene_pos();
    int i                  = calc_cell_length(length);
    float x = 0.0f, y = 0.0f;
    get_normalize_d(sr->get_facing(), x, y);
    ux = (uint32)(pos.x - x * i);
    uy = (uint32)(pos.y - y * i);
    /*switch (sr->get_facing()) {
      case NORTH    : ux = pos.x    , uy = pos.y + i; break;
      case NORTHEAST: ux = pos.x - i, uy = pos.y + i; break;
      case EAST     : ux = pos.x - i, uy = pos.y    ; break;
      case SOUTHEAST: ux = pos.x - i, uy = pos.y - i; break;
      case SOUTH    : ux = pos.x    , uy = pos.y - i; break;
      case SOUTHWEST: ux = pos.x + i, uy = pos.y - i; break;
      case WEST     : ux = pos.x + i, uy = pos.y    ; break;
      case NORTHWEST: ux = pos.x + i, uy = pos.y + i; break;
      default: break;
      }*/
}

void scene_inst::calc_pos_to_pos(const map_postion& pos, ROLE_FACING rf, int length, map_postion& p)
{
    float x = 0.0f, y = 0.0f;
    get_normalize_d(rf, x, y);
    p.x = (int)(pos.x + x * length);
    p.y = (int)(pos.y + y * length);
}

bool scene_inst::get_normalize_d(ROLE_FACING rf, float& x, float& y)
{
    switch (rf) {
    case NORTH    : x =  0    , y = -1    ; break;
    case NORTHEAST: x =  0.7f , y = -0.7f ; break;
    case EAST     : x =  1    , y =  0    ; break;
    case SOUTHEAST: x =  0.7f , y =  0.7f ; break;
    case SOUTH    : x =  0    , y =  1    ; break;
    case SOUTHWEST: x = -0.7f , y =  0.7f ; break;
    case WEST     : x = -1    , y =  0    ; break;
    case NORTHWEST: x = -0.7f , y = -0.7f ; break;
    default: return false;
    }

    return true;
}

int scene_inst::calc_cell_length(int l)
{
    const config_map* mc = get_map().get_config_map();
    if (mc == NULL) {
        return l * DEFAULT_GRID_WIDTH;
    }

    return l * mc->get_unit_grid_width();
}

void scene_inst::fix_pos(map_postion& p)
{
    p.x = max(0, p.x);
    p.y = max(0, p.y);
    p.x = min(p.x, (int32)get_scene().get_scene_real_width());
    p.y = min(p.y, (int32)get_scene().get_scene_real_height());
}

void scene_inst::fix_role_pos(map_postion& p)
{
    fix_pos(p);
    if (get_cell().is_empty(to_cell_pos(p))) {
        return ;
    }

    std::vector<cell_pos> vtr;
    get_cell().get_around_empty(to_cell_pos(p), vtr);
    if (vtr.empty()) {
        return ;
    }

    p = to_map_postion(vtr[0]);
}

void scene_inst::role_move_check_terrain(scene_role* sr, const map_postion& from, const map_postion& to)
{
    std::vector<client::terrain_info> from_tis, to_tis;
    get_terrain_mgr().get_terrain_by_pos(from, from_tis);
    bool notify = false;
    std::vector<const config_effect*> dels;
    for (size_t i = 0; i < from_tis.size(); ++i) {
        const config_effect* ce = CONFMGR->get_config_effect(from_tis[i].effect_id());
        if (ce == NULL) {
            continue;
        }

        //sr->mutable_buffer()->del_buf(ce);
        //notify = true;
        dels.push_back(ce);
    }

    get_terrain_mgr().get_terrain_by_pos(to, to_tis);
    std::vector<const config_effect*> adds;
    for (size_t i = 0; i < to_tis.size(); ++i) {
        const config_effect* ce = CONFMGR->get_config_effect(to_tis[i].effect_id());
        if (ce == NULL) {
            continue;
        }

        bool target_enemy  = false;
        bool target_own    = false;
        bool target_friend = false;
        const std::vector<int>& target = ce->get_effect_object_type();
        for (size_t k = 0; k < target.size(); ++k) {
            switch (target[k]) {
            case EOT_ENEMY:  target_enemy  = true; break;
            case EOT_OWN:    target_own    = true; break;
            case EOT_FRIEND: target_friend = true; break;
            }
        }

        if (to_tis[i].has_rd()) {
            if (to_tis[i].rd().inst_id() == sr->get_inst_id() && !target_own) {
                continue;
            } else {
                if (!target_enemy || !target_friend) {
                    scene_role* psr = get_scene_role(to_tis[i].rd().inst_id());
                    bool is_f = is_friend(psr, sr);
                    if ((is_f && target_enemy) || (!is_f && target_friend)) {
                        continue;
                    }
                }
            }
        }

        std::vector<const config_effect*>::iterator f = std::find(dels.begin(), dels.end(), ce);
        if (f != dels.end()) {
            dels.erase(f);
        }

        adds.push_back(ce);

//         sr->mutable_buffer()->add_buf(ce);
//         notify = true;

        for (int32 j = 0; j < to_tis[i].trigger_effects_size(); ++j) {
             ce = CONFMGR->get_config_effect(to_tis[i].trigger_effects(j));
             sr->mutable_buffer()->add_buf(ce);
//            adds.push_back(ce);
        }
    }

    for (size_t i = 0; i < dels.size(); ++i) {
        sr->mutable_buffer()->del_buf(dels[i]);
        notify = true;
    }

    for (size_t i = 0; i < adds.size(); ++i) {
        sr->mutable_buffer()->add_buf(adds[i]);
        notify = true;
    }

    if (notify) {
        sr->screen_broadcast_change_buf();
    }
}

void scene_inst::role_on_check_terrain(const client::terrain_info& ti)
{
    postion pos;
    get_terrain_mgr().get_center_point(ti, pos);
    std::vector<scene_role*> vsr;
    get_screen_role(pos, vsr);
    for (size_t i = 0; i < vsr.size(); ++i) {
        if (vsr[i] == NULL) {
            continue;
        }
        role_move_check_terrain(vsr[i], vsr[i]->get_scene_pos(), vsr[i]->get_scene_pos());
    }
}

void scene_inst::scene_broadcast_terrain_change()
{
    if (get_terrain_mgr().is_change()) {
        client::terrain_list tl;
        get_terrain_mgr().fill_change_list(tl);
        scene_broadcast(tl);
    }
}

void scene_inst::role_move_affect(scene_role* r, std::vector<style_box*>& push_box, std::vector<style_box*>& move_box, std::vector<style_box*>& left_box)
{
    std::vector<scene_user*> push_user, left_user;
    std::vector<scene_npc*> push_npc, left_npc;
    get_screen_user(push_box, push_user);
    get_screen_npc(push_box, push_npc);
    get_screen_user(left_box, left_user);
    get_screen_npc(left_box, left_npc);

    scene_user* psu = NULL;
    if (r->get_role_type() == RTE_USER) {
        psu = static_cast<scene_user*>(r);
    }

    // 通知原来区域的用户A已经离开
    if (left_user.size() > 0) {
        multicast_role_remove(left_user, *r);
    }

	// 将A从反击列表中移除
	for(size_t i = 0; i < left_user.size(); ++i)
	{
		left_user[i]->del_counter_role(r);
		r->del_counter_role(left_user[i]);
	}
	for(size_t i = 0; i < left_npc.size(); ++i)
	{
		left_npc[i]->del_counter_role(r);
		r->del_counter_role(left_npc[i]);
	}

    if (psu != NULL) {
        // 将老的角色移除通知给A
        if (left_user.size() > 0 || left_npc.size() > 0) {
            client::role_remove_msg rrm;
            multi_fill_role_remove_msg(*r, left_user, left_npc, rrm);
            psu->send_client(rrm);
        }

        // 新的角色增加通知给A
        if (push_user.size() > 0 || push_npc.size() > 0) {
            push_roles_to_user(*psu, &push_user, &push_npc);
        }
    }

    // 将A推给新的用户
    if (push_user.size() > 0) {
        push_role_to_users(*r, &push_user);
    }

    // 触发AI
    if (psu != NULL) {
        npc_watched_user(psu, push_npc);
    } else {
        npc_watch_user(r, push_user);
    }
}

void scene_inst::move_style_test(const map_postion& from, const map_postion& to, std::vector<style_box*>& push_box, std::vector<style_box*>& move_box, std::vector<style_box*>& left_box)
{
    style_box* old_box = get_style_box(from);
    style_box* now_box = get_style_box(to);
    if (old_box != NULL){
        printf ("from(%d,%d)_to(%d,%d): %d,%d---->", from.x, from.y, to.x, to.y, old_box->get_box_x(), old_box->get_box_y());
    }
    if (now_box != NULL){
        printf ("%d,%d", now_box->get_box_x(), now_box->get_box_y());
    }
    printf("\n");
    printf("push_box:");
    for (uint32 i = 0; i < push_box.size(); ++i){
        printf(" %d,%d ", push_box[i]->get_box_x(), push_box[i]->get_box_y());
    }
    printf("\n");
    printf("move_box:");
    for (uint32 i = 0; i < move_box.size(); ++i){
        printf(" %d,%d ", move_box[i]->get_box_x(), move_box[i]->get_box_y());
    }
    printf("\n");
    printf("left_box:");
    for (uint32 i = 0; i < left_box.size(); ++i){
        printf(" %d,%d ", left_box[i]->get_box_x(), left_box[i]->get_box_y());
    }
    printf("\n");
}

int scene_inst::role_move_stylebox(scene_role* r, const map_postion& from, const map_postion& to)
{
    style_box* old_box = get_style_box(from);
    style_box* now_box = get_style_box(to);

    if (old_box != now_box) {
        switch (r->get_role_type()) {
        case RTE_USER: {
                if (old_box != NULL) {
                    old_box->remove_scene_user(r->get_inst_id());
                }

                if (now_box != NULL) {
                    now_box->add_scene_user(r->get_inst_id(), r->get_role_id());
                }
                break;
        }
        case RTE_NPC:
        case RTE_PET: {
                if (old_box != NULL) {
                    old_box->remove_scene_npc(r->get_inst_id());
                }

                if (now_box != NULL) {
                    now_box->add_scene_npc(r->get_inst_id(), r->get_role_id());
                }
                break;
        }
        default:              break;
        }
    }

    return 0;
}

int scene_inst::on_role_move(scene_role* r, const map_postion& from, const map_postion& to)
{
    std::vector<style_box*> push_box;
    std::vector<style_box*> move_box;
    std::vector<style_box*> left_box;

    scene_user* su = NULL;
    if (r->get_role_type() == RTE_USER) {
        su = static_cast<scene_user*>(r);
        /*const region_info* ri_from = get_region_info(from);
          const region_info* ri_to   = get_region_info(to);
          printf("from(%d,%d)[region id=%d  attk_type=%d, attk_mod=%d]-->to(%d,%d)[region id=%d attk_type=%d attk_mod=%d]\n",
          from.x, from.y, ri_from != NULL ? ri_from->id : 0, ri_from != NULL ? ri_from->attk_type : 0, ri_from != NULL ? ri_from->attk_mode : 0,
          to.x, to.y, ri_to != NULL ? ri_to->id : 0, ri_to != NULL ? ri_to->attk_type : 0, ri_to != NULL ? ri_to->attk_mode : 0);*/
        //su->on_region_change(get_region_info(from), get_region_info(to));
    }

    bool change_box = get_style_move_boxs(from, to, push_box, move_box, left_box);
    //move_style_test(from, to, push_box, move_box, left_box);

    role_move_cell(r, from, to);
    role_move_check_terrain(r, from, to);
    //if (su != NULL) {
        get_check_create_npc_rule()->on_create_npc_into_appeare(r, from, to);
    //}

    uint32 tick = time_utils::tenms();
    std::vector<scene_role*> psrs;
    get_screen_role(left_box, psrs);
    for (size_t index = 0; index < psrs.size(); ++index) {
        psrs[index]->role_out(r, tick);
    }

    psrs.clear();
    get_screen_role(r->get_scene_pos(), psrs);
    for (size_t index = 0; index < psrs.size(); ++index) {
        psrs[index]->role_in(r, tick);
    }

    if (!change_box) {
        if (su != NULL) {
            npc_watched_user(su);
        } else {
            npc_watch_user(r, move_box);
        }

        return 0;
    }

    role_move_stylebox(r, from, to);
    role_move_affect(r, push_box, move_box, left_box);

    return 0;
}

int scene_inst::get_screen_role(std::vector<style_box*>& box, std::vector<scene_role*>& vsr)
{
    std::vector<scene_user*> sus;
    get_screen_user(box, sus);
    for (size_t i = 0; i < sus.size(); ++i) {
        vsr.push_back(sus[i]);
    }

    std::vector<scene_npc*> sns;
    get_screen_npc(box, sns);
    for (size_t i = 0; i < sns.size(); ++i) {
        vsr.push_back(sns[i]);
    }

    return vsr.size();
}

int scene_inst::get_screen_user(std::vector<style_box*>& box, std::vector<scene_user*>& vsr)
{
    size_t count = box.size();
    for (size_t i = 0; i < count; ++i) {
        std::vector<uint64> vi;
        box[i]->get_users(vi);
        size_t vi_count = vi.size();
        for (size_t j = 0; j < vi_count; ++j) {
            uint64 inst_id = vi[j];
            scene_user* tmp_user = static_cast<scene_user*>(_user_map.mut(inst_id));
            if (tmp_user == NULL) {
                box[i]->remove_scene_user(inst_id);
                continue;
            }
            vsr.push_back(tmp_user);
        }
    }

    return vsr.size();
}

int scene_inst::get_screen_npc(std::vector<style_box*>& box, std::vector<scene_npc*>& vsr)
{
    size_t count = box.size();
    for (size_t i = 0; i < count; ++i) {
        std::vector<uint64> vi;
        box[i]->get_npcs(vi);
        size_t vi_count = vi.size();
        for (size_t j = 0; j < vi_count; ++j) {
            uint64 inst_id = vi[j];
            scene_npc* sn = static_cast<scene_npc*>(_npc_map.mut(inst_id));
            if (sn == NULL) {
                box[i]->remove_scene_npc(inst_id);
                continue;
            }
            vsr.push_back(sn);
        }
    }

    return vsr.size();
}


int scene_inst::get_around_role(const map_postion& p, std::vector<scene_role *>& vsr, uint32 distance, bool get_user, bool get_npc)
{
	std::vector<scene_role *> v;
	get_scene_roles(v);
	for(size_t i = 0; i < v.size(); ++i)
	{
		if(v[i]->get_role_type() == RTE_USER && get_user && p.is_near_pos(v[i]->get_scene_pos(), distance))
		{
			vsr.push_back(v[i]);
			continue;
		}
		if(v[i]->get_role_type() == RTE_NPC  && get_npc  && p.is_near_pos(v[i]->get_scene_pos(), distance))
		{
			vsr.push_back(v[i]);
		}
	}
	return vsr.size();
}

int scene_inst::get_screen_role(const postion& p, std::vector<scene_role*>& vsr, bool get_user /* = true */, bool get_npc /* = true */)
{
    std::vector<style_box*> box;
    get_style_boxs(p, box);
    size_t count = box.size();
    for (size_t i = 0; i < count; ++i) {
        if (get_user) {
            std::vector<uint64> vi;
            box[i]->get_users(vi);
            size_t vi_count = vi.size();
            for (size_t j = 0; j < vi_count; ++j) {
                uint64 inst_id = vi[j];
                scene_user* tmp_user = static_cast<scene_user*>(_user_map.mut(inst_id));
                if (tmp_user == NULL) {
                    box[i]->remove_scene_user(inst_id);
                    continue;
                }
                vsr.push_back(tmp_user);
            }
        }

        if (get_npc) {
            std::vector<uint64> vi;
            box[i]->get_npcs(vi);
            size_t vi_count = vi.size();
            for (size_t j = 0; j < vi_count; ++j) {
                uint64 inst_id = vi[j];
                scene_npc* sn = static_cast<scene_npc*>(_npc_map.mut(inst_id));
                if (sn == NULL) {
                    box[i]->remove_scene_npc(inst_id);
                    continue;
                }
                vsr.push_back(sn);
            }
        }
    }

    return vsr.size();
}

int scene_inst::remove_style_box_pos(scene_role& r)
{
    style_box* box = get_style_box(r.get_scene_pos());
    if (box == NULL) {
        return -1;
    }

    switch (r.get_role_type()) {
    case RTE_USER: box->remove_scene_user(r.get_inst_id()); break;
    case RTE_NPC:
    case RTE_PET: box->remove_scene_npc(r.get_inst_id()); break;
    default:    break;
    }

    screen_broadcast_remove(r);
    get_cell().leave_cell(&r);

    return 0;
}

int scene_inst::remove_role(scene_role* r)
{
    if (r == NULL || r->get_role_type() == RTE_PET || _npc_map.remove(r->get_inst_id()) != 0) {
        return -1;
    }

    _remove_role.insert(std::pair<uint64, scene_role*>(r->get_inst_id(), r));

    return 0;
}

// 场景内的公告(仅当前场景的人看的到)
int scene_inst::scene_announcement(uint32 id, const vector<client::content_data> &data)
{	
	const config_mail *cm = CONFMGR->get_config_mail_mgr().get_config_mail(id);
	if(cm == NULL)
	{
		return -1;
	}
	client::broadcast_area_rsp rsp;
	client::mail_body *mb = rsp.mutable_area_msg();
	rsp.set_area_id(id);
	mb->set_content(cm->get_content_value());
	for(size_t i = 0; i < data.size(); ++i)
	{
		client::content_data *cd = mb->add_data();
		cd->CopyFrom(data[i]);
	}
	scene_broadcast(rsp);
	return 0;
}

int scene_inst::scene_broadcast(google::protobuf::Message& msg)
{
    int id_count = 0;
    uint32 ids[MAX_BROADCAST_AGENT_NUM];

    shobj* cur = _user_map.get_all_objs_head();
    for (; cur != NULL; cur = cur->next[SLPE_LIST]) {
        ids[id_count++] = (static_cast<scene_user*>(cur->sr)->get_client_id());
    }

    if (id_count == 0) {
        return 0;
    }

    size_t msg_len = 0;
    void* buf = encode(msg, msg_len);
    if (buf == NULL) {
        return 0;
    }

    amtsvr_broadcast(_ctx, ids, id_count, buf, msg_len);

    return 0;
}

int scene_inst::scene_broadcast(scene_role& r, google::protobuf::Message& msg, bool except_me)
{
    uint64 rid = r.get_inst_id();
    int id_count = 0;
    uint32 ids[MAX_BROADCAST_AGENT_NUM];
    shobj* cur = _user_map.get_all_objs_head();
    for (; cur != NULL; cur = cur->next[SLPE_LIST]) {
        if (except_me && cur->sr->get_inst_id() == rid) {
            continue;
        }

        ids[id_count++] = (static_cast<scene_user*>(cur->sr)->get_client_id());
    }

    if (id_count == 0) {
        return 0;
    }

    size_t msg_len = 0;
    void* buf = encode(msg, msg_len);
    if (buf == NULL) {
        return 0;
    }

    amtsvr_broadcast(_ctx, ids, id_count, buf, msg_len);

    return 0;
}

int scene_inst::scene_broadcast(std::vector<google::protobuf::Message*>& msgs)
{
    int id_count = 0;
    uint32 ids[MAX_BROADCAST_AGENT_NUM];
    shobj* cur = _user_map.get_all_objs_head();
    for (; cur != NULL; cur = cur->next[SLPE_LIST]) {
        ids[id_count++] = (static_cast<scene_user*>(cur->sr)->get_client_id());
    }

    if (id_count == 0) {
        return 0;
    }

    uint32 msg_size = msgs.size();
    for (uint32 i = 0; i < msg_size; ++i) {
        size_t msg_len = 0;
        void* buf = encode(*msgs[i], msg_len);
        if (buf == NULL) {
            continue;
        }

        amtsvr_broadcast(_ctx, ids, id_count, buf, msg_len);
    }

    return 0;
}

int scene_inst::screen_fill_move_role_msg(scene_role& r, client::role_move_msg& rmm, client::multi_role_msg& mrm, client::role_name_msg& rnm, client::buffer_list& bl)
{
    std::vector<scene_role*> vsr;
    int count = get_screen_role(r.get_scene_pos(), vsr, true, true);
    int i = 0;
    bool send_name = is_need_broadcast_npc_name();
    for ( ; i < count; ++i) {
        if (vsr[i]->is_same_role(r)) {
            continue;
        }

        vsr[i]->fill_role_move_msg(rmm);
        vsr[i]->fill_role_broadcast_msg(*(mrm.add_rm()));
        if (send_name || vsr[i]->get_role_type() == RTE_USER || vsr[i]->get_role_type() == RTE_PET) {
            vsr[i]->fill_role_name(*(rnm.add_rn()));
        }

        vsr[i]->fill_buffer(bl);
    }

    return i;
}

void scene_inst::multi_broadcast_modify_role_msg(scene_role& r, std::vector<scene_role*>& roles)
{
    client::multi_role_msg mrm;
    bool has_change     = false;
    bool need_broadcast = false;
    size_t count = roles.size();
    client::role_attr broadcast_attr;
    bool change_role = false;

    for (size_t i = 0; i < count; ++i) {
        client::role_msg* rm = mrm.add_rm();
        change_role = roles[i]->fill_role_modify_msg(*rm, broadcast_attr, need_broadcast);
        if (has_change) {
            continue;
        }

        if (change_role) {
            has_change = true;
        }
    }

    if (has_change) {
        screen_broadcast(r, mrm, false);
    }
}

scene_user* scene_inst::get_scene_user(uint64 id)
{
    return static_cast<scene_user*>(_user_map.mut(id));
}

scene_user* scene_inst::get_scene_user_by_role_id(uint32 id)
{
    return get_scene_user(id);
}

scene_npc* scene_inst::get_scene_npc(uint64 id)
{
    return static_cast<scene_npc*>(_npc_map.mut(id));
}

scene_npc* scene_inst::get_scene_npc_by_role_id(uint32 id)
{
    shobj* cur = _npc_map.get_all_objs_head();
    for (; cur != NULL; cur = cur->next[SLPE_LIST]) {
        if (cur->sr->get_role_id() == id) {
            return static_cast<scene_npc*>(cur->sr);
        }
    }

    return NULL;
}

scene_npc* scene_inst::get_scene_pick(uint64 id)
{
    return static_cast<scene_npc*>(_npc_map.mut(id));
}

scene_role* scene_inst::get_scene_role(uint64 id)
{
    scene_role* r= get_scene_user(id);
    if (r != NULL) {
        return r;
    }

    return get_scene_npc(id);
}

scene_role* scene_inst::get_scene_role(const ::client::role_data& rd)
{
    scene_role* dst_role = NULL;
    switch (rd.role_typ()) {
    case client::role_data_TYPE_USER: dst_role = get_scene_user(rd.inst_id()); break;
    case client::role_data_TYPE_NPC :
    case client::role_data_TYPE_PET : dst_role = get_scene_npc(rd.inst_id()); break;
    default: break;
    }

    return dst_role;
}

bool scene_inst::get_rectangle(scene_role* psr, const map_postion& p1, const map_postion& p2, uint32 length, map_postion& mp1, map_postion& mp2, map_postion& mp3, map_postion& mp4)
{
    if (psr == NULL) {
        return false;
    }

    ROLE_FACING l, r;
    switch (psr->get_facing()) {
    case NORTH    : l = WEST     , r = EAST     ; break;
    case NORTHEAST: l = NORTHWEST, r = SOUTHEAST; break;
    case EAST     : l = NORTH    , r = SOUTH    ; break;
    case SOUTHEAST: l = NORTHEAST, r = SOUTHWEST; break;
    case SOUTH    : l = EAST     , r = WEST     ; break;
    case SOUTHWEST: l = SOUTHEAST, r = NORTHWEST; break;
    case WEST     : l = SOUTH    , r = NORTH    ; break;
    case NORTHWEST: l = SOUTHWEST, r = NORTHWEST; break;
    default: return false;
    }

    calc_pos_to_pos(p1, l, length, mp1);
    calc_pos_to_pos(p1, r, length, mp2);
    calc_pos_to_pos(p2, r, length, mp3);
    calc_pos_to_pos(p2, l, length, mp4);

    return true;
}

class distance_lesser{
public:
    distance_lesser(const map_postion& pos) {
        _pos = pos;
    }

    ~distance_lesser() {
    }

public:
    bool operator()(const scene_role* sr0, const scene_role* sr1)
    {
        if (sr0 == NULL) {
            return false;
        } else if (sr1 == NULL) {
            return true;
        }

        return _pos.get_fdistance(sr0->get_map_pos()) < _pos.get_fdistance(sr1->get_map_pos());
    }

private:
    map_postion _pos;
};

void scene_inst::sort_by_distance(const map_postion& pos, std::vector<scene_role*>& vsr)
{
    std::sort(vsr.begin(), vsr.end(), distance_lesser(pos));
}

int scene_inst::select_target(scene_role* atk, skill* ps, std::vector<scene_role*>& enemies, std::vector<scene_role*>& friends, std::vector<map_postion>& miss)
{
    if (atk == NULL || ps == NULL) {
        return -1;
    }

    const config_skill* pcs = ps->get_config_skill();
    if (pcs == NULL) {
        return -2;
    }

    const std::vector<int>& ts = pcs->get_effect_object_type();
    uint32 t = EOT_ENEMY;
    if (!ts.empty()) {
        t = ts[0];
    }

    cell_pos pos = to_cell_pos(atk->get_scene_pos());
    map_postion mp1, mp2, mp3, mp4;
    int ux = 0, uy = 0, r = 0;
    std::vector<uint64>   vi;
    std::vector<cell_pos> vc;
    std::vector<scene_role*> out_space_role;
    switch(pcs->get_method_shape()) {
    case MST_POINT: {
        break;
    }
    case MST_POINT_AROUND:
    case MST_LINE: {
        float w = calc_cell_length(pcs->get_line_width()) / 2.0f;
        map_postion p1, p2;
        p1 = to_map_postion(to_cell_pos(atk->get_scene_pos()));
        calc_pos_to_pos(p1, atk->get_facing(), calc_cell_length(pcs->get_distance() + 1) , p2);
        get_rectangle(atk, p1, p2, (uint32)w, mp1, mp2, mp3, mp4);
        get_cell().line(pos, atk->get_facing(), pcs->get_distance(), pcs->get_line_width(), pcs->get_affect_object_max(), vi, vc);
        break;
    }
    case MST_CONE: {
        calc_role_cell_to_map_postion(atk, pcs->get_distance(), ux, uy);
        r = calc_cell_length(pcs->get_distance());
        get_cell().triangle(pos, atk->get_facing(), pcs->get_distance(), pcs->get_affect_object_max(), vi, vc);
        break;
    }
    case MST_MOVE_CIRCLE:
    case MST_CIRCLE: {
        get_cell().around(pos, atk->get_facing(), pcs->get_distance(), pcs->get_affect_object_max(), vi, vc);
        break;
    }
    case MST_TRIGEMINAL: {
        calc_role_cell_to_map_postion(atk, pcs->get_distance(), ux, uy);
        r = calc_cell_length(pcs->get_distance());
        get_cell().trigeminal(pos, atk->get_facing(), pcs->get_distance(), pcs->get_affect_object_max(), vi, vc);
        break;
    }
    default: break;
    }
    for (size_t i = 0; i < vi.size(); ++i) {
        scene_role* psr = get_scene_role(vi[i]);
		if(psr == NULL || psr == atk || psr->get_feature() == NT_PICK) {
			continue ;
		}
		if(psr->get_status().is_dead())
		{
			if(psr->get_role_type() != RTE_NPC)
			{
				continue;
			}
			scene_npc *sn = static_cast<scene_npc *>(psr);
			if(!sn->get_origin_npc().is_boss() || get_scene_type() != MAP_TYPE_WORLD_BOSS)
			{
				continue;
			}
		}
        switch (pcs->get_method_shape()) {
        case MST_POINT_AROUND:
        case MST_LINE: {
            if (!map_postion::is_point_in_rectangle(mp1, mp2, mp3, mp4, psr->get_scene_pos())) {
                continue;
            }
            break;
        }
        case MST_CONE:
        case MST_TRIGEMINAL: {
            if (!map_postion::is_point_in_circular_sector((float)atk->get_scene_pos().x, (float)atk->get_scene_pos().y, (float)ux, (float)uy, (float)r, pcs->get_arc_angle() / 2.0f, (float)psr->get_scene_pos().x, (float)psr->get_scene_pos().y)) {
                out_space_role.push_back(psr);
                continue;
            }
            break;
        }

        default: break;
        }

        if (is_friend(atk, psr)) {
            friends.push_back(psr);
            continue;
        }

        if (atk->get_role_type() == RTE_NPC && psr->get_role_type() == RTE_NPC && atk->get_role_id() == psr->get_role_id()) {
            friends.push_back(psr);
            continue;
        }

        if (psr->get_master() == atk) {
            friends.push_back(psr);
            continue;
        }

        if (atk->get_buffer().get_taunt().inst_id() != 0 && atk->get_buffer().get_taunt().inst_id() != psr->get_inst_id()) {
            continue;
        }

        enemies.push_back(psr);
    }

    if (pcs->get_method_shape() == MST_TRIGEMINAL && enemies.empty() && t == EOT_ENEMY && !out_space_role.empty()) {
        bool fix = false;
        for (size_t i = 0; i < out_space_role.size(); ++i) {
            if (is_friend(atk, out_space_role[i])) {
                continue;
            }

            if (!fix) {
                atk->set_facing((float)out_space_role[i]->get_scene_pos().x, (float)out_space_role[i]->get_scene_pos().y);
                calc_role_cell_to_map_postion(atk, pcs->get_distance(), ux, uy);
                fix = true;
            }

            if (!map_postion::is_point_in_circular_sector((float)atk->get_scene_pos().x, (float)atk->get_scene_pos().y, (float)ux, (float)uy, (float)r, pcs->get_arc_angle() / 2.0f, (float)out_space_role[i]->get_scene_pos().x, (float)out_space_role[i]->get_scene_pos().y)) {
                continue;
            }

            if (atk->get_buffer().get_taunt().inst_id() != 0 && atk->get_buffer().get_taunt().inst_id() != out_space_role[i]->get_inst_id()) {
                continue;
            }
            enemies.push_back(out_space_role[i]);
        }
    }

    if ((pcs->get_method_shape() == MST_LINE || pcs->get_method_shape() == MST_POINT_AROUND) && enemies.empty()) {
        for (size_t i = 0; i < vi.size(); ++i) {
            scene_role* psr = get_scene_role(vi[i]);

			if(psr == NULL || psr == atk || psr->get_feature() == NT_PICK)
			{
				continue ;
			}
			if(psr->get_status().is_dead())
			{
				if(psr->get_role_type() != RTE_NPC)
				{
					continue;
				}
				scene_npc *sn = static_cast<scene_npc *>(psr);
				if(!sn->get_origin_npc().is_boss() || get_scene_type() != MAP_TYPE_WORLD_BOSS)
				{
					continue;
				}
			}
            if (is_friend(atk, psr)) {
                continue;
            }

            if (atk->get_role_type() == RTE_NPC && psr->get_role_type() == RTE_NPC && atk->get_role_id() == psr->get_role_id()) {
                continue;
            }

            if (atk->get_buffer().get_taunt().inst_id() != 0 && atk->get_buffer().get_taunt().inst_id() != psr->get_inst_id()) {
                continue;
            }

            enemies.push_back(psr);
        }
    }

    sort_by_distance(atk->get_scene_pos(), enemies);
    if (enemies.size() > pcs->get_affect_object_max()) {
        enemies.erase(enemies.begin() + pcs->get_affect_object_max(), enemies.end());
    }

    if (pcs->get_method_shape() == MST_POINT_AROUND && !enemies.empty()) {
        vi.clear();
        vc.clear();
        enemies.erase(enemies.begin() + 1, enemies.end());
        get_cell().around(to_cell_pos(enemies[0]->get_scene_pos()), atk->get_facing(), pcs->get_round_radius(), pcs->get_affect_object_max(), vi, vc);
        for (size_t i = 0; i < vi.size(); ++i) {
            scene_role* psr = get_scene_role(vi[i]);

            if (is_friend(atk, psr)) {
                continue;
            }

			if(psr == NULL || psr->get_master() == atk || psr == atk) {
				continue ;
			}
			if(psr->get_status().is_dead())
			{
				if(psr->get_role_type() != RTE_NPC)
				{
					continue;
				}
				scene_npc *sn = static_cast<scene_npc *>(psr);
				if(!sn->get_origin_npc().is_boss() || get_scene_type() != MAP_TYPE_WORLD_BOSS)
				{
					continue;
				}
			}
            if (atk->get_role_type() == RTE_NPC && psr->get_role_type() == RTE_NPC && atk->get_role_id() == psr->get_role_id()) {
                continue;
            }

            if (atk->get_buffer().get_taunt().inst_id() != 0 && atk->get_buffer().get_taunt().inst_id() != psr->get_inst_id()) {
                continue;
            }

            enemies.push_back(psr);

            if (enemies.size() >= pcs->get_affect_object_max()) {
                break;
            }
        }
    }

    for (size_t i = 0; i < vc.size(); ++i) {
        miss.push_back(to_map_postion(vc[i]));
    }

    if (pcs->get_miss_blow() == SMB_MISS_BLOW) {
        size_t hit_count  = 0;
        size_t miss_count = 0;
        for (uint32 i = 0; i < pcs->get_affect_object_max(); ++i) {
            if (mtrandom::rand_int32(10000) < pcs->get_hit_rate() && hit_count < enemies.size()) {
                ++hit_count;
            } else if (miss_count < miss.size()) {
                ++miss_count;
            } else if (hit_count < enemies.size()) {
                ++hit_count;
            }
        }

        std::random_shuffle(enemies.begin(), enemies.end());
        enemies.erase(enemies.begin() + hit_count, enemies.end());
        std::random_shuffle(miss.begin(), miss.end());
        miss.erase(miss.begin() + miss_count, miss.end());
    }

    if (atk->get_real_role_type() == RTE_USER) {
//        filter_target ft;
//        ft.filter_target_by_atk_mode(atk, enemies, friends);
    }

    return 0;
}

int scene_inst::on_attack(scene_role* src_role, std::vector<scene_role*>& dst_role_list, skill& sk)
{
    return 0;
}

void scene_inst::team_add_exp(scene_user* su, uint32 exp)
{
    if (su == NULL) {
        return ;
    }

    std::vector<style_box*> boxs;
    get_style_boxs(su->get_scene_pos().get_point(), boxs);

    std::vector<scene_user*> role_list;
    get_screen_user(boxs, role_list);

    uint32 role_num = role_list.size();
    for (uint32 i = 0; i < role_num; ++i) {
        scene_user* u = role_list[i];
        if (u == NULL || su->is_same_role(*u) || su->get_team_id() != u->get_team_id()) {
            continue;
        }

        u->add_exp(exp);
    }
}

void scene_inst::add_activity(uint64 id, uint32 count)
{
#ifndef __LIB_MINI_SERVER__
    scene_user* su = get_scene_user(id);
    if (su == NULL) {
        return ;
    }

    bool add = false;
    switch (count) {
    case ACTIVITY_FAMILY_BOSS:
        add = su->get_player_role().get_role_statis_mgr().family_boss();
        break;
    case ACTIVITY_SECRETE_ELITE:
        add = su->get_player_role().get_role_statis_mgr().secrete_elite();
        su->get_player_role().get_role_statis_mgr().get_target().kill_elite();
        break;
    case ACTIVITY_WORLD_BOSS:
        add = su->get_player_role().get_role_statis_mgr().world_boss();
        break;
    default: break;
    }

    if (add) {
        su->add_activity(count);
    }
#endif
}

bool scene_inst::is_friend(scene_role* psr0, scene_role* psr1)
{
    if (psr0 == NULL || psr1 == NULL) {
        return false;
    }

    if (get_scene_type() == MAP_TYPE_NORMAL) {
        return true;
    }

    if (psr0->get_feature() == MT_NPC || psr1->get_feature() == MT_NPC ||
        psr0->get_feature() == MT_PICK || psr1->get_feature() == MT_PICK ||
        psr0->get_feature() == MT_TASK_NPC || psr1->get_feature() == MT_TASK_NPC) {
        return true;
    }

    if (psr0->same_master(psr1)) {
        return true;
    }

    if (psr0->get_role_type() == RTE_USER && psr0->get_real_role_type() == psr1->get_real_role_type()) {
        return false;
    }

    ROLE_TYPE_E rt0 = psr0->get_real_role_type();
    ROLE_TYPE_E rt1 = psr1->get_real_role_type();

    if (rt0 == RTE_NPC) {
        scene_npc* sn = static_cast<scene_npc*>(psr0);
        if (sn->get_origin_npc().get_monster_type() == MT_USER_FRIEND) {
            rt0 = RTE_USER;
        }
    }

    if (rt1 == RTE_NPC) {
        scene_npc* sn = static_cast<scene_npc*>(psr1);
        if (sn->get_origin_npc().get_monster_type() == MT_USER_FRIEND) {
            rt1 = RTE_USER;
        }
    }

    return rt0 == rt1;
}

void scene_inst::get_scene_users(std::vector<scene_user*>& us)
{
    shobj* cur = _user_map.get_all_objs_head();
    for (; cur != NULL; cur = cur->next[SLPE_LIST]) {
        us.push_back(static_cast<scene_user*>(cur->sr));
    }
}

void scene_inst::get_scene_roles(std::vector<scene_role *>&v)
{
	shobj *cur = _user_map.get_all_objs_head();
	for(; cur != NULL; cur = cur->next[SLPE_LIST]) {
		v.push_back(cur->sr);
	}
	cur = _npc_map.get_all_objs_head();
	for(; cur != NULL; cur = cur->next[SLPE_LIST]) {
		v.push_back(cur->sr);
	}
}

uint32 scene_inst::get_survive_count()
{
	uint32 count = _user_map.obj_num();
    shobj* cur_npcs = _npc_map.get_all_objs_head();
    for ( ; cur_npcs != NULL; cur_npcs = cur_npcs->next[SLPE_LIST]) {
        scene_npc* n = static_cast<scene_npc*>(cur_npcs->sr);
        if (n->get_status().is_dead()) {
            continue;
        }

		if (n->get_feature() == MT_USER_FRIEND) {
			count++;
		}
    }

	return count;
}

scene_user* scene_inst::get_first_user()
{
    shobj* cur = _user_map.get_all_objs_head();
    if (cur == NULL) {
        return NULL;
    }

    return static_cast<scene_user*>(cur->sr);
}

void scene_inst::get_alive_scene_npcs(std::vector<scene_npc*>& ns)
{
    shobj* cur = _npc_map.get_all_objs_head();
    for (; cur != NULL; cur = cur->next[SLPE_LIST]) {
        scene_npc* n = static_cast<scene_npc*>(cur->sr);
        if (n->get_status().is_dead()) {
            continue;
        }

        ns.push_back(n);
    }
}

bool scene_inst::is_scene_npc_all_dead()
{
    shobj* cur = _npc_map.get_all_objs_head();
    for (; cur != NULL; cur = cur->next[SLPE_LIST]) {
        scene_npc* n = static_cast<scene_npc*>(cur->sr);
        if (n->get_status().is_dead()) {
            continue;
        }

        if (n->get_real_role_type() == RTE_USER) {
            continue;
        }

        if (n->get_origin_npc().get_monster_type() == MT_USER_FRIEND ||
            n->get_origin_npc().get_monster_type() == MT_TEMP_PET ||
            n->get_origin_npc().get_monster_type() == MT_PICK ||
            n->get_origin_npc().get_monster_type() == MT_NPC ||
            n->get_origin_npc().get_monster_type() == MT_TASK_NPC) {
                continue;
        }

        return false;
    }

    return true;
}


uint32 scene_inst::get_alive_monster(std::vector<scene_npc*>& ns)
{
    shobj* cur = _npc_map.get_all_objs_head();
    for (; cur != NULL; cur = cur->next[SLPE_LIST]) {
        scene_npc* n = static_cast<scene_npc*>(cur->sr);
        if (n->get_status().is_dead()) {
            continue;
        }

        if (n->get_real_role_type() == RTE_USER) {
            continue;
        }

        if (n->get_origin_npc().get_monster_type() == MT_USER_FRIEND ||
            n->get_origin_npc().get_monster_type() == MT_TEMP_PET ||
            n->get_origin_npc().get_monster_type() == MT_PICK ||
            n->get_origin_npc().get_monster_type() == MT_NPC ||
            n->get_origin_npc().get_monster_type() == MT_TASK_NPC) {
                continue;
        }

        ns.push_back(n);
    }

    return ns.size();
}

void scene_inst::get_scene_users(std::vector<uint32>& us)
{
    shobj* cur = _user_map.get_all_objs_head();
    for (; cur != NULL; cur = cur->next[SLPE_LIST]) {
        us.push_back(cur->sr->get_role_id());
    }
}

int scene_inst::find_path(const map_postion& from, const map_postion& to, std::vector<move_map_postion>& path)
{
    return get_scene().get_nav().find_path(from, to, path);
}

void scene_inst::destroy_user(scene_user* su)
{
    client::user_lose ul;
    ul.set_role_id(su->get_role_id());
    send_center(ul); // 通知Center用户离线了

    leave_scene(su);
    su->detach_scene();

    delete su;
}

void scene_inst::destory_user_all()
{
    shobj* cur = _user_map.get_all_objs_head();
    for (; cur != NULL; cur = cur->next[SLPE_LIST]) {
        scene_user* psu = static_cast<scene_user*>(cur->sr);
        if (psu == NULL) {
            continue;
        }

        destroy_user(psu);
    }

    _user_map.init();
}

void scene_inst::save()
{
//	return ;
    shobj* cur = _user_map.get_all_objs_head();
    for (; cur != NULL; cur = cur->next[SLPE_LIST]) {
        scene_user* psu = static_cast<scene_user*>(cur->sr);
        if (psu == NULL) {
            continue;
        }

        psu->save_user();
        psu->get_player_role().write_all(_ctx);
    }
}

void scene_inst::release()
{
    shobj* cur = _user_map.get_all_objs_head();
    for (; cur != NULL; cur = cur->next[SLPE_LIST]) {
        scene_user* psu = static_cast<scene_user*>(cur->sr);
        if (psu == NULL) {
            continue;
        }

		this->leave_scene(psu);
		psu->detach_scene();
        player_role* p = psu->detach_player_role();
        delete p;
        delete psu;
    }
}

int scene_inst::push_role_to_users(scene_role& sr, std::vector<scene_user*>* push_user /* = NULL */, bool born /* = false */)
{
    std::vector<google::protobuf::Message*> msgs;

    client::multi_role_msg mrm;
    sr.fill_role_broadcast_msg(*mrm.add_rm(), born);
    msgs.push_back(&mrm);

    /*client::role_pos_msg rpm;
    sr.fill_role_pos_msg(rpm);
    msgs.push_back(rpm);*/

    client::role_move_msg rmm ;
    sr.fill_role_move_msg(rmm);
    msgs.push_back(&rmm);

    client::role_name_msg rnm;
    if ((sr.get_role_type() == RTE_PET && sr.get_master() != NULL) 
        || sr.get_role_type() == RTE_USER 
        || is_need_broadcast_npc_name()) {
        sr.fill_role_name_msg(rnm);
        msgs.push_back(&rnm);
    }

    client::buffer_list bl;
    if (sr.fill_buffer(bl)) {
        msgs.push_back(&bl);
    }

    if (push_user == NULL) {
        screen_broadcast(sr, msgs, true);
    } else {
        multicast(sr, *push_user, msgs, true);
    }

    return 0;
}

int scene_inst::push_roles_to_user(scene_user& su, std::vector<scene_user*>* push_user, std::vector<scene_npc*>* push_npc)
{
    client::multi_role_msg mrm;
    client::role_move_msg rmm;
    client::role_name_msg rnm;
    client::buffer_list bl;

    if (push_user != NULL && push_npc != NULL) {
        if (multi_fill_move_role_msg(su, *push_user, *push_npc, rmm, mrm, rnm, bl) > 0) {
            su.send_client(_ctx, mrm);
            su.send_client(_ctx, rmm);
            su.send_client(_ctx, rnm);
            if (bl.boi_size() > 0) {
                su.send_client(_ctx, bl);
            }
        }
    } else {
        if (screen_fill_move_role_msg(su, rmm, mrm, rnm, bl) > 0) {
            su.send_client(_ctx, mrm);
            su.send_client(_ctx, rmm);
            su.send_client(_ctx, rnm);
            if (bl.boi_size() > 0) {
                su.send_client(_ctx, bl);
            }
        }
    }

    return 0;
}

int scene_inst::screen_push_role(scene_role* sr)
{
    if (sr == NULL)  {
        return -1;
    }

    switch(sr->get_role_type()) {
    case RTE_USER: screen_push_user(static_cast<scene_user*>(sr)); break;
    case RTE_NPC : screen_push_npc (static_cast<scene_npc* >(sr)); break;
    case RTE_PET : screen_push_pet (static_cast<scene_pet* >(sr)); break;
    default: return -2;
    }

    return 0;
}

int scene_inst::screen_push_user(scene_user* su)
{
    assert(su != NULL);

    style_box* box = get_style_box(su->get_scene_pos());
    if (box != NULL) {
        box->add_scene_user((uint64)su->get_inst_id(), su->get_role_id());
        cell_pos cp = to_cell_pos(su->get_scene_pos());
        get_cell().enter_cell(su, cp);
        enter_region(su, cp, cp);
    }

    std::vector<style_box*>  boxs;
    std::vector<scene_user*> sus;
    std::vector<scene_npc*>  sns;
    get_style_boxs(su->get_scene_pos(), boxs);
    get_screen_user(boxs, sus);
    get_screen_npc(boxs, sns);
    //get_alive_scene_npcs(sns);

    push_role_to_users(*su, &sus);       // 将A推送给同屏用户,后面优化做数据包合并
    push_roles_to_user(*su, &sus, &sns); // 将同屏用户推送给A

    // 触发其它事件
    add_tick_user(su);
    npc_watched_user(su);

    scene_push_init_info(su);

    return 0;
}

int scene_inst::screen_push_npc(scene_npc* sn)
{
    assert(sn != NULL);

    push_role_to_users(*sn);

    return 0;
}

int scene_inst::screen_push_npcs(std::vector<scene_npc*>& sns, bool bron /* = false */)
{
    uint32 size = sns.size();
    if (size == 0) {
        return 0;
    }

    client::multi_role_msg mrm;
    client::role_move_msg rmm ;
    client::role_name_msg rnm;
    client::buffer_list bl;
    std::vector<google::protobuf::Message*> msgs;
    bool send_name = is_need_broadcast_npc_name();
    for (uint32 i = 0; i < size; ++i) {
        if (sns[i] != NULL) {
            msgs.clear();
            mrm.Clear();
            rmm.Clear();
            rnm.Clear();
            bl.Clear();
            sns[i]->fill_role_broadcast_msg(*mrm.add_rm(), bron);
            sns[i]->fill_role_move_msg(rmm);
            if (send_name || sns[i]->get_role_type() == RTE_PET) {
                sns[i]->fill_role_name_msg(rnm);
                msgs.push_back(&rnm);
            }

            if (sns[i]->fill_buffer(bl)) {
                msgs.push_back(&bl);
            }

            msgs.push_back(&rmm);

            screen_broadcast(*(sns[i]), mrm);
            screen_broadcast(*(sns[i]), msgs);
        }
    }

    return 0;
}

int scene_inst::screen_push_pet(scene_pet* sp)
{
    return push_role_to_users(*sp, NULL, true);
}

int scene_inst::trans_back_all_user()
{
    std::vector<scene_user*> sus;
    get_scene_users(sus);
    size_t size = sus.size();
    for (uint32 i = 0; i < size; ++i) {
        if (sus[i]->trans_back_user_at_once() != 0) {
            destroy_user(sus[i]);
        }
    }

    return 0;
}

int scene_inst::trans_back_all_user_by_msg()
{
    std::vector<scene_user*> sus;
    get_scene_users(sus);
    size_t size = sus.size();
    for (uint32 i = 0; i < size; ++i) {
        sus[i]->trans_back_user();
    }

    return 0;
}

void scene_inst::send_sys_msg(uint32 msg_id, const char* cmd, uint32 type/* = 0*/)
{
    client::client_chat_msg ccm;
    client::chat_msg* cm = ccm.mutable_cm();
    cm->set_channel(client::chat_msg_CHANNEL_SYSTEM);
    cm->set_src_rold_id(msg_id);
    cm->set_content(cmd);
    cm->set_dst_role_id(type);
    if (type != scene_user::SYS_MSG_TO_SCENE) {
        send_center(ccm);
    } else {
        scene_broadcast(ccm);
    }
}

void scene_inst::sys_msg_filed_score(scene_user* atk, scene_user* def, uint32 score)
{
    char cmd[128] = {0};
    snprintf(cmd, sizeof(cmd), "%s|%u|%s|%u|%u",  atk->get_role_name(), atk->get_role_id(), def->get_role_name(), def->get_role_id(), score);
    send_sys_msg(308, cmd);
}

void scene_inst::sys_mail(scene_user* su, string& title, string& content,std::vector<client::mail_attr>& mail_attr, std::vector<props_info>& ma,std::vector<client::content_data>& info)
{
	if (su == NULL) {
		return ;
	}

	uint32 attachment_num = 0;
	attachment_num = ma.size();

	if (mail_attr.size()!= 0){
		attachment_num += mail_attr.size();
	}

	const std::vector<role_mail_info>& mail_list  = su->get_player_role().get_role_mails().get_mail_list();
	uint32 size = mail_list.size();
	client::delivery_mail_info dmi;
	client::mail_head* mh = dmi.mutable_mi()->mutable_mh();
	mh->set_mail_id(size +1);
	mh->set_mail_type(MAIL_TYPE_SYSTEM);
	mh->set_recv_role_id(su->get_role_id());
	mh->set_send_role_id(0);
	mh->set_send_role_name("system");
	mh->set_title(title);
	mh->set_send_time((uint32)time(NULL));
	mh->set_past_time(su->get_mail_past_time(MAIL_TYPE_SYSTEM,true));
	mh->set_attachment_num(attachment_num);
	mh->set_is_take(false);
	mh->set_opened(false);
	client::mail_body* mb = dmi.mutable_mi()->mutable_mb();
	mb->set_content(content);
	for (uint32 j = 0; j < info.size(); ++j){
		client::content_data* data_info = mb->add_data();
		data_info->set_n(info[j].n());
		data_info->set_s(info[j].s());
		data_info->set_f(info[j].f());
	}

	for (uint32 j = 0; j < mail_attr.size(); j++){
		client::mail_attr* attr = dmi.mutable_mi()->add_attr();
		attr->set_attr_type(mail_attr[j].attr_type());
		attr->set_attr_value(mail_attr[j].attr_value());
	}

	for (size_t i = 0; i < ma.size(); ++i) {
		dmi.mutable_mi()->add_ma()->CopyFrom(ma[i]);
	}

	dmi.set_recv_name(su->get_role_name());

	su->send_center(dmi);
}

int scene_inst::on_kill_role(scene_role* atk, scene_role* def)
{
	get_check_create_npc_rule()->on_create_npc_kill_monster(def);

    if (atk == NULL || def == NULL) {
        return -1;
    }

    return 0;
}

void scene_inst::enter_region(scene_role* psr, const cell_pos& from_cp, const cell_pos& to_cp)
{
    uint32 from_region = get_cell().get_region(from_cp);
    uint32 to_region   = get_cell().get_region(to_cp);
    if (psr->get_region() != to_region) {
        psr->on_region_change(from_region, to_region);

        if (psr->get_role_type() == RTE_USER) {
            get_region_over()->activation_region(to_region);
        }
    }
}

void scene_inst::on_pick(scene_user *psu, uint64 inst_id)
{
	client::PICK_RET_E ret = psu->pick(inst_id);
	client::pick_rsp rsp;
	rsp.set_role_id(psu->get_role_id());
	rsp.set_inst_id(inst_id);
	rsp.set_ret(ret);
	rsp.set_action(client::PAE_ADD);
	psu->send_client(rsp);
}

int scene_inst::on_role_attr_change(scene_role* psr)
{
    get_region_over()->mark_role_attribute(psr);

    return 0;
}

void scene_inst::broadcast_sculpture()
{
	map<uint32, client::kingdom_sculpture_info>::iterator it;
	client::kingdom_sculpture ks;
	for(it = _sculpture.begin(); it != _sculpture.end(); ++it)
	{
		client::kingdom_sculpture_info *ksi = ks.add_ksi();
		ksi->CopyFrom(it->second);
	}
	scene_broadcast(ks);
}

void scene_inst::send_scene_sculpture(scene_user *su)
{
	if(NULL == su)
	{
		return;
	}
	map<uint32, client::kingdom_sculpture_info>::iterator it;
	client::kingdom_sculpture ks;
	for(it = _sculpture.begin(); it != _sculpture.end(); ++it)
	{
		client::kingdom_sculpture_info *ksi = ks.add_ksi();
		ksi->CopyFrom(it->second);
	}
	su->send_client(ks);
}

void scene_inst::update_sculpture(uint32 npc_id, const string &king, const string &title, const string &family_name)
{
	if(_sculpture.empty())
	{
		client::kingdom_sculpture_info ksi;
		ksi.set_role_id(npc_id);
		ksi.set_king(king);
		ksi.set_title(title);
		ksi.set_family_name(family_name);
		ksi.set_inst_id(generate_npc_inst_id());
		ksi.set_x(1844);
		ksi.set_y(1364);
		_sculpture.insert(make_pair(npc_id, ksi));
	}
	else
	{
		map<uint32, client::kingdom_sculpture_info>::iterator it = _sculpture.begin();
		it->second.set_king(king);
		it->second.set_title(title);
		it->second.set_family_name(family_name);
		it->second.set_role_id(npc_id);
	}
	broadcast_sculpture();
}

void scene_inst::fill_load_info(client::offline_load_image &li)
{
	li.set_scene_id(get_scene_id());
	shobj *cur_npc = _npc_map.get_all_objs_head();
    for (; cur_npc != NULL; cur_npc = cur_npc->next[SLPE_LIST]) 
	{
        scene_npc* n = static_cast<scene_npc*>(cur_npc->sr);
        if (n->is_robot()) 
		{
			li.add_role(n->get_role_id());
        }
    }
	shobj *cur_user = _user_map.get_all_objs_head();
	for (; cur_user != NULL; cur_user = cur_user->next[SLPE_LIST]) 
	{
		li.add_role(cur_user->sr->get_role_id());
	}
	li.set_limit_lv(get_ghost_limit_lv());
	li.set_limit_power(get_ghost_limit_power());
	li.set_limit_tl(get_ghost_limit_tl());
}

bool scene_inst::ghost_can_load()
{
	uint32 now = (uint32)time(NULL);
	if(now < time_utils::make_time(now, 0, 8, 0, 0))
	{
		return false;
	}
	return (int)_user_map.obj_num() + _robot_num < CONFMGR->get_basic_value(10203);
}

void scene_inst::load_ghost()
{
	if(!ghost_can_load())
	{
		return;
	}
	client::offline_load_image li;
	fill_load_info(li);
	send_center(li);
}

void scene_inst::erase_ghost(uint32 role_id)
{
	if(get_scene_type() != MAP_TYPE_NORMAL && get_scene_type() != MAP_TYPE_BATTLE)
	{
		return ;
	}
	vector<uint64> v;
	shobj *cur_npc = _npc_map.get_all_objs_head();
    for (; cur_npc != NULL; cur_npc = cur_npc->next[SLPE_LIST]) 
	{
        scene_npc* n = static_cast<scene_npc*>(cur_npc->sr);
		if(n->get_role_id() == role_id)
		{
			v.push_back(n->get_inst_id());
			break;
		}
    }
	clear_npc(v);
}

scene_robot *scene_inst::generate_robot(const client::robot_attr *ra, const map_postion &p, uint32 config_id, uint32 type, uint32 disappear)
{
	config_npc *cn = CONFMGR->get_config_npc_mgr().generate_config_npc(ra->role_id(), config_id, ra->level(), type, disappear);
	if(NULL == cn)
	{
		return NULL;
	}
	origin_npc *o = GAME_MAP_MGR->generate_origin_npc(cn, ra->sk());
	scene_npc *npc = generate_npc_inst(*o, (uint32)time(NULL), p, GNTE_ROBOT, false);
	if (npc == NULL) 
	{
		delete cn;
		delete o;
		return NULL;
	}
	scene_robot *r = static_cast<scene_robot*>(npc);
	r->attach_config_npc(cn);
	r->attach_origin_npc(o);
	r->set_family_id(ra->family_id());
	if(enter_scene(npc) != 0)
	{
		delete r;
		return NULL;
	}
	r->init_robot(ra);
	screen_push_npc(r);
	return r;
}

int scene_inst::push(const client::robot_attr *ra)
{
	map_postion p(ra->x(), ra->y(), (uint32)0);
	scene_robot *sr = generate_robot(ra, p, 1113, 0, CONFMGR->get_basic_value(10206));
	if(NULL == sr)
	{
		return -1;
	}
	return 0;
}

