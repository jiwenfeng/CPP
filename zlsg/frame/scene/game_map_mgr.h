// 地图管理类，负责管理整个地图配置、NPC配置、掉落等相关的数据 
#ifndef __GAME_MAP_MGR_H__
#define __GAME_MAP_MGR_H__

#include "singleton.h"
#include "game_map.h"
#include "origin_npc.h"
#include "farm_npc_mgr.h"

class game_map_mgr
{
public:
    game_map_mgr();
    virtual ~game_map_mgr();

public:
    int init();
    bool ready();

public:
    const game_map* get_map(uint32 map_id) const;
    const origin_npc* get_npc(uint32 npc_id);
	origin_npc *generate_origin_npc(const config_npc *cn, const client::skills &sk);
    inline const std::map<uint32, game_map*>& get_map_list() const { return _gms; }
    inline const farm_npc_mgr& get_farm_npc_mgr() const { return _fnm; }

public:
    int init_map();
    int init_map(uint32 map_id);
    int init_npc();


private:
    const origin_npc* init_npc(uint32 npc_id);

private:
    std::map<uint32, game_map*>   _gms;       // map_id-->game map
    std::map<uint32, origin_npc*> _npcs;      // npc_id-->origin_npc 

    farm_npc_mgr                  _fnm;       // 农场采集物的地图配置管理器

    int                           _lock;
    int                           _bok;
};

extern game_map_mgr* GAME_MAP_MGR;

#endif

