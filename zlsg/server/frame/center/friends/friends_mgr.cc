#include "friends_mgr.h"

friends_mgr::friends_mgr()
{

}

friends_mgr::~friends_mgr()
{
	std::map<uint32, player_role_friends*>::iterator it = map_roles.begin();
	while (it != map_roles.end()) {
		if (it->second != NULL) {
			delete it->second;
		}

		map_roles.erase(it++);
	}

    changes.clear();
}

int friends_mgr::init()
{
	hash_map_obj<CACHE_KEY, cfg_cache_obj, CFG_CACHE_BULB_NUM>::pair_t* cur = (hash_map_obj<CACHE_KEY, cfg_cache_obj, CFG_CACHE_BULB_NUM>::pair_t*)CONFMGR->get_role_friends_head();
	while (cur != NULL) {
		mysql::tb_role_friends* trf = static_cast<mysql::tb_role_friends*>(cur->mutable_val().pop_msg());
		cur = (hash_map_obj<CACHE_KEY, cfg_cache_obj, CFG_CACHE_BULB_NUM>::pair_t*)cur->get_next();
		if (trf != NULL){
			player_role_friends* prf = new player_role_friends(trf->role_id());
			int ret = prf->set_db_friend(trf);
			map_roles[trf->role_id()] = prf;
            if (ret > 0) {
                prf->set_change(true);
                changes[trf->role_id()] = prf;
            }
		}
	}

	return 0;
}

int friends_mgr::save(amtsvr_context* ctx)
{
    if (changes.empty()) {
        return 0;
    }

    std::map<uint32, player_role_friends*>::iterator it = changes.begin();
    for (; it != changes.end(); ++it) {
        it->second->save(ctx);
    }

    changes.clear();

    return 0;
}

const player_role_friends* friends_mgr::get(uint32 role_id) const
{
	map<uint32, player_role_friends*>::const_iterator it = map_roles.find(role_id);
	if (it == map_roles.end()){
		return NULL;
	}
	return it->second;
}

player_role_friends* friends_mgr::mut(uint32 role_id)
{
	std::map<uint32, player_role_friends*>::iterator it = map_roles.find(role_id);
	if (it == map_roles.end()){
		return NULL;
	}

	return it->second;
}

bool friends_mgr::can_add_friend(uint32 src_role_id, uint32 dst_role_id)
{
	if (src_role_id == dst_role_id){
		return false;
	}

	const player_role_friends* src = get(src_role_id);
	if (src != NULL && (src->is_friends_num_limit()) ){
		return false;
	}

	return true;
}

bool friends_mgr::is_aready_friend(uint32 src_role_id, uint32 dst_role_id)
{
	if (src_role_id == dst_role_id){
		return false;
	}

	const player_role_friends* src = get(src_role_id);
	if (src == NULL){
		src = create(src_role_id);
	}

	if (!src->has_friend(dst_role_id)){
		return false;
	}

	return true;
}

player_role_friends* friends_mgr::create(uint32 role_id)
{
	player_role_friends* new_prf = new player_role_friends(role_id);

	map_roles.insert(pair<uint32, player_role_friends*>(role_id, new_prf));

	return new_prf;
}

int friends_mgr::add_friend(uint32 src_role_id, uint32 dst_role_id,uint32 friend_level,string friend_name,uint32 vip_level)
{
	player_role_friends* src = mut(src_role_id);
	if (src != NULL && src->is_friends_num_limit() && !src->has_friend(dst_role_id) ){
		return -1;
	}

	//player_role_friends* dst = mut(dst_role_id);
	//if (dst != NULL && dst->is_friends_num_limit() && !dst->has_friend(src_role_id) ){
	//	return -2;
	//}

	if (src == NULL){
		src = create(src_role_id);
	}

	//if (dst == NULL){
	//	dst = create(dst_role_id);
	//}

	src->add_friend(dst_role_id,friend_level,friend_name,vip_level);
	//dst->add_friend(src_role_id);

    changes.insert(pair<uint32, player_role_friends*>(src_role_id, src));
    //changes.insert(pair<uint32, player_role_friends*>(dst_role_id, dst));

	return 0;
}
int friends_mgr::add_temporary_friend(uint32 src_role_id, uint32 dst_role_id,uint32 friend_level,string friend_name,uint32 vip_level,client::friend_list_rsp& rsp_msg,bool& has_del,client::friend_list_rsp& del_rsp)
{
	rsp_msg.set_list_type(FLRE_ADD);
	player_role_friends* src = mut(src_role_id);
	if (src != NULL && src->is_friends_num_limit() && !src->has_temporary_friend(dst_role_id) ){
		return -1;
	}

	if (src == NULL){
		src = create(src_role_id);
	}

	if (src->add_temporary_friend(dst_role_id,friend_level,vip_level,friend_name,has_del)){
		changes.insert(pair<uint32, player_role_friends*>(src_role_id, src));
	}

	src->fill_add_friend_info(rsp_msg);
	
	if (has_del){
		src->fill_del_friend_info(del_rsp);
	}

	return 0;
}

int friends_mgr::del_friend(uint32 src_role_id, uint32 dst_role_id)
{
	player_role_friends* src = mut(src_role_id);
	//player_role_friends* dst = mut(dst_role_id);

	if (src != NULL){
		src->del_friend(dst_role_id);
        changes.insert(pair<uint32, player_role_friends*>(src_role_id, src));
	}

	//if (dst != NULL){
	//	dst->del_friend(src_role_id);
 //       changes.insert(pair<uint32, player_role_friends*>(dst_role_id, dst));
	//}

	return 0;
}

int friends_mgr::blk_friend(uint32 src_role_id, uint32 dst_role_id)
{
    player_role_friends* src = mut(src_role_id);
	if (src != NULL && src->is_friends_num_limit() && !src->has_friend(dst_role_id) ){
		return -1;
	}

	/*player_role_friends* dst = mut(dst_role_id);
	if (dst != NULL && dst->is_friends_num_limit() && !dst->has_friend(src_role_id) ){
		return -2;
        }*/

	if (src == NULL){
		src = create(src_role_id);
	}

	/*if (dst == NULL){
		dst = create(dst_role_id);
        }*/

	src->blk_friend(dst_role_id);
    changes.insert(pair<uint32, player_role_friends*>(src_role_id, src));
    return 0;
}

int friends_mgr::add_degree(uint32 src_role_id, uint32 dst_role_id, uint32 val, uint32& src_ret, uint32& dst_ret)
{
    player_role_friends* src = mut(src_role_id);
    if (src != NULL){
        src_ret = src->add_degree(dst_role_id, val);
        changes.insert(pair<uint32, player_role_friends*>(src_role_id, src));
    }

    player_role_friends* dst = mut(dst_role_id);
    if (dst != NULL){
        dst_ret = dst->add_degree(src_role_id, val);
        changes.insert(pair<uint32, player_role_friends*>(dst_role_id, dst));
    }

    return 0;
}

uint32 friends_mgr::get_degree(uint32 src_role_id, uint32 dst_role_id)
{
    player_role_friends* src = mut(src_role_id);
    if (src != NULL){
        return src->get_degree(dst_role_id);
    }

    player_role_friends* dst = mut(dst_role_id);
    if (dst != NULL){
        return dst->get_degree(src_role_id);
    }

    return 0;
}
