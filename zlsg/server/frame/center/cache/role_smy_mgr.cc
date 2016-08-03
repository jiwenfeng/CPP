#include "role_smy_mgr.h"
#include "config_mgr.h"

role_smy_mgr::role_smy_mgr()
{
}

role_smy_mgr::~role_smy_mgr()
{
    std::map<uint32, mysql::sp_load_all_user_smy*>::iterator i = id_user_smys.begin();
    while (i != id_user_smys.end()) {
        delete i->second;
        ++i;
    }
    id_user_smys.clear();
    name_user_smys.clear();
}

int role_smy_mgr::init()
{
    hash_map_obj<CACHE_KEY, cfg_cache_obj, CFG_CACHE_BULB_NUM>::pair_t* cur = (hash_map_obj<CACHE_KEY, cfg_cache_obj, CFG_CACHE_BULB_NUM>::pair_t*)CONFMGR->get_all_user_smy_head();
    while (cur != NULL){
        mysql::sp_load_all_user_smy* smy = static_cast<mysql::sp_load_all_user_smy*>(cur->mutable_val().pop_msg());
        cur = (hash_map_obj<CACHE_KEY, cfg_cache_obj, CFG_CACHE_BULB_NUM>::pair_t*)cur->get_next();
        if (smy == NULL){
            continue;
        }

        /*if (smy->sex() == 0 || smy->profession() == 0) {
            printf("%d, %s, %d, %d\n", smy->role_id(), smy->role_name().c_str(), smy->sex(), smy->profession());
        }*/

        name_user_smys[smy->role_name()] = smy;
		id_user_smys[smy->role_id()] = smy;

    }


    return 0;
}

int role_smy_mgr::update_lvl(uint32 role_id, uint32 new_lvl)
{
    map<uint32, mysql::sp_load_all_user_smy*>::const_iterator it = id_user_smys.find(role_id);
	if (it == id_user_smys.end()){
		return -1;
	}
    
	it->second->set_level(new_lvl);

    return 0;
}

int role_smy_mgr::update_weapon_id(uint32 role_id, uint32 weapon_id)
{

	map<uint32, mysql::sp_load_all_user_smy*>::const_iterator it = id_user_smys.find(role_id);
	if (it == id_user_smys.end()){
		return -1;
	}

	it->second->set_weapon_id(weapon_id);

	return 0;
}

int role_smy_mgr::update_armor_id(uint32 role_id, uint32 armor_id)
{
	map<uint32, mysql::sp_load_all_user_smy*>::const_iterator it = id_user_smys.find(role_id);
	if (it == id_user_smys.end()){
		return -1;
	}

	it->second->set_armor_id(armor_id);

	return 0;
}

int role_smy_mgr::update_fight_power(uint32 role_id, uint32 fight_power)
{
	map<uint32, mysql::sp_load_all_user_smy*>::const_iterator it = id_user_smys.find(role_id);
	if (it == id_user_smys.end()){
		return -1;
	}

	it->second->set_fighting_power(fight_power);

	return 0;
}

uint32 role_smy_mgr::get_lvl(uint32 role_id)
{
	map<uint32, mysql::sp_load_all_user_smy*>::const_iterator it = id_user_smys.find(role_id);
	if (it == id_user_smys.end()){
		return -1;
	}

	return it->second->level();
}

const mysql::sp_load_all_user_smy* role_smy_mgr::get_user_smy(const string& name) const
{
    map<string, mysql::sp_load_all_user_smy*>::const_iterator it = name_user_smys.find(name);
    if (it == name_user_smys.end()){
        return NULL;
    }
    return it->second;
}

mysql::sp_load_all_user_smy* role_smy_mgr::mutable_user_smy(const string& name)
{
    map<string, mysql::sp_load_all_user_smy*>::iterator it = name_user_smys.find(name);
    if (it == name_user_smys.end()){
        return NULL;
    }
    return it->second;
}

const mysql::sp_load_all_user_smy* role_smy_mgr::get_user_smy(uint32 role_id) const
{
	map<uint32, mysql::sp_load_all_user_smy*>::const_iterator it = id_user_smys.find(role_id);
	if (it == id_user_smys.end()){
		return NULL;
	}
	return it->second;
}

mysql::sp_load_all_user_smy* role_smy_mgr::mutable_user_smy(uint32 role_id)
{
	map<uint32, mysql::sp_load_all_user_smy*>::iterator it = id_user_smys.find(role_id);
	if (it == id_user_smys.end()){
		return NULL;
	}
	return it->second;
}

void role_smy_mgr::record(mysql::sp_load_all_user_smy& smy)
{
    mysql::sp_load_all_user_smy* s = mutable_user_smy(smy.role_name());
    if (s != NULL) {
        s->CopyFrom(smy);
		return ;
    }

//    s = mutable_user_smy(smy.role_id());
//    if (s != NULL) {
//        s->CopyFrom(smy);
//        
//        return ;
//    }

    mysql::sp_load_all_user_smy* new_smy = new mysql::sp_load_all_user_smy(smy);
    name_user_smys[smy.role_name()] = new_smy;
	id_user_smys[smy.role_id()] = new_smy;
}

int role_smy_mgr::update_vip_level(uint32 role_id, uint32 lv)
{
	map<uint32, mysql::sp_load_all_user_smy*>::const_iterator it = id_user_smys.find(role_id);
	if (it == id_user_smys.end()){
		return -1;
	}
	it->second->set_vip_level(lv);
	return 0;
}

int role_smy_mgr::update_mount_info(uint32 role_id,uint32 mount_level,uint32 mount_power)
{
	map<uint32, mysql::sp_load_all_user_smy*>::const_iterator it = id_user_smys.find(role_id);
	if (it == id_user_smys.end()){
		return -1;
	}

	it->second->set_mount_level(mount_level);

	it->second->set_mount_power(mount_power);
	return 0;
}

int role_smy_mgr::update_user_offline_time(uint32 role_id, uint32 t)
{
	map<uint32, mysql::sp_load_all_user_smy *>::iterator it = id_user_smys.find(role_id);
	if(it == id_user_smys.end()) {
		return -1;
	}
	it->second->set_offline_time(t);
	return 0;
}

void role_smy_mgr::get_offline_user(client::offline_load_image *req, vector<uint32> &v)
{
	map<uint32, mysql::sp_load_all_user_smy *>::iterator it;
	uint32 now = (uint32)time(NULL);
	for(it = id_user_smys.begin(); it != id_user_smys.end(); ++it)
	{
		int flag = false;
		if(now - it->second->offline_time() > req->limit_tl())
		{
			continue;
		}
		if(it->second->level() < req->limit_lv())
		{
			continue;
		}
		if(it->second->fighting_power() < req->limit_power())
		{
			continue;
		}
		for(int i = 0; i < req->role_size(); ++i)
		{
			if(req->role(i) == it->first)
			{
				flag = true;
				break;
			}
		}
		if(flag)
		{
			continue;
		}
		v.push_back(it->first);
	}
}

void role_smy_mgr::pack_role_smy_info(const mysql::sp_load_all_user_smy* smy, client::role_smy_info& rsi)
{
	rsi.set_role_id(smy->role_id());
	rsi.set_role_name(smy->role_name());
	rsi.set_fight_power(smy->fighting_power());
	rsi.set_profession(smy->profession());
    //rsi.set_online(smy->status() >= 1);
    rsi.set_level(smy->level());
	rsi.set_armor_id(smy->armor_id());
	rsi.set_weapon_id(smy->weapon_id());
	rsi.set_mount_level(smy->mount_level());
	rsi.set_mount_power(smy->mount_power());
	rsi.set_vip_level(smy->vip_level());
}

void role_smy_mgr::get_users_greater_lvl(vector<uint32> &v, uint32 lvl, uint32 power, const vector<uint32> &e)
{
	map<uint32, mysql::sp_load_all_user_smy*>::iterator it;
	for(it = id_user_smys.begin(); it != id_user_smys.end(); ++it)
	{
		if(it->second->level() >= lvl && it->second->fighting_power() >= power && (find(e.begin(), e.end(), it->first) == e.end()))
		{
			v.push_back(it->first);
		}
	}
}

