#include "config_map.h"
#include "config_npc.h"
#include "config_mgr.h"
#include "game_map_mgr.h"

#ifndef WIN32
#include <unistd.h>
#endif

game_map_mgr::game_map_mgr()
{
    _lock = 0;
    _bok  = 0;
}

game_map_mgr::~game_map_mgr()
{
    for (std::map<uint32, game_map*>::iterator i = _gms.begin(); i != _gms.end(); ++i) {
        delete i->second;
    }

    _gms.clear();

    for (std::map<uint32, origin_npc*>::iterator i = _npcs.begin(); i != _npcs.end(); ++i) {
        delete i->second;
    }

    _npcs.clear();
}

bool game_map_mgr::ready()
{
    bool ret;
    __sync_lock_test_and_set(&_lock,1);{}
    ret = (_bok == 1);
    __sync_lock_release(&_lock);
    return ret;
}

int game_map_mgr::init()
{
//     EPAPSED_TIME;

//     if (init_npc() != 0) {
//         return -1;
//     }

    if (init_map() != 0) {
        return -2;
    }

    if (_fnm.init() != 0) {
        return -3;
    }

    __sync_lock_test_and_set(&_lock,1);{}
    _bok = 1;
    //printf("set bok = 1-----------------------------------------------\n");
    __sync_lock_release(&_lock);

    return 0;

}

const game_map* game_map_mgr::get_map(uint32 map_id) const
{
    std::map<uint32, game_map*>::const_iterator it = _gms.find(map_id);
    if (it == _gms.end()) {
        return NULL;
    }

    return it->second;
}

const origin_npc* game_map_mgr::get_npc(uint32 npc_id)
{
    std::map<uint32, origin_npc*>::const_iterator it = _npcs.find(npc_id);
    if (it == _npcs.end()) {
        return init_npc(npc_id);
    }

    return it->second;
}

int game_map_mgr::init_map()
{
    const std::map<uint32, config_map*>& mm = CONFMGR->get_config_map_mgr().get_config_map();
    std::map<uint32, config_map*>::const_iterator i = mm.begin();
    for (; i != mm.end(); ++i) {
        std::map<uint32, game_map*>::iterator m = _gms.find(i->second->id);
        if (m != _gms.end() && m->second != NULL) {
            continue;
        }

        game_map* ngm = new game_map(i->second);
        _gms.insert(std::pair<uint32, game_map*>(i->second->id, ngm));
        ngm->init();
    }

    return 0;
}

int game_map_mgr::init_map(uint32 map_id)
{
    const config_map* mm = CONFMGR->get_config_map(map_id);
    std::map<uint32, game_map*>::iterator m = _gms.find(mm->id);
    if (m != _gms.end() && m->second != NULL) {
        delete m->second;
        m->second = NULL;
    }

    game_map* ngm = new game_map(mm);
    _gms.insert(std::pair<uint32, game_map*>(mm->id, ngm));
    ngm->init();

    return 0;
}

origin_npc* game_map_mgr::generate_origin_npc(const config_npc *cn, const client::skills &sk)
{
	origin_npc *o = new origin_npc(cn);
	o->init();
	config_skill_mgr* pcsm = MUTABLE_CONFIG_MGR(config_skill_mgr);
    if (pcsm == NULL) {
        return NULL;
    }
	for(int i = 0; i < sk.sk_size(); ++i)
	{
		const config_skill* pcs = pcsm->get_config_skill(sk.sk(i).id());
		if (pcs == NULL) 
		{
			continue;
		}
		o->mutable_skill()->add_skill(skill(sk.sk(i), *pcs));
	}
	return o;
}

int game_map_mgr::init_npc()
{
    const config_npc_mgr& p = CONFMGR->get_config_npc_mgr();
    const std::map<uint32, config_npc*> n = p.get_all_npc();
    int count = 0;
    for (std::map<uint32, config_npc*>::const_iterator i = n.begin(); i != n.end(); ++i) {
        std::map<uint32, origin_npc*>::iterator npc = _npcs.find(i->second->get_npc_id());
        if (npc != _npcs.end() && npc->second != NULL) {
            delete npc->second;
            npc->second = NULL;
        }

        origin_npc* n = new origin_npc(i->second);
        _npcs.insert(std::pair<uint32, origin_npc*>(i->second->get_npc_id(), n));
        n->init();
        ++count;
    }

    //printf("npc all count %d\n", count);

    return 0;
}

const origin_npc* game_map_mgr::init_npc(uint32 npc_id)
{
    int ret = 1;
    do {
        ret = __sync_lock_test_and_set(&_lock, 1);
        if (ret != 0) {
            usleep(1);
        }
    } while (ret != 0);

    const config_npc_mgr& cnm = CONFMGR->get_config_npc_mgr();
    const config_npc* pcn = cnm.get_config_npc(npc_id);
    if (pcn == NULL) {
        amtsvr_log("not found npc:%d\n", npc_id);
        __sync_lock_release(&_lock);
        return NULL;
    }

    origin_npc* pon = new origin_npc(pcn);
    _npcs.insert(std::pair<uint32, origin_npc*>(npc_id, pon));
    pon->init();

    __sync_lock_release(&_lock);
    return pon;
}
