#include "cfg_parser.h"
#include "config_map.h"

config_map::config_map(void)
{
}

config_map::~config_map(void)
{
}

const bool config_map::is_block(int x, int y) const
{
    return unit_mask(x, y, MAP_UNIT_BLOCK);
}

const bool config_map::unit_mask(int x, int y, char mask) const
{
	int index = y * this->unitGridWidthNum + x;
	if (index >= this->totalUnitGridNum) {
        return false;
    }

    return (this->units[index] & 0x0F) == mask;
}

const uint32 config_map::get_region(int x, int y) const
{
    int index = y * this->unitGridWidthNum + x;
    if (index >= this->totalUnitGridNum) {
        return false;
    }

    return this->units[index] >> 4;
}

config_map_mgr::config_map_mgr(std::map<std::string, MapConfig_s*>* p/* = NULL*/)
{

}

config_map_mgr::~config_map_mgr(void)
{
    //for (std::map<uint32, config_map*>::iterator i = _cm.begin(); i != _cm.end(); ++i) {
    //    delete i->second;
    //}
    _cm.clear();
}

int config_map_mgr::init(std::map<std::string, MapConfig_s*>* p)
{
    if (p == NULL) {
        return -1;
    }

    std::map<std::string, MapConfig_s*>::iterator i = p->begin();
    for ( ; i != p->end(); ++i) {
        _cm.insert(std::pair<uint32, config_map*>(i->second->id, static_cast<config_map*>(i->second)));
    }

    return 0;
}

int config_map_mgr::init(TTableGroup_s* p)
{
    return 0;
}

const config_map* config_map_mgr::get_config_map(uint32 id) const
{
    std::map<uint32, config_map*>::const_iterator i = _cm.find(id);
    if (i == _cm.end()) {
        return NULL;
    }

    return i->second;
}