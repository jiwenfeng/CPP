#include "player_role_cycle.h"
#include "config_cycle.h"
#define CYCLE_ROLE_BUF 1 << 16

player_role_cycle::player_role_cycle(mysql::tb_role_cycle *trc) : _trc(trc)
{
	writer_type = WOTE_UPDATE;
}

player_role_cycle::~player_role_cycle()
{
	if(_trc == NULL)
	{
		return;
	}
	delete _trc;
	_trc = NULL;
}

int player_role_cycle::save(uint32 role_id, amtsvr_context *ctx)
{
	if(_trc == NULL)
	{
		init_role_cycle(role_id);
	}
	string str1, str2;
	if(-1 == serialize_helpers(str1))
	{
		return -1;
	}
	if(-1 == serialize_levels(str2))
	{
		return -1;
	}
	mysql::tb_role_cycle *trc = new mysql::tb_role_cycle(*_trc);
	trc->set_passed_levels(str2);
	trc->set_helpers(str1);
	CACHE_KEY k(trc->role_id(), 0);
	write(ctx, k, trc);
	writer_type = WOTE_UPDATE;
	return 0;
}

void player_role_cycle::attach_role_cycle(int count, cmq_svr_pb_ctx *pb_ctx)
{
	for(int i = 0; i < count; ++i)
	{
		if (i == 0)
		{
			_trc = static_cast<mysql::tb_role_cycle *>(pb_ctx[i].pb_msg);
			unserialize_helpers(_trc->helpers());
			unserialize_levels(_trc->passed_levels());
		}
		else
		{
			delete pb_ctx[i].pb_msg;
		}
	}
}

int player_role_cycle::init_role_cycle(uint32 role_id)
{
	if(_trc != NULL)
	{
		return -1;
	}
	_trc = new mysql::tb_role_cycle;
	_trc->set_role_id(role_id);
	_trc->set_reset_time(0);
	_trc->set_cur_level(1);
	_trc->set_top_level(0);
	_trc->set_passed_levels("");
	_trc->set_helpers("");
	writer_type = WOTE_INSERT;
	return 0;
}

bool player_role_cycle::is_helped_me(uint32 role_id) const
{
	std::map<uint32, uint32>::const_iterator i = _helpers.find(role_id);
	if(i == _helpers.end())
	{
		return false;
	}
	return true;
}

void player_role_cycle::add_helper(uint32 role_id)
{
	_helpers[role_id] = (uint32)time(NULL);
}

void player_role_cycle::clear_helpers()
{
	_helpers.clear();
}

int player_role_cycle::serialize_helpers(string &str)
{
	std::map<uint32, uint32>::iterator i;
	client::cycle_helpers ch;
	uint32 now = (uint32)time(NULL);
	for(i = _helpers.begin(); i != _helpers.end(); ++i)
	{
		if(!time_utils::is_same_day(now, i->second))
		{
			continue;
		}
		client::helper_info *hi = ch.add_hi();
		hi->set_role_id(i->first);
		hi->set_time(i->second);
	}
	if(!ch.SerializeToString(&str))
	{
		return -1;
	}
	return str.length();
}

int player_role_cycle::unserialize_helpers(const string &str)
{
	_helpers.clear();
	client::cycle_helpers ch;
	uint32 now = (uint32)time(NULL);
	if(!ch.ParseFromString(str))
	{
		return -1;
	}
	for(int i = 0; i < ch.hi_size(); ++i)
	{
		const client::helper_info &hi = ch.hi(i);
		if(!time_utils::is_same_day(now, hi.time()))
		{
			continue;
		}
		_helpers[hi.role_id()] = hi.time();
	}
	return 0;
}

int player_role_cycle::serialize_levels(string &str)
{
	std::map<uint32, client::cycle_level_info>::iterator i;
	char buf[CYCLE_ROLE_BUF] = {0};
	int offset = 0;
	for(i = _levels.begin(); i != _levels.end(); ++i)
	{
		uint16 len = i->second.ByteSize();
		if(offset + len + sizeof(uint16) >= CYCLE_ROLE_BUF)
		{
			amtsvr_log("out of bound\n");
			return -1;
		}
		*(uint16 *)(buf + offset) = len;
		offset += sizeof(uint16);
		if(!i->second.SerializeToArray((void *)(buf + offset), len))
		{
			amtsvr_log("serialize failed\n");
			return -1;
		}
		offset += len;
	}
	str.append(buf, offset);
	return str.length();
}

int player_role_cycle::unserialize_levels(const string &str)
{
	_levels.clear();
	int offset = 0;
	const char *ptr = str.c_str();
	int len = str.length();
	while(offset < len)
	{
		uint16 len = *(uint16 *)(ptr + offset);
		offset += sizeof(uint16);
		client::cycle_level_info cli;
		if(!cli.ParseFromArray(ptr + offset, len))
		{
			return -1;
		}
		_levels.insert(make_pair(cli.level(), cli));
		offset += len;
	}
	return 0;
}

int player_role_cycle::set_cur_level(uint32 level)
{
	if(NULL == _trc)
	{
		return -1;
	}
	_trc->set_cur_level(level);
	return 0;
}

void player_role_cycle::update_pass_level_time(uint32 level)
{
	std::map<uint32, client::cycle_level_info>::iterator i = _levels.find(level);
	if(i != _levels.end())
	{
		i->second.set_time((uint32)time(NULL));
	}
}

void player_role_cycle::update_role_level(uint32 role_id, uint32 level, uint32 tick, uint32 update)
{
	if(_trc == NULL)
	{
		init_role_cycle(role_id);
	}
	std::map<uint32, client::cycle_level_info>::iterator i = _levels.find(level);
	if(i == _levels.end())
	{
		client::cycle_level_info cli;
		cli.set_level(level);
		cli.set_update(update);
		cli.set_time((uint32)time(NULL));
		cli.set_tick(tick);
		_levels.insert(make_pair(level, cli));
	}
	else
	{
		i->second.set_time((uint32)time(NULL));
		if(i->second.update() == 1)
		{
			if(i->second.tick() > tick && tick != 0)
			{
				i->second.set_tick(tick);
			}
			i->second.set_update(update);
		}
	}
	uint32 limit = CONFMGR->get_max_cycle_level();
	if(limit > level + 1)
	{
		_trc->set_cur_level(level + 1);
	}
	else
	{
		_trc->set_cur_level(limit);
	}
	if(level > _trc->top_level())
	{
		_trc->set_top_level(level);
	}
}

const client::cycle_level_info *player_role_cycle::get_role_level_info(uint32 level) const
{
	std::map<uint32, client::cycle_level_info>::const_iterator i = _levels.find(level);
	if(i == _levels.end())
	{
		return NULL;
	}
	return &(i->second);
}

// 0 首杀
// 1 再杀
// 2 没奖
uint32 player_role_cycle::get_level_reward_type(uint32 level)
{
	std::map<uint32, client::cycle_level_info>::iterator i = _levels.find(level);
	if(i == _levels.end())
	{
		return 0;
	}
	if(i->second.time() <= _trc->reset_time())
	{
		return 1;
	}
	return 2;
}

int player_role_cycle::reset_level()
{
	if(_trc == NULL)
	{
		return -1;
	}
	const config_cycle *cc = CONFMGR->get_config_cycle(_trc->cur_level());
	if(NULL == cc)
	{
		return -2;
	}
	uint32 start = CONFMGR->get_start_level(cc->get_difficulty());
	_trc->set_cur_level(start);
	_trc->set_reset_time((uint32)time(NULL));
	return 0;
}

uint32 player_role_cycle::get_cur_level()
{
	if(_trc == NULL)
	{
		return 1;
	}
	return _trc->cur_level();
}

// top_level 当前已通过的最高关卡
// cur_level 当前挑战关卡
uint32 player_role_cycle::get_top_level()
{
	if(_trc == NULL)
	{
		return 0;
	}
	return _trc->top_level();
}


uint32 player_role_cycle::get_level_used_tick(uint32 level)
{
	std::map<uint32, client::cycle_level_info>::iterator i = _levels.find(level);
	if(i == _levels.end())
	{
		return 0;
	}
	return i->second.tick();
}

bool player_role_cycle::award_fastest_prize(uint32 level)
{
	std::map<uint32, client::cycle_level_info>::iterator i = _levels.find(level);
	if(i == _levels.end())
	{
		return false;
	}
	return i->second.update() == 0;
}
