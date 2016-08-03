#include <limits.h>
#include "hate_man.h"

hate::hate(uint64 id /*= 0*/, uint32 first /*= 0*/, uint32 last /*= 0*/, uint32 v /*= 0*/, uint32 real_damage /*= 0*/, uint32 distance /*= 0*/) : _inst_id(id), _first(first), _last(last), _val(v), _distance(distance), _real_damage(real_damage), _last_damage(real_damage), _high_damage(real_damage)
{

}

bool hate::operator>(const hate& rhs)
{
    if (_val > rhs._val) {
        return true;
    } else if (_val == rhs._val) {
        return _distance > rhs._distance;
    }

    return false;
}

bool hate::operator>=(const hate& rhs)
{
    if (this->operator>(rhs)) {
        return true;
    } else if (_val == rhs._val && _distance == rhs._distance) {
        return true;
    }

    return false;
}

hate_man::hate_man()
{
}

hate_man::~hate_man()
{
}

void hate_man::add_hate_role(uint64 id, int val, uint32 real_damage, uint32 distance)
{
    uint32 now = (uint32)time(NULL);
    std::map<uint64, hate>::iterator it = _hate_map.find(id);
    if (it != _hate_map.end()) {
        it->second._last        =  now;
        it->second._val         += val;
        it->second._distance    =  distance;
        it->second._real_damage += real_damage;
        it->second._last_damage =  real_damage;
        if (it->second._high_damage < real_damage) {
            it->second._high_damage = real_damage;
        }

        return ;
    }

    _hate_map[id]._inst_id     = id;
    _hate_map[id]._first       = now;
    _hate_map[id]._last        = now;
    _hate_map[id]._val         = val;
    _hate_map[id]._distance    = distance;
    _hate_map[id]._real_damage = real_damage;
    _hate_map[id]._last_damage = real_damage;
    _hate_map[id]._high_damage = real_damage;
}

void hate_man::clean_hate_map()
{
    _hate_map.clear();
}

void hate_man::del_hate_role(uint64 id)
{
    _hate_map.erase(id);
}

void hate_man::get_hate_rank_list(std::vector<uint64>& v, int num)
{
    std::map<uint64, hate>::iterator it = _hate_map.begin();
    for (; it != _hate_map.end(); ++it) {
        v.push_back(it->second._inst_id);
    }
}

uint64 hate_man::get_hate_role()
{
    uint64 id = 0;
    uint32 max_hate_val = 0;
    uint32 min_distance = INT_MAX;
    bool rep = false;
    std::map<uint64, hate>::iterator it = _hate_map.begin();
    for (; it != _hate_map.end(); ++it) {
        uint32 val      = it->second._val;
        uint32 distance = it->second._distance;
        if (val > max_hate_val) {
            rep = true;
        } else if (val == max_hate_val && distance < min_distance) {
            rep = true;
        }

        if (rep) {
            max_hate_val = val;
            min_distance = distance;
            id = it->second._inst_id;
            rep = false;
        }
    }

    return id;
}

void hate_man::get_real_damage_rank_list(map<uint32, uint64>& rank)
{
    std::map<uint64, hate>::iterator it = _hate_map.begin();
    for (; it != _hate_map.end(); ++it){
        rank.insert(std::pair<uint32, uint64>(it->second._real_damage, it->first));
    }
}
