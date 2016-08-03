#if 0
#ifndef __SIEGE_MGR_H__
#define __SIEGE_MGR_H__

#include "comm_define.h"
#include "siege_center.h"

class siege_mgr
{
public:
    siege_mgr();
    virtual ~siege_mgr();

public:
    int init(struct cmq_t* l);

    int save(amtsvr_context* ctx);

public:
    int update_official(client::broadcast_siege_official* req);
      
public:
    int city_official(client::query_city_official_rsp& rsp);

    int city_camp(client::query_city_camp_rsp& rsp);

    int set_city_camp(client::query_city_camp_rsp& rsp);
    
    int is_start(client::query_siege_start_rsp& rsp);

private:
    map<uint32, uint32>        _city_camp;
    map<uint32, siege_center*> _siege;
};

#endif /* __SIEGE_MGR_H__ */
#endif
