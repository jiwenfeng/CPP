#include "family_skill_mgr.h"

family_skill_mgr::family_skill_mgr()
{
}

family_skill_mgr::~family_skill_mgr()
{
    map<uint32, family_skill*>::iterator i = _skill.begin();
    for (; i != _skill.end(); ++i) {
        delete i->second;
    }
    _skill.clear();

    vector<family_skill*>::iterator iter = _remove.begin();
    for (; iter != _remove.end(); ++iter) {
        delete *iter;
    }
    _remove.clear();
}

#define CUR hash_map_obj<CACHE_KEY, cfg_cache_obj, CFG_CACHE_BULB_NUM>::pair_t

int family_skill_mgr::init(struct cmq_t* l)
{
    CUR* cur = (CUR*)CONFMGR->get_family_skill_head();

    while (cur != NULL) {
        google::protobuf::Message* p = cur->mutable_val().pop_msg();
        mysql::tb_family_skill* tfs = static_cast<mysql::tb_family_skill*>(p);
        cur = (CUR*)cur->get_next();
        if (tfs != NULL) {
            family_skill* fs = new family_skill(tfs);
            _skill.insert(pair<uint32, family_skill*>(tfs->family_id(), fs));
        }
    }
    
    return 0;
}

int family_skill_mgr::save(amtsvr_context* ctx)
{
    map<uint32, family_skill*>::iterator i = _skill.begin();
    for (; i != _skill.end(); ++i) {
        i->second->save(ctx);
    }

    vector<family_skill*>::iterator iter = _remove.begin();
    for (; iter != _remove.end(); ++iter) {
        (*iter)->save(ctx);
        delete *iter;
    }
    _remove.clear();
    
    return 0;
}

family_skill* family_skill_mgr::add(uint32 family_id)
{
    family_skill* p = new family_skill(family_id);
    _skill.insert(pair<uint32, family_skill*>(family_id, p));

    return p;
}

int family_skill_mgr::remove(uint32 family_id)
{
    map<uint32, family_skill*>::iterator i = _skill.find(family_id);
    if (i == _skill.end()) {
        return -1;
    }

    if (i->second != NULL){
        i->second->remove();
        _remove.push_back(i->second);
    }
    
    _skill.erase(i);

    return 0;
}
        
//int family_skill_mgr::fill_skills(uint32 family_id, client::family_skills_rsp& rsp)
//{
//    map<uint32, family_skill*>::iterator i = _skill.find(family_id);
//    if (i == _skill.end()) {
//        add(family_id);
//        return fill_skills(family_id, rsp);
//    }
//
//    rsp.set_family_id(family_id);
//    i->second->fill_skills(rsp);
//    
//    return 0;
//}

//int family_skill_mgr::upgrade_skill(family* f, uint32 skill_id, client::family_skill_upgrade_rsp& rsp)
//{
//    uint32 family_id = f->get()->id();
//    rsp.set_family_id(family_id);
//    rsp.set_skill_id(skill_id);
//    
//    int ret = upgrade_limit(f, skill_id);
//    switch (ret) {
//    case -1: rsp.set_ret(client::family_skill_upgrade_rsp_result_system_error);            return ret;
//    case -2: rsp.set_ret(client::family_skill_upgrade_rsp_result_limit_family_lvl); return ret;
//    case -3: rsp.set_ret(client::family_skill_upgrade_rsp_result_skill_id_error);   return ret;
//    default:
//        break;
//    }
//    
//    map<uint32, family_skill*>::iterator i = _skill.find(family_id);
//    if (i == _skill.end() || i->second == NULL) {
//        rsp.set_ret(client::family_skill_upgrade_rsp_result_system_error);
//        return -1;
//    }
//    
//    skill_id = i->second->upgrade(skill_id);
//    rsp.set_ret(client::family_skill_upgrade_rsp_result_success);
//    i->second->fill_skill(skill_id, rsp.mutable_fsi());
//    
//    return 0;
//}

int family_skill_mgr::upgrade_limit(family* f, uint32 skill_id)
{
    //uint32 family_id = f->get()->id();
    //map<uint32, family_skill*>::iterator i = _skill.find(family_id);
    //family_skill* p = NULL;
    //if (i == _skill.end()) {
    //    p = add(family_id);
    //} else {
    //    p = i->second;
    //}

    //uint32 lvl = p->get_lvl(skill_id);
    //uint32 index = p->id_to_index(skill_id);
    //if (index == 0) {
    //    return -1;
    //}
    //
    //bool   blvl = false;
    //if (index > 8) {
    //    blvl = (2 * (lvl + 1)) <= f->lvl();
    //} else {
    //    blvl = (lvl + 1) <= f->lvl();
    //}

    //if (!blvl) {
    //    return -2;
    //}

    //bool   bmoney =  false;
    //uint32 money = family_skill::UPGRADE_MONEY[index > 8 ? 1 : 0][lvl];
    //bmoney = f->get()->money() >= money;
    //if (!bmoney) {
    //    return -3;
    //}
    //
    //f->set_money(f->get()->money() - money);
    
    return 0;
}
