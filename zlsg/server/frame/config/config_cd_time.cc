#include "cfg_parser.h"
#include "config_cd_time.h"

config_cd_time::config_cd_time(void)
{
}

config_cd_time::~config_cd_time(void)
{
}

config_cd_time_mgr::config_cd_time_mgr(TTableGroup_s* p)
        : _cd(p)
{
    init(p);
}


config_cd_time_mgr::~config_cd_time_mgr(void)
{
    clear();
}

int config_cd_time_mgr::init(TTableGroup_s* p)
{
    if (p == NULL) {
        return -1;
    }

    _cd = p;
    _table = _cd->GetTable("t_itemcd");
    if (_table == NULL) {
        printf("not found t_itemcd\n");
        return -2;
    }

    cfg_parser cp;
    for (int i = 1; i <= _table->m_recordCount; ++i) {
        if (_table->GetField      (i, "type_id"         ) == 0) {
            continue;
        }

        config_cd_time* p = new config_cd_time;
		p->type_id									= _table->GetField      (i, "type_id"					  );
		p->type									    = _table->GetField      (i, "type"					      );
		p->is_save									= _table->GetField      (i, "is_save"					  );
		p->is_send									= _table->GetField      (i, "is_send"					  );
		p->life_time								= _table->GetField      (i, "life_time"					  );
		p->cd_time_max								= _table->GetField      (i, "cd_time_max"				  );


		_cp.insert(std::pair<uint32, config_cd_time*>(p->type_id,p));
    }

    return 0;
}

int config_cd_time_mgr::clear()
{
    for (std::map<uint32, config_cd_time*>::iterator i = _cp.begin(); i != _cp.end(); ++i) {
        if (i->second != NULL) {
            delete i->second;
        }
    }

    _cp.clear();

    return 0;
}

const config_cd_time* config_cd_time_mgr::get_config_cd_time(uint32 type_id) const
{
    std::map<uint32, config_cd_time*>::const_iterator itr = _cp.find(type_id);

	if (itr == _cp.end()){
		return NULL;
	}

	return itr->second;
}
