#ifndef __ONLINE_USER_H__
#define __ONLINE_USER_H__

#include <map>
#include <vector>
#include <string>
#include "define.h"
#include "center_role.h"

class online_role
{
public:
    online_role();
    online_role(const online_role& src);
    online_role(uint32 rid, uint32 sid, uint32 aid, uint32 cid, const string& name, uint32 scid, uint32 vip_exp);
    virtual ~online_role();

public:
    void attach(center_role* c);

    center_role* pop_center_role();

    center_role* mutable_center_role();

    const center_role* get_const_center_role() const {return _pcr;}

	bool in_main_town() const;
	bool in_battle_scene() const;

    inline uint32 vip_exp() const { return _vip_exp; }

public:
    uint32      role_id;
    uint32      scene_svr_id;
    uint32      agent_id;
    uint32      client_id;
    std::string role_name;
    uint32      last_chat_time; // 上一次聊天的时间
    uint32      scene_id;
    uint32      _vip_exp;

private:
    center_role* _pcr;
};

#endif
