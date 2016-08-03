#include "cycle_mgr.h"

cycle::cycle(mysql::tb_center_cycle *tcc) : _tcc(tcc), _need_save(false)
{
}

cycle::~cycle()
{
	if(NULL == _tcc)
	{
		delete _tcc;
		_tcc = NULL;
	}
}

void cycle::cycle_init(uint32 level, uint32 role_id, uint32 tick)
{
	if(_tcc != NULL)
	{
		return ;
	}
	_tcc = new mysql::tb_center_cycle;
	_tcc->set_level(level);
	_tcc->set_role_id(role_id);
	_tcc->set_tick(tick);
	writer_type = WOTE_INSERT;
	_need_save = true;
}

void cycle::update_fastest_record(uint32 role_id, uint32 tick)
{
	if(_tcc == NULL)
	{
		return ;
	}
	_tcc->set_role_id(role_id);
	_tcc->set_tick(tick);
	_need_save = true;
}

uint32 cycle::get_role_id()
{
	if(NULL == _tcc)
	{
		return 0;
	}
	return _tcc->role_id();
}

uint32 cycle::get_tick()
{
	if(NULL == _tcc)
	{
		return 0;
	}
	return _tcc->tick();
}

int cycle::save(amtsvr_context *ctx)
{
	if(_tcc == NULL)
	{
		return -1;
	}
	if(!_need_save)
	{
		return 0;
	}
	mysql::tb_center_cycle *tcc = new mysql::tb_center_cycle(*_tcc);
    CACHE_KEY k(tcc->level(), 0);
    write(ctx, k, tcc);
    writer_type = WOTE_UPDATE;
	_need_save = false;
	return 0;
}

cycle_mgr::cycle_mgr()
{
	_ctx = NULL;
}

cycle_mgr::~cycle_mgr()
{
	std::map<uint32, cycle *>::iterator i;
	for(i = _cycles.begin(); i != _cycles.end(); ++i)
	{
		delete i->second;
	}
	_cycles.clear();
}

void cycle_mgr::save(amtsvr_context *ctx)
{
	_crm.save(ctx);
	std::map<uint32, cycle *>::iterator i;
	for(i = _cycles.begin(); i != _cycles.end(); ++i)
	{
		i->second->save(ctx);
	}
}

int cycle_mgr::init(amtsvr_context *ctx)
{
	_ctx = ctx;
	load_level_data();
	_crm.init(ctx);
	return 0;
}

int cycle_mgr::load_level_data()
{
	hash_map_obj<CACHE_KEY, cfg_cache_obj, CFG_CACHE_BULB_NUM>::pair_t *cur = (hash_map_obj<CACHE_KEY, cfg_cache_obj, CFG_CACHE_BULB_NUM>::pair_t *)CONFMGR->get_center_cycle_head();
	while(cur != NULL)
	{
		mysql::tb_center_cycle *tcc = static_cast<mysql::tb_center_cycle *>(cur->mutable_val().pop_msg());
		cur = cur->get_next();
		if(NULL == tcc)
		{
			continue;
		}
		cycle *c = new cycle(tcc);
		_cycles.insert(make_pair(tcc->level(), c));
	}
	return 0;
}

uint32 cycle_mgr::get_left_helped_count(uint32 role_id)
{
	uint32 limit = CONFMGR->get_basic_value(10137);
	uint32 used = _crm.get_helped_count(role_id);
	if(limit < used)
	{
		return 0;
	}
	return limit - used;
}

bool cycle_mgr::level_passed(uint32 role_id, uint32 level)
{
	return _crm.level_passed(role_id, level);
}

void cycle_mgr::add_helped_count(uint32 role_id, uint32 count)
{
	_crm.add_helped_count(role_id, count);
}

int cycle_mgr::add_passed_level(uint32 role_id, uint32 level, uint32 tick)
{
	_crm.add_passed_level(role_id, level);
	std::map<uint32, cycle *>::iterator i = _cycles.find(level);
	if(i == _cycles.end())
	{
		cycle *c = new cycle;
		c->cycle_init(level, role_id, tick);
		_cycles.insert(make_pair(level, c));
		return 1;
	}
	else
	{
		if(i->second->get_tick() > tick)
		{
			i->second->update_fastest_record(role_id, tick);
			return 1;
		}
	}
	return 0;
}

void cycle_mgr::get_fastest_info(uint32 level, uint32 &role_id, uint32 &tick)
{
	std::map<uint32, cycle *>::iterator i = _cycles.find(level);
	if(i == _cycles.end())
	{
		return ;
	}
	role_id = i->second->get_role_id();
	tick = i->second->get_tick();
}
