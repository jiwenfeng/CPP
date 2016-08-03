#ifndef __PB_CODER_BASE_AUTO_H__
#define __PB_CODER_BASE_AUTO_H__

#include <iostream>
#include <sstream>
#include <string>
#include <map>
#include <vector>
#include <google/protobuf/descriptor.h>
#include "define.h"

//------------cfg pb code------------------------

//------------data pb code------------------------
#include "mysql.tb_role_mail.pb.h"
#include "mysql.tb_role_pet.pb.h"
#include "mysql.tb_role_task.pb.h"
#include "mysql.tb_role_task_chain.pb.h"
#include "mysql.tb_role_label.pb.h"
#include "mysql.tb_role_center_set.pb.h"
#include "mysql.tb_role_explore.pb.h"
#include "mysql.tb_role_explore_detail.pb.h"
#include "mysql.tb_role_statis.pb.h"
#include "mysql.tb_role_set.pb.h"
#include "mysql.tb_role_order.pb.h"
#include "mysql.tb_role_ext_attr.pb.h"
#include "mysql.tb_player.pb.h"
#include "mysql.tb_player_role.pb.h"
#include "mysql.tb_role_status.pb.h"
#include "mysql.tb_role_skill.pb.h"
#include "mysql.tb_role_buffer.pb.h"
#include "mysql.tb_role_props.pb.h"
#include "mysql.tb_role_raid.pb.h"
#include "mysql.tb_role_auto_raid.pb.h"
#include "mysql.tb_role_mounts.pb.h"
#include "mysql.tb_role_potential.pb.h"
#include "mysql.tb_role_cd_cooldown.pb.h"
#include "mysql.tb_role_treasure.pb.h"
#include "mysql.tb_role_target.pb.h"
#include "mysql.tb_role_vip.pb.h"
#include "mysql.tb_role_lottery.pb.h"
#include "mysql.tb_role_black_shop.pb.h"
#include "mysql.tb_role_levels.pb.h"
#include "mysql.tb_role_gem.pb.h"
#include "mysql.tb_role_bows.pb.h"
#include "mysql.tb_role_cycle.pb.h"
#include "mysql.tb_role_devil.pb.h"


class pb_mgr_base
{
public:
    pb_mgr_base(){}
    ~pb_mgr_base(){}

    int base_init();


protected:
    map<string, pb_obj*> cfg_mappbs;
    map<string, pb_obj*> data_mappbs;

//------------cfg pb obj code------------------------

//------------data pb obj code------------------------
    mysql::tb_role_mail mpb_tb_role_mail;
    mysql::tb_role_pet mpb_tb_role_pet;
    mysql::tb_role_task mpb_tb_role_task;
    mysql::tb_role_task_chain mpb_tb_role_task_chain;
    mysql::tb_role_label mpb_tb_role_label;
    mysql::tb_role_center_set mpb_tb_role_center_set;
    mysql::tb_role_explore mpb_tb_role_explore;
    mysql::tb_role_explore_detail mpb_tb_role_explore_detail;
    mysql::tb_role_statis mpb_tb_role_statis;
    mysql::tb_role_set mpb_tb_role_set;
    mysql::tb_role_order mpb_tb_role_order;
    mysql::tb_role_ext_attr mpb_tb_role_ext_attr;
    mysql::tb_player mpb_tb_player;
    mysql::tb_player_role mpb_tb_player_role;
    mysql::tb_role_status mpb_tb_role_status;
    mysql::tb_role_skill mpb_tb_role_skill;
    mysql::tb_role_buffer mpb_tb_role_buffer;
    mysql::tb_role_props mpb_tb_role_props;
    mysql::tb_role_raid mpb_tb_role_raid;
    mysql::tb_role_auto_raid mpb_tb_role_auto_raid;
    mysql::tb_role_mounts mpb_tb_role_mounts;
    mysql::tb_role_potential mpb_tb_role_potential;
    mysql::tb_role_cd_cooldown mpb_tb_role_cd_cooldown;
    mysql::tb_role_treasure mpb_tb_role_treasure;
    mysql::tb_role_target mpb_tb_role_target;
    mysql::tb_role_vip mpb_tb_role_vip;
    mysql::tb_role_lottery mpb_tb_role_lottery;
    mysql::tb_role_black_shop mpb_tb_role_black_shop;
    mysql::tb_role_levels mpb_tb_role_levels;
    mysql::tb_role_gem mpb_tb_role_gem;
    mysql::tb_role_bows mpb_tb_role_bows;
    mysql::tb_role_cycle mpb_tb_role_cycle;
    mysql::tb_role_devil mpb_tb_role_devil;
};

#endif
//------------cfg pb code------------------------

//------------data pb code------------------------
