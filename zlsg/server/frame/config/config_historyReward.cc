#include "config_historyReward.h"
#include <algorithm>
#include <cmath>
#define ARENA_RANK_UPPER 30000	 // 竞技场奖励上限
class cmp
{
private:
	int _n;
public:
	cmp(int n):_n(n) {}
	bool operator()(const config_history *h)
	{
		return h->_min <= _n && _n <= h->_max;
	}
};

int config_history_mgr::init(TTableGroup_s *p)
{
	if(!p)
	{
		return -1;
	}
	_history = p;
	_table = _history->GetTable("t_historyReward");;
	if(NULL == _table)
	{
		return -2;
	}
	for(int i = 1; i <= _table->m_recordCount; ++i)
	{
		config_history *p = new config_history;
		p->_min = _table->GetField(i, "RankMin");
		p->_max = _table->GetField(i, "RankMax");
		p->_rate = _table->GetField(i, "RankRate");
		p->_float = _table->GetField(i, "RankFloat");
		_v.push_back(p);
	}
	return 0;
}

double config_history_mgr::get_reward(int history, int cur) const
{
	if(history - 1 <= 0 || cur > ARENA_RANK_UPPER)
	{
		return 0;
	}
	history = min(history, ARENA_RANK_UPPER + 1);
	std::vector<config_history *>::const_iterator it = find_if(_v.begin(), _v.end(), cmp(history - 1));
	if(it == _v.end())
	{
		return 0;
	}
	double n = 0.0f;
	if((*it)->_min > cur)
	{
		 n += get_reward((*it)->_min, cur) + max((history - (*it)->_min), 1) * (*it)->_rate / 10000.0f + (*it)->_float;
	}
	else
	{
		n = max((history - cur), 1) * (*it)->_rate / 10000.0f + (*it)->_float;
	}
	return n;
}

config_history_mgr::~config_history_mgr()
{
	std::vector<config_history *>::iterator it;
	for(it = _v.begin(); it != _v.end(); ++it)
	{
		delete *it;
	}
	_v.clear();
}
