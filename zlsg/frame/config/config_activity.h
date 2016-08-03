#ifndef __CONFIG_ACTIVITY_H__
#define __CONFIG_ACTIVITY_H__

#include "cfg_cache_mgr.h"
#include "cfg_parser.h"
#include "config_base.h"

// ��ѯ���
class config_activity
{
public:
    config_activity();
    ~config_activity();

public:
	// GET
	inline uint32                              get_activity_id()		const { return activity_id		;}
	inline uint32                              get_open_type()		    const { return open_type		;}
	inline uint32                              get_interval_day()		const { return interval_day		;}
	inline uint32                              get_start_year()		    const { return start_year		;}
	inline uint32                              get_start_month()		const { return start_month		;}
	inline uint32                              get_start_day()		    const { return start_day		;}
	inline uint32                              get_start_hour()		    const { return start_hour		;}
	inline uint32                              get_start_minute()		const { return start_minute		;}
	inline uint32                              get_start_second()		const { return start_second		;}
	inline uint32                              get_last_time()		    const { return last_time		;}
	inline uint32                              get_fixed_day()		    const { return fixed_day		;}
	inline uint32                              get_repeat_time()		const { return repeat_time		;}

public:

public:
	// ģ�������
	uint32                       activity_id                     ; // �ID
	uint32                       open_type                       ; // ��������
	uint32                       interval_day                    ; // �����������ڼ��쿪��
	uint32                       start_year                      ; // ָ��������
	uint32                       start_month                     ; // ָ��������
	uint32                       start_day                       ; // ָ��������
	uint32						 start_hour						 ; // ָ������ʱ
	uint32						 start_minute					 ; // ָ��������
	uint32						 start_second					 ; // ָ��������
	uint32                       last_time                       ; // �����ʱ�䣨�룩
	uint32                       fixed_day                       ; // ָ�����ڼ���1Ϊ����һ��7Ϊ�����죩
	uint32						 repeat_time					 ; // ѭ�����ʱ�䣨�룩������0Ϊ��ѭ������һ�ֻ����������
};

class config_activity_mgr : public config_base_mgr
{
public:
    config_activity_mgr(TTableGroup_s* p = NULL);
    ~config_activity_mgr();

public:
    int                 init(TTableGroup_s* p);
    int                 clear();

public:
    const TTable*       get_table() const { return _table; }
    const config_activity* get_config_activity(uint32 activity_id) const;
	const std::map<uint32, config_activity*>& get_config_activity_list() const;

private:
    TTableGroup_s*                            _activity;
    TTable*                                   _table;
    std::map<uint32, config_activity*>        _cp;
};

#endif // __CONFIG_MALL_H__
