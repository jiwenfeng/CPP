#ifndef __FAMILY_RECORD_H__
#define __FAMILY_RECORD_H__

#include "define.h"
#include "pb_mgr.h"
#include "client.family.pb.h"
#include "pb_writer.h"

class family_record : public pb_writer
{
public:
    family_record(mysql::tb_family_record* tfr = NULL);
    family_record(uint32 type, uint32 killer_family, uint32 killer, uint32 slain_family, uint32 slain_title, uint32 slain);
    virtual ~family_record();

public:
    int save(amtsvr_context* ctx);

    mysql::tb_family_record* get() { return _record; }

public:
    int set(uint32 type, uint32 killer_family, uint32 killer, uint32 slain_family, uint32 slain_title, uint32 slain);

    int remove();
    
private:
    mysql::tb_family_record* _record;
    bool _change;
};

#endif /* __FAMILY_RECORD_H__ */
