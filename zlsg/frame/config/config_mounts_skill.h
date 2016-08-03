#ifndef __CONFIG_MOUNTS_SKILL_H__
#define __CONFIG_MOUNTS_SKILL_H__

#include "cfg_cache_mgr.h"
#include "cfg_parser.h"
#include "config_base.h"

// ���ļ�ֻ�Ǵ������＼�����������츳��ļ������ݣ�

class config_mounts_skill
{
public:
    config_mounts_skill();
    ~config_mounts_skill();

public:
    inline uint32								get_skill_id                 () const { return _skill_id                 ;}
	inline uint32								get_skill_index              () const { return _skill_index              ;}
	inline uint32								get_skill_level              () const { return _skill_level              ;}
	inline uint32								get_show_need_advance        () const { return _show_need_advance        ;}
	inline uint32								get_activation_need_advance  () const { return _activation_need_advance  ;}
	inline uint32								get_select_probability_1     () const { return _select_probability_1     ;}
	inline uint32								get_select_probability_2     () const { return _select_probability_2     ;}
	inline uint32								get_select_probability_3     () const { return _select_probability_3     ;}
	inline uint32								get_upgrade_need_exp         () const { return _upgrade_need_exp         ;}
	inline uint32								get_addtion_exp_1            () const { return _addtion_exp_1            ;}
	inline uint32								get_addtion_exp_2            () const { return _addtion_exp_2            ;}
	inline uint32								get_addtion_exp_3            () const { return _addtion_exp_3            ;}
	inline const std::map<int, int>&			get_consume_money_game       () const { return _consume_money_game       ;}
	inline const std::map<int, int>&			get_consume_money_cash       () const { return _consume_money_cash       ;}
	inline const std::map<int, int>&			get_consume_repute           () const { return _consume_repute           ;}
public:



public:
	uint32                                      _skill_id                 ;// ���＼��
	uint32                                      _skill_index              ;// ��������
	uint32                                      _skill_level              ;// ���ܵȼ�
	uint32										_show_need_advance        ;// ��ʾ��Ҫ�������
	uint32										_activation_need_advance  ;// ����������������
	uint32										_select_probability_1     ;// ��һ��ѡ�м���(���以��)
	uint32										_select_probability_2	  ;// �ڶ���ѡ�м���(��ֱ�)
	uint32										_select_probability_3	  ;// ��������ѡ�м���(��ֱ�)
	uint32										_upgrade_need_exp         ;// �����������辭��
	uint32										_addtion_exp_1            ;// �������Ӿ���
	uint32										_addtion_exp_2            ;// С�������Ӿ���
	uint32										_addtion_exp_3            ;// �󱬻����Ӿ���
	std::map<int, int>							_consume_money_game       ;// �����������Ľ�ң�1|XXXX;2|XXX;�������ƣ�
	std::map<int, int>							_consume_money_cash       ;// ���������������ᣨ1|XXXX;2|XXX;�������ƣ�����Ʒ��
	std::map<int, int>							_consume_repute           ;// ������������������1|XXXX;2|XXX;�������ƣ�
	uint32										_manual_active            ;// �Ƿ��ֶ����0�Զ�1�������飩
};

class config_mounts_skill_mgr : public config_base_mgr
{
public:
    config_mounts_skill_mgr(TTableGroup_s* p = NULL);
    ~config_mounts_skill_mgr();

public:
    int                               init(TTableGroup_s* p);
    int                               clear();

public:
    const TTable*                     get_table() const { return _table; }
    const config_mounts_skill*        get_config_mounts_skill(uint32 skill_id) const;
	const config_mounts_skill*        get_config_mounts_money_pay(uint32 activation_need_advance) const;

private:
    TTableGroup_s*                            _mounts_skill;
    TTable*                                 _table;
    std::map<uint32, config_mounts_skill*>  _cp;
	std::map<uint32, config_mounts_skill*>  _money_pay;// key���������
};

#endif // __CONFIG_MOUNTS_SKILL_H__
