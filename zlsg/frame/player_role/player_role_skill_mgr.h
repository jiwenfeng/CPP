#ifndef __PLAYER_ROLE_SKILL_MGR_H__
#define __PLAYER_ROLE_SKILL_MGR_H__

#include "player_role_skill.h"
#include "config_skill.h"

class player_role_skill_mgr
{
public:
    player_role_skill_mgr();
    ~player_role_skill_mgr();

public:
    int attach_role_skill(uint32 role_id,int pb_count, cmq_svr_pb_ctx* pb_ctx);
    int init_new_role_skill(uint32 role_id, uint32 job_id);

    int check_skill(uint32 role_id, uint32 profession, uint32 level);

public:
    player_role_skill* get_skill(uint32 skill_id,uint32 ser_index = 0);
    std::vector<player_role_skill>& get_skills() { return role_skill_list; }
    inline const bool has_skill_data() const { return role_skill_list.empty(); }

    void write(amtsvr_context* ctx);
	inline bool is_gem_skill(uint32 skill_belong){ return skill_belong == SB_GEM   ;};
	inline bool is_bow_skill(uint32 skill_belong){ return skill_belong == SB_ARCHER;};
    bool add_skill(uint32 role_id, uint32 skill_id,uint32 ser_index = 0);
    bool add_skill(uint32 role_id, const config_skill* pcs,uint32 ser_index = 0);
    bool add_skill(uint32 role_id, const client::skill_data& skill);
    bool delete_skill(uint32 skill_id,uint32 ser_index = 0);
	bool check_old_skill();
	bool upgrade_skill_effect(uint32 skill_id,uint32 add_level = 1);
	bool set_skill_effect(uint32 skill_id,uint32 effect_id,uint32 level,uint32 ser_index = 0);
	bool upgrade_mount_skill_effect(uint32 skill_id);
    void set_default_skill(uint32 skill_id);
	uint32 get_bows_skill_count();
	uint32 create_skill_info(player_role_skill& prs,uint32 role_id,const config_skill* skill_base,uint32 ser_index = 0);
	uint32 get_skill_effect_level(uint32 skill_id,uint32 ser_index = 0);
public:
    void fill_role_skill(client::role_skill_msg& rsm) const;
    void fill_role_skill(client::skills* sm) const;
public:
	// 组装技能系统ID
	inline void set(uint32 sid, uint32 fid) {
		_role_id    = sid;
		smy_id    = fid;
	}

	inline uint32 get() {
		uint32 id = smy_id + 1;
		if (id == 0) {
			return 0;
		}

		++smy_id;

		return smy_id;
	}

	inline uint32 get_skill_smy_id() const { return smy_id; }

	int  init_skill_sid(uint32 role_id);

	bool generate_skill_smy_id(uint32 role_id,uint32& fid);

	int set_skill_smy_id(const mysql::tb_role_skill& db_skil);
protected:
    std::vector<player_role_skill> role_skill_list;
    std::vector<player_role_skill> del;
private:
	uint32 _role_id;
	uint32 smy_id;
};

#endif
