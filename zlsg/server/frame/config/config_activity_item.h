#ifndef __CONFIG_ACTIVITY_ITEM_H__
#define __CONFIG_ACTIVITY_ITEM_H__

#include "cfg_cache_mgr.h"
#include "cfg_parser.h"
#include "config_base.h"

class config_activity_item
{
public:
    config_activity_item();
    ~config_activity_item();

public:
	// GET
	inline uint32                              get_activity_type()		const { return activity_type		;}
	inline uint32                              get_activity_id()		const { return activity_id		    ;}
	inline uint32                              get_item_order()		    const { return item_order		    ;}
	inline const std::map<int, int>&		   get_a_conditon()		    const { return a_conditon		    ;}
	inline uint32							   get_get_item()		    const { return get_item		        ;}
	//inline uint32                              get_agio_item()		    const { return agio_item			;}
	inline const std::map<int, int>&		   get_buy_times()		    const { return buy_times		    ;}
	inline uint32                              get_is_reset()		    const { return is_reset				;}
	inline const std::map<int, int>&		   get_b_item_price2()		const { return b_item_price2		;}
	inline uint32                              get_start_time()		    const { return start_time			;}
	inline uint32                              get_pass_time()		    const { return pass_time			;}
	inline uint32                              get_equip_order()		const { return equip_order		    ;}

public:

public:
	// ģ�������
	uint32                       activity_type                    ; // ���
	uint32				         activity_id					  ; // �����ID��1=���ﵤ��ȡ��2=���ﵤ�ۿۣ���
	uint32						 item_order						  ; // ��Ʒ�ڸû�е�˳��(����ʶ��˳����)
	std::map<int,int>			 a_conditon						  ; // ��ȡ����������ID|�׼�;����1=���������
	uint32						 get_item						  ; // ��ȡ��Ʒ����ƷID1|����;ID2|����;��
	uint32                       agio_item						  ; // �ۿ����ID����ƷID��
	std::map<int,int>			 buy_times						  ; // �ɹ����������������ID|����������1=���ˣ�2=�����������ã������ʾû��������
	uint32					     is_reset						  ; // �Ƿ�ÿ�����ã�0�����ã�1���ã�
	std::map<int,int>			 b_item_price2					  ; // �ۿۻ������ۼ۸�Ԫ�����һ�����Ķ���
	uint32						 start_time						  ; // ��������ޣ����뿪��ʱ��X�룩
	uint32						 pass_time						  ; // ��������ޣ����뿪��ʱ��X�룩
	uint32						 equip_order					  ; // �������а�����
};

class config_activity_item_mgr : public config_base_mgr
{
public:
    config_activity_item_mgr(TTableGroup_s* p = NULL);
    ~config_activity_item_mgr();

public:
    int                 init(TTableGroup_s* p);
    int                 clear();

public:
    const TTable*				get_table() const { return _table; }
    const config_activity_item* get_config_activity_item(uint32 activity_type) const;
	const uint32 get_act_vec(uint32 activity_id,std::vector<config_activity_item*>& item_vec) const;
	const uint32 get_act_limit_type(uint32 activity_id)const;
private:
    TTableGroup_s*									_act_item;
    TTable*											_table;
    std::map<uint32, config_activity_item*>         _cp;
};

#endif // __CONFIG_MALL_H__
