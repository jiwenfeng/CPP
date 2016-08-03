#include "cfg_parser.h"
#include "config_land.h"

config_land::config_land(void)
{
}

config_land::~config_land(void)
{
}

config_land_mgr::config_land_mgr(TTableGroup_s* p)
        : _land(p)
{
    init(p);
}


config_land_mgr::~config_land_mgr(void)
{
    clear();
}

int config_land_mgr::init(TTableGroup_s* p)
{
    if (p == NULL) {
        return -1;
    }

    _land = p;
    _table = _land->GetTable("t_land");
    if (_table == NULL) {
        printf("not found t_land\n");
        return -2;
    }

    cfg_parser cp;
    for (int i = 1; i <= _table->m_recordCount; ++i) {
        if (_table->GetField      (i, "LandID"         ) == 0) {
            continue;
        }

        config_land* p = new config_land;
        p->land_id                         = _table->GetField            (i, "LandID"              );
		p->yuan_bao                        = _table->GetField            (i, "LYuanBao"            );
		p->land_money                      = _table->GetField            (i, "LMoney"              );

		cpf_succinct_info f1;
		f1.set_cfg(&p->item_1);
		cp.parser<cpf_succinct_info>(_table->GetFieldString(i, "LItem1").c_str(), ";", f1);

		cpf_succinct_info f2;
		f2.set_cfg(&p->item_2);
		cp.parser<cpf_succinct_info>(_table->GetFieldString(i, "LItem2").c_str(), ";", f2);

		cpf_succinct_info f3;
		f3.set_cfg(&p->item_3);
		cp.parser<cpf_succinct_info>(_table->GetFieldString(i, "LItem3").c_str(), ";", f3);

		cpf_0 f4;
		f4.set_cfg(&p->land_condition);
		cp.parser<cpf_0>(_table->GetFieldString(i, "Lcondition").c_str(), ";", f4);

		_cp.insert(std::pair<int32, config_land*>(p->land_id,p));
    }

    return 0;
}

int config_land_mgr::clear()
{
    for (std::map<uint32, config_land*>::iterator i = _cp.begin(); i != _cp.end(); ++i) {
        if (i->second != NULL) {
            delete i->second;
        }
    }

    _cp.clear();

    return 0;
}

const config_land* config_land_mgr::get_config_land(uint32 land_id) const
{
    std::map<uint32, config_land*>::const_iterator itr = _cp.find(land_id);
	if (itr != _cp.end()){
		return itr->second;
	}
	

	return NULL;
}
