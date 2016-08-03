#include "cycle_role_mgr.h"
#include "client.cycle.pb.h"
#include "config_cycle.h"
#include "base_time.h"

cycle_role::~cycle_role()
{
	if(_data != NULL)
	{
		delete _data;
		_data = NULL;
	}
}

int cycle_role::save(amtsvr_context *ctx)
{
	if(_data == NULL)
	{
		return -1;
	}
	if(!_need_save)
	{
		return 0;
	}
	mysql::tb_center_cycle_role *data = new mysql::tb_center_cycle_role(*_data);
	string str;
	if(-1 == serialize_levels(str))
	{
		return -1;
	}
	data->set_passed_levels(str);
    CACHE_KEY k(data->role_id(), 0);
    write(ctx, k, data);
    writer_type = WOTE_UPDATE;
	_need_save = false;
	return 0;
}

void cycle_role::add_passed_level(uint32 level)
{
	if(_levels.find(level) == _levels.end())
	{
		_levels[level] = 1;
		_need_save = true;
	}
}

void cycle_role::init_cycle_role(uint32 role_id)
{
	if(NULL != _data)
	{
		return;
	}
	_data = new mysql::tb_center_cycle_role;
	_data->set_role_id(role_id);
	_data->set_helped_count(0);
	_data->set_timestamp(0);
	_data->set_passed_levels("");
	writer_type = WOTE_INSERT;
	_need_save = true;
}

int cycle_role::serialize_levels(string &str)
{
	client::cycle_passed_levels cpl;
	std::map<uint32, uint32>::iterator i;
	for(i = _levels.begin(); i != _levels.end(); ++i)
	{
		cpl.add_level(i->first);
	}
	if(!cpl.SerializeToString(&str))
	{
		return -1;
	}
	return 0;
}

int cycle_role::unserialize_levels(const string &str)
{
	_levels.clear();
	client::cycle_passed_levels cpl;
	if(!cpl.ParseFromString(str))
	{
		return -1;
	}
	for(int i = 0; i < cpl.level_size(); ++i)
	{
		_levels[cpl.level(i)] = 1;
	}
	return 0;
}

void cycle_role::add_helped_count(uint32 count)
{
	if(_data == NULL)
	{
		return;
	}
	uint32 now = (uint32)time(NULL);
	if(!time_utils::is_same_day(now, _data->timestamp()))
	{
		_data->set_helped_count(0);
	}
	_data->set_helped_count(_data->helped_count() + count);
	_data->set_timestamp(now);
	_need_save = true;
}

uint32 cycle_role::get_helped_count()
{
	if(_data == NULL)
	{
		return 0;
	}
	uint32 now = (uint32)time(NULL);
	if(!time_utils::is_same_day(now, _data->timestamp()))
	{
		_data->set_helped_count(0);
	}
	return _data->helped_count();
}

void cycle_role::reset_helped_count()
{
	if(_data == NULL)
	{
		return;
	}
	_data->set_helped_count(0);
	_need_save = true;
}

bool cycle_role::level_passed(uint32 level)
{
	return _levels.find(level) != _levels.end();
}

cycle_role_mgr::~cycle_role_mgr()
{
	std::map<uint32, cycle_role *>::iterator i;
	for(i = _roles.begin(); i != _roles.end(); ++i)
	{
		delete i->second;
	}
	_roles.clear();
}

int cycle_role_mgr::save(amtsvr_context *ctx)
{
	std::map<uint32, cycle_role *>::iterator i;
	for(i = _roles.begin(); i != _roles.end(); ++i)
	{
		i->second->save(ctx);
	}
	return 0;
}

int cycle_role_mgr::init(amtsvr_context *ctx)
{
	hash_map_obj<CACHE_KEY, cfg_cache_obj, CFG_CACHE_BULB_NUM>::pair_t *cur = (hash_map_obj<CACHE_KEY, cfg_cache_obj, CFG_CACHE_BULB_NUM>::pair_t *)CONFMGR->get_center_cycle_role_head();
	while(cur != NULL)
	{
		mysql::tb_center_cycle_role *data = static_cast<mysql::tb_center_cycle_role *>(cur->mutable_val().pop_msg());
		cur = cur->get_next();
		if(NULL == data)
		{
			continue;
		}
		cycle_role *cr = new cycle_role(data);
		cr->unserialize_levels(data->passed_levels());
		_roles.insert(make_pair(data->role_id(), cr));
	}
	_ctx = ctx;
	return 0;
}


void cycle_role_mgr::reset_helped_count()
{
	std::map<uint32, cycle_role *>::iterator i;
	for(i = _roles.begin(); i != _roles.end(); ++i)
	{
		i->second->reset_helped_count();
	}
}

uint32 cycle_role_mgr::get_helped_count(uint32 role_id)
{
	std::map<uint32, cycle_role *>::iterator i = _roles.find(role_id);
	if(i == _roles.end())
	{
		return -1;
	}
	return i->second->get_helped_count();
}

void cycle_role_mgr::add_passed_level(uint32 role_id, uint32 level)
{
	std::map<uint32, cycle_role *>::iterator i = _roles.find(role_id);
	if(i == _roles.end())
	{
		cycle_role *cr = new cycle_role;
		cr->init_cycle_role(role_id);
		cr->add_passed_level(level);
		_roles.insert(make_pair(role_id, cr));
		return ;
	}
	i->second->add_passed_level(level);
}

bool cycle_role_mgr::level_passed(uint32 role_id, uint32 level)
{
	std::map<uint32, cycle_role *>::iterator i = _roles.find(role_id);
	if(i == _roles.end())
	{
		return false;
	}
	return i->second->level_passed(level);
}

void cycle_role_mgr::add_helped_count(uint32 role_id, uint32 count)
{
	std::map<uint32, cycle_role *>::iterator i = _roles.find(role_id);
	if(i == _roles.end())
	{
		return ;
	}
	i->second->add_helped_count(count);
}
