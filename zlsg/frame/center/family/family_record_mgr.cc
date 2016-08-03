#include "family_record_mgr.h"

family_record_mgr::family_record_mgr()
{
}

family_record_mgr::~family_record_mgr()
{
    multimap<uint32, multimap<uint32, family_record*> >::iterator i = _record.begin();
    for (; i != _record.end(); ++i) {
        for (multimap<uint32, family_record*>::iterator j = i->second.begin(); j != i->second.end(); ++j) {
            delete j->second;
        }
        i->second.clear();
    }
    _record.clear();

    std::map<uint32, family_record*>::iterator j;
    for (j = _remove.begin(); j != _remove.end(); ++j) {
        delete j->second;
    }
    _remove.clear();

    
    map<uint32, multimap<uint32, client::family_record_info*> >::iterator k = _info.begin();
    multimap<uint32, client::family_record_info*>::iterator l;
    for (; k != _info.end(); ++k) {
        for (l = k->second.begin(); l != k->second.end(); ++l) {
            delete l->second;
        }
        k->second.clear();
    }
    _info.clear();
}

#define CUR hash_map_obj<CACHE_KEY, cfg_cache_obj, CFG_CACHE_BULB_NUM>::pair_t

int family_record_mgr::init(struct cmq_t* l)
{
    CUR* cur = (CUR*)CONFMGR->get_family_record_head();

    while (cur != NULL) {
        google::protobuf::Message* p = cur->mutable_val().pop_msg();
        mysql::tb_family_record* tfr = static_cast<mysql::tb_family_record*>(p);
        cur = (CUR*)cur->get_next();
        if (tfr != NULL) {
            family_record* fr = new family_record(tfr);
            multimap<uint32, multimap<uint32, family_record*> >::iterator i = _record.find(tfr->killer_family());
            if (i != _record.end()) {
                i->second.insert(pair<uint32, family_record*>(tfr->killer(), fr));
            } else {
                multimap<uint32, family_record*> map;
                map.insert(pair<uint32, family_record*>(tfr->killer(), fr));
                _record.insert(pair<uint32, multimap<uint32, family_record*> >(tfr->killer_family(), map));
            }
        }
    }

    make_info();

    set_kill_info();
    
    return 0;
}

int family_record_mgr::save(amtsvr_context* ctx)
{
    //multimap<uint32, multimap<uint32, family_record*> >::iterator i = _record.begin();
    //std::map<uint32, family_record*>::iterator j;
    //for (; i != _record.end(); ++i) {
    //    for (j = i->second.begin(); j != i->second.end(); ++j) {
    //        j->second->save(ctx);
    //    }
    //}

    //for (j = _remove.begin(); j != _remove.end(); ++j) {
    //    j->second->save(ctx);
    //    delete j->second;
    //}

    //_remove.clear();
    //
    return 0;
}

int family_record_mgr::update(client::family_record_update* fru)
{
    family_record* fr = new family_record(fru->type(), fru->killer_family(), fru->killer(), fru->slain_family(), fru->slain_title(), fru->slain());
    add(fr);
    return 0;
}

int family_record_mgr::remove(uint32 family_id, uint32 role_id)
{
    if (family_id == 0) {
        return -1;
    }

    multimap<uint32, multimap<uint32, family_record*> >::iterator i = _record.find(family_id);
    if (i == _record.end()) {
        return 0;
    }
    
    return remove(i->second, role_id);
}

int family_record_mgr::remove(multimap<uint32, family_record*>& m, uint32 role_id)
{
    multimap<uint32, family_record*>::iterator j;    
    if (role_id == 0) {
        j = m.begin();
    } else {
        j = m.find(role_id);
    }

    if (j == m.end()) {
        return 0;
    }

    while (j != m.end()) {
        if (j->second != NULL){
            j->second->remove();
            _remove.insert(pair<uint32, family_record*>(j->second->get()->id(), j->second));
        }
        
        m.erase(j++);

        if (role_id != 0 && j->second->get()->killer() != role_id) {
            break;
        }
    }
    
    return 0;
}

int family_record_mgr::fill(uint32 family_id, client::family_record_rsp& rsp)
{
    rsp.set_family_id(family_id);
    map<uint32, multimap<uint32, client::family_record_info*> >::iterator i = _info.find(family_id);
    if (i != _info.end()) {
        multimap<uint32, client::family_record_info*>::iterator j = i->second.begin();
        for (; j != i->second.end(); ++j) {
            client::family_record_info* fri = rsp.add_fri();
            fri->CopyFrom(*(j->second));
            // printf("r = %d, kc = %d, dn = %d, bk = %d, bkn = %d\n", fri->role_id(), fri->kill_chief_num(), fri->dead_num(), fri->be_kill(), fri->be_kill_num());
        }
    }

    return 0;
}

void family_record_mgr::delete_all()
{
    multimap<uint32, multimap<uint32, family_record*> >::iterator i = _record.begin();
    for (; i != _record.end(); ++i) {
        remove(i->second, 0);
    }
}

void family_record_mgr::add(family_record* fr)
{
    multimap<uint32, multimap<uint32, family_record*> >::iterator i = _record.find(fr->get()->killer_family());
    if (i != _record.end()) {
        i->second.insert(pair<uint32, family_record*>(fr->get()->killer(), fr));
    } else {
        multimap<uint32, family_record*> map;
        map.insert(pair<uint32, family_record*>(fr->get()->killer(), fr));
        _record.insert(pair<uint32, multimap<uint32, family_record*> >(fr->get()->killer_family(), map));
    }

    to_info(fr);
}

void family_record_mgr::make_info()
{
    multimap<uint32, multimap<uint32, family_record*> >::iterator i = _record.begin();
    for (; i != _record.end(); ++i) {
        multimap<uint32, family_record*>::iterator j = i->second.begin();
        for (; j != i->second.end(); ++j) {
            map<uint32, multimap<uint32, client::family_record_info*> >::iterator it = _info.find(i->first);
            if (it == _info.end()) {
                _info.insert(pair<uint32, multimap<uint32, client::family_record_info*> >(i->first, multimap<uint32, client::family_record_info*>()));
                it = _info.find(i->first);
            }

            multimap<uint32, client::family_record_info*>::iterator jt = it->second.find(j->first);
            if (jt == it->second.end()) {
                init_info(j->second, it->second);
            } else {
                for (; jt != it->second.end(); ++jt) {
                    if (jt->second->type() == j->second->get()->type() && jt->second->role_id() == j->second->get()->killer()) {
                        set_info(j->second, jt->second);
                    }
                }
            }
        }
    }
}

void family_record_mgr::init_info(family_record* record, multimap<uint32, client::family_record_info*>& info)
{
    uint32 role_id = record->get()->killer();
    uint32 type    = record->get()->type();
    // _kill.insert(pair<uint32, map<uint32, uint32> >(role_id, map<uint32, uint32>()));
    for (int i = 0; i < RECORD_TYPE_MAX; ++i) {
        client::family_record_info* p = new client::family_record_info;
        p->set_role_id(role_id);
        p->set_join_num(0);
        p->set_kill_chief_num(0);
        p->set_kill_vice_chief_num(0);
        p->set_kill_elder_num(0);
        p->set_kill_elite_num(0);
        p->set_kill_member_num(0);
        p->set_dead_num(0);
        p->set_be_kill(0);
        p->set_be_kill_num(0);
        p->set_type(i);
        if (p->type() == type) {
            set_info(record, p);
        }

        info.insert(pair<uint32, client::family_record_info*>(role_id, p));
    }

}
// 看看怎么改
void family_record_mgr::set_info(family_record* record, client::family_record_info* info)
{
    switch (record->get()->slain_title()) {
    case MEMBER:
        info->set_kill_member_num(info->kill_member_num() + 1);
        break;
    case ELITE:
        info->set_kill_elite_num(info->kill_elite_num() + 1);
        break;
    case ELDER:
        info->set_kill_elder_num(info->kill_elder_num() + 1);
        break;
    case VICE_CHIEF:
        info->set_kill_vice_chief_num(info->kill_vice_chief_num() + 1);
        break;
    case CHIEF:
        info->set_kill_chief_num(info->kill_chief_num() + 1);
        break;
    default:
        info->set_kill_member_num(info->kill_member_num() + 1);
        break;
    }

    uint32 type   = record->get()->type();
    uint32 killer = record->get()->killer();
    uint32 slain  = record->get()->slain();
    map<uint32, uint32>::iterator jt = _kill[type][slain].find(killer);
    if (jt == _kill[type][slain].end()) {
        _kill[type][slain][killer] = 1;
    } else {
        _kill[type][slain][killer] += 1;
    }
}

void family_record_mgr::to_info(family_record* p)
{
    //uint32 family_id = p->get()->killer_family();
    //uint32 role_id   = p->get()->killer();
    //map<uint32, multimap<uint32, client::family_record_info*> >::iterator it = _info.find(family_id);
    //if (it == _info.end()) {
    //    _info.insert(pair<uint32, multimap<uint32, client::family_record_info*> >(family_id, multimap<uint32, client::family_record_info*>()));
    //     it = _info.find(family_id);
    //}
    //
    //multimap<uint32, client::family_record_info*>::iterator jt = it->second.find(role_id);
    //if (jt == it->second.end()) {
    //    init_info(p, it->second);
    //} else {
    //    for (; jt != it->second.end(); ++jt) {
    //        if (jt->second != NULL && jt->second->type() == p->get()->type()) {
    //            set_info(p, jt->second);
    //        }
    //    }
    //}
}

void family_record_mgr::set_kill_info() 
{
    map<uint32, multimap<uint32, client::family_record_info*> >::iterator i = _info.begin();
    for (; i != _info.end(); ++i) {
        multimap<uint32, client::family_record_info*>::iterator j = i->second.begin();
        for (; j != i->second.end(); ++j) {
            uint32 role_id  = j->second->role_id();
            uint32 type     = j->second->type();
            uint32 dead_num = 0;
            uint32 max_killer   = 0;
            uint32 max_kill_num = 0;
            map<uint32, uint32>::iterator it = _kill[type][role_id].begin();
            for (; it != _kill[type][role_id].end(); ++it) {
                if (max_kill_num < it->second) {
                    max_killer   = it->first;
                    max_kill_num = it->second;
                }
                
                dead_num += it->second;
            }
            
            j->second->set_dead_num(dead_num);
            j->second->set_be_kill(max_killer);
            j->second->set_be_kill_num(max_kill_num);
        }
    }
}

