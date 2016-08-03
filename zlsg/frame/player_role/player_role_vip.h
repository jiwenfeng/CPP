#ifndef __PLAYER_ROLE_VIP_H__
#define __PLAYER_ROLE_VIP_H__

#include "comm_define.h"
#include "mysql.tb_role_vip.pb.h"
#include "client.vip.pb.h"

class player_role_vip : public pb_writer
{
public:
    player_role_vip(mysql::tb_role_vip *trv = NULL);
    virtual ~player_role_vip();

public:
	int save(amtsvr_context *ctx, uint32 role_id);
	void attach_role_vip(int count, cmq_svr_pb_ctx *pb_ctx);
	void clear_vip_op_info();
	void set_op_count(uint32 op, uint32 id = 1, uint32 times = 1);
	void add_recharge_times(uint32 id, uint32 n = 1) { _buy[id] += n; }
	bool op_has_left_times(uint32 op, uint32 id = 1);
	uint32 get_op_count(uint32 op, uint32 id = 1);
	uint32 get_vip_exp();
	uint32 get_recharge_times(uint32 id) { return _buy[id]; }
	uint32 get_month_timeout();
	uint32 add_vip_exp(uint32 role_id, uint32 n, bool month = false);
	uint32 get_vip_lv();
	void set_vip_lv(uint32 role_id, uint32 lv, bool month = false);

private:
	int unserialize_op(const string &str);
	int serialize_op(string &str);
	int serialize_buy(string &str);
	int unserialize_buy(const string &str);

private:
	mysql::tb_role_vip *_trv;
	// map<uint32, uint32> _op;
	map<uint32, uint32> _buy;
	map<uint32, map<uint32, uint32> > _op;
};

#endif /* __PLAYER_ROLE_VIP_H__ */
