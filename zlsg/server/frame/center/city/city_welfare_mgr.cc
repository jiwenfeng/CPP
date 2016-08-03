#include "city_welfare_mgr.h"
city_welfare_mgr::city_welfare_mgr():_tcw(NULL), _need_save(false)
{
}

city_welfare_mgr::~city_welfare_mgr()
{
	if(_tcw)
	{
		delete _tcw;
		_tcw = NULL;
	}
}

int city_welfare_mgr::save(amtsvr_context *ctx)
{
	if(NULL == _tcw)
	{
		return 0;
	}
	mysql::tb_city_welfare *tcw = new mysql::tb_city_welfare(*_tcw);
	CACHE_KEY k(tcw->id(), 0);
	write(ctx, k, tcw);
	writer_type = WOTE_UPDATE;
	_need_save = false;
	return 0;
}

int city_welfare_mgr::init(amtsvr_context *ctx)
{
	hash_map_obj<CACHE_KEY, cfg_cache_obj, CFG_CACHE_BULB_NUM>::pair_t *cur = (hash_map_obj<CACHE_KEY, cfg_cache_obj, CFG_CACHE_BULB_NUM>::pair_t *)CONFMGR->get_city_welfare_info();
	writer_type = WOTE_INSERT;
	while(cur != NULL)
	{
		mysql::tb_city_welfare *tcw = static_cast<mysql::tb_city_welfare *>(cur->mutable_val().pop_msg());
		cur = cur->get_next();
		if(NULL == tcw)
		{
			continue;
		}
		_tcw = tcw;//new mysql::tb_city_welfare(*tcw);
		writer_type = WOTE_UPDATE;
	}
	_ctx = ctx;
	return 0;
}

int city_welfare_mgr::mod_family_id(amtsvr_context *ctx, uint32 family_id)
{
	if(_tcw == NULL)
	{
		_tcw = new mysql::tb_city_welfare();
		writer_type = WOTE_INSERT;
	}
	_tcw->set_id(1);
	_tcw->set_family_id(family_id);
	_tcw->set_official_1(0);
	_tcw->set_official_2(0);
	_tcw->set_official_3(0);
	_tcw->set_official_4(0);
	_tcw->set_official_5(0);
	_tcw->set_official_6(0);
	save(ctx);
	return 0;
}

uint32 city_welfare_mgr::get_family_id()
{
	return _tcw == NULL ? 0 : _tcw->family_id();
}

int city_welfare_mgr::set_time(int pos)
{
	uint32 now = (uint32)time(NULL);
	switch(pos)
	{
		case 1: 
			_tcw->set_official_1(now);
			break;
		case 2:
			_tcw->set_official_2(now);
			break;
		case 3:
			_tcw->set_official_3(now);
			break;
		case 4:
			_tcw->set_official_4(now);
			break;
		case 5:
			_tcw->set_official_5(now);
			break;
		case 6:
			_tcw->set_official_6(now);
			break;
		default:
			break;
	}
	save(_ctx);
	return 0;
}

uint32 city_welfare_mgr::get_time(int index)
{
	switch(index)
	{
		case 1:
			return _tcw->official_1();
		case 2:
			return _tcw->official_2();
		case 3:
			return _tcw->official_3();
		case 4:
			return _tcw->official_4();
		case 5:
			return _tcw->official_5();
		case 6:
			return _tcw->official_6();
		default:
			return (uint32)time(NULL);
	}
}

bool city_welfare_mgr::welfare_has_got(int index)
{
	uint32 now = (uint32)time(NULL);
	return time_utils::is_same_day(now, get_time(index));
}
