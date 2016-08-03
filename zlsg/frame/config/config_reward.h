#ifndef __CONFIG_REWARD_H__
#define __CONFIG_REWARD_H__

#include "cfg_cache_mgr.h"
#include "cfg_parser.h"
#include "config_base.h"

class config_reward
{
public:
	config_reward() {};
	~config_reward() {};

public:
	uint32 system_id;
	uint32 reward_condt;
	map<int, int> rank;
	uint32 integral;
	uint32 reward_pack;
	uint32 money;
	uint32 military;
	uint32 physical;
	uint32 experience;
	uint32 reputation;
	uint32 jungong;
};

class config_reward_mgr : public config_base_mgr
{
public:
	config_reward_mgr() {};
	~config_reward_mgr();
	int init(TTableGroup_s *p);

	const config_reward *get_reward(int i)const;

private:
	TTableGroup_s *_reward;
	TTable *_table;
	std::map<int, config_reward *> _cp;
};

#endif
