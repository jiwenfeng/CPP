#include "player_role_order_mgr.h"

player_role_order_mgr::player_role_order_mgr()
{
}

player_role_order_mgr::~player_role_order_mgr()
{
    map<uint32, player_role_order>::iterator i = _order.begin();
    for (; i != _order.end(); ++i) {
        i->second.release();
    }
    _order.clear();

    i = _npc.begin();
    for (; i != _npc.end(); ++i) {
        i->second.release();
    }
    _npc.clear();
}

int player_role_order_mgr::init()
{
    return 0;
}

int player_role_order_mgr::attach_role_order(int pb_count, cmq_svr_pb_ctx* pb_ctx)
{
    for (int i = 0; i < pb_count; ++i) {
        if (pb_ctx[0].pb_msg == NULL) {
            continue;
        }
        if (push(static_cast<mysql::tb_role_order*>(pb_ctx[i].pb_msg)) != 0) {
            delete pb_ctx[i].pb_msg;
        }
    }

    return 0;
}

int player_role_order_mgr::push(mysql::tb_role_order* tro)
{
    if (tro == NULL) {
        return -1;
    }

    if (tro->type() == 0) {
        map<uint32, player_role_order>::iterator it = _order.find(tro->store_id());
        if (it != _order.end()){
            return -2;
        }

        player_role_order& pro = _order[tro->store_id()];
        pro.attach(tro);
    } else {
        map<uint32, player_role_order>::iterator it = _npc.find(tro->store_id());
        if (it != _npc.end()){
            return -2;
        }

        player_role_order& pro = _npc[tro->store_id()];
        pro.attach(tro);
    }

    return 0;
}

int player_role_order_mgr::save(amtsvr_context* ctx)
{
    map<uint32, player_role_order>::iterator i = _order.begin();
    for (; i != _order.end(); ++i) {
        i->second.save(ctx);
    }

    i = _npc.begin();
    for (; i != _npc.end(); ++i) {
        i->second.save(ctx);
    }
    
    return 0;
}

int player_role_order_mgr::add(uint32 role_id, uint32 store_id, uint32 count, uint32 type)
{
    if (type == RED_STORE) {
        map<uint32, player_role_order>::iterator i = _order.find(store_id);
        if (i == _order.end()) {
            player_role_order& pro = _order[store_id];
            pro.create(role_id, store_id, count, type);
        } else {
            i->second.add(count);
        }
    } else if (type == NPC_STORE) {
        map<uint32, player_role_order>::iterator i = _npc.find(store_id);
        if (i == _npc.end()) {
            player_role_order& pro = _npc[store_id];
            pro.create(role_id, store_id, count, type);
        } else {
            i->second.add(count);
        }
    }
    
    return 0;
}

uint32 player_role_order_mgr::get(uint32 store_id, uint32 type)
{
    if (type == RED_STORE) {
        map<uint32, player_role_order>::iterator i = _order.find(store_id);
        if (i != _order.end()) {
            return i->second.get();
        }
    } else if (type == NPC_STORE) {
        map<uint32, player_role_order>::iterator i = _npc.find(store_id);
        if (i != _npc.end()) {
            return i->second.get();
        }
    }

    return 0;
}

int player_role_order_mgr::fill(client::order_tracking_rsp& rsp)
{
    map<uint32, player_role_order>::iterator i = _order.begin();
    for (; i != _order.end(); ++i) {
        client::order_tracking_info* oti = rsp.add_oti();
        i->second.fill(oti);
    }

    i = _npc.begin();
    for (; i != _npc.end(); ++i) {
        client::order_tracking_info* oti = rsp.add_oti();
        i->second.fill(oti);
    }
    
    return 0;
}

void time_out()
{
}
