#ifndef __CONFIG_SIEGE_H__
#define __CONFIG_SIEGE_H__

#include "cfg_cache_mgr.h"
#include "cfg_parser.h"
#include "config_base.h"

class config_siege
{
public:
	config_siege() {};
	~config_siege() {};
	const string &get_model_id(uint32 pro_id);
	const map<int, int> &get_reward(){return item;}

public:
	map<uint32, string> model;
	map<int, int> item;
};

class config_siege_mgr : public config_base_mgr
{
public:
	config_siege_mgr() {};
	~config_siege_mgr();
	int init(TTableGroup_s *p);
	const map<int, int> &get_reward(uint32 off_id) const;
	const string &get_profession_model(uint32 off_id, uint32 pro_id) const;
	const map<int, config_siege *> &get_welfare_list() const{return _cp;}
private:
	TTableGroup_s *_siege;
	TTable *_table;
	std::map<int, config_siege *> _cp;
};

#endif
