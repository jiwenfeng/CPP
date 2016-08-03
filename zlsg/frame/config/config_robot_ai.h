#ifndef __CONFIG_ROBOT_AI_H__
#define __CONFIG_ROBOT_AI_H__

#include "cfg_cache_mgr.h"
#include "cfg_parser.h"
#include "pb_mgr.h"
#include "config_base.h"

class config_robot_ai
{
public:
	uint32 _id;
	uint32 _type;
	uint32 _time;
	string _ai;
public:
	const uint32 get_robot_ai_id()		 const		{ return _id; }
	const uint32 get_robot_ai_type()	 const		{ return _type; }
	const uint32 get_robot_ai_time()	 const		{ return _time; }
	const string &get_robot_ai()		 const		{ return _ai; }
};

class config_robot_ai_mgr : public config_base_mgr
{
public:
	const config_robot_ai *get_config_robot_ai(uint32 id) const;
	const config_robot_ai *random_config_robot_ai(uint32 type) const;
	int init(TTableGroup_s* p);
	void clear();
	~config_robot_ai_mgr();

private:
	vector<uint32> _attack;
	vector<uint32> _defence;
	map<uint32, config_robot_ai *> _ai;
    TTableGroup_s*                  _robot;
    TTable*                       _table;

};

#endif // __config_robot_ai_H__
