#ifndef __ROLE_LOGIN_MGR_H__ 
#define __ROLE_LOGIN_MGR_H__

#include "role_define.h"
#include "pb_mgr.h"

/*
class role_login_key
{
public:
role_login_key()
{
svr_id = 0;
}

role_login_key(const role_login_key& rhs)
{
openid = rhs.openid;
svr_id = rhs.svr_id;
}

role_login_key(const string& id, uint32 sid)
{
openid = id;
svr_id = sid;
}

bool operator==(const role_login_key& rhs) const
{
return (svr_id == rhs.svr_id && openid.compare(rhs.openid) == 0);
}

bool operator<(const role_login_key& rhs) const
{
return (svr_id < rhs.svr_id);
}

public:
string openid;
uint32 svr_id;
};*/

class role_login_mgr
{
public:
    role_login_mgr();
    virtual ~role_login_mgr();

public:
    void put(mysql::sp_login_auth* sla);
    mysql::sp_login_auth* mut(const string& openid, uint32 svr_id);
    void erase(const string& openid, uint32 svr_id);

private:
    std::map<string, mysql::sp_login_auth> map_sla;
};

#endif
