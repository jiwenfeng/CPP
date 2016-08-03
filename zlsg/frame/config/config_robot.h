#ifndef __CONFIG_ROBOT_H__
#define __CONFIG_ROBOT_H__

#include "cfg_cache_mgr.h"
#include "cfg_parser.h"
#include "pb_mgr.h"
#include "config_base.h"

class config_robot {
public:
    config_robot() {}
    ~config_robot() {}

public:
    inline const uint32 get_robot_level() const { return _robot_level; }
    inline const uint32 get_robot_name() const { return _robot_name; }
	inline const uint32 get_robot_job()	const { return _robot_job; }
	inline const uint32 get_robot_combat() const { return _robot_combat; }
	inline const uint32 get_skill_combat() const { return _skill_combat; }
	inline const string& get_robot_ai() const { return _robot_ai; }
    inline const std::map<int, int>& get_game_attribute() const { return _game_attribute;}
	inline const uint32 get_mount_id() const { return _mount_id; }
	inline const uint32 get_mount_skill() const { return _mount_skill; }
	inline const uint32 get_weapon_id() const { return _weapon_id; }
	inline const uint32 get_armor_id() const { return _armor_id; }
	inline const uint32 get_general_id() const {return _general_id; }
	inline const uint32 get_general_level() const { return _general_level ;}
	inline const uint32 get_standby_pet_id() const { return _standby_pet_id; }
	inline const uint32 get_standby_pet_level() const { return _standby_pet_level; }
	inline const std::vector<uint32> &get_skills() const {return _skills; }
	inline const uint32 get_robot_percent() const { return _percent; }
	inline const uint32 get_attr_float_min() const { return _min; }
	inline const uint32 get_attr_float_max() const { return _max; }
	inline const uint32 get_robot_config_id() const { return _config_id; }
	inline const uint32 get_magic_star() const { return 0; }
	inline const map<int, int> &get_robot_attr() const { return _game_attribute; }

public:
	uint32					 _config_id			  ;
    uint32                   _robot_level         ;
    uint32                   _robot_name          ;
	uint32					 _robot_job			  ;
	uint32					 _robot_combat		  ;
	uint32					 _skill_combat		  ;
    string                   _robot_ai			  ;
    std::map<int, int>       _game_attribute      ;
	uint32					 _mount_id			  ;
	uint32					 _mount_skill		  ;
	uint32					 _weapon_id			  ;
	uint32					 _armor_id			  ;
	uint32					 _general_id		  ;
	uint32					 _general_level		  ;
	uint32					 _standby_pet_id	  ;
	uint32					 _standby_pet_level	  ;
	uint32					 _percent			  ;
	uint32					 _min				  ;
	uint32					 _max				  ;
	uint32					 _attack_cool		  ;
	uint32					 _attack_mode		  ;
	uint32					 _corpse_remain_time  ;
	uint32					 _visible_range		  ;
	uint32					 _follow_range		  ;
	uint32					 _monster_type		  ;
	uint32					 _default_skill		  ;
	uint32					 _random_move_range	  ;
	std::map<int, int>		 _g_game_attribute	  ;
public:
    std::vector<uint32>      _skills;
};

class config_robot_mgr : public config_base_mgr
{
public:
	config_robot_mgr(TTableGroup_s* p = NULL);
	~config_robot_mgr(void);

public:
    int init(TTableGroup_s* p);
    int clear();
	inline const std::map<uint32, config_robot *> & get_config_robots() const { return _cb; }
	const config_robot* get_config_robot(uint32 id) const;
	string get_robot_ai(uint32 id) const;
	const std::vector<uint32> &get_skills(uint32 id) const;

private:
    TTableGroup_s*                  _robot;
    TTable*                       _table;
	std::map<uint32, config_robot *> _cb;
};

#endif // __config_robot_H__
