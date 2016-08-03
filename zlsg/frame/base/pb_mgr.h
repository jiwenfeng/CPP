#ifndef __PB_MGR_H__
#define __PB_MGR_H__

// �����ӿڣ�ȫ�����ʵ�ֻ����

#include <iostream>
#include <sstream>
#include <vector>
#include <map>
#include <string>
#include <google/protobuf/descriptor.h>

#include "define.h"
#include "singleton.h"

#include "client_pb_mgr.h"

// inside
#include "client.test.pb.h"

// server֮���PB��Ϣ����
#include "server.register_role.pb.h"
#include "server.init_data_req.pb.h"
#include "server.loader.pb.h"
#include "server.goods.pb.h"
#include "server.record_offline.pb.h"
#include "server.close_agent.pb.h"
#include "server.team_notify.pb.h"
#include "server.trans_back_user.pb.h"
#include "server.log.pb.h"
#include "server.service_notify.pb.h"
#include "server.ai.pb.h"

// ���ݿ��û����ݶ�ӦPB����
#include "pb_mgr_base.h"
#include "mysql.tb_mail_center.pb.h"
#include "mysql.tb_raid_center.pb.h"
#include "mysql.tb_family.pb.h"
#include "mysql.tb_family_log.pb.h"
#include "mysql.tb_family_member.pb.h"
#include "mysql.tb_family_record.pb.h"
#include "mysql.tb_family_skill.pb.h"
#include "mysql.tb_family_domain.pb.h"
#include "mysql.tb_family_diplomacy.pb.h"
#include "mysql.auth_player.pb.h"
#include "mysql.tb_task_log.pb.h"
#include "mysql.tb_money_log.pb.h"
#include "mysql.tb_role_friends.pb.h"
#include "mysql.tb_gamesvr.pb.h"
#include "mysql.tb_mate.pb.h"
#include "mysql.tb_role_potential.pb.h"
#include "mysql.tb_role_farm.pb.h"
#include "mysql.tb_role_pet.pb.h"
#include "mysql.tb_role_cd_cooldown.pb.h"
#include "mysql.tb_role_treasure.pb.h"
#include "mysql.tb_role_black_shop.pb.h"
#include "mysql.tb_activity_record.pb.h"
#include "mysql.tb_activity_open.pb.h"
#include "mysql.tb_role_gem.pb.h"

#ifndef __LIB_MINI_SERVER__
#include "mysql.tb_promotions.pb.h"
#include "mysql.tb_center_arena.pb.h"
#include "mysql.tb_arena_record.pb.h"
#include "mysql.tb_arena_robot.pb.h"
#include "mysql.tb_center_cycle.pb.h"
#include "mysql.tb_center_cycle_role.pb.h"
#include "mysql.tb_rank.pb.h"
#include "mysql.tb_center_devil.pb.h"
#include "mysql.tb_city_attack.pb.h"
#include "mysql.tb_city_welfare.pb.h"
#include "mysql.tb_center_notice.pb.h"
#endif

// һЩ�洢���̵ĵ��ò���������PB����
#include "mysql.sp_get_gdata.pb.h"
#include "mysql.sp_login_auth.pb.h"

// inner
#ifndef __LIB_MINI_SERVER__
#include "inner.comm.pb.h"
#include "inner.notice.pb.h"
#include "inner.player.pb.h"
#include "inner.backpack.pb.h"
#include "inner.card.pb.h"
#include "inner.restict.pb.h"
#include "inner.delivery.pb.h"
#include "inner.function_on.pb.h"
#endif // __LIB_MINI_SERVER__

using namespace std;

using namespace client;
using namespace mysql;

class cache_mgr;

class pb_mgr : public pb_mgr_base
{
public:
    pb_mgr();

    virtual ~pb_mgr();

    const char* get_querysql(const string& type_name);

    const pb_obj* get_data_obj(const string& type_name);

    int assemble_init_insert_sql(google::protobuf::Message* msg, string& sql);
    int assemble_init_update_sql(google::protobuf::Message* msg, string& sql);
    int assemble_init_remove_sql(google::protobuf::Message* msg, string& sql, const vector<string>& key_names, const CACHE_KEY& key);
    int assemble_init_call_sql(google::protobuf::Message* msg, string& sql);
    
    template<typename T> 
    int assemble_add_insert_field(const string& field_name, const T& val, string& sql, bool first = false, bool quot = false);
    template<typename T> 
    int assemble_add_update_field(const string& field_name, const T& val, string& sql, bool first = false, bool quot = false);
    
    int assemble_end_update(string& sql, const vector<string>& key_names, const CACHE_KEY& key);
    int assemble_end_insert(string& sql);
    int assemble_end_call(string& sql);

    void reg();
    void create();
    void destroy();

    map<string, pb_obj*>& get_cfg_pb_maps()
    {
        return cfg_mappbs;
    }

    map<string, pb_obj*>& get_data_pb_maps()
    {
        return data_mappbs;
    }

private:

    //-------------------------------------------------------------------
    mysql::sp_login_auth mpb_sp_login_auth;                     // ��֤���ݷ���ģ��
    mysql::tb_mail_center mpb_tb_mail_center;                   // �ʼ���������
   // mysql::tb_rank_center mpb_tb_rank_center;                   // ���а�����
    mysql::tb_raid_center mpb_tb_raid_center;                   // ��������

    mysql::tb_mate         mpb_tb_mate;                         // ����С����

    mysql::tb_role_farm    mpb_tb_role_farm;                    // �û�����԰��Ϣ

    mysql::tb_family          mpb_tb_family;                    // ��������
    mysql::tb_family_member   mpb_tb_family_member;             // �����Ա
    mysql::tb_family_log      mpb_tb_family_log;                // ������־
    mysql::tb_family_record   mpb_tb_family_record;             // �����¼
    mysql::tb_family_skill    mpb_tb_family_skill;
    mysql::tb_family_domain   mpb_tb_family_domain;
	mysql::tb_family_diplomacy mpb_tb_family_diplomacy; // �����⽻

	mysql::tb_activity_open   mpb_tb_activity_open; // ��������
	mysql::tb_activity_record mpb_tb_activity_record;      // �޹��

    mysql::tb_task_log  mpb_tb_center_task_log;
    mysql::tb_money_log mpb_tb_center_money_log;

#ifndef __LIB_MINI_SERVER__
    mysql::tb_promotions mpb_tb_promotions; // ��̨��������ݴ洢
	mysql::tb_city_attack mpb_tb_city_attack;
	mysql::tb_city_welfare mpb_tb_city_welfare;
    mysql::tb_center_arena mpb_tb_center_arena;                 // ��������
    mysql::tb_arena_record mpb_tb_arena_record;                 // ����������ս��¼
	mysql::tb_arena_robot mpb_tb_arena_robot;
	mysql::tb_rank		mpb_tb_rank;
	mysql::tb_center_cycle		 mpb_tb_center_cycle;
	mysql::tb_center_cycle_role	 mpb_tb_center_cycle_role;
	mysql::tb_center_devil mpb_tb_center_devil;
	mysql::tb_center_notice mpb_tb_center_notice;
#endif

//    mysql::tb_siege_warfare       mpb_tb_siege_warfare;
   // mysql::tb_promo_list_of_names mpb_tb_promo_list_of_names;
    mysql::tb_role_friends        mpb_tb_role_friends;           // �û���������

//	mysql::tb_saparatism_info   mpb_tb_saparatism_info;     //�������������Ϣ

    mysql::tb_gamesvr             mpb_tb_gamesvr;                // ��ű����̷�������Ϣ
    
    mysql::sp_get_gdata          msp_gdata;                      // ȫ����ߵȼ�������صȵ������õĴ洢���̵���
    mysql::sp_get_gdata_max_role msp_gdata_max_role;             // ÿ���������Ľ�ɫid
    mysql::sp_load_all_user_smy  msp_load_all_user_smy;          // ����������ʱ��Ҫ����������û���ժҪ��Ϣ
    mysql::sp_load_offline_user  msp_load_offline_user;          // �����û��Ļ�������
};

extern pb_mgr* PBMGR;

template<typename T> 
int pb_mgr::assemble_add_update_field(const string& field_name, const T& val, string& sql, bool first, bool quot)
{
    string strval;
    std::stringstream tmp;
    tmp << val << std::endl;
    tmp.flush();
    tmp >> strval;

    string t = "";
    if (!first) {
        t =  ",";
    }
    
    if (quot) {
       t += "`" + field_name + "`='" + strval + "'";
    } else {
        t += "`" + field_name + "`=" + strval;
    }
    
    sql += t;

    return 0;
}

template<typename T> 
int pb_mgr::assemble_add_insert_field(const string& field_name, const T& val, string& sql, bool first, bool quot)
{
    string strval;
    std::stringstream tmp;
    tmp << val << std::endl;
    tmp.flush();
    tmp >> strval;

    string t = "";
    if (!first) {
        t =  ",";
    }
    
    if (quot) {
        t += "'" + strval + "'";
    } else {
        t += strval;
    }
    
    sql += t;

    return 0;
}


#define DEFAULT_PB_SVRHEAD_NUM 1000

inline client::server_head* pb_msg_svrhead(google::protobuf::Message* msg)
{
    const google::protobuf::Reflection* ref  = msg->GetReflection();
    const google::protobuf::Descriptor* desc = msg->GetDescriptor();
    if (ref == NULL || desc == NULL) {
        return NULL;
    }

    const google::protobuf::FieldDescriptor* field = desc->FindFieldByNumber(DEFAULT_PB_SVRHEAD_NUM);
    if (field == NULL) {
        return NULL;
    }

    return static_cast<client::server_head*>(ref->MutableMessage(msg, field));
}

#define DEFAULT_PB_INNERHEAD_NUM 1000

#ifndef __LIB_MINI_SERVER__

inline inner::inner_head* pb_msg_inhead(google::protobuf::Message* msg)
{
    const google::protobuf::Reflection* ref  = msg->GetReflection();
    const google::protobuf::Descriptor* desc = msg->GetDescriptor();
    if (ref == NULL || desc == NULL) {
        return NULL;
    }

    const google::protobuf::FieldDescriptor* field = desc->FindFieldByNumber(DEFAULT_PB_INNERHEAD_NUM);
    if (field == NULL) {
        return NULL;
    }

    return static_cast<inner::inner_head*>(ref->MutableMessage(msg, field));
}

#endif // __LIB_MINI_SERVER__
/*void copy_from_different_msg(const google::protobuf::Message& a, int field_fix_a, int field_fix_b, google::protobuf::Message* b)
{
    const google::protobuf::Reflection* ref_a = a.GetReflection();
    const google::protobuf::Descriptor* des_a = a.GetDescriptor();
    const google::protobuf::Reflection* ref_b = b->GetReflection();
    const google::protobuf::Descriptor* des_b = b->GetDescriptor();
    if (ref_a == NULL || des_a == NULL || ref_b == NULL || des_b == NULL) {
        return ;
    }

    int field_count = des_a->field_count() > des_b->field_count() ? des_a->field_count() : des_b->field_count();
    for (int index = field_fix_b; index < field_count; ++index) {
        if ((index + field_fix_a) >= des_a->field_count() || index >= des_b->field_count()) {
            break;
        }
        
        switch (des_a->field(index + field_fix_a)->type()) {
        case google::protobuf::FieldDescriptor::TYPE_UINT32: {
            uint32 val = ref_a->GetUInt32(a, des_a->field(index + field_fix_a));
            ref_b->SetUInt32(b, des_b->field(index), val);
            break;
        }
        case google::protobuf::FieldDescriptor::TYPE_UINT64: {
            uint32 val = ref_a->GetUInt64(a, des_a->field(index + field_fix_a));
            ref_b->SetUInt64(b, des_b->field(index), val);
            break;
        }
        default: break;
        }
    }
    }*/

#endif

