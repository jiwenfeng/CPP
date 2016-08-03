#ifndef __OFFLINE_ROLE_MGR_H__
#define __OFFLINE_ROLE_MGR_H__

#include <stack>
#include "offline_role.h"

#ifdef __INNER_DEBUG__PACKAGE_
#define MAX_OFFLINE_USER  1000
#else
#define MAX_OFFLINE_USER  3000
#endif // __INNER_DEBUG__PACKAGE_

#define ELIMINATE_NUM     1

class offline_role_mgr
{
public:
    offline_role_mgr();
    virtual ~offline_role_mgr();

public:
    void push(uint32 role_id, player_role* pr, center_role* cr);
	const offline_role* get(uint32 role_id);
    offline_role* mutable_role(uint32 role_id);
    int pop(uint32 role_id, player_role*& pr, center_role*& cr);
    int remove(uint32 role_id);
    void remove_all();

protected:
    inline uint32 hash_index(uint32 role_id) const { return (role_id % MAX_OFFLINE_USER); }

    void reset_user(offline_role* ou);
    int  pop_role(offline_role* ou, player_role*& pr, center_role*& cr);
    bool replace(uint32 index, uint32 role_id, player_role* pr, center_role* cr);
    bool check_space();

protected:
    offline_role* roles[MAX_OFFLINE_USER]; 

    offline_role* head;
    offline_role* tail;

    std::stack<offline_role*> role_list;
};


#endif
