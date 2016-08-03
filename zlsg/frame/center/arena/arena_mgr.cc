#include "arena_mgr.h"
#include "base_time.h"

arena_mgr::arena_mgr():_reward_time(0)
{
}

arena_mgr::~arena_mgr()
{
	_list.clear();
	map<uint32, arena *>::iterator it = _arena.begin() ;
	for(; it != _arena.end(); ++it)
	{
		delete it->second;
	}
	_arena.clear();
}

int arena_mgr::save(amtsvr_context *ctx)
{
	map<uint32, arena *>::iterator it = _arena.begin();
	for(; it != _arena.end(); ++it)
	{
		it->second->save(ctx);
	}
	_arena_record.save(ctx);
	return 0;
}

void arena_mgr::reset_reward_time()
{
	if(0 == _reward_time)
	{
		_reward_time = time_utils::make_time_special(0, 21, 0, 0);
	}
	else
	{
		_reward_time += 24 * 3600;
	}
}

void arena_mgr::init()
{
	load_data();				// 加载竞技场数据
	_arena_record.init();		// 加载日志数据
	reset_reward_time();
}

void arena_mgr::init_challenge_formula(vector<formula> &v)
{
	v.clear();
	for(int i = 0; i < 4; ++i)
	{
		v.push_back(formula(CONFMGR->get_basic_value(10024 + i * 4 + 0),
					CONFMGR->get_basic_value(10024 + i * 4 + 1),
					CONFMGR->get_basic_value(10024 + i * 4 + 2),
					CONFMGR->get_basic_value(10024 + i * 4 + 3)));
	}
}

int arena_mgr::load_data()
{
	hash_map_obj<CACHE_KEY, cfg_cache_obj, CFG_CACHE_BULB_NUM>::pair_t *cur = (hash_map_obj<CACHE_KEY, cfg_cache_obj, CFG_CACHE_BULB_NUM>::pair_t *)CONFMGR->get_center_arena_head();
//	int max = 0;
	while(cur != NULL)
	{
		mysql::tb_center_arena *tca = static_cast<mysql::tb_center_arena *>(cur->mutable_val().pop_msg());
		cur = cur->get_next();
		if(NULL == tca)
		{
			continue;
		}
		arena *a = new arena(tca);
		_arena.insert(make_pair(tca->role_id(), a));
		_list.push_back(tca->role_id());
//		if(a->get_rank() > max)
//		{
//			max = a->get_rank();
//		}
	}
//	_list.resize(max);
//	map<uint32, arena *>::iterator it = _arena.begin();
//	for(; it != _arena.end(); ++it)
//	{
//		_list[it->second->get_rank() - 1] = it->second->get_role_id();
//	}
	return 0;
}

//新进入竞技场的总是排在最末尾
void arena_mgr::add_role(uint32 role_id, int type)
{
	if(_arena.find(role_id) != _arena.end())
	{
		return;
	}
	_list.push_back(role_id);
	arena *a = new arena;
	a->add_data(role_id, _list.size(), type);
	_arena.insert(make_pair(role_id, a));
}

bool arena_mgr::is_not_add(uint32 role_id) const
{
	return _arena.find(role_id) == _arena.end();
}

//获取可挑战的名次
void arena_mgr::get_challenge_rank_list(uint32 role_id, vector<int> &v)
{
	map<uint32, arena *>::iterator it = _arena.find(role_id);
	if(it == _arena.end())
	{
		return;
	}
	int rank = it->second->get_rank();
	vector<formula> vf;
	init_challenge_formula(vf);
	for(size_t i = 0; i < vf.size(); ++i)
	{
		if(-1 == vf[i].get_challenge_rank(rank, _list.size(), v))
		{
			break;
		}
	}
}

//可挑战的玩家列表
void arena_mgr::get_challenge_role_list(uint32 role_id, vector<int> &v)
{
	get_challenge_rank_list(role_id, v);
}

//更新排名
void arena_mgr::change_rank(uint32 r1, uint32 r2)
{
	map<uint32, arena *>::iterator i1 = _arena.find(r1);
	if(i1 == _arena.end())
	{
		return;
	}
	map<uint32, arena *>::iterator i2 = _arena.find(r2);
	if(i2 == _arena.end())
	{
		return;
	}
	if(i1->second->get_rank() < i2->second->get_rank())
	{
		return;
	}
	if(i1->second->get_rank() < 0 || i2->second->get_rank() < 0)
	{
		return;
	}
	_list[i1->second->get_rank() - 1] = i2->second->get_role_id();
	_list[i2->second->get_rank() - 1] = i1->second->get_role_id();
	int rank = i1->second->get_rank();
	i1->second->set_rank(i2->second->get_rank());
	i2->second->set_rank(rank);
}

const arena *arena_mgr::get_arena_info(uint32 role_id) const
{
	map<uint32, arena *>::const_iterator it = _arena.find(role_id);
	if(it == _arena.end())
	{
		return NULL;
	}
	return it->second;
}

const arena *arena_mgr::get_arena_info(int rank) const
{
	if(rank < 1 || rank > (int)_list.size())
	{
		return NULL;
	}
	uint32 role_id = _list[rank - 1];
	return get_arena_info(role_id);
}

void arena_mgr::challenge_over(client::arena_fight_ret *ret)
{
	const arena *a1 = get_arena_info(ret->role_id());
	if(!a1)
	{
		return;
	}
	const arena *a2 = get_arena_info(ret->target());
	if(!a2)
	{
		return;
	}
	int diff = a1->get_rank() - a2->get_rank();
	int action = 0;
	add_record(ret->role_id(), ret->t_name(), ret->ret(), diff < 0 ? 0 : a2->get_rank(), action);	// 0 主动
	add_record(ret->target(), ret->r_name(), !ret->ret(), diff < 0 ? 0 : a1->get_rank(), !action);	// 1 被动
	// 交换排名
	if((ret->ret() == client::FIGHT_WIN) && (a1->get_rank() > a2->get_rank()))
	{
		change_rank(ret->role_id(), ret->target());
	}
}

void arena_mgr::add_record(uint32 r1, const string &name, int ret, int rank,int action)
{
	// 机器人不需要记录日志
	if(get_role_type(r1) == ARENA_ROBOT)
	{
		return;
	}
	if((action == 1 && ret == client::FIGHT_WIN) || (action == 0 && ret == client::FIGHT_LOSE))
	{
		rank = 0;
	}
	_arena_record.add_record(r1, name, rank, action, ret);
}

int arena_mgr::get_role_type(uint32 role_id)const
{
	map<uint32, arena *>::const_iterator it = _arena.find(role_id);
	if(it == _arena.end())
	{
		return 0;
	}
	return it->second->get_role_type();
}

int arena_mgr::attach_ctx(amtsvr_context *ctx)
{
	_ctx = ctx;
	_arena_record.attach_ctx(ctx);
	return 0;
}

void arena_mgr::get_record(uint32 role_id, vector<client::arena_record_info> &v)
{
	return _arena_record.get_record(role_id, v);
}

int arena_mgr::get_record_status(uint32 role_id)
{
	return _arena_record.get_record_status(role_id);
}
