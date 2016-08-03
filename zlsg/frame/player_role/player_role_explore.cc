#include "random.h"
#include "player_role_explore.h"

player_role_explore::player_role_explore(mysql::tb_role_explore* p/* = NULL*/)
    : _explore(p)
{
    writer_type = WOTE_NULL;
}

player_role_explore::~player_role_explore()
{
    if (_explore != NULL) {
        delete _explore;
        _explore = NULL;
    }

    multimap<uint32, player_role_explore_detail*>::iterator i = _detail.begin();
    for (; i != _detail.end(); ++i) {
        delete i->second;
    }
    _detail.clear();
}

int player_role_explore::init(uint32 role_id, mysql::tb_role_explore* tre)
{
    if (_explore != NULL){
        return (_explore == tre ? 0 : -1);
    }

    if (tre != NULL) {
        _explore = tre;
    } else {
        if (_explore != NULL) {
            return 0;
        }
        _explore = new mysql::tb_role_explore;
        _explore->set_role_id(role_id);
        _explore->set_num(0);
        _explore->set_money(0);
        _explore->set_success_ratio(0);
        _explore->set_failure_money(0);
        writer_type = WOTE_INSERT;
    }

    return 0;
}

int player_role_explore::attach_role_explore(int pb_count, cmq_svr_pb_ctx* pb_ctx)
{
    for (int i = 0; i < pb_count; ++i) {
        if (pb_ctx[i].pb_msg == NULL) {
            continue;
        }

        mysql::tb_role_explore* p = static_cast<mysql::tb_role_explore*>(pb_ctx[i].pb_msg);
        if (init(p->role_id(), p) != 0) {
            delete pb_ctx[i].pb_msg;
        }
    }
    return 0;
}

int player_role_explore::attach_role_explore_detail(int pb_count, cmq_svr_pb_ctx* pb_ctx)
{
    for (int i = 0; i < pb_count; ++i) {
        if (pb_ctx[i].pb_msg == NULL) {
            continue;
        }

        mysql::tb_role_explore_detail* p = static_cast<mysql::tb_role_explore_detail*>(pb_ctx[i].pb_msg);
        if (init_detail(p) != 0) {
            delete pb_ctx[i].pb_msg;
        }
    }
    return 0;
}

int player_role_explore::init_detail(mysql::tb_role_explore_detail* tred)
{
    if (tred == NULL) {
        return -1;
    }

    player_role_explore_detail* pred = new player_role_explore_detail(tred);
    _detail.insert(pair<uint32, player_role_explore_detail*>(pred->get()->time(), pred));
    
    return 0;
}

int player_role_explore::save(amtsvr_context* ctx)
{
    if (_explore == NULL) {
        return -1;
    }

    mysql::tb_role_explore* tre = new mysql::tb_role_explore(*_explore);
    CACHE_KEY k(tre->role_id(), 0);
    write(ctx, k, tre);
    writer_type = WOTE_UPDATE;
    
    return 0;
}

int player_role_explore::save_detail(amtsvr_context* ctx)
{
    multimap<uint32, player_role_explore_detail*>::iterator i = _detail.begin();
    for (; i != _detail.end(); ++i) {
        i->second->save(ctx);
    }
    
    return 0;
}

