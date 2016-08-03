#include "cfg_parser.h"
#include "config_gem.h"

config_gem::config_gem(void)
{
}

config_gem::~config_gem(void)
{
}

config_gem_mgr::config_gem_mgr(TTableGroup_s* p)
        : _gem(p)
{
    init(p);
}


config_gem_mgr::~config_gem_mgr(void)
{
    clear();
}

int config_gem_mgr::init(TTableGroup_s* p)
{
    if (p == NULL) {
        return -1;
    }

    _gem = p;
    _table = _gem->GetTable("t_gem");
    if (_table == NULL) {
        printf("not found t_gem\n");
        return -2;
    }

    cfg_parser cp;
    for (int i = 1; i <= _table->m_recordCount; ++i) {
        if (_table->GetField      (i, "GemType"         ) == 0) {
            continue;
        }

        config_gem* p = new config_gem;
        p->gem_type                       = _table->GetField      (i, "GemType"			   );
		p->gem_lv                         = _table->GetField      (i, "Gemlv"              );
		p->up_exp                         = _table->GetField      (i, "UpgradeExperience"  );
		p->upgrade_id                     = _table->GetField      (i, "UpgradeID"          );
		p->need_count                     = _table->GetField      (i, "DemandQuantity"     );

		cpf_1 f0;
		f0.set_cfg(&p->game_attr);
		cp.parser<cpf_1>(_table->GetFieldString(i, "Attribute").c_str(), ";", f0);

		cpf f1;
		f1.set_cfg(&p->upgrade_goods);
		cp.parser<cpf>(_table->GetFieldString(i, "UpgradeGoodsID").c_str(), ";", f1);

		cpf_succinct_info f2;
		f2.set_cfg(&p->add_exp_list);
		cp.parser<cpf_succinct_info>(_table->GetFieldString(i, "GainExperience").c_str(), ";", f2);

		cpf f3;
		f3.set_cfg(&p->using_component);
		cp.parser<cpf>(_table->GetFieldString(i, "UsingComponent").c_str(), ";", f3);

		_cp.insert(std::pair<uint32, config_gem*>(p->gem_type,p));
    }

    return 0;
}

int config_gem_mgr::clear()
{
    for (std::multimap<uint32, config_gem*>::iterator i = _cp.begin(); i != _cp.end(); ++i) {
        if (i->second != NULL) {
            delete i->second;
        }
    }

    _cp.clear();

    return 0;
}

const config_gem* config_gem_mgr::get_config_gem(uint32 gem_type,uint32 gem_level) const
{
    std::multimap<uint32, config_gem*>::const_iterator itr = _cp.find(gem_type);

	for (;itr != _cp.end(); ++itr){
		if (itr->second->get_gem_lv() == gem_level && itr->first == gem_type){
			return itr->second;
		}
	}

	return NULL;
}
