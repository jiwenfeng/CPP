#ifndef __CONFIG_CYCLE_H__
#define __CONFIG_CYCLE_H__

#include "cfg_cache_mgr.h"
#include "config_base.h"

class config_cycle
{
public:
    config_cycle(void);
    ~config_cycle(void);

public:
	inline uint32 get_cycle_id() const { return _id; }
	inline uint32 get_monster_id() const { return _monster_id; }
	inline uint32 get_difficulty() const { return _difficulty; }
	inline uint32 get_fast_reward() const { return _fast_reward; }
	inline uint32 get_first_reward() const { return _first_reward; }
	inline uint32 get_again_reward() const { return _again_reward; }

public:
	uint32 _id;
	uint32 _monster_id;
	uint32 _difficulty;
	uint32 _fast_reward;
	uint32 _first_reward;
	uint32 _again_reward;
};

class config_cycle_mgr : public config_base_mgr
{
public:
    config_cycle_mgr(TTableGroup_s* p = NULL);
    virtual ~config_cycle_mgr(void);

public:
    virtual int init(TTableGroup_s* p);
    int clear();

public:
    inline const TTable* get_table() const { return _table; }

    const config_cycle* get_config_cycle(uint32 id) const;

	const uint32 get_max_cycle_level() const { return _max; }

	const uint32 get_start_level(uint32 diff) const;

private:
    TTableGroup_s* _group;
    TTable*        _table;

private:
    std::map<uint32, config_cycle*> _cfg;
	std::map<uint32, uint32> _start;
	uint32 _max;
};

#endif // __CONFIG_CYCLE_H__
