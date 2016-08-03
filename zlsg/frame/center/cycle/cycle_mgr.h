#ifndef __CYCLE_MGR_H__
#define __CYCLE_MGR_H__

#include "cycle_role_mgr.h"

class cycle : public pb_writer
{
public:
	cycle(mysql::tb_center_cycle *tcc = NULL);
	~cycle();
public:
	int save(amtsvr_context *ctx);

public:
	void cycle_init(uint32 level, uint32 role_id, uint32 tick);
	uint32 get_role_id();
	uint32 get_tick();
	void update_fastest_record(uint32 role_id, uint32 tick);

private:
	mysql::tb_center_cycle *_tcc;
	bool _need_save;
};

class cycle_mgr
{
public:
	cycle_mgr();
	~cycle_mgr();

public:
	void save(amtsvr_context *ctx);
	int init(amtsvr_context *ctx);
	int load_level_data();

public:
	uint32 get_left_helped_count(uint32 role_id);
	bool level_passed(uint32 role_id, uint32 level);
	void add_helped_count(uint32 role_id, uint32 count);
	int add_passed_level(uint32 role_id, uint32 level, uint32 tick);
	void get_fastest_info(uint32 level, uint32 &role_id, uint32 &tick);

private:
	std::map<uint32, cycle *> _cycles;
	amtsvr_context *_ctx;
	cycle_role_mgr _crm;
};

#endif
