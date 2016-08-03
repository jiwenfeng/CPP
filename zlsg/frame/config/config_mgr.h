#ifndef __CONFIG_MGR_H__
#define __CONFIG_MGR_H__

#include <vector>
#include "hash_map_obj.h"
#include "cfg_cache_mgr.h"

class config_base_mgr;
class config_birth_mgr        ;
class config_upgrade_mgr      ;
class config_npc_mgr          ;
class config_skill_mgr        ;
class config_effect_mgr       ;
class config_map_mgr          ;
class config_raid_mgr         ;
class config_props_mgr        ;
class config_upequip_mgr      ;
class config_mounts_mgr       ;
class config_melting_equip_mgr;
class config_cast_equip_mgr   ;
class config_quest_mgr        ;
class config_mounts_skill_mgr ;
class config_potential_mgr    ;
class config_beauty_mgr       ;
class config_gift_mgr         ;
class config_legion_mgr       ;
#ifndef __LIB_MINI_SERVER__
class config_siege_mgr        ;
class config_flag_mgr         ;
class config_robot_mgr        ;
#endif
class config_packet_mgr     ;
class config_mall_mgr         ;
class config_basic_mgr        ;
class config_mount_exp_mgr    ;
class config_magic_mgr        ;
class config_treasure_mgr     ;
class config_skill_effect_mgr ;
class config_skill_role_mgr   ;
class config_edreward_mgr	  ;
class config_material_mgr	  ;
#ifndef __LIB_MINI_SERVER__
class config_target_mgr		  ;
class config_vip_mgr		  ;
class config_pay_mgr		  ;
class config_recharge_mgr	  ;
class config_lottery_mgr	  ;
class config_history_mgr      ;
class config_name_mgr		  ;
#endif
class config_equip_upgrade_mgr;
class config_cd_time_mgr	  ;
class config_sign_mgr		  ;
class config_giant_mgr		  ;
class config_giantraid_mgr	  ;
class config_pet_skill_mgr	  ;
class config_mail_mgr		  ;
class config_bipize_mgr		  ;
class config_land_mgr		  ;
class config_activity_mgr	  ;
class config_activity_item_mgr;
class config_gem_mgr		  ;
class config_bow_exp_mgr	  ;
class config_bows_mgr		  ;
class config_gem_act_mgr	  ;
#ifndef __LIB_MINI_SERVER__
class config_robot_ai_mgr	  ;
class config_cycle_mgr		  ;			// 沙场百战
class config_devil_mgr		  ;
class config_lottery	  ;
class config_vip		  ;
class config_pay		  ;
#endif

class config_raid   ;
class config_birth  ;
class config_upgrade;
class config_skill  ;
class config_effect ;
class config_map    ;
class config_props  ;
class config_upequip;
class config_mounts ;
class config_melting_equip;
class config_cast_equip   ;
class config_mounts_skill ;
class config_potential    ;
class config_beauty       ;
class config_gift         ;
class config_legion       ;
#ifndef __LIB_MINI_SERVER__
class config_siege        ;
class config_robot        ;
#endif
//class config_reward       ;
class config_mall         ;
class config_mount_exp    ;
class config_magic        ;
class config_treasure     ;
class config_skill_effect ;
class config_skill_role   ;
class config_edreward	  ;
class config_material	  ;
#ifndef __LIB_MINI_SERVER__
class config_target		  ;
class config_recharge	  ;
#endif
class config_equip_upgrade;
class config_cd_time	  ;
class config_sign		  ;
class config_npc		  ;
class config_giantraid	  ;
class config_pet_skill	  ;
class config_mail		  ;
class config_bipize		  ;
class config_land		  ;
class config_activity	  ;
class config_activity_item;
class config_gem		  ;
class config_bow_exp	  ;
class config_bows		  ;
class config_gem_act	  ;
class config_cycle		  ;			// 沙场百战
class config_drop		  ;
struct drop_content		  ;
#ifndef __LIB_MINI_SERVER__
class config_robot_ai	  ;
class config_devil		  ;
#endif

struct cfg_packet_content;

class config_mgr
{
public:
    config_mgr();
    virtual ~config_mgr();

public:
    int init(cfg_cache_mgr* ccm);

    const hash_map_obj<CACHE_KEY, cfg_cache_obj, CFG_CACHE_BULB_NUM>::pair_t* get_cfg(const char*cfg_name);

    bool ready();

    int init();

    const std::map<std::string, MapConfig_s*>* get_map_config() const;

    inline const hash_map_obj<CACHE_KEY, cfg_cache_obj, CFG_CACHE_BULB_NUM>::pair_t* get_gdata_max_role_head() const {return gdata_max_role_head;}
    inline const hash_map_obj<CACHE_KEY, cfg_cache_obj, CFG_CACHE_BULB_NUM>::pair_t* get_all_user_smy_head() const {return all_user_smy_head;};
    inline const hash_map_obj<CACHE_KEY, cfg_cache_obj, CFG_CACHE_BULB_NUM>::pair_t* get_offline_user_head() const {return offline_user_head;};
    inline const hash_map_obj<CACHE_KEY, cfg_cache_obj, CFG_CACHE_BULB_NUM>::pair_t* get_mail_center_head() const {return mail_center_head;};
    inline const hash_map_obj<CACHE_KEY, cfg_cache_obj, CFG_CACHE_BULB_NUM>::pair_t* get_role_friends_head() const {return role_friends_head;};
    inline const hash_map_obj<CACHE_KEY, cfg_cache_obj, CFG_CACHE_BULB_NUM>::pair_t* get_rank_center_head() const {return rank_center_head;};
    inline const hash_map_obj<CACHE_KEY, cfg_cache_obj, CFG_CACHE_BULB_NUM>::pair_t* get_raid_center_head() const {return raid_center_head;};

    inline const hash_map_obj<CACHE_KEY, cfg_cache_obj, CFG_CACHE_BULB_NUM>::pair_t* get_family_head()        const {return family_head;};
    inline const hash_map_obj<CACHE_KEY, cfg_cache_obj, CFG_CACHE_BULB_NUM>::pair_t* get_family_member_head() const {return family_member_head;};
    inline const hash_map_obj<CACHE_KEY, cfg_cache_obj, CFG_CACHE_BULB_NUM>::pair_t* get_family_log_head()    const {return family_log_head;};
    inline const hash_map_obj<CACHE_KEY, cfg_cache_obj, CFG_CACHE_BULB_NUM>::pair_t* get_family_record_head() const {return family_record_head;};
    inline const hash_map_obj<CACHE_KEY, cfg_cache_obj, CFG_CACHE_BULB_NUM>::pair_t* get_family_skill_head()  const {return family_skill_head;};
    inline const hash_map_obj<CACHE_KEY, cfg_cache_obj, CFG_CACHE_BULB_NUM>::pair_t* get_family_domain_head() const {return family_domain_head;};
	inline const hash_map_obj<CACHE_KEY, cfg_cache_obj, CFG_CACHE_BULB_NUM>::pair_t* get_family_diplomacy_head() const {return family_diplomacy_head;};
	
	inline const hash_map_obj<CACHE_KEY, cfg_cache_obj, CFG_CACHE_BULB_NUM>::pair_t* get_activity_open_head() const {return activity_open_head;};
	inline const hash_map_obj<CACHE_KEY, cfg_cache_obj, CFG_CACHE_BULB_NUM>::pair_t* get_activity_record_head() const {return activity_record_head;};
	inline const hash_map_obj<CACHE_KEY, cfg_cache_obj, CFG_CACHE_BULB_NUM>::pair_t* get_promotions_head() const {return promotions_head;};

    inline const hash_map_obj<CACHE_KEY, cfg_cache_obj, CFG_CACHE_BULB_NUM>::pair_t* get_siege_warfare_head() const {return siege_warfare_head;};

    inline const hash_map_obj<CACHE_KEY, cfg_cache_obj, CFG_CACHE_BULB_NUM>::pair_t* get_gamesvr_head() const {return gamesvr_head;}

    inline const hash_map_obj<CACHE_KEY, cfg_cache_obj, CFG_CACHE_BULB_NUM>::pair_t* get_promo_list_of_names_head() const {return promo_list_of_names_head;}

	inline const hash_map_obj<CACHE_KEY, cfg_cache_obj, CFG_CACHE_BULB_NUM>::pair_t* get_rank_head() { return center_rank_head; }
    inline const hash_map_obj<CACHE_KEY, cfg_cache_obj, CFG_CACHE_BULB_NUM>::pair_t* get_center_arena_head() {return center_arena_head;};
    inline const hash_map_obj<CACHE_KEY, cfg_cache_obj, CFG_CACHE_BULB_NUM>::pair_t* get_arena_record_head() {return arena_record_head;};
    inline const hash_map_obj<CACHE_KEY, cfg_cache_obj, CFG_CACHE_BULB_NUM>::pair_t *get_arena_robot() const {return arena_robot_head;}

    inline const hash_map_obj<CACHE_KEY, cfg_cache_obj, CFG_CACHE_BULB_NUM>::pair_t* get_city_attack_info()	 {return city_attack_info;};

    inline const hash_map_obj<CACHE_KEY, cfg_cache_obj, CFG_CACHE_BULB_NUM>::pair_t* get_city_welfare_info() {return city_welfare_info;};

    inline const hash_map_obj<CACHE_KEY, cfg_cache_obj, CFG_CACHE_BULB_NUM>::pair_t* get_mate_head() {return mate_head;};

    inline const hash_map_obj<CACHE_KEY, cfg_cache_obj, CFG_CACHE_BULB_NUM>::pair_t* get_farm_head() const {return farm_head;};
    inline const hash_map_obj<CACHE_KEY, cfg_cache_obj, CFG_CACHE_BULB_NUM>::pair_t* get_farm_set_head() const {return farm_set_head;}
    inline const hash_map_obj<CACHE_KEY, cfg_cache_obj, CFG_CACHE_BULB_NUM>::pair_t* get_center_cycle_head() const {return center_cycle_head;}
    inline const hash_map_obj<CACHE_KEY, cfg_cache_obj, CFG_CACHE_BULB_NUM>::pair_t* get_center_cycle_role_head() const {return center_cycle_role_head;}
    inline const hash_map_obj<CACHE_KEY, cfg_cache_obj, CFG_CACHE_BULB_NUM>::pair_t* get_center_devil_head() const {return center_devil_head;}
	inline const hash_map_obj<CACHE_KEY, cfg_cache_obj, CFG_CACHE_BULB_NUM>::pair_t* get_center_notice_head() const {return center_notice_head;}
	
    inline const std::map<std::string, TTableGroup_s*>* get_all_cfg() const { return _ccm->get_cache_cfg(); }

protected:
    cfg_cache_ctx* init_ctx(const char* pb_name);
    const hash_map_obj<CACHE_KEY, cfg_cache_obj, CFG_CACHE_BULB_NUM>::pair_t* init_obj(const char* pb_name);

private:
    cfg_cache_mgr* _ccm;

    // 全局属性相关读取到的值
    const hash_map_obj<CACHE_KEY, cfg_cache_obj, CFG_CACHE_BULB_NUM>::pair_t* gdata_max_role_head;
    const hash_map_obj<CACHE_KEY, cfg_cache_obj, CFG_CACHE_BULB_NUM>::pair_t* all_user_smy_head;
    const hash_map_obj<CACHE_KEY, cfg_cache_obj, CFG_CACHE_BULB_NUM>::pair_t* offline_user_head;

    // 邮件中心相关
    const hash_map_obj<CACHE_KEY, cfg_cache_obj, CFG_CACHE_BULB_NUM>::pair_t* mail_center_head;

    // 好友中心相关
    const hash_map_obj<CACHE_KEY, cfg_cache_obj, CFG_CACHE_BULB_NUM>::pair_t* role_friends_head;

    // 排行榜相关
    const hash_map_obj<CACHE_KEY, cfg_cache_obj, CFG_CACHE_BULB_NUM>::pair_t* rank_center_head;

    // 副本时间排行
    const hash_map_obj<CACHE_KEY, cfg_cache_obj, CFG_CACHE_BULB_NUM>::pair_t* raid_center_head;

    // 家族
    const hash_map_obj<CACHE_KEY, cfg_cache_obj, CFG_CACHE_BULB_NUM>::pair_t* family_head;
    const hash_map_obj<CACHE_KEY, cfg_cache_obj, CFG_CACHE_BULB_NUM>::pair_t* family_member_head;
    const hash_map_obj<CACHE_KEY, cfg_cache_obj, CFG_CACHE_BULB_NUM>::pair_t* family_log_head;
    const hash_map_obj<CACHE_KEY, cfg_cache_obj, CFG_CACHE_BULB_NUM>::pair_t* family_record_head;
    const hash_map_obj<CACHE_KEY, cfg_cache_obj, CFG_CACHE_BULB_NUM>::pair_t* family_skill_head;
    const hash_map_obj<CACHE_KEY, cfg_cache_obj, CFG_CACHE_BULB_NUM>::pair_t* family_domain_head;
	const hash_map_obj<CACHE_KEY, cfg_cache_obj, CFG_CACHE_BULB_NUM>::pair_t* family_diplomacy_head;

	const hash_map_obj<CACHE_KEY, cfg_cache_obj, CFG_CACHE_BULB_NUM>::pair_t* activity_record_head;
	const hash_map_obj<CACHE_KEY, cfg_cache_obj, CFG_CACHE_BULB_NUM>::pair_t* activity_open_head;
	const hash_map_obj<CACHE_KEY, cfg_cache_obj, CFG_CACHE_BULB_NUM>::pair_t* promotions_head;
	
	
    const hash_map_obj<CACHE_KEY, cfg_cache_obj, CFG_CACHE_BULB_NUM>::pair_t* siege_warfare_head;

    const hash_map_obj<CACHE_KEY, cfg_cache_obj, CFG_CACHE_BULB_NUM>::pair_t* gamesvr_head;

    const hash_map_obj<CACHE_KEY, cfg_cache_obj, CFG_CACHE_BULB_NUM>::pair_t* promo_list_of_names_head; 

    // 竞技场相关
    const hash_map_obj<CACHE_KEY, cfg_cache_obj, CFG_CACHE_BULB_NUM>::pair_t* center_arena_head;
	const hash_map_obj<CACHE_KEY, cfg_cache_obj, CFG_CACHE_BULB_NUM>::pair_t* center_rank_head;
    const hash_map_obj<CACHE_KEY, cfg_cache_obj, CFG_CACHE_BULB_NUM>::pair_t* arena_record_head;
    const hash_map_obj<CACHE_KEY, cfg_cache_obj, CFG_CACHE_BULB_NUM>::pair_t* arena_robot_head;

	// 沙场百战
    const hash_map_obj<CACHE_KEY, cfg_cache_obj, CFG_CACHE_BULB_NUM>::pair_t* center_cycle_head;
	const hash_map_obj<CACHE_KEY, cfg_cache_obj, CFG_CACHE_BULB_NUM>::pair_t* center_cycle_role_head;

	const hash_map_obj<CACHE_KEY, cfg_cache_obj, CFG_CACHE_BULB_NUM>::pair_t *center_devil_head;
	// 公告信息
	const hash_map_obj<CACHE_KEY, cfg_cache_obj, CFG_CACHE_BULB_NUM>::pair_t* center_notice_head;

    //攻城相关
    const hash_map_obj<CACHE_KEY, cfg_cache_obj, CFG_CACHE_BULB_NUM>::pair_t* city_attack_info;

    const hash_map_obj<CACHE_KEY, cfg_cache_obj, CFG_CACHE_BULB_NUM>::pair_t* city_welfare_info;

    // 修仙小伙伴相关
    const hash_map_obj<CACHE_KEY, cfg_cache_obj, CFG_CACHE_BULB_NUM>::pair_t* mate_head;

    // 仙园信息
    const hash_map_obj<CACHE_KEY, cfg_cache_obj, CFG_CACHE_BULB_NUM>::pair_t* farm_head;
    const hash_map_obj<CACHE_KEY, cfg_cache_obj, CFG_CACHE_BULB_NUM>::pair_t* farm_set_head;

    int lock;
    int bok;

public:
    void init_map();
    void init_dot_bin();
    void reload();

public: // 配置分段加载
    // 基础 场景 怪物 技能
    void load_base();
    // 功能 人物 效果 
    // 

public:
    template<class T>
    const T* get_mgr(const char* name) const {
        std::map<std::string, config_base_mgr*>::const_iterator i = _mgr.find(name);
        if (i == _mgr.end()) {
            return NULL;
        }

        return static_cast<const T*>(i->second);
    }

    template<class T>
    T* mutable_mgr(const char* name) {
        std::map<std::string, config_base_mgr*>::const_iterator i = _mgr.find(name);
        if (i == _mgr.end()) {
            return NULL;
        }

        return static_cast<T*>(i->second);
    }

public:
    const config_birth_mgr&              get_config_birth_mgr() const { return *birth; }
    const config_birth*                  get_birth(uint32 job);
    const config_upgrade_mgr&            get_config_upgrade_mgr() const { return *upgrade; }
    const config_upgrade*                get_config_upgrade(uint32 job, uint32 level);
	uint32								 get_config_city_war_exp(uint32 job, uint32 lvl);
    const config_npc_mgr&                get_config_npc_mgr() const { return *npc; }
    config_skill_mgr&                    get_config_skill_mgr() { return *skill; }
    const config_skill*                  get_config_skill(uint32 skill_id);
    config_effect_mgr&                   get_config_effect_mgr() { return *effect; }
    const config_effect*				 get_config_effect(uint32 effect_id);
    const config_map_mgr&				 get_config_map_mgr() const { return *map; }
    const config_map*					 get_config_map(uint32 map_id) const;
    const config_map*					 get_config_map_default() const;
    const config_raid_mgr&				 get_config_raid_mgr() const { return *raid; }
    const config_raid*					 get_config_raid(uint32 raid_id);
    const config_props_mgr&				 get_config_props_mgr() const { return *props; }
    const config_props*					 get_config_props(uint32 props_id);
    const config_upequip_mgr&			 get_config_upequip_mgr() const { return *upequip; }
    const config_upequip*				 get_config_upequip(uint32 equip_id,int strengthen_level);
    const config_mounts_mgr&			 get_config_mounts_mgr() const { return *mounts; }
    const config_mounts*				 get_config_mounts(uint32 mounts_id);
    const config_mounts*			     get_config_mounts_min();
    const config_mounts*                 get_config_mounts_max();
    const config_melting_equip_mgr&		 get_config_melting_equip_mgr() const { return *melting_equip;}
    const config_melting_equip*			 get_config_melting_equip(uint32 equip_id,int improve_level);
    const config_cast_equip_mgr&		 get_config_cast_equip_mgr() const { return *cast_equip;}
    const config_cast_equip*			 get_config_cast_equip(uint32 equip_id);
    const config_quest_mgr&              get_config_quest_mgr() const { return *quests; }
    const config_mounts_skill*			 get_config_mounts_skill(uint32 skill_id);
    const config_mounts_skill*           get_config_mounts_money_pay(uint32 activation_need_advance);
    const config_mounts_skill_mgr&       get_config_mounts_skill_mgr() const { return *mounts_skill; }
    const config_potential_mgr&          get_config_potential_mgr() const { return *potentials; }
    const config_potential*              get_config_potential(uint32 potential_id);
    const config_beauty_mgr&			 get_config_beauty_mgr() const { return *beautys;}
    const config_beauty*				 get_config_beauty(uint32 general_id,uint32 general_level);
	const config_beauty*				 get_config_beauty_by_lingering_count(uint32 general_id,uint32 lingering_count);
    const config_gift_mgr&				 get_config_gift_mgr() const { return *gifts;}
    const config_gift*					 get_config_gift(uint32 gift_id);
    const config_legion_mgr&			 get_config_legion_mgr() const { return *legions;}
    const config_legion*				 get_config_legion(uint32 legion_level);
#ifndef __LIB_MINI_SERVER__
	const string get_prop_name(uint32 prop_id) const;
    const config_siege_mgr&				 get_config_siege_mgr() const { return *siege;}
    const std::map<int, config_siege *>& get_config_siege_welfare_list()const;

	const config_target_mgr &			 get_config_target_mgr() const { return *target; }
	const std::map<uint32, config_target *>& get_target_list() const;
	const vector<config_target *> &get_sub_targets(uint32 subtype) const;
	const config_target *				 get_config_target(int id) const;
	const config_vip_mgr &				 get_config_vip_mgr() const { return *vip; }
	const config_vip *					 get_config_vip(uint32 i) const;
	const config_pay_mgr &				 get_config_pay_mgr() const { return *pay; }
	const config_pay *					 get_config_pay(uint32 type, uint32 num) const;
	const config_recharge_mgr &			 get_config_recharge_mgr() const { return *recharge; }
	const uint32	get_min_recharge() const;
	const config_recharge *				 get_config_recharge(uint32 i) const;
	const std::map<uint32, config_recharge *> & get_config_recharge_list() const;
	const config_recharge *				 get_config_recharge_by_n(uint32 n) const;
	const std::vector<config_vip *> &	 get_vip_list() const ;

	const config_edreward *				 get_edreward(int i) const ;
	const std::vector<config_edreward *> &get_all_edreward() const ;
#endif
    const config_packet_mgr &			 get_config_packet_mgr() const{ return *packet;}
	const drop_content *			 get_config_packet(int i) const;
#ifndef __LIB_MINI_SERVER__
    const config_flag_mgr &				 get_config_flag_mgr() const { return *flag ; }
#endif
    const uint32						 get_config_flag_id(int id) const;
    const config_mall_mgr&				 get_config_mall_mgr() const {return *malls;}
    const config_mall*					 get_config_mall(uint32 mall_id,uint32 item_id);

    const config_basic_mgr &			 get_config_basic_mgr() const {return *basic; }
    int									 get_basic_value(int id)const;
#ifndef __LIB_MINI_SERVER__
    const config_robot_mgr &			 get_config_robot_mgr() const { return *robot; }
    const config_robot *				 get_config_robot(uint32 id) const;
	
	const std::map<uint32, config_robot *>& get_config_robots() const;
#endif
    const config_mount_exp_mgr&			 get_mount_exp_mgr() const {return *mountexp;}
    const config_mount_exp*				 get_config_mount_exp(int mount_level);
    const config_magic_mgr&				 get_config_magic_mgr() const {return *magices;}
    const config_magic*					 get_config_magic(uint32 magic_id,uint32 stage_star);
    const config_treasure_mgr&                get_config_treasure_mgr() const {return *treasures;}
    const config_treasure*                    get_config_treasure(uint32 key_id);
    const std::map<uint32, config_treasure*>& get_config_treasure_map();
#ifndef __LIB_MINI_SERVER__
    const config_history_mgr &			 get_config_history_reward_mgr() const {return *history; }
	const std::vector<uint32>					 &get_config_lottery_list(uint32 type) const;
	const config_lottery					 *get_config_lottery(uint32 id) const;
#endif
	const config_material_mgr&                get_config_material_mgr() const { return *material;}
	const config_material*                    get_config_material(uint32 compound_id);
	const config_equip_upgrade_mgr&                get_config_equip_upgrade_mgr() const { return *equip_upgrade;}
	const config_equip_upgrade*                    get_config_equip_upgrade(uint32 upgrade_level);
	const config_cd_time_mgr&                get_config_cd_time_mgr() const { return *cd_time;}
	const config_cd_time*                    get_config_cd_time(uint32 type_id);
	const config_sign_mgr&                   get_config_sign_mgr() const { return *signs;}
	const config_sign*                       get_config_sign(uint32 sign_id);

	const config_npc *						 get_config_giant(uint32 id) const;
	void									 get_config_giantraid(uint32 level, vector<config_giantraid *> &v);
	const config_giantraid *				 get_config_giantraid(uint32 id) const;
	void									 get_giant_unlock_group(uint32 lvl, vector<uint32> &v);
	uint32									 get_giant_id(uint32 group, uint32 lv);

	const vector<string>&					 get_config_first_name() const ;
	const vector<string>&					 get_config_female_second_name() const;
	const vector<string>&					 get_config_male_second_name() const;
	const config_pet_skill *					 get_config_pet_skill(uint32 id, uint32 level) const;

	const config_mail_mgr &			         get_config_mail_mgr() const {return *mail_mgr;}
	const config_mail *				         get_config_mail(uint32 mail_id) const;

	const config_bipize_mgr &			     get_config_bipize_mgr() const {return *bipize_mgr;}
	const config_bipize *				     get_config_bipize(uint32 range_id) const;

	const config_land_mgr &			         get_config_land_mgr() const {return *land_mgr;}
	const config_land *				         get_config_land(uint32 land_id) const;

	const config_activity_mgr &			     get_config_activity_mgr() const {return *activity_mgr;}
	const config_activity *				     get_config_activity(uint32 activity_id) const;
	const std::map<uint32, config_activity*>& get_config_activity_list();

	const config_activity_item_mgr &	     get_config_activity_item_mgr() const {return *act_item_mgr;}
	const config_activity_item *			 get_config_activity_item(uint32 activity_type) const;

	const config_gem_mgr &					 get_config_gem_mgr() const {return *gem_mgr;}
	const config_gem *						 get_config_gem(uint32 gem_type,uint32 gem_level) const;

	const config_bow_exp_mgr &					 get_config_bow_exp_mgr() const {return *bow_exp_mgr;}
	const config_bow_exp *						 get_config_bow_exp(uint32 bow_level) const;

	const config_bows_mgr&				 get_config_bows_mgr() const { return *bows_mgr; }
	const config_bows*					 get_config_bows(uint32 bow_id);
	const config_bows*					 get_config_bows_min();
	const config_bows*					 get_config_bows_max();
#ifndef __LIB_MINI_SERVER__
	const config_robot_ai *				 get_config_robot_ai(uint32 id) const;
	const config_robot_ai *				 random_config_robot_ai(uint32 type) const;
#endif
	const config_gem_act_mgr&			 get_config_gem_act_mgr() const { return *gem_act_mgr; }

	const config_drop *					 get_config_drop(uint32 id) const;
#ifndef __LIB_MINI_SERVER__
	const config_cycle *				 get_config_cycle(uint32 id) const;
	const uint32						 get_max_cycle_level() const;
	const uint32						 get_start_level(uint32 diff) const;

	const config_devil *get_config_devil(uint32 id) const;

	const vector<uint32> &get_all_maps_by_stage(uint32 id) const;
	const uint32 get_stage_reward(uint32 id) const;
#endif

private:
    std::map<std::string, config_base_mgr*> _mgr;

private:
    config_birth_mgr        * birth;
    config_upgrade_mgr      * upgrade;
    config_npc_mgr          * npc;
    config_skill_mgr        * skill;
    config_effect_mgr       * effect;
    config_map_mgr          * map;
    config_raid_mgr         * raid;
    config_props_mgr        * props;
    config_upequip_mgr      * upequip;
    config_mounts_mgr       * mounts;
    config_melting_equip_mgr* melting_equip;
    config_cast_equip_mgr   * cast_equip;
    config_quest_mgr        * quests;
    config_mounts_skill_mgr * mounts_skill;
    config_potential_mgr    * potentials;
    config_beauty_mgr       * beautys;
    config_gift_mgr			* gifts;
    config_legion_mgr		* legions;
#ifndef __LIB_MINI_SERVER__
    config_siege_mgr		* siege;
#endif
	config_edreward_mgr		* edreward;
    config_packet_mgr		* packet;
#ifndef __LIB_MINI_SERVER__
    config_flag_mgr			* flag;
    config_robot_mgr		* robot;
#endif
    config_mall_mgr			* malls;
    config_basic_mgr		* basic;
    config_mount_exp_mgr	* mountexp;
    config_magic_mgr		* magices;
    config_treasure_mgr		* treasures;
	config_material_mgr     * material;
#ifndef __LIB_MINI_SERVER__
	config_target_mgr		* target;
    config_history_mgr		* history;	// 竞技场最高历史记录奖励
	config_vip_mgr			* vip;
	config_pay_mgr			* pay;
	config_recharge_mgr		* recharge;
	config_lottery_mgr		* lottery;
	config_name_mgr			* name;
#endif
	config_equip_upgrade_mgr* equip_upgrade;
	config_cd_time_mgr      * cd_time;
	config_sign_mgr         * signs;
	config_giant_mgr		* giant;
	config_giantraid_mgr	* giantraid;
	config_pet_skill_mgr	* pet_skill_mgr;
	config_mail_mgr         * mail_mgr;
	config_bipize_mgr	    * bipize_mgr;
	config_land_mgr			* land_mgr;
	config_activity_mgr     * activity_mgr;
	config_activity_item_mgr* act_item_mgr;
	config_gem_mgr          * gem_mgr;
	config_bow_exp_mgr      * bow_exp_mgr;
	config_bows_mgr		    * bows_mgr;
	config_gem_act_mgr		* gem_act_mgr;
#ifndef __LIB_MINI_SERVER__
	config_robot_ai_mgr		* robot_ai_mgr;
	config_cycle_mgr		* cycle_mgr;
	config_devil_mgr		* devil_mgr;
#endif
};

extern config_mgr* CONFMGR;

#define GET_CONFIG_MGR(CLASS_NAME) CONFMGR->get_mgr<CLASS_NAME>(#CLASS_NAME)
#define MUTABLE_CONFIG_MGR(CLASS_NAME) CONFMGR->mutable_mgr<CLASS_NAME>(#CLASS_NAME)

#endif
