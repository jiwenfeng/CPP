#include "player_role_vip.h"
#include "config_vip.h"
#include "base_time.h"
#define BUFFLEN 65535

player_role_vip::player_role_vip(mysql::tb_role_vip *trv):_trv(trv)
{
}

player_role_vip::~player_role_vip()
{
	if(_trv != NULL)
	{
		delete _trv;
		_trv = NULL;
	}
}

uint32 player_role_vip::add_vip_exp(uint32 role_id, uint32 n, bool month /* = false */)
{
#ifndef __LIB_MINI_SERVER__
	if(NULL == _trv)
	{
		_trv = new mysql::tb_role_vip;
		_trv->set_role_id(role_id);
		_trv->set_vip_lv(0);
		_trv->set_vip_exp(0);
		_trv->set_month_timeout(0);
		_trv->set_list("");
		_trv->set_buy_record("");
		writer_type = WOTE_INSERT;
	}
	n += _trv->vip_exp();
	_trv->set_vip_exp(n);
	const std::vector<config_vip *> vip_list = CONFMGR->get_vip_list();
	uint32 lv = _trv->vip_lv();
	for(; lv < vip_list.size() - 1; lv++)
	{
		if(n < (uint32)vip_list[lv]->_need)
		{
			break;
		}
	}
	if(lv == 0)
	{
		return 0;
	}
	_trv->set_vip_lv(lv);
	if(month)
	{
		_trv->set_month_timeout(time_utils::make_day_start_time() + 30 * 24 * 3600);
	}
	return lv;
#else
    return 0;
#endif // __LIB_MINI_SERVER__
}

int player_role_vip::save(amtsvr_context *ctx, uint32 role_id)
{
	if(NULL == _trv)
	{
		_trv = new mysql::tb_role_vip;
		_trv->set_role_id(role_id);
		_trv->set_vip_lv(0);
		_trv->set_vip_exp(0);
		_trv->set_month_timeout(0);
		_trv->set_list("");
		_trv->set_buy_record("");
		writer_type = WOTE_INSERT;
	}
	string str1, str2;
	if(-1 == serialize_op(str1) || -1 == serialize_buy(str2))
	{
		return -1;
	}
	_trv->set_list(str1);
	_trv->set_buy_record(str2);
	mysql::tb_role_vip *trv = new mysql::tb_role_vip(*_trv);
	CACHE_KEY k(trv->role_id(), 0);
	write(ctx, k, trv);
	writer_type = WOTE_UPDATE;
	return 0;
}

int player_role_vip::unserialize_buy(const string & str)
{
	int len = str.length();
	int offset = 0;
	const char *ptr = str.c_str();
	while(offset < len)
	{
		uint16 len = *(uint16 *)(ptr + offset);
		offset += sizeof(uint16);
		client::recharge_info ri;
		if(!ri.ParseFromArray(ptr + offset, len))
		{
			return -1;
		}
		_buy.insert(make_pair(ri.id(), ri.num()));
		offset += len;
	}
	return 0;
}

int player_role_vip::serialize_buy(string &str)
{
	char buf[BUFFLEN] = {0};
	int offset = 0;
	map<uint32, uint32>::iterator it;
	for(it = _buy.begin(); it != _buy.end(); ++it)
	{
		client::recharge_info ri;
		ri.set_id(it->first);
		ri.set_num(it->second);
		uint16 len = ri.ByteSize();
		*(uint16 *)(buf + offset) = len;
		offset += sizeof(uint16);
		if(offset + len >= BUFFLEN)
		{
			amtsvr_log("out of bound\n");
			return -1;
		}
		if(!ri.SerializeToArray(buf + offset, len))
		{
			amtsvr_log("serialize failed\n");
			return -1;
		}
		offset += len;
	}
	str.append(buf, offset);
	return offset;
}

int player_role_vip::unserialize_op(const string & str)
{
	int len = str.length();
	int offset = 0;
	const char *ptr = str.c_str();
	while(offset < len)
	{
		uint16 len = *(uint16 *)(ptr + offset);
		if(len <= 0)
		{
			amtsvr_log("unserialize_op failed\n");
			return -1;
		}
		offset += sizeof(uint16);
		client::vip_op_info voi;
		if(!voi.ParseFromArray((void *)(ptr + offset), len))
		{
			amtsvr_log("unserialize_op failed\n");
			return -1;
		}
		map<uint32, map<uint32, uint32> >::iterator it = _op.find(voi.op());
		if(it == _op.end())
		{
			map<uint32, uint32> m;
			for(int i = 0; i < voi.vo_size(); ++i)
			{
				m.insert(make_pair(voi.vo(i).id(), voi.vo(i).count()));
			}
			_op.insert(make_pair(voi.op(), m));
		}
		else
		{
			for(int i = 0; i < voi.vo_size(); ++i)
			{
				it->second[voi.vo(i).id()] = voi.vo(i).count();
			}
		}
		offset += len;
	}
	return 0;
}

int player_role_vip::serialize_op(string &str)
{
	char buf[BUFFLEN] = {0};
	int offset = 0;
	map<uint32, map<uint32, uint32> >::iterator it;
	for(it = _op.begin(); it != _op.end(); ++it)
	{
		client::vip_op_info voi;
		voi.set_op(it->first);
		map<uint32, uint32>::iterator j = it->second.begin();
		for(; j != it->second.end(); ++j)
		{
			client::vip_op *vo = voi.add_vo();
			vo->set_count(j->second);
			vo->set_id(j->first);
		}
		uint16 len = voi.ByteSize();
		if(offset + len + sizeof(uint16) >= BUFFLEN)
		{
			amtsvr_log("out of bound\n");
			return -1;
		}
		*(uint16 *)(buf + offset) = len;
		offset += sizeof(uint16);
		if(!voi.SerializeToArray((void *)(buf + offset), len))
		{
			amtsvr_log("serialize failed\n");
			return -1;
		}
		offset += len;
	}
	str.append(buf, offset);
	return offset;
}

void player_role_vip::attach_role_vip(int count, cmq_svr_pb_ctx *pb_ctx)
{
	for(int i = 0; i < count; ++i)
	{
		if (i == 0)
		{
			_trv = static_cast<mysql::tb_role_vip *>(pb_ctx[i].pb_msg);
			unserialize_op(_trv->list());
			unserialize_buy(_trv->buy_record());
		}
		else
		{
			delete pb_ctx[i].pb_msg;
		}
	}
}

uint32 player_role_vip::get_vip_lv()
{
	if(NULL == _trv)
	{
		return 0;
	}
	return _trv->vip_lv();
}

uint32 player_role_vip::get_vip_exp()
{
	if(NULL == _trv)
	{
		return 0;
	}
	return _trv->vip_exp();
}

void player_role_vip::clear_vip_op_info()
{
	std::map<uint32, map<uint32, uint32> >::iterator it = _op.begin();
	for(; it != _op.end(); ++it)
	{
		it->second.clear();
	}
	_op.clear();
	if(NULL == _trv || _trv->month_timeout() == 0)
	{
		return;
	}
	if((uint32)time(NULL) >= _trv->month_timeout())
	{
		_trv->set_month_timeout(0);
	}
}

bool player_role_vip::op_has_left_times(uint32 op, uint32 id)
{
	const config_vip *cv = CONFMGR->get_config_vip(get_vip_lv());
	if(NULL == cv)
	{
		return false;
	}
	map<int, int>::const_iterator it = cv->_op.find(op);
	if(it == cv->_op.end())
	{
		return false;
	}
	map<uint32, map<uint32, uint32> >::iterator i = _op.find(op);
	if(i == _op.end())
	{
		return true;
	}
	map<uint32, uint32>::iterator j = i->second.find(id);
	if(j == i->second.end())
	{
		return true;
	}
	return j->second < (uint32)it->second;
}

void player_role_vip::set_op_count(uint32 op, uint32 id, uint32 times /* = 1*/)
{
	std::map<uint32, map<uint32, uint32> >::iterator i = _op.find(op);
	if(i == _op.end())
	{
		map<uint32, uint32> v;
		v.insert(make_pair(id, times));
		_op.insert(make_pair(op, v));
	}
	else
	{
		i->second[id] += times;
	}
}

uint32 player_role_vip::get_op_count(uint32 op, uint32 id)
{
	std::map<uint32, map<uint32, uint32> >::iterator i = _op.find(op);
	if(i == _op.end())
	{
		return 0;
	}
	std::map<uint32, uint32>::iterator j = i->second.find(id);
	if(j == i->second.end())
	{
		return 0;
	}
	return j->second;
}

uint32 player_role_vip::get_month_timeout()
{
	if(NULL == _trv)
	{
		return 0;
	}
	return _trv->month_timeout();
}

void player_role_vip::set_vip_lv(uint32 role_id, uint32 lv, bool month)
{
	const config_vip *cv = CONFMGR->get_config_vip(lv);
	if(NULL == cv)
	{
		return ;
	}
	if(NULL == _trv)
	{
		_trv = new mysql::tb_role_vip;
		_trv->set_role_id(role_id);
		_trv->set_vip_lv(0);
		_trv->set_vip_exp(0);
		_trv->set_month_timeout(0);
		_trv->set_list("");
		_trv->set_buy_record("");
		writer_type = WOTE_INSERT;
	}
	_trv->set_vip_lv(lv);
	_trv->set_vip_exp(cv->_need);
	if(month)
	{
		_trv->set_month_timeout(time_utils::make_day_start_time() + 30 * 24 * 3600);
	}
}
