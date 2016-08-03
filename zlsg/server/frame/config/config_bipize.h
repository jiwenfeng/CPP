#ifndef __CONFIG_BIPIZE_H__
#define __CONFIG_BIPIZE_H__

#include "cfg_cache_mgr.h"
#include "cfg_parser.h"
#include "config_base.h"

class config_bipize
{
public:
    config_bipize();
    ~config_bipize();

public:
	// GET
	inline uint32                              get_range_id()		        const { return range_id		        ;}
	inline uint32                              get_coin_lower_limit()		const { return coin_lower_limit		;}
	inline uint32                              get_ore1_lower_limit()		const { return ore1_lower_limit		;}
	inline uint32                              get_ore2_lower_limit()		const { return ore2_lower_limit		;}
public:

public:
	// 模版表数据
	uint32                       range_id                     ; // 区间编号
	uint32                       coin_lower_limit             ; // 铜钱洗 下限系数(需除以100)
	uint32                       ore1_lower_limit             ; // 一级洗练石洗 下限系数(需除以100)
	uint32                       ore2_lower_limit             ; // 二级洗练石洗 下限系数(需除以100)
};

class config_bipize_mgr : public config_base_mgr
{
public:
    config_bipize_mgr(TTableGroup_s* p = NULL);
    ~config_bipize_mgr();

public:
    int                 init(TTableGroup_s* p);
    int                 clear();

public:
    const TTable*       get_table() const { return _table; }
    const config_bipize* get_config_bipize(uint32 range_id) const;

private:
    TTableGroup_s*                            _bipize;
    TTable*                                   _table;
    std::map<uint32, config_bipize*>          _cp;
};

#endif // __CONFIG_BIPIZE_H__
