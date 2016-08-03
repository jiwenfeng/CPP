#ifndef __CONFIG_EQUIP_UPGRADE_H__
#define __CONFIG_EQUIP_UPGRADE_H__

#include "cfg_cache_mgr.h"
#include "cfg_parser.h"
#include "config_base.h"

class config_equip_upgrade
{
public:
    config_equip_upgrade();
    ~config_equip_upgrade();

public:
	// GET
	inline uint32                              get_upgrade_level()				const { return upgrade_level		    ;}
	inline uint32                              get_upgrade_stone_ratio()		const { return upgrade_stone_ratio		;}
	inline uint32                              get_upgrade_coin_ratio()			const { return upgrade_coin_ratio		;}
	inline uint32                              get_start_stone_ration()			const { return start_stone_ration		;}
	inline uint32                              get_upgrade_stone_id()			const { return upgrade_stone_id			;}
	inline uint32                              get_start_stone_id()			    const { return start_stone_id			;}

public:

public:
	// 模版表数据
	uint32                       upgrade_level                     ; // 强化的等级
	uint32						 upgrade_stone_ratio			   ; // 强化石消耗系数(0=不可强化)
	uint32						 upgrade_coin_ratio				   ; // 强化铜币消耗系数（0=不可强化）
	uint32						 start_stone_ration				   ; // 升星石消耗系数（0=不可升星）
	uint32					     upgrade_stone_id				   ; // 强化石ID
	uint32						 start_stone_id					   ; // 升星石ID
};

class config_equip_upgrade_mgr : public config_base_mgr
{
public:
    config_equip_upgrade_mgr(TTableGroup_s* p = NULL);
    ~config_equip_upgrade_mgr();

public:
    int                 init(TTableGroup_s* p);
    int                 clear();

public:
    const TTable*       get_table() const { return _table; }
    const config_equip_upgrade* get_config_equip_upgrade(uint32 upgrade_level) const;

private:
    TTableGroup_s*										 _equip_upgrade;
    TTable*												 _table;
    std::map<uint32, config_equip_upgrade*>				 _cp;
};

#endif // __CONFIG_EQUIP_UPGRADE_H__
