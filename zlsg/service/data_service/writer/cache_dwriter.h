#ifndef __CACHE_DWRITER_H__
#define __CACHE_DWRITER_H__

#include "cache_mgr.h"
#include "pb_coder.h"
#include "amtsvr.h"

#define DOUBLE_MAX_SQL_FILED_BUF_LEN 2621500 // 单字段最长数据，为2.5M数据应该足够
#define MAX_SQL_FILED_BUF_LEN        1310720 

class cache_dwriter
{
public:
    cache_dwriter(int n);

    virtual ~cache_dwriter();

    void attach(cache_mgr* c, amtsvr_context* t);

    int push(const char* pbname, CACHE_KEY& key, google::protobuf::Message* msg, int type);

    int write();

    int write_all();
    
    int write(const char* pbname);

    int write(const char* pbname, int n, cache_dwctx* dwctx = NULL);

    int write_all(const char* pbname);

    int close_all();

private:
    int sqlmgr_write(const string& sql, CACHE_KEY& key);
    int write_msg_update(google::protobuf::Message* msg, const vector<string>& veckey_name, CACHE_KEY& key);
    int write_msg_insert(google::protobuf::Message* msg, const vector<string>& veckey_name, CACHE_KEY& key);
    int write_msg_remove(google::protobuf::Message* msg, const vector<string>& veckey_name, CACHE_KEY& key);

private:
    amtsvr_context* ctx;
    cache_mgr*      cmgr;
    int             num;

    void* mmysql;
    char* buffer;
};


#endif
