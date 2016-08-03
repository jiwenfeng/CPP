#include "amtsvr.h"
#include <mysql.h>
#include "pb_mgr.h"
#include "cache_dwriter.h"

cache_dwriter::cache_dwriter(int n)
{
    ctx  = NULL;
    cmgr = NULL;
    num = n;

    mmysql = mysql_init( NULL );
    buffer = new char[DOUBLE_MAX_SQL_FILED_BUF_LEN];
    buffer[DOUBLE_MAX_SQL_FILED_BUF_LEN-1] = '\0';
}

cache_dwriter::~cache_dwriter()
{
    delete []buffer;
    buffer = NULL;

    if( mmysql != NULL )
    {
        mysql_close( (MYSQL*)mmysql );
        mmysql = NULL;
    }
}

void cache_dwriter::attach(cache_mgr* c, amtsvr_context* t)
{
    cmgr = c;
    ctx = t;
}

int cache_dwriter::write_all()
{
    num = 2147483647; // 取最大值写入，保证链表中数据全部回写数据库
    return write();
}

int cache_dwriter::write()
{
    uint32 count = 0;
    std::vector<cache_dwctx*> cdw;
    cmgr->get_dw_ctxs(cdw);
    uint32 size = cdw.size();
    for (uint32 i = 0; i < size; ++i) {
        count += write(NULL, num, cdw[i]);
    }

    return count;
}

int cache_dwriter::write(const char* pbname, int n, cache_dwctx* dwctx)
{
    if (dwctx == NULL) {
        dwctx =  cmgr->get_dwctx(pbname);
        if (dwctx == NULL) {
            return 0;
        }
    }

    int count = 0;
    const vector<string>& veckey_name = dwctx->get_key_name();
    for (int i = 0; i < n; ++i) {
        CACHE_KEY key;
        uint32 type = 0;
        google::protobuf::Message* msg = dwctx->pop(key, type);
        if (msg == NULL) {
            break;
        }

        int ret = -1;
        switch (type)
        {
        case WOTE_INSERT:
            {
                ret = write_msg_insert(msg, veckey_name, key);
            }
            break;
        case WOTE_UPDATE:
            {
                ret = write_msg_update(msg, veckey_name, key);
            }
            break;
        case WOTE_REMOVE:
            {
                ret = write_msg_remove(msg, veckey_name, key);
            }
            break;
        }

        if (ret == 0){
            ++count;
        }
        
        delete msg;
    }

    return count;
}

int cache_dwriter::write(const char* pbname)
{
    return write(pbname, num);
}

int cache_dwriter::write_all(const char* pbname)
{
    return write(pbname, 2147483647);
}

int cache_dwriter::write_msg_update(google::protobuf::Message* msg, const vector<string>& veckey_name, CACHE_KEY& key)
{
    const google::protobuf::Reflection* ref  = msg->GetReflection();
    const google::protobuf::Descriptor* desc = msg->GetDescriptor();
    if (ref == NULL || desc == NULL) {
        return -1;
    }

    string sql;
    if (PBMGR->assemble_init_update_sql(msg, sql) != 0) {
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
        uint32 k = 0;
        for (; k < veckey_name.size(); ++k) {
            if (veckey_name[k].compare(name) == 0) {
                break;
            }
        }

        if (k < veckey_name.size()) { // 主键不需要在key中出现
            continue;
        }

        switch(field->type()) {
            case google::protobuf::FieldDescriptor::TYPE_UINT64 :
                PBMGR->assemble_add_update_field<uint64>(name, ref->GetUInt64(*msg, field), sql, first);
                break;
            case google::protobuf::FieldDescriptor::TYPE_FIXED64 :
            case google::protobuf::FieldDescriptor::TYPE_INT64 :
                PBMGR->assemble_add_update_field<long long>(name, ref->GetUInt64(*msg, field), sql, first);
                break;
            case google::protobuf::FieldDescriptor::TYPE_UINT32 :
                PBMGR->assemble_add_update_field<uint32>(name, ref->GetUInt32(*msg, field), sql, first);
                break;
            case google::protobuf::FieldDescriptor::TYPE_FIXED32:
            case google::protobuf::FieldDescriptor::TYPE_INT32 :
                PBMGR->assemble_add_update_field<int>(name, ref->GetInt32(*msg, field), sql, first);
                break;
            case google::protobuf::FieldDescriptor::TYPE_STRING:
                PBMGR->assemble_add_update_field<string>(name, ref->GetString(*msg, field), sql, first, true);
                break;
            case google::protobuf::FieldDescriptor::TYPE_DOUBLE:
                PBMGR->assemble_add_update_field<double>(name, ref->GetDouble(*msg, field), sql, first);
                break;
            case google::protobuf::FieldDescriptor::TYPE_FLOAT:
                PBMGR->assemble_add_update_field<float>(name, ref->GetFloat(*msg, field), sql, first);
                break;
            case google::protobuf::FieldDescriptor::TYPE_BOOL:
                PBMGR->assemble_add_update_field<bool>(name, ref->GetBool(*msg, field), sql, first);
                break;
            case google::protobuf::FieldDescriptor::TYPE_BYTES:
                {
                    if (!first) {
                        sql += ',';    
                    }
                    const string& src_str = ref->GetString(*msg, field);
                    uint32 blob_len = src_str.size();
                    if (blob_len < MAX_SQL_FILED_BUF_LEN) {
                        blob_len = mysql_real_escape_string((MYSQL*)mmysql, buffer, src_str.c_str(), blob_len);
                        sql += (name + "='");
                        sql.append(buffer, blob_len);
                        sql += "'";
                    } else {
                        sql += (name + "=''");
                    }
                }
                break;
            default:
                break;
        }  

        first = false;
    }

    if (i == 0) {
        return -3;
    }

    PBMGR->assemble_end_update(sql, veckey_name, key);

    sqlmgr_write(sql, key);

    return 0;
}

int cache_dwriter::write_msg_insert(google::protobuf::Message* msg, const vector<string>& veckey_name, CACHE_KEY& key)
{
    const google::protobuf::Reflection* ref  = msg->GetReflection();
    const google::protobuf::Descriptor* desc = msg->GetDescriptor();
    if (ref == NULL || desc == NULL) {
        return -1;
    }

    string sql;
    if (PBMGR->assemble_init_insert_sql(msg, sql) != 0) {
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

        std::string name = field->name();
        switch(field->type()){
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
            case google::protobuf::FieldDescriptor::TYPE_BYTES:
                {
                    if (first){
                        sql += "'"; 
                    } else {
                        sql += ",'";
                    }
                    
                    const string& src_str = ref->GetString(*msg, field);
                    uint32 blob_len = src_str.size();
                    blob_len = mysql_real_escape_string((MYSQL*)mmysql, buffer, src_str.c_str(), blob_len);
                    if (blob_len < MAX_SQL_FILED_BUF_LEN) {
                        sql.append(buffer, blob_len);
                        sql += "'";
                    } else {
                        sql +="'";
                    }
                }
                break;
            default:
                break;
        }  

        first = false;
    }

    if (i == 0) {
        return -3;
    }

    PBMGR->assemble_end_insert(sql);

    sqlmgr_write(sql, key);

    return 0;
}

int cache_dwriter::sqlmgr_write(const string& sql, CACHE_KEY& key)
{
    size_t len = sql.size()+1;
    char* msg = (char*)malloc(len);

    memcpy(msg, sql.c_str(), sql.size());
    msg[sql.size()] = '\0';

    uint32 id = key.first;
    GET_WRITE_SQLMGR_NAME(id);

    return amtsvr_sendname(ctx, sqlmgr_name, PTYPE_CLIENT|PTYPE_TAG_DONTCOPY, 0, (void*)msg, len);
}
 
int cache_dwriter::write_msg_remove(google::protobuf::Message* msg, const vector<string>& veckey_name, CACHE_KEY& key)
{
    const google::protobuf::Reflection* ref  = msg->GetReflection();
    const google::protobuf::Descriptor* desc = msg->GetDescriptor();
    if (ref == NULL || desc == NULL) {
        return -1;
    }

    string sql;
    if (PBMGR->assemble_init_remove_sql(msg, sql, veckey_name, key) != 0) {
        return -2;
    }

    sqlmgr_write(sql, key);

    return 0;
}

int cache_dwriter::push(const char* pbname, CACHE_KEY& key, google::protobuf::Message* msg, int type)
{
    cache_dwctx* dwctx =  cmgr->get_dwctx(pbname);
    if (dwctx == NULL) {
        delete msg;
        return -1;
    }

    return dwctx->push(key, msg, type);
}

int cache_dwriter::close_all()
{
    for (int id = 0; id < 20; ++id) {
        std::string sql = "close ";
        char buf[32] = {0};
        sprintf(buf, "%d", id);
        sql +=  buf;
        size_t len = sql.size() + 1;
        char* msg  = (char*)malloc(len);
        memcpy(msg, sql.c_str(), sql.size());
        msg[sql.size()] = '\0';
        char sqlmgr_name[128];
        sprintf(sqlmgr_name, "%s_%d", SQLMGR_NAME, id);
        amtsvr_sendname(ctx, sqlmgr_name, PTYPE_CLIENT|PTYPE_TAG_DONTCOPY, 0, (void*)msg, len);
    }

    std::string sql = "close first";
    size_t len = sql.size() + 1;
    char* msg  = (char*)malloc(len);
    memcpy(msg, sql.c_str(), sql.size());
    msg[sql.size()] = '\0';
    amtsvr_sendname(ctx, SQLMGR_NAME, PTYPE_CLIENT|PTYPE_TAG_DONTCOPY, 0, (void*)msg, len);

    return 0;
}
