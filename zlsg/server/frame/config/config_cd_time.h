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
	// ģ�������
	uint32						  type_id					   ; // CD����ID
	uint32						  type						   ; // CD����:�ۼ�����/����ʱ���ͣ�1=�ۼӣ�2=�����ͣ�
	uint32						  is_save					   ; // �Ƿ���Ҫ�����CD��0=���洢��1=�洢��
	uint32				          is_send					   ; // �Ƿ��͸��ͻ��ˣ�0=�����ͣ�1=���ͣ�
	uint32						  life_time					   ; // CDʱ�����ã���λ10���룩
	uint32						  cd_time_max				   ; // CD�ۼ�����(����1���ã�2����Ϊ0��CD�ۼӵ������޲���ʹ�ã��������㣬)
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
