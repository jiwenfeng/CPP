#include "mail_center_mgr.h"

mail_center_mgr::mail_center_mgr()
{
}

mail_center_mgr::~mail_center_mgr()
{
    map<uint32, mail_center>::iterator it = center.begin();
    for (; it != center.end(); ++it){
        it->second.release();
    }
    center.clear();
}

int mail_center_mgr::push_new_mail(uint32 role_id, const client::mail_info& mi)
{
    mail_center* mc = mutable_mail(role_id);
    if (mc == NULL){
        mc = &(center[role_id]);
    }

    changes.push_back(role_id);

    mc->push_new_mail(role_id, mi);

    return 0;
}

mail_center* mail_center_mgr::mutable_mail(uint32 role_id)
{
    map<uint32, mail_center>::iterator it = center.find(role_id);
    if (it == center.end()){
        return NULL;
    }

    changes.push_back(it->second.get_role_id());

    return &(it->second);
}

int mail_center_mgr::save_mail(amtsvr_context* ctx)
{
    uint32 size = changes.size();
    for (uint32 i = 0; i < size; ++i){
        map<uint32, mail_center>::iterator it = center.find(changes[i]);
        if (it == center.end()){
            continue;
        }

        it->second.save_mail(ctx);
    }

    changes.clear();

    return 0;
}

int mail_center_mgr::init(struct cmq_t* l)
{
    hash_map_obj<CACHE_KEY, cfg_cache_obj, CFG_CACHE_BULB_NUM>::pair_t* cur = (hash_map_obj<CACHE_KEY, cfg_cache_obj, CFG_CACHE_BULB_NUM>::pair_t*)CONFMGR->get_mail_center_head();
    while (cur != NULL){
        mysql::tb_mail_center* tmc = static_cast<mysql::tb_mail_center*>(cur->mutable_val().pop_msg());
        cur = (hash_map_obj<CACHE_KEY, cfg_cache_obj, CFG_CACHE_BULB_NUM>::pair_t*)cur->get_next();
        if (tmc != NULL){
            center[tmc->role_id()].attach(tmc);
        }
    }

    return 0;
}
