#ifndef __FAMILY_MGR_H__
#define __FAMILY_MGR_H__

#include <map>
#include "comm_define.h"
#include "family.h"
#include "family_member_mgr.h"
#include "family_log_mgr.h"
#include "family_skill_mgr.h"
#include "family_record_mgr.h"
#include "family_domain_mgr.h"
#include "family_diplomacy_mgr.h"

#define DEFAULT_FLAG_ID 1

class family_fsid
{
public:
    family_fsid() {
        svr_id    = 0;
        family_id = 0;
    }

    virtual ~family_fsid() {

    }

    inline void set(uint32 sid, uint32 fid) {
        svr_id    = sid;
        family_id = fid;
    }

    inline uint32 get() {
        uint32 id = family_id + 1;
        if (id == 0 || id > FAMILY_ID_MASK) {
            return 0;
        }

        ++family_id;

        return PACK_SVR_FAMILY_ID(svr_id, family_id);
    }

    inline uint32 get_family_id() const { return family_id; }
private:
    uint32 svr_id;
    uint32 family_id;
};

class family_mgr
{
public:
    family_mgr();
    virtual ~family_mgr();

public:
    int init(struct cmq_t* l);

    int save(amtsvr_context* ctx);

public:
	bool check_family(uint32 family_id,uint32 role_id);
	// 军团列表
    int family_list(uint32 city_family,client::family_list_req* req, client::family_list_rsp& rsp);
	int family_title_list(client::family_title_list_rsp& rsp);
	int family_title(uint32 family_id ,client::family_title_list_rsp& rsp);
    int family_info(client::family_req* req, client::family_rsp& rsp);
    int family_info(uint32 family_id, client::donate_family_rsp& rsp);
    int create_family(client::create_family_req* req, const std::string create_name,client::create_family_rsp& rsp, uint32& family_id,uint32 create_power);
    int apply_family(client::apply_family_req* req, client::apply_family_rsp& rsp,uint32 fight_power,client::approval_family_rsp& rsp_msg);
    int apply_family_list(client::apply_family_list_req* req, client::apply_family_list_rsp& rsp);
    int apply_family_list_self(client::apply_family_list_self_req* req, client::apply_family_list_self_rsp& rsp);
    int approval_family(client::approval_family_req* req, client::approval_family_rsp& rsp,uint32 fight_power);
	int reject_family(client::approval_family_req* req, client::approval_family_rsp& rsp);
	// 邀请进团
    int invite_family(client::invite_family_req* req, client::invite_family_rsp& rsp);
    int invite_family_list(client::invite_family_list_req* req, client::invite_family_list_rsp& rsp);
    int agree_family(client::agree_family_req* req, client::agree_family_rsp& rsp, uint32& referrer,uint32 fight_power);
	// 自动退团
	int secede_family(client::secede_family_to_center* req, client::secede_family_rsp& rsp,uint32 fight_power);
	// 逐出某人出军团
	int expel_member(client::expel_member_req* req, client::expel_member_rsp& rsp);
    int modify_family_post(client::modify_family_post_req* req, client::modify_family_post_rsp& rsp);
    int family_member_list(client::family_member_list_req* req, client::family_member_list_rsp& rsp);
	int family_member_list(uint32 family_id, vector<uint32>& v);
    int modify_family_member(client::modify_family_member_req* req, client::modify_family_member_rsp& rsp,uint32 fight_power,string role_name,uint32& be_change_id);
	// 解散军团
    int disband_family(client::disband_family_req* req, client::disband_family_rsp& rsp);
    int donate_family(client::set_family_money* req,client::set_donate_consume& rsp_msg);
    int upgrade_family(client::upgrade_family_req* req, client::upgrade_family_rsp& rsp);
    int family_self(client::family_self_req* req, client::family_self_rsp& rsp);
    int family_log(client::family_log_req* req, client::family_log_rsp& rsp);
    //int family_skills(client::family_skills_req* req, client::family_skills_rsp& rsp);
    //int family_skills(uint32 family_id, client::family_skills_rsp& rsp);
    //int family_skill_upgrade(client::family_skill_upgrade_req* req, client::family_skill_upgrade_rsp& rsp);
    int family_record(client::family_record_req* req, client::family_record_rsp& rsp);
    //int family_monster_boss(client::family_monster_boss_req* req, client::family_monster_boss_rsp& rsp);
   // int family_monster_boss_set(client::family_monster_boss_set_req* req, client::family_monster_boss_set_rsp& rsp);
    //int family_monster_boss_call_jump(client::family_monster_boss_call_jump_req* req, client::family_monster_boss_call_jump_rsp& rsp);
	int family_set(client::family_set_req* req, client::family_set_rsp& rsp);
	int change_family_flag_id(client::change_family_flag_id_req *req,client::change_family_flag_id_rsp& rsp_msg);
	int change_family_flag_name(client::change_family_flag_name_req *req,client::change_family_flag_name_rsp& rsp_msg);
	int impeach_chief(client::impeach_chief_req *req,client::impeach_chief_rsp& rsp_msg,string role_name);
	int donate_list(client::donate_list_req *req,client::donate_list_rsp& rsp_msg);
	int ally_family(client::ally_family_req *req,client::ally_family_rsp& rsp_msg);
	int remove_foreign(client::remove_foreign_req *req,client::remove_foreign_rsp& rsp_msg,bool& is_remove_ally);
	int ally_family_list(client::ally_family_list_req *req,client::ally_family_list_rsp& rsp_msg);
public:
    int family_log_update(client::family_log_update* req);
    int modify_family_power(client::modify_family_power* req);
	// 只对新创建的创建者军团战力增加还有新入团人的战力添加
	int add_family_info_power(uint32 family_id,uint32 role_id,uint32 add_power);
	// 只对退出军团或者逐出军团时候军团战斗力减少的处理
	int del_family_info_power(uint32 family_id,uint32 del_power);
    //int family_collect_power(client::family_collect_power_rsp* rsp);
    int family_record_update(client::family_record_update* req);
    //int family_domain_update(client::family_domain_update* req);
    
public:
    family* get_family(uint32 family_id);
    family* get_family_by_role_id(uint32 role_id);
	int   fill_smy_family_base_data(const mysql::sp_load_all_user_smy* smy, client::role_smy_info& rsi);
	int   fill_query_family_base(uint32 role_id,client::role_query_info& role_info);
    uint32 get_chief(uint32 family_id);
	const string &get_chief_name(uint32 family_id);
	void   get_diplomacy_relation(uint32 family_id,uint32 relation_type,vector<uint32>& vec_family_id);
	uint32 get_family_relation_type(uint32 f1, uint32 f2) { return _diplomacy_mgr.get_relation_type(f1, f2); }
    int    set_chief(uint32 family_id, uint32 role_id,string role_name);
    int    get_member(uint32 family_id, std::vector<uint32>& member);
    int    get_manager(uint32 family_id, std::vector<uint32>& manager);
	int	   get_manager(uint32 family_id, std::vector<struct manager_info>&manager);
    int    get_manager_add(uint32 family_id, std::vector<uint32>& manager);
    uint32 get_family_id(uint32 role_id, uint32* sect_id = NULL);
    uint32 get_sect_id(uint32 role_id);
    uint32 get_family_title(uint32 role_id, uint32 family_id);
    uint32 get_family_power(uint32 family_id);
	uint32 get_family_money(uint32 role_id);
    uint32 get_domain_num(uint32 family_id);
    uint32 get_domain_owner(uint32 scene_id);
    multimap<uint32, family_domain*>& get_domain_scene();
    uint32 get_op_tag(uint32 family_id);
    uint32 set_op_tag(uint32 family_id, uint32 op_tag);
    uint32 get_strength(uint32 family_id, uint32 role_id);
	uint32 get_flag_id(uint32 family_id);
	uint32 get_join_time(uint32 family_id, uint32 role_id);
	const string& get_family_name(uint32 family_id);
	family_role_value*    get_member_info(uint32 family_id,uint32 role_id); 
	void   mod_family_money(uint32 family_id, uint32 money);
    void   set_strength(uint32 family_id, uint32 role_id, uint32 strength);
    void   record_delete_all();
	int trans_permission_to_index(uint32 permission);
	int get_family_title_index(uint32 role_id, uint32 family_id);
	uint32 get_member_donate(uint32 family_id,uint32 role_id);
	uint32 get_family_level(uint32 family_id);
	uint32 clear_famliy_apply_relation();
public:
    family_domain_mgr& get_domain_mgr() {return _domain_mgr;}

public:
    int fill_family_info(client::family_info* fi);
//	int fill_family_info(client::load_family_list_rsp &rsp);

	const map<uint32, family*>& get_family_list() const{return _family;}
private:
    bool check_name(const string& name);
    bool check_flag_name(const string& name);
    bool check_permissions_num(uint32 family_id, uint32 permission);
    void remove(uint32 family_id);

    int  init_family_fsid();
    int  set_family_fsid(const mysql::tb_family& tf);
    bool generate_family_id(uint32 svr_id, uint32& fid);

    int reset_family_power(uint32 family_id);
    int reset_family_power(family* f);

    void insert_lvl_to_family(uint32 id, uint32 lvl);
    void update_lvl_to_family(uint32 id, uint32 old_lvl, uint32 new_lvl);
    void remove_lvl_to_family(uint32 id, uint32 lvl);

public:
    static const uint32 FAMILY_SCENE_ID = 351;

private:
    map<uint32, family*>      _family;
    map<uint32, family*>      _remove;
    family_member_mgr         _member_mgr;
    family_log_mgr            _log_mgr;
    family_skill_mgr          _skill_mgr;
    family_record_mgr         _record_mgr;
    family_domain_mgr         _domain_mgr;
	family_diplomacy_mgr	  _diplomacy_mgr;
    map<uint32, family_fsid>  _fsids; // svr_id-->family_fsid
};

#endif /* __FAMILY_MGR_H__ */

