#ifndef __CONFIG_TARGET_H__
#define __CONFIG_TARGET_H__

#include "cfg_cache_mgr.h"
#include "cfg_parser.h"
#include "config_base.h"
#include "client.target.pb.h"
#include <map>
#include <vector>

using namespace std;


class config_target
{
public:
	inline const uint32 get_config_target_id() const { return id; }
	inline const uint32 get_config_target_type() const { return type; }
	inline const uint32 get_config_target_subtype() const { return subtype; }
	inline const uint32 get_config_target_prev() const { return prev; }
	inline const uint32 get_config_target_next() const { return next; }
	inline const uint32 get_config_target_line() const { return line; }
	inline const uint32 get_config_target_level_limit() const { return level_limit; }
	inline const uint32 get_config_target_complete() const { return complete; }
	inline const uint32 get_config_target_mount_limit() const { return mount_limit; }
	inline const uint32 get_config_target_start_hour() const { return hour_limit; }
	inline const uint32 get_config_target_last() const { return last; }
	inline const map<int, int> & get_config_target_reward() const { return items; }
	inline const uint32 get_config_target_ingot() const { return ingot; }
	inline const uint32 get_config_target_copper() const { return copper; }
	inline const uint32 get_config_target_power() const { return power; }
	inline const uint32 get_config_target_exp() const { return exp; }
	inline const uint32 get_config_target_taolve() const { return taolve; }
	inline const client::target_complete_cond & get_config_target_complete_cond() const {return tcc; }
	inline const string &get_descrition() const { return description; }

public:
	uint32 id;
	uint32 type;
	uint32 subtype;
	uint32 prev;
	uint32 next;
	uint32 line;
	uint32 complete;
	client::target_complete_cond tcc;
	uint32 level_limit;
	uint32 mount_limit;
	uint32 hour_limit;
	uint32 last;
	map<int, int> items;
	uint32 ingot;
	uint32 copper;
	uint32 power;
	uint32 exp;
	uint32 taolve;
	string description;
};

class config_target_mgr : public config_base_mgr
{
public:
	int init(TTableGroup_s *p);
	~config_target_mgr();
	const config_target * get_config_target(int id) const;
	const map<uint32, config_target *> &get_target_list() const { return _target_list; }

public:

	void init_less_cond(const char *str, client::cond_less *less);
	void init_great_cond(const char *str, client::cond_great *great);
	void init_equal_cond(const char *str, client::cond_equal *equal);
	void init_one_cond(const char *str, client::target_complete_cond &tcc);
	void init_complete_cond(const char *cstr, client::target_complete_cond &tcc);
	const vector<config_target *> &get_sub_targets(uint32 subtype) const;

public:
	TTableGroup_s *_target;
	TTable *_table;
	map<uint32, vector<config_target *> > _sub;
	map<uint32, config_target *> _target_list;				// ÒÔIDÎªKEY
};

#endif
