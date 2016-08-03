#ifndef __config_giant_H__
#define __config_giant_H__

#include "cfg_cache_mgr.h"
#include "cfg_parser.h"
#include "config_npc.h"
#include "config_base.h"

class config_giant
{
public:
	inline uint32 get_giant_id() const { return _giant_id; }
	inline uint32 get_giant_group() const { return _giant_group; }
	inline uint32 get_level() const { return _level; }
public:
	uint32 _giant_id;
	uint32 _giant_group;
	uint32 _level;
};

class config_giant_mgr : public config_base_mgr
{
public:
	config_giant_mgr(TTableGroup_s* p = NULL);
	~config_giant_mgr(void);

public:
    int                                  init(TTableGroup_s* p);
    int                                  clear();
	void get_giant_unlock_group(uint32 lvl, vector<uint32> &v);
	uint32 get_giant_id(uint32 group, uint32 lvl);

public:
    const TTable* get_table()   const { return _table; }
    const config_npc* get_config_giant(uint32 id) const;

private:
    TTableGroup_s*                _npc;
    TTableGroup_s*                _gs;
    TTable*                       _table;
    std::map<uint32, config_npc*> _cb;
	std::multimap<uint32, config_giant *> _cg;
};

#endif // __config_npc_H__

