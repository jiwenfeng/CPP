#include "config_recharge.h"

config_recharge_mgr::~config_recharge_mgr()
{
	std::map<uint32, config_recharge *>::iterator it = _cr.begin();
	for(; it != _cr.end(); ++it)
	{
		delete it->second;
	}
	_cr.clear();
}

int config_recharge_mgr::init(TTableGroup_s *p)
{
	if(!p)
	{
		return -1;
	}
	_recharge = p;
	_table = _recharge->GetTable("t_recharge");;
	if(NULL == _table)
	{
		return -2;
	}
	cfg_parser cp;
	for(int i = 1; i <= _table->m_recordCount; ++i)
	{
		uint32 id = _table->GetField(i, "ID");
		if(_cr.find(id) != _cr.end())
		{
			continue;
		}
		config_recharge *cr = new config_recharge;
		cr->_id = id;
		cr->_type = _table->GetField(i, "Type");
//		cr->_price = _table->GetField(i, "Price");
		cr->_ingot = _table->GetField(i, "Ingot");
		cr->_times = _table->GetField(i, "Times");
		_min_recharge = _min_recharge > cr->_ingot ? cr->_ingot : _min_recharge;
		_cr.insert(make_pair(id, cr));
	}
	return 0;
}

const config_recharge * config_recharge_mgr::get_config_recharge(uint32 id)const
{
	std::map<uint32, config_recharge *>::const_iterator it = _cr.find(id);
	if(it == _cr.end())
	{
		return NULL;
	}
	return it->second;
}

const config_recharge *config_recharge_mgr::get_config_recharge_by_n(uint32 n) const
{
	std::map<uint32, config_recharge *>::const_reverse_iterator i;
	for(i = _cr.rbegin(); i != _cr.rend(); ++i)
	{
		if(i->second->get_ingot() <= n)
		{
			return i->second;
		}
	}
	return NULL;
}
