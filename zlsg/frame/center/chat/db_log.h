#ifndef  __DB_LOG_H__
#define  __DB_LOG_H__

#include <string>
#include <list>

struct amtsvr_context;

class db_log {
public:
    db_log(amtsvr_context* ctx);
    ~db_log();

public:
    void push(google::protobuf::Message* log);

    void write();

private:
    int pb_to_sql(google::protobuf::Message* msg, std::string& sql);

private:
    amtsvr_context* _ctx;
    std::list<google::protobuf::Message*> _logs;
};

#endif // __DB_LOG_H__