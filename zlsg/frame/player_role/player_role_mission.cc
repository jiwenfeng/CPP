#include "player_role_mission.h"
#include "config_target.h"
#define T_LEN 65535

player_role_mission::player_role_mission(mysql::tb_role_target *trt) : _trt(trt), _time(0)
{
	_target.clear();
}

player_role_mission::~player_role_mission()
{
	map<uint32, client::target_info *>::iterator it ;
	for(it = _target.begin(); it != _target.end(); ++it)
	{
		delete it->second;
	}
	_target.clear();
	if(_trt)
	{
		delete _trt;
	}
}

void player_role_mission::attach_role_target(int count, cmq_svr_pb_ctx *pb_ctx)
{
	for(int i = 0; i < count; ++i)
	{
		if (i == 0)
		{
			_trt = static_cast<mysql::tb_role_target *>(pb_ctx[i].pb_msg);
			unserialize(_trt->list());
		}
		else
		{
			delete pb_ctx[i].pb_msg;
		}
	}
}

void player_role_mission::clear_daily_progress()
{
#ifndef __LIB_MINI_SERVER__
	map<uint32, client::target_info *>::iterator it;
	for(it = _target.begin(); it != _target.end();)
	{
		const config_target *t = CONFMGR->get_config_target(it->first);
		if(t != NULL && t->type == client::DAILY_MISSION)
		{
			delete it->second;
			_target.erase(it++);
		}
		else
		{
			it++;
		}
	}
	_time = (uint32)time(NULL);
#endif 
}

void player_role_mission::save(amtsvr_context *ctx, uint32 role_id)
{
	string out;
	if(!_trt)
	{
		_trt = new mysql::tb_role_target;
		_trt->set_role_id(role_id);
		writer_type = WOTE_INSERT;
	}
	if(-1 == serialize(out) || out.length() == 0)
	{
		return ;
	}
	_trt->set_time(_time);
	_trt->set_list(out);
	mysql::tb_role_target *trt = new mysql::tb_role_target(*_trt);
	CACHE_KEY k(trt->role_id(), 0);
	write(ctx, k, trt);
	writer_type = WOTE_UPDATE;
}

int player_role_mission::serialize(string &str)
{
	char buf[T_LEN] = {0};
	int offset = 0;
	map<uint32, client::target_info *>::iterator it ;
	for(it = _target.begin(); it != _target.end(); ++it)
	{
		uint16 msg_len = it->second->ByteSize();
		if(offset + msg_len + sizeof(uint16) >= T_LEN)
		{
			amtsvr_log("out of bound\n");
			return -1;
		}
		*(uint16 *)(buf + offset) = (uint16)msg_len;
		offset += sizeof(uint16);
		if(!it->second->SerializeToArray((void *)(buf + offset), msg_len))
		{
			amtsvr_log("SerializeToArray Failed\n");
			return -1;
		}
		offset += msg_len;
	}
	str.append(buf, offset);
	return 0;
}

int player_role_mission::unserialize(const string &str)
{
#ifndef __LIB_MINI_SERVER__
	const char *ptr = str.c_str();
	size_t len = str.length();
	size_t offset = 0;
	while(offset < len)
	{
		uint16 msg_len = *(uint16 *)(ptr + offset);
		if(msg_len <= 0)
		{
			amtsvr_log("msg_len error\n");
			return -1;
		}
		offset += sizeof(uint16);
		client::target_info *ti = new client::target_info;
		if(!ti->ParseFromArray((void *)(ptr + offset), msg_len))
		{
			delete ti;
			amtsvr_log("ParseFromArray Failed\n");
			return -1;
		}
		offset += msg_len;
		const config_target *tc = CONFMGR->get_config_target(ti->target_id());
		if(tc == NULL)
		{
			delete ti;
			continue;
		}
		_target.insert(make_pair(tc->id, ti));
	}
#endif
	return 0;
}

bool player_role_mission::target_can_prize(uint32 id) const
{
	map<uint32, client::target_info *>::const_iterator it = _target.find(id);
	if(it != _target.end() && it->second->status() == client::STATUS_PRIZE)
	{
		return true;
	}
	return false;
}

void player_role_mission::set_target_status(uint32 id, client::TARGET_STATUS_E status)
{
	map<uint32, client::target_info *>::const_iterator it = _target.find(id);
	if(it == _target.end())
	{
		return;
	}
	if(it->second->status() == status)
	{
		return;
	}
	it->second->set_status(status);
}

bool player_role_mission::target_is_doing(uint32 id) const
{
	map<uint32, client::target_info *>::const_iterator it = _target.find(id);
	if(it == _target.end() || it->second->status() == client::STATUS_DOING)
	{
		return true;
	}
	return false;
}

bool player_role_mission::target_has_finished(uint32 id) const
{
	map<uint32, client::target_info *>::const_iterator it = _target.find(id);
	if(it != _target.end() && it->second->status() == client::STATUS_FINISH)
	{
		return true;
	}
	return false;
}

const client::target_info *player_role_mission::get_target_info(uint32 id)
{
	map<uint32, client::target_info *>::const_iterator it = _target.find(id);
	if(it != _target.end())
	{
		return it->second;
	}
	return NULL;
}

uint32 player_role_mission::modify_target_progress(uint32 id, uint32 num, bool flag)
{
	map<uint32, client::target_info *>::iterator it = _target.find(id);
	if(it == _target.end())
	{
		client::target_info *ti = new client::target_info;
		ti->set_target_id(id);
		ti->set_num(num);
		ti->set_status(client::STATUS_DOING);
		_target.insert(make_pair(id, ti));
		return num;
	}
	if(flag)
	{
		it->second->set_num(it->second->num() + num);
	}
	else
	{
		it->second->set_num(num);
	}
	return it->second->num();
}
