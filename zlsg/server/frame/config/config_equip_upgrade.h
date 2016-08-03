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
	// ģ�������
	uint32                       upgrade_level                     ; // ǿ���ĵȼ�
	uint32						 upgrade_stone_ratio			   ; // ǿ��ʯ����ϵ��(0=����ǿ��)
	uint32						 upgrade_coin_ratio				   ; // ǿ��ͭ������ϵ����0=����ǿ����
	uint32						 start_stone_ration				   ; // ����ʯ����ϵ����0=�������ǣ�
	uint32					     upgrade_stone_id				   ; // ǿ��ʯID
	uint32						 start_stone_id					   ; // ����ʯID
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
