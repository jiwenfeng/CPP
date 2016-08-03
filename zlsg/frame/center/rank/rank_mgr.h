#ifndef __RANK_MGR_H__
#define __RANK_MGR_H__

#include "rank.h"
#include <vector>

class rank_mgr
{
public:
	rank_mgr() {};
	~rank_mgr();
	void init();
	void update(amtsvr_context *ctx);
	void save(amtsvr_context *ctx);
	void add_to_rank(uint32 type, client::rank_rule ru);
	void add_to_act_rank(uint32 type, client::rank_rule ru);
	const std::vector<client::rank_rule> &get_rank_list(int type);
	const std::vector<client::rank_rule> &get_real_rank_list(int type);
	uint32 get_rank_num(uint32 type,uint32 role_id);
	uint32 get_act_rank_value(uint32 type,uint32 role_id);
	void clear_rank(uint32 type);
private:
	void init_rank_list();
private:
	std::vector<clsrank *> _rank_list;
};

#endif
