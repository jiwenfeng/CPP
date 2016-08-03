#include "cfg_parser.h"
#include "config_legion.h"

config_legion::config_legion(void)
{
}

config_legion::~config_legion(void)
{
}

config_legion_mgr::config_legion_mgr(TTableGroup_s* p)
        : _legion(p)
{
    init(p);
}


config_legion_mgr::~config_legion_mgr(void)
{
    clear();
}

int config_legion_mgr::init(TTableGroup_s* p)
{
    if (p == NULL) {
        return -1;
    }

    _legion = p;
    _table = _legion->GetTable("t_legion");
    if (_table == NULL) {
        printf("not found t_legion\n");
        return -2;
    }

    cfg_parser cp;
    for (int i = 1; i <= _table->m_recordCount; ++i) {
        if (_table->GetField      (i, "LegionLv"         ) == 0) {
            continue;
        }
        config_legion* p = new config_legion;
        p->legion_level             = _table->GetField			(i, "LegionLv"				);
		p->upgrade_need_lv          = _table->GetField			(i, "UpgradeNeedLv"         );
		p->member_limit             = _table->GetField			(i, "MemberLimit"           );
		p->create_need_money        = _table->GetField			(i, "CreateNeedMoney"       );
		p->upgrade_need_money       = _table->GetField			(i, "UpgradeNeedMoney"      );
		p->create_need_silver		= _table->GetField			(i, "CreateNeedYin"			);
		p->upgrade_need_silver      = _table->GetField			(i, "UpgradeNeedYin"        );
		p->add_experience			= _table->GetField			(i, "AddExperience"         );
		p->change_flag_pay			= _table->GetField			(i, "CflagNeedMoney"        );
		p->change_flag_name_pay		= _table->GetField		    (i, "CnameNeedMoney"        );

		cpf_0 f0;// map
		f0.set_cfg(&p->upgrade_need_item);
		cp.parser<cpf_0>(_table->GetFieldString(i, "UpgradeNeedItem").c_str(), ";", f0);

		cpf_0 f1;// map
		f1.set_cfg(&p->add_property);
		cp.parser<cpf_0>(_table->GetFieldString(i, "AddProperty").c_str(), ";", f1);

		cpf_0 f2;// 
		f2.set_cfg(&p->donation_1);
		cp.parser<cpf_0>(_table->GetFieldString(i, "Donation1").c_str(), ";", f2);
		cpf_0 f3;// 
		f3.set_cfg(&p->donation_2);
		cp.parser<cpf_0>(_table->GetFieldString(i, "Donation2").c_str(), ";", f3);
		cpf_0 f4;// 
		f4.set_cfg(&p->donation_3);
		cp.parser<cpf_0>(_table->GetFieldString(i, "Donation3").c_str(), ";", f4);
		cpf_0 f5;// 
		f5.set_cfg(&p->donation_4);
		cp.parser<cpf_0>(_table->GetFieldString(i, "Donation4").c_str(), ";", f5);

		cpf_0 f6;// map
		f6.set_cfg(&p->donation_5);
		cp.parser<cpf_0>(_table->GetFieldString(i, "Donation5").c_str(), ";", f6);

		cpf_0 f7;// map
		f7.set_cfg(&p->change_flag_item);
		cp.parser<cpf_0>(_table->GetFieldString(i, "CflagNeedItem").c_str(), ";", f7);

		cpf_0 f8;// map
		f8.set_cfg(&p->change_flag_name_item);
		cp.parser<cpf_0>(_table->GetFieldString(i, "CnameNeedItem").c_str(), ";", f8);

        _cp.insert(std::pair<uint32, config_legion*>(p->legion_level, p));
    }

    return 0;
}

int config_legion_mgr::clear()
{
    for (std::map<uint32, config_legion*>::iterator i = _cp.begin(); i != _cp.end(); ++i) {
        if (i->second != NULL) {
            delete i->second;
        }
    }

    _cp.clear();

    return 0;
}

const config_legion* config_legion_mgr::get_config_legion(uint32 legion_level) const
{
    std::map<uint32, config_legion*>::const_iterator i = _cp.find(legion_level);
    if (i == _cp.end()) {
        return NULL;
    }

    return i->second;
}
