#include "mate_mgr.h"
#include "config_mgr.h"

mate_mgr::mate_mgr()
{
    ctx = NULL;
}

mate_mgr::~mate_mgr()
{
    map<uint32, mate*>::iterator it = map_mates.begin();
    for (; it != map_mates.end(); ++it){
        delete it->second;
        it->second = NULL;
    }
    map_mates.clear();
}

int mate_mgr::init()
{
    hash_map_obj<CACHE_KEY, cfg_cache_obj, CFG_CACHE_BULB_NUM>::pair_t* cur = (hash_map_obj<CACHE_KEY, cfg_cache_obj, CFG_CACHE_BULB_NUM>::pair_t*)CONFMGR->get_mate_head();
    while (cur != NULL){
        mysql::tb_mate* tm = static_cast<mysql::tb_mate*>(cur->mutable_val().pop_msg());
        cur = (hash_map_obj<CACHE_KEY, cfg_cache_obj, CFG_CACHE_BULB_NUM>::pair_t*)cur->get_next();
        if (tm == NULL){
            continue;
        }

        uint32 role_id = tm->role_id();
        if (map_mates.find(role_id) != map_mates.end()){
            continue;
        }

        mate* m = new mate;
        m->attach(tm);
        map_mates.insert(pair<uint32, mate*>(role_id, m));
    }

    return 0;
}

mate* mate_mgr::get(uint32 role_id)
{
    map<uint32, mate*>::iterator it = map_mates.find(role_id);
    if (it == map_mates.end()){
        return NULL;
    }
    return it->second;
}

const mate* mate_mgr::get_my_mate(uint32 role_id) const
{
    map<uint32, mate*>::const_iterator it = map_mates.find(role_id);
    if (it == map_mates.end()){
        return NULL;
    }
    return it->second;
}

client::MATE_OPERATE_RET_E mate_mgr::fill_mate_info_rsp(uint32 role_id, client::mate_info_rsp& rsp)
{
    mate* m = get(role_id);
    if (m == NULL){
        return client::MORE_NO_MATE;
    }

    return m->fill_mate_info_rsp(rsp);
}

client::MATE_OPERATE_RET_E mate_mgr::create_mate_relation(uint32 role_id, uint32 mate_id)
{
    mate* src = NULL;
    mate* dst = NULL;
    client::MATE_OPERATE_RET_E ret = can_mate_with_role(role_id, src, mate_id, dst);
    if (ret != client::MORE_SUCCESS){
        return ret;
    }

    if (src == NULL){
        src = new mate;
        map_mates.insert(pair<uint32, mate*>(role_id, src));
    }

    if (dst == NULL){
        dst = new mate;
        map_mates.insert(pair<uint32, mate*>(mate_id, dst));
    }

    src->create(role_id, mate_id);
    dst->create(mate_id, role_id);

    src->save_data(ctx);
    dst->save_data(ctx);

    return client::MORE_SUCCESS;
}

client::MATE_OPERATE_RET_E mate_mgr::remove_mate_relation(uint32 role_id, uint32& mate_id)
{
    mate* src = get(role_id);
    if (src == NULL){
        return client::MORE_NO_MATE;
    }

    mate_id = src->get_mate_role_id();
    if (mate_id == 0){
        return client::MORE_NO_MATE;
    }

    mate* dst = get(mate_id);
    if (dst != NULL){
        if (dst->get_mate_role_id() != role_id){
            mate_id = 0;
        } else {
            dst->remove();
            dst->save_data(ctx);
        }
    } else {
        mate_id = 0;
    }

    src->remove();
    src->save_data(ctx);

    return client::MORE_SUCCESS;
}

client::MATE_OPERATE_RET_E mate_mgr::can_mate_with_role(uint32 role_id, mate*& src, uint32 mate_id, mate*& dst)
{
    if (role_id == mate_id || role_id == 0 || mate_id == 0){
        return client::MORE_CANT_SELF_MATE;
    }

    src = get(role_id);
    if (src != NULL && src->get_mate_role_id() > 0){
        return client::MORE_HAD_MATE;
    }

    dst = get(mate_id);
    if (dst != NULL && dst->get_mate_role_id() > 0){
        return client::MORE_DST_HAD_MATE;
    }

    return client::MORE_SUCCESS;
}

client::MATE_OPERATE_RET_E mate_mgr::can_enter_depth_mate(uint32 role_id, mate*& src, uint32& src_depth_times, uint32& mate_id, mate*& dst, uint32& dst_depth_times)
{
    src = get(role_id);
    if (src == NULL){
        return client::MORE_NO_MATE;
    }

    if (src->is_in_depth_mate()){
        return client::MORE_IN_DEPTH;
    }

    mate_id = src->get_mate_role_id();
    if (mate_id == 0){
        return client::MORE_NO_MATE;
    }

    dst = get(mate_id);
    if (dst == NULL){
        src->remove();
        return client::MORE_NO_MATE;
    }

    if (dst->get_mate_role_id() != role_id){
        src->remove();
        return client::MORE_OTHER_NOT_MATE;
    }

    if ((src_depth_times = src->get_can_depth_times()) == 0 
        || (dst_depth_times = dst->get_can_depth_times()) == 0){
        return client::MORE_TIMES_LIMIT;
    }

    return client::MORE_SUCCESS;
}

client::MATE_OPERATE_RET_E mate_mgr::enter_depth_mate(uint32 role_id, uint32& mate_id, bool advanced)
{
    mate* src = NULL;
    mate* dst = NULL;
    uint32 src_depth_times = 0;
    uint32 dst_depth_times = 0;
    client::MATE_OPERATE_RET_E ret = can_enter_depth_mate(role_id, src, src_depth_times, mate_id, dst, dst_depth_times);
    if (ret != client::MORE_SUCCESS){
        return ret;
    }

    uint32 now = (uint32)time(NULL);
    src->depth_mate(now, advanced);
    dst->depth_mate(now, advanced);

    src->save_data(ctx);
    dst->save_data(ctx);

    return client::MORE_SUCCESS;
}

client::MATE_OPERATE_RET_E mate_mgr::end_depth_mate(uint32 role_id, client::MATE_OPERATE_RET_E& src_ret, bool& src_advanced, uint32& mate_id, client::MATE_OPERATE_RET_E& dst_ret, bool& dst_advanced)
{
    mate* src = get(role_id);
    if (src == NULL){
        return client::MORE_NO_MATE;
    }

    mate_id = src->get_mate_role_id();
    if (mate_id == 0){
        return client::MORE_NO_MATE;
    }

    mate* dst = get(mate_id);
    if (dst == NULL){
        return client::MORE_NO_MATE;
    }

    uint32 now = (uint32)time(NULL);
    src_ret = src->depth_mate_end(now, src_advanced);
    dst_ret = dst->depth_mate_end(now, dst_advanced);

    src->save_data(ctx);
    dst->save_data(ctx);

    return src_ret;
}

client::MATE_OPERATE_RET_E mate_mgr::end_depth_mate(uint32 role_id, bool& advanced, uint32& mate_id)
{
    mate* src = get(role_id);
    if (src == NULL){
        return client::MORE_NO_MATE;
    }

    mate_id = src->get_mate_role_id();
    if (mate_id == 0){
        return client::MORE_NO_MATE;
    }

    uint32 now = (uint32)time(NULL);
    client::MATE_OPERATE_RET_E ret = src->depth_mate_end(now, advanced);
    src->save_data(ctx);

    return ret;
}
