#include "config_pay.h"

config_pay_mgr::~config_pay_mgr()
{
	std::map<int, std::map<int, config_pay *> >::iterator it;
	for(it = _pay_list.begin(); it != _pay_list.end(); ++it)
	{
		map<int, config_pay *>::iterator j = it->second.begin();
		for(; j != it->second.end(); ++j)
		{
			delete j->second;
		}
		it->second.clear();
	}
	_pay_list.clear();
}

int config_pay_mgr::init(TTableGroup_s *p)
{
	if(!p)
	{
		return -1;
	}
	_pay = p;
	_table = _pay->GetTable("t_payfunction");;
	if(NULL == _table)
	{
		return -2;
	}
	cfg_parser cp;
	for(int i = 1; i <= _table->m_recordCount; ++i)
	{
		uint32 id = _table->GetField(i, "PayType");
		config_pay *cp = new config_pay;
		cp->_times  = _table->GetField(i, "Number");
		cp->_need = _table->GetField(i, "Price");
		cp->_get = _table->GetField(i, "Preward");
		std::map<int, std::map<int, config_pay *> >::iterator it = _pay_list.find(id);
		if(it == _pay_list.end())
		{
			map<int, config_pay *> m;
			m.insert(make_pair(cp->_times, cp));
			_pay_list.insert(make_pair(id, m));
		}
		else
		{
			it->second.insert(make_pair(cp->_times, cp));
		}
	}
	return 0;
}

const config_pay *config_pay_mgr::get_config_pay(uint32 type, uint32 times) const
{
	std::map<int, std::map<int, config_pay *> >::const_iterator it = _pay_list.find(type);
	if(it == _pay_list.end())
	{
		return NULL;
	}
	std::map<int, config_pay *>::const_iterator j = it->second.find(times);
	if(j == it->second.end())
	{
		return NULL;
	}
	return j->second;
}
