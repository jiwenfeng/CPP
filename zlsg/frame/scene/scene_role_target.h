#ifndef __SCENE_ROLE_TARGET_H__
#define __SCENE_ROLE_TARGET_H__

#include "scene_define.h"
#include "client.target.pb.h"
#include <stdarg.h>

class scene_user;
class config_target;

typedef int(*func_ptr)(scene_user *psu, uint32 type, ...);

class scene_role_target
{
public:
	static void init_target_func();
	static void *get_func_ptr(uint32 type);
private:
   static int check_user_upgrade_target(scene_user *psu, uint32 type, uint32 lv);					// 玩家升级
   static int check_raid_target(scene_user *psu, uint32 subtype, uint32 raid_id, uint32 star, uint32 type, uint32 num = 1);
   static int check_pet_num_target(scene_user *psu, uint32 type);						// 招募美人数量
   static int check_pet_upgrade_target(scene_user *psu, uint32 type, uint32 lv);			// 美人升级
   static int check_pet_linger_target(scene_user *psu, uint32 type, uint32 count);		// 美人缠绵
   static int check_mount_upgrade_target(scene_user *psu, uint32 type, uint32 lv);		// 坐骑升级
   static int check_mount_advance_target(scene_user *psu, uint32 type, uint32 id);		// 坐骑进阶
   static int check_pentation_target(scene_user *psu, uint32 type, uint32 id);			// 激活心法
   static int check_equip_star_target(scene_user *psu, uint32 type, uint32 n);			// 装备升星
   static int check_equip_xilian_target(scene_user *psu, uint32 type);					// 装备洗练
   static int check_user_skill_upgrade_target(scene_user *psu, uint32 type);			// 玩家技能升级
   static int check_mount_skill_upgrade_target(scene_user *psu, uint32 type);			// 坐骑技能升级
   static int check_treasure_target(scene_user *psu, uint32 type, uint32 n);				// 寻宝
   static int check_pet_skill_upgrade_target(scene_user *psu, uint32 type);				// 美人技能升级
   static int check_arena_challenge_target(scene_user *psu, uint32 type);				// 竞技场挑战
   static int check_family_donate_target(scene_user *psu, uint32 subtype, uint32 type);		// 军团捐献
   static int check_buy_copper_target(scene_user *psu, uint32 type);					// 购买铜钱
   static int check_get_power_target(scene_user *psu, uint32 type);						// 登录领取体力
   static int check_get_ingot_target(scene_user *psu, uint32 type);						// 领取元宝
   static int check_pet_raid_target(scene_user *psu, uint32 type, uint32 raid_id, uint32 num = 1);// 美人
   static int check_levels_raid_target(scene_user *psu, uint32 type, uint32 level);		// 过关斩
   static int check_team_raid_target(scene_user *psu, uint32 type, uint32 raid_id);		// 组队副
   static int check_smelt_equip_target(scene_user *psu, uint32 type);					// 熔炼装备
   static int check_user_skill_upgrade_star_target(scene_user *psu, uint32 type, uint32 id);
   static int check_buy_physical_power(scene_user *psu, uint32 type);
   static int check_shopping_target(scene_user *psu, uint32 subtype, uint32 type);
   static int check_cycle_mopup_target(scene_user *psu, uint32 type);
   static int check_devil_reset_target(scene_user *psu, uint32 type);
   static int check_gem_upgrade_target(scene_user *psu, uint32 type);
   static int check_gem_level_target(scene_user *psu, uint32 type, uint32 n);
   static int check_bow_level_target(scene_user *psu, uint32 type, uint32 n);

private:
   static int check_progress(scene_user *psu, uint32 type, const target_progress *tp);
   static bool check_target_cond(const config_target *ct, const target_progress *tp);
private:
	static std::map<uint32, void *> _func_ptrs;
};

#endif // __SCENE_ROLE_TARGET_H__
