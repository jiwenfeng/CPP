#ifndef __FAMILY_DIPLOMACY_H__
#define __FAMILY_DIPLOMACY_H__

#include "define.h"
#include "pb_mgr.h"
#include "client.family.pb.h"
#include "pb_writer.h"

class family_diplomacy : public pb_writer
{
public:
    family_diplomacy(mysql::tb_family_diplomacy* family_diplomacy_db = NULL);
    family_diplomacy(uint32 family_id, uint32 target_id,uint32 relation_type,uint32 time = 0);
    virtual ~family_diplomacy();

public:
    int save(amtsvr_context* ctx);

    mysql::tb_family_diplomacy* get_family_diplomacy() { return _diplomacy; }

public:
    int create_diplomacy_info(uint32 family_id, uint32 target_id,uint32 relation_type,uint32 apply_time = 0);
    
	int change_relaition_type(uint32 relation_type);

    int remove();

	uint32 get_target_family_relaition();
private:
    mysql::tb_family_diplomacy* _diplomacy;
    bool _change;
};

#endif /* __FAMILY_DOMAIN_H__ */
