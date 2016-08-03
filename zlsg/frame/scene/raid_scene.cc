#include "origin_npc.h"
#include "game_map_mgr.h"
#include "config_raid.h"
#include "raid_scene.h"

raid_scene::raid_scene(const config_map* p, const config_raid* cr) : scene(p, cr)
{
}

raid_scene::~raid_scene()
{
}

int raid_scene::init()
{
    if (_cr == NULL) {
        return -1;
    }

    if(scene::init() != 0) {
        return -2;
    }

    return 0;
}

uint32 raid_scene::get_type()            const { return _cr->get_map_type()     ; }
uint32 raid_scene::get_raid_id()         const { return _cr->get_map_id()       ; }
uint32 raid_scene::get_min_team_num()    const { return 1                       ; }
int    raid_scene::get_raid_type()       const { return _cr->get_map_type()     ; }
uint32 raid_scene::get_standard_time()   const { return _cr->get_standard_time(); }
uint32 raid_scene::get_battle_ui_id()    const { return _cr->get_battle_ui_id() ; }

const uint32 raid_scene::get_evaluation(uint32 tenms) const
{
    if (get_config_raid() == NULL) {
        return 1;
    }

    uint32 standard_time = get_config_raid()->get_standard_time() / 10;
    for (size_t i = 0; i < get_config_raid()->get_evaluation().size(); ++i) {
        if (tenms <= standard_time * (get_config_raid()->get_evaluation().at(i) / 10.0f)) {
            return i + 1;
        }
    }

    return min(3, (int)(get_config_raid()->get_evaluation().size() + 1));
}

uint32 raid_scene::cal_npc_lose(const std::vector<const origin_npc*>& npcs, std::vector<client::props_info>& pis) const
{
    uint32 size = npcs.size();
    uint32 ret  = 0;
    for (uint32 i = 0; i < size; ++i) {
        ret += single_npc_lose(*npcs[i], pis);
    }

    return ret;
}

uint32 raid_scene::get_falling_objects(std::vector<client::props_info>& pis) const
{
    uint32 ret = 0;
    if (!_npc_list.empty()) {
        ret = cal_npc_lose(_npc_list, pis);
    }

    std::vector<const origin_npc*> npcs;
    get_dynamic_npc(npcs);
    if (!npcs.empty()) {
        ret += cal_npc_lose(npcs, pis);
    }

    return ret;
}
