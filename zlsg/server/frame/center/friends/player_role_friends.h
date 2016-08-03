#ifndef __PLAYER_ROLE_FRIENDS_H__
#define __PLAYER_ROLE_FRIENDS_H__

#include "define.h"
#include "pb_mgr.h"
#include "friend_parser.h"
#include "pb_writer.h"

#define MAX_FRIENDS_NUM 200

class player_role_friends : public friend_parser, public pb_writer
{
public:
	player_role_friends(uint32 id);

	virtual ~player_role_friends();

	int set_db_friend(mysql::tb_role_friends* trf);

    int save(amtsvr_context* ctx);

    inline bool get_change() const { return change; }
    inline void set_change(bool val = true) { change = val; }

public:

	bool is_friends_num_limit() const {return map_fi.size() >= MAX_FRIENDS_NUM;}

	inline bool has_friend(uint32 role_id) const{
		std::map<uint32, client::friend_info>::const_iterator it = map_fi.find(role_id);
		if (it == map_fi.end()){
			return true;
		}else {
			if (it->second.relation_type() == friend_info_friend_type_real_friend || it->second.relation_type() == friend_info_friend_type_temporary_and_real){
				return false;
			}
		}

		return true;
	}

	inline bool has_temporary_friend(uint32 role_id) const{
		std::map<uint32, client::friend_info>::const_iterator it = map_fi.find(role_id);
		if (it	!= map_fi.end()){
			if (it->second.relation_type() == friend_info_friend_type_temporary_and_real || it->second.relation_type() == friend_info_friend_type_temporary_friend){
				return false;
			}
		}
		return true;
	}

	void add_friend(uint32 role_id,uint32 friend_level,string friend_name,uint32 vip_level);

	// 私聊的最近联系人（临时好友）
	bool add_temporary_friend(uint32 role_id,uint32 friend_level,uint32 vip_level,string friend_name,bool& has_del);

	bool del_friend(uint32 role_id);

	bool del_temporary_friend(uint32 role_id);

    void blk_friend(uint32 role_id);

	void fill_add_friend_info(client::friend_list_rsp& flr);

	void fill_del_friend_info(client::friend_list_rsp& flr);

	void fill_del_tem_friend_info(client::friend_list_rsp& flr);

    void fill_blk_friend_info(client::friend_list_rsp& flr);

    void set_contact_time(uint32 role_id, uint32 time);

    void set_enemy(uint32 role_id);

    map<uint32, client::friend_info>* get_friend() { return &map_fi; }

    bool in_black_list(uint32 role_id);

	inline uint32 get_vec_del_friends_size(){return vec_del_friends.size();}

	inline uint32 get_vec_vec_tem_del_friends_size(){return vec_tem_del_friends.size();}
    // 增加好友度
    int add_degree(uint32 dst_role_id, uint32 val);

    uint32 get_degree(uint32 dst_role_id);

	const map<uint32, client::friend_info> get_friend_list() const;

	void update_friend_data(uint32 friend_id,uint32 role_level,uint32 role_vip_level);
protected:
    uint32 self_role_id;
	mysql::tb_role_friends* db_role_friends;
	vector<client::friend_info> vec_add_friends;
	vector<client::friend_info> vec_del_friends;
    vector<client::friend_info> vec_blk_friends;
	vector<client::friend_info> vec_tem_del_friends;
    bool change;
};

#endif
