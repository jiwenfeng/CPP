#include "cfg_parser.h"
#include "config_mall.h"

config_mall::config_mall(void)
{
}

config_mall::~config_mall(void)
{
}

config_mall_mgr::config_mall_mgr(TTableGroup_s* p)
        : _mall(p)
{
    init(p);
}


config_mall_mgr::~config_mall_mgr(void)
{
    clear();
}

int config_mall_mgr::init(TTableGroup_s* p)
{
    if (p == NULL) {
        return -1;
    }

    _mall = p;
    _table = _mall->GetTable("t_mall");
    if (_table == NULL) {
        printf("not found mall\n");
        return -2;
    }

    cfg_parser cp;
    for (int i = 1; i <= _table->m_recordCount; ++i) {
        if (_table->GetField      (i, "MallID"         ) == 0) {
            continue;
        }

        config_mall* p = new config_mall;
        p->mall_id                         = _table->GetField      (i, "MallID"              );
		p->item_id                         = _table->GetField      (i, "MuseID"              );
		p->pay_type                        = _table->GetField      (i, "PayType"             );
		p->price_value                     = _table->GetField      (i, "Mprice"              );
		p->daily_quota					   = _table->GetField      (i, "DailyQuota"          );

		cpf_0 f0;
		f0.set_cfg(&p->extra_limit);
		cp.parser<cpf_0>(_table->GetFieldString(i, "ExtraLimit").c_str(), ";", f0);

		_cp.insert(std::pair<int32, config_mall*>(p->mall_id,p));
    }

    return 0;
}

int config_mall_mgr::clear()
{
    for (std::multimap<uint32, config_mall*>::iterator i = _cp.begin(); i != _cp.end(); ++i) {
        if (i->second != NULL) {
            delete i->second;
        }
    }

    _cp.clear();

    return 0;
}

const config_mall* config_mall_mgr::get_config_mall(uint32 mall_id,uint32 item_id) const
{
    std::multimap<uint32, config_mall*>::const_iterator itr = _cp.find(mall_id);

	for (; itr != _cp.end(); ++itr){
		if (itr->second->get_item_id() == item_id && itr->first == mall_id){
			return itr->second;
		}
	}

	return NULL;
}
