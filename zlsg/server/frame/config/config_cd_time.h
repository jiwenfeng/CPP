#ifndef __CONFIG_CD_TIME_H__
#define __CONFIG_CD_TIME_H__

#include "cfg_cache_mgr.h"
#include "cfg_parser.h"
#include "config_base.h"

class config_cd_time
{
public:
    config_cd_time();
    ~config_cd_time();

public:
	// GET
	inline uint32                              get_type_id			() const {return type_id			 ;}
	inline uint32                              get_type				() const {return type				 ;}
	inline uint32                              get_is_save			() const {return is_save			 ;}
	inline uint32                              get_is_send			() const {return is_send			 ;}
	inline uint32                              get_life_time		() const {return life_time			 ;}
	inline uint32                              get_cd_time_max		() const {return cd_time_max		 ;}
public:

public:
	// 模版表数据
	uint32						  type_id					   ; // CD类型ID
	uint32						  type						   ; // CD类型:累加类型/倒计时类型（1=累加，2=倒计型）
	uint32						  is_save					   ; // 是否需要保存的CD（0=不存储，1=存储）
	uint32				          is_send					   ; // 是否发送给客户端（0=不发送，1=发送）
	uint32						  life_time					   ; // CD时间配置（单位10毫秒）
	uint32						  cd_time_max				   ; // CD累加上限(类型1配置，2配置为0，CD累加到达上限不可使用，必须清零，)
};

class config_cd_time_mgr : public config_base_mgr
{
public:
    config_cd_time_mgr(TTableGroup_s* p = NULL);
    ~config_cd_time_mgr();

public:
    int                 init(TTableGroup_s* p);
    int                 clear();

public:
    const TTable*       get_table() const { return _table; }
    const config_cd_time* get_config_cd_time(uint32 type_id) const;
private:
    TTableGroup_s*                          _cd;
    TTable*                                 _table;
    std::map<uint32, config_cd_time*>		_cp;
};

#endif // __CONFIG_TREASURE_H__
