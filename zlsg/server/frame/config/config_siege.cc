#include "config_siege.h"

const string &config_siege::get_model_id(uint32 pro_id)
{
	map<uint32, string>::iterator it = model.find(pro_id);
	if(it == model.end())
	{
		static string ret = "";
		return ret;
	}
	return it->second;
}

int config_siege_mgr::init(TTableGroup_s *p)
{
	if(!p)
	{
		return -1;
	}
	_siege = p;
	_table = _siege->GetTable("t_capital");;
	if(NULL == _table)
	{
		return -2;
	}
	cfg_parser cp;
	for(int i = 1; i <= _table->m_recordCount; ++i)
	{
		int id = _table->GetField(i, "CapitalID");
		config_siege *p = NULL;
		if(_cp.find(id) != _cp.end())
		{
			p = _cp[i];
		}
		else
		{
			p = new config_siege;
		}
		p->model.insert(make_pair(0, _table->GetFieldString(i, "SoldierImage")));
//		p->model.insert(pair<uint32, string>(1, _table->GetFieldString(i, "MasterImage")));
		p->model.insert(make_pair(2, _table->GetFieldString(i, "NmasterImage")));
//		p->model.insert(pair<uint32, string>(3, _table->GetFieldString(i, "TaoistImage")));
		cpf_0 f0;
		f0.set_cfg(&p->item);
		cp.parser<cpf_0>(_table->GetFieldString(i, "CapitalReward").c_str(), ";", f0);
		_cp.insert(pair<uint32, config_siege *>(id, p));
	}
	return 0;
}

const string &config_siege_mgr::get_profession_model(uint32 off_id, uint32 pro_id)const
{
	std::map<int, config_siege *>::const_iterator it = _cp.find(off_id);
	if(it == _cp.end())
	{
		static const string ret = "";
		return ret;
	}
	return it->second->get_model_id(pro_id);
}

const map<int, int> &config_siege_mgr::get_reward(uint32 off_id) const
{
	std::map<int, config_siege *>::const_iterator it = _cp.find(off_id);
	if(it == _cp.end())
	{
		static map<int, int> m;
		m.insert(pair<int, int>(0, 0));
		return m;
	}
	return it->second->get_reward();
}

config_siege_mgr::~config_siege_mgr()
{
	map<int, config_siege *>::iterator it = _cp.begin();
	for(; it != _cp.end(); ++it)
	{
		delete it->second;
	}
	_cp.clear();
}
