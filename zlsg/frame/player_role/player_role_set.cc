#include "player_role_set.h"

player_role_set::player_role_set(mysql::tb_role_set* s) : _db_role_set(s)
{
    if (_db_role_set != NULL) {
        parse(_db_role_set->shortcut());
    }

    writer_type = WOTE_UPDATE;
}

player_role_set::~player_role_set()
{
    if (_db_role_set != NULL) {
        delete  _db_role_set;
        _db_role_set = NULL;
    }
}

int player_role_set::init(mysql::tb_role_set* s)
{
    if (s == _db_role_set || s == NULL) {
        return 0;
    }

    if (_db_role_set != NULL) {
        delete _db_role_set;
    }
    
    _db_role_set = s;
    parse(_db_role_set->shortcut());
    
    return 0;
}

int player_role_set::save(amtsvr_context* ctx)
{
    if (_db_role_set == NULL) {
        return -1;
    }
    
    mysql::tb_role_set* trs = new mysql::tb_role_set(*_db_role_set);
    CACHE_KEY k(trs->role_id(), 0);
    string out;
    serialize(out);
    trs->set_shortcut(out);
    write(ctx, k, trs);
    writer_type = WOTE_UPDATE;

    return 0;
}

void player_role_set::fill_shortcut(client::shortcut_rsp& rsp)
{
    for (int32 i = 0; i < _role_set_info.si_size(); ++i) {
        const client::shortcut_info& csi = _role_set_info.si(i);
        if (csi.val() != 0) {
            client::shortcut_info* psi = rsp.add_si();
            psi->CopyFrom(csi);
        }
    }
}
    
void player_role_set::set_shortcut(client::shortcut_set_req* req, client::shortcut_set_rsp& rsp)
{
    if (_db_role_set == NULL) {
        writer_type = WOTE_INSERT;
        _db_role_set = new  mysql::tb_role_set;
        _db_role_set->set_role_id(req->svr_head().role_id());
        parse(_db_role_set->shortcut());
    } 

    //printf("set_shortcut\n");
    int32 size = req->si_size();
    for (int32 i = 0; i < size; ++i) {
        const client::shortcut_info& si = req->si(i);
        client::shortcut_info* rsp_si = rsp.add_si();
        rsp_si->CopyFrom(si);
        if (si.id() > SHORTCUT_MAX_GRID || si.type() > SHORTCUT_MAX_TYPE) {
            rsp.add_ret(1);
            continue ;
        }

        //printf("id = %d, type = %d , val = %d\n", si.id(), si.type(), si.val());
        rsp.add_ret(0);
        if (si.id() >= (uint32)_role_set_info.si_size()) {
            for (uint32 j = _role_set_info.si_size(); j <= si.id(); ++j) {
                _role_set_info.add_si();
            }
        }
        
        client::shortcut_info* psi = _role_set_info.mutable_si(si.id());
        psi->CopyFrom(si);
    }
    //printf("\n");
}

void player_role_set::fill_handup(client::handup_rsp& rsp)
{
    client::handup_info* hi = rsp.mutable_hi();
    if (_role_set_info.has_hi()) {
        hi->CopyFrom(_role_set_info.hi());
    } 
}

void player_role_set::set_handup(client::handup_set_req* req, client::handup_set_rsp& rsp)
{
    const client::handup_info& hi = req->hi();
    client::handup_info* p  = _role_set_info.mutable_hi();
    if (hi.has_relive()) {
        p->set_relive(hi.relive());
    }

    if (hi.has_pick_equip()) {
        p->set_pick_equip(hi.pick_equip());
    }

    if (hi.has_pick_prop()) {
        p->set_pick_equip(hi.pick_prop() > 0);
    }

    if (hi.has_back_base()) {
        p->set_back_base(hi.back_base());
    }
    
    if (hi.skills_size() <= 12) {
        p->clear_skills();
        p->mutable_skills()->CopyFrom(hi.skills());
    }

    rsp.mutable_hi()->CopyFrom(_role_set_info.hi());
}

int player_role_set::attach_role_set(int pb_count, cmq_svr_pb_ctx* pb_ctx)
{
    for (int i = 0; i < pb_count; ++i) {
        if (pb_ctx[i].pb_msg == NULL) {
            continue;
        }

        mysql::tb_role_set* p = static_cast<mysql::tb_role_set*>(pb_ctx[i].pb_msg);
        if (init(p) != 0) {
            delete pb_ctx[i].pb_msg;
        }
    }

    return 0;
}

