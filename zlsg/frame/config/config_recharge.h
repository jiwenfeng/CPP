#ifndef __CONFIG_RECHARGE_H__
#define __CONFIG_RECHARGE_H__

#include "cfg_cache_mgr.h"
#include "cfg_parser.h"
#include "config_base.h"

class config_recharge
{
public:
	config_recharge() {};
	~config_recharge() {};

public:
	inline uint32 get_id()	  const { return _id; }
	inline uint32 get_type()  const { return _type; }
	inline uint32 get_ingot() const { return _ingot; }
	inline uint32 get_times() const { return _times; }

public:
	uint32 _id;
	uint32 _type;
	uint32 _ingot;
	uint32 _times;
};

class config_recharge_mgr : public config_base_mgr
{
public:
	config_recharge_mgr() : _min_recharge(-1) {}
	~config_recharge_mgr();
	int init(TTableGroup_s *p);
	const config_recharge *get_config_recharge(uint32 i)const;
	const config_recharge *get_config_recharge_by_n(uint32 i) const;
	const std::map<uint32, config_recharge *> &get_config_recharge_list() const { return _cr; }
	const uint32 get_min_recharge() { return _min_recharge; }

private:
	TTableGroup_s *_recharge;
	TTable *_table;
	std::map<uint32, config_recharge *> _cr;
	uint32 _min_recharge;
};

#endif
