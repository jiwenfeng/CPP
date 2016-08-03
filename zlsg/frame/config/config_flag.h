#ifndef __CONFIG_FLAG_H__
#define __CONFIG_FLAG_H__

#include "cfg_cache_mgr.h"
#include "cfg_parser.h"
#include "config_base.h"

class config_flag
{
public:
	config_flag() {};
	~config_flag() {};

public:
	int flag_id;
	int npc_id;
};

class config_flag_mgr : public config_base_mgr
{
public:
	config_flag_mgr() {};
	~config_flag_mgr();
	int init(TTableGroup_s *p);

	const config_flag *get_flag(int i)const;

private:
	TTableGroup_s *_flag;
	TTable *_table;
	std::map<int, config_flag *> _cp;
};

#endif
