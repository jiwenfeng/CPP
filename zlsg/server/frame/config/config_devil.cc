#include "cfg_parser.h"
#include "config_mgr.h"
#include "config_devil.h"

config_devil_mgr::config_devil_mgr(TTableGroup_s* p) : _npc(p)
{
	init(p);
}

config_devil_mgr::~config_devil_mgr(void)
{
	clear();
}

int config_devil_mgr::init(TTableGroup_s* p)
{
	if (p == NULL) {
		return -1;
	}

	_npc = p;
	_table = _npc->GetTable("t_mhgiant");
	if(_table == NULL) 
	{
		printf("not found t_devil\n");
		return -2;
	}
	for(int i = 1; i <= _table->m_recordCount; ++i) 
	{
		uint32 id = _table->GetField(i, "MapID");
		if(_devils.find(id) != _devils.end())
		{
			printf("t_scgiant repeat raid_id:%u\n", id);
			continue;
		}
		config_devil *cd = new config_devil;
		cd->_diffculty = _table->GetField(i, "Difficulty");
		uint32 reward = _table->GetField(i, "FifteenReward");
		cd->_fast_reward = _table->GetField(i, "FastReward");
		cd->_1st_reward = _table->GetField(i, "FirstReward");
		cd->_again_reward = _table->GetField(i, "AgainReward");
		cd->_map_id = id;
		_devils.insert(make_pair(id, cd));
		_stage[cd->_diffculty].push_back(cd->_map_id);
		if(reward != 0)
		{
			_reward[cd->_diffculty] = reward;
		}
	}
	return 0;
}

int config_devil_mgr::clear()
{
	map<uint32, config_devil *>::iterator i;
	for(i = _devils.begin(); i != _devils.end(); ++i)
	{
		delete i->second;
	}
	_devils.clear();
	map<uint32, vector<uint32> >::iterator j;
	for(j = _stage.begin(); j != _stage.end(); ++j)
	{
		j->second.clear();
	}
	_stage.clear();
	return 0;
}

const config_devil *config_devil_mgr::get_config_devil(uint32 raid_id) const 
{
	map<uint32, config_devil *>::const_iterator i = _devils.find(raid_id);
	if(i == _devils.end())
	{
		return NULL;
	}
	return i->second;
}

const vector<uint32> &config_devil_mgr::get_all_maps_by_stage(uint32 id) const
{
	map<uint32, vector<uint32> >::const_iterator i = _stage.find(id);
	if(i == _stage.end())
	{
		static vector<uint32> v;
		return v;
	}
	return i->second;
}

const uint32 config_devil_mgr::get_stage_reward(uint32 id) const
{
	map<uint32, uint32>::const_iterator i = _reward.find(id);
	if(i == _reward.end())
	{
		return 0;
	}
	return i->second;
}
