#ifndef __FAMILY_DOMAIN_H__
#define __FAMILY_DOMAIN_H__

#include "define.h"
#include "pb_mgr.h"
#include "client.family.pb.h"
#include "pb_writer.h"

class family_domain : public pb_writer
{
public:
    family_domain(mysql::tb_family_domain* tfl = NULL);
    family_domain(uint32 family_id, uint32 scene_id);
    virtual ~family_domain();

public:
    int save(amtsvr_context* ctx);

    mysql::tb_family_domain* get() { return _domain; }

public:
    int set(uint32 family_id, uint32 scene_id);
    
    void set_family_id(uint32 family_id);
    
    int set_scene_id(uint32 scene_id);

    int remove();

private:
    mysql::tb_family_domain* _domain;
    bool _change;
};

#endif /* __FAMILY_DOMAIN_H__ */
