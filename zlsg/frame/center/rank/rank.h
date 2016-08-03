#ifndef __RANK_H__
#define __RANK_H__

#include <vector>
#include "client.rank.pb.h"
#include "define.h"
#include "pb_writer.h"
#include "comm_define.h"

class bigger
{
public:
	bool operator()(const client::rank_rule& r1, const client::rank_rule& r2)
	{
		return (r1.first() > r2.first()) ||
			(r1.first() == r2.first() && r1.second() > r2.second()) ||
			(r1.first() == r2.first() && r1.second() == r2.second() && r1.third() > r2.third()) ||
			(r1.first() == r2.first() && r1.second() == r2.second() && r1.third() == r2.third() && r1.forth() < r2.forth());
	}
};

class clsrank : public pb_writer
{
public:
	clsrank(int type);
	~clsrank();
public:
	int get_rank_capacity();
	int get_update_frequence();
	int get_rank_type() { return _rank_type; }
	void add_to_rank(client::rank_rule rd);
	void add_to_act_rank(client::rank_rule rd);
	uint32 get_rank_num(uint32 role_id);
	uint32 get_act_rank_value(uint32 role_id);
	void init(mysql::tb_rank *tr);
	void update(amtsvr_context *ctx);
	void save(amtsvr_context *ctx);
	bool need_update();
	const std::vector<client::rank_rule> &get_rank_list() { return _show; }
	const std::vector<client::rank_rule> &get_real_rank_list() { return _real; }
	void clear_rank();
private:
	int serialize(string &out);
	int unserialize(const string &str);
	bool is_full();
	bool rank_list_full() {return true;}
	void print();

protected:
	int _rank_type;
	int _update_time;
	int _need_save;
	int _need_update;
	std::vector<client::rank_data> _rd;
	std::vector<client::rank_rule> _show;
	std::vector<client::rank_rule> _real;
};

#endif
