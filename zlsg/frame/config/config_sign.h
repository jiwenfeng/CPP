#ifndef __CONFIG_SIGN_H__
#define __CONFIG_SIGN_H__

#include "cfg_cache_mgr.h"
#include "cfg_parser.h"
#include "config_base.h"

class config_sign
{
public:
    config_sign();
    ~config_sign();

public:
	// GET
	inline uint32                              get_sign_id()		const { return sign_id		       ;}
	inline const std::map<int32,int32>&		   get_reward_item()    const { return sign_reward_item    ;}
	inline uint32                              get_reward_ingot()	const { return reward_ingot		   ;}
	inline uint32                              get_reward_tael()	const { return reward_tael		   ;}
public:

public:
	// 模版表数据
	uint32                       sign_id                     ; // 次数ID
	std::map<int32,int32>        sign_reward_item            ; // 签到奖励物品（道具ID|数量；）
	uint32					     reward_ingot				 ; // 签到奖励的元宝
	uint32						 reward_tael				 ; // 签到讲流利的铜钱
};

class config_sign_mgr : public config_base_mgr
{
public:
    config_sign_mgr(TTableGroup_s* p = NULL);
    ~config_sign_mgr();

public:
    int                 init(TTableGroup_s* p);
    int                 clear();

public:
    const TTable*       get_table() const { return _table; }
    const config_sign* get_config_sign(uint32 sign_id) const;

private:
    TTableGroup_s*                            _sign;
    TTable*                                   _table;
    std::map<uint32, config_sign*>            _cp;
};

#endif // __CONFIG_MALL_H__
