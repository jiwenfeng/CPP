#include "define.h"
#include "role_key.h"

std::string role_key_mgr::_null = "";

role_key_mgr::role_key_mgr()
{

}

role_key_mgr::~role_key_mgr()
{

}

std::string& role_key_mgr::operator[](uint32 i)
{
    return _keys[i]._key;
}

const void role_key_mgr::set_key(uint32 role_id, const std::string& key)
{
    _keys[role_id]._key  = key;
}

const std::string& role_key_mgr::find(uint32 role_id) const
{
    std::map<uint32, role_key>::const_iterator f = _keys.find(role_id);
    if (f == _keys.end()) {
        return _null;
    }

    return f->second._key;
}

void role_key_mgr::erase(uint32 id)
{
    _keys.erase(id);
}

void role_key_mgr::add_key_time(uint32 id)
{
    _dels.insert(std::pair<uint32, uint32>(id, 1));
}

void role_key_mgr::del_key_time(uint32 id)
{
    _dels.erase(id);
}

void role_key_mgr::update()
{
    std::map<uint32, uint32>::iterator i = _dels.begin();
    while (i != _dels.end()) {
        if (i->second > 1) {
            i->second -= 1;
        }

        if (i->second == 0) {
            erase(i->first);
            _dels.erase(i++);
            continue;
        }

        ++i;
    }
}

