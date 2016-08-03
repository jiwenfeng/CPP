#ifndef __CLIENT_PB_MGR_H__
#define __CLIENT_PB_MGR_H__

#include <iostream>
#include <sstream>
#include <string>
#include <map>
#include <vector>
#include <google/protobuf/descriptor.h>

#include "define.h"
#include "hash_map_obj.h"

// 客户端交互协议PB头文件
// #include "client.create_role.pb.h"
// #include "client.role_attr_msg.pb.h"
// #include "client.role_pos_msg.pb.h"
// #include "client.role_name_msg.pb.h"
// #include "client.scene_ready.pb.h"
// #include "client.role_move_msg.pb.h"
// #include "client.attack.pb.h"
// #include "client.role_status.pb.h"
// #include "client.role_remove_msg.pb.h"
// #include "client.role_skill_msg.pb.h"
// #include "client.offline.pb.h"
// #include "client.trans_user.pb.h"
// #include "client.role_props.pb.h"
// #include "client.jump_scene.pb.h"
// #include "client.role_login.pb.h"
// #include "client.terrain.pb.h"
// #include "client.raid_operate.pb.h"
// #include "client.effect.pb.h"
// #include "client.auth.pb.h"
// #include "client.mounts.pb.h"
// #include "client.heart.pb.h"
// #include "client.task.pb.h"
// #include "client.promo.pb.h"
// #include "client.pets.pb.h"
// #include "client.role_potential.pb.h"
// #include "client.pick.pb.h"
// #include "client.chat_msg.pb.h"
// #include "client.practice.pb.h"
// #include "client.family.pb.h"
// #include "client.city.pb.h"
// #include "client.region.pb.h"
// #include "client.role_cd_cooldown.pb.h"
#include "client.poll_activity.pb.h"
#include "client.role_gem.pb.h"
#ifdef __LIB_MINI_SERVER__
#include "client.miniserver.pb.h"
#endif // __LIB_MINI_SERVER__

// 以下废弃 有用到再拿上去
#include "client.operate_mail.pb.h"
#include "client.operate_shop.pb.h"
#include "client.operate_pet.pb.h"
#include "client.battle.pb.h"
#include "client.world_boss.pb.h"
#include "client.query.pb.h"
#include "client.friend.pb.h"
//#include "client.rank.pb.h"
#include "client.role_label.pb.h"
#include "client.team.pb.h"
#include "client.role_vip.pb.h"
#include "client.role_tower_raid.pb.h"
#include "client.secret_place.pb.h"
#include "client.team_raid.pb.h"
#include "client.role_statis.pb.h"
#include "client.role_set.pb.h"
#include "client.welfare.pb.h"
#include "client.invest.pb.h"
#include "client.flower.pb.h"
#include "client.spa.pb.h"
#include "client.arena.pb.h"
#include "client.field_score.pb.h"
#include "client.mate.pb.h"
#include "client.randnpc.pb.h"
#include "client.time.pb.h"
#include "client.farm.pb.h"
#include "client.promo.pb.h"
#define DEFAULT_CLIENT_PB_BUCKET_NUM 500

class client_pb_mgr
{
public:
    client_pb_mgr();
    virtual ~client_pb_mgr();

protected:
    int init();

public:
    const char* get_dst_name(const char* name) const;

public:
    void insert(const char* type_name, const char* dst_name);
    bool is_script(const char* type_name);

protected:
    hash_map_obj<std::string, std::string, DEFAULT_CLIENT_PB_BUCKET_NUM> cpbs;
    std::map<std::string, std::string> script;
};

extern client_pb_mgr* CLIENT_PB_MGR;

#endif
