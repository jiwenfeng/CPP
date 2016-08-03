#ifndef __ALL_USER_SMY_MGR_H__
#define __ALL_USER_SMY_MGR_H__

#include <iostream>
#include "pb_mgr.h"
#include "client.offline.pb.h"

using namespace std;

class role_smy_mgr
{
public:
    role_smy_mgr();

    virtual ~role_smy_mgr();

public:
    int init();

    const mysql::sp_load_all_user_smy* get_user_smy(const string& name) const;
    mysql::sp_load_all_user_smy* mutable_user_smy(const string& name);
	const mysql::sp_load_all_user_smy* get_user_smy(uint32 role_id) const;
    mysql::sp_load_all_user_smy* mutable_user_smy(uint32 role_id);
    
    const std::map<uint32, mysql::sp_load_all_user_smy*>& get_all() const { return id_user_smys; }

public:
    void record(mysql::sp_load_all_user_smy& smy);
	int update_mount_info(uint32 role_id,uint32 mount_level,uint32 mount_power);
	int update_vip_level(uint32 role_id, uint32 vip_lv);
    int update_lvl(uint32 role_id, uint32 new_lvl);
	int update_weapon_id(uint32 role_id, uint32 weapon_id);
	int update_armor_id(uint32 role_id, uint32 armor_id);
	int update_fight_power(uint32 role_id, uint32 fight_power);
	uint32 get_lvl(uint32 role_id);
	int update_user_offline_time(uint32 role_id, uint32 t);
	void get_offline_user(client::offline_load_image *req, vector<uint32> &v);

public:
	void pack_role_smy_info(const mysql::sp_load_all_user_smy* smy, client::role_smy_info& rsi);
	void get_users_greater_lvl(vector<uint32> &v, uint32 lvl, uint32 power, const vector<uint32> &except);

protected:
    map<string, mysql::sp_load_all_user_smy*> name_user_smys;
	map<uint32, mysql::sp_load_all_user_smy*> id_user_smys; 
};

#endif
