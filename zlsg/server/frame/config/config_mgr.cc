#include <time.h>

#include "base_time.h"
#include "pb_mgr.h"

#include "config_raid.h"
#include "config_birth.h"
#include "config_upgrade.h"
#include "config_npc.h"
#include "config_skill.h"
#include "config_map.h"
#include "config_effect.h"
#include "config_props.h"
#include "config_upequip.h"
#include "config_melting_equip.h"
#include "config_cast_equip.h"
#include "config_mounts.h"
#include "config_quest.h"
#include "config_mounts_skill.h"
#include "config_potential.h"
#include "config_beauty.h"
#include "config_gift.h"
#include "config_legion.h"
#ifndef __LIB_MINI_SERVER__
#include "config_siege.h"
#include "config_edreward.h"
#include "config_flag.h"
#include "config_robot.h"
#endif
#include "config_fallitem.h"
#include "config_mall.h"
#include "config_basic.h"
#include "config_mount_exp.h"
#include "config_magic.h"
#include "config_treasure.h"
#include "config_material.h"
#include "data_role.h"
#include "config_mgr.h"

#ifndef __LIB_MINI_SERVER__
#include "config_target.h"
#include "config_vip.h"
#include "config_pay.h"
#include "config_recharge.h"
#include "config_lottery.h"
#include "config_historyReward.h"
#include "config_name.h"
#endif

#include "config_equip_upgrade.h"
#include "config_cd_time.h"
#include "config_sign.h"
#include "config_giant.h"
#include "config_giantraid.h"
#include "config_open.h"
#include "config_pet_skill.h"
#include "config_mail.h"
#include "config_bipize.h"
#include "config_land.h"
#include "config_activity.h"
#include "config_activity_item.h"
#include "config_gem.h"
#include "config_bow_exp.h"
#include "config_bows.h"
#include "config_gem_act.h"
#ifndef __LIB_MINI_SERVER__
#include "config_robot_ai.h"
#include "config_cycle.h"
#include "config_devil.h"
#endif

config_mgr::config_mgr() : _ccm(NULL)
{
    lock = 0;
    bok = 0;
    gdata_max_role_head = NULL;

    _ccm = NULL;

    gdata_max_role_head = NULL;
    all_user_smy_head = NULL;
    offline_user_head = NULL;
    mail_center_head = NULL;
    role_friends_head = NULL;
    rank_center_head = NULL;
    raid_center_head = NULL;
    family_head = NULL;
    family_member_head = NULL;
    family_log_head = NULL;
    family_record_head = NULL;
    family_skill_head = NULL;
    family_domain_head = NULL;
	family_diplomacy_head = NULL;
    siege_warfare_head = NULL;
    gamesvr_head = NULL;
    promo_list_of_names_head = NULL; 
    center_arena_head = NULL;
    arena_record_head = NULL;
	arena_robot_head = NULL;
	center_rank_head = NULL;
    mate_head = NULL;
    farm_head = NULL;
    farm_set_head = NULL;
	city_attack_info = NULL;
	city_welfare_info = NULL;
	activity_open_head = NULL;
	activity_record_head = NULL;
	center_cycle_head = NULL;
	center_cycle_role_head = NULL;

    birth         = new config_birth_mgr        ;
    upgrade       = new config_upgrade_mgr      ;
    npc           = new config_npc_mgr          ;
    skill         = new config_skill_mgr        ;
    effect        = new config_effect_mgr       ;
    map           = new config_map_mgr          ;
    raid          = new config_raid_mgr         ;
    props         = new config_props_mgr        ;
    upequip       = new config_upequip_mgr      ;
    mounts        = new config_mounts_mgr       ;
    melting_equip = new config_melting_equip_mgr;
    cast_equip    = new config_cast_equip_mgr   ;
    quests        = new config_quest_mgr        ;
    mounts_skill  = new config_mounts_skill_mgr ;
    potentials    = new config_potential_mgr    ;
    beautys       = new config_beauty_mgr       ;
    gifts         = new config_gift_mgr         ;
    legions       = new config_legion_mgr       ;
#ifndef __LIB_MINI_SERVER__
    siege         = new config_siege_mgr        ;
	edreward	  = new config_edreward_mgr		;
    robot         = new config_robot_mgr        ;
#endif
    packet      = new config_packet_mgr     ;
    malls         = new config_mall_mgr         ;
    basic         = new config_basic_mgr        ;
    mountexp      = new config_mount_exp_mgr    ;
    magices       = new config_magic_mgr        ;
    treasures     = new config_treasure_mgr     ;
    material      = new config_material_mgr		;
#ifndef __LIB_MINI_SERVER__
    flag          = new config_flag_mgr         ;
	target		  = new config_target_mgr		;
    history       = new config_history_mgr      ;
	vip			  = new config_vip_mgr			;
	pay			  = new config_pay_mgr			;
	recharge	  = new config_recharge_mgr		;
	lottery		  = new config_lottery_mgr		;
	name		  = new config_name_mgr			;
#endif
    equip_upgrade = new config_equip_upgrade_mgr;
	cd_time       = new config_cd_time_mgr		;
	signs         = new config_sign_mgr		    ;
	giant		  = new config_giant_mgr		;
	giantraid	  = new config_giantraid_mgr	;
	pet_skill_mgr = new config_pet_skill_mgr	;
	mail_mgr      = new config_mail_mgr		    ;
	bipize_mgr    = new config_bipize_mgr		;
	land_mgr	  = new config_land_mgr			;
	activity_mgr  = new config_activity_mgr	    ;
	act_item_mgr  = new config_activity_item_mgr;
	gem_mgr       = new config_gem_mgr		    ;
	bow_exp_mgr   = new config_bow_exp_mgr		;
	bows_mgr      = new config_bows_mgr			;
	gem_act_mgr	  = new config_gem_act_mgr		;
#ifndef __LIB_MINI_SERVER__
	robot_ai_mgr  = new config_robot_ai_mgr		;
	cycle_mgr	  = new config_cycle_mgr		;
	devil_mgr	  = new config_devil_mgr		; 
#endif

    _mgr["config_birth_mgr"         ] = birth        ;
    _mgr["config_upgrade_mgr"       ] = upgrade      ;
    _mgr["config_npc_mgr"           ] = npc          ;
    _mgr["config_skill_mgr"         ] = skill        ;
    _mgr["config_effect_mgr"        ] = effect       ;
    _mgr["config_map_mgr"           ] = map          ;
    _mgr["config_raid_mgr"          ] = raid         ;
    _mgr["config_props_mgr"         ] = props        ;
    _mgr["config_upequip_mgr"       ] = upequip      ;
    _mgr["config_mounts_mgr"        ] = mounts       ;
    _mgr["config_melting_equip_mgr" ] = melting_equip;
    _mgr["config_cast_equip_mgr"    ] = cast_equip   ;
    _mgr["config_quest_mgr"         ] = quests       ;
    _mgr["config_mounts_skill_mgr"  ] = mounts_skill ;
    _mgr["config_potential_mgr"     ] = potentials   ;
    _mgr["config_beauty_mgr"        ] = beautys      ;
    _mgr["config_gift_mgr"          ] = gifts        ;
    _mgr["config_legion_mgr"        ] = legions      ;
    _mgr["config_packet_mgr"		] = packet		 ;
    _mgr["config_mall_mgr"          ] = malls        ;
    _mgr["config_basic_mgr"         ] = basic        ;
    _mgr["config_mount_exp_mgr"     ] = mountexp     ;
    _mgr["config_magic_mgr"         ] = magices      ;
    _mgr["config_treasure_mgr"      ] = treasures    ;
	_mgr["config_material_mgr"      ] = material     ;
#ifndef __LIB_MINI_SERVER__
    _mgr["config_robot_mgr"         ] = robot        ;
    _mgr["config_edreward_mgr"      ] = edreward     ;
    _mgr["config_siege_mgr"         ] = siege        ;
    _mgr["config_flag_mgr"          ] = flag         ;
	_mgr["config_target_mgr"		] = target		 ;
    _mgr["config_history_mgr"       ] = history      ;
	_mgr["config_vip_mgr"			] = vip			 ;
	_mgr["config_pay_mgr"			] = pay			 ;
	_mgr["config_recharge_mgr"		] = recharge	 ;
	_mgr["config_lottery_mgr"		] = lottery		 ;
	_mgr["config_name_mgr"			] = name         ;
#endif
	_mgr["config_equip_upgrade_mgr" ] = equip_upgrade;
	_mgr["config_cd_time_mgr"	    ] = cd_time		 ;
	_mgr["config_sign_mgr"	        ] = signs		 ;
	_mgr["config_giant_mgr"			] = giant		 ;
	_mgr["config_giantraid_mgr"		] = giantraid	 ;
    _mgr["config_open_mgr"          ] = new config_open_mgr;
	_mgr["config_pet_skill_mgr"		] = pet_skill_mgr;
	_mgr["config_mail_mgr"		    ] = mail_mgr     ;
	_mgr["config_bipize_mgr"		] = bipize_mgr   ;
	_mgr["config_land_mgr"		    ] = land_mgr     ;
	_mgr["config_activity_mgr"	    ] = activity_mgr ;
	_mgr["config_activity_item_mgr"	] = act_item_mgr ;
	_mgr["config_gem_mgr"	        ] = gem_mgr		 ;
	_mgr["config_bow_exp_mgr"	    ] = bow_exp_mgr	 ;
	_mgr["config_bows_mgr"			] = bows_mgr	 ;
	_mgr["config_gem_act_mgr"		] = gem_act_mgr	 ;
#ifndef __LIB_MINI_SERVER__
	_mgr["config_robot_ai_mgr"		] = robot_ai_mgr ;
	_mgr["config_cycle_mgr"			] = cycle_mgr	 ;
	_mgr["config_devil_mgr"			] = devil_mgr	 ;
#endif
}

config_mgr::~config_mgr()
{
    for (std::map<std::string, config_base_mgr*>::iterator i = _mgr.begin(); i != _mgr.end(); ++i) {
        delete i->second;
    }

    _mgr.clear();
}

int config_mgr::init(cfg_cache_mgr* ccm)
{
    _ccm = ccm;

    init_dot_bin();

//     int i = 0;
//     i = 
    init();

    return 0;
}

const hash_map_obj<CACHE_KEY, cfg_cache_obj, CFG_CACHE_BULB_NUM>::pair_t* config_mgr::get_cfg(const char*cfg_name)
{
    cfg_cache_ctx* ctx = _ccm->get_cache_ctx(cfg_name);
    if (ctx == NULL) {
        return NULL;
    }

    return ctx->get_obj_head();
}

bool config_mgr::ready()
{ 
    bool ret;
    __sync_lock_test_and_set(&lock,1);{}
    ret = (bok == 1);
    __sync_lock_release(&lock);
    return ret;
}

cfg_cache_ctx* config_mgr::init_ctx(const char* pb_name)
{
    cfg_cache_ctx* ctx = _ccm->get_cache_ctx(pb_name);
    return ctx;
}

const hash_map_obj<CACHE_KEY, cfg_cache_obj, CFG_CACHE_BULB_NUM>::pair_t* config_mgr::init_obj(const char* pb_name)
{
    cfg_cache_ctx* ctx = _ccm->get_cache_ctx(pb_name);
    if (ctx == NULL) {
        return NULL;
    }
    
    return ctx->get_obj_head();
}

int config_mgr::init()
{
    int ret = 0;
//     const hash_map_obj<CACHE_KEY, cfg_cache_obj, CFG_CACHE_BULB_NUM>::pair_t* head = NULL;
    do {
//         if ((head = init_obj("mysql.sp_get_gdata")) != NULL) {
//             const cfg_cache_obj& cco = head->get_val();
//             global_data = static_cast<const mysql::sp_get_gdata*>((cco.get_msg()));
//         } 

        if ((gdata_max_role_head = init_obj("mysql.sp_get_gdata_max_role")) == NULL) {
            --ret;
            break;
        }

        if ((all_user_smy_head = init_obj("mysql.sp_load_all_user_smy")) == NULL) {
            //--ret;
            //break;
        }

        if ((offline_user_head = init_obj("mysql.sp_load_offline_user")) == NULL) {
            //--ret;
            //break;
        }
        
        arena_record_head    = init_obj("mysql.tb_arena_record"   );
		arena_robot_head	 = init_obj("mysql.tb_arena_robot"	  );
        center_arena_head    = init_obj("mysql.tb_center_arena"   );
		center_rank_head	 = init_obj("mysql.tb_rank"			  );
        family_head          = init_obj("mysql.tb_family"         );
        family_domain_head   = init_obj("mysql.tb_family_domain"  );
        family_log_head      = init_obj("mysql.tb_family_log"     );
        family_member_head   = init_obj("mysql.tb_family_member"  );
        family_record_head   = init_obj("mysql.tb_family_record"  );
        family_skill_head    = init_obj("mysql.tb_family_skill"   );
		family_diplomacy_head= init_obj("mysql.tb_family_diplomacy"   );
        gamesvr_head         = init_obj("mysql.tb_gamesvr"        );
        mail_center_head     = init_obj("mysql.tb_mail_center"    );
        mate_head            = init_obj("mysql.tb_mate"           );
        raid_center_head     = init_obj("mysql.tb_raid_center"    );
        rank_center_head     = init_obj("mysql.tb_rank_center"    );
        farm_head            = init_obj("mysql.tb_role_farm"      );
        role_friends_head    = init_obj("mysql.tb_role_friends"   );
        siege_warfare_head   = init_obj("mysql.tb_siege_warfare"  );
        city_attack_info     = init_obj("mysql.tb_city_attack"    );
        city_welfare_info    = init_obj("mysql.tb_city_welfare"   );
		activity_record_head = init_obj("mysql.tb_activity_record"  );
		promotions_head		 = init_obj("mysql.tb_promotions"     );
		activity_open_head   = init_obj("mysql.tb_activity_open"  );
		center_cycle_head	 = init_obj("mysql.tb_center_cycle"	  );
		center_cycle_role_head = init_obj("mysql.tb_center_cycle_role");
		center_devil_head	 = init_obj("mysql.tb_center_devil"		);
		center_notice_head	 = init_obj("mysql.tb_center_notice"	);

    } while (false);

    if (ret != 0) {
        return ret;
    }

    __sync_lock_test_and_set(&lock,1);{}
    bok = 1;
    printf("set bok == 1\n");
    __sync_lock_release(&lock);

    return ret;
}

void config_mgr::init_map()
{
    std::map<std::string, MapConfig_s*>* m = _ccm->get_cache_map();
    map->init(m);
}

void config_mgr::init_dot_bin()
{
    EPAPSED_TIME;
    init_map();
    std::map<std::string, TTableGroup_s*>* p = _ccm->get_cache_cfg();
    std::map<std::string, TTableGroup_s*>::iterator i = p->end();

    i = p->find("birth");
    if (i != p->end()) {
        birth->init(i->second);
    } else {
        printf("not found TTableGroup_s birth\n");
    }

    i = p->find("upgrade");
    if (i != p->end()) {
        upgrade->init(i->second);
    } else {
        printf("not found TTableGroup_s upgrade\n");
    }

    i = p->find("npc_drop");
    if (i != p->end()) {
        npc->init_drop(i->second);
    } else {
        printf("not found TTableGroup_s npc_drop\n");
    }

    i = p->find("monster");
    if (i != p->end()) {
        npc->init(i->second);
    } else {
        printf("not found TTableGroup_s monster\n");
    }

//#define __QUICK_INIT_SKILL__
#ifdef __LIB_MINI_SERVER__
    std::map<std::string, TTableGroup_s*>::iterator table_skill = p->find("skill");
    std::map<std::string, TTableGroup_s*>::iterator table_role_skill = p->find("skill_role");
    if (table_skill != p->end() && table_role_skill != p->end()) {
        skill->quick_init(table_skill->second, table_role_skill->second);
    } else {
        printf("%s:%d not found TTableGroup_s skill or skill_role\n", __FILE__, __LINE__);
    }
#else
    i = p->find("skill");
    if (i != p->end()) {
        skill->init(i->second, "t_skill");
    } else {
        printf("not found TTableGroup_s skill\n");
    }

    i = p->find("skill_role");
    if(i != p->end()) {
        skill->init(i->second, "t_skill_role");
    } else {
        printf("not found TTableGroup_s skill_role\n");
    }
#endif // __QUICK_INIT_SKILL__

//#define __QUICK_INIT_EFFECT__
#ifdef __LIB_MINI_SERVER__
    std::map<std::string, TTableGroup_s*>::iterator table_effect = p->find("effect");
    std::map<std::string, TTableGroup_s*>::iterator table_role_effect = p->find("effect_role");
    if (table_effect != p->end() && table_role_effect != p->end()) {
        effect->quick_init(table_effect->second, table_role_effect->second);
    } else {
        printf("%s:%d not found TTableGroup_s effect or effect_role\n", __FILE__, __LINE__);
    }
#else
    i = p->find("effect");
    if (i != p->end()) {
        effect->init(i->second, "t_effect");
    } else {
        printf("not found TTableGroup_s effect\n");
    }

    i = p->find("effect_role");
    if(i != p->end()) {
        effect->init(i->second, "t_effect_role");
    } else {
        printf("not found TTableGroup_s effect_role\n");
    }

    skill->link_effects(effect);
#endif // __QUICK_INIT_EFFECT__

    i = p->find("scene");
    if (i != p->end()) {
        raid->init(i->second);
    } else {
        printf("not found TTableGroup_s scene\n");
    }

    i = p->find("item");
    if (i != p->end()) {
        props->init(i->second);
    } else {
        printf("not found TTableGroup_s item\n");
    }

    i = p->find("strengthen");
    if (i != p->end()) {
        upequip->init(i->second);
    } else {
        printf("not found TTableGroup_s strengthen\n");
    }

    i = p->find("mount");
    std::map<std::string, TTableGroup_s*>::iterator j = p->find("mount_attr");
    if (i != p->end() && j != p->end()) {
        mounts->init(i->second, j->second);
    } else {
        printf("not found TTableGroup_s mount or mount_attr\n");
    }

    // 熔炼表加载
    i = p->find("melting");
    if (i != p->end()) {
        melting_equip->init(i->second);
    } else {
        printf("not found TTableGroup_s melting\n");
    }

    // 铸造表加载
    i = p->find("cast");
    if (i != p->end()) {
        cast_equip->init(i->second);
    } else {
        printf("not found TTableGroup_s melting\n");
    }

    i = p->find("mainquest");
    if (i != p->end()) {
        quests->init(i->second);
    } else {
        printf("not found TTableGroup_s mainquest\n");
    }

	i = p->find("mount_skill");
	if (i != p->end()) {
		mounts_skill->init(i->second);
	} else {
		printf("not found TTableGroup_s mount_skill\n");
	}

	i = p->find("potential");
	if (i != p->end()) {
		potentials->init(i->second);
	} else {
		printf("not found TTableGroup_s potentials\n");
	}

	i = p->find("general");
	if (i != p->end()) {
		beautys->init(i->second);
	} else {
		printf("not found TTableGroup_s beautys\n");
	}

	i = p->find("pack");
	if (i != p->end()) {
		gifts->init(i->second);
	} else {
		printf("not found TTableGroup_s gifts\n");
	}

	i = p->find("legion");
	if (i != p->end()) {
		legions->init(i->second);
	} else {
		printf("not found TTableGroup_s legions\n");
	}
#ifndef __LIB_MINI_SERVER__
	i = p->find("capital");
	if(i != p->end()){
		siege->init(i->second);
	} else {
		printf("not found TTableGroup_s capital\n");
	}
	i = p->find("everydayreward");
	if(i != p->end()) {
		edreward->init(i->second);
	} else {
		printf("not found TTableGroup_s everydayreward\n");
	}
#endif
	i = p->find("fallteam");
	if(i != p->end()) {
		packet->init(i->second);
	} else {
		printf("not found TTableGroup_s fallteam\n");
	}

#ifndef __LIB_MINI_SERVER__
	i = p->find("flag");
	if(i != p->end()) {
		flag->init(i->second);
	} else {
		printf("not found TTableGroup_s flag\n");
	}
#endif
	i = p->find("mall");
	if(i != p->end()) {
		malls->init(i->second);
	} else {
		printf("not found TTableGroup_s mall\n");
	}

	i = p->find("basic");
	if(i != p->end()) {
		basic->init(i->second);
	} else {
		printf("not found TTableGroup_s basic\n");
	}

#ifndef __LIB_MINI_SERVER__
	i = p->find("robot");
	if(i != p->end()) {
		robot->init(i->second);
	} else {
		printf("not found TTableGroup_s robot\n");
	}
#endif

	i = p->find("mountexp");
	if(i != p->end()) {
		mountexp->init(i->second);
	} else {
		printf("not found TTableGroup_s mountexp\n");
	}

	i = p->find("magic");
	if(i != p->end()) {
		magices->init(i->second);
	} else {
		printf("not found TTableGroup_s magic\n");
	}
#ifndef __LIB_MINI_SERVER__
	i = p->find("historyReward");
	if(i != p->end()) {
		history->init(i->second);
	} else {
		printf("not found TTableGroup_s historyReward\n");
	}
#endif
	// 黑市表
	i = p->find("BlackShop");
	if(i != p->end()) {
		treasures->init(i->second);
	} else {
		printf("not found TTableGroup_s BlackShop\n");
	}
#ifndef __LIB_MINI_SERVER__
	i = p->find("target");
	if(i != p->end()) {
		target->init(i->second);
	} else {
		printf("not found TTableGroup_s target\n");
	}
#endif

	i = p->find("material");
	if(i != p->end()) {
		material->init(i->second);
	} else {
		printf("not found TTableGroup_s material\n");
	}

	i = p->find("equipupgrade");
	if(i != p->end()) {
		equip_upgrade->init(i->second);
	} else {
		printf("not found TTableGroup_s equipupgrade\n");
	}
#ifndef __LIB_MINI_SERVER__
	i = p->find("privilege");
	if(i != p->end()) {
		vip->init(i->second);
	} else {
		printf("not found TTableGroup_s privilege\n");
	}

	i = p->find("payfunction");
	if(i != p->end()) {
		pay->init(i->second);
	} else {
		printf("not found TTableGroup_s payfunction\n");
	}

	i = p->find("recharge");
	if(i != p->end()) {
		recharge->init(i->second);
	} else {
		printf("not found TTableGroup_s recharge\n");
	}

	i = p->find("treasurebox");
	if(i != p->end()) {
		lottery->init(i->second);
	} else {
		printf("not found TTableGroup_s treasurebox\n");
	}
#endif
	i = p->find("itemcd");
	if(i != p->end()) {
		cd_time->init(i->second);
	} else {
		printf("not found TTableGroup_s itemcd\n");
	}

	i = p->find("sign");
	if(i != p->end()) {
		signs->init(i->second);
	} else {
		printf("not found TTableGroup_s sign\n");
	}

	// 过关斩将
	i = p->find("giant");
	if(i != p->end()) {
		giant->init(i->second);
	} else {
		printf("not found TTableGroup_s giant\n");
	}
	i = p->find("giantraid");
	if(i != p->end()) {
		giantraid->init(i->second);
	} else {
		printf("not found TTableGroup_s giant\n");
	}

    i = p->find("funcAvailable");
    if (i != p->end()) {
        _mgr["config_open_mgr"]->init(i->second);
    } else {
        printf("not found TTableGroup_s funcAvailable\n");
    }
#ifndef __LIB_MINI_SERVER__
	i = p->find("randomName");
	if(i != p->end()) {
		name->init(i->second);
	} else {
		printf("not found TTableGroup_s randomName\n");
	}
#endif
	i = p->find("rgeneral");
	if(i != p->end()) {
		pet_skill_mgr->init(i->second);
	} else {
		printf("not found TTableGroup_s rgeneral\n");
	}

	i = p->find("mail");
	if(i != p->end()) {
		mail_mgr->init(i->second);
	} else {
		printf("not found TTableGroup_s mail\n");
	}

	i = p->find("equipfluctuate");
	if(i != p->end()) {
		bipize_mgr->init(i->second);
	} else {
		printf("not found TTableGroup_s equipfluctuate\n");
	}

	i = p->find("land");
	if(i != p->end()) {
		land_mgr->init(i->second);
	} else {
		printf("not found TTableGroup_s land\n");
	}

	i = p->find("activity");
	if(i != p->end()) {
		activity_mgr->init(i->second);
	} else {
		printf("not found TTableGroup_s activity\n");
	}

	i = p->find("activity_item");
	if(i != p->end()) {
		act_item_mgr->init(i->second);
	} else {
		printf("not found TTableGroup_s activity_item\n");
	}

	i = p->find("gem");
	if(i != p->end()) {
		gem_mgr->init(i->second);
	} else {
		printf("not found TTableGroup_s gem\n");
	}

	i = p->find("bowexp");
	if(i != p->end()) {
		bow_exp_mgr->init(i->second);
	} else {
		printf("not found TTableGroup_s bowexp\n");
	}

	i = p->find("bow");
	std::map<std::string, TTableGroup_s*>::iterator k = p->find("bow_attr");
	if (i != p->end() && k != p->end()) {
		bows_mgr->init(i->second, k->second);
	} else {
		printf("not found TTableGroup_s bow or bow_attr\n");
	}
#ifndef __LIB_MINI_SERVER__
	i = p->find("robotAI");
	if(i != p->end()) {
		robot_ai_mgr->init(i->second);
	} else {
		printf("not found TTableGroup_s robot_ai_mgr\n");
	}
#endif

	i = p->find("gemactivation");
	if(i != p->end()) {
		gem_act_mgr->init(i->second);
	} else {
		printf("not found TTableGroup_s gemactivation\n");
	}

#ifndef __LIB_MINI_SERVER__
	i = p->find("scgiant");
	if( i != p->end()) {
		cycle_mgr->init(i->second);
	} else {
		printf("not found TTableGroup_s scgiant\n");
	}
	i = p->find("mhgiant");
	if( i != p->end()) {
		devil_mgr->init(i->second);
	} else {
		printf("not found TTableGroup_s mhgiant\n");
	}
#endif
}

void config_mgr::reload()
{
    birth->clear();
    upgrade->clear();
    npc->clear();
    skill->clear();
    effect->clear();
    raid->clear();
    props->clear();
    upequip->clear();
    mounts->clear();
    melting_equip->clear();
    cast_equip->clear();
    quests->clear();
    mounts_skill->clear();
    potentials->clear();
    beautys->clear();
    init_dot_bin();
}

const config_birth* config_mgr::get_birth(uint32 job)
{
    return birth->get_config_birth(job);
}

const config_upgrade* config_mgr::get_config_upgrade(uint32 job, uint32 level)
{
    return upgrade->get_config_upgrade(job, level);
}


uint32 config_mgr::get_config_city_war_exp(uint32 job, uint32 lvl)
{
	return upgrade->get_config_city_war_exp(job, lvl);
}

const config_skill* config_mgr::get_config_skill(uint32 skill_id)
{
    return skill->get_config_skill(skill_id);
}

const config_effect* config_mgr::get_config_effect(uint32 effect_id)
{
    return effect->get_config_effect(effect_id);
}

const config_map* config_mgr::get_config_map(uint32 map_id) const
{
    return map->get_config_map(map_id);
}

const config_map* config_mgr::get_config_map_default() const
{
    return get_config_map(10100);
}

const config_raid* config_mgr::get_config_raid(uint32 raid_id)
{
    return raid->get_config_raid(raid_id);
}

const config_props* config_mgr::get_config_props(uint32 props_id)
{
    return props->get_config_props(props_id);
}

const config_upequip* config_mgr::get_config_upequip(uint32 equip_id,int strengthen_level)
{
	return upequip->get_config_upequip(equip_id,strengthen_level);
}

const config_melting_equip* config_mgr::get_config_melting_equip(uint32 equip_id,int improve_level)
{
	return melting_equip->get_config_melting_equip(equip_id,improve_level);
}

const config_cast_equip* config_mgr::get_config_cast_equip(uint32 equip_id)
{
	return cast_equip->get_config_cast_equip(equip_id);
}

const config_mounts* config_mgr::get_config_mounts(uint32 mounts_id)
{
    return mounts->get_config_mounts(mounts_id);
}

const config_mounts* config_mgr::get_config_mounts_min()
{
    return mounts->get_config_mounts_min();
}

const config_mounts* config_mgr::get_config_mounts_max()
{
	return mounts->get_config_mounts_max();
}

const config_mounts_skill* config_mgr::get_config_mounts_skill(uint32 skill_id)
{
	return mounts_skill->get_config_mounts_skill(skill_id);
}

const config_mounts_skill* config_mgr::get_config_mounts_money_pay(uint32 activation_need_advance)
{
	return mounts_skill->get_config_mounts_money_pay(activation_need_advance);
}

const config_potential* config_mgr::get_config_potential(uint32 potential_id)
{
	return potentials->get_config_potential(potential_id);
}

const config_beauty* config_mgr::get_config_beauty(uint32 general_id,uint32 general_level)
{
	return beautys->get_config_beauty(general_id,general_level);
}

const config_beauty* config_mgr::get_config_beauty_by_lingering_count(uint32 general_id,uint32 lingering_count)
{
	return beautys->get_config_beauty_by_lingering_count(general_id,lingering_count);
}

const config_gift* config_mgr::get_config_gift(uint32 gift_id)
{
	return gifts->get_config_gift(gift_id);
}

const config_legion* config_mgr::get_config_legion(uint32 legion_level)
{
	return legions->get_config_legion(legion_level);
}

#ifndef __LIB_MINI_SERVER__
const std::map<int, config_siege *>& config_mgr::get_config_siege_welfare_list()const
{
	return siege->get_welfare_list();
}

const uint32 config_mgr::get_config_flag_id(int id) const
{
	const config_flag *f = flag->get_flag(id);
	if(NULL == f)
	{
		return 0;
	}
	return f->npc_id;
}
#endif

const config_mall* config_mgr::get_config_mall(uint32 mall_id,uint32 item_id)
{
	return malls->get_config_mall(mall_id,item_id);
}

int config_mgr::get_basic_value(int id)const
{
	return basic->get_basic_value(id);
}

#ifndef __LIB_MINI_SERVER__
const config_robot * config_mgr::get_config_robot(uint32 id) const 
{ 
    return robot->get_config_robot(id);
}

const std::map<uint32, config_robot *>& config_mgr::get_config_robots() const
{
	return robot->get_config_robots();
}
#endif
const config_mount_exp* config_mgr::get_config_mount_exp(int mount_level)
{
	return mountexp->get_config_mount_exp(mount_level);
}

const config_magic*	config_mgr::get_config_magic(uint32 magic_id,uint32 stage_star)
{
	return magices->get_config_magic(magic_id,stage_star);
}

const config_treasure*	config_mgr::get_config_treasure(uint32 key_id)
{
	return treasures->get_config_treasure(key_id);
}

const std::map<uint32, config_treasure*>& config_mgr::get_config_treasure_map()
{
	return treasures->get_config_treasure_map();
}

#ifndef __LIB_MINI_SERVER__
const config_edreward * config_mgr::get_edreward(int i) const
{
	return edreward->get_edreward(i); 
}

const std::vector<config_edreward *> &config_mgr::get_all_edreward() const 
{
	return edreward->get_all_edreward();
}

const vector<config_target *> &config_mgr::get_sub_targets(uint32 subtype) const
{
	return target->get_sub_targets(subtype);
}

const map<uint32, config_target *> &config_mgr::get_target_list() const
{
	return target->get_target_list();
}

const config_target * config_mgr::get_config_target(int id) const
{
	return target->get_config_target(id);
}
#endif

const config_material*  config_mgr::get_config_material(uint32 compound_id)
{
	return material->get_config_material(compound_id);
}

const config_equip_upgrade*  config_mgr::get_config_equip_upgrade(uint32 upgrade_level)
{
	return equip_upgrade->get_config_equip_upgrade(upgrade_level);
}
const config_cd_time*   config_mgr::get_config_cd_time(uint32 type_id)
{
	return cd_time->get_config_cd_time(type_id);
}

#ifndef __LIB_MINI_SERVER__
const config_vip * config_mgr::get_config_vip(uint32 i)const
{
	return vip->get_config_vip(i);
}

const std::vector<config_vip *> & config_mgr::get_vip_list() const
{
	return vip->get_vip_list();
}

const config_pay *config_mgr::get_config_pay(uint32 type, uint32 num)const
{
	return pay->get_config_pay(type, num);
}

const config_recharge * config_mgr::get_config_recharge(uint32 i) const
{
	return recharge->get_config_recharge(i);
}

const uint32 config_mgr::get_min_recharge() const
{
	return recharge->get_min_recharge();
}

const config_recharge * config_mgr::get_config_recharge_by_n(uint32 n) const
{
	return recharge->get_config_recharge_by_n(n);
}

const std::map<uint32, config_recharge *>& config_mgr::get_config_recharge_list() const
{
	return recharge->get_config_recharge_list();
}

const std::vector<uint32> &config_mgr::get_config_lottery_list(uint32 type) const
{
	return lottery->get_config_lottery_list(type);
}

const config_lottery *config_mgr::get_config_lottery(uint32 id) const
{
	return lottery->get_config_lottery(id);
}
#endif

const drop_content * config_mgr::get_config_packet(int i) const
{
	return packet->get_config_packet(i);
}

const config_sign* config_mgr::get_config_sign(uint32 sign_id)
{
	return signs->get_config_sign(sign_id);
}

const config_npc *config_mgr::get_config_giant(uint32 id) const
{
	return giant->get_config_giant(id);
}

void config_mgr::get_giant_unlock_group(uint32 lvl, vector<uint32> &v)
{
	return giant->get_giant_unlock_group(lvl, v);
}

uint32 config_mgr::get_giant_id(uint32 group_id, uint32 lv)
{
	return giant->get_giant_id(group_id, lv);
}

#ifndef __LIB_MINI_SERVER__
const vector<string>& config_mgr::get_config_first_name() const
{
	return name->get_first_name();
}

const vector<string>& config_mgr::get_config_female_second_name() const
{
	return name->get_female_second_name();
}

const vector<string>& config_mgr::get_config_male_second_name() const
{
	return name->get_male_second_name();
}
#endif

const config_pet_skill *config_mgr::get_config_pet_skill(uint32 id, uint32 level) const
{
	return pet_skill_mgr->get_config_pet_skill(id, level);
}

const config_mail *	config_mgr::get_config_mail(uint32 mail_id) const
{
	return mail_mgr->get_config_mail(mail_id);
}

const config_bipize * config_mgr::get_config_bipize(uint32 range_id) const
{
	return bipize_mgr->get_config_bipize(range_id);
}

const config_land * config_mgr::get_config_land(uint32 land_id) const
{
	return land_mgr->get_config_land(land_id);
}

const config_activity *	config_mgr::get_config_activity(uint32 activity_id) const
{
	return activity_mgr->get_config_activity(activity_id);
}

const std::map<uint32, config_activity*>& config_mgr::get_config_activity_list()
{
	return activity_mgr->get_config_activity_list();
}

const config_activity_item * config_mgr::get_config_activity_item(uint32 activity_type) const
{
	return act_item_mgr->get_config_activity_item(activity_type);
}

const config_gem *	config_mgr::get_config_gem(uint32 gem_type,uint32 gem_level) const
{
	return gem_mgr->get_config_gem(gem_type,gem_level);
}

const config_bow_exp *	config_mgr::get_config_bow_exp(uint32 bow_level) const
{
	return bow_exp_mgr->get_config_bow_exp(bow_level);
}

const config_bows* config_mgr::get_config_bows(uint32 bow_id)
{
	return bows_mgr->get_config_bows(bow_id);
}

const config_bows* config_mgr::get_config_bows_min()
{
	return bows_mgr->get_config_bows_min();
}

const config_bows* config_mgr::get_config_bows_max()
{
	return bows_mgr->get_config_bows_max();
}
#ifndef __LIB_MINI_SERVER__
const config_robot_ai *config_mgr::get_config_robot_ai(uint32 id) const
{
	return robot_ai_mgr->get_config_robot_ai(id);
}

const config_robot_ai *config_mgr::random_config_robot_ai(uint32 type) const
{
	return robot_ai_mgr->random_config_robot_ai(type);
}
#endif

const config_drop *config_mgr::get_config_drop(uint32 id) const
{
	return npc->get_config_drop(id);
}

#ifndef __LIB_MINI_SERVER__
const config_cycle *config_mgr::get_config_cycle(uint32 id) const
{
	return cycle_mgr->get_config_cycle(id);
}

const uint32 config_mgr::get_max_cycle_level() const
{
	return cycle_mgr->get_max_cycle_level();
}

const uint32 config_mgr::get_start_level(uint32 diff) const
{
	return cycle_mgr->get_start_level(diff);
}
#endif


void config_mgr::get_config_giantraid(uint32 level, vector<config_giantraid *> &v)
{
	giantraid->get_config_giantraid(level, v);
}

const config_giantraid *config_mgr::get_config_giantraid(uint32 id) const
{
	return giantraid->get_config_giantraid(id);
}

#ifndef __LIB_MINI_SERVER__
const config_devil *config_mgr::get_config_devil(uint32 id) const
{
	return devil_mgr->get_config_devil(id);
}

const vector<uint32> &config_mgr::get_all_maps_by_stage(uint32 id) const
{
	return devil_mgr->get_all_maps_by_stage(id);
}

const uint32 config_mgr::get_stage_reward(uint32 id) const
{
	return devil_mgr->get_stage_reward(id);
}

const string config_mgr::get_prop_name(uint32 prop_id) const
{
	return props->get_prop_name(prop_id);
}
#endif
