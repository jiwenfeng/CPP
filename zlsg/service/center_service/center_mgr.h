#ifndef __CENTER_MGR_H__
#define __CENTER_MGR_H__

#include "amtsvr.h"

class center_mgr
{
public:
    center_mgr() {}

    virtual ~center_mgr() {}

    virtual int init(struct amtsvr_context* c, char* args) = 0;

    void attach_amtsvr(struct amtsvr_context* c) { ctx = c; }

protected:
    struct amtsvr_context* ctx;

};

#endif
