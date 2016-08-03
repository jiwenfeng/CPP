#include "config_flag.h"

config_flag_mgr::~config_flag_mgr()
{
	std::map<int, config_flag *>::iterator it = _cp.begin();
	for(; it != _cp.end(); ++it)
	{
		delete it->second;
	}
	_cp.clear();
}

int config_flag_mgr::init(TTableGroup_s *p)
{
	if(!p)
	{
		return -1;
	}
	_flag = p;
	_table = _flag->GetTable("t_flag");;
	if(NULL == _table)
	{
		return -2;
	}
	cfg_parser cp;
	for(int i = 1; i <= _table->m_recordCount; ++i)
	{
		int id = _table->GetField(i, "FlagID");
		config_flag *p = NULL;
		if(_cp.find(id) != _cp.end())
		{
			p = _cp[i];
		}
		else
		{
			p = new config_flag;
		}
		p->flag_id = id;
		p->npc_id = _table->GetField(i, "NpcID");
		_cp.insert(pair<uint32, config_flag *>(id, p));
	}
	return 0;
}

const config_flag * config_flag_mgr::get_flag(int id)const
{
	std::map<int, config_flag *>::const_iterator it = _cp.find(id);
	if(it == _cp.end())
	{
		return NULL;
	}
	return it->second;
}
