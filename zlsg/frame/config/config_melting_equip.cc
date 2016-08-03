#include "cfg_parser.h"
#include "config_melting_equip.h"

config_melting_equip::config_melting_equip(void)
{
}

config_melting_equip::~config_melting_equip(void)
{
}

config_melting_equip_mgr::config_melting_equip_mgr(TTableGroup_s* p)
        : _melting_equip(p)
{
    init(p);
}


config_melting_equip_mgr::~config_melting_equip_mgr(void)
{
    clear();
}

int config_melting_equip_mgr::init(TTableGroup_s* p)
{
    if (p == NULL) {
        return -1;
    }

    _melting_equip = p;
    _table = _melting_equip->GetTable("t_melting");
    if (_table == NULL) {
        printf("not found melting_equip\n");
        return -2;
    }

    cfg_parser cp;
    for (int i = 1; i <= _table->m_recordCount; ++i) {
        if (_table->GetField      (i, "item_id"         ) == 0) {
            continue;
        }

        config_melting_equip* p = new config_melting_equip;
        p->item_id                           = _table->GetField      (i, "item_id"               );
        p->improve_level                     = _table->GetField      (i, "improve_level"         );
        p->melting_value                     = _table->GetField      (i, "melting_value"         );

		_cp.insert(std::pair<int32, config_melting_equip*>(p->item_id,p));
    }

    return 0;
}

int config_melting_equip_mgr::clear()
{
    for (std::multimap<uint32, config_melting_equip*>::iterator i = _cp.begin(); i != _cp.end(); ++i) {
        if (i->second != NULL) {
            delete i->second;
        }
    }

    _cp.clear();

    return 0;
}

const config_melting_equip* config_melting_equip_mgr::get_config_melting_equip(uint32 item_id,int improve_level) const
{
	std::multimap<uint32,config_melting_equip*>::const_iterator itr = _cp.find(item_id);
	for (;itr != _cp.end(); ++itr){
		if (itr->second->get_improve_level() == improve_level && itr->first == item_id){
			return itr->second;
		}
	}

	return NULL;
}
