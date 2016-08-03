#ifndef __CONFIG_BASIC_H__
#define __CONFIG_BASIC_H__

#include "cfg_cache_mgr.h"
#include "cfg_parser.h"
#include "config_base.h"

class config_basic
{
public:
	config_basic() {};
	~config_basic() {};

public:
	int value;
};

class config_basic_mgr : public config_base_mgr
{
public:
	config_basic_mgr() {};
	~config_basic_mgr();
	int init(TTableGroup_s *p);

	int get_basic_value(int i)const;

    uint32 get_physical_max()  const { return _physical_max ; }
    uint32 get_physical_tick() const { return _physical_tick; }
    uint32 get_physical_add()  const { return _physical_add ; }

private:
	TTableGroup_s *_basic;
	TTable *_table;
	std::map<int, config_basic *> _cp;

    uint32 _physical_max;
    uint32 _physical_tick;
    uint32 _physical_add;
};

#endif // __CONFIG_BASIC_H__
