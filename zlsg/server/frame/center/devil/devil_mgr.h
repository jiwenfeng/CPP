#ifndef __DEVIL_MGR_H__
#define __DEVIL_MGR_H__

#include "define.h"
#include "pb_writer.h"
#include "comm_define.h"
#include "mysql.tb_center_devil.pb.h"
class devil_mgr : public pb_writer
{
public:
	devil_mgr();
	~devil_mgr();

public:
	void init();
	void save(amtsvr_context *ctx);

public:
	int add_pass_level(uint32 id, uint32 role_id, uint32 tick);
	const mysql::tb_center_devil *get_pass_info(uint32 id) const;

private:
	std::map<uint32, mysql::tb_center_devil *> _devils;
	vector<mysql::tb_center_devil *> _update;
	vector<mysql::tb_center_devil *> _insert;
};

#endif
