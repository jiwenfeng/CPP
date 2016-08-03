#ifndef __FAMILY_SKILL_H__
#define __FAMILY_SKILL_H__

#include "define.h"
#include "pb_mgr.h"
#include "client.family.pb.h"
#include "pb_writer.h"

#define MAX_SKILLS_BOLB_LEN 66560

class family_skill : public pb_writer
{
public:
    family_skill(mysql::tb_family_skill* tfs = NULL);
    family_skill(uint32 family_id);
    virtual ~family_skill();

public:
    int init(uint32 family_id);

    void clear();
    
    int parse(const string& in);

    int serialize(string& out);
    
    int save(amtsvr_context* ctx);

    void remove();
    
public:
    int upgrade(uint32 id);
    
    int set(uint32 id, uint32 lvl);

    int get_lvl(uint32 id);

public:
    //int    fill_skills(client::family_skills_rsp& rsp);
    //int    fill_skill(uint32 skill_id, client::family_skill_info* fsi);

    uint32 id_to_index(uint32 id);

public:
    static const uint32 SKILL_COUNT = 13;
    static const uint32 SKILL_ID[SKILL_COUNT];
    static const uint32 MAX_FAMILY_SKILL_LVL = 10;
    static const uint32 UPGRADE_MONEY[2][MAX_FAMILY_SKILL_LVL];
    
private:
    mysql::tb_family_skill*    _skill;
    bool                       _change;
    //client::family_skills_rsp  _info;
};

#endif /* __FAMILY_SKILL_H__ */
