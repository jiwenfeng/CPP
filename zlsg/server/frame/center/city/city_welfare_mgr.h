#ifndef __CITY_WELFARE_MGR_H__
#define __CITY_WELFARE_MGR_H__

#include "define.h"
#include "pb_writer.h"
#include "comm_define.h"

class city_welfare_mgr:public pb_writer
{
public:
	city_welfare_mgr();
	~city_welfare_mgr();
	int save(amtsvr_context *ctx);
	int init(amtsvr_context *ctx);
	int mod_family_id(amtsvr_context *ctx, uint32 family_id);
	int check_welfare_status(uint32 family_id, uint32 pos);
	int set_time(int index);
	uint32 get_family_id();
	uint32 mod_pos_welfare_time(uint32 pos);
	uint32 get_time(int index);
	bool welfare_has_got(int index);
private:
	mysql::tb_city_welfare *_tcw;
	bool _need_save;
	amtsvr_context *_ctx;
};

#endif
