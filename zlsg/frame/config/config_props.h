#ifndef __CONFIG_PROPS_H__
#define __CONFIG_PROPS_H__

#include "cfg_cache_mgr.h"
#include "cfg_parser.h"
#include "config_base.h"

enum PROPS_BIND_TYPE {
    PBT_NOT_BIND = 0,
    PBT_BIND     = 1,
    PBT_USE_BIND = 2,
};

enum PROPS_TYPE {
    PT_MONEY		= 0,// 金钱	
    PT_NORMAL		= 1,// 普通物品
    PT_EQUIP		= 2,// 角色装备
    PT_PET_EQUIP    = 3,// 美人装备
    PT_REEL			= 4,// 传送卷轴
    PT_USE			= 5,// 使用道具
    PT_MEDICINE		= 6,// 药水
	PT_COMPOUND		= 7,// 合成
};

enum PROPS_SUB_TYPE {
    PST_WEAPON   = 1,   // 武器
    PST_ARMOR    = 2,   // 衣服
	PST_BELT     = 3,   // 腰带
	PST_HELMET   = 4,   // 头盔
	PST_NECKLACE = 5,   // 项链
	PST_SHOE     = 6,   // 鞋子
};
enum PICK_UP_TYPE {
	PUT_GOLD      = 1, // 自动拾取金币
	PUT_MEDICINE  = 2, // 自动拾取药品
	PUT_TASK      = 3, // 自动拾取任务物品
	PUT_EQUIP     = 4, // 自动拾取装备
	PUT_MATERIAL  = 5, // 自动拾取材料
	PUT_SHOP_ITEM = 6, // 自动拾取商城道具
	PUT_OTHER     = 7, // 其他物品
};

class config_props
{
public:
    config_props();
    ~config_props();

public:
    inline int                             get_item_id         () const { return _item_id         ; } 
    inline string                          get_item_name       () const { return _item_name       ; } 
    inline int                             get_item_type       () const { return _item_type       ; } 
    inline int                             get_item_sub_type   () const { return _item_sub_type   ; } 
    inline int                             get_bind_type       () const { return _bind_type       ; } 
    inline int                             get_is_get_use      () const { return _is_get_use      ; } 
    inline int                             get_pick_range      () const { return _pick_range      ; } 
    inline int                             get_total           () const { return _total           ; } 
    inline int                             get_need_job        () const { return _need_job        ; } 
    inline int                             get_need_level      () const { return _need_level      ; } 
    inline int                             get_cool_group_id   () const { return _cool_group_id   ; } 
    inline int                             get_cool_group_tick () const { return _cool_group_tick ; } 
    inline int                             get_buy_price       () const { return _buy_price       ; } 
    inline int                             get_is_shop_recover () const { return _is_shop_recover ; } 
    inline int                             get_sales_price     () const { return _sales_price     ; } 
    inline int                             get_is_shop_confirm () const { return _is_shop_confirm ; } 
    inline int                             get_can_repair      () const { return _can_repair      ; } 
    inline int                             get_repair_price    () const { return _repair_price    ; } 
    inline int                             get_can_discard     () const { return _can_discard     ; } 
    inline int                             get_discard_confirm () const { return _discard_confirm ; } 
    inline int                             get_can_inlay       () const { return _can_inlay       ; } 
    inline int                             get_can_synthesis   () const { return _can_synthesis   ; } 
    inline int                             get_durable_max     () const { return _durable_max     ; } 
    inline int                             get_increase_max    () const { return _increase_max    ; } 
    inline const std::map<int, int>&       get_game_attribute  () const { return _game_attribute  ; } 
    inline const std::map<int, cpf_val_3>& get_random_attribute() const { return _random_attribute; } 
    inline int                             get_affect_hp       () const { return _affect_hp       ; } 
    inline int                             get_affect_mp       () const { return _affect_mp       ; } 
    inline int                             get_can_shortcut    () const { return _can_shortcut    ; } 
    inline int                             get_name_color_index() const { return _name_color_index; } 
    inline string                          get_describes       () const { return _describes       ; } 
    inline int                             get_pickup_type     () const { return _pickup_type     ; } 
    inline int                             get_lotuser_count   () const { return _lotuser_count   ; }
	inline const std::vector<int>&         get_trigger_list	() const { return _trigger_list    ; }
	inline int                             get_exceed_limit    () const { return _exceed_limit    ; }
	inline const std::map<int, succinct_info>&   get_succinct_need_material  () const { return _succinct_need_material  ; } 
	inline int                             get_smelting_coefficient    () const { return _smelting_coefficient    ; }
	inline int                             get_basic_upgrade			() const { return _basic_upgrade		   ; }
	inline int                             get_upgrade_cost			() const { return _upgrade_cost            ; }
	inline int                             get_start_upgrade_cost		() const { return _start_upgrade_cost      ; }
public:
    inline bool is_equip() const { return get_item_type() == PT_EQUIP; }
	inline bool is_pet_equip() const { return get_item_type() == PT_PET_EQUIP;}
    inline bool is_lnlaid_stone() const { return false; }
    inline uint32 get_quality() const { return get_name_color_index(); }
public:
    int                       _item_id         ; // 物品ID
    string                    _item_name       ; // 道具名字
    int                       _item_type       ; // 道具类型
    int                       _item_sub_type   ; // 道具子类型
    int                       _bind_type       ; // 绑定类型（0不绑定，1拾取绑定，2,使用后绑定）
    int                       _is_get_use      ; // 获得时是否立即使用(0,1)
    int                       _pick_range      ; // 拾取/触发范围(最大7)格
    int                       _total           ; // 最大叠加
    int                       _need_job        ; // 需求职业
    int                       _need_level      ; // 需求等级
    int                       _cool_group_id   ; // 公共冷却组（1~64）
    int                       _cool_group_tick ; // 公共冷却时间（单位：毫秒）
    int                       _buy_price       ; // 商店中的购买价格
    int                       _is_shop_recover ; // 商店是否回收(0,1)
    int                       _sales_price     ; // 商店回收价格
    int                       _is_shop_confirm ; // 回收时是否弹出确认（0否，1是）
    int                       _can_repair      ; // 是否能修理（0否，1是）
    int                       _repair_price    ; // 修理价格
    int                       _can_discard     ; // 是否可丢弃（0否，1,是）
    int                       _discard_confirm ; // 丢弃是否弹出确认
    int                       _can_inlay       ; // 是否是镶嵌物品（0否，1是）
    int                       _can_synthesis   ; // 能否合成(0=否，1=是)
    int                       _durable_max     ; // 最大耐久
    int                       _increase_max    ; // 最大可强化等级
    std::map<int, int>        _game_attribute  ; // 游戏属性列表（类型|值/Ratio<Max|Job(0通用)）
    std::map<int, cpf_val_3>  _random_attribute; // 随机附加属性(类性|几率,最小/最大;)
    int                       _affect_hp       ; // 影响生命(可填写固定数值及比例)
    int                       _affect_mp       ; // 影响魔法(可填写固定数值及比例)
    int                       _can_shortcut    ; // 是否可拖入快捷栏
    int                       _name_color_index; // 物品名字颜色索引（0蓝色，1无色，2紫色，3绿色，4橙色）
    string                    _describes       ; // 物品描述信息
    int                       _pickup_type     ; // 道具拾取类型（拾取物品类型1、自动拾取金币；2、自动拾取药品；3、自动拾取任务物品；4、自动拾取装备；5、自动拾取材料；6、自动拾取商城道具；7=其他物品）
    int                       _lotuser_count   ; // 批量使用最大数
	std::vector<int>		  _trigger_list	   ; // 物品效果ID列表
	int						  _exceed_limit	   ; // 超出上限是否允许使用（是1；否2）
	std::map<int, succinct_info>	  _succinct_need_material; // 洗练消耗材料序列（格式：材料ID|数量;材料ID_数量;）
	int					      _smelting_coefficient; // 装备熔炼系数(该值已算成百分比）
	int                       _basic_upgrade	;  // 装备成长值
	int                       _upgrade_cost     ;  // 强化消耗基数
	int						  _start_upgrade_cost; // 升星消耗基数
};

class config_props_mgr : public config_base_mgr
{
public:
    config_props_mgr(TTableGroup_s* p = NULL);
    ~config_props_mgr();

public:
    int                 init(TTableGroup_s* p);
    int                 clear();

public:
    const TTable*       get_table() const { return _table; }
    const config_props* get_config_props(uint32 props_id) const;
	const string get_prop_name(uint32 prop_id) const ;

private:
    TTableGroup_s*                     _props;
    TTable*                          _table;
    std::map<uint32, config_props*>  _cp;
};

#endif // __CONFIG_PROPS_H__
