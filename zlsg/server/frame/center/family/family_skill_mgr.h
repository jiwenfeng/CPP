#ifndef __FAMILY_SKILL_MGR_H__
#define __FAMILY_SKILL_MGR_H__

#include "comm_define.h"
#include "family.h"
#include "family_skill.h"

class family_skill_mgr
{
public:
    family_skill_mgr();
    virtual ~family_skill_mgr();

public:
    int init(struct cmq_t* l);

    int save(amtsvr_context* ctx);

public:
    family_skill* add(uint32 family_id);

    int remove(uint32 family_id);
    
public:
    //int fill_skills(uint32 family_id, client::family_skills_rsp& rsp);

    //int upgrade_skill(family* f, uint32 skill_id, client::family_skill_upgrade_rsp& rsp);

    int upgrade_limit(family* f, uint32 skill_id);

private:
    map<uint32, family_skill*> _skill;
    vector<family_skill*> _remove;
};

#endif /* __FAMILY_SKILL_MGR_H__ */
