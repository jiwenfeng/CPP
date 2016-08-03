#include "config_basic.h"

int config_basic_mgr::init(TTableGroup_s *p)
{
	if(!p)
	{
		return -1;
	}
	_basic = p;
	_table = _basic->GetTable("t_basic");;
	if(NULL == _table)
	{
		return -2;
	}
	cfg_parser cp;
	for(int i = 1; i <= _table->m_recordCount; ++i)
	{
		int id = _table->GetField(i, "BasicID");

		config_basic *p = NULL;
		map<int, config_basic *>::iterator it = _cp.find(id);
		if(it != _cp.end())
		{
			p = it->second;
			printf("basic table has repeated id:%d\n", id);
		}
		else
		{
			p = new config_basic;
		}
		p->value = _table->GetField(i, "BasicValue");
		_cp.insert(pair<uint32, config_basic *>(id, p));
	}

    _physical_max  = get_basic_value(10003);
    _physical_tick = get_basic_value(10004);
    _physical_add  = get_basic_value(10005);

	return 0;
}

int config_basic_mgr::get_basic_value(int id)const
{
	std::map<int, config_basic *>::const_iterator it = _cp.find(id);
	if(it == _cp.end())
	{
		return -1;
	}
	return it->second->value;
}

config_basic_mgr::~config_basic_mgr()
{
	std::map<int, config_basic *>::iterator it;
	for(it = _cp.begin(); it != _cp.end(); ++it)
	{
		delete it->second;
	}
	_cp.clear();
}
