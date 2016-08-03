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
	inline const std::map<int, int>&		get_improve_failed_reward() const {return improve_failed_reward;}//？
	inline int								get_improve_failed_down() const {return improve_failed_down;}
	inline int								get_improve_broadcast() const {return improve_broadcast;}
	inline const std::map<int, int>&		get_improve_update_attr() const {return improve_update_attr;}// key :属性类型，value:属性值
	inline int								get_can_upgrade() const {return can_upgrade;}
	inline const std::map<int, int>&		get_upgrade_need_material() const {return upgrade_need_material;}// key:材料ID value :数量（进阶）
	inline int								get_upgrade_need_game_money() const {return upgrade_need_game_money;}
	inline int								get_upgrade_min_times() const {return upgrade_min_times;}
	inline int								get_upgrade_rate() const {return upgrade_rate;}
	inline int								get_upgrade_max_times() const {return upgrade_max_times;}
	inline int								get_upgrade_target_equip() const {return upgrade_target_equip;}
	inline int								get_upgraded_improve_level() const {return upgraded_improve_level;}
	inline int								get_upgrade_broadcast() const {return upgrade_broadcast;}
	inline const std::vector<int>& 		    get_upgrade_replace_item() const {return upgrade_replace_item;}
	inline const std::vector<int>& 		    get_improve_replace_item() const {return improve_replace_item;}
	inline const std::map<int, int>&		get_improve_update_attr_1() const {return improve_update_attr_1;}//？
public:
	//inline int get_improve_need_material_by_id(int item_id)
	//{
	//	std::map<int, int>::iterator it = improve_need_material.find(item_id);
	//	return it != improve_need_material.end() ? it->second : 0;
	//}
public:
	// 模版表数据
	int                       equip_id                     ; // 物品ID
	int                       improve_level                ; // 强化等级
	int                       can_improve                  ; // 能否允许强化
	std::map<int, int>        improve_need_material		   ; // 强化所需数据（材料ID,数量）
	int                       improve_need_game_money      ; // 强化消耗铜钱
	int                       improve_need_time            ; // 强化耗时（单位：秒）
	int                       improve_now_need_money       ; // 立即完成功能消耗元宝数量
	int                       improve_min_times            ; // 强化成功所需强化次数min
	int                       improve_rate                 ; // 服务器端计算用成功几率
	int                       improve_max_times            ; // 强化成功所需强化次数max
	std::map<int, int>        improve_failed_reward        ; // 强化失败给予物品序列（格式：几率万分比分子|物品ID_数量;几率万分比分子|物品ID_数量）
	int                       improve_failed_down          ; // 强化失败降星颗数
	int                       improve_broadcast            ; // 强化成功全服公告内容（对应广播表id）
	std::map<int, int>        improve_update_attr          ; // 强化成功增加基本属性序列
	int                       can_upgrade                  ; // 是否允许进阶（0不允许，1允许）
	std::map<int, int>		  upgrade_need_material        ; // 进阶消耗材料序列（格式：材料ID_数量;材料ID_数量;）
	int                       upgrade_need_game_money      ; // 进阶消耗铜钱
	int                       upgrade_min_times            ; // 进阶成功所需操作次数min
	int                       upgrade_rate                 ; // 服务器端计算用进阶成功几率
	int						  upgrade_max_times			   ; // 进阶成功所需操作次数max
	int						  upgrade_target_equip         ; // 进阶至装备ID
	int                       upgraded_improve_level	   ; // 进阶后装备强化等级
	int                       upgrade_broadcast			   ; // 进阶成功是否全服公告（对应广播表id）
	std::vector<int> 		  upgrade_replace_item		   ; // 升级可使用代币列表(itemid_itemid;itemid_itemid)
	std::vector<int>         improve_replace_item		   ; // 强化可使用代币列表(itemid_itemid;itemid_itemid)
	std::map<int, int> 		  improve_update_attr_1        ; // 强化成功增加基本属性序列【橙色】
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
