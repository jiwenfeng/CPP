#ifndef __CONFIG_HISTORY_REWARD_H__
#define __CONFIG_HISTORY_REWARD_H__

#include "cfg_cache_mgr.h"
#include "cfg_parser.h"
#include "config_base.h"

class config_history
{
public:
	config_history() {};
	~config_history() {};

public:
	int _min;
	int _max;
	int _rate;
	int _float;
};

class config_history_mgr : public config_base_mgr
{
public:
	config_history_mgr() {};
	~config_history_mgr() ;
	int init(TTableGroup_s *p);
	double get_reward(int history, int cur) const;

private:
	TTableGroup_s *_history;
	TTable *_table;
	std::vector<config_history *> _v;
};

#endif
