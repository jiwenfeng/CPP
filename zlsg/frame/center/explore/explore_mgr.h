#ifndef __EXPLORE_MGR_H__
#define __EXPLORE_MGR_H__

#include <map>
#include "comm_define.h"
#include "explore_center.h"


class explore_mgr
{
public:
    explore_mgr();
    virtual ~explore_mgr();

public:
    int init(struct cmq_t* l);

    int update(client::explore_update_list* eul);
    
    int save(amtsvr_context* ctx);
    
    int get(vector<explore_center>& vtr);

public:
    int fill_explore_list(client::broadcast_explore_list_rsp& rsp);

private:
    multimap<uint32, explore_center*> _map;
};

#endif /* __EXPLORE_MGR_H__ */
