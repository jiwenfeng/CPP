#ifndef __CENTER_MAP_MGR__
#define __CENTER_MAP_MGR__

#include "center_mgr.h"

class game_map;

class center_map_mgr : public center_mgr
{
public:
    center_map_mgr();
    virtual ~center_map_mgr();

public:
    virtual int init(struct amtsvr_context* c, char* args);

    int  start_scene_service(char* args, uint32 map_id, uint32 scene_id);
    void delete_scene_service(uint32 scene_id);

public:
    uint32 get_scene_service_id(uint32 scene_id);
    void   get_boss_scene_service_id(std::vector<uint32> &v);

    inline const std::map<uint32, uint32>& get_scene_service() const { return _scene_service; }

private:
    int init(struct amtsvr_context* c, char* args, uint32 map_id, const game_map* gm);

    int check_scene(uint32 scene_id);

private:
    std::map<uint32, uint32> _scene_service;  // scene_id --->map_service id, 场景到场景服务的id
};

#endif
