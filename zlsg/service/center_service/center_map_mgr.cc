#include "config_raid.h"
#include "game_map_mgr.h"
#include "center_map_mgr.h"

center_map_mgr::center_map_mgr()
{
}

center_map_mgr::~center_map_mgr()
{
}

int center_map_mgr::init(struct amtsvr_context* c, char* args, uint32 map_id, const game_map* gm)
{
    const std::map<uint32, scene*>& scene_list = gm->get_scene_map();
    std::map<uint32, scene*>::const_iterator it = scene_list.begin();
    for (; it != scene_list.end(); ++it) {
        uint32 scene_id = it->first;
        if (map_id == 0 || scene_id == 0) {
            amtsvr_log("game error args:%s map_id:%u scene_id:%u", args, map_id, scene_id);
            continue;
        }

        int ret = start_scene_service(args, map_id, scene_id);
        if (ret != 0) {
            return ret;
        }
    }

    return 0;
}

// args: max_session_num max_session_user map_id
int center_map_mgr::init(struct amtsvr_context* c, char* args)
{
    attach_amtsvr(c);

    return 0;

    const std::map<uint32, game_map*>& map_list = GAME_MAP_MGR->get_map_list();
    std::map<uint32, game_map*>::const_iterator it = map_list.begin();
    for (; it != map_list.end(); ++it) {
        if (it->second == NULL || it->second->get_type() == MAP_TYPE_RAID) {
            continue;
        }

        if (init(c, args, it->first, it->second) != 0) {
            return -1;
        }
    }

    return 0;
}

void center_map_mgr::delete_scene_service(uint32 scene_id)
{
    _scene_service.erase(scene_id);
}

uint32 center_map_mgr::get_scene_service_id(uint32 scene_id)
{
    std::map<uint32, uint32>::iterator it =  _scene_service.find(scene_id);
    if (it == _scene_service.end()) {
        return check_scene(scene_id);
    }

    return it->second;
}

void center_map_mgr::get_boss_scene_service_id(std::vector<uint32> &v)
{
    const std::map<uint32, game_map*>& map_list = GAME_MAP_MGR->get_map_list();
    std::map<uint32, game_map*>::const_iterator it = map_list.begin();
    for (; it != map_list.end(); ++it) {
        if(it->second->get_type() == MAP_TYPE_WORLD_BOSS) {
            v.push_back(get_scene_service_id(it->first));
        }
    }
}

int center_map_mgr::start_scene_service(char* args, uint32 map_id, uint32 scene_id)
{
    char tmp[256] = {0};
    snprintf(tmp, sizeof(tmp), "game %s %u %u", args, map_id, scene_id);
    const char* service_addr = amtsvr_command(ctx, "LAUNCH", tmp);
    if (service_addr == NULL) {
        amtsvr_syslog(ctx, "center_map_mgr : launch scene_service_addr failed");
        return -1;
    }

    uint32 service_id = strtoul(service_addr + 1 , NULL, 16);
    if (service_id == 0) {
        amtsvr_syslog(ctx, "center_map_mgr : launch scene service invalid %s", service_addr);
        return -2;
    }

    _scene_service.insert(std::pair<uint32, uint32>(scene_id, service_id));

    return 0;
}

int center_map_mgr::check_scene(uint32 scene_id)
{
    const std::map<uint32, game_map*>& m = GAME_MAP_MGR->get_map_list();
    const std::map<uint32, game_map*>::const_iterator it = m.find(scene_id);
    char args[] = "1 3000";
    if (it == m.end() || init(ctx, args, it->first, it->second) != 0) {
        return 0;
    }

    return get_scene_service_id(scene_id);
}
