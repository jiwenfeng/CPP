#ifndef __FAMILY_RECORD_MGR_H__
#define __FAMILY_RECORD_MGR_H__

#include "comm_define.h"
#include "family_record.h"
#include "family_member_mgr.h"

#define RECORD_TYPE_MAX 3

class family_record_mgr
{
public:
    family_record_mgr();
    virtual ~family_record_mgr();

public:
    int init(struct cmq_t* l);

    int save(amtsvr_context* ctx);

public:
    int get_record(uint32 family_id);
    
    int update(client::family_record_update* fru);

    int remove(uint32 family_id, uint32 role_id = 0);
    int remove(multimap<uint32, family_record*>& m, uint32 role_id);

    int fill(uint32 family_id, client::family_record_rsp& rsp);

public:
    void delete_all();

private:
    void add(family_record* fr);

    void init_info(family_record* record, multimap<uint32, client::family_record_info*>& info);
    void set_info(family_record* record, client::family_record_info* info);
    
    void make_info();

    void to_info(family_record* p);

    void set_kill_info();
    
private:
    multimap<uint32, multimap<uint32, family_record*> > _record;   // family to role to record
    map<uint32, multimap<uint32, client::family_record_info*> > _info;

    map<uint32, map<uint32, map<uint32, uint32> > > _kill;  // type to slain to killer and num

    map<uint32, family_record*> _remove;
};

#endif /* __FAMILY_RECORD_MGR_H__ */

