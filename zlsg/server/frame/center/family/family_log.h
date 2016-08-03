#ifndef __FAMILY_LOG_H__
#define __FAMILY_LOG_H__

#include "define.h"
#include "pb_mgr.h"
#include "client.family.pb.h"
#include "pb_writer.h"

class family_log : public pb_writer
{
public:
    family_log(mysql::tb_family_log* tfl = NULL);
    family_log(uint32 family_id, uint32 time, uint32 type, uint32 key, uint32 value,string family_name,uint32 smy_id);
    virtual ~family_log();

public:
    int save(amtsvr_context* ctx);

    mysql::tb_family_log* get() { return _log; }
    
public:
    int set(uint32 family_id,uint32 time, uint32 type, uint32 key, uint32 value,string family_name,uint32 smy_id);

    int remove();

    int fill(client::family_log_info* fli);
    
private:
    mysql::tb_family_log* _log;
    bool change;
};

#endif /*__FAMILY_LOG_H__ */
