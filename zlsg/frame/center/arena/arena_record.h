#ifndef __ARENA_RECORD_H__
#define __ARENA_RECORD_H__

#include "define.h"
#include "pb_writer.h"
#include "comm_define.h"

#define MAX_RECORD_NUM 20

class arena_record : public pb_writer
{
public:
	arena_record(mysql::tb_arena_record *tar = NULL);
	~arena_record();
	void push(client::arena_record_info *tar);
	void unserialize(const string &str);
	void save(uint32 role_id, amtsvr_context *ctx);
	void mod_record_status(int status);
	void mod_writer_type(WRITER_OPERATE_TYPE_E status);
	int serialize(string &str);
	int get_record_status();
public:
	vector<client::arena_record_info *> _r;
	bool _need_save;
private:
	mysql::tb_arena_record *_tar;
	int _tail;
};

class arena_record_mgr
{
public:
	arena_record_mgr();
	~arena_record_mgr();
	int init();
	int get_record_status(uint32 role_id);
	void save(amtsvr_context *ctx);
	void add_record(uint32 role_id, const string &name, uint32 rank, int action, int ret);
	void get_record(uint32 role_id, vector<client::arena_record_info> &v);
	void attach_ctx(amtsvr_context *ctx);
private:
	std::map<uint32, arena_record *> _record;
	amtsvr_context *_ctx;
};

#endif
