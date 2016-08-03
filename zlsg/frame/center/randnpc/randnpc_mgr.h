#ifndef __RAND_NPC_MGR_H__
#define __RAND_NPC_MGR_H__

#include "pb_mgr.h"

class randnpc_mgr
{
public:
    randnpc_mgr();

    virtual ~randnpc_mgr();

    void init();

    bool is_need_fresh(uint32 now, uint32 hour);

    client::randnpc* get_pos();

    client::randnpc_set& get_rns(){return rns;}

private:
    void add_one(client::randnpc& rn, uint32 map_id, uint32 scene_id, int x, int y);
    void add_path(client::randnpc& rn, int x, int y);
    void init_tianqicheng();
    void init_ganlincun();
    void init_yunwuchangshan();
    void init_lyflg();
    void init_xms();
    void init_mlgd();
private:
    uint32 last_fresh_time;
    client::randnpc_set rns;
};

#endif // !__RAND_NPC_H__
