#ifndef __LOADER_INTERFACE_H__
#define __LOADER_INTERFACE_H__

#include "loader_role_mgr.h"
#include "cmq_svr_msg.h"

struct loader_t{
    loader_role_mgr lr_mgr;
    amtsvr_context*   ctx;

    loader_t()// : lr_mgr(this)
    {
        lr_mgr.set_ud(this);
        ctx = NULL;
    }
};

#endif
