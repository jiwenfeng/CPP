#ifndef __CONFIG_MELTING_EQUIP_H__
#define __CONFIG_MELTING_EQUIP_H__

#include "cfg_cache_mgr.h"
#include "cfg_parser.h"
#include "config_base.h"

// 熔炼模版数据
class config_melting_equip
{
public:
    config_melting_equip();
    ~config_melting_equip();

public:
	inline int                         get_equip_id     () const {return item_id        ;}
	inline int                         get_improve_level() const {return improve_level  ;}
	inline int                         get_melting_value() const {return melting_value  ;}
public:

public:
	// 模版表数据
	int                                item_id;       // 装备ID
	int                                improve_level;  // 强化等级
	int                                melting_value;  // 熔炼值 
};

class config_melting_equip_mgr : public config_base_mgr
{
public:
    config_melting_equip_mgr(TTableGroup_s* p = NULL);
    ~config_melting_equip_mgr();

public:
    int                 init(TTableGroup_s* p);
    int                 clear();

public:
    const TTable*       get_table() const { return _table; }
    const config_melting_equip* get_config_melting_equip(uint32 item_id,int improve_level) const;

private:
    TTableGroup_s*                                  _melting_equip;
    TTable*                                       _table;
    std::multimap<uint32, config_melting_equip*>  _cp;
};

#endif // __CONFIG_UPEQUIP_H__
