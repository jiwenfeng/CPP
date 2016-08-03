#include "cfg_parser.h"
#include "config_cast_equip.h"

config_cast_equip::config_cast_equip(void)
{
}

config_cast_equip::~config_cast_equip(void)
{
}

config_cast_equip_mgr::config_cast_equip_mgr(TTableGroup_s* p)
        : _cast_equip(p)
{
    init(p);
}


config_cast_equip_mgr::~config_cast_equip_mgr(void)
{
    clear();
}

int config_cast_equip_mgr::init(TTableGroup_s* p)
{
    if (p == NULL) {
        return -1;
    }

    _cast_equip = p;
    _table = _cast_equip->GetTable("t_cast");
    if (_table == NULL) {
        printf("not found cast_equip\n");
        return -2;
    }

    cfg_parser cp;
    for (int i = 1; i <= _table->m_recordCount; ++i) {
        if (_table->GetField      (i, "order_id"         ) == 0) {
            continue;
        }

        config_cast_equip* p = new config_cast_equip;
        p->_order_id                         = _table->GetField      (i, "order_id"              );
		p->_need_melting                     = _table->GetField      (i, "need_melting"          );

		cpf_1 f0;// map
		f0.set_cfg(&p->_show_level);
		cp.parser<cpf_1>(_table->GetFieldString(i, "show_level").c_str(), ";", f0);

		cpf_cast_equip ce(&p->_item_data);
		cp.parser<cpf_cast_equip>(_table->GetFieldString(i, "item_data").c_str(), ";", ce);

		_cp.insert(std::pair<int32, config_cast_equip*>(p->_order_id,p));

		//for (std::map<uint32, config_cast_equip*>::const_iterator it = _cp.begin();it!= _cp.end();++it)
		//{
		//	printf("ID = %d", it->first);
		//	for (std::map<int,cast_equip>::const_iterator itr = it->second->_item_data.begin();itr != it->second->_item_data.end();++itr)
		//	{
		//		printf("¸ÅÂÊÊÇ = %d", itr->second.probability);
		//	}
		//}
    }

    return 0;
}

int config_cast_equip_mgr::clear()
{
    for (std::map<uint32, config_cast_equip*>::iterator i = _cp.begin(); i != _cp.end(); ++i) {
        if (i->second != NULL) {
            delete i->second;
        }
    }

    _cp.clear();

    return 0;
}

const config_cast_equip* config_cast_equip_mgr::get_config_cast_equip(uint32 equip_id) const
{
	std::map<uint32,config_cast_equip*>::const_iterator itr = _cp.find(equip_id);
	if (itr == _cp.end()){
		return NULL;
	}

	return itr->second;
}
