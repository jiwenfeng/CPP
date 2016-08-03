#ifndef __CONFIG_LAND_H__
#define __CONFIG_LAND_H__

#include "cfg_cache_mgr.h"
#include "cfg_parser.h"
#include "config_base.h"



// ��������������
class config_land
{
public:
    config_land();
    ~config_land();

public:
	// GET
	inline uint32                              get_land_id			()const { return land_id				;}
	inline const std::map<int, succinct_info>& get_item_1			()const { return item_1				    ;}
	inline const std::map<int, succinct_info>& get_item_2			()const { return item_2					;}
	inline const std::map<int, succinct_info>& get_item_3			()const { return item_3				    ;}
	inline uint32                              get_yuan_bao			()const { return yuan_bao				;}
	inline uint32                              get_land_money		()const { return land_money				;}
	inline const std::map<int, int>&		   get_land_condition	()const { return land_condition			;}
public:

public:
	// ģ�������
	uint32                       land_id                     ; // ��½����
	std::map<int, succinct_info> item_1						 ; // ��½������Ʒ1��ְҵID_����ID_����������ְҵID��0=��ս��2=Ů����5=ȫְҵ��
	std::map<int, succinct_info> item_2						 ; // ��Ʒ2��ְҵID_����ID_����������ְҵID��0=��ս��2=Ů����5=ȫְҵ��
	std::map<int, succinct_info> item_3						 ; // ��Ʒ3��ְҵID_����ID_����������ְҵID��0=��ս��2=Ů����5=ȫְҵ��
	uint32						 yuan_bao					 ; // ����Ԫ��
	uint32						 land_money					 ; // ����ͭǮ
	std::map<int, int>		     land_condition				 ; //��ȡ����������ID|��������������1=��ҵȼ���
};


class config_land_mgr : public config_base_mgr
{
public:
    config_land_mgr(TTableGroup_s* p = NULL);
    ~config_land_mgr();

public:
    int                 init(TTableGroup_s* p);
    int                 clear();

public:
    const TTable*       get_table() const { return _table; }
    const config_land* get_config_land(uint32 land_id) const;

private:
    TTableGroup_s*                            _land;
    TTable*                                   _table;
    std::map<uint32, config_land*>            _cp;
};

#endif // __CONFIG_LAND_H__
