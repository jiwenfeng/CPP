#include "ban_user.h"

ban_user::ban_user()
{
}

ban_user::~ban_user()
{
}

std::map<uint32, uint32>& ban_user::get_chat()
{
    return _chat;
}

int ban_user::add_chat(uint32 id, uint32 time)
{
    _chat[id] = time;
    
    return 0;
}

int ban_user::del_chat(uint32 id)
{
    std::map<uint32, uint32>::iterator i = _chat.find(id);
    if (i == _chat.end()) {
        return -1;
    }

    _chat.erase(i);
    
    return 0;
}

uint32 ban_user::find_chat(uint32 id)
{
    std::map<uint32, uint32>::iterator i = _chat.find(id);
    if (i == _chat.end()) {
        return 0;
    }

    return i->second;
}

std::map<uint32, uint32>& ban_user::get_login()
{
    return _login;
}

int ban_user::add_login(uint32 id, uint32 time)
{
    _login[id] = time;
        
    return 0;
}

int ban_user::del_login(uint32 id)
{
    std::map<uint32, uint32>::iterator i = _login.find(id);
    if (i == _login.end()) {
        return -1;
    }

    _login.erase(i);
    
    return 0;
}
