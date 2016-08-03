#include "cfg_parser.h"
#include "config_material.h"

config_material::config_material(void)
{
}

config_material::~config_material(void)
{
}

config_material_mgr::config_material_mgr(TTableGroup_s* p)
        : _material(p)
{
    init(p);
}


config_material_mgr::~config_material_mgr(void)
{
    clear();
}

int config_material_mgr::init(TTableGroup_s* p)
{
    if (p == NULL) {
        return -1;
    }

    _material = p;
    _table = _material->GetTable("t_material");
    if (_table == NULL) {
        printf("not found t_material\n");
        return -2;
    }

    cfg_parser cp;
    for (int i = 1; i <= _table->m_recordCount; ++i) {
        if (_table->GetField      (i, "CompoundID"         ) == 0) {
            continue;
        }

        config_material* p = new config_material;
        p->compound_id                         = _table->GetField      (i, "CompoundID"              );
		cpf_0 p1;// map
		p1.set_cfg(&p->material_list);
		cp.parser<cpf_0>(_table->GetFieldString(i, "MaterialID").c_str(), ";", p1);

		_cp.insert(std::pair<int32, config_material*>(p->compound_id,p));
    }

    return 0;
}

int config_material_mgr::clear()
{
    for (std::map<uint32, config_material*>::iterator i = _cp.begin(); i != _cp.end(); ++i) {
        if (i->second != NULL) {
            delete i->second;
        }
    }

    _cp.clear();

    return 0;
}

const config_material* config_material_mgr::get_config_material(uint32 compound_id) const
{
    std::map<uint32, config_material*>::const_iterator itr = _cp.find(compound_id);

	if (itr != _cp.end()){
		return itr->second;
	}

	return NULL;
}
