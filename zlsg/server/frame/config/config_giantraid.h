#ifndef __config_giantraid_H__
#define __config_giantraid_H__

#include "cfg_cache_mgr.h"
#include "cfg_parser.h"
#include "config_base.h"

class config_giantraid
{
public:
	uint32  _raid_id;
	uint32  _player_min_level;
	uint32  _player_max_level;
	uint32  _rand_power_min;
	uint32  _rand_power_max;
	int		_rand_lv_min;
	int		_rand_lv_max;
	std::vector<int> _reward_item;
	uint32  _reward_money;
	uint32  _reward_resource;
	uint32  _index;
public:
	inline uint32 get_raid_id() const { return _raid_id; }
	inline uint32 get_player_min_level() const { return _player_min_level; }
	inline uint32 get_player_max_level() const { return _player_max_level; }
	inline uint32 get_rand_power_min() const { return _rand_power_min; }
	inline uint32 get_rand_power_max() const { return _rand_power_max; }
	inline int get_rand_lv_min() const	{ return _rand_lv_min; }
	inline int get_rand_lv_max() const	{ return _rand_lv_max; }
	inline uint32 get_reward_money() const { return _reward_money; }
	inline uint32 get_reward_resource() const { return _reward_resource; }
	inline const std::vector<int> &get_reward_item() const { return _reward_item; }
	inline uint32 get_giant_index() const { return _index; }
};

class config_giantraid_mgr : public config_base_mgr
{
public:
	config_giantraid_mgr(TTableGroup_s* p = NULL);
	~config_giantraid_mgr(void);

public:
    int                                  init(TTableGroup_s* p);
    int                                  clear();

public:
    const TTable*                        get_table()   const { return _table; }
	void get_config_giantraid(uint32 level, vector<config_giantraid *> &v);
	const config_giantraid * get_config_giantraid(uint32 id) const;
private:
    TTableGroup_s*                _ts;
    TTable*                       _table;
    std::vector<config_giantraid*> _cb;
	map<uint32, config_giantraid *> _cgs;
};

#endif // __config_npc_H__
