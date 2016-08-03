#include "cfg_parser.h"
#include "config_treasure.h"

config_treasure::config_treasure(void)
{
}

config_treasure::~config_treasure(void)
{
}

config_treasure_mgr::config_treasure_mgr(TTableGroup_s* p)
        : _treasure(p)
{
    init(p);
}


config_treasure_mgr::~config_treasure_mgr(void)
{
    clear();
}

int config_treasure_mgr::init(TTableGroup_s* p)
{
    if (p == NULL) {
        return -1;
    }

    _treasure = p;
    _table = _treasure->GetTable("t_BlackShop");
    if (_table == NULL) {
        printf("not found t_BlackShop\n");
        return -2;
    }

    cfg_parser cp;
    for (int i = 1; i <= _table->m_recordCount; ++i) {
        if (_table->GetField      (i, "LotteryID"         ) == 0) {
            continue;
        }

        config_treasure* p = new config_treasure;
		p->key_id										= _table->GetField      (i, "LotteryID"					  );
        p->item_idx										= _table->GetField      (i, "ItemID"					  );
		p->amount_max_today                             = _table->GetField      (i, "AmountMaxToday"              );
		p->probability								    = _table->GetField      (i, "Probability"                 );
		p->single_price								    = _table->GetField      (i, "Price"                       );
		p->level_min								    = _table->GetField      (i, "LevelMin"                    );
		p->level_max								    = _table->GetField      (i, "LevelMax"                    );
		p->min_quantity								    = _table->GetField      (i, "MinQuantity"                 );
		p->max_quantity								    = _table->GetField      (i, "MaxQuantity"                 );

		_cp.insert(std::pair<uint32, config_treasure*>(p->key_id,p));
    }

    return 0;
}

int config_treasure_mgr::clear()
{
    for (std::map<uint32, config_treasure*>::iterator i = _cp.begin(); i != _cp.end(); ++i) {
        if (i->second != NULL) {
            delete i->second;
        }
    }

    _cp.clear();

    return 0;
}

const config_treasure* config_treasure_mgr::get_config_treasure(uint32 key_id) const
{
    std::map<uint32, config_treasure*>::const_iterator itr = _cp.find(key_id);

	if (itr == _cp.end()){
		return NULL;
	}

	return itr->second;
}

const std::map<uint32, config_treasure*>& config_treasure_mgr::get_config_treasure_map() const
{
	return _cp;
}