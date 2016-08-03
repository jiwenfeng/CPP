#include "player_role_raid_mgr.h"

player_role_raid_mgr::player_role_raid_mgr()
{
}

player_role_raid_mgr::~player_role_raid_mgr()
{
    map<uint32, player_role_raid>::iterator it = _raids.begin();
    for (; it != _raids.end(); ++it) {
        it->second.release();
    }
}

int player_role_raid_mgr::attach_role_raid(int pb_count, cmq_svr_pb_ctx* pb_ctx)
{
    for (int i = 0; i < pb_count; ++i) {
        if (pb_ctx[i].pb_msg == NULL) {
            continue;
        }
        
        if (push(static_cast<mysql::tb_role_raid*>(pb_ctx[i].pb_msg)) != 0 && pb_ctx[i].pb_msg != NULL) {
            delete pb_ctx[i].pb_msg;
            pb_ctx[i].pb_msg = NULL;
        }
    }
    
    return 0;
}

int player_role_raid_mgr::push(mysql::tb_role_raid* p)
{
    if (p == NULL) {
        return -1;
    }

    player_role_raid& raid = _raids[p->raid_id()];
    raid.init(p);

    return 0;
}

int player_role_raid_mgr::push(uint32 role_id, uint32 raid_id, uint32 star_num, uint32 complete_tick, uint32 complete_time, uint32 type /* = 0 */, uint32 count /* = 0 */)
{
    map<uint32, player_role_raid>::iterator it = _raids.find(raid_id);
    if (it == _raids.end()) {
        player_role_raid& prp = _raids[raid_id];
        if (prp.create(role_id, raid_id, star_num, complete_tick, complete_time, type, count) != 0) {
            _raids.erase(raid_id);
            return -1;
        } 
    } else {
        it->second.set(star_num, complete_tick, complete_time, type);
    }

    return 0;
}

int player_role_raid_mgr::modify_raid_star(uint32 raid_id, uint32 star_num)
{
	map<uint32, player_role_raid>::iterator it = _raids.find(raid_id);
	if(it == _raids.end())
	{
		return -1;
	}
	it->second.modify_raid_star(star_num);
	return 0;
}

int player_role_raid_mgr::set_auto(uint32 raid_id, uint32 type)
{
    map<uint32, player_role_raid>::iterator it = _raids.find(raid_id);
    if (it == _raids.end()) {
        return -1;
    }

    return it->second.set_auto(type);
}

void player_role_raid_mgr::save(amtsvr_context* ctx)
{
    map<uint32, player_role_raid>::iterator it = _raids.begin();
    for (; it != _raids.end(); ++it) {
        it->second.save(ctx);
    }
}

player_role_raid* player_role_raid_mgr::mutable_raid(uint32 raid_id)
{
    map<uint32, player_role_raid>::iterator it = _raids.find(raid_id);
    if (it == _raids.end()) {
        return NULL;
    }

    return &(it->second);
}

void player_role_raid_mgr::fill_raid_info_msg(client::raid_info_msg& rim)
{
    std::map<uint32, player_role_raid>::iterator it;
    for (it = _raids.begin(); it != _raids.end(); ++it) {
        it->second.fill_info(*rim.add_ri());
    }
}

void player_role_raid_mgr::fill_raid_info_msg(client::raid_info_msg& rim, uint32 raid_id)
{
    std::map<uint32, player_role_raid>::iterator it = _raids.find(raid_id);
    if (it != _raids.end()) {
        it->second.fill_info(*rim.add_ri());
    }
}
