#include "devil_mgr.h"

devil_mgr::devil_mgr()
{
}

devil_mgr::~devil_mgr()
{
	std::map<uint32, mysql::tb_center_devil *>::iterator i;
	for(i = _devils.begin(); i != _devils.end(); ++i)
	{
		delete i->second;
	}
	_devils.clear();
	_update.clear();
	_insert.clear();
}

void devil_mgr::save(amtsvr_context *ctx)
{
	writer_type = WOTE_UPDATE;
	for(size_t i = 0; i < _update.size(); ++i)
	{
		mysql::tb_center_devil *tcd = new mysql::tb_center_devil(*_update[i]);	
		CACHE_KEY k(tcd->id(), 0);
		write(ctx, k, tcd);
	}
	writer_type = WOTE_INSERT;
	for(size_t i = 0; i < _insert.size(); ++i)
	{
		mysql::tb_center_devil *tcd = new mysql::tb_center_devil(*_insert[i]);	
		CACHE_KEY k(tcd->id(), 0);
		write(ctx, k, tcd);
	}
	_update.clear();
	_insert.clear();
}

void devil_mgr::init()
{
	hash_map_obj<CACHE_KEY, cfg_cache_obj, CFG_CACHE_BULB_NUM>::pair_t *cur = (hash_map_obj<CACHE_KEY, cfg_cache_obj, CFG_CACHE_BULB_NUM>::pair_t *)CONFMGR->get_center_devil_head();
	while(cur != NULL)
	{
		mysql::tb_center_devil *devil = static_cast<mysql::tb_center_devil *>(cur->mutable_val().pop_msg());
		cur = cur->get_next();
		if(NULL == devil)
		{
			continue;
		}
		_devils.insert(make_pair(devil->id(), devil));
	}
}

int devil_mgr::add_pass_level(uint32 id, uint32 role_id, uint32 time)
{
	std::map<uint32, mysql::tb_center_devil *>::iterator i = _devils.find(id);
	if(i == _devils.end())
	{
		mysql::tb_center_devil *tcd = new mysql::tb_center_devil;
		tcd->set_id(id);
		tcd->set_role_id(role_id);
		tcd->set_time(time);
		_devils.insert(make_pair(id, tcd));
		_insert.push_back(tcd);
		return 1;
	}
	else
	{
		if(i->second->time() > time)
		{
			i->second->set_time(time);
			i->second->set_role_id(role_id);
			_update.push_back(i->second);
			return 1;
		}
	}
	return 0;
}

const mysql::tb_center_devil *devil_mgr::get_pass_info(uint32 id) const
{
	std::map<uint32, mysql::tb_center_devil *>::const_iterator i = _devils.find(id);
	if(i == _devils.end())
	{
		return NULL;
	}
	return i->second;
}
