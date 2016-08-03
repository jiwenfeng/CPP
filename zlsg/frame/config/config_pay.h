#ifndef __CONFIG_PAY_H__
#define __CONFIG_PAY_H__

#include "cfg_cache_mgr.h"
#include "cfg_parser.h"
#include "config_base.h"

class config_pay
{
public:
	config_pay() {};
	~config_pay() {};

public:
	uint32 _times;
	uint32 _need;
	uint32 _get;
};

class config_pay_mgr : public config_base_mgr
{
public:
	config_pay_mgr() {};
	~config_pay_mgr();
	int init(TTableGroup_s *p);
	const config_pay *get_config_pay(uint32 type, uint32 times) const;
private:
	TTableGroup_s *_pay;
	TTable *_table;
	std::map<int, std::map<int, config_pay *> > _pay_list;
};


#endif
