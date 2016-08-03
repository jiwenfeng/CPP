#include "farm_npc_mgr.h"
#include "game_map_mgr.h"

farm_npc::farm_npc()
{
//     tfs = NULL;
    for (int i = 0; i < client::FCSE_MATURE; ++i){
        ons[i] = NULL;
    }
}

farm_npc::~farm_npc()
{

}

// int farm_npc::attach(const mysql::tb_farm_set* p)
// {
//     if (p == NULL){
//         return -1;
//     }
// 
//     tfs = p;
//     ons[0] = GAME_MAP_MGR->get_npc(tfs->seed_id()); // 种子阶段的采集物NPC原型
//     ons[1] = GAME_MAP_MGR->get_npc(tfs->growup_id()); // 成长阶段的采集物NPC原型 
//     ons[2] = GAME_MAP_MGR->get_npc(tfs->mature_id()); // 成熟阶段的采集物NPC原型 
// 
//     return 0;
// }

const origin_npc* farm_npc::get_origin_npc(client::FARM_CROP_STATUS_E s) const
{
    if (s == client::FCSE_NULL || s > client::FCSE_MATURE){
        return NULL;
    }

    return ons[s-1];
}

////////////////////////////////////////////////////////////////////////////////

farm_npc_mgr::farm_npc_mgr()
{

}

farm_npc_mgr::~farm_npc_mgr()
{

}

int farm_npc_mgr::init()
{
//     const config_farm_mgr& cfm = CONFMGR->get_config_farm_mgr();
//     const vector<const mysql::tb_farm_set*>& tmp = cfm.get_all();
//     uint32 size = tmp.size();
//     if (size == 0){
//         return 0;
//     }
// 
//     fns.resize(size);
//     for (uint32 i = 0; i < size; ++i){
//         fns[i].attach(tmp[i]);
//     }

    return 0;
}

const farm_npc* farm_npc_mgr::get(uint32 serial) const
{
    if (serial == 0 || serial > fns.size()){
        return NULL;
    }

    return &(fns[serial-1]);
}
