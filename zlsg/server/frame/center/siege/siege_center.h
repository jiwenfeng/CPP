#if 0
#ifndef __SIEGE_CENTER_H__
#define __SIEGE_CENTER_H__

#include "define.h"
#include "pb_mgr.h"
#include "pb_writer.h"

class siege_center : public pb_writer
{
public:
    siege_center(mysql::tb_siege_warfare* tsw = NULL);
    virtual ~siege_center();

public:
    mysql::tb_siege_warfare* get() { return _siege_warfare; }

    int save(amtsvr_context* ctx);

public:
    int update(client::broadcast_siege_official* req);
    
public:
    int fill(client::query_city_official_rsp& rsp);

private:
    void set_official(uint32 role_id);
    void set_depety_castellan(uint32 role_id);
    
private:
    mysql::tb_siege_warfare* _siege_warfare;
    bool                     _change;
    uint32                  official_tag;
    uint32                  depety_castellan_tag;
};

#endif /* __SIEGE_CENTER_H__ */
#endif
