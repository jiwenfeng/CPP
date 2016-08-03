#ifndef __SCENE_ROBOT_H__
#define __SCENE_ROBOT_H__
#include "scene_npc.h"
#include "game_map_mgr.h"

class scene_robot : public scene_npc
{
public:
    scene_robot(const origin_npc& n, const uint64 id);
    ~scene_robot();

public:
	int init_robot(const client::robot_attr *ra);
    int init_robot_skill();
    int re_init_attr(const client::robot_attr *ra);
	int call_backup();
	int call_pet();
    void attach_config_npc(config_npc *npc) { _cfg = npc; }
    void attach_origin_npc(origin_npc *o) { _ori = o ; }
	void rule_out_skill();
	void set_is_mirror(bool b) { _mirror = b; }
	void attach_pet(const client::pet_db_info &gi);
	void attach_backup(const client::pet_db_info &gi);
	bool is_mirror() { return _mirror; }

public:
    virtual void fill_role_data(client::role_data& rd) const;
    virtual void fill_role_data(client::role_data* rd) const;
    virtual void on_dead(uint32 tick);
	virtual bool is_robot() { return true; }
	virtual const char *get_role_name() const  { return role::get_role_name(); }

private:
	int generate_pet(client::pet_db_info *pet);
	void set_backup_status(bool status) { _status = status; }
	bool has_backup() { return _status; }
	void init_pet_skill(const client::pet_db_info &pet, bool flag);

private:
    config_npc *_cfg;
    origin_npc *_ori;
	client::pet_db_info *_pet;
	client::pet_db_info *_backup;
	map<uint32, vector<skill> > _pet_skill;
	bool _mirror;
	bool _status;
};

#endif
