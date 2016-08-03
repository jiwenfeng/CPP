#include "rank.h"

#define MAX_BUFF_LEN 65535

clsrank::clsrank(int type):_rank_type(type), _update_time(0), _need_save(false), _need_update(false)
{
	writer_type = WOTE_INSERT;
}

clsrank::~clsrank()
{
	_rd.clear(); 
	_show.clear();
	_real.clear();
}

void clsrank::update(amtsvr_context *ctx)
{
	if(!_need_update)
	{
		return ;
	}
	_show.clear();
	for(size_t i = 0; i < _real.size(); ++i)
	{
		client::rank_rule ru;
		ru.set_role_id(_real[i].role_id());
		ru.set_first(_real[i].first());
		_show.push_back(ru);
	}
	sort(_show.begin(), _show.end(), bigger());
	_need_save = true;
	_need_update = false;
//	save(ctx);
}

void clsrank::print()
{
	amtsvr_log("排行榜类型:%d\n", _rank_type);
	for(size_t i = 0; i < _show.size(); ++i)
	{
		amtsvr_log("role_id:%u, first:%u\n", _show[i].role_id(), _show[i].first());
	}
}

void clsrank::save(amtsvr_context *ctx)
{
	if(!_need_save)
	{
		return ;
	}
	_rd.clear();
	size_t i = 0;
	size_t r = _real.size();
	size_t s = _show.size();
	while(i < r || i < s)
	{
		client::rank_data rd;
		if(i < r)
		{
			client::rank_rule *ru_real = rd.mutable_ru_real();
			*ru_real = _real[i];
		}
		if(i < s)
		{
			client::rank_rule *ru_show = rd.mutable_ru_show();
			*ru_show = _show[i];
		}
		_rd.push_back(rd);
		++i;
	}
	string out;
	serialize(out);
	mysql::tb_rank tr;
	tr.set_rank_type(_rank_type);
	tr.set_update_time(_update_time);
	tr.set_rank_list(out);
	mysql::tb_rank *ptr = new mysql::tb_rank(tr);
	CACHE_KEY k(_rank_type, 0);
	write(ctx, k, ptr);
	_need_save = false;
	writer_type = WOTE_UPDATE;
}

bool clsrank::need_update()
{
	int fre = get_update_frequence();
	if(fre <= 0)
	{
		return false;
	}
	int now = (int)time(NULL);
	int diff = now - _update_time;
	if(diff > fre)
	{
		int ut = now - diff % fre;	// 当前时间的前一个刷新点
		if(ut > _update_time)
		{
			_update_time = ut;
			return true;
		}
	}
	return false;
}

void clsrank::add_to_rank(client::rank_rule ru)
{
	for(size_t i = 0; i < _real.size(); ++i)
	{
		if(_real[i].role_id() == ru.role_id())
		{
			_real[i] = ru;
			_need_save = true;
			_need_update = true;
			sort(_real.begin(), _real.end(), bigger());
			return;
		}
	}
	if(!is_full())
	{
		_real.push_back(ru);
		_need_save = true;
		_need_update = true;
		sort(_real.begin(), _real.end(), bigger());
		return;
	}
	int cap = get_rank_capacity();
	if(bigger()(ru, _real[cap - 1]))
	{
		_real[cap - 1] = ru;
		_need_save = true;
		_need_update = true;
	}
}

void clsrank::add_to_act_rank(client::rank_rule ru)
{
	for(size_t i = 0; i < _real.size(); ++i)
	{
		if(_real[i].role_id() == ru.role_id())
		{
			_real[i] = ru;
			_need_save = true;
			_need_update = true;
			sort(_real.begin(), _real.end(), bigger());
			return;
		}
	}

	_real.push_back(ru);
	_need_save = true;
	_need_update = true;
	sort(_real.begin(), _real.end(), bigger());
}

uint32 clsrank::get_rank_num(uint32 role_id)
{
	for(size_t i = 0; i < _real.size(); ++i){
		if (_real[i].role_id() == role_id){
			return i + 1;
		}
	}

	return 0;
}

uint32 clsrank::get_act_rank_value(uint32 role_id)
{
	for(size_t i = 0; i < _real.size(); ++i){
		if (_real[i].role_id() == role_id){
			return _real[i].first();
		}
	}

	return 0;
}

void clsrank::clear_rank()
{
	_real.clear();
	_show.clear();
	_rd.clear();
	_need_save = true;
}

int clsrank::serialize(string &out)
{
	char buf[MAX_BUFF_LEN] = {0};
	uint32 offset = 0;
	int count = 0;
	for(size_t i = 0; i < _rd.size(); ++i)
	{
		uint32 len = _rd[i].ByteSize();
		if(offset + len + sizeof(uint16) >= MAX_BUFF_LEN)
		{
			amtsvr_log("out of bound\n");
			return 0;
		}
		*(uint16 *)(buf + offset) = (uint16)len;
		offset += sizeof(uint16);
		_rd[i].SerializeToArray((void *)(buf + offset), len);
		offset += len;
		++count;
	}
	out.append(buf, offset);
	return count;
}

int clsrank::unserialize(const string &str)
{
	int len = str.length();
	const char *ptr = str.c_str();
	int offset = 0;
	while(offset < len)
	{
		uint16 msg_len = *(uint16 *)(ptr + offset);
		offset += sizeof(uint16);
		client::rank_data rd;
		if(!rd.ParseFromArray(ptr + offset, msg_len))
		{
			amtsvr_log("unserialize data failed\n");
			return -1;
		}
		offset += msg_len;
		if(rd.has_ru_show())
		{
			_show.push_back(rd.ru_show());
		}
		if(rd.has_ru_real())
		{
			_real.push_back(rd.ru_real());
		}
	}
	return 0;
}

void clsrank::init(mysql::tb_rank *tr)
{
	_update_time = tr->update_time();
	if(-1 == unserialize(tr->rank_list()))
	{
		return ;
	}
	sort(_show.begin(), _show.end(), bigger());
	sort(_real.begin(), _real.end(), bigger());
	writer_type = WOTE_UPDATE;
}

int clsrank::get_update_frequence()
{
	return CONFMGR->get_basic_value(10148);
}

bool clsrank::is_full()
{
	return (int)_real.size() == get_rank_capacity();
}

int clsrank::get_rank_capacity()
{
	return CONFMGR->get_basic_value(10163);
}
