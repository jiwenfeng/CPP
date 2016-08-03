#ifndef __CONFIG_MAGIC_H__
#define __CONFIG_MAGIC_H__

#include "cfg_cache_mgr.h"
#include "cfg_parser.h"
#include "config_base.h"

class config_magic
{
public:
    config_magic();
    ~config_magic();

public:
	// GET
	inline uint32                              get_magic_id				()const { return magic_id			;}
	inline uint32                              get_stage_star			()const { return stage_star			;}
	inline const std::map<int,int>&			   get_star_need			()const { return star_need			;}
	inline uint32                              get_magic_skill			()const { return magic_skill		;}
	//inline uint32                              get_magic_add_attack		()const { return magic_add_attack	;}
	inline uint32                              get_general_id			()const { return general_id			;}
	inline const std::map<int,int>&			   get_up_charater			()const { return up_charater		;}
	inline const std::map<int,int>&			   get_up_general			()const { return up_general			;}
public:

public:
	// ģ�������
	uint32                       magic_id                     ; // ���ID	
	uint32						 stage_star					  ; // �Ǽ�
	std::map<int,int>			 star_need					  ; // �������ĵ��ߣ�����ID|������
	uint32						 magic_skill				  ; // �������
	//uint32						 magic_add_attack			  ; // ���ӵ�����˹�����ֱ�
	uint32						 general_id					  ; // ���ӵ����ID���������˱�general_id��
	std::map<int,int>			 up_charater				  ;//�������������ۼ�(����1ID|����ֵ;����2ID|����ֵ;����3ID|����ֵ;)
	std::map<int,int>			 up_general					  ;//�������������ۼ�(����1ID|����ֵ;����2ID|����ֵ;����3ID|����ֵ;)
};

class config_magic_mgr : public config_base_mgr
{
public:
    config_magic_mgr(TTableGroup_s* p = NULL);
    ~config_magic_mgr();

public:
    int                 init(TTableGroup_s* p);
    int                 clear();

public:
    const TTable*       get_table() const { return _table; }
    const config_magic* get_config_magic(uint32 magic_id,uint32 stage_star) const;

private:
    TTableGroup_s*                            _magic;
    TTable*                                 _table;
    std::multimap<uint32, config_magic*>    _cp;
};

#endif // __CONFIG_MAGIC_H__
