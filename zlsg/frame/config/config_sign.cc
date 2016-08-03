#include "cfg_parser.h"
#include "config_sign.h"

config_sign::config_sign(void)
{
}

config_sign::~config_sign(void)
{
}

config_sign_mgr::config_sign_mgr(TTableGroup_s* p)
        : _sign(p)
{
    init(p);
}


config_sign_mgr::~config_sign_mgr(void)
{
    clear();
}

int config_sign_mgr::init(TTableGroup_s* p)
{
    if (p == NULL) {
        return -1;
    }

    _sign = p;
    _table = _sign->GetTable("t_sign");
    if (_table == NULL) {
        printf("not found sign\n");
        return -2;
    }

    cfg_parser cp;
    for (int i = 1; i <= _table->m_recordCount; ++i) {
        if (_table->GetField      (i, "SignID"         ) == 0) {
            continue;
        }

        config_sign* p = new config_sign;
        p->sign_id                         = _table->GetField      (i, "SignID"                );
		p->reward_ingot                    = _table->GetField      (i, "SYuanBao"              );
		p->reward_tael                     = _table->GetField      (i, "SMoney"                );

		cpf_0 p1;// map
		p1.set_cfg(&p->sign_reward_item);
		cp.parser<cpf_0>(_table->GetFieldString(i, "SItem").c_str(), ";", p1);

		_cp.insert(std::pair<uint32, config_sign*>(p->sign_id,p));
    }

    return 0;
}

int config_sign_mgr::clear()
{
    for (std::map<uint32, config_sign*>::iterator i = _cp.begin(); i != _cp.end(); ++i) {
        if (i->second != NULL) {
            delete i->second;
        }
    }

    _cp.clear();

    return 0;
}

const config_sign* config_sign_mgr::get_config_sign(uint32 sign_id) const
{
    std::map<uint32, config_sign*>::const_iterator itr = _cp.find(sign_id);

	if (itr != _cp.end()){
		return itr->second;
	}

	return NULL;
}
