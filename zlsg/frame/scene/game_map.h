#ifndef __GAME_MAP_H__
#define __GAME_MAP_H__

#include "scene_define.h"

class scene;

// 游戏地图类，一个地图中包含多个场景，是一系列同一类型的场景配置集合
class game_map  
{
public:
    game_map(const config_map* p);
    virtual ~game_map();

public:
    int init();

public:
    inline const config_map* get_config_map() const { return _mc; }
    inline const config_raid* get_config_raid() const { return _cr; }

    const scene* get_scene(uint32 scene_id) const;

    inline uint32 get_id()       const { return _map_id; }
    inline uint32 get_type()     const { return _map_type; }
    inline uint32 get_sub_type() const { return _sub_type; }
    const std::map<uint32, scene*>& get_scene_map() const { return _scenes; }

    bool is_pre_create() const;

    inline uint32 get_min_lvl() const { return _min_lvl; }

private:
    uint32 get_map_type(uint32 map_id);
    uint32 get_map_sub_type(uint32 map_id);

private:
    const config_map*        _mc;
    const config_raid*       _cr;

    uint32                   _map_id;      // 地图ID
    uint32                   _map_type;    // 地图类型
    uint8                    _sub_type;    // 地图的分类型
    uint8                    _min_lvl;     // 最小等级限制
    uint8                    _max_lvl;     // 最大等级限制

    uint16                   _scene_num;   // 包含的场景ID个数
    std::map<uint32, scene*> _scenes;      // 该地图包含的场景, scene_id--->scene
};

#endif
