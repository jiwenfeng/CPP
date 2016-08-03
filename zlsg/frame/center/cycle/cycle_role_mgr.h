#ifndef __CYCLE_ROLE_MGR_H__
#define __CYCLE_ROLE_MGR_H__

#include "define.h"
#include "pb_writer.h"
#include "comm_define.h"
#include "client.cycle.pb.h"

class cycle_role : public pb_writer
{
public:
	cycle_role(mysql::tb_center_cycle_role *data = NULL) : _data(data), _need_save(false) {}
	~cycle_role();
	void add_passed_level(uint32 level); 
	void add_helped_count(uint32 count = 1);
	void reset_helped_count();
	void init_cycle_role(uint32 role_id);
	uint32 get_helped_count();
	bool level_passed(uint32 level);

public:
	int save(amtsvr_context *ctx);
	int init(amtsvr_context *ctx);
	int serialize_levels(string &str);
	int unserialize_levels(const string &str);

private:
	mysql::tb_center_cycle_role *_data;
	std::map<uint32, uint32> _levels; // 单独通过的关卡
	bool _need_save;
};

class cycle_role_mgr
{
public:
	cycle_role_mgr() {}
	~cycle_role_mgr();
public:
	int save(amtsvr_context *ctx);
	int init(amtsvr_context *ctx);

public:
	uint32 get_helped_count(uint32 role_id);
	void reset_helped_count();
	void add_helped_count(uint32 role_id, uint32 count);
	bool level_passed(uint32 role_id, uint32 level);
	void add_passed_level(uint32 role_id, uint32 level);

private:
	std::map<uint32, cycle_role *> _roles;
	amtsvr_context *_ctx;
};

#endif
