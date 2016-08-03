#include "config_reward.h"

int config_reward_mgr::init(TTableGroup_s *p)
{
	if(!p)
	{
		return -1;
	}
	_reward = p;
	_table = _reward->GetTable("t_reward");;
	if(NULL == _table)
	{
		return -2;
	}
	cfg_parser cp;
	for(int i = 1; i <= _table->m_recordCount; ++i)
	{
		int id = _table->GetField(i, "RewardID");
		config_reward *p = NULL;
		if(_cp.find(id) != _cp.end())
		{
			p = _cp[i];
		}
		else
		{
			p = new config_reward;
		}
		p->system_id = _table->GetField(i, "SystemID");
		p->reward_condt = _table->GetField(i, "RewardCondition");
		cpf_0 f0;
		f0.set_cfg(&p->rank);
		cp.parser<cpf_0>(_table->GetFieldString(i, "Rank").c_str(), ";", f0);
		p->integral = _table->GetField(i, "Integral");
		p->reward_pack = _table->GetField(i, "RewardPack");
		p->money = _table->GetField(i, "Money");
		p->military = _table->GetField(i, "Military");
		p->physical = _table->GetField(i, "Physical");
		p->experience = _table->GetField(i, "experience");
		p->reputation = _table->GetField(i, "Reputation");
		p->jungong = _table->GetField(i, "Jungong");
		_cp.insert(pair<uint32, config_reward *>(id, p));
	}
	return 0;
}

#if 0
const config_reward * config_reward_mgr::get_reward(int id)const
{
	std::map<int, config_reward *>::const_iterator it = _cp.find(id);
	if(it == _cp.end())
	{
		return NULL;
	}
	return it->second;
}
#endif

config_reward_mgr::~config_reward_mgr()
{
	std::map<int, config_reward *>::iterator it;
	for(it = _cp.begin(); it != _cp.end(); ++it)
	{
		delete it->second;
	}
	_cp.clear();
}
