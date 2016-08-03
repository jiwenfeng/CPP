#include "cfg_parser.h"
#include "config_bow_exp.h"

config_bow_exp::config_bow_exp(void)
{
}

config_bow_exp::~config_bow_exp(void)
{
}

config_bow_exp_mgr::config_bow_exp_mgr(TTableGroup_s* p)
        : _bow_exp(p)
{
    init(p);
}


config_bow_exp_mgr::~config_bow_exp_mgr(void)
{
    clear();
}

int config_bow_exp_mgr::init(TTableGroup_s* p)
{
    if (p == NULL) {
        return -1;
    }

    _bow_exp = p;
    _table = _bow_exp->GetTable("t_bowexp");
    if (_table == NULL) {
        printf("not found t_bowexp\n");
        return -2;
    }

    cfg_parser cp;
    for (int i = 1; i <= _table->m_recordCount; ++i) {
        if (_table->GetField      (i, "BowLevel"         ) == 0) {
            continue;
        }

        config_bow_exp* p = new config_bow_exp;
        p->bow_level                         = _table->GetField      (i, "BowLevel"           );
		p->upgrade_exp                       = _table->GetField      (i, "UpgradeEXP"         );
		p->bow_id                            = _table->GetField      (i, "BowID"              );

		_cp.insert(std::pair<int32, config_bow_exp*>(p->bow_level,p));
    }

    return 0;
}

int config_bow_exp_mgr::clear()
{
    for (std::map<uint32, config_bow_exp*>::iterator i = _cp.begin(); i != _cp.end(); ++i) {
        if (i->second != NULL) {
            delete i->second;
        }
    }

    _cp.clear();

    return 0;
}

const config_bow_exp* config_bow_exp_mgr::get_config_bow_exp(uint32 bow_level) const
{
    std::map<uint32, config_bow_exp*>::const_iterator itr = _cp.find(bow_level);

	if (itr != _cp.end()){
		return itr->second;
	}

	return NULL;
}
