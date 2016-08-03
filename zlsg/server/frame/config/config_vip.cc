#include "config_vip.h"

config_vip_mgr::~config_vip_mgr()
{
	std::vector<config_vip *>::iterator it = _cv.begin();
	for(; it != _cv.end(); ++it)
	{
		delete *it;
	}
	_cv.clear();
}

int config_vip_mgr::init(TTableGroup_s *p)
{
	if(!p)
	{
		return -1;
	}
	_vip = p;
	_table = _vip->GetTable("t_privilege");;
	if(NULL == _table)
	{
		return -2;
	}
	cfg_parser cp;
	const char *op[] = {"Pphysical", "Pcopper", "Pcarbon2", "Parena", "Pcskills", "Pmskills", "SCRefresh", "MHRefresh"};
#define n (sizeof(op) / sizeof(op[0]))
	for(int i = 1; i <= _table->m_recordCount; ++i)
	{
		int id = _table->GetField(i, "VipLv");
		config_vip *p = new config_vip;
		p->_lv = id;
		p->_need = _table->GetField(i, "Pingot");
		p->_grid = _table->GetField(i, "Plattice");
		p->_sweep = _table->GetField(i, "Psweep");
		p->_refresh_count = _table->GetField(i, "BStoreRefresh");
		for(size_t j = 0; j < n; ++j)
		{
			p->_op.insert(make_pair(j + 1, _table->GetField(i, op[j])));
		}
		_cv.push_back(p);
	}
	return 0;
}

const config_vip * config_vip_mgr::get_config_vip(uint32 id)const
{
	if(id < 0 || id >= _cv.size())
	{
		return NULL;
	}
	return _cv[id];
}
