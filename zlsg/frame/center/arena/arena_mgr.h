#ifndef ARENA_MGR_H
#define ARENA_MGR_H


#include "arena.h"
#include "random.h"
#include "arena_record.h"
#include <cmath>

#define RATE 10000.0f

class formula
{
private:
	float u1;	// 上限系数
	float l1;	// 下限系数
	float u2;	// 上限浮动值
	float l2;	// 下限浮动值
private:
	int get_upper_ret(int rank)
	{
		return max((int)floor(u1 * rank + u2), 1);
	}

	int get_lower_ret(int rank)
	{
		return (int)floor(l1 * rank + l2);
	}

public:
	formula(int u1, int l1, int u2, int l2) : u1(u1 / RATE), l1(l1 / RATE), u2(u2 / RATE), l2(l2 / RATE) 
	{
	}
	int get_challenge_rank(int rank, int max, vector<int> &e)
	{
		vector<int> v;
		int upper = get_upper_ret(rank);
		int lower = min(get_lower_ret(rank), max);
		for(int i = upper; i <= lower; ++i)
		{
			if((i != rank) && (find(e.begin(), e.end(), i) == e.end()))
			{
				v.push_back(i);
			}
		}
		if(v.size() == 0)
		{
			printf("can not find a challenge rang:%d %lf %lf %lf %lf %d %d\n", rank, u1, l1, u2, l2, upper, lower);
			return -1;
		}
		uint32 index = mtrandom::rand_int32(v.size() - 1);
		e.push_back(v[index]);
		return 0;
	}
};

class arena_mgr
{
public:
	arena_mgr();
	~arena_mgr();
public:
	int save(amtsvr_context *ctx);
	int get_role_type(uint32 role_id)const;
	int attach_ctx(amtsvr_context *ctx);
	int get_record_status(uint32 role_id);
	void init();
	void add_role(uint32 role_id, int type);
	void get_challenge_role_list(uint32 role_id, vector<int>& v);
	void challenge_over(client::arena_fight_ret *ret);
	void get_record(uint32 role_id, vector<client::arena_record_info> &v);
	bool is_not_add(uint32 role_id) const;
	const arena *get_arena_info(uint32 role_id) const;
	const arena *get_arena_info(int rank) const;
	const vector<uint32>& get_arena_list() const { return _list; }
	uint32 get_reward_time() { return _reward_time; }

	void reset_reward_time();

private:
	void get_challenge_rank_list(uint32 role_id, vector<int> &v);
	void add_record(uint32 r1, const string &name, int ret, int rank, int action);
	void change_rank(uint32 r1, uint32 r2);
	int load_data();
	void init_challenge_formula(vector<formula> &v);

private:
	map<uint32, arena *>_arena;
	amtsvr_context *_ctx;
	arena_record_mgr _arena_record;
	vector<uint32> _list;
	uint32 _reward_time;		// 下一次发送奖励的时间
};
#endif
