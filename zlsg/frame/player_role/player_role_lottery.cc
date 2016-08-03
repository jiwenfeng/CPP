#include "player_role_lottery.h"
#include "config_lottery.h"
#include "client.lottery.pb.h"

#define LOTTERY_LEN 65535

player_role_lottery::player_role_lottery(mysql::tb_role_lottery *trl):_trl(trl)
{
}

player_role_lottery::~player_role_lottery()
{
	_list.clear();
	if(_trl)
	{
		delete _trl;
		_trl = NULL;
	}
}

void player_role_lottery::clear_props()
{
	_list.clear();
}

void player_role_lottery::save(amtsvr_context *ctx, uint32 role_id)
{
	if(!_trl)
	{
		_trl = new mysql::tb_role_lottery;
		_trl->set_role_id(role_id);
		_trl->set_gold_time(0);
		_trl->set_bronze_time(0);
		_trl->set_num(0);
		writer_type = WOTE_INSERT;
	}
	string str;
	if(-1 == serialize(str))
	{
		return;
	}
	_trl->set_props_list(str);
	mysql::tb_role_lottery *trl = new mysql::tb_role_lottery(*_trl);

	CACHE_KEY k(trl->role_id(), 0);
	write(ctx, k, trl);
	writer_type = WOTE_UPDATE;
}

void player_role_lottery::attach_role_lottery(int count, cmq_svr_pb_ctx *pb_ctx)
{
	for(int i = 0; i < count; ++i)
	{
		if (i == 0)
		{
			_trl = static_cast<mysql::tb_role_lottery *>(pb_ctx[i].pb_msg);
			unserialize(_trl->props_list());
		}
		else
		{
			delete pb_ctx[i].pb_msg;
		}
	}
}

int player_role_lottery::serialize(string &str)
{
	int offset = 0;
	std::map<uint32, uint32>::iterator it;
	char buf[LOTTERY_LEN] = {0};
	for(it = _list.begin(); it != _list.end(); ++it)
	{
		client::lottery_item li;
		li.set_prop_id(it->first);
		li.set_num(it->second);
		uint16 len = li.ByteSize();
		if(offset + len + sizeof(uint16) >= LOTTERY_LEN)
		{
			amtsvr_log("out of bound\n");
			return -1;
		}
		*(uint16 *)(buf + offset) = len;
		offset += sizeof(uint16);
		if(!li.SerializeToArray((void *)(buf + offset), len))
		{
			amtsvr_log("SerializeToArray Failed\n");
			return -1;
		}
		offset += len;
	}
	str.append(buf, offset);
	return 0;
}

int player_role_lottery::unserialize(const string &str)
{
	uint32 offset = 0;
	uint32 len = str.length();
	const char *ptr = str.c_str();
	while(offset < len)
	{
		uint16 len = *(uint16 *)(ptr + offset);
		if(len <= 0)
		{
			return -1;
		}
		offset += sizeof(uint16);
		client::lottery_item li;
		if(!li.ParseFromArray((void *)(ptr + offset), len))
		{
			amtsvr_log("ParseFromArray Failed\n");
			return -1;
		}
		_list[li.prop_id()] = li.num();
		offset += len;
	}
	return 0;
}

uint32 player_role_lottery::get_prop_num(uint32 lottery_id)
{
	return _list[lottery_id];
}

void player_role_lottery::modify_lottery_time(uint32 type, uint32 role_id)
{
	if(!_trl)
	{
		_trl = new mysql::tb_role_lottery;
		_trl->set_role_id(role_id);
		_trl->set_gold_time(0);
		_trl->set_bronze_time(0);
		_trl->set_num(0);
		writer_type = WOTE_INSERT;
	}
	switch(type)
	{
		case client::GOLD_BOX:
			_trl->set_gold_time((uint32)time(NULL));
			break;
		case client::BRONZE_BOX:
			_trl->set_bronze_time((uint32)time(NULL));
			break;
		default:
			break;
	}
}

void player_role_lottery::add_prop_num(uint32 lottery_id, uint32 num /* = 1*/)
{
	_list[lottery_id] += num;
}

uint32 player_role_lottery::get_lottery_time(uint32 type)
{
	if(!_trl)
	{
		return 0;
	}
	if(type == client::GOLD_BOX)
	{
		return _trl->gold_time();
	}
	return _trl->bronze_time();
}

uint32 player_role_lottery::get_used_num()
{
	if(!_trl)
	{
		return 0;
	}
	return _trl->num();
}

void player_role_lottery::modify_used_num(uint32 role_id, uint32 num)
{
	if(!_trl)
	{
		_trl = new mysql::tb_role_lottery;
		_trl->set_role_id(role_id);
		_trl->set_gold_time(0);
		_trl->set_bronze_time(0);
		_trl->set_num(0);
		writer_type = WOTE_INSERT;
	}
	_trl->set_num(_trl->num() + num);
}
