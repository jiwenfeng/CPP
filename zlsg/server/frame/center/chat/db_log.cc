#include "amtsvr.h"
#include <mysql.h>
#include "mysql.t_upgradelog.pb.h"
#include "pb_writer.h"
#include "pb_mgr.h"
#include "cache_dwriter.h"
#include "db_log.h"

db_log::db_log(amtsvr_context* ctx)
    : _ctx(ctx)
{

}

db_log::~db_log()
{
    write();
}

void db_log::push(google::protobuf::Message* log)
{
    google::protobuf::Message* p = create_msg(log->GetTypeName());
    p->CopyFrom(*log);
    _logs.push_back(p);

    write();
}

void db_log::write()
{
    if (_logs.empty()) {
        return ;
    }

    string sql;
    for (std::list<google::protobuf::Message*>::iterator iter_upgrade = _logs.begin(); iter_upgrade != _logs.end(); ++iter_upgrade) {
        int ret = pb_to_sql(*iter_upgrade, sql);
        if (ret == 0) {
            amtsvr_sendname(_ctx, SQLMGR_NAME, PTYPE_CLIENT|PTYPE_TAG_ALLOCSESSION, 0, (void*)sql.c_str(), sql.length());
        } else {
            amtsvr_log("pb_to_sql error %d\n", ret);
        }

        delete (*iter_upgrade);
    }

    _logs.clear();
}

int db_log::pb_to_sql(google::protobuf::Message* msg, string& sql)
{
    const google::protobuf::Reflection* ref  = msg->GetReflection();
    const google::protobuf::Descriptor* desc = msg->GetDescriptor();
    if (ref == NULL || desc == NULL) {
        return -1;
    }

    if (PBMGR->assemble_init_call_sql(msg, sql) != 0) {
        return -2;
    }

    int count = desc->field_count();
    int i = 0;
    bool first = true;
    for (; i < count; ++i) {
        const google::protobuf::FieldDescriptor* field = desc->field(i);
        if (!ref->HasField(*msg, field)) {
            continue;
        }

        const std::string& name = field->name();
        //uint32 k = 0;
        //for (; k < veckey_name.size(); ++k) {
        //    if (veckey_name[k].compare(name) == 0) {
        //        break;
        //    }
        //}

        //if (k < veckey_name.size()) { // 主键不需要在key中出现
        //    continue;
        //}

        switch(field->type()) {
        case google::protobuf::FieldDescriptor::TYPE_UINT64 :
            PBMGR->assemble_add_insert_field<uint64>(name, ref->GetUInt64(*msg, field), sql, first);
            break;
        case google::protobuf::FieldDescriptor::TYPE_FIXED64 :
        case google::protobuf::FieldDescriptor::TYPE_INT64 :
            PBMGR->assemble_add_insert_field<long long>(name, ref->GetUInt64(*msg, field), sql, first);
            break;
        case google::protobuf::FieldDescriptor::TYPE_UINT32 :
            PBMGR->assemble_add_insert_field<uint32>(name, ref->GetUInt32(*msg, field), sql, first);
            break;
        case google::protobuf::FieldDescriptor::TYPE_FIXED32:
        case google::protobuf::FieldDescriptor::TYPE_INT32 :
            PBMGR->assemble_add_insert_field<int>(name, ref->GetInt32(*msg, field), sql, first);
            break;
        case google::protobuf::FieldDescriptor::TYPE_STRING:
            PBMGR->assemble_add_insert_field<string>(name, ref->GetString(*msg, field), sql, first, true);
            break;
        case google::protobuf::FieldDescriptor::TYPE_DOUBLE:
            PBMGR->assemble_add_insert_field<double>(name, ref->GetDouble(*msg, field), sql, first);
            break;
        case google::protobuf::FieldDescriptor::TYPE_FLOAT:
            PBMGR->assemble_add_insert_field<float>(name, ref->GetFloat(*msg, field), sql, first);
            break;
        case google::protobuf::FieldDescriptor::TYPE_BOOL:
            PBMGR->assemble_add_insert_field<bool>(name, ref->GetBool(*msg, field), sql, first);
            break;
//         case google::protobuf::FieldDescriptor::TYPE_BYTES:
//             {
//                 if (!first) {
//                     sql += ',';    
//                 }
//                 const string& src_str = ref->GetString(*msg, field);
//                 uint32 blob_len = src_str.size();
//                 if (blob_len < MAX_SQL_FILED_BUF_LEN) {
//                     blob_len = mysql_real_escape_string((MYSQL*)mmysql, buffer, src_str.c_str(), blob_len);
//                     sql += (name + "='");
//                     sql.append(buffer, blob_len);
//                     sql += "'";
//                 } else {
//                     sql += (name + "=''");
//                 }
//             }
//             break;
        default:
            break;
        }  

        first = false;
    }

    if (i == 1) {
        return -3;
    }

    PBMGR->assemble_end_call(sql);

    return 0;
}
