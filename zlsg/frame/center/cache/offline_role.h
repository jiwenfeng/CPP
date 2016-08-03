#ifndef __OFFLINE_ROLE_H__
#define __OFFLINE_ROLE_H__

#include "player_role.h"
#include "center_role.h"

enum OFFLIEN_PTR_TYPE_E
{
    OPTE_CONFLICT = 0,
    OPTE_LIST     = 1,
    OPTE_MAX      = 2,
};

class offline_role_mgr;

class offline_role
{
friend class offline_role_mgr;
public:
    offline_role(player_role* r = NULL, center_role* c = NULL);
    virtual ~offline_role();

public:
    uint32 get_role_id() const;

    void attach(player_role* r);
    void attach(center_role* c);
    void detach();

    player_role* pop();
    center_role* pop_cr();

    inline player_role* get() const { return _pr; }
    inline center_role* get_cr() const { return _cr; }

protected:
    player_role* _pr;
    center_role* _cr;

    offline_role* prev[OPTE_MAX];
    offline_role* next[OPTE_MAX];
};

#endif
