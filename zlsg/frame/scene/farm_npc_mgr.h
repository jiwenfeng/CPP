#ifndef __FARM_NPC_MGR_H__
#define __FARM_NPC_MGR_H__

#include "origin_npc.h"

class farm_npc
{
public:
    farm_npc();

    virtual ~farm_npc();

//     int attach(const mysql::tb_farm_set* p);
// 
//     inline const mysql::tb_farm_set* get_tfs() const {return tfs;}

    const origin_npc* get_origin_npc(client::FARM_CROP_STATUS_E s) const;

private:
//     const mysql::tb_farm_set* tfs; 
    const origin_npc*         ons[client::FCSE_MATURE];
};

class farm_npc_mgr
{
public:
    farm_npc_mgr();

    virtual ~farm_npc_mgr();

    int init();

    const farm_npc* get(uint32 serial) const;

private:
    vector<farm_npc> fns;
};

#endif
