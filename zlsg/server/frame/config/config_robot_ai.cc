#include "cfg_parser.h"
#include "random.h"
#include "config_robot_ai.h"

int config_robot_ai_mgr::init(TTableGroup_s* p)
{
    if (p == NULL) {
        return -1;
    }

    _robot = p;
    _table = _robot->GetTable("t_robotAI");
    if (_table == NULL) {
        printf("not found t_robotAI\n");
        return -2;
    }

    for (int i = 1; i <= _table->m_recordCount; ++i) {
		config_robot_ai *ai = new config_robot_ai;
		ai->_id = _table->GetField(i, "RobotAIID");
		ai->_type = _table->GetField(i, "RobotAIType");
		ai->_time = _table->GetField(i, "RobotAITime");
		ai->_ai = _table->GetFieldString(i, "RobotAI");
		_ai.insert(make_pair(ai->_id, ai));
		if(ai->_type == 1)
		{
			_attack.push_back(ai->_id);
		}
		else
		{
			_defence.push_back(ai->_id);
		}
    }

    return 0;
}

void config_robot_ai_mgr::clear()
{
	map<uint32, config_robot_ai *>::iterator i;
	for(i = _ai.begin(); i != _ai.end(); ++i)
	{
		delete i->second;
	}
	_ai.clear();
	_attack.clear();
	_defence.clear();
}

config_robot_ai_mgr::~config_robot_ai_mgr()
{
	clear();
}


const config_robot_ai *config_robot_ai_mgr::get_config_robot_ai(uint32 id) const
{
	map<uint32, config_robot_ai *>::const_iterator i = _ai.find(id);
	if(i == _ai.end())
	{
		return NULL;
	}
	return i->second;
}

const config_robot_ai* config_robot_ai_mgr::random_config_robot_ai(uint32 type) const
{
	uint32 id = 0;
	if(type == 1)
	{
		if(_attack.empty())
		{
			return NULL;
		}
		id = _attack[mtrandom::rand_mid32(0, _attack.size() - 1)];
	}
	else
	{
		if(_defence.empty())
		{
			return NULL;
		}
		id = _defence[mtrandom::rand_mid32(0, _defence.size() - 1)];
	}
	return get_config_robot_ai(id);
}
