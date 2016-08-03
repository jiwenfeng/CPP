#ifndef __CONFIG_LEGION_H__
#define __CONFIG_LEGION_H__

#include "cfg_cache_mgr.h"
#include "cfg_parser.h"
#include "config_base.h"

class config_legion
{
public:
    config_legion();
    ~config_legion();

public:
    inline int                             get_legion_level         () const { return legion_level          ; }
	inline int                             get_upgrade_need_lv      () const { return upgrade_need_lv       ; }
	inline int                             get_member_limit         () const { return member_limit			; }
	inline int                             get_create_need_money    () const { return create_need_money		; }
	inline int                             get_upgrade_need_money   () const { return upgrade_need_money	; }
	inline int                             get_create_need_silver	() const { return create_need_silver	; }
	inline int                             get_upgrade_need_silver  () const { return upgrade_need_silver	; }
	inline const std::map<int,int>&		   get_upgrade_need_item    () const { return upgrade_need_item	    ; }
	inline int                             get_add_experience		() const { return add_experience		; }
	inline const std::map<int,int>&		   get_add_property			() const { return add_property			; }
	inline const std::map<int,int>&		   get_donation_1			() const { return donation_1			; }
	inline const std::map<int,int>&		   get_donation_2			() const { return donation_2			; }
	inline const std::map<int,int>&		   get_donation_3			() const { return donation_3			; }
	inline const std::map<int,int>&		   get_donation_4			() const { return donation_4			; }
	inline const std::map<int,int>&		   get_donation_5			() const { return donation_5			; }
	inline int                             get_change_flag_pay		() const { return change_flag_pay		; }
	inline const std::map<int,int>&		   get_change_flag_item	    () const { return change_flag_item	    ; }
	inline int                             get_change_flag_name_pay	() const { return change_flag_name_pay	; }
	inline const std::map<int,int>&		   get_change_flag_name_item() const { return change_flag_name_item	; }
public:

public:
    int                       legion_level         ; // ���ŵȼ�
	int						  upgrade_need_lv	   ; // ������������������ȼ�
	int						  member_limit		   ; // �����ɳ�Ա����
	int						  create_need_money	   ; // �������ĵ�ͭǮ
	int						  upgrade_need_money   ; //	��������ͭǮ
	int						  create_need_silver   ; // ��������Ԫ��
	int						  upgrade_need_silver  ; // ��������Ԫ��
	std::map<int,int>		  upgrade_need_item	   ; // ����������������Ʒ����������Ʒ|����;��Ʒ|����;��
	int						  add_experience	   ; // Ϊ��Ա���ṩɱ�־���ӳɣ��ٷֱȣ�
	std::map<int,int>		  add_property		   ; // Ϊ��Ա���ӵ����ԣ�����|����ֵ������|����ֵ����
	std::map<int,int>		  donation_1		   ; // ��������1 ��������_����_��ðﹱֵ��
	std::map<int,int>         donation_2		   ; // ��������2����ȸ��_����_��ðﹱֵ��
	std::map<int,int>         donation_3		   ; // ��������3��������_����_��ðﹱֵ��
	std::map<int,int>         donation_4		   ; // ��������4���׻���_����_��ðﹱֵ��
	std::map<int,int>		  donation_5		   ; // ��������5ͭǮ������1|��ðﹱֵ������2|��ðﹱֵ����
	int                       change_flag_pay      ; // ����ս����������ͭǮ
	std::map<int,int>         change_flag_item	   ; // ����ս���������ĵ��ߣ���ƷID|��������ƷID|��������
	int                       change_flag_name_pay ; // ����ս����������ͭǮ
	std::map<int,int>         change_flag_name_item; // ����ս���������ĵ��ߣ���ƷID|��������ƷID|��������
};

class config_legion_mgr : public config_base_mgr
{
public:
    config_legion_mgr(TTableGroup_s* p = NULL);
    ~config_legion_mgr();

public:
    int                 init(TTableGroup_s* p);
    int                 clear();

public:
    const TTable*       get_table() const { return _table; }
    const config_legion* get_config_legion(uint32 legion_level) const;

private:
    TTableGroup_s*                     _legion;
    TTable*                          _table;
    std::map<uint32, config_legion*>  _cp;
};

#endif // __CONFIG_LEGION_H__
