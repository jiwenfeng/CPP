#include "cache_mgr.h"
#include "pb_mgr.h"

using namespace std;

/////////////////////////////////////////////////////////////////////////////////

pb_mgr::pb_mgr()
{
    reg();
    create();
}

pb_mgr::~pb_mgr()
{
    destroy();
}

const char* pb_mgr::get_querysql(const string& type_name)
{
    map<string, pb_obj*>::const_iterator it = data_mappbs.find(type_name);
    if (it != data_mappbs.end()) {
        return it->second->query_sql.c_str();
    }

    it = cfg_mappbs.find(type_name);
    if (it != cfg_mappbs.end()) {
        return it->second->query_sql.c_str();
    }

    return NULL;
}

const pb_obj* pb_mgr::get_data_obj(const string& type_name)
{
    map<string, pb_obj*>::const_iterator it = data_mappbs.find(type_name);
     if (it == data_mappbs.end()) {
         return NULL;
     }

     return it->second;
}

void pb_mgr::create()
{

}

void pb_mgr::destroy()
{
    std::map<string, pb_obj*>::iterator it;
    for (it = cfg_mappbs.begin(); it != cfg_mappbs.end(); ++it) {
        std::map<string, pb_obj*>::iterator f = data_mappbs.find(it->first);
        if (f != data_mappbs.end() && f->second == it->second) {
            data_mappbs.erase(f);
        }

        if (it->second != NULL) {
            delete it->second;
        }
    }

    cfg_mappbs.clear();

    for (it = data_mappbs.begin(); it != data_mappbs.end(); ++it) {
        if (it->second != NULL) {
            delete it->second;
        }
    }

    data_mappbs.clear();
}

// 将PB对象注册到管理器
void pb_mgr::reg()
{
#ifdef __LIB_MINI_SERVER__
    return ;
#endif // __LIB_MINI_SERVER__
	pb_mgr_base::base_init();

    // 数据库用户数据pb
    /*obj = new pb_obj(&mpb_tb_player_role, "", "select role_id, system_id from tb_player_role where player_id=%s and system_id=%s;", "tb_player_role");
    data_mappbs.insert(pair<string, pb_obj*>(mpb_tb_player_role.GetTypeName(), obj));
    obj->add_key("player_id");
    obj->add_key("system_id");*/

    // mail_center一开始初始化时全量加载，所以放到配置项里,但回写时要用到用户数据，这里要交集map
    pb_obj* obj = new pb_obj(&mpb_tb_mail_center, "", "select * from tb_mail_center;", "tb_mail_center");
    cfg_mappbs.insert(pair<string, pb_obj*>(mpb_tb_mail_center.GetTypeName(), obj));
    data_mappbs.insert(pair<string, pb_obj*>(mpb_tb_mail_center.GetTypeName(), obj));
    obj->add_key("role_id");

   // obj = new pb_obj(&mpb_tb_rank_center, "", "select * from tb_rank_center;", "tb_rank_center");
   // cfg_mappbs.insert(pair<string, pb_obj*>(mpb_tb_rank_center.GetTypeName(), obj));
   // data_mappbs.insert(pair<string, pb_obj*>(mpb_tb_rank_center.GetTypeName(), obj));
   // obj->add_key("index");

    obj = new pb_obj(&mpb_tb_raid_center, "", "select * from tb_raid_center;", "tb_raid_center");
    cfg_mappbs.insert(pair<string, pb_obj*>(mpb_tb_raid_center.GetTypeName(), obj));
    data_mappbs.insert(pair<string, pb_obj*>(mpb_tb_raid_center.GetTypeName(), obj));
    obj->add_key("raid_id");
#ifndef __LIB_MINI_SERVER__
    obj = new pb_obj(&mpb_tb_center_arena, "", "select * from tb_center_arena order by rank DESC;", "tb_center_arena");
    cfg_mappbs.insert(pair<string, pb_obj*>(mpb_tb_center_arena.GetTypeName(), obj));
    data_mappbs.insert(pair<string, pb_obj*>(mpb_tb_center_arena.GetTypeName(), obj));
    obj->add_key("rank");

	obj = new pb_obj(&mpb_tb_rank, "", "select * from tb_rank;", "tb_rank");
	cfg_mappbs.insert(pair<string, pb_obj *>(mpb_tb_rank.GetTypeName(), obj));
	data_mappbs.insert(pair<string, pb_obj *>(mpb_tb_rank.GetTypeName(), obj));
	obj->add_key("rank_type");

	obj = new pb_obj(&mpb_tb_arena_robot, "", "select * from tb_arena_robot;", "tb_arena_robot");
	cfg_mappbs.insert(pair<string, pb_obj *>(mpb_tb_arena_robot.GetTypeName(), obj));
	data_mappbs.insert(pair<string, pb_obj *>(mpb_tb_arena_robot.GetTypeName(), obj));
	obj->add_key("id");

    obj = new pb_obj(&mpb_tb_arena_record, "", "select * from tb_arena_record;", "tb_arena_record");
    cfg_mappbs.insert(pair<string, pb_obj*>(mpb_tb_arena_record.GetTypeName(), obj));
    data_mappbs.insert(pair<string, pb_obj*>(mpb_tb_arena_record.GetTypeName(), obj));
    obj->add_key("role_id");
#endif
    obj = new pb_obj(&mpb_tb_mate, "", "select * from tb_mate;", "tb_mate");
    cfg_mappbs.insert(pair<string, pb_obj*>(mpb_tb_mate.GetTypeName(), obj));
    data_mappbs.insert(pair<string, pb_obj*>(mpb_tb_mate.GetTypeName(), obj));
    obj->add_key("role_id");

    obj = new pb_obj(&mpb_tb_role_farm, "", "select * from tb_role_farm;", "tb_role_farm");
    cfg_mappbs.insert(pair<string, pb_obj*>(mpb_tb_role_farm.GetTypeName(), obj));
    data_mappbs.insert(pair<string, pb_obj*>(mpb_tb_role_farm.GetTypeName(), obj));
    obj->add_key("role_id");

    obj = new pb_obj(&mpb_tb_family, "", "select * from tb_family;", "tb_family");
    cfg_mappbs.insert(pair<string, pb_obj*>(mpb_tb_family.GetTypeName(), obj));
    data_mappbs.insert(pair<string, pb_obj*>(mpb_tb_family.GetTypeName(), obj));
    obj->add_key("id");
    obj = new pb_obj(&mpb_tb_family_member, "", "select * from tb_family_member;", "tb_family_member");
    cfg_mappbs.insert(pair<string, pb_obj*>(mpb_tb_family_member.GetTypeName(), obj));
    data_mappbs.insert(pair<string, pb_obj*>(mpb_tb_family_member.GetTypeName(), obj));
    obj->add_key("family_id");
    obj->add_key("role_id");
	obj = new pb_obj(&mpb_tb_family_diplomacy, "", "select * from tb_family_diplomacy;", "tb_family_diplomacy");
	cfg_mappbs.insert(pair<string, pb_obj*>(mpb_tb_family_diplomacy.GetTypeName(), obj));
	data_mappbs.insert(pair<string, pb_obj*>(mpb_tb_family_diplomacy.GetTypeName(), obj));
	obj->add_key("family_id");
	obj->add_key("target_id");

	obj = new pb_obj(&mpb_tb_activity_open, "", "select * from tb_activity_open;", "tb_activity_open");
	cfg_mappbs.insert(pair<string, pb_obj*>(mpb_tb_activity_open.GetTypeName(), obj));
	data_mappbs.insert(pair<string, pb_obj*>(mpb_tb_activity_open.GetTypeName(), obj));
	obj->add_key("activity_id");

	obj = new pb_obj(&mpb_tb_activity_record, "", "select * from tb_activity_record;", "tb_activity_record");
	cfg_mappbs.insert(pair<string, pb_obj*>(mpb_tb_activity_record.GetTypeName(), obj));
	data_mappbs.insert(pair<string, pb_obj*>(mpb_tb_activity_record.GetTypeName(), obj));
	obj->add_key("activity_id");

    obj = new pb_obj(&mpb_tb_family_log, "", "select * from tb_family_log;", "tb_family_log");
    cfg_mappbs.insert(pair<string, pb_obj*>(mpb_tb_family_log.GetTypeName(), obj));
    data_mappbs.insert(pair<string, pb_obj*>(mpb_tb_family_log.GetTypeName(), obj));
    obj->add_key("id");
    obj->add_key("family_id");
    obj = new pb_obj(&mpb_tb_family_record, "", "select * from tb_family_record;", "tb_family_record");
    cfg_mappbs.insert(pair<string, pb_obj*>(mpb_tb_family_record.GetTypeName(), obj));
    data_mappbs.insert(pair<string, pb_obj*>(mpb_tb_family_record.GetTypeName(), obj));
    obj->add_key("id");
    obj = new pb_obj(&mpb_tb_family_skill, "", "select * from tb_family_skill;", "tb_family_skill");
    cfg_mappbs.insert(pair<string, pb_obj*>(mpb_tb_family_skill.GetTypeName(), obj));
    data_mappbs.insert(pair<string, pb_obj*>(mpb_tb_family_skill.GetTypeName(), obj));
    obj->add_key("family_id");
    obj = new pb_obj(&mpb_tb_family_domain, "", "select * from tb_family_domain;", "tb_family_domain");
    cfg_mappbs.insert(pair<string, pb_obj*>(mpb_tb_family_domain.GetTypeName(), obj));
    data_mappbs.insert(pair<string, pb_obj*>(mpb_tb_family_domain.GetTypeName(), obj));
    obj->add_key("scene_id");

    obj = new pb_obj(&mpb_tb_center_task_log, "", "", "tb_task_log");
    data_mappbs.insert(pair<string, pb_obj*>(mpb_tb_center_task_log.GetTypeName(), obj));
    obj->add_key("id");
    obj = new pb_obj(&mpb_tb_center_money_log, "", "", "tb_money_log");
    data_mappbs.insert(pair<string, pb_obj*>(mpb_tb_center_money_log.GetTypeName(), obj));
    obj->add_key("id");

//    obj = new pb_obj(&mpb_tb_siege_warfare, "", "select * from tb_siege_warfare;", "tb_siege_warfare");
//    cfg_mappbs.insert(pair<string, pb_obj*>(mpb_tb_siege_warfare.GetTypeName(), obj));
//    data_mappbs.insert(pair<string, pb_obj*>(mpb_tb_siege_warfare.GetTypeName(), obj));
//    obj->add_key("id");

    obj = new pb_obj(&mpb_tb_role_friends, "", "select * from tb_role_friends;", "tb_role_friends");
    cfg_mappbs.insert(pair<string, pb_obj*>(mpb_tb_role_friends.GetTypeName(), obj));
    data_mappbs.insert(pair<string, pb_obj*>(mpb_tb_role_friends.GetTypeName(), obj));
    obj->add_key("role_id");

   // obj = new pb_obj(&mpb_tb_promo_list_of_names, "", "select * from tb_promo_list_of_names;", "tb_promo_list_of_names");
   // cfg_mappbs.insert(pair<string, pb_obj*>(mpb_tb_promo_list_of_names.GetTypeName(), obj));
   // data_mappbs.insert(pair<string, pb_obj*>(mpb_tb_promo_list_of_names.GetTypeName(), obj));
   // obj->add_key("type");
   // obj->add_key("role_id");

    obj = new pb_obj(&mpb_tb_gamesvr, "", "select * from tb_gamesvr;", "tb_gamesvr");
    cfg_mappbs.insert(pair<string, pb_obj*>(mpb_tb_gamesvr.GetTypeName(), obj));
    data_mappbs.insert(pair<string, pb_obj*>(mpb_tb_gamesvr.GetTypeName(), obj));
    obj->add_key("svr_id");

#ifndef __LIB_MINI_SERVER__

    obj = new pb_obj(&mpb_tb_promotions, "", "select * from tb_promotions;", "tb_promotions");
    cfg_mappbs.insert(pair<string, pb_obj*>(mpb_tb_promotions.GetTypeName(), obj));
    data_mappbs.insert(pair<string, pb_obj*>(mpb_tb_promotions.GetTypeName(), obj));
    obj->add_key("id");

	obj = new pb_obj(&mpb_tb_city_attack, "", "select * from tb_city_attack;", "tb_city_attack");
	cfg_mappbs.insert(pair<string, pb_obj*>(mpb_tb_city_attack.GetTypeName(), obj));
    data_mappbs.insert(pair<string, pb_obj*>(mpb_tb_city_attack.GetTypeName(), obj));
	obj->add_key("id");

	obj = new pb_obj(&mpb_tb_city_welfare, "", "select * from tb_city_welfare;", "tb_city_welfare");
	cfg_mappbs.insert(pair<string, pb_obj*>(mpb_tb_city_welfare.GetTypeName(), obj));
    data_mappbs.insert(pair<string, pb_obj*>(mpb_tb_city_welfare.GetTypeName(), obj));
	obj->add_key("id");

	obj = new pb_obj(&mpb_tb_center_cycle, "", "select * from tb_center_cycle;", "tb_center_cycle");
	cfg_mappbs.insert(pair<string, pb_obj*>(mpb_tb_center_cycle.GetTypeName(), obj));
    data_mappbs.insert(pair<string, pb_obj*>(mpb_tb_center_cycle.GetTypeName(), obj));
	obj->add_key("level");

	obj = new pb_obj(&mpb_tb_center_cycle_role, "", "select * from tb_center_cycle_role;", "tb_center_cycle_role");
	cfg_mappbs.insert(pair<string, pb_obj*>(mpb_tb_center_cycle_role.GetTypeName(), obj));
    data_mappbs.insert(pair<string, pb_obj*>(mpb_tb_center_cycle_role.GetTypeName(), obj));
	obj->add_key("role_id");

	obj = new pb_obj(&mpb_tb_center_devil, "", "select * from tb_center_devil;", "tb_center_devil");
	cfg_mappbs.insert(pair<string, pb_obj*>(mpb_tb_center_devil.GetTypeName(), obj));
    data_mappbs.insert(pair<string, pb_obj*>(mpb_tb_center_devil.GetTypeName(), obj));
	obj->add_key("id");

	obj = new pb_obj(&mpb_tb_center_notice, "", "select * from tb_center_notice;", "tb_center_notice");
	cfg_mappbs.insert(pair<string, pb_obj*>(mpb_tb_center_notice.GetTypeName(), obj));
	data_mappbs.insert(pair<string, pb_obj*>(mpb_tb_center_notice.GetTypeName(), obj));
	obj->add_key("msg_id");
#endif
//	obj = new pb_obj(&mpb_tb_saparatism_info, "", "select * from tb_saparatism_info", "tb_saparatism_info");
//	cfg_mappbs.insert(pair<string, pb_obj*>(mpb_tb_saparatism_info.GetTypeName(), obj));
//	data_mappbs.insert(pair<string, pb_obj*>(mpb_tb_saparatism_info.GetTypeName(), obj));
//	obj->add_key("region_id");

    obj = new pb_obj(&mpb_sp_login_auth, "", "call sp_login_auth(%s,%s,%u);", "");
    data_mappbs.insert(pair<string, pb_obj*>(mpb_sp_login_auth.GetTypeName(), obj));

    // 存储过程产生的数据配置pb
    //obj = new pb_obj(&msp_gdata, "", "call sp_get_gdata();", "");
    //cfg_mappbs.insert(pair<string, pb_obj*>(msp_gdata.GetTypeName(), obj));

    obj = new pb_obj(&msp_gdata_max_role, "", "call sp_get_gdata_max_role();", "");
    cfg_mappbs.insert(pair<string, pb_obj*>(msp_gdata_max_role.GetTypeName(), obj));
    obj->add_key("svr_id");

    obj = new pb_obj(&msp_load_all_user_smy, "", "call sp_load_all_user_smy();", "");
    cfg_mappbs.insert(pair<string, pb_obj*>(msp_load_all_user_smy.GetTypeName(), obj));
    obj->add_key("role_id");

    obj = new pb_obj(&msp_load_offline_user, "", "call sp_load_offline_user();", "");
    cfg_mappbs.insert(pair<string, pb_obj*>(msp_load_offline_user.GetTypeName(), obj));
    obj->add_key("role_id");

}

int pb_mgr::assemble_init_update_sql(google::protobuf::Message* msg, string& sql)
{
    const pb_obj* obj = get_data_obj(msg->GetTypeName());
    if (obj == NULL){
        return -1;
    }

    char tmp[512] = {0};
    snprintf(tmp, sizeof(tmp), "set %s update %s set ", msg->GetTypeName().c_str(), obj->table_name.c_str());
    sql = tmp;

    return 0;
}

int pb_mgr::assemble_init_insert_sql(google::protobuf::Message* msg, string& sql)
{
    const pb_obj* obj = get_data_obj(msg->GetTypeName());
    if (obj == NULL){
        return -1;
    }

    char tmp[512] = {0};
    snprintf(tmp, sizeof(tmp), "set %s insert into %s values( ", msg->GetTypeName().c_str(), obj->table_name.c_str());
    sql = tmp;

    return 0;
}

int pb_mgr::assemble_init_remove_sql(google::protobuf::Message* msg, string& sql, const vector<string>& key_names, const CACHE_KEY& key)
{
    const pb_obj* obj = get_data_obj(msg->GetTypeName());
    if (obj == NULL){
        return -1;
    }

    char tmp[512] = {0};
    size_t n = key_names.size();
     if (n == 0){
        snprintf(tmp, sizeof(tmp), "set %s delete from %s;", msg->GetTypeName().c_str(), obj->table_name.c_str());
    } else if (n == 1){
        snprintf(tmp, sizeof(tmp), "set %s delete from %s where %s=%u;", msg->GetTypeName().c_str(), obj->table_name.c_str(), key_names[0].c_str(), key.first);
    } else {
        snprintf(tmp, sizeof(tmp), "set %s delete from %s where %s=%u and %s=%u;", msg->GetTypeName().c_str(), obj->table_name.c_str(), key_names[0].c_str(), key.first, key_names[1].c_str(), key.second);
    }
    sql = tmp;

    return 0;
}

int pb_mgr::assemble_init_call_sql(google::protobuf::Message* msg, string& sql)
{
    //const pb_obj* obj = get_data_obj(msg->GetTypeName());
    //if (obj == NULL) {
    //    return -1;
    //}

    //char tmp[512] = { 0 };
    //snprintf(tmp, sizeof(tmp), "call %s ", obj->table_name.c_str());

    // mysql.

    string table_name = msg->GetTypeName().substr(6, msg->GetTypeName().length() - 6);

    char tmp[512] = { 0 };
    snprintf(tmp, sizeof(tmp), "push %s insert into %s values( ", 
        msg->GetTypeName().c_str(), 
        table_name.c_str());
    sql = tmp;

    return 0;
}

int pb_mgr::assemble_end_update(string& sql, const vector<string>& key_names, const CACHE_KEY& key)
{
    char tmp[512] = {0};
    size_t n = key_names.size();
    if (n == 0){
        snprintf(tmp, sizeof(tmp), ";");
    } else if (n == 1){
        snprintf(tmp, sizeof(tmp), " where `%s`=%u;", key_names[0].c_str(), key.first);
    } else {
        snprintf(tmp, sizeof(tmp), " where `%s`=%u and `%s`=%u;", key_names[0].c_str(), key.first, key_names[1].c_str(), key.second);
    }

    sql += tmp;
    return 0;
}

int pb_mgr::assemble_end_insert(string& sql)
{
    sql += ");";
    return 0;
}

int pb_mgr::assemble_end_call(string& sql)
{
    sql += ");";
    return 0;
}
