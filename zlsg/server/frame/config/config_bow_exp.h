#ifndef __CONFIG_BOW_EXP_H__
#define __CONFIG_BOW_EXP_H__

#include "cfg_cache_mgr.h"
#include "cfg_parser.h"
#include "config_base.h"

class config_bow_exp
{
public:
    config_bow_exp();
    ~config_bow_exp();

public:
	// GET
	inline uint32                              get_bow_level()			const { return bow_level		;}
	inline uint32                              get_upgrade_exp()		const { return upgrade_exp		;}
	inline uint32                              get_bow_id()				const { return bow_id			;}
public:

public:
	// 模版表数据
	uint32                       bow_level                  ; // 弓箭等级
	uint32						 upgrade_exp				; // 弓箭具体属性
	uint32                       bow_id                     ; // 弓箭ID
};

class config_bow_exp_mgr : public config_base_mgr
{
public:
    config_bow_exp_mgr(TTableGroup_s* p = NULL);
    ~config_bow_exp_mgr();

public:
    int                 init(TTableGroup_s* p);
    int                 clear();

public:
    const TTable*				get_table() const { return _table; }
    const config_bow_exp*		get_config_bow_exp(uint32 bow_level) const;

private:
    TTableGroup_s*                            _bow_exp;
    TTable*                                   _table;
    std::map<uint32, config_bow_exp*>         _cp;
}; 

#endif // __CONFIG_MALL_H__
