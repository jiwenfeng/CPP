#include "rank_mgr.h"

rank_mgr::~rank_mgr()
{
	std::vector<clsrank *>::iterator it;
	for(it = _rank_list.begin(); it != _rank_list.end(); ++it)
	{
		delete *it;
	}
	_rank_list.clear();
}

void rank_mgr::init_rank_list()
{
	int max = RANK_GEMS_INTEGRAL + 1;
	for(int i = 0; i < max; ++i)
	{
		_rank_list.push_back(new clsrank(i));
	}
}

void rank_mgr::init()
{
	init_rank_list();

	hash_map_obj<CACHE_KEY, cfg_cache_obj, CFG_CACHE_BULB_NUM>::pair_t *cur = (hash_map_obj<CACHE_KEY, cfg_cache_obj, CFG_CACHE_BULB_NUM>::pair_t *)CONFMGR->get_rank_head();
	while(cur != NULL)
	{
		mysql::tb_rank *tr = static_cast<mysql::tb_rank *>(cur->mutable_val().pop_msg());
		cur = cur->get_next();
		if(NULL == tr)
		{
			continue;
		}
		_rank_list[tr->rank_type()]->init(tr);
        delete tr;
	}
}

void rank_mgr::update(amtsvr_context *ctx)
{
	std::vector<clsrank *>::iterator it;
	for(it = _rank_list.begin(); it != _rank_list.end(); ++it)
	{
		if((*it)->need_update())
		{
			(*it)->update(ctx);
		}
	}
}

void rank_mgr::save(amtsvr_context *ctx)
{
	std::vector<clsrank *>::iterator it;
	for(it = _rank_list.begin(); it != _rank_list.end(); ++it)
	{
		(*it)->save(ctx);
	}
}

void rank_mgr::add_to_rank(uint32 type, client::rank_rule ru)
{
	if(type >= _rank_list.size())
	{
		return ;
	}
	_rank_list[type]->add_to_rank(ru);
}

void rank_mgr::add_to_act_rank(uint32 type, client::rank_rule ru)
{
	if(type >= _rank_list.size()){
		return ;
	}

	_rank_list[type]->add_to_act_rank(ru);
}

uint32 rank_mgr::get_rank_num(uint32 type,uint32 role_id)
{
	if(type >= _rank_list.size()){
		return 0;
	}

	return _rank_list[type]->get_rank_num(role_id);
}

uint32 rank_mgr::get_act_rank_value(uint32 type,uint32 role_id)
{
	if(type >= _rank_list.size()){
		return 0;
	}

	return _rank_list[type]->get_act_rank_value(role_id);
}

void rank_mgr::clear_rank(uint32 type)
{
	if(type >= _rank_list.size()){
		return;
	}

	if (type == RANK_EQUIP_POWER || type == RANK_GEMS_INTEGRAL){
		_rank_list[type]->clear_rank();	
	}
}

const std::vector<client::rank_rule>& rank_mgr::get_rank_list(int type)
{
	if(type < 0 || type >= (int)_rank_list.size())
	{
		static std::vector<client::rank_rule> e;
		return e;
	}
	return _rank_list[type]->get_rank_list();
}

const std::vector<client::rank_rule>& rank_mgr::get_real_rank_list(int type)
{
	if(type < 0 || type >= (int)_rank_list.size())
	{
		static std::vector<client::rank_rule> e;
		return e;
	}

	return _rank_list[type]->get_real_rank_list();
}