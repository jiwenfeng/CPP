#ifndef __CONFIG_FALLITEM_H__
#define __CONFIG_FALLITEM_H__

#include "cfg_cache_mgr.h"
#include "cfg_parser.h"
#include "config_base.h"

class config_packet
{
public:
	config_packet():total(0) {};
	~config_packet() {};

public:
	vector<drop_content> list;
	int total;

public:
	const drop_content *get_drop_content();
};

class config_packet_mgr : public config_base_mgr
{
public:
	config_packet_mgr() {};
	~config_packet_mgr();
	int init(TTableGroup_s *p);

	const drop_content *get_config_packet(int i) const;

private:
	TTableGroup_s *_group;
	TTable *_table;
	std::map<int, config_packet *> _cp;
};

#endif
