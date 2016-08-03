#include "cfg_parser.h"
#include "config_gift.h"

config_gift::config_gift(void)
{
}

config_gift::~config_gift(void)
{
}

config_gift_mgr::config_gift_mgr(TTableGroup_s* p)
        : _gift(p)
{
    init(p);
}


config_gift_mgr::~config_gift_mgr(void)
{
    clear();
}

int config_gift_mgr::init(TTableGroup_s* p)
{
    if (p == NULL) {
        return -1;
    }

    _gift = p;
    _table = _gift->GetTable("t_pack");
    if (_table == NULL) {
        printf("not found pack\n");
        return -2;
    }

    cfg_parser cp;
    for (int i = 1; i <= _table->m_recordCount; ++i) {
        if (_table->GetField      (i, "item_pack_id"         ) == 0) {
            continue;
        }

        config_gift* p = new config_gift;
        p->gift_id                         = _table->GetField      (i, "item_pack_id"              );
		p->item_list                         = _table->GetField    (i, "item_list"                 );

		//cpf_gift ce(&p->item_list);
		//cp.parser<cpf_gift>(_table->GetFieldString(i, "item_list").c_str(), ";", ce);

		_cp.insert(std::pair<int32, config_gift*>(p->gift_id,p));
    }

    return 0;
}

int config_gift_mgr::clear()
{
    for (std::map<uint32, config_gift*>::iterator i = _cp.begin(); i != _cp.end(); ++i) {
        if (i->second != NULL) {
            delete i->second;
        }
    }

    _cp.clear();

    return 0;
}

const config_gift* config_gift_mgr::get_config_gift(uint32 gift_id) const
{
    std::map<uint32, config_gift*>::const_iterator itr = _cp.find(gift_id);

	if (itr != _cp.end()){
		return itr->second;
	}

	return NULL;
}
