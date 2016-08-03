#include "cfg_parser.h"
#include "config_equip_upgrade.h"

config_equip_upgrade::config_equip_upgrade(void)
{
}

config_equip_upgrade::~config_equip_upgrade(void)
{
}

config_equip_upgrade_mgr::config_equip_upgrade_mgr(TTableGroup_s* p)
        : _equip_upgrade(p)
{
    init(p);
}


config_equip_upgrade_mgr::~config_equip_upgrade_mgr(void)
{
    clear();
}

int config_equip_upgrade_mgr::init(TTableGroup_s* p)
{
    if (p == NULL) {
        return -1;
    }

    _equip_upgrade = p;
    _table = _equip_upgrade->GetTable("t_equipupgrade");
    if (_table == NULL) {
        printf("not found equipupgrade\n");
        return -2;
    }

    cfg_parser cp;
    for (int i = 1; i <= _table->m_recordCount; ++i) {
        //if (_table->GetField      (i, "UpgradeLevel"         ) == 0) {
        //    continue;
        //}

        config_equip_upgrade* p = new config_equip_upgrade;
        p->upgrade_level                   = _table->GetField      (i, "UpgradeLevel"                   );
		p->upgrade_stone_ratio             = _table->GetField      (i, "UpgradeStoneRatio"              );
		p->upgrade_coin_ratio              = _table->GetField      (i, "UpgradeCoinRatio"               );
		p->start_stone_ration              = _table->GetField      (i, "StartStoneRation"               );
		p->upgrade_stone_id                = _table->GetField      (i, "UpgradeStoneID"                 );
		p->start_stone_id                  = _table->GetField      (i, "StartStoneID"                   );



		_cp.insert(std::pair<int32, config_equip_upgrade*>(p->upgrade_level,p));
    }

    return 0;
}

int config_equip_upgrade_mgr::clear()
{
    for (std::map<uint32, config_equip_upgrade*>::iterator i = _cp.begin(); i != _cp.end(); ++i) {
        if (i->second != NULL) {
            delete i->second;
        }
    }

    _cp.clear();

    return 0;
}

const config_equip_upgrade* config_equip_upgrade_mgr::get_config_equip_upgrade(uint32 upgrade_level) const
{
    std::map<uint32, config_equip_upgrade*>::const_iterator itr = _cp.find(upgrade_level);

	if (itr != _cp.end()){
		return itr->second;
	}

	return NULL;
}
