#ifndef __CONFIG_VIP_H__
#define __CONFIG_VIP_H__

#include "cfg_cache_mgr.h"
#include "cfg_parser.h"
#include "config_base.h"

class config_vip
{
public:
	config_vip() {};
	~config_vip() {};

public:
	inline int get_lv() const { return _lv; }
	inline int get_need() const { return _need; }
	inline int get_grid() const { return _grid; }
	inline int get_sweep() const { return _sweep; }
	inline int get_refresh_count() const { return _refresh_count; }
	inline const std::map<int, int> &get_op() const { return _op; }


public:
	int _lv;
	int _need;
	int _grid;
	int _sweep;
	int _refresh_count;
	std::map<int, int> _op;
};

class config_vip_mgr : public config_base_mgr
{
public:
	config_vip_mgr() {};
	~config_vip_mgr();
	int init(TTableGroup_s *p);
	const config_vip *get_config_vip(uint32 i)const;
	const std::vector<config_vip *>& get_vip_list() const { return _cv; }

private:
	TTableGroup_s *_vip;
	TTable *_table;
	std::vector<config_vip *> _cv;
};

#endif
