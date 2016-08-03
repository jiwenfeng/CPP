#include "config_raid.h"
#include "scene_user.h"
#include "raid_scene_inst.h"
#include "levels_scene_inst.h"

#ifndef __LIB_MINI_SERVER__
#include "farm_scene_inst.h"
#include "boss_scene_inst.h"
#include "client.family.pb.h"
#include "siege_scene_inst.h"
#include "arena_scene_inst.h"
#include "family_scene_inst.h"
#include "battle_scene_inst.h"
#include "team_scene_inst.h"
#include "cycle_scene_inst.h"	
#endif // __LIB_MINI_SERVER__

#include "scene_inst_mgr.h"

scene_inst_mgr::scene_inst_mgr(const game_map* p, const uint32 sid, struct amtsvr_context* c) : gm(p), scene_id(sid), ctx(c)
{
    alloc_scene_inst_id = 0;
}

scene_inst_mgr::~scene_inst_mgr()
{
    std::map<uint32, scene_inst*>::iterator it = scene_inst_map.begin();
    while (it != scene_inst_map.end()) {
        if (it->second != NULL) {
            delete it->second;
        }

        scene_inst_map.erase(it++);
    }

    scene_inst_map.clear();
    family_ref_scene.clear();
    team_ref_scene.clear();
    farm_ref_scene.clear();
}

int scene_inst_mgr::init()
{
    if (!gm->is_pre_create()) {
        return 0;
    }
    scene_inst* si = alloc(NULL);
    if (si == NULL) {
        return -1;
    }
    return 0;
}

scene_inst* scene_inst_mgr::get_raid_scene(const raid_scene* rs, scene_user* su)
{
    if (su == NULL) {
        return NULL;
    }

    if (gm->get_id() == 20101) {
        return get_scene_by_test(20101);
    }

    if (gm->get_sub_type() == RAID_FAMILY_SCENE) {
        uint32 family_id = 0;//su->get_attr().family();
        if (family_id == 0) {
            return NULL;
        }
        return get_scene_by_family(family_id);
    }

    if (gm->get_sub_type() == RAID_WEAPON) {
        uint32 farm_role_id = su->get_role_id();
        //printf("will enter %u scene inst\n", farm_role_id);
        if (farm_role_id == 0){
            return NULL;
        }
        return get_scene_by_farmer(farm_role_id);
    }

    uint32 min_team_num = rs->get_min_team_num();
    if (min_team_num > 0) {
        uint32 team_id = su->get_team_id();
        if (team_id == 0) {
            return NULL;
        }
        return get_scene_by_team(team_id);
    }
    return NULL;
}

scene_inst* scene_inst_mgr::create_world_boss_scene(const scene *s)
{
    scene_inst* inst = NULL;
#ifndef __LIB_MINI_SERVER__
    inst = new boss_scene_inst(++alloc_scene_inst_id, gm, s, ctx);
#endif // __LIB_MINI_SERVER__
    return inst;
}

scene_inst* scene_inst_mgr::create_raid_scene(const scene* s, scene_user* su, bool& bnew)
{
    const raid_scene* rs = static_cast<const raid_scene*>(s);
    scene_inst* inst = get_raid_scene(rs, su);
    if (inst != NULL) {
        bnew = false;
        return inst;
    }

    uint32 family_id, team_id, farm_role_id;
    if (su != NULL) {
        family_id    = su->get_family_id();
        team_id      = su->get_team_id();
        farm_role_id = su->get_role_id();
    } else {
        family_id    = 0;
        team_id      = 0;
        farm_role_id = 0;
    }

    bnew = true;
    switch (gm->get_sub_type()) {
    case RAID_PUSHFIG: {
        break;
    }
    case RAID_LAYER: {
        break;
    }
    case RAID_FAMILY_SCENE: {
#ifndef __LIB_MINI_SERVER__
        family_scene_inst* p = new family_scene_inst(++alloc_scene_inst_id, gm, rs, ctx);
        p->set_family_id(family_id);
        inst = p;
        family_ref_scene.insert(pair<uint32, uint32>(family_id, alloc_scene_inst_id));
#endif // __LIB_MINI_SERVER__
        break;
    }
    case RAID_ROLE_MATCH: {
        break;
    }
    case RAID_MASTER_CAVE: {
        break;
    }
    case RAID_ROLE_FIGHT: {
        break;
    }
    case RAID_TEAM_MONEY: {
        break;
    }
    case RAID_PROPS: {
        break;
    }
    case RAID_LEARN: {
        break;
    }
    case RAID_FAMILY_FRONTIER: {
        break;
    }
    case RAID_WEAPON: {
#ifndef __LIB_MINI_SERVER__
        inst = new farm_scene_inst(++alloc_scene_inst_id, gm, rs, ctx);
        farm_ref_scene.insert(pair<uint32, uint32>(farm_role_id, alloc_scene_inst_id));
#endif // __LIB_MINI_SERVER__
        break;
    }
    case MAP_TYPE_TEAM: {
#ifndef __LIB_MINI_SERVER__
        inst = new team_scene_inst(++alloc_scene_inst_id, gm, rs, ctx);
#endif
        break;
    }
    case MAP_TYPE_LEVELS: {
        inst = new levels_scene_inst(++alloc_scene_inst_id, gm, rs, ctx);
        break;
    }
    case RAID_STONE: {
        break;
    }
	case MAP_TYPE_ARENA: { 
#ifndef __LIB_MINI_SERVER__
		inst = new arena_scene_inst(++alloc_scene_inst_id, gm, rs, ctx);
#endif
		break;
	}
	case MAP_TYPE_CYCLE: {
#ifndef __LIB_MINI_SERVER__
		inst = new cycle_scene_inst(++alloc_scene_inst_id, gm, rs, ctx);
#endif
		break;
	}
    default: {
        inst = new raid_scene_inst(++alloc_scene_inst_id, gm, rs, ctx);
        break;
    }
    }

    if (gm->get_id() == 20101) {
        test_ref_scene.insert(pair<uint32, uint32>(gm->get_id(), alloc_scene_inst_id));
    }

    if (inst != NULL) {
        raid_scene_inst* prsi = static_cast<raid_scene_inst*>(inst);
        prsi->set_type(su->get_raid_type());
        if (su->get_raid_type() == client::enter_raid_req_type_normal) {
            prsi->set_first(su->get_raid_complete_count(s->get_scene_id()) == 0);
        }

        prsi->set_family_id(family_id);
        prsi->set_team_id(team_id);
        prsi->set_farmer_id(farm_role_id);
    }

    if (rs->get_min_team_num() > 0) {
        team_ref_scene.insert(pair<uint32, uint32>(team_id, alloc_scene_inst_id));
    }
    return inst;
}

scene_inst* scene_inst_mgr::create_battle_scene(const scene* s)
{
    scene_inst* inst = NULL;
#ifndef __LIB_MINI_SERVER__
    switch (gm->get_sub_type()) {
    case BATTLE_NORMAL: {
        inst = new battle_scene_inst(++alloc_scene_inst_id, gm, static_cast<const battle_scene*>(s), ctx);
        break;
    }

    case BATTLE_SIEGE: {
        inst = new siege_scene_inst(++alloc_scene_inst_id, gm, static_cast<const battle_scene*>(s), ctx);
        break;
    }

    case BATTLE_DOMAIN: {
        break;
    }

    default: break;
    }
#endif // __LIB_MINI_SERVER__

    return inst;
}

scene_inst* scene_inst_mgr::get_siege_scene()
{
    std::map<uint32, scene_inst*>::iterator it;
    for (it = scene_inst_map.begin(); it != scene_inst_map.end(); ++it)
    {
        return it->second;
    }
    return NULL;
}

scene_inst* scene_inst_mgr::create_siege_scene()
{
    return alloc(NULL);
}

void scene_inst_mgr::destroy_all()
{
    std::map<uint32, scene_inst*>::iterator it;
    for (it = scene_inst_map.begin(); it != scene_inst_map.end(); ++it) {
        if (it->second == NULL) {
            continue;
        }

        it->second->destory_user_all();
        delete it->second;
    }

    scene_inst_map.clear();
}

scene_inst* scene_inst_mgr::alloc(scene_user* su)
{
    const scene* s = gm->get_scene((uint32)scene_id);
    if (s == NULL) {
        return NULL;
    }

    bool bnew = true;
    scene_inst* inst = NULL;
    switch (gm->get_type()) 
	{
		case MAP_TYPE_NORMAL :
		{
			inst = new scene_inst(++alloc_scene_inst_id, gm, s, ctx);
			break;
		}
		case MAP_TYPE_BATTLE : 
		{
			inst = create_battle_scene(s);
			break;
		}
		case MAP_TYPE_WORLD_BOSS : 
		{
			inst = create_world_boss_scene(s);
			break;
		}
		default : 
		{
			inst = create_raid_scene(s, su, bnew);
			break;
		}
	}

    if (!bnew || inst == NULL) {
        return inst;
    }

    if (inst->init() != 0) {
        delete inst;
        return NULL;
    }

    scene_inst_map[alloc_scene_inst_id] = inst;

    return inst;
}

void scene_inst_mgr::destroy(scene_inst* inst)
{
    if (inst == NULL) {
        return;
    }

    if (gm->get_type() == MAP_TYPE_RAID || gm->get_type() == MAP_TYPE_TEAM) {
        raid_scene_inst* rsi = static_cast<raid_scene_inst*>(inst);

        std::map<uint32, uint32>::iterator it = team_ref_scene.find(rsi->get_team_id());
        if (it != team_ref_scene.end() && it->second == inst->get_inst_id()) {
            team_ref_scene.erase(it);
        }

        it = family_ref_scene.find(rsi->get_family_id());
        if (it != family_ref_scene.end() && it->second == inst->get_inst_id()) {
            family_ref_scene.erase(it);
        }

        it = farm_ref_scene.find(rsi->get_farmer_id());
        if (it != farm_ref_scene.end() && it->second == inst->get_inst_id()) {
            farm_ref_scene.erase(it);
        }
    }
}

void scene_inst_mgr::destroy(uint32 scene_inst_id)
{
    std::map<uint32, scene_inst*>::iterator it = scene_inst_map.find(scene_inst_id);
    if (it == scene_inst_map.end()) {
        return;
    }

    if (it->second != NULL) {
        destroy(it->second);
        delete it->second;
    }

    scene_inst_map.erase(it);
}

scene_inst* scene_inst_mgr::get(uint32 scene_inst_id)
{
    std::map<uint32, scene_inst*>::iterator it = scene_inst_map.find(scene_inst_id);
    if (it == scene_inst_map.end()) {
        return NULL;
    }

    return it->second;
}

scene_inst *scene_inst_mgr::get_boss_scene()
{
    std::map<uint32, scene_inst*>::iterator it = scene_inst_map.begin();
    for(; it != scene_inst_map.end(); ++it)
    {
        if(it->second == NULL)
        {
            continue;
        }
        return it->second;
    }
    return NULL;
}

scene_inst* scene_inst_mgr::get_battle_scene()
{
    std::map<uint32, scene_inst*>::iterator it = scene_inst_map.begin();
    for (; it != scene_inst_map.end(); ++it) {
        // 战场人数满
        if (it->second == NULL) {
            continue;
        }

        return it->second;
    }

    return NULL;
}

scene_inst *scene_inst_mgr::get_normal_scene()
{
    if (scene_inst_map.empty()) {
        return NULL;
    }

	return scene_inst_map.begin()->second;
}

scene_inst* scene_inst_mgr::get_scene(uint32 scene_inst_id, scene_user* su)
{
    if (scene_inst_id > 0) {
        return get(scene_inst_id);
    }

    scene_inst* inst = NULL;
    if (gm->get_type() == MAP_TYPE_RAID || gm->get_type() == MAP_TYPE_ARENA ||gm->get_type() == MAP_TYPE_PET || gm->get_type() == MAP_TYPE_TEAM || gm->get_type() == MAP_TYPE_LEVELS || gm->get_type() == MAP_TYPE_CYCLE || gm->get_type() == MAP_TYPE_DEVIL) { // 副本和竞技场需要新启实例
        inst = alloc(su);
    } else {
        inst = get_battle_scene();
        if (inst == NULL && gm->get_type() == MAP_TYPE_BATTLE) {
            inst = alloc(su);
        }
    }

    return inst;
}

int scene_inst_mgr::update(uint32 tick, bool is_one_sec, bool is_fiv_sec, bool is_ten_sec, bool is_thirty_sec, bool is_one_minute)
{
    std::vector<scene_inst*> si;
    std::map<uint32, scene_inst*>::iterator it = scene_inst_map.begin();
    while (it != scene_inst_map.end()) {
        scene_inst* s = it->second;
        ++it;

        s->update(tick, is_one_sec, is_fiv_sec, is_ten_sec, is_thirty_sec, is_one_minute);
        if (s->is_close()) {
            si.push_back(s);
        }
    }

    uint32 size = si.size();
    for (uint32 i = 0; i < size; ++i) {
        scene_inst* s = si[i];
        s->trans_back_all_user();
        uint32 id = s->get_inst_id();
        scene_inst_map.erase(id);
        delete s;
    }

    return 0;
}

int scene_inst_mgr::get_scene_by_scene_id(uint32 scene_id, std::vector<scene_inst*>& vsi)
{
    std::map<uint32, scene_inst*>::iterator it = scene_inst_map.begin();
    for (; it != scene_inst_map.end(); ++it) {
        if (it->second->get_scene_id() == scene_id)
            vsi.push_back(it->second);
    }

    return 0;
}

scene_user* scene_inst_mgr::get_user(uint32 role_id)
{
    uint64 role_inst_id = (uint64)role_id;
    std::map<uint32, scene_inst*>::const_iterator it;
    for (it = scene_inst_map.begin(); it != scene_inst_map.end(); ++it) {
        scene_user* su = it->second->get_scene_user(role_inst_id);
        if (su != NULL) {
            return su;
        }
    }

    return NULL;
}

scene_user* scene_inst_mgr::find_user(uint32 role_id)
{
    scene_user* psu = get_user(role_id);
    if (psu != NULL) {
        return psu;
    }

    return get_space_user(role_id);
}

scene_inst* scene_inst_mgr::get_scene_by_team(uint32 team_id)
{
    std::map<uint32, uint32>::iterator it = team_ref_scene.find(team_id);
    if (it == team_ref_scene.end()) {
        return NULL;
    }

    std::map<uint32, scene_inst*>::iterator it_scene = scene_inst_map.find(it->second);
    if (it_scene == scene_inst_map.end()) {
        team_ref_scene.erase(it);
        return NULL;
    }

    return it_scene->second;
}

scene_inst* scene_inst_mgr::get_scene_by_family(uint32 family_id)
{
    std::map<uint32, uint32>::iterator it = family_ref_scene.find(family_id);
    if (it == family_ref_scene.end()) {
        return NULL;
    }

    std::map<uint32, scene_inst*>::iterator it_scene = scene_inst_map.find(it->second);
    if (it_scene == scene_inst_map.end()) {
        family_ref_scene.erase(it);
        return NULL;
    }

    return it_scene->second;
}

scene_inst* scene_inst_mgr::get_scene_by_farmer(uint32 farm_role_id)
{
    std::map<uint32, uint32>::iterator it = farm_ref_scene.find(farm_role_id);
    if (it == farm_ref_scene.end()) {
        return NULL;
    }

    std::map<uint32, scene_inst*>::iterator it_scene = scene_inst_map.find(it->second);
    if (it_scene == scene_inst_map.end()){
        farm_ref_scene.erase(it);
        return NULL;
    }

    return it_scene->second;
}

scene_inst* scene_inst_mgr::get_scene_by_test(uint32 scene_id)
{
    std::map<uint32, uint32>::iterator it = test_ref_scene.find(scene_id);
    if (it == test_ref_scene.end()) {
        return NULL;
    }

    std::map<uint32, scene_inst*>::iterator it_scene = scene_inst_map.find(it->second);
    if (it_scene == scene_inst_map.end()) {
        test_ref_scene.erase(it);
        return NULL;
    }

    return it_scene->second;
}

int scene_inst_mgr::login_scene(scene_user* su, uint32 scene_id, uint32 scene_inst_id)
{
    scene_inst* si = get_scene(scene_inst_id, su);
    if (si == NULL) {
        return -1;
    }

    int ret = si->enter_scene(su);
    if (ret != 0) {
        return -2;
    }

    su->attach_scene(si);

    return 0;
}

int scene_inst_mgr::logout_scene(scene_user* su, scene_inst* si)
{
    if (si != NULL) {
        si->leave_scene(su);
    }

    su->detach_scene();

    return 0;
}


int scene_inst_mgr::create_randnpc(client::randnpc* rn)
{
    if (rn->scene_id() != scene_id) {
        return -1;
    }

    std::map<uint32, scene_inst*>::iterator it = scene_inst_map.begin();
    if (it == scene_inst_map.end()) {
        return -2;
    }

    scene_inst* si = it->second;
    if (si == NULL) {
        scene_inst_map.erase(it);
        return -3;
    }

    int ret = si->create_randnpc(rn);
    if (ret != 0) {
        return -4;
    }

    return 0;
}

void scene_inst_mgr::on_update_farm_info(client::update_farm_info* ufi)
{
    uint32 farm_role_id = ufi->fi().role_id();
    scene_inst* si = get_scene_by_farmer(farm_role_id);
    if (si == NULL) {
        if (!ufi->is_create()) { // 如果没有创建仙园,则不需要更新地图里的信息
            return;
        }

        const raid_scene* rs = static_cast<const raid_scene*>(gm->get_scene((uint32)scene_id));
        if (rs == NULL) {
            return;
        }

        switch (gm->get_sub_type()) {
        case RAID_WEAPON:
            {
#ifndef __LIB_MINI_SERVER__
                si = new farm_scene_inst(++alloc_scene_inst_id, gm, rs, ctx);
                farm_ref_scene.insert(std::pair<uint32, uint32>(farm_role_id, alloc_scene_inst_id));
                static_cast<raid_scene_inst*>(si)->set_farmer_id(farm_role_id);
                if (si->init() != 0) {
                    delete si;
                    return;
                }
                scene_inst_map[alloc_scene_inst_id] = si;
#endif // __LIB_MINI_SERVER__
            }
            break;
        default:
            break;
        }
    }

    if (si == NULL) {
        return;
    }

#ifndef __LIB_MINI_SERVER__
    farm_scene_inst* fsi = static_cast<farm_scene_inst*>(si);
    fsi->on_update_farm_info(ufi);
#endif // __LIB_MINI_SERVER__
}

void scene_inst_mgr::go_to_space(scene_user* su)
{
    if (su == NULL || su->get_scene() == NULL) {
        return ;
    }

    su->get_scene()->leave_scene(su);

    _space.insert(std::pair<uint32, scene_user*>(su->get_role_id(), su));
}

void scene_inst_mgr::go_to_scene(scene_user* su)
{
    if (su == NULL || su->get_scene() == NULL) {
        return ;
    }

    su->get_scene()->enter_scene(su);
    su->get_scene()->on_role_enter(su);

    _space.erase(su->get_role_id());
}

scene_user* scene_inst_mgr::get_space_user(uint32 role_id)
{
    std::map<uint32, scene_user*>::const_iterator i = _space.find(role_id);
    if (i != _space.end()) {
        return i->second;
    }

    return NULL;
}

void scene_inst_mgr::save()
{
    for (std::map<uint32, scene_inst*>::iterator i = scene_inst_map.begin(); i != scene_inst_map.end(); ++i) {
        i->second->save();
    }
}

void scene_inst_mgr::release()
{
    for (std::map<uint32, scene_inst*>::iterator i = scene_inst_map.begin(); i != scene_inst_map.end(); ++i) {
        i->second->release();
    }
}
