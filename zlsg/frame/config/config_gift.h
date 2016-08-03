#ifndef __CONFIG_GIFT_H__
#define __CONFIG_GIFT_H__

#include "cfg_cache_mgr.h"
#include "cfg_parser.h"
#include "config_base.h"

class config_gift
{
public:
    config_gift();
    ~config_gift();

public:
	// GET
	inline int                                      get_gift_id() const { return gift_id;}
	inline int										get_item_list() const { return item_list;}
public:

public:
	// 模版表数据
	int                       gift_id                     ; // 礼包ID
	int						  item_list					  ; // 物品列表
};

class config_gift_mgr : public config_base_mgr
{
public:
    config_gift_mgr(TTableGroup_s* p = NULL);
    ~config_gift_mgr();

public:
    int                 init(TTableGroup_s* p);
    int                 clear();

public:
    const TTable*       get_table() const { return _table; }
    const config_gift* get_config_gift(uint32 gift_id) const;

private:
    TTableGroup_s*                            _gift;
    TTable*                                 _table;
    std::map<uint32, config_gift*>  _cp;
};

#endif // __CONFIG_GIFT_H__
