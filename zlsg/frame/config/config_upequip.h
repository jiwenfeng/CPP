#ifndef __CONFIG_UPEQUIP_H__
#define __CONFIG_UPEQUIP_H__

#include "cfg_cache_mgr.h"
#include "cfg_parser.h"
#include "config_base.h"

class config_upequip
{
public:
    config_upequip();
    ~config_upequip();

public:
	// GET
	inline int                              get_equip_id() const {return equip_id;}
	inline int				                get_improve_level() const {return improve_level;}
	inline int                              get_can_improve() const {return can_improve;}
	inline const std::map<int, int>&        get_improve_need_material () const {return improve_need_material;}
	inline int								get_improve_need_game_money() const {return improve_need_game_money;}
	inline int								get_improve_need_time() const {return improve_need_time;}
	inline int								get_improve_now_need_money() const {return improve_now_need_money;}
	inline int								get_improve_min_times() const {return improve_min_times;}
	inline int								get_improve_rate() const {return improve_rate;}
	inline int								get_improve_max_times() const {return improve_max_times;}
	inline const std::map<int, int>&		get_improve_failed_reward() const {return improve_failed_reward;}//��
	inline int								get_improve_failed_down() const {return improve_failed_down;}
	inline int								get_improve_broadcast() const {return improve_broadcast;}
	inline const std::map<int, int>&		get_improve_update_attr() const {return improve_update_attr;}// key :�������ͣ�value:����ֵ
	inline int								get_can_upgrade() const {return can_upgrade;}
	inline const std::map<int, int>&		get_upgrade_need_material() const {return upgrade_need_material;}// key:����ID value :���������ף�
	inline int								get_upgrade_need_game_money() const {return upgrade_need_game_money;}
	inline int								get_upgrade_min_times() const {return upgrade_min_times;}
	inline int								get_upgrade_rate() const {return upgrade_rate;}
	inline int								get_upgrade_max_times() const {return upgrade_max_times;}
	inline int								get_upgrade_target_equip() const {return upgrade_target_equip;}
	inline int								get_upgraded_improve_level() const {return upgraded_improve_level;}
	inline int								get_upgrade_broadcast() const {return upgrade_broadcast;}
	inline const std::vector<int>& 		    get_upgrade_replace_item() const {return upgrade_replace_item;}
	inline const std::vector<int>& 		    get_improve_replace_item() const {return improve_replace_item;}
	inline const std::map<int, int>&		get_improve_update_attr_1() const {return improve_update_attr_1;}//��
public:
	//inline int get_improve_need_material_by_id(int item_id)
	//{
	//	std::map<int, int>::iterator it = improve_need_material.find(item_id);
	//	return it != improve_need_material.end() ? it->second : 0;
	//}
public:
	// ģ�������
	int                       equip_id                     ; // ��ƷID
	int                       improve_level                ; // ǿ���ȼ�
	int                       can_improve                  ; // �ܷ�����ǿ��
	std::map<int, int>        improve_need_material		   ; // ǿ���������ݣ�����ID,������
	int                       improve_need_game_money      ; // ǿ������ͭǮ
	int                       improve_need_time            ; // ǿ����ʱ����λ���룩
	int                       improve_now_need_money       ; // ������ɹ�������Ԫ������
	int                       improve_min_times            ; // ǿ���ɹ�����ǿ������min
	int                       improve_rate                 ; // �������˼����óɹ�����
	int                       improve_max_times            ; // ǿ���ɹ�����ǿ������max
	std::map<int, int>        improve_failed_reward        ; // ǿ��ʧ�ܸ�����Ʒ���У���ʽ��������ֱȷ���|��ƷID_����;������ֱȷ���|��ƷID_������
	int                       improve_failed_down          ; // ǿ��ʧ�ܽ��ǿ���
	int                       improve_broadcast            ; // ǿ���ɹ�ȫ���������ݣ���Ӧ�㲥��id��
	std::map<int, int>        improve_update_attr          ; // ǿ���ɹ����ӻ�����������
	int                       can_upgrade                  ; // �Ƿ�������ף�0������1����
	std::map<int, int>		  upgrade_need_material        ; // �������Ĳ������У���ʽ������ID_����;����ID_����;��
	int                       upgrade_need_game_money      ; // ��������ͭǮ
	int                       upgrade_min_times            ; // ���׳ɹ������������min
	int                       upgrade_rate                 ; // �������˼����ý��׳ɹ�����
	int						  upgrade_max_times			   ; // ���׳ɹ������������max
	int						  upgrade_target_equip         ; // ������װ��ID
	int                       upgraded_improve_level	   ; // ���׺�װ��ǿ���ȼ�
	int                       upgrade_broadcast			   ; // ���׳ɹ��Ƿ�ȫ�����棨��Ӧ�㲥��id��
	std::vector<int> 		  upgrade_replace_item		   ; // ������ʹ�ô����б�(itemid_itemid;itemid_itemid)
	std::vector<int>         improve_replace_item		   ; // ǿ����ʹ�ô����б�(itemid_itemid;itemid_itemid)
	std::map<int, int> 		  improve_update_attr_1        ; // ǿ���ɹ����ӻ����������С���ɫ��
};

class config_upequip_mgr : public config_base_mgr
{
public:
    config_upequip_mgr(TTableGroup_s* p = NULL);
    ~config_upequip_mgr();

public:
    int                 init(TTableGroup_s* p);
    int                 clear();

public:
    const TTable*       get_table() const { return _table; }
    const config_upequip* get_config_upequip(uint32 equip_id,int strengthen_level) const;

private:
    TTableGroup_s*                            _upequip;
    TTable*                                 _table;
    std::multimap<uint32, config_upequip*>  _cp;
};

#endif // __CONFIG_UPEQUIP_H__
