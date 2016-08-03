#include "config_raid.h"
#include "online_role.h"

online_role::online_role()
{
    role_id        = 0;
    scene_svr_id   = 0;
    agent_id       = 0;
    client_id      = 0;
    last_chat_time = 0;
    scene_id       = 0;
    _pcr           = NULL;
    _vip_exp       = 0;
}

online_role::online_role(uint32 rid, uint32 sid, uint32 aid, uint32 cid, const string& name, uint32 scid, uint32 vip_exp)
{
    role_id        = rid;
    scene_svr_id   = sid;
    agent_id       = aid;
    client_id      = cid;
    last_chat_time = 0;
    role_name      = name;
    scene_id       = scid;
    _pcr           = NULL;
    _vip_exp       = vip_exp;
}

online_role::online_role(const online_role& src)
{
    role_id        = src.role_id;
    role_name      = src.role_name;
    scene_svr_id   = src.scene_svr_id;
    agent_id       = src.agent_id;
    client_id      = src.client_id;
    last_chat_time = src.last_chat_time;
    scene_id       = src.scene_id;
    _vip_exp       = src._vip_exp;
    _pcr           = new center_role(src._pcr);
}

online_role::~online_role()
{
    if (_pcr != NULL) {
        delete _pcr;
        _pcr = NULL;
    }
}

void online_role::attach(center_role* c)
{
    if (c == _pcr) {
        return;
    }

    if (_pcr != NULL) {
        delete _pcr;
    }

    _pcr = c;
}

center_role* online_role::pop_center_role()
{
    center_role* tmp = _pcr;
    _pcr = NULL;
    return tmp;
}

center_role* online_role::mutable_center_role()
{
    if (_pcr == NULL) {
        _pcr = new center_role;
    }

    return _pcr;
}

bool online_role::in_main_town()const
{
    const config_raid_mgr &mgr = CONFMGR->get_config_raid_mgr();
    return mgr.is_normal(scene_id);
}

bool online_role::in_battle_scene() const
{
    const config_raid_mgr &mgr = CONFMGR->get_config_raid_mgr();
    return mgr.is_battle(scene_id);
}
