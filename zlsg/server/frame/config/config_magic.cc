#include "cfg_parser.h"
#include "config_magic.h"

config_magic::config_magic(void)
{
}

config_magic::~config_magic(void)
{
}

config_magic_mgr::config_magic_mgr(TTableGroup_s* p)
        : _magic(p)
{
    init(p);
}


config_magic_mgr::~config_magic_mgr(void)
{
    clear();
}

int config_magic_mgr::init(TTableGroup_s* p)
{
    if (p == NULL) {
        return -1;
    }

    _magic = p;
    _table = _magic->GetTable("t_magic");
    if (_table == NULL) {
        printf("not found magic\n");
        return -2;
    }

    cfg_parser cp;
    for (int i = 1; i <= _table->m_recordCount; ++i) {
        if (_table->GetField      (i, "MagicID"         ) == 0) {
            continue;
        }

        config_magic* p = new config_magic;
        p->magic_id                         = _table->GetField      (i, "MagicID"					);
		p->stage_star                       = _table->GetField      (i, "StageStar"					);
		p->magic_skill                      = _table->GetField      (i, "MagicSkill"				);
		//p->magic_add_attack                 = _table->GetField      (i, "MagicAddAttack"            );
		p->general_id						= _table->GetField      (i, "GeneralID"					);

		cpf_0 f0;// map
		f0.set_cfg(&p->star_need);
		cp.parser<cpf_0>(_table->GetFieldString(i, "StarNeed").c_str(), ";", f0);

		cpf_0 f2;// map
		f2.set_cfg(&p->up_charater);
		cp.parser<cpf_0>(_table->GetFieldString(i, "UpCharater").c_str(), ";", f2);

		cpf_0 f3;// map
		f3.set_cfg(&p->up_general);
		cp.parser<cpf_0>(_table->GetFieldString(i, "UpGeneral").c_str(), ";", f3);

		_cp.insert(std::pair<int32, config_magic*>(p->magic_id,p));
    }

    return 0;
}

int config_magic_mgr::clear()
{
    for (std::multimap<uint32, config_magic*>::iterator i = _cp.begin(); i != _cp.end(); ++i) {
        if (i->second != NULL) {
            delete i->second;
        }
    }

    _cp.clear();

    return 0;
}

const config_magic* config_magic_mgr::get_config_magic(uint32 magic_id,uint32 stage_star) const
{
    std::multimap<uint32, config_magic*>::const_iterator itr = _cp.find(magic_id);

	for (; itr != _cp.end(); ++itr){
		if (itr->second->get_stage_star() == stage_star && itr->first == magic_id){
			return itr->second;
		}
	}

	return NULL;
}
