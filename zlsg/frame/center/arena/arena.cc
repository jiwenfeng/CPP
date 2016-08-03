#include "arena.h"

arena::arena(mysql::tb_center_arena *tca):data(tca)
{
	_need_save = false;
}

arena::~arena()
{
	if(NULL == data)
	{
		return;
	}
	delete data;
	data = NULL;
}

int arena::save(amtsvr_context *ctx)
{
    if (data == NULL || !_need_save){
        return -1;
    }
    mysql::tb_center_arena* tca = new mysql::tb_center_arena(*data);
    CACHE_KEY k(tca->rank(), 0);
    write(ctx, k, tca);
    writer_type = WOTE_UPDATE;
	_need_save = false;
	return 0;
}

void arena::add_data(uint32 role_id, int rank, int type)
{
	if(data == NULL)
	{
		data = new mysql::tb_center_arena;
		writer_type = WOTE_INSERT;
	}
	data->set_role_id(role_id);
	data->set_rank(rank);
	data->set_history(rank);
	data->set_type(type);
	_need_save = true;
}

int arena::get_history_rank() const
{
	if(NULL == data)
	{
		return -1;
	}
	return data->history();
}

void arena::set_history_rank(int rank)
{
	if(NULL == data)
	{
		return;
	}
	data->set_history(rank);
}

uint32 arena::get_role_id() const
{
	if(NULL == data)
	{
		return 0;
	}
	return data->role_id();
}

int arena::get_role_type() const
{
	if(NULL == data)
	{
		return 0;
	}
	return data->type();
}

void arena::set_rank(int rank)
{
	if(NULL == data)
	{
		return;
	}
	data->set_rank(rank);
	if(rank < get_history_rank())
	{
		set_history_rank(rank);
	}
	_need_save = true;
}

int arena::get_rank() const
{
	if(NULL == data)
	{
		return -1;
	}
	return data->rank();
}
