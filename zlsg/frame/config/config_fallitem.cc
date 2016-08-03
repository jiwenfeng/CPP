#include "config_fallitem.h"
#include "random.h"
#include <numeric>

template<class T>
class add
{
public:
	int operator()(int &denominator, const T &v)
	{
		denominator += v.ratio_molecular;
		return denominator;
	}
};

// 掉落组里面的物品不做数量循环
const drop_content *config_packet::get_drop_content()
{
	int rate = mtrandom::rand_int32(total);
	int r = 0;
	for(size_t i = 0; i < list.size(); ++i)
	{
		r += list[i].ratio_molecular;
		if(rate <= r)
		{
			return &list[i];
		}
	}
	return NULL;
}

int config_packet_mgr::init(TTableGroup_s *p)
{
	if(!p)
	{
		return -1;
	}
	_group = p;
	_table = _group->GetTable("t_fallteam");;
	if(NULL == _table)
	{
		return -2;
	}
	cfg_parser cp;
	for(int i = 1; i <= _table->m_recordCount; ++i)
	{
		int id = _table->GetField(i, "DropGroupID");
		config_packet *p = NULL;
		if(_cp.find(id) != _cp.end())
		{
			p = _cp[i];
		}
		else
		{
			p = new config_packet;
		}
		cpf_drop_content cdc;
		cdc.set_cfg(&p->list);
		cp.parser<cpf_drop_content>(_table->GetFieldString(i, "ItemContent").c_str(), ";", cdc);
		p->total = accumulate(p->list.begin(), p->list.end(), 
				0, 
				add<drop_content>()
				);
		_cp.insert(pair<int, config_packet *>(id, p));
	}
	return 0;
}

const drop_content *config_packet_mgr::get_config_packet(int id)const
{
	std::map<int, config_packet *>::const_iterator it = _cp.find(id);
	if(it != _cp.end())
	{
		return it->second->get_drop_content();
	}
	return NULL;
}

config_packet_mgr::~config_packet_mgr()
{
	std::map<int, config_packet *>::const_iterator it;
	for(it = _cp.begin(); it != _cp.end(); ++it)
	{
		delete it->second;
	}
	_cp.clear();
}
