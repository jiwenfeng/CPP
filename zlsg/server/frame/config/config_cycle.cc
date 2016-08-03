#include "cfg_parser.h"
#include "config_cycle.h"

config_cycle::config_cycle(void)
{
}

config_cycle::~config_cycle(void)
{
}

config_cycle_mgr::config_cycle_mgr(TTableGroup_s* p)
    : _group(p)
{
	_max = 0;
    init(p);
}

config_cycle_mgr::~config_cycle_mgr(void)
{
    clear();
}

int config_cycle_mgr::init(TTableGroup_s* p)
{
    if (p == NULL) {
        return -1;
    }

    _group = p;
    _table = _group->GetTable("t_scgiant");
    if (_table == NULL) {
        printf("not found t_scgiant\n");
        return -2;
    }

    cfg_parser cp;
    for (int i = 1; i <= _table->m_recordCount; ++i) {
        config_cycle* p = new config_cycle;
		p->_id = _table->GetField(i, "GiantID");
		p->_monster_id = _table->GetField(i, "MonsterID");
		p->_difficulty = _table->GetField(i, "Difficulty");
		p->_fast_reward = _table->GetField(i, "FastReward");
		p->_first_reward = _table->GetField(i, "FirstReward");
		p->_again_reward = _table->GetField(i, "AgainReward");
		if(p->_id > _max)
		{
			_max = p->_id;
		}
		_cfg.insert(make_pair(p->_id, p));
		if(_start.find(p->_difficulty) == _start.end())
		{
			_start[p->_difficulty] = p->_id;
		}
    }

    return 0;
}

int config_cycle_mgr::clear()
{
    for (std::map<uint32, config_cycle*>::iterator i = _cfg.begin(); i != _cfg.end(); ++i) {
        if (i->second != NULL) {
            delete i->second;
        }
    }

    _cfg.clear();

    return 0;
}

const config_cycle* config_cycle_mgr::get_config_cycle(uint32 id) const
{
    std::map<uint32, config_cycle*>::const_iterator i = _cfg.find(id);
    if (i == _cfg.end()) {
        return NULL;
    }
    return i->second;
}

const uint32 config_cycle_mgr::get_start_level(uint32 diff) const
{
	std::map<uint32, uint32>::const_iterator i = _start.find(diff);
	if(i == _start.end())
	{
		return 0;
	}
	return i->second;
}
