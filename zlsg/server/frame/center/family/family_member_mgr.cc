#include "family_member_mgr.h"

family_member_mgr::family_member_mgr()
{
}

family_member_mgr::~family_member_mgr()
{
    map_role_family_member::iterator i = _map_role_family_member.begin();
    for (; i != _map_role_family_member.end(); ++i) {
        delete i->second;
    }
    _map_role_family_member.clear();

    family_role::iterator iter = _family_role.begin();
    for (; iter != _family_role.end(); ++iter) {
        delete iter->second;
    }
    _family_role.clear();

    multimap<uint32, family_member*>::iterator it = _remove.begin();
    for (; it != _remove.end(); ++it) {
        delete it->second;
    }
    _remove.clear();
}

#define CUR hash_map_obj<CACHE_KEY, cfg_cache_obj, CFG_CACHE_BULB_NUM>::pair_t

int family_member_mgr::init(struct cmq_t* l)
{
    CUR* cur = (CUR*)CONFMGR->get_family_member_head();

    while (cur != NULL) {
        google::protobuf::Message* p = cur->mutable_val().pop_msg();
        mysql::tb_family_member* tfm = static_cast<mysql::tb_family_member*>(p);
        cur = (CUR*)cur->get_next();
        if (tfm != NULL) {
            family_member* fm = new family_member(tfm);
            _map_role_family_member.insert(pair_role_family_member(tfm->role_id(), fm));
            family_role::iterator i = _family_role.find(tfm->family_id());
            if (i == _family_role.end()) {
                role_permissions* rp = new role_permissions;
                rp->insert(pair_role_permissions(tfm->role_id(), family_role_value(tfm->permissions(), tfm->add_by(), tfm->time(), tfm->strength(),tfm->dragon_count(),tfm->tiger_count(),tfm->rosefinch_count(),tfm->tortoise_count(),tfm->money(),tfm->total_donate())));
                _family_role.insert(pair_family_role(tfm->family_id(), rp));
            } else {
                i->second->insert(pair_role_permissions(tfm->role_id(), family_role_value(tfm->permissions(), tfm->add_by(), tfm->time(), tfm->strength(),tfm->dragon_count(),tfm->tiger_count(),tfm->rosefinch_count(),tfm->tortoise_count(),tfm->money(),tfm->total_donate())));
            }          
        }
    }

    return 0;
}

int family_member_mgr::save(amtsvr_context* ctx)
{
    map_role_family_member::iterator i = _map_role_family_member.begin();
    for (; i != _map_role_family_member.end(); ++i) {
        i->second->save(ctx);
    }

    multimap<uint32, family_member*>::iterator it = _remove.begin();
    for (; it != _remove.end(); ++it) {
        it->second->save(ctx);
        delete it->second;
    }
    _remove.clear();    
    
    return 0;
}

role_permissions* family_member_mgr::get_role_permissions(uint32 family_id)
{
    family_role::iterator i =  _family_role.find(family_id);
    if (i == _family_role.end()) {
        return NULL;
    }

    return i->second;
}
// 
family_role_value* family_member_mgr::get_family_role(uint32 family_id, uint32 role_id)
{
    role_permissions* rp = get_role_permissions(family_id);
    if (rp == NULL) {
        return NULL;
    }

    role_permissions::iterator iter = rp->find(role_id);
    if (iter == rp->end()) {
        return NULL;
    }

    return &(iter->second);
}

int family_member_mgr::add_role(uint32 add_by, uint32 family_id, uint32 role_id, FAMILY_MEMBER_TYPE type)
{
    family_role::iterator i =  _family_role.find(family_id);

    if (i != _family_role.end()) {
        if (i->second->find(role_id) != i->second->end()) {
            return -1;
        }
        i->second->insert(pair_role_permissions(role_id, family_role_value(type, add_by)));
    } else if (i == _family_role.end() && type == CHIEF) {
        
        role_permissions* rp = new role_permissions;
        rp->insert(pair_role_permissions(role_id, family_role_value(type, add_by)));
        _family_role.insert(pair_family_role(family_id, rp));
    } else {
        return -1;
    }

    family_member* p = new family_member(family_id, role_id, type, (uint32)time(NULL), add_by);
    _map_role_family_member.insert(pair_role_family_member(role_id, p));

    return 0;
}


int family_member_mgr::apply(uint32 add_by, uint32 family_id, uint32 role_id)
{
    return add_role(add_by, family_id, role_id, APPLY);
}

int family_member_mgr::auto_add(uint32 add_by, uint32 family_id, uint32 role_id)
{
	add_role(add_by, family_id, role_id, APPLY);

	join(family_id,role_id);

	return 0;
}

int family_member_mgr::invite(uint32 add_by, uint32 family_id, uint32 role_id)
{
    return add_role(add_by, family_id, role_id, INVITE);
}

int family_member_mgr::join(uint32 family_id, uint32 role_id)
{
    family_role::iterator i =  _family_role.find(family_id);
    if (i == _family_role.end()) {
        return -1;
    }

    role_permissions::iterator iter = i->second->find(role_id);
    if (iter == i->second->end()) {
        return -2;
    }

    iter->second._permissions = MEMBER;

    std::vector<uint32> del;
    map_role_family_member::iterator i0 = _map_role_family_member.end();
    i0 = _map_role_family_member.find(role_id);
    for (; i0 != _map_role_family_member.end(); ++i0) {
        if (i0->first == role_id && i0->second->family_id() == family_id) {
            i0->second->set_permissions(MEMBER);
            i0->second->set_time((uint32)time(NULL));
        } else if (i0->first == role_id) {
            del.push_back(i0->second->family_id());
        } else {
            break;
        }
    }

    size_t size = del.size();
    for (size_t i = 0; i < size; ++i) {
        remove(del[i], role_id);
    }

    return 0;
}

int family_member_mgr::modify_role_permissions(uint32 family_id, uint32 role_id, uint32 permissions)
{
	
    family_role::iterator i =  _family_role.find(family_id);
    if (i == _family_role.end()) {
        return -1;
    }
	
    role_permissions::iterator iter = i->second->find(role_id);
    if (iter == i->second->end()) {
        return -2;
    }
	
    iter->second._permissions = permissions;

    std::vector<uint32> del;
    map_role_family_member::iterator i0 = _map_role_family_member.end();
    i0 = _map_role_family_member.find(role_id);
    for (; i0 != _map_role_family_member.end(); ++i0) {
        if (i0->first == role_id && i0->second->family_id() == family_id) {
            i0->second->set_permissions(permissions);
        } else if (i0->first == role_id) {
            del.push_back(i0->second->family_id());
        } else {
            break;
        }
    }

    size_t size = del.size();
    for (size_t i = 0; i < size; ++i) {
        remove(del[i], role_id);
    }

    return 0;
}

int family_member_mgr::fill_member(uint32 family_id, vector<uint32> &v)
{
    role_permissions* p = get_role_permissions(family_id);
    if (p == NULL) {
        return -1;
    }
    role_permissions::iterator i = p->begin();
    for (; i != p->end(); ++i) {
        if (i->second._permissions < MEMBER) {
            continue;
        }
		v.push_back(i->first);
    }
    return 0;
}

int family_member_mgr::fill_member(uint32 family_id, client::family_member_list_rsp& rsp)
{
    role_permissions* p = get_role_permissions(family_id);
    if (p == NULL) {
        return -1;
    }

    role_permissions::iterator i = p->begin();
    for (; i != p->end(); ++i) {
        if (i->second._permissions < MEMBER) {
            continue;
        }

        client::family_member_info* fmi = rsp.add_fmi();
        fmi->set_id(i->first);
        fmi->set_permissions(i->second._permissions);
        fmi->set_time(i->second._time);
		fmi->set_total_donate(i->second._total_donate);
    }
    
    return 0;
}

int family_member_mgr::fill_member_donate_info(uint32 family_id, client::donate_list_rsp& rsp_msg)
{
	role_permissions* member_donate_list_ = get_role_permissions(family_id);
	if (member_donate_list_ == NULL) {
		return -1;
	}

	role_permissions::iterator itr = member_donate_list_->begin();

	for (;itr != member_donate_list_->end();++itr){
		client::donate_info* info = rsp_msg.add_info();
		info->set_role_id(itr->first);
		info->set_dragon_count(itr->second._dragon_count);
		info->set_tiger_count(itr->second._tiger_count);
		info->set_rosefinch_count(itr->second._rosefinch_count);
		info->set_tortoise_count(itr->second._tortoise_count);
		info->set_money(itr->second._money);
	}

	return 0;
}

int family_member_mgr::fill_member_apply(uint32 family_id, client::apply_family_list_rsp& rsp)
{
    role_permissions* p = get_role_permissions(family_id);
    if (p == NULL) {
        return -1;
    }

    role_permissions::iterator i = p->begin();
    for (; i != p->end(); ++i) {
        if (i->second._permissions == 0) {
            rsp.add_role_id(i->first);
        }
    }

    return 0;
}

int family_member_mgr::fill_member_apply_self(uint32 role_id, client::apply_family_list_self_rsp& rsp)
{
    map_role_family_member::iterator i0 = _map_role_family_member.end();
    i0 = _map_role_family_member.find(role_id);
    for (; i0 != _map_role_family_member.end(); ++i0) {
        if (role_id != i0->first) {
            break;
        }

        if (i0->second->permissions() >= MEMBER) {
            continue;
        }

        rsp.add_family_id(i0->second->family_id());
        rsp.add_time(i0->second->time());
    }
    
    return 0;
}

int family_member_mgr::fill_member_invite(uint32 role_id, client::invite_family_list_rsp& rsp)
{
    map_role_family_member::iterator i0 = _map_role_family_member.end();
    i0 = _map_role_family_member.find(role_id);
    for (; i0 != _map_role_family_member.end(); ++i0) {
        if (i0->first != role_id) {
            break;
        }

        if (i0->second->permissions() != APPLY) {
            continue;
        }
        
        client::invite_family_info* ifi = rsp.add_ifi();
        ifi->set_family_id(i0->second->family_id());
        ifi->set_role_id(i0->second->add_by());
    }
    
    return 0;
}

int family_member_mgr::remove(uint32 family_id, uint32 role_id)
{
    role_permissions* p = get_role_permissions(family_id);
    if (p == NULL) {
        return -1;
    }

    if (p->find(role_id) == p->end()) {
        return -2;
    }

    p->erase(role_id);
    map_role_family_member::iterator i0 = _map_role_family_member.find(role_id);
    for (; i0 != _map_role_family_member.end(); ++i0) {
        if (role_id == i0->first && i0->second->family_id() == family_id) {
            if (i0->second != NULL) {
                i0->second->remove();
                _remove.insert(pair<uint32, family_member*>(i0->second->get()->role_id(), i0->second));
            }
        } else {
            break;
        }
    }

    map_role_family_member::iterator iter = _map_role_family_member.find(role_id);
    while (iter != _map_role_family_member.end()) {
        if (iter->first == role_id && iter->second->family_id() == family_id) {
            map_role_family_member::iterator erase_iter = iter++;
            _map_role_family_member.erase(erase_iter);
        } else {
            ++iter;
        }
    }
    
    return 0;
}

int family_member_mgr::remove(uint32 family_id)
{
    family_role::iterator i =  _family_role.find(family_id);
    if (i == _family_role.end()) {
        return -1;
    }
    
    for (role_permissions::iterator member = i->second->begin(); member != i->second->end(); ++member) {
        map_role_family_member::iterator i0 = _map_role_family_member.find(member->first);
        for (; i0 != _map_role_family_member.end(); ++i0) {
            if (i0->second != NULL && i0->second->family_id() == family_id) {
                i0->second->remove();
                _remove.insert(pair<uint32, family_member*>(i0->second->get()->role_id(), i0->second));
            }
        }

        map_role_family_member::iterator iter = _map_role_family_member.find(member->first);
        while (iter != _map_role_family_member.end()) {
            if (iter->second->family_id() == family_id) {
                map_role_family_member::iterator erase_iter = iter++;
                _map_role_family_member.erase(erase_iter);
            } else {
                ++iter;
            }
        }
    }

    i->second->clear();
    delete i->second;
    _family_role.erase(i);

    return 0;
}

uint32 family_member_mgr::member_num(uint32 family_id)
{
    role_permissions* p = get_role_permissions(family_id);
    if (p == NULL) {
        return 0;
    }

    uint32 count = 0;
    for (role_permissions::iterator i = p->begin(); i != p->end(); ++i) {
        if (i->second._permissions < MEMBER) {
            ++count;
        }
    }

    if (p->size() < count) {
        return 0;
    }
    
    return p->size() - count;
}

uint32 family_member_mgr::chief_id(uint32 family_id)
{
    role_permissions* p = get_role_permissions(family_id);
    if (p == NULL) {
        return 0;
    }

    for (role_permissions::iterator i = p->begin(); i != p->end(); ++i) {
        if (i->second._permissions == CHIEF) {
            return i->first;
        }
    }

    return 0;
}

uint32 family_member_mgr::vice_chief_id(uint32 family_id)
{
	role_permissions* p = get_role_permissions(family_id);
	if (p == NULL) {
		return 0;
	}

	for (role_permissions::iterator i = p->begin(); i != p->end(); ++i) {
		if (i->second._permissions == VICE_CHIEF) {
			return i->first;
		}
	}

	return 0;
}

uint32 family_member_mgr::permission_id(uint32 family_id,uint32 permission_value)
{
	role_permissions* p = get_role_permissions(family_id);
	if (p == NULL) {
		return 0;
	}

	for (role_permissions::iterator i = p->begin(); i != p->end(); ++i) {
		if (i->second._permissions == permission_value && permission_value != MEMBER) {
			return i->first;
		}
	}

	return 0;
}

uint32 family_member_mgr::official_id(uint32 family_id, std::vector<uint32>& official_id)
{
    role_permissions* p = get_role_permissions(family_id);
    if (p == NULL) {
        return -1;
    }

    for (role_permissions::iterator i = p->begin(); i != p->end(); ++i) {
        if (i->second._permissions > MEMBER) {
            official_id.push_back(i->first);
        }
    }

    return 0;
}

uint32 family_member_mgr::family_id(uint32 role_id)
{
    map_role_family_member::iterator i0 = _map_role_family_member.begin();
    i0 = _map_role_family_member.find(role_id);
    for (; i0 != _map_role_family_member.end(); ++i0) {
        //printf("role_id = %d, family_id = %d, permissions = %d\n", i0->first, i0->second->family_id(), i0->second->permissions());
        if (role_id != i0->first) {
            break;
        }
        
        if (i0->second->permissions() > INVITE) {
            return i0->second->family_id();
        }
    }

    return 0;
}

uint32 family_member_mgr::member_id(uint32 family_id, std::vector<uint32>& member_id)
{
    role_permissions* p = get_role_permissions(family_id);
    if (p == NULL) {
        return -1;
    }
    
    for (role_permissions::iterator i = p->begin(); i != p->end(); ++i) {
        if (i->second._permissions < MEMBER) {
            continue;
        }
        member_id.push_back(i->first);
    }

    return 0;
}

int family_member_mgr::is_already_apply(uint32 family_id, uint32 role_id)
{

	role_permissions* p = get_role_permissions(family_id);
	if (p == NULL) {
		return -1;
	}

	role_permissions::iterator i = p->find(role_id);
	if (i == p->end()) {
		return -1;
	}

	if (i->second._permissions != APPLY){
		return -1;
	}

	return 0;
}

uint32 family_member_mgr::get_permissions(uint32 family_id, uint32 role_id)
{
    role_permissions* p = get_role_permissions(family_id);
    if (p == NULL) {
        return 0;
    }

    role_permissions::iterator i = p->find(role_id);
    if (i == p->end()) {
        return 0;
    }

    return i->second._permissions;
}

uint32 family_member_mgr::elite_num(uint32 family_id)
{
    return permissions_num(ELITE, family_id);
}

uint32 family_member_mgr::elder_num(uint32 family_id)
{
    return permissions_num(ELDER, family_id);
}

uint32 family_member_mgr::tianshi_num(uint32 family_id)
{
	return permissions_num(TIANSHI, family_id);
}
uint32 family_member_mgr::vice_chief_num(uint32 family_id)
{
    return permissions_num(VICE_CHIEF, family_id);
}

uint32 family_member_mgr::permissions_num(uint32 permissions, uint32 family_id)
{
    role_permissions* p = get_role_permissions(family_id);
    if (p == NULL) {
        return -1;
    }

    uint32 num = 0;
    for (role_permissions::iterator i = p->begin(); i != p->end(); ++i) {
        if (i->second._permissions == permissions) {
            ++num;
        }
    }

    return num;
}

uint32 family_member_mgr::get_apply_count(uint32 family_id)
{
    uint32 count = 0;
    map_role_family_member::iterator i0 = _map_role_family_member.begin();
    for (; i0 != _map_role_family_member.end(); ++i0) {
		if (i0->second->family_id() == family_id && i0->second->permissions() == APPLY){
			++count;
		}
    }

    return count;
}

int family_member_mgr::get_family_manager(uint32 family_id, std::vector<struct manager_info>& manager)
{
	role_permissions *p = get_role_permissions(family_id);
	if(p == NULL)
	{
		return -1;
	}
	for(role_permissions::iterator i = p->begin(); i != p->end(); ++i)
	{
		if(MEMBER < i->second._permissions && i->second._permissions < MYSTERY)
		{
//			struct manager_info m;
//			m.role_id = i->first;
//			m.official_id = i->second._permissions;
			manager.push_back(manager_info(i->first, i->second._permissions));
		}
	}
	return 0;
}

int family_member_mgr::get_family_manager(uint32 family_id, std::vector<uint32>& manager)
{
    role_permissions* p = get_role_permissions(family_id);
    if (p == NULL) {
        return -1;
    }

    for (role_permissions::iterator i = p->begin(); i != p->end(); ++i) {
        if (i->second._permissions > MEMBER) {
            manager.push_back(i->first);
        }
    }
    
    return 0;
}

int family_member_mgr::get_family_manager_add(uint32 family_id, std::vector<uint32>& manager)
{
    role_permissions* p = get_role_permissions(family_id);
    if (p == NULL) {
        return -1;
    }

    for (role_permissions::iterator i = p->begin(); i != p->end(); ++i) {
        if (i->second._permissions > ELITE) {
            manager.push_back(i->first);
        }
    }
    
    return 0;
}

uint32 family_member_mgr::get_join_time(uint32 family_id, uint32 role_id)
{
    map_role_family_member::iterator i0 = _map_role_family_member.find(role_id);
    for (; i0 != _map_role_family_member.end(); ++i0) {
        if (i0->first == role_id && i0->second->family_id() == family_id) {
            return i0->second->time();
        } 
    }
    
    return 0;
}

uint32 family_member_mgr::get_referrer(uint32 family_id, uint32 role_id)
{
    map_role_family_member::iterator i0 = _map_role_family_member.end();
    i0 = _map_role_family_member.find(role_id);
    for (; i0 != _map_role_family_member.end(); ++i0) {
        if (i0->first == role_id && i0->second->family_id() == family_id) {
            return i0->second->add_by();
        } 
    }
    
    return 0;
}

uint32 family_member_mgr::set_strength(uint32 family_id, uint32 role_id, uint32 strength)
{
    family_role::iterator i =  _family_role.find(family_id);
    if (i == _family_role.end()) {
        return -1;
    }

    role_permissions::iterator iter = i->second->find(role_id);
    if (iter == i->second->end()) {
        return -2;
    }

    iter->second._strength = strength;
    
    map_role_family_member::iterator i0 = _map_role_family_member.end();
    i0 = _map_role_family_member.find(role_id);
    for (; i0 != _map_role_family_member.end(); ++i0) {
        if (i0->first == role_id && i0->second->family_id() == family_id) {
            i0->second->set_strength(strength);
            break;
        } 
    }

    return 0;
}
uint32 family_member_mgr::set_dragon_count(uint32 family_id, uint32 role_id, uint32 dragon_count)
{
	family_role::iterator i =  _family_role.find(family_id);
	if (i == _family_role.end()) {
		return -1;
	}

	role_permissions::iterator iter = i->second->find(role_id);
	if (iter == i->second->end()) {
		return -2;
	}

	iter->second._dragon_count += dragon_count;

	map_role_family_member::iterator i0 = _map_role_family_member.end();
	i0 = _map_role_family_member.find(role_id);
	for (; i0 != _map_role_family_member.end(); ++i0) {
		if (i0->first == role_id && i0->second->family_id() == family_id) {
			i0->second->set_dragon_count(iter->second._dragon_count);
			break;
		} 
	}

	return 0;
}

uint32 family_member_mgr::set_tiger_count(uint32 family_id, uint32 role_id, uint32 tiger_count)
{

	family_role::iterator i =  _family_role.find(family_id);
	if (i == _family_role.end()) {
		return -1;
	}

	role_permissions::iterator iter = i->second->find(role_id);
	if (iter == i->second->end()) {
		return -2;
	}

	iter->second._tiger_count += tiger_count;

	map_role_family_member::iterator i0 = _map_role_family_member.end();
	i0 = _map_role_family_member.find(role_id);
	for (; i0 != _map_role_family_member.end(); ++i0) {
		if (i0->first == role_id && i0->second->family_id() == family_id) {
			i0->second->set_tiger_count(iter->second._tiger_count);
			break;
		} 
	}

	return 0;
}

uint32 family_member_mgr::set_rosefinch_count(uint32 family_id, uint32 role_id, uint32 rosefinch_count)
{

	family_role::iterator i =  _family_role.find(family_id);
	if (i == _family_role.end()) {
		return -1;
	}

	role_permissions::iterator iter = i->second->find(role_id);
	if (iter == i->second->end()) {
		return -2;
	}

	iter->second._rosefinch_count += rosefinch_count;

	map_role_family_member::iterator i0 = _map_role_family_member.end();
	i0 = _map_role_family_member.find(role_id);
	for (; i0 != _map_role_family_member.end(); ++i0) {
		if (i0->first == role_id && i0->second->family_id() == family_id) {
			i0->second->set_rosefinch_count(iter->second._rosefinch_count);
			break;
		} 
	}

	return 0;
}

uint32 family_member_mgr::set_tortoise_count(uint32 family_id, uint32 role_id, uint32 tortoise_count)
{
	family_role::iterator i =  _family_role.find(family_id);
	if (i == _family_role.end()) {
		return -1;
	}

	role_permissions::iterator iter = i->second->find(role_id);
	if (iter == i->second->end()) {
		return -2;
	}

	iter->second._tortoise_count += tortoise_count;

	map_role_family_member::iterator i0 = _map_role_family_member.end();
	i0 = _map_role_family_member.find(role_id);
	for (; i0 != _map_role_family_member.end(); ++i0) {
		if (i0->first == role_id && i0->second->family_id() == family_id) {
			i0->second->set_tortoise_count(iter->second._tortoise_count);
			break;
		} 
	}


	return 0;
}

uint32 family_member_mgr::set_money(uint32 family_id, uint32 role_id, uint32 money)
{
	family_role::iterator i =  _family_role.find(family_id);
	if (i == _family_role.end()) {
		return -1;
	}

	role_permissions::iterator iter = i->second->find(role_id);
	if (iter == i->second->end()) {
		return -2;
	}

	iter->second._money += money;

	map_role_family_member::iterator i0 = _map_role_family_member.end();
	i0 = _map_role_family_member.find(role_id);
	for (; i0 != _map_role_family_member.end(); ++i0) {
		if (i0->first == role_id && i0->second->family_id() == family_id) {
			i0->second->set_money(iter->second._money);
			break;
		} 
	}

	return 0;
}

uint32 family_member_mgr::get_strength(uint32 family_id)
{
    role_permissions* p = get_role_permissions(family_id);
    if (p == NULL) {
        return 0;
    }

    uint32 strength = 0;
    for (role_permissions::iterator i = p->begin(); i != p->end(); ++i) {
        if (i->second._permissions >= MEMBER) {
            strength += i->second._strength;
        }
    }

    return strength;
}

uint32 family_member_mgr::set_total_donate(uint32 family_id, uint32 role_id, uint32 total)
{
	family_role::iterator i =  _family_role.find(family_id);
	if (i == _family_role.end()) {
		return -1;
	}

	role_permissions::iterator iter = i->second->find(role_id);
	if (iter == i->second->end()) {
		return -2;
	}

	iter->second._total_donate += total;

	map_role_family_member::iterator i0 = _map_role_family_member.end();
	i0 = _map_role_family_member.find(role_id);
	for (; i0 != _map_role_family_member.end(); ++i0) {
		if (i0->first == role_id && i0->second->family_id() == family_id) {
			i0->second->set_total_donate(iter->second._total_donate);
			break;
		} 
	}

	return 0;
}

uint32 family_member_mgr::get_member_total_donate(uint32 family_id, uint32 role_id)
{
	family_role_value* p = get_family_role(family_id, role_id);
	if (p == NULL) {
		return 0;
	}

	return p->_total_donate;
}
uint32 family_member_mgr::get_strength(uint32 family_id, uint32 role_id)
{
    family_role_value* p = get_family_role(family_id, role_id);
    if (p == NULL) {
        return 0;
    }

    return p->_strength;
}

int family_member_mgr::fill_role(uint32 role_id, client::family_self_rsp& rsp)
{
    rsp.set_family_id(0);
    rsp.set_permissions(0);
    rsp.set_time(0);
    rsp.set_add_by(0);
    
    map_role_family_member::iterator i0 = _map_role_family_member.end();
    i0 = _map_role_family_member.find(role_id);
    for (; i0 != _map_role_family_member.end(); ++i0) {
        if (i0->second->permissions() > INVITE) {
            rsp.set_family_id(i0->second->family_id());
            rsp.set_permissions(i0->second->permissions());
            rsp.set_time(i0->second->time());
            rsp.set_add_by(i0->second->add_by());
            break;
        }
    }

    return 0;
}
