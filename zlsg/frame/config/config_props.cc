#include "cfg_parser.h"
#include "config_props.h"

config_props::config_props(void)
{
}

config_props::~config_props(void)
{
}

config_props_mgr::config_props_mgr(TTableGroup_s* p)
        : _props(p)
{
    init(p);
}


config_props_mgr::~config_props_mgr(void)
{
    clear();
}

int config_props_mgr::init(TTableGroup_s* p)
{
    if (p == NULL) {
        return -1;
    }

    _props = p;
    _table = _props->GetTable("t_item");
    if (_table == NULL) {
        printf("not found t_item\n");
        return -2;
    }

    cfg_parser cp;
    for (int i = 1; i <= _table->m_recordCount; ++i) {
        if (_table->GetField      (i, "ItemID"         ) == 0) {
            continue;
        }
        config_props* p = new config_props;
        p->_item_id          = _table->GetField      (i, "ItemID"         );
        p->_item_name        = _table->GetFieldString(i, "ItemName"       );
        p->_item_type        = _table->GetField      (i, "ItemType"       );
        p->_item_sub_type    = _table->GetField      (i, "ItemSubType"    );
        p->_bind_type        = _table->GetField      (i, "BindType"       );
        p->_is_get_use       = _table->GetField      (i, "GetUse"         );
        p->_pick_range       = _table->GetField      (i, "PickRange"      );
        p->_total            = _table->GetField      (i, "Total"          );
		p->_need_job         = _table->GetField      (i, "NeedJob"        );
        p->_need_level       = _table->GetField      (i, "NeedLevel"      );
        p->_cool_group_id    = _table->GetField      (i, "CoolGroupID"    );
        p->_cool_group_tick  = _table->GetField      (i, "CoolGroupTick"  );
        p->_buy_price        = _table->GetField      (i, "BuyPrice"       );
        p->_is_shop_recover  = _table->GetField      (i, "isShopRecover"  );
        p->_sales_price      = _table->GetField      (i, "SalesPrice"     );
        p->_is_shop_confirm  = _table->GetField      (i, "isShopConfirm"  );
        p->_can_repair       = _table->GetField      (i, "CanRepair"      );
        p->_repair_price     = _table->GetField      (i, "RepairPrice"    );
        p->_can_discard      = _table->GetField      (i, "CanDiscard"     );
        p->_discard_confirm  = _table->GetField      (i, "DiscardConfirm" );
        p->_can_inlay        = _table->GetField      (i, "CanInlay"       );
        p->_can_synthesis    = _table->GetField      (i, "CanSynthesis"   );
        p->_durable_max      = _table->GetField      (i, "DurableMax"     );
        p->_increase_max     = _table->GetField      (i, "IncreaseMax"    );
        p->_affect_hp        = _table->GetField      (i, "AffectHP"       );
        p->_affect_mp        = _table->GetField      (i, "AffectMP"       );
        p->_can_shortcut     = _table->GetField      (i, "CanShortcut"    );
        p->_name_color_index = _table->GetField      (i, "NameColorIndex" );
        p->_describes        = _table->GetFieldString(i, "Describes"      );
        p->_pickup_type      = _table->GetField      (i, "PickupType"     );
        p->_lotuser_count    = _table->GetField      (i, "lotUserCount"   );
		p->_exceed_limit	 = _table->GetField      (i, "ExceedLimit"    );
		p->_smelting_coefficient	 = _table->GetField  (i, "SmeltingCoefficient"    );
		p->_basic_upgrade	 = _table->GetField      (i, "BasicUpgrade"        );
		p->_upgrade_cost	 = _table->GetField      (i, "UpgradeCost"		   );
		p->_start_upgrade_cost	 = _table->GetField  (i, "StartUpgradeCost"    );

        cpf_0 f0;
        f0.set_cfg(&p->_game_attribute);
        cp.parser<cpf_0>(_table->GetFieldString(i, "GameAttribute").c_str(), ";", f0);

		cpf f2;
		f2.set_cfg(&p->_trigger_list);
		cp.parser<cpf>(_table->GetFieldString(i, "TriggerList").c_str(), ";", f2);

        cpf_3 f3;
        f3.set_cfg(&p->_random_attribute);
        cp.parser<cpf_3>(_table->GetFieldString(i, "RandomAttribute").c_str(), ";", f3);

		cpf_succinct_info f4;
		f4.set_cfg(&p->_succinct_need_material);
		cp.parser<cpf_succinct_info>(_table->GetFieldString(i, "SuccinctNeedMaterial").c_str(), ";", f4);

        _cp.insert(std::pair<uint32, config_props*>(p->_item_id, p));
    }

    return 0;
}

int config_props_mgr::clear()
{
    for (std::map<uint32, config_props*>::iterator i = _cp.begin(); i != _cp.end(); ++i) {
        if (i->second != NULL) {
            delete i->second;
        }
    }

    _cp.clear();

    return 0;
}

const config_props* config_props_mgr::get_config_props(uint32 props_id) const
{
    std::map<uint32, config_props*>::const_iterator i = _cp.find(props_id);
    if (i == _cp.end()) {
        return NULL;
    }

    return i->second;
}

const string config_props_mgr::get_prop_name(uint32 prop_id) const 
{
	std::map<uint32, config_props *>::const_iterator i = _cp.find(prop_id);
	if(i == _cp.end())
	{
		return "";
	}
	return i->second->get_item_name();
}
