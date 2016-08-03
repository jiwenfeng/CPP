#ifndef __RECORD_PARSER_H__
#define __RECORD_PARSER_H__

#include "comm_define.h"
#include "client.poll_activity.pb.h"

#define MAX_ROLE_BUF_NUM 128
#define MAX_RECORD_BOLB_LEN 65535  // POTENTIAL最大缓存长度 

class record_parser
{
public:
    record_parser();

    virtual ~record_parser();
	// 
    int add_record(const client::act_db_info& bi);
	// 清空数据
    int remove_all();

    inline const client::act_db_info& get_act_db_info() const { return _record_info; }

	inline client::act_db_info& mut_db_info(){return _record_info;}
protected:
    int parse_record(const string& in); // 将DB中的数据，解析成PB
    int serialize_record(string& out);  // 将PB序列化到DB数据

protected:
    client::act_db_info _record_info;
};



#endif
