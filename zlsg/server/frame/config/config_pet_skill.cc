#include "config_pet_skill.h"

config_pet_skill_mgr::~config_pet_skill_mgr()
{
	std::map<uint32, std::map<uint32, config_pet_skill *> >::iterator i = _list.begin();
	for(; i != _list.end(); ++i)
	{
		std::map<uint32, config_pet_skill *>::iterator j = i->second.begin();
		for(; j != i->second.end(); ++j)
		{
			delete j->second;
		}
		i->second.clear();
	}
	_list.clear();
}

int config_pet_skill_mgr::init(TTableGroup_s *p)
{
	if(!p)
	{
		return -1;
	}
	_pet_skill = p;
	_table = _pet_skill->GetTable("t_rgeneral");;
	if(NULL == _table)
	{
		return -2;
	}
	cfg_parser cp;
	for(int i = 1; i <= _table->m_recordCount; ++i)
	{
		config_pet_skill *cps = new config_pet_skill;
		cps->_id = _table->GetField(i, "RgneralID");
		cps->_lv = _table->GetField(i, "RgeneralLv");
		cps->_default_skill = _table->GetField(i, "RgdefaultSkill");
		for(int j = 1; j < 10; ++j)
		{
			char buf[128] = {0};
			snprintf(buf, 128, "Rgskill%d", j);
			uint32 skill_id = _table->GetField(i, buf);
			if(skill_id == 0)
			{
				break;
			}
			cps->_skills.push_back(skill_id);
		}
        cpf_0 p0(&cps->_attribute);
        cp.parser<cpf_0>(_table->GetFieldString(i, "RgeneralAttribute").c_str(), ";", p0);
		insert_config_pet_skill(cps);
	}
	return 0;
}


const config_pet_skill *config_pet_skill_mgr::get_config_pet_skill(uint32 id, uint32 lv) const
{
	std::map<uint32, std::map<uint32, config_pet_skill *> >::const_iterator it = _list.find(id);
	if(it == _list.end())
	{
		return NULL;
	}
	std::map<uint32, config_pet_skill *>::const_iterator j = it->second.find(lv);
	if(j == it->second.end())
	{
		return NULL;
	}
	return j->second;
}

void config_pet_skill_mgr::insert_config_pet_skill(config_pet_skill * cps)
{
	std::map<uint32, std::map<uint32, config_pet_skill *> >::iterator it = _list.find(cps->get_pet_id());
	if(it == _list.end())
	{
		map<uint32, config_pet_skill *> m;
		m.insert(make_pair(cps->get_pet_level(), cps));
		_list.insert(make_pair(cps->get_pet_id(), m));
	}
	else
	{
		it->second.insert(make_pair(cps->get_pet_level(), cps));
	}
}
