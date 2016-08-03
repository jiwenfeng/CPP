#ifndef __PLAYER_ROLE_LOTTERY_H__
#define __PLAYER_ROLELOTTERY_H___

#include "comm_define.h"
#include "mysql.tb_role_lottery.pb.h"

class player_role_lottery : public pb_writer
{
public:
    player_role_lottery(mysql::tb_role_lottery *trl = NULL);
    virtual ~player_role_lottery();

public:
	void attach_role_lottery(int count, cmq_svr_pb_ctx *pb_ctx);
	uint32 get_prop_num(uint32 lottery_id);
	void add_prop_num(uint32 lottery_id, uint32 num = 1);
	void save(amtsvr_context *ctx, uint32 role_id);
	void modify_lottery_time(uint32 type, uint32 role_id);
	uint32 get_lottery_time(uint32 type);
	uint32 get_used_num();
	void modify_used_num(uint32 role_id, uint32 num);
	void clear_props();
private:
	int serialize(string &str);
	int unserialize(const string &str);

private:
	mysql::tb_role_lottery *_trl;
	std::map<uint32, uint32> _list;
};

#endif /* __PLAYER_ROLE_LOTTERY_H__ */
