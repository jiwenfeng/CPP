// 场景类，主要是该场景包含的信息等配置内容，要靠实例化场景实现游戏内场景逻辑
#ifndef __SCENE_H__
#define __SCENE_H__

#include "scene_define.h"
#include "rect.h"
#include "navmesh.h"
#include "postion.h"
#include "server.ai.pb.h"

#define GRID_WIDTH_SIZE  48
#define GRID_HEIGHT_SIZE 48

class origin_npc;

class scene
{
public:
    scene(const config_map* p, const config_raid* cr);
    virtual ~scene();

public:
    virtual int init();

    void clean();

public:
    inline const std::vector<const origin_npc*>& get_npc_list() const {return _npc_list;}
    inline const std::vector<const origin_npc*>& get_dynamic_npcs() const { return _dynamic_npcs; }

    inline const rect& get_scene_ret() const { return _scene_rect; }

    inline const bool is_out_scene_range(const map_postion& p) const {
        return (p.x > (int)(_scene_real_width) || p.y > (int)(_scene_real_height));
    }

    inline const uint32     get_scene_id() const {return _scene_id;}
    inline const navmesh_s& get_nav() const {return _nav;}
    inline const uint32     get_scene_width() const { return _scene_width; }
    inline const uint32     get_scene_height() const { return _scene_height; }
    inline const uint32     get_scene_real_width () const { return _scene_real_width; }
    inline const uint32     get_scene_real_height() const { return _scene_real_height; }

    inline const config_map*                                get_config_map()            const { return _cm                 ; }
    inline const config_raid*                               get_config_raid()           const { return _cr                 ; }
    inline const server::create_npc_rules&                  get_create_npc_rules()      const { return _create_npc_rules   ; }
    inline const server::create_effect_rules&               get_create_effect_rules()   const { return _create_effect_rules; }
    inline const server::raid_over_rules&                   get_raid_over_rules()       const { return _rules              ; }
    inline const server::raid_over_rules&                   get_raid_over_rules_first() const { return _rules_first        ; }
    inline const server::raid_over_rules&                   get_raid_over_rules_elite() const { return _rules_elite        ; }
    inline const std::map<uint32, server::raid_over_rules>& get_region_rules()          const { return _region_rules       ; }
    inline const std::map<uint32, region_data>&             get_regions()               const { return _regions            ; }

public:
    uint32 single_npc_lose(const origin_npc& n, std::vector<client::props_info>& pis, uint32 type = 0) const;
    uint32 single_npc_exp(const origin_npc& n, uint32 lvl, uint32 type = 0) const;

protected:
    int init_create_npc_rule(const std::string& rule);
    int init_create_effect_rule(const std::string& rule);
    int init_raid_over_rules();
    int init_region_over_rules();

    void get_dynamic_npc(std::vector<const origin_npc*>& npcs) const;

protected:
    uint32                                    _scene_id;
    uint32                                    _scene_width;
    uint32                                    _scene_height;
    uint32                                    _scene_real_width;
    uint32                                    _scene_real_height;

    std::vector<const origin_npc*>            _npc_list;
    std::vector<const origin_npc*>            _dynamic_npcs;
    rect                                      _scene_rect;

    const config_map*                         _cm;
    const config_raid*                        _cr;
    server::create_npc_rules                  _create_npc_rules;
    server::create_effect_rules               _create_effect_rules;
    server::raid_over_rules                   _rules;
    server::raid_over_rules                   _rules_first;
    server::raid_over_rules                   _rules_elite;
    std::map<uint32, server::raid_over_rules> _region_rules;
    std::map<uint32, region_data>             _regions;
    navmesh_s                                 _nav;
};

#endif
