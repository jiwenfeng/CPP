#include <math.h>
#include "config_map.h"
#include "config_raid.h"
#include "config_npc.h"
#include "config_props.h"
#include "config_mgr.h"
#include "base64_coder.h"
#include "origin_npc.h"
#include "game_map_mgr.h"
#include "create_npc_rule.h"
#include "raid_over.h"
#include "region_over.h"
#include "scene.h"

scene::scene(const config_map* p, const config_raid* cr)
    : _cm(p), _cr(cr)
{
    _scene_id          = p->get_id();
    _scene_width       = p->get_width();
    _scene_height      = p->get_height();
    _scene_real_width  = p->get_real_width();
    _scene_real_height = p->get_real_height();
    _scene_rect.set(0, 0, _scene_real_width, _scene_real_height);

    const std::vector<MapObject>& monsters = p->get_monsters();
    for (size_t i = 0; i < monsters.size(); ++i) {
        const origin_npc* cn = GAME_MAP_MGR->get_npc(monsters[i].id);
        if (cn != NULL) {
            origin_npc * n = NULL; 
            n = new origin_npc(*cn); 
            map_postion pos;
            pos.x = monsters[i].x;
            pos.y = monsters[i].y;
            pos.z = 0;
            n->set_born_pos(pos);
            _npc_list.push_back(n);

            if ((uint32)pos.x > _scene_real_width || (uint32)pos.y > _scene_real_height) {
                amtsvr_log("scene:%u, npc:%u, pos error x:%d y:%d\n", _scene_id, monsters[i].id, monsters[i].x, monsters[i].y);
            }
        } else {
             amtsvr_log("scene:%u, npc:%u, not found\n", _scene_id, monsters[i].id);
        }
    }

    for (int32 i = 0; i < p->unitGridWidthNum; ++i) {
        for (int32 j = 0; j < p->unitGridHeightNum; ++j) {
            uint32 region_id = p->get_region(i, j);
            if (_regions.find(region_id) == _regions.end()) {
                _regions[region_id];
                _region_rules[region_id];
            }
        }
    }

    _nav.init(get_scene_id());

    // printf("scene %d, npc_list size %d \n", scene_id, npc_list.size());
}

scene::~scene()
{
    clean();
    genterate_create_npc_rule::release();
}

int scene::init()
{
    if (get_config_raid() == NULL) {
        return 0;
    }

    if (init_create_npc_rule(get_config_raid()->get_monster_appeare_script())) {
        return -1;
    }

    if (init_create_effect_rule(get_config_raid()->get_screen_buff())) {
        return -2;
    }

    if (init_raid_over_rules() != 0) {
        return -3;
    }

    if (init_region_over_rules() != 0) {
        return -4;
    }

    // _pather.init(_scene_id);
    get_dynamic_npc(_dynamic_npcs);

    return 0;
}

void scene::clean()
{
    for (size_t i = 0; i < _npc_list.size(); ++i) {
        delete _npc_list[i];
    }

    _npc_list.clear();
}

int scene::init_create_npc_rule(const std::string& rule)
{
    if (rule.empty()) {
        return 0;
    }

    //if (_scene_id == 90202) {
    //    int breakpoint = 0;
    //    ++breakpoint;
    //}

    genterate_create_npc_rule* gcnr = genterate_create_npc_rule::get_instance();
    gcnr->genterate(rule, _create_npc_rules);

    //if (_scene_id == 90202) {
    //    printf("\n------------\n%d\n%s\n--------------\n", _scene_id, _create_npc_rules.DebugString().c_str());
    //}

    return 0;
}

int scene::init_create_effect_rule(const std::string& rule)
{
    if (rule.empty()) {
        return 0;
    }

    genterate_create_npc_rule* gcnr = genterate_create_npc_rule::get_instance();
    gcnr->genterate_effect(rule, _create_effect_rules);

    //printf("\n------------\n%d\n%s\n--------------\n", scene_id, _create_effect_rules.DebugString().c_str());
    return 0;
}

int scene::init_raid_over_rules()
{
    genterate_raid_over_rule g(&_rules_first);
    g.init(_cr->get_success_condition(), _cr->get_fail_condition());

    g.set_rules(&_rules);
    g.init(_cr->get_success_condition_2(), _cr->get_fail_condition());

    g.set_rules(&_rules_elite);
    g.init(_cr->get_success_condition_3(), _cr->get_fail_condition());
//     if (_scene_id == 20103) {
//          printf("\n-----------------------\n%d\n%s\n--------------------\n", _scene_id, _rules.DebugString().c_str());
//     }

    return 0;
}

int scene::init_region_over_rules()
{
    std::string rules_str = _cr->get_district_open_condition();
    genterate_region_over_rule g;
    while (g.init(rules_str) >= 0) {
        _region_rules[g.get_id()].CopyFrom(g.get_rules());
    }

    return 0;
}

void scene::get_dynamic_npc(std::vector<const origin_npc*>& npcs) const
{
    for (int32 i = 0; i < _create_npc_rules.rules_size(); ++i) {
        for (int32 j = 0; j < _create_npc_rules.rules(i).monster_size(); ++j) {
            const origin_npc* npc = GAME_MAP_MGR->get_npc(_create_npc_rules.rules(i).monster(j).monster_id());
            npcs.push_back(npc);
        }
    }
}

uint32 scene::single_npc_lose(const origin_npc& n, std::vector<client::props_info>& pis, uint32 type /* = 0 */) const
{
    const config_npc* cn = n.get_config_npc();
    if (cn == NULL) {
        return 0;
    }

    const std::vector<config_drop*> cds = cn->get_config_drops();
    if (cds.empty()) {
        return 0;
    }
	for(size_t i = 0; i < cds.size(); ++i)
	{
		make_props(cds[i]->get_drop_id(), pis, 5);
	}
	return pis.size();
}

uint32 scene::single_npc_exp(const origin_npc& n, uint32 lvl, uint32 type /*= 0*/) const
{
    const config_npc* pcn = n.get_config_npc();
    if (pcn == NULL) {
        return 0;
    }

    int lv     = lvl;
    int npc_lv = pcn->get_monster_level();
    int ret    = abs(lv - npc_lv);
    uint32 exp = pcn->get_experience() * (type == 1 ? pcn->get_monster_difficulty_1() / 10000 : 1);

    if (ret > 5 && ret < 10) {
        return exp / 2;
    } else if (ret <= 5) {
        return exp;
    }

    return 1;
}
