// ��ͼ�����࣬�������������ͼ���á�NPC���á��������ص����� 
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

    farm_npc_mgr                  _fnm;       // ũ���ɼ���ĵ�ͼ���ù�����

    int                           _lock;
    int                           _bok;
};

extern game_map_mgr* GAME_MAP_MGR;

#endif

