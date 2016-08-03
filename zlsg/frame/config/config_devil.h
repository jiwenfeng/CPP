#ifndef __config_devil_H__
#define __config_devil_H__

#include "cfg_cache_mgr.h"
#include "cfg_parser.h"
#include "config_base.h"

class config_devil
{
public:
	inline uint32 get_difficulty()		const { return _diffculty; }
	inline uint32 get_fast_reward()		const { return _fast_reward; }
	inline uint32 get_1st_reward()		const { return _1st_reward; }
	inline uint32 get_again_reward()	const { return _again_reward; }
	inline uint32 get_map_id()			const { return _map_id; }
public:
	uint32 _diffculty;			// 关卡难度
	uint32 _fast_reward;		// 最快通关奖励
	uint32 _1st_reward;			// 首次通关奖励
	uint32 _again_reward;		// 再次通关奖励
	uint32 _map_id;				// 地图ID
};

class config_devil_mgr : public config_base_mgr
{
public:
	config_devil_mgr(TTableGroup_s* p = NULL);
	~config_devil_mgr(void);

public:
    int                                  init(TTableGroup_s* p);
    int                                  clear();

public:
    const TTable* get_table()   const { return _table; }
	const config_devil *get_config_devil(uint32 raid_id) const ;
	const vector<uint32> &get_all_maps_by_stage(uint32 id) const;
	const uint32 get_stage_reward(uint32 id) const;

private:
    TTableGroup_s*                _npc;
    TTableGroup_s*                _gs;
    TTable*                       _table;
	map<uint32, vector<uint32> >  _stage;	// 难度对应的MAP_ID
	map<uint32, config_devil *>	  _devils;
	map<uint32, uint32>			  _reward;	// 难度-->奖励
};

#endif // __config_devil_H__

