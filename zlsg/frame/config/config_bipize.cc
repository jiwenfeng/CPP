#include "cfg_parser.h"
#include "config_bipize.h"

config_bipize::config_bipize(void)
{
}

config_bipize::~config_bipize(void)
{
}

config_bipize_mgr::config_bipize_mgr(TTableGroup_s* p)
        : _bipize(p)
{
    init(p);
}


config_bipize_mgr::~config_bipize_mgr(void)
{
    clear();
}

int config_bipize_mgr::init(TTableGroup_s* p)
{
    if (p == NULL) {
        return -1;
    }

    _bipize = p;
    _table = _bipize->GetTable("t_equipfluctuate");
    if (_table == NULL) {
        printf("not found t_equipfluctuate\n");
        return -2;
    }

    cfg_parser cp;
    for (int i = 1; i <= _table->m_recordCount; ++i) {
        if (_table->GetField      (i, "RangeID"         ) == 0) {
            continue;
        }

        config_bipize* p = new config_bipize;

        p->range_id                         = _table->GetField      (i, "RangeID"					  );
		p->coin_lower_limit                 = _table->GetField      (i, "CoinLowerLimit"              );
		p->ore1_lower_limit                 = _table->GetField      (i, "Ore1LowerLimit"              );
		p->ore2_lower_limit                 = _table->GetField      (i, "Ore2LowerLimit"              );


		_cp.insert(std::pair<int32, config_bipize*>(p->range_id,p));
    }

    return 0;
}

int config_bipize_mgr::clear()
{
    for (std::map<uint32, config_bipize*>::iterator i = _cp.begin(); i != _cp.end(); ++i) {
        if (i->second != NULL) {
            delete i->second;
        }
    }

    _cp.clear();

    return 0;
}

const config_bipize* config_bipize_mgr::get_config_bipize(uint32 range_id) const
{
    std::map<uint32, config_bipize*>::const_iterator itr = _cp.find(range_id);

	if (itr != _cp.end()){
		return itr->second;
	}

	return NULL;
}
