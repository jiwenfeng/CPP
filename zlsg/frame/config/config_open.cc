#include "cfg_parser.h"
#include "config_open.h"

config_open::config_open(void)
{
}

config_open::~config_open(void)
{
}

config_open_mgr::config_open_mgr(TTableGroup_s* p)
    : _group(p)
{
    init(p);
}

config_open_mgr::~config_open_mgr(void)
{
    clear();
}

int config_open_mgr::init(TTableGroup_s* p)
{
    if (p == NULL) {
        return -1;
    }

    _group = p;
    _table = _group->GetTable("t_funcAvailable");
    if (_table == NULL) {
        printf("not found t_funcAvailable\n");
        return -2;
    }

    cfg_parser cp;
    for (int i = 1; i <= _table->m_recordCount; ++i) {
        config_open* p = new config_open;
        p->_func_id              = _table->GetField(i, "FuncID"             );
        p->_open_type            = _table->GetField(i, "OpenType"           );
        p->_open_condition       = _table->GetField(i, "OpenCondition"      );
        _cfg.insert(std::pair<uint32, config_open*>(p->_func_id, p));
    }

    return 0;
}

int config_open_mgr::clear()
{
    for (std::map<uint32, config_open*>::iterator i = _cfg.begin(); i != _cfg.end(); ++i) {
        if (i->second != NULL) {
            delete i->second;
        }
    }

    _cfg.clear();

    return 0;
}

const config_open* config_open_mgr::get_config_open(uint32 id) const
{
    std::map<uint32, config_open*>::const_iterator i = _cfg.find(id);
    if (i == _cfg.end()) {
        return NULL;
    }

    return i->second;
}

uint32 config_open_mgr::get_max() const
{
    if (_cfg.empty()) {
        return 0;
    }

    return _cfg.rbegin()->first;
}
