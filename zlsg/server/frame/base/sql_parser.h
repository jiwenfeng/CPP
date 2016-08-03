#ifndef __SQL_PARSOR_H__
#define __SQL_PARSOR_H__

#include <vector>

#include "sql_operator.h"
#include "pb_coder.h"
#include "pb_mgr.h"
#include "cmq_svr_msg.h"

using namespace std;

class sql_parser 
{
public:
    sql_parser(){}

    virtual ~sql_parser(){}

    google::protobuf::Message* get(const char* name, const char* sql, database& db);

    struct cmq_svr_msg_t* gets(const char* name, const char* sql, database& db, size_t& rsp_len);

    int set(const char* sql, database&db, int& ret);

protected:
    int get(const char* name, const char* sql, database& db, std::string& data);

    google::protobuf::Message* get(std::string& name, record_set* rst);

private:
    void Fill(google::protobuf::Message* msg, const google::protobuf::FieldDescriptor* field, 
        const google::protobuf::Reflection* ref, record_set* rst);

private:
    
};

#endif
