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
    int                       legion_level         ; // 军团等级
	int						  upgrade_need_lv	   ; // 创建、升级所需人物等级
	int						  member_limit		   ; // 可容纳成员上限
	int						  create_need_money	   ; // 创建消耗的铜钱
	int						  upgrade_need_money   ; //	升级消耗铜钱
	int						  create_need_silver   ; // 创建消耗元宝
	int						  upgrade_need_silver  ; // 升级消耗元宝
	std::map<int,int>		  upgrade_need_item	   ; // 创建、升级消耗物品及数量（物品|数量;物品|数量;）
	int						  add_experience	   ; // 为成员增提供杀怪经验加成（百分比）
	std::map<int,int>		  add_property		   ; // 为成员增加的属性（属性|增加值；属性|增加值；）
	std::map<int,int>		  donation_1		   ; // 捐赠类型1 （青龙令_数量_获得帮贡值）
	std::map<int,int>         donation_2		   ; // 捐赠类型2（朱雀令_数量_获得帮贡值）
	std::map<int,int>         donation_3		   ; // 捐赠类型3（玄武令_数量_获得帮贡值）
	std::map<int,int>         donation_4		   ; // 捐赠类型4（白虎令_数量_获得帮贡值）
	std::map<int,int>		  donation_5		   ; // 捐赠类型5铜钱（数量1|获得帮贡值；数量2|获得帮贡值；）
	int                       change_flag_pay      ; // 更换战旗外形消耗铜钱
	std::map<int,int>         change_flag_item	   ; // 更换战旗外形消耗道具（物品ID|数量；物品ID|数量；）
	int                       change_flag_name_pay ; // 更改战旗名字消耗铜钱
	std::map<int,int>         change_flag_name_item; // 更换战旗外形消耗道具（物品ID|数量；物品ID|数量；）
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
