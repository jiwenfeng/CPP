#ifndef __CONFIG_POTENTIAL_H__
#define __CONFIG_POTENTIAL_H__

#include "cfg_cache_mgr.h"
#include "cfg_parser.h"
#include "config_base.h"

class config_potential
{
public:
    config_potential();
    ~config_potential();

public:
	// GET
	inline uint32                              get_potential_id			    () const { return _potential_id						;}
	inline uint32                              get_oder_id					() const { return _oder_id							;}
	inline uint32                              get_pre_potential			() const { return _pre_potential					;}
	inline uint32                              get_need_level				() const { return _need_level						;}
	inline uint32                              get_need_potential			() const { return _need_potential                   ;}
	inline uint32                              get_need_game_money			() const { return _need_game_money					;}
	inline uint32                              get_need_probability		    () const { return _need_probability					;}
	inline uint32                              get_open_server_relief       () const { return _open_server_relief               ;}
	inline uint32                              get_need_count_min			() const { return _need_count_min					;}
	inline uint32                              get_need_count_max           () const { return _need_count_max					;}
	inline const std::map<int,int>&            get_game_attribute			() const { return _game_attribute					;}
	inline const std::vector<int>&             get_reward_skill_list        () const { return _reward_skill_list                ;}
	inline const std::vector<int>&             get_clear_skill_list			() const { return _clear_skill_list					;}
	inline uint32                              get_fail_skill_id			() const { return _fail_skill_id					;}
	inline uint32                              get_success_skill_id			() const { return _success_skill_id					;}
	inline uint32                              get_is_world_broadcast       () const { return _is_world_broadcast               ;}
	inline const std::map<int,int>&			   get_append_attribute			() const { return _append_attribute					;}
	inline const std::map<int,int>&			   get_append_need_item			() const { return _append_need_item					;}
	inline std::string                         get_number_name              () const { return _number_name                      ;}

public:

public:
	// 模版表数据
	uint32								 _potential_id                ; // 道纹编号
	uint32								 _oder_id                     ; // 阶数
	uint32								 _pre_potential				  ; // 前置道纹编号
	uint32								 _need_level				  ; // 激活需要等级
	uint32								 _need_potential			  ; // 激活消耗韬略
	uint32								 _need_game_money			  ; // 激活消耗游戏币
	uint32								 _need_probability			  ; // 激活计算用成功几率（万分比本处填分子）
	uint32								 _open_server_relief		  ; // 开服天数减免系数
	uint32								 _need_count_min			  ; // 激活所需次数下限
	uint32								 _need_count_max			  ; // 激活所需次数上限
	std::map<int,int>					 _game_attribute			  ; // 加成属性序列（属性类型|加成值:生效职业;属性类型|加成值;）
	std::vector<int>					 _reward_skill_list			  ; // 激活后奖励特殊技能技能编号序列
	std::vector<int>					 _clear_skill_list			  ; // 激活后清除技能技能编号序列
	uint32								 _fail_skill_id				  ; // 激活失败冲击波技能编号（对应技能数据库）
	uint32								 _success_skill_id			  ; // 激活成功冲击波技能编号（对应技能数据库）
	uint32								 _is_world_broadcast		  ; // 激活成功是否发送全服滚动公告（0不发送，1发送）
	std::map<int,int>					 _append_attribute			  ; // 附加属性属性序列（属性类型|加成值:生效职业;属性类型|加成值;）
	std::map<int,int>					 _append_need_item			  ; // 需要道具(道具id_数量)只支持一种道具
	std::string							 _number_name			      ; // 道纹名称
};

class config_potential_mgr : public config_base_mgr
{
public:
    config_potential_mgr(TTableGroup_s* p = NULL);
    ~config_potential_mgr();

public:
    int                 init(TTableGroup_s* p);
    int                 clear();

public:
    const TTable*       get_table() const { return _table; }
    const config_potential* get_config_potential(uint32 potential_id) const;

private:
    TTableGroup_s*                               _potential;
    TTable*                                    _table;
    std::map<uint32, config_potential*>        _cp;
};

#endif // __CONFIG_POTENTIAL_H__
