#include "scene.h"
#include "config_map.h"
#include "config_raid.h"
#include "raid_scene.h"

#ifndef __LIB_MINI_SERVER__
#include "battle_scene.h"
#include "boss_scene.h"
#endif // __LIB_MINI_SERVER__

#include "game_map.h"

game_map::game_map(const config_map* p)
{
    if (p == NULL) {
        amtsvr_log("game_map error\n");
        return ;
    }

    _mc       = p;
    _cr       = CONFMGR->get_config_raid(p->id);
    _map_id   = p->id;
    _map_type = get_map_type(_map_id);
    _sub_type = get_map_sub_type(_map_id);
    _min_lvl  = 0;   // p->min_lv();
    _max_lvl  = 100; // p->max_lv();

    scene* s = NULL;
    switch (_map_type) {
    case MAP_TYPE_NORMAL	 :  s = new scene(p, _cr);			break;
    case MAP_TYPE_RAID		 :  s = new raid_scene(p, _cr);		break;
#ifndef __LIB_MINI_SERVER__
    case MAP_TYPE_BATTLE	 :  s = new battle_scene(p, _cr);	break;
    case MAP_TYPE_WORLD_BOSS :  s = new boss_scene(p, _cr);		break;
	case MAP_TYPE_ARENA		 :	s = new raid_scene(p, _cr);		break;
	case MAP_TYPE_CYCLE		 :  s = new raid_scene(p, _cr);		break;
#endif // __LIB_MINI_SERVER__
    case MAP_TYPE_PET		 :  s = new raid_scene(p, _cr);		break;
	case MAP_TYPE_TEAM		 :	s = new raid_scene(p, _cr);		break;
	case MAP_TYPE_LEVELS	 :	s = new raid_scene(p, _cr);		break;
	case MAP_TYPE_DEVIL		 :	s = new raid_scene(p, _cr);		break;
    default: break;
    }

    if (s != NULL) {
        _scenes.insert(pair<uint32, scene*>(p->id, s));
    } else {
        amtsvr_log("map:%u type:%u error\n", _map_id, _map_type);
    }

    _scene_num = _scenes.size();
}

game_map::~game_map()
{
    std::map<uint32, scene*>::iterator it = _scenes.begin();
    while (it != _scenes.end()) {
        if (it->second != NULL) {
            delete it->second;
            it->second = NULL;
        }
        ++it;
    }
    _scenes.clear();
}

int game_map::init()
{
    std::map<uint32, scene*>::iterator it = _scenes.begin();
    while (it != _scenes.end()) {
        int ret = it->second->init();
        if (ret != 0) {
            delete it->second;
            _scenes.erase(it++);
        } else {
            ++it;
        }
    }

    return 0;
}

const scene* game_map::get_scene(uint32 scene_id) const
{
    std::map<uint32, scene*>::const_iterator it = _scenes.find(scene_id);
    if (it == _scenes.end()) {
        return NULL;
    }

    return it->second;
}

uint32 game_map::get_map_type(uint32 id)
{
    const config_raid* cr = get_config_raid();
    if (cr != NULL) {
        return cr->get_map_type();
    }

    return MAP_TYPE_NORMAL;
}

uint32 game_map::get_map_sub_type(uint32 id)
{
    const config_raid* cr = get_config_raid();
    if (cr != NULL) {
        return cr->get_map_type();
    }

    return 0;
}

bool game_map::is_pre_create() const
{
    return _map_type == MAP_TYPE_NORMAL ||
        _map_type == MAP_TYPE_WORLD_BOSS;
}
