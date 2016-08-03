#ifndef __ARENA_H__
#define __ARENA_H__

#include "define.h"
#include "pb_writer.h"
#include "comm_define.h"

#define ARENA_ROBOT		1
#define ARENA_PLAYER	0

class arena : public pb_writer
{
public:
	arena(mysql::tb_center_arena *tca = NULL);
	~arena();
public:
	int get_history_rank() const;
	int get_rank() const;
	int get_role_type() const;
	int save(amtsvr_context *ctx);
	uint32 get_role_id() const; 
	void set_rank(int rank);
	void add_data(uint32 role_id, int rank, int type);

private:
	void set_history_rank(int rank);
	bool _need_save;
private:
    mysql::tb_center_arena* data;
};

#endif
