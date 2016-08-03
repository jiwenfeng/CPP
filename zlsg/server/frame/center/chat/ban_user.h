#ifndef __BAN_USER_H__
#define __BAN_USER_H__

#include "comm_define.h"

class ban_user
{
public:
    ban_user();
    ~ban_user();

public:
    std::map<uint32, uint32>& get_chat();

    int add_chat(uint32 id, uint32 time);
    
    int del_chat(uint32 id);
    
    uint32 find_chat(uint32 id);

    std::map<uint32, uint32>& get_login();

    int add_login(uint32 id, uint32 time);

    int del_login(uint32 id);
    
private:
    std::map<uint32, uint32> _chat;
    std::map<uint32, uint32> _login;
};

#endif // __BAN_USER_H__
