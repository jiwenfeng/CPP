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
   static int check_user_upgrade_target(scene_user *psu, uint32 type, uint32 lv);					// �������
   static int check_raid_target(scene_user *psu, uint32 subtype, uint32 raid_id, uint32 star, uint32 type, uint32 num = 1);
   static int check_pet_num_target(scene_user *psu, uint32 type);						// ��ļ��������
   static int check_pet_upgrade_target(scene_user *psu, uint32 type, uint32 lv);			// ��������
   static int check_pet_linger_target(scene_user *psu, uint32 type, uint32 count);		// ���˲���
   static int check_mount_upgrade_target(scene_user *psu, uint32 type, uint32 lv);		// ��������
   static int check_mount_advance_target(scene_user *psu, uint32 type, uint32 id);		// �������
   static int check_pentation_target(scene_user *psu, uint32 type, uint32 id);			// �����ķ�
   static int check_equip_star_target(scene_user *psu, uint32 type, uint32 n);			// װ������
   static int check_equip_xilian_target(scene_user *psu, uint32 type);					// װ��ϴ��
   static int check_user_skill_upgrade_target(scene_user *psu, uint32 type);			// ��Ҽ�������
   static int check_mount_skill_upgrade_target(scene_user *psu, uint32 type);			// ���＼������
   static int check_treasure_target(scene_user *psu, uint32 type, uint32 n);				// Ѱ��
   static int check_pet_skill_upgrade_target(scene_user *psu, uint32 type);				// ���˼�������
   static int check_arena_challenge_target(scene_user *psu, uint32 type);				// ��������ս
   static int check_family_donate_target(scene_user *psu, uint32 subtype, uint32 type);		// ���ž���
   static int check_buy_copper_target(scene_user *psu, uint32 type);					// ����ͭǮ
   static int check_get_power_target(scene_user *psu, uint32 type);						// ��¼��ȡ����
   static int check_get_ingot_target(scene_user *psu, uint32 type);						// ��ȡԪ��
   static int check_pet_raid_target(scene_user *psu, uint32 type, uint32 raid_id, uint32 num = 1);// ����
   static int check_levels_raid_target(scene_user *psu, uint32 type, uint32 level);		// ����ն
   static int check_team_raid_target(scene_user *psu, uint32 type, uint32 raid_id);		// ��Ӹ�
   static int check_smelt_equip_target(scene_user *psu, uint32 type);					// ����װ��
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
