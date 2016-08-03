#include "cfg_parser.h"
#include "config_mount_exp.h"

config_mount_exp::config_mount_exp(void)
{
}

config_mount_exp::~config_mount_exp(void)
{
}

config_mount_exp_mgr::config_mount_exp_mgr(TTableGroup_s* p)
        : _mountexp(p)
{
    init(p);
}


config_mount_exp_mgr::~config_mount_exp_mgr(void)
{
    clear();
}

int config_mount_exp_mgr::init(TTableGroup_s* p)
{
    if (p == NULL) {
        return -1;
    }

    _mountexp = p;
    _table = _mountexp->GetTable("t_mountexp");
    if (_table == NULL) {
        printf("not found mountex\n");
        return -2;
    }

    cfg_parser cp;
    for (int i = 1; i <= _table->m_recordCount; ++i) {
        if (_table->GetField      (i, "MountLevel"         ) == 0) {
            continue;
        }

        config_mount_exp* p = new config_mount_exp;
        p->mount_level                         = _table->GetField      (i, "MountLevel"              );
		p->upgrate_exp                         = _table->GetField      (i, "UpgradeEXP"              );
		p->mount_idx_limit                     = _table->GetField      (i, "MountID"                 );


		_cp.insert(std::pair<int32, config_mount_exp*>(p->mount_level,p));
    }

    return 0;
}

int config_mount_exp_mgr::clear()
{
    for (std::map<uint32, config_mount_exp*>::iterator i = _cp.begin(); i != _cp.end(); ++i) {
        if (i->second != NULL) {
            delete i->second;
        }
    }

    _cp.clear();

    return 0;
}

const config_mount_exp* config_mount_exp_mgr::get_config_mount_exp(int mount_level) const
{
    std::map<uint32, config_mount_exp*>::const_iterator itr = _cp.find(mount_level);

	if (itr == _cp.end()){
		return NULL;
	}

	return itr->second;
}
