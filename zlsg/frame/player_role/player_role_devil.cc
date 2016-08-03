#include "player_role_devil.h"
#include "config_devil.h"

#define DEVIL_ROLE_BUF 65535

player_role_devil::~player_role_devil()
{
	if(_trd != NULL)
	{
		delete _trd;
	}
	_stage.clear();
	_raid.clear();
}

void player_role_devil::init_role_devil(uint32 role_id)
{
	if(_trd != NULL)
	{
		return ;
	}
	_trd = new mysql::tb_role_devil;
	_trd->set_role_id(role_id);
	_trd->set_stage_info("");
	_trd->set_raid_info("");
	writer_type = WOTE_INSERT;
}

void player_role_devil::save(uint32 role_id, amtsvr_context *ctx)
{
	if(NULL == _trd)
	{
		init_role_devil(role_id);
	}
	string str1, str2;
	if(serialize_stage(str1) == -1 || serialize_raid(str2) == -1)
	{
		return ;
	}
	mysql::tb_role_devil *trd = new mysql::tb_role_devil(*_trd);
	trd->set_stage_info(str1);
	trd->set_raid_info(str2);
	CACHE_KEY k(trd->role_id(), 0);
	write(ctx, k, trd);
	writer_type = WOTE_UPDATE;
	return ;
}

int player_role_devil::serialize_stage(string &str)
{
	char buf[DEVIL_ROLE_BUF] = {0};
	int offset = 0;
	std::map<uint32, uint32>::iterator i;
	for(i = _stage.begin(); i != _stage.end(); ++i)
	{
		client::devil_stage_info dsi;
		dsi.set_id(i->first);
		dsi.set_state(i->second);
		uint16 len = dsi.ByteSize();
		if(offset + len + sizeof(uint16) > DEVIL_ROLE_BUF)
		{
			amtsvr_log("Serialize Devil stage Faield\n");
			return -1;
		}
		*(uint16 *)(buf + offset) = len;
		offset += sizeof(uint16);
		if(!dsi.SerializeToArray(buf + offset, len))
		{
			amtsvr_log("Serialize Devil stage Failed\n");
			return -1;
		}
		offset += len;
	}
	str.append(buf, offset);
	return str.length();
}

int player_role_devil::serialize_raid(string &str)
{
	char buf[DEVIL_ROLE_BUF] = {0};
	int offset = 0;
	std::map<uint32, client::devil_raid_info>::iterator i;
	for(i = _raid.begin(); i != _raid.end(); ++i)
	{
		uint16 len = i->second.ByteSize();
		if(offset + len + sizeof(uint16) >= DEVIL_ROLE_BUF)
		{
			amtsvr_log("serialize_raid failed\n");
			return -1;
		}
		*(uint16 *)(buf + offset) = len;
		offset += sizeof(uint16);
		if(!i->second.SerializeToArray(buf + offset, len))
		{
			amtsvr_log("serialize_raid failed\n");
			return -1;
		}
		offset += len;
	}
	str.append(buf, offset);
	return str.length();
}

int player_role_devil::unserialize_stage(const string &str)
{
	const char *ptr = str.c_str();
	int length = str.length();
	int offset = 0;
	while(offset < length)
	{
		uint16 len  = *(uint16 *)(ptr + offset);
		offset += sizeof(uint16);
		client::devil_stage_info dsi;
		if(!dsi.ParseFromArray(ptr + offset, len))
		{
			amtsvr_log("Parse stage failed\n");
			return -1;
		}
		offset += len;
		_stage.insert(make_pair(dsi.id(), dsi.state()));
	}
	return 0;
}

int player_role_devil::unserialize_raid(const string &str)
{
	const char *ptr = str.c_str();
	int length = str.length();
	int offset = 0;
	while(offset < length)
	{
		uint16 len = *(uint16 *)(ptr + offset);
		offset += sizeof(uint16);
		client::devil_raid_info dri;
		if(!dri.ParseFromArray(ptr + offset, len))
		{
			amtsvr_log("Parse Devil Raid Failed\n");
			return -1;
		}
		_raid.insert(make_pair(dri.raid_id(), dri));
		offset += len;
	}
	return 0;
}

void player_role_devil::attach_role_devil(int count, cmq_svr_pb_ctx *pb_ctx)
{
	for(int i = 0; i < count; ++i)
	{
		if (i == 0)
		{
			_trd = static_cast<mysql::tb_role_devil *>(pb_ctx[i].pb_msg);
			unserialize_stage(_trd->stage_info());
			unserialize_raid(_trd->raid_info());
			return ;
		}
		else
		{
			delete pb_ctx[i].pb_msg;
		}
	}
}

bool player_role_devil::is_stage_award(uint32 id)
{
	std::map<uint32, uint32>::iterator i = _stage.find(id);
	return (i != _stage.end()) && (i->second == 1);
}

void player_role_devil::set_stage_award(uint32 id)
{
	_stage[id] = 1;
}

void player_role_devil::add_raid_info(uint32 raid_id, uint32 star_num, uint32 tick, uint32 champion)
{
	std::map<uint32, client::devil_raid_info>::iterator i = _raid.find(raid_id);
	if(i == _raid.end())
	{
		client::devil_raid_info dri;
		dri.set_raid_id(raid_id);
		dri.set_star(star_num);
		dri.set_tick(tick);
		dri.set_champion(champion);
		dri.set_time((uint32)time(NULL));
		_raid.insert(make_pair(raid_id, dri));
		return ;
	}
	i->second.set_time((uint32)time(NULL));
	if(i->second.star() > star_num)
	{
		i->second.set_star(star_num);
	}
	if(i->second.champion())
	{
		return ;
	}
	if(i->second.tick() > tick)
	{
		i->second.set_tick(tick);
	}
	i->second.set_champion(champion);
}

// 0 首杀
// 1 再杀
// 2 没有
int player_role_devil::get_reward_type(uint32 id)
{
	std::map<uint32, client::devil_raid_info>::iterator i = _raid.find(id);
	if(i == _raid.end())
	{
		return 0;
	}
	if(i->second.time() == 0)
	{
		return 1;
	}
	return 2;
}

int player_role_devil::reset_raid(uint32 id)
{
	std::map<uint32, client::devil_raid_info>::iterator i = _raid.find(id);
	if(i == _raid.end())
	{
		return -1;
	}
	i->second.set_time(0);
	return 0;
}

const client::devil_raid_info *player_role_devil::get_devil_raid(uint32 raid_id) const
{
	std::map<uint32, client::devil_raid_info>::const_iterator i = _raid.find(raid_id);
	if(i == _raid.end())
	{
		return NULL;
	}
	return &i->second;
}

bool player_role_devil::raid_is_perfect(uint32 raid_id)
{
	std::map<uint32, client::devil_raid_info>::iterator i = _raid.find(raid_id);
	if(i == _raid.end())
	{
		return false;
	}
	return i->second.star() == 1;
}

bool player_role_devil::raid_is_champion(uint32 raid_id)
{
	std::map<uint32, client::devil_raid_info>::iterator i = _raid.find(raid_id);
	if(i == _raid.end())
	{
		return false;
	}
	return i->second.champion() == 1;
}

bool player_role_devil::raid_is_complete(uint32 raid_id)
{
	std::map<uint32, client::devil_raid_info>::iterator i = _raid.find(raid_id);
	return i != _raid.end();
}

void player_role_devil::fill_devil_raids(client::devil_stage_list &list)
{
	std::map<uint32, client::devil_raid_info>::iterator i;
	for(i = _raid.begin(); i != _raid.end(); ++i)
	{
		client::devil_raid_detail *drd = list.add_drd();
		drd->set_raid_id(i->first);
		drd->set_star(i->second.star());
		drd->set_tick(i->second.tick());
		drd->set_champion(i->second.champion());
		drd->set_reward_type(get_reward_type(i->first));
	}
	std::map<uint32, uint32>::iterator j;
	for(j = _stage.begin(); j != _stage.end(); ++j)
	{
		client::devil_stage_info *dsi = list.add_dsi();
		dsi->set_id(j->first);
		dsi->set_state(j->second);
	}
}

int player_role_devil::fill_devil_raid_info(uint32 id, client::devil_raid_detail &drd)
{
	std::map<uint32, client::devil_raid_info>::iterator i = _raid.find(id);
	if(i == _raid.end())
	{
		return -1;
	}
	drd.set_raid_id(id);
	drd.set_star(i->second.star());
	drd.set_tick(i->second.tick());
	drd.set_champion(i->second.champion());
	drd.set_reward_type(get_reward_type(id));
	return 0;
}
