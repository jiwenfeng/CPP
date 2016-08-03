#include "cfg_parser.h"
#include "config_mgr.h"
#include "config_giantraid.h"

config_giantraid_mgr::config_giantraid_mgr(TTableGroup_s* p)
: _ts(p)
{
	init(p);
}

config_giantraid_mgr::~config_giantraid_mgr(void)
{
	clear();
}

int config_giantraid_mgr::init(TTableGroup_s* p)
{
	if (p == NULL) {
		return -1;
	}

	_ts = p;
	_table = _ts->GetTable("t_giantraid");
	if (_table == NULL) {
		printf("not found t_giantraid\n");
		return -2;
	}

	cfg_parser cp;
	for (int i = 1; i <= _table->m_recordCount; ++i) {
		uint32 id = _table->GetField      (i, "RaidID"   );
		if (id  == 0) {
			continue;
		}

		config_giantraid* p = new config_giantraid;

        p->_raid_id					= id;
		p->_player_min_level		= _table->GetField(i, "PlayerMinLevel");
		p->_player_max_level		= _table->GetField(i, "PlayerMaxLevel");
        p->_rand_power_min          = _table->GetField(i, "RandPowerMin");
        p->_rand_power_max          = _table->GetField(i, "RandPowerMax");
		p->_rand_lv_min				= _table->GetField(i, "RandLevelMin");
		p->_rand_lv_max				= _table->GetField(i, "RandLevelMax");
		p->_reward_money			= _table->GetField(i, "CoinRate");
		p->_reward_resource			= _table->GetField(i, "Resource");
		p->_index					= _table->GetField(i, "giantIndex");
        cpf f;
        f.set_cfg(&p->_reward_item);
        cp.parser<cpf>(_table->GetFieldString(i, "RewardItem").c_str(), ";", f);
        _cb.push_back(p);
		_cgs.insert(make_pair(p->_raid_id, p));
	}

	return 0;
}

int config_giantraid_mgr::clear()
{
	for (size_t i = 0; i < _cb.size(); ++i)
	{
		if (_cb[i] != NULL) {
			delete _cb[i];
		}
	}
	_cb.clear();
	_cgs.clear();
	return 0;
}

void config_giantraid_mgr::get_config_giantraid(uint32 level, vector<config_giantraid *> &v)
{
	for(size_t i = 0; i < _cb.size(); ++i)
	{
		if(_cb[i]->get_player_min_level() <= level && level <= _cb[i]->get_player_max_level())
		{
			v.push_back(_cb[i]);
		}
	}
}

const config_giantraid *config_giantraid_mgr::get_config_giantraid(uint32 id) const
{
	map<uint32, config_giantraid *>::const_iterator i = _cgs.find(id);
	if(i == _cgs.end())
	{
		return NULL;
	}
	return i->second;
}
