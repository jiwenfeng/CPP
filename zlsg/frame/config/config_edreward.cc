#include "config_edreward.h"
#include "data_role.h"

config_edreward::~config_edreward()
{
	_reward.clear();
}

int config_edreward_mgr::init(TTableGroup_s *p)
{
	if(!p)
	{
		return -1;
	}
	_edreward = p;
	_table = _edreward->GetTable("t_everydayreward");;
	if(NULL == _table)
	{
		return -2;
	}
	cfg_parser cp;
	for(int i = 1; i <= _table->m_recordCount; ++i)
	{
		config_edreward *p = new config_edreward;
		p->min_rank = _table->GetField(i, "RankMin");
		p->max_rank = _table->GetField(i, "RankMax");
		p->id = _table->GetField(i, "RewardPack");
		int n = _table->GetField(i, "YuanBao");
		if(n > 0)
		{
			p->_reward.push_back(make_pair(INGOT, n));
		}
		n = _table->GetField(i, "Money");
		if(n > 0)
		{
			p->_reward.push_back(make_pair(TAEL, n));
		}
		n = _table->GetField(i, "Military");
		if(n > 0)
		{
			p->_reward.push_back(make_pair(MILITARY_STRATEGY, n));
		}
		n = _table->GetField(i, "Physical");
		if(n > 0)
		{
			p->_reward.push_back(make_pair(PHYSICAL_POWER, n));
		}

		n = _table->GetField(i, "Experience");
		if(n > 0)
		{
			p->_reward.push_back(make_pair(EXPERIENCE, n));
		}


		n = _table->GetField(i, "Reputation");
		if(n > 0)
		{
			p->_reward.push_back(make_pair(POPULARITY, n));
		}

		n = _table->GetField(i, "JunGong");
		if(n > 0)
		{
			p->_reward.push_back(make_pair(SIEGE_REPUTATION, n));
		}
		_cp.push_back(p);
	}
	return 0;
}

const config_edreward * config_edreward_mgr::get_edreward(int rank) const
{
	for(size_t i = 0; i < _cp.size(); ++i)
	{
		if(_cp[i]->min_rank <= rank && rank <= _cp[i]->max_rank)
		{
			return _cp[i];
		}
	}
	return NULL;
}

config_edreward_mgr::~config_edreward_mgr()
{
	for(size_t i = 0; i < _cp.size(); ++i)
	{
		delete _cp[i];
	}
	_cp.clear();
}
