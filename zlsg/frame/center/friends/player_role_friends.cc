#include "player_role_friends.h"

player_role_friends::player_role_friends(uint32 id) : self_role_id(id)
{
	db_role_friends = NULL;
    change = false;
}

player_role_friends::~player_role_friends()
{
	if (db_role_friends != NULL){
		delete db_role_friends;
		db_role_friends = NULL;
	}
}

int player_role_friends::set_db_friend(mysql::tb_role_friends* trf)
{
	if (trf == NULL) {
		return -1;
	}

	db_role_friends = trf;
    change = false;

	return parse_friend(trf->friends());
}

int player_role_friends::save(amtsvr_context* ctx)
{
    if (!change) {
        return -1;
    }

    if (db_role_friends == NULL){
        db_role_friends = new mysql::tb_role_friends;
        db_role_friends->set_role_id(self_role_id);
        writer_type = WOTE_INSERT;
    } else {
        writer_type = WOTE_UPDATE;
    }

    serialize_friend(*db_role_friends->mutable_friends());
    mysql::tb_role_friends* tf = new mysql::tb_role_friends(*db_role_friends);
    CACHE_KEY k(tf->role_id(), 0);
    write(ctx, k, tf);
    change = false;

    return 0;
}

void player_role_friends::add_friend(uint32 role_id,uint32 friend_level,string friend_name,uint32 vip_level)
{
	client::friend_info fi;
	std::map<uint32,client::friend_info>::iterator itr_1 = map_fi.find(role_id);
	fi.set_role_id(role_id);
	fi.set_degree(0);
	fi.set_friend_level(friend_level);
	fi.set_friend_name(friend_name);
	fi.set_vip_level(vip_level);

	if (itr_1 == map_fi.end()){
		fi.set_relation_type(friend_info_friend_type_real_friend);
	}else {
		fi.set_relation_type(friend_info_friend_type_temporary_and_real);
	}

	map_fi[role_id] = fi;

	vec_add_friends.push_back(fi);

    change = true;
}

void player_role_friends::update_friend_data(uint32 friend_id,uint32 role_level,uint32 role_vip_level)
{
	std::map<uint32,client::friend_info>::iterator itr_1 = map_fi.find(friend_id);

	if (itr_1 != map_fi.end()){
		if (itr_1->second.has_vip_level()){
			if (itr_1->second.vip_level() != role_vip_level){
				itr_1->second.set_vip_level(role_vip_level);
				change = true;
			}
		}else{
			itr_1->second.set_vip_level(role_vip_level);
			change = true;
		}

		if (itr_1->second.has_friend_level()){
			if (itr_1->second.friend_level() != role_level){
				itr_1->second.set_friend_level(role_level);
				change = true;
			}
		}else{
			itr_1->second.set_friend_level(role_level);
			change = true;
		}
	}
}

bool add_temporary_friend_sort(const client::friend_info& a,const client::friend_info& b)
{
	return a.contact_time() > b.contact_time();
}

bool player_role_friends::add_temporary_friend(uint32 role_id,uint32 friend_level,uint32 vip_level,string friend_name, bool& has_del)
{
	bool is_add = false;
	uint32 now = (uint32)time(NULL);
	client::friend_info fi;
	fi.set_role_id(role_id);
	fi.set_degree(0);
	fi.set_friend_level(friend_level);
	fi.set_friend_name(friend_name);
	fi.set_contact_time(now);
	fi.set_vip_level(vip_level);

	std::map<uint32,client::friend_info>::iterator itr_1 = map_fi.find(role_id);
	if (itr_1 == map_fi.end()){// 找不到，则加为临时好友
		fi.set_relation_type(friend_info_friend_type_temporary_friend);
		is_add = true;
	}
	else if (itr_1->second.relation_type() == friend_info_friend_type_real_friend){//若是好友
		fi.set_relation_type(friend_info_friend_type_temporary_and_real);
		is_add = true;
	}
	else
	{
		fi.set_relation_type(itr_1->second.relation_type());
	}

	map_fi[role_id] = fi;
	std::vector<client::friend_info> friend_vec;
	friend_vec.clear();
	std::map<uint32,client::friend_info>::iterator itr = map_fi.begin();
	for (;itr != map_fi.end(); ++itr){
		if (itr->second.relation_type() != friend_info_friend_type_real_friend){
			// 对比最近联系时间
			friend_vec.push_back(itr->second);
		}
	}
	// 对最近联系的时间排序，删除
	if (friend_vec.size() != 0 && friend_vec.size() > 5){
		std::sort(friend_vec.begin(),friend_vec.end(),add_temporary_friend_sort);
		for (size_t i = 5; i < friend_vec.size();++i){
			del_temporary_friend(friend_vec[i].role_id());
			has_del= true;
		}
	}

	if (is_add){
		vec_add_friends.push_back(fi);
	}

	change = true;

	return true;
}

bool player_role_friends::del_friend(uint32 role_id)
{
	map<uint32, client::friend_info>::iterator it = map_fi.find(role_id);
	if (it == map_fi.end()){
		return false;
	}
	if (it->second.relation_type() == friend_info_friend_type_real_friend){
		vec_del_friends.push_back(it->second);
		map_fi.erase(it);
	}
	else if (it->second.relation_type() == friend_info_friend_type_temporary_and_real){
		it->second.set_relation_type(friend_info_friend_type_temporary_friend);
		vec_tem_del_friends.push_back(it->second);
	}
    change = true;
	return true;
}

bool player_role_friends::del_temporary_friend(uint32 role_id)
{
	std::map<uint32, client::friend_info>::iterator it = map_fi.find(role_id);
	if (it == map_fi.end()){
		return false;
	}
	else if (it->second.relation_type() == friend_info_friend_type_temporary_friend)
	{
		vec_del_friends.push_back(it->second);
		map_fi.erase(it);
	}
	else if (it->second.relation_type() == friend_info_friend_type_temporary_and_real){
		it->second.set_relation_type(friend_info_friend_type_real_friend);
	}

	return true;
}
void player_role_friends::blk_friend(uint32 role_id)
{
    map_fi[role_id].set_role_id(role_id);
    map_fi[role_id].set_degree((uint32)-1);
    vec_blk_friends.push_back(map_fi[role_id]);
    change = true;
}

int player_role_friends::add_degree(uint32 dst_role_id, uint32 val)
{
    map<uint32, client::friend_info>::iterator it = map_fi.find(dst_role_id);
    if (it == map_fi.end()){
        return -1;
    }

    uint32 new_val = it->second.degree() + val;
    if (new_val >= 0xFFFFFFFE){
        new_val = 0xFFFFFFFD;
    }
    it->second.set_degree(new_val);
    change = true;

    return 0;
}

uint32 player_role_friends::get_degree(uint32 dst_role_id)
{
    map<uint32, client::friend_info>::iterator it = map_fi.find(dst_role_id);
    if (it == map_fi.end()){
        return 0;
    }

    return it->second.degree();
}

const map<uint32, client::friend_info> player_role_friends::get_friend_list() const
{
	return get_list();
}

void player_role_friends::fill_add_friend_info(client::friend_list_rsp& flr)
{
	flr.set_list_type(client::FLRE_ADD);
	uint32 size = vec_add_friends.size();
	for (uint32 i = 0; i < size; ++i){
		*flr.add_fi() = vec_add_friends[i];
	}
	vec_add_friends.clear();
}

void player_role_friends::fill_del_friend_info(client::friend_list_rsp& flr)
{
	flr.set_list_type(client::FLRE_DEL);
	uint32 size = vec_del_friends.size();
	for (uint32 i = 0; i < size; ++i){
		*flr.add_fi() = vec_del_friends[i];
	}
	vec_del_friends.clear();
	
}

void player_role_friends::fill_del_tem_friend_info(client::friend_list_rsp& flr)
{
	flr.set_list_type(client::FLRE_CHA);
	uint32 size = vec_tem_del_friends.size();
	for (uint32 i = 0; i < size; ++i){
		*flr.add_fi() = vec_tem_del_friends[i];
	}
	vec_tem_del_friends.clear();
}

void player_role_friends::fill_blk_friend_info(client::friend_list_rsp& flr)
{
	flr.set_list_type(client::FLRE_BLK);
	uint32 size = vec_blk_friends.size();
	for (uint32 i = 0; i < size; ++i){
		*flr.add_fi() = vec_blk_friends[i];
	}
	vec_blk_friends.clear();
}

void player_role_friends::set_contact_time(uint32 role_id, uint32 time)
{
    map<uint32, client::friend_info>::iterator i = map_fi.find(role_id);
    if (i == map_fi.end()) {
        return ;
    }
    i->second.set_contact_time(time);
    change = true;
}

bool player_role_friends::in_black_list(uint32 role_id)
{
    map<uint32, client::friend_info>::iterator i = map_fi.find(role_id);
    if (i == map_fi.end()) {
        return false;
    }
    
    return i->second.degree() == (uint32)-1;
}
