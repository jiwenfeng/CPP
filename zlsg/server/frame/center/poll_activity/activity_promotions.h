#ifndef __ACTIVITY_PROMOTIONS_H__
#define __ACTIVITY_PROMOTIONS_H__

#include "define.h"
#include "pb_mgr.h"
#include "pb_writer.h"

#define MAX_ROLE_BUF_NUM 128

#define MAX_ACTIVITY_RECORD_BOLB_LEN 65535  // POTENTIAL最大缓存长度 

class activity_promotions : public pb_writer
{
public:

	activity_promotions(mysql::tb_promotions* db = NULL);

	activity_promotions(uint32 act_id,string name,uint32 type,uint32 start_time,uint32 duration_time,uint32 loop_time);

	virtual ~activity_promotions();

	int save(amtsvr_context* ctx);

public:

	int update_pro(string name,uint32 type,uint32 start_time,uint32 duration_time,uint32 loop_time);

	void set_act_status(uint32 act_status);

	inline uint32 get_state(){return _db_promotions->state();}

	void set_activity_time(uint32 start_time,uint32 close_time);

	inline const mysql::tb_promotions* get_db_info()const{return _db_promotions;}

	mysql::tb_promotions* mutable_db_info() { return _db_promotions;}

private:

	mysql::tb_promotions* _db_promotions;

	bool _change;
};

#endif