#ifndef __CENTER_ROLE_H__
#define __CENTER_ROLE_H__

#include "center_role_set.h"

class center_role
{
public:
    center_role();
    center_role(const center_role* cr);
    virtual ~center_role();

public:
    int          attach_role_set(mysql::tb_role_center_set* trs);
    inline       center_role_set* mutable_center_role_set()   { return &crs; }
    inline const center_role_set& get_center_role_set() const { return crs; }

    void write(amtsvr_context* ctx);

private:
    center_role_set crs;
};

#endif
