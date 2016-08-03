#include "config_target.h"
#include "data_role.h"

void config_target_mgr::init_less_cond(const char *str, client::cond_less *less)
{
	int value = 0;
	size_t len = strlen(str);
	for(size_t i = 0; i < len; ++i)
	{
		if(str[i] == '|')
		{
			less->add_value(value);
			value = 0;
			++i;
		}
		value = value * 10 + str[i] - '0';
	}
	less->add_value(value);
}

void config_target_mgr::init_great_cond(const char *str, client::cond_great *great)
{
	int value = 0;
	size_t len = strlen(str);
	for(size_t i = 0; i < len; ++i)
	{
		if(str[i] == '|')
		{
			great->add_value(value);
			value = 0;
			++i;
		}
		value = value * 10 + str[i] - '0';
	}
	great->add_value(value);
}

void config_target_mgr::init_equal_cond(const char *str, client::cond_equal *equal)
{
	int value = 0;
	size_t len = strlen(str);
	for(size_t i = 0; i < len; ++i)
	{
		if(str[i] == '|')
		{
			equal->add_value(value);
			value = 0;
			++i;
		}
		value = value * 10 + str[i] - '0';
	}
	equal->add_value(value);
}

void config_target_mgr::init_one_cond(const char *str, client::target_complete_cond &tcc)
{
	switch(str[0])
	{
		case '&':
			init_equal_cond(str + 1, tcc.add_equal());
			break;
		case '>':
			init_great_cond(str + 1, tcc.add_great());
			break;
		case '<':
			init_less_cond(str + 1, tcc.add_less());
			break;
	}
}

void config_target_mgr::init_complete_cond(const char *cstr, client::target_complete_cond &tcc)
{
	size_t len = strlen(cstr);
	if(len < 1)
	{
		return;
	}
	const char *next = NULL;
	char *str = new char[len + 1];
	char *savedptr = NULL;
	strcpy(str, cstr);
#ifndef _WIN32
	next = strtok_r(str, ";", &savedptr);
#else
	next = strtok_s(str, ";", &savedptr);
#endif
	while(next != NULL)
	{
		init_one_cond(next, tcc);

#ifndef _WIN32
		next = strtok_r(NULL, ";", &savedptr);
#else
		next = strtok_s(NULL, ";", &savedptr);
#endif
	}
	delete []str;
}

config_target_mgr::~config_target_mgr()
{
//	_subtype_list.clear();
	map<uint32, config_target *>::iterator it;
	for(it = _target_list.begin(); it != _target_list.end(); ++it)
	{
		delete it->second;
	}
	_target_list.clear();
}

int config_target_mgr::init(TTableGroup_s *p)
{
	if(!p)
	{
		return -1;
	}
	_target = p;
	_table = _target->GetTable("t_target");;
	if(NULL == _table)
	{
		return -2;
	}
	cfg_parser cp;
	for(int i = 1; i <= _table->m_recordCount; ++i)
	{
		uint32 id = _table->GetField(i, "TargetID");
		if(_target_list.find(id) != _target_list.end())
		{
			continue;
		}
		config_target *t = new config_target;
		t->id = id;
		t->type = _table->GetField(i, "Type");
		t->subtype = _table->GetField(i, "SubType");
		t->line = _table->GetField(i, "LineID");
		t->prev = _table->GetField(i, "PTargetID");
		t->next = _table->GetField(i, "NTargetID");
		t->complete = _table->GetField(i, "Show");
		init_complete_cond(_table->GetFieldString(i, "Condition").c_str(), t->tcc);
		t->level_limit = _table->GetField(i, "Trigger1");
		t->mount_limit = _table->GetField(i, "Trigger2");
		t->hour_limit = _table->GetField(i, "Hour");
		t->last = _table->GetField(i, "Last");
		cpf_0 f0;
		f0.set_cfg(&t->items);
		cp.parser<cpf_0>(_table->GetFieldString(i, "Items").c_str(), ";", f0);
		t->ingot = _table->GetField(i, "Ingot");
		t->copper = _table->GetField(i, "Copper");
		t->power = _table->GetField(i, "Power");
		t->exp = _table->GetField(i, "Experience");
		t->taolve = _table->GetField(i, "YuanLi");
		t->description = _table->GetFieldString(i, "Description");
		_target_list.insert(make_pair(t->id, t));
		_sub[t->subtype].push_back(t);
	}
	return 0;
}

const config_target * config_target_mgr::get_config_target(int id) const
{
	map<uint32, config_target *>::const_iterator it = _target_list.find(id);
	if(it == _target_list.end())
	{
		return NULL;
	}
	return it->second;
}

const vector<config_target *> &config_target_mgr::get_sub_targets(uint32 subtype) const
{
	map<uint32, vector<config_target *> >::const_iterator i = _sub.find(subtype);
	if(i == _sub.end())
	{
		static vector<config_target *> v;
		return v;
	}
	return i->second;
}
