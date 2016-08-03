#ifndef __CONFIG_MOUNTS_SKILL_H__
#define __CONFIG_MOUNTS_SKILL_H__

#include "cfg_cache_mgr.h"
#include "cfg_parser.h"
#include "config_base.h"

// 该文件只是处理坐骑技能升级（非天赋类的技能数据）

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
	uint32                                      _skill_id                 ;// 坐骑技能
	uint32                                      _skill_index              ;// 技能索引
	uint32                                      _skill_level              ;// 技能等级
	uint32										_show_need_advance        ;// 显示需要坐骑阶数
	uint32										_activation_need_advance  ;// 激活技能所需坐骑阶数
	uint32										_select_probability_1     ;// 第一格被选中几率(区间互斥)
	uint32										_select_probability_2	  ;// 第二格被选中几率(万分比)
	uint32										_select_probability_3	  ;// 第三个被选中几率(万分比)
	uint32										_upgrade_need_exp         ;// 升级技能所需经验
	uint32										_addtion_exp_1            ;// 单球增加经验
	uint32										_addtion_exp_2            ;// 小爆击增加经验
	uint32										_addtion_exp_3            ;// 大爆击增加经验
	std::map<int, int>							_consume_money_game       ;// 历次拉动消耗金币（1|XXXX;2|XXX;依次类推）
	std::map<int, int>							_consume_money_cash       ;// 历次拉动消耗银贯（1|XXXX;2|XXX;依次类推）（物品）
	std::map<int, int>							_consume_repute           ;// 历次拉动消耗声望（1|XXXX;2|XXX;依次类推）
	uint32										_manual_active            ;// 是否手动激活（0自动1需求技能书）
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
	std::map<uint32, config_mounts_skill*>  _money_pay;// key是坐骑阶数
};

#endif // __CONFIG_MOUNTS_SKILL_H__
