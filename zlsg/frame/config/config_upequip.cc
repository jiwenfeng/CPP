#include "cfg_parser.h"
#include "config_upequip.h"

config_upequip::config_upequip(void)
{
}

config_upequip::~config_upequip(void)
{
}

config_upequip_mgr::config_upequip_mgr(TTableGroup_s* p)
        : _upequip(p)
{
    init(p);
}


config_upequip_mgr::~config_upequip_mgr(void)
{
    clear();
}

int config_upequip_mgr::init(TTableGroup_s* p)
{
    if (p == NULL) {
        return -1;
    }

    _upequip = p;
    _table = _upequip->GetTable("t_strengthen");
    if (_table == NULL) {
        printf("not found strengthen\n");
        return -2;
    }

    cfg_parser cp;
    for (int i = 1; i <= _table->m_recordCount; ++i) {
        if (_table->GetField      (i, "EquipID"         ) == 0) {
            continue;
        }

        config_upequip* p = new config_upequip;
        p->equip_id                         = _table->GetField      (i, "EquipID"              );
        p->improve_level                    = _table->GetField      (i, "ImproveLevel"         );
        p->can_improve                      = _table->GetField      (i, "CanImprove"           );
        p->improve_need_game_money          = _table->GetField      (i, "ImproveNeedGameMoney" );
        p->improve_need_time                = _table->GetField      (i, "ImproveNeedTime"      );
        p->improve_now_need_money           = _table->GetField      (i, "ImproveNowNeedMoney"  );
        p->improve_min_times                = _table->GetField      (i, "ImproveMinTimes"      );
        p->improve_rate                     = _table->GetField      (i, "ImproveRate"          );
        p->improve_max_times                = _table->GetField      (i, "ImproveMaxTimes"      );
        p->improve_failed_down              = _table->GetField      (i, "ImproveFailedDown"    );
        p->improve_broadcast                = _table->GetField      (i, "ImproveBroadcast"     );
        p->can_upgrade                      = _table->GetField      (i, "CanUpgrade"           );
        p->upgrade_need_game_money          = _table->GetField      (i, "UpgradeNeedGameMoney" );
        p->upgrade_min_times                = _table->GetField      (i, "UpgradeMminTimes"     );
        p->upgrade_rate                     = _table->GetField      (i, "UpgradeRate"          );
        p->upgrade_max_times                = _table->GetField      (i, "UpgradeMaxTimes"      );
        p->upgrade_target_equip             = _table->GetField      (i, "UpgradeTargetEquip"   );
        p->upgraded_improve_level           = _table->GetField      (i, "UpgradedImproveLevel" );
        p->upgrade_broadcast                = _table->GetField      (i, "UpgradeBroadcast"     );

		cpf_0 f0;// map
		f0.set_cfg(&p->improve_need_material);
		cp.parser<cpf_0>(_table->GetFieldString(i, "ImproveNeedMaterial").c_str(), ";", f0);
		cpf_0 f1;
		f1.set_cfg(&p->improve_failed_reward);
		cp.parser<cpf_0>(_table->GetFieldString(i, "ImproveFailedReward").c_str(), ";", f1);
		cpf_0 f2;
		f2.set_cfg(&p->improve_update_attr);
		cp.parser<cpf_0>(_table->GetFieldString(i, "ImproveUpdateAttr").c_str(), ";", f2);
		cpf_0 f3;
		f3.set_cfg(&p->upgrade_need_material);
		cp.parser<cpf_0>(_table->GetFieldString(i, "UpgradeNeedMaterial").c_str(), ";", f3);

		cpf f4;// vector
		f4.set_cfg(&p->upgrade_replace_item);
		cp.parser<cpf>(_table->GetFieldString(i, "UpgradeReplaceItem").c_str(), ";", f4);
		cpf f5;
		f5.set_cfg(&p->improve_replace_item);
		cp.parser<cpf>(_table->GetFieldString(i, "ImproveReplaceItem").c_str(), ";", f5);

		cpf_0 f6;// map
		f6.set_cfg(&p->improve_update_attr_1);
		cp.parser<cpf_0>(_table->GetFieldString(i, "ImproveUpdateAttr1").c_str(), ";", f6);
		_cp.insert(std::pair<int32, config_upequip*>(p->equip_id,p));
    }

    return 0;
}

int config_upequip_mgr::clear()
{
    for (std::multimap<uint32, config_upequip*>::iterator i = _cp.begin(); i != _cp.end(); ++i) {
        if (i->second != NULL) {
            delete i->second;
        }
    }

    _cp.clear();

    return 0;
}

const config_upequip* config_upequip_mgr::get_config_upequip(uint32 equip_id,int strengthen_level) const
{
    std::multimap<uint32, config_upequip*>::const_iterator itr = _cp.find(equip_id);

	for (; itr != _cp.end(); ++itr){
		if (itr->second->get_improve_level() == strengthen_level && itr->first == equip_id){
			return itr->second;
		}
	}

	return NULL;
}
