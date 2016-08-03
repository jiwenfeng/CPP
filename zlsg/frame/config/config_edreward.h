#ifndef __CONFIG_EDREWARD_H__
#define __CONFIG_EDREWARD_H__

#include "cfg_cache_mgr.h"
#include "cfg_parser.h"
#include "config_base.h"

class config_edreward
{
public:
	config_edreward() {};
	~config_edreward();

public:
	int min_rank;
	int max_rank;
	int id;					// 礼包ID /* 暂时未用到 */
	std::vector<std::pair<int, int> > _reward;
};

class config_edreward_mgr : public config_base_mgr
{
public:
	config_edreward_mgr() {};
	~config_edreward_mgr();
	int init(TTableGroup_s *p);

	const config_edreward *get_edreward(int i) const;
	const std::vector<config_edreward *> &get_all_edreward() const {return _cp;}

private:
	TTableGroup_s *_edreward;
	TTable *_table;
	std::vector<config_edreward *> _cp;
};

#endif
