#include "sql_parser.h"

int sql_parser::get(const char* name, const char* sql, database& db, std::string& data)
{
    //printf("%s\n", sql);
    google::protobuf::Message* msg = get(name, sql, db);
    if (msg == NULL) {
        return -1;
    }
    
    msg->SerializeToString(&data);
    delete msg;

    return 0;
}

google::protobuf::Message* sql_parser::get(const char* name, const char* sql, database& db)
{
    //printf("%s\n", sql);
    my_ulonglong rows = 0;
    record_set* record = db.query(sql, rows);
    if (record == NULL) {
		if(db.get_errno() != 0)
		{
			printf("%s:%d:db.query failed:%s:%s %d\n", __FILE__, __LINE__, name, sql, db.get_errno());
		}
        return NULL;
    }

    google::protobuf::Message* msg = NULL;
    std::string str = name;
    if (record->next_row()) {
        msg = get(str, record);
    }
    
    delete record;

    return msg;
}

struct cmq_svr_msg_t* sql_parser::gets(const char* name, const char* sql, database& db, size_t& rsp_len)
{
    //printf("%s\n", sql);
    my_ulonglong rows = 0;
    struct cmq_svr_msg_t* ret_svr_msg = NULL;
    record_set* record = db.query(sql, rows);
    if (record == NULL) {
        ret_svr_msg = cmq_msg_create(SSTE_SQLMGR, SQLMGR_NAME, 0, &rsp_len);
        ret_svr_msg->ret = db.get_errno();
		if(db.get_errno() != 0)
		{
			printf("%s:%d:db.query failed:%s:%s:%d\n", __FILE__, __LINE__, name, sql, db.get_errno());
		}
        return ret_svr_msg;
    }

    ret_svr_msg = cmq_msg_create(SSTE_SQLMGR, SQLMGR_NAME, (int)rows, &rsp_len);
    google::protobuf::Message* msg = NULL;
    std::string str = name;
    while (record->next_row()) {
        msg = get(str, record);
        if (msg != NULL) {
            cmq_msg_add(ret_svr_msg, msg, false);
        }
    }
    
    delete record;

    return ret_svr_msg;
}

google::protobuf::Message* sql_parser::get(std::string& name, record_set* rst)
{
    if (rst == NULL) {
        return NULL;
    }

    google::protobuf::Message* msg = create_msg(name);
    if (msg == NULL) {
        return NULL;
    }

    const google::protobuf::Reflection* ref  = msg->GetReflection();
    const google::protobuf::Descriptor* desc = msg->GetDescriptor();
    if (ref == NULL || desc == NULL) {
        delete msg;
    }

    for (int i = 0; i < desc->field_count(); ++i) {
        Fill(msg, desc->field(i), ref, rst);
    }

    return msg;
}

int sql_parser::set(const char* sql, database&db, int& ret)
{
    my_ulonglong rows = 0;
    //printf("%s\n", sql);
    record_set* record = db.query(sql, rows);
    if (record == NULL) {
		if(db.get_errno() != 0)
		{
			printf("%s:%d:db.query failed:%s:%d\n", __FILE__, __LINE__, sql, db.get_errno());
		}
        return db.get_errno();
    }

    string name = "ret";
    record->get(name, ret);
    delete record;

    return 0;
}

void sql_parser::Fill(google::protobuf::Message* msg, const google::protobuf::FieldDescriptor* field, 
        const google::protobuf::Reflection* ref, record_set* rst)
{
    if(field->label() == google::protobuf::FieldDescriptor::LABEL_REPEATED) {
        return;
    }

    int    ival;
    uint32 uival;
    int64  lval;
    uint64 ulval;
    std::string sval;
    double dval;
    float  fval;
    bool   bval;
    std::string name = field->name();

    switch(field->type()) {
        case google::protobuf::FieldDescriptor::TYPE_UINT64 :
            if (!rst->get(name, ulval)) {
                ulval = 0;
            }
            if (field->is_repeated()) {
                ref->AddUInt64(msg, field, ulval);
            } else {
                ref->SetUInt64(msg, field, ulval);
            }
            break;
        case google::protobuf::FieldDescriptor::TYPE_FIXED64 :
        case google::protobuf::FieldDescriptor::TYPE_INT64 :
            if (!rst->get(name, lval)) {
                lval = 0;
            }
            if (field->is_repeated()) {
                ref->AddInt64(msg, field, lval);
            } else {
                ref->SetInt64(msg, field, lval);
            }
            break;
        case google::protobuf::FieldDescriptor::TYPE_UINT32 :
            if (!rst->get(name, uival)) {
                uival = 0;
            }
            if (field->is_repeated()) {
                ref->AddUInt32(msg, field, uival);
            } else {
                ref->SetUInt32(msg, field, uival);
            }
            break;
        case google::protobuf::FieldDescriptor::TYPE_FIXED32:
        case google::protobuf::FieldDescriptor::TYPE_INT32 :
            if (!rst->get(name, ival)) {
                ival = 0;
            }
            if (field->is_repeated()) {
                ref->AddInt32(msg, field, ival);
            } else {
                ref->SetInt32(msg, field, ival);
            }
            break;
        case google::protobuf::FieldDescriptor::TYPE_STRING:
            if (!rst->get(name, sval)) {
                sval = "";
            }
            if (field->is_repeated()) {
                ref->AddString(msg, field, sval);
            } else {
                ref->SetString(msg, field, sval);
            }
            break;
        case google::protobuf::FieldDescriptor::TYPE_DOUBLE:
            if (!rst->get(name, dval)) {
                dval = 0;
            }
            if (field->is_repeated()) {
                ref->AddDouble(msg, field, dval);
            } else {
                ref->SetDouble(msg, field, dval);
            }
            break;
        case google::protobuf::FieldDescriptor::TYPE_FLOAT:
            if (!rst->get(name, fval)) {
                fval = 0;
            }
            if (field->is_repeated()) {
                ref->AddFloat(msg, field, fval);
            } else {
                ref->SetFloat(msg, field, fval);
            }
            break;
        case google::protobuf::FieldDescriptor::TYPE_BOOL:
            if (!rst->get(name, bval)) {
                bval = false;
            }
            if (field->is_repeated()) {
                ref->AddBool(msg, field, bval);
            } else {
                ref->SetBool(msg, field, bval);
            }
            break;
        case google::protobuf::FieldDescriptor::TYPE_BYTES:
            if (!rst->get_blob(name, sval)) {
                sval = "";
            }
            if (field->is_repeated()) {
                ref->AddString(msg, field, sval);
            } else {
                ref->SetString(msg, field, sval);
            }
        default:
            break;
    }
}

