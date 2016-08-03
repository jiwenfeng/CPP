#ifndef __FRIENDS_MGR_H__
#define __FRIENDS_MGR_H__

#include "player_role_friends.h"

class friends_mgr 
{
public:
	friends_mgr();

	virtual ~friends_mgr();

	int init();

    int save(amtsvr_context* ctx);

public:
	const player_role_friends* get(uint32 role_id) const;

	player_role_friends* mut(uint32 role_id);

	bool can_add_friend(uint32 src_role_id, uint32 dst_role_id);

	bool is_aready_friend(uint32 src_role_id, uint32 dst_role_id);

	int add_friend(uint32 src_role_id, uint32 dst_role_id,uint32 friend_level,string friend_name,uint32 vip_level);

	int add_temporary_friend(uint32 src_role_id, uint32 dst_role_id,uint32 friend_level,string friend_name,uint32 vip_level,client::friend_list_rsp& rsp_msg,bool& has_del,client::friend_list_rsp& del_rsp);

	int del_friend(uint32 src_role_id, uint32 dst_role_id);

    int blk_friend(uint32 src_role_id, uint32 dst_role_id);

    int add_degree(uint32 src_role_id, uint32 dst_role_id, uint32 val, uint32& src_ret, uint32& dst_ret); // Ôö¼ÓÇ×ÃÜÖµ

    uint32 get_degree(uint32 src_role_id, uint32 dst_role_id);

private:
	player_role_friends* create(uint32 role_id);

private:
	map<uint32, player_role_friends*> map_roles; // role_id-->player_role_friends
    map<uint32, player_role_friends*> changes;
};


#endif
