#ifndef __CONFIG_UPGRADE_H__
#define __CONFIG_UPGRADE_H__

#include "cfg_cache_mgr.h"
#include "config_base.h"

class config_upgrade
{
public:
    config_upgrade(void);
    ~config_upgrade(void);

public:
    inline uint64                         get_need_exp						  () const { return _uneed_exp                          ; }
	inline uint32                         get_level							  () const { return _ulevel                             ; }
	inline uint32                         get_rest_income_interval            () const { return _rest_income_interval               ; }
	const std::vector<int32>&             get_strategy                        () const { return _strategy                           ; }
	inline uint32                         get_potential_exp_fail		      () const { return _potential_exp_fail					; }
	inline uint32                         get_potential_exp_success		      () const { return _potential_exp_success				; }
	inline uint32                         get_potential_exp_impact		      () const { return _potential_exp_impact				; }
	inline uint32                         get_potential_daily_exp_impact_max  () const { return _potential_daily_exp_impact_max     ; }
	inline uint32                         get_life_tree_becharge_exp          () const { return _life_tree_becharge_exp             ; }
	inline uint32                         get_life_tree_watering_exp          () const { return _life_tree_watering_exp             ; }
	inline uint32                         get_life_tree_irrigate_exp          () const { return _life_tree_irrigate_exp             ; }
	inline uint32                         get_life_tree_daily_exp_max         () const { return _life_tree_daily_exp_max            ; }
	inline uint32                         get_daily_item_exp_max              () const { return _daily_item_exp_max                 ; }
	inline uint32                         get_qa_reward_exp                   () const { return _qa_reward_exp                      ; }
	inline uint32                         get_qa_reward_strategy              () const { return _qa_reward_strategy                 ; }
	inline uint32                         get_city_war_exp                    () const { return _city_war_exp                       ; }
    const std::map<int, int>&             get_game_attribute                  () const { return _game_attribute						; }
	inline uint32                         get_max_melt_value                  () const { return _max_melt_value					    ; }
	inline uint32						  get_bz_obtain						  () const { return _bz_obtain							; }
	inline uint32						  get_buy_coin						  () const { return _buy_coin							; }
	inline uint32						  get_giant_factor					  () const { return _giant_factor						; }

public:
    uint32                                   _ujob								;// 职业
    uint32                                   _ulevel							;// 等级
    uint64                                   _uneed_exp							;// 经验
    uint32                                   _rest_income_interval				;// 打坐收益时间间隔(秒)
	std::vector<int32>                       _rest_exp							;// 打坐获得经验
	std::vector<int32>                       _strategy							;// 打坐获得韬略
	uint32                                   _potential_exp_fail				;// 道纹失败增加经验
	uint32                                   _potential_exp_success				;// 道纹成功增加经验
	uint32                                   _potential_exp_impact				;// 被道纹冲击波命中获得经验
	uint32                                   _potential_daily_exp_impact_max    ;// 每日最多可获得道纹冲击波经验
	uint32                                   _life_tree_becharge_exp            ;// 被收取获得经验
	uint32                                   _life_tree_watering_exp            ;// 浇水获得经验
	uint32                                   _life_tree_irrigate_exp            ;// 仙露浇灌收益
	uint32                                   _life_tree_daily_exp_max           ;// 神树系统收获每日经验上限
	uint32                                   _daily_item_exp_max                ;// 每日经验丹影响最大数量
	uint32                                   _qa_reward_exp                     ;// 问答奖励经验
	uint32                                   _qa_reward_strategy                ;// 问答奖励韬略
	uint32                                   _city_war_exp                      ;// 城战泡点经验基数
    std::map<int, int>                       _game_attribute					;// 游戏属性列表
	uint32                                   _max_melt_value					;// 可拥有熔炼量大值
	uint32									 _bz_obtain; // 兵阵铜钱系数
	uint32									 _buy_coin; // 购买铜钱系数
	uint32									 _giant_factor; // 过关斩将等级影响因子
};

class config_upgrade_mgr : public config_base_mgr
{
public:
    config_upgrade_mgr(void);
    ~config_upgrade_mgr(void);

public:
    int init(TTableGroup_s* p);
    int clear();

public:
    const TTable* get_table() const { return _table; }
    const config_upgrade* get_config_upgrade(uint32 job, uint32 lvl);
	uint32 get_config_city_war_exp(uint32 job, uint32 lvl);

private:
    TTableGroup_s* _upgrade;
    TTable*      _table;
    std::map<uint32, std::map<uint32, config_upgrade*> > _cu;
};

#endif // __CONFIG_UPGRADE_H__
