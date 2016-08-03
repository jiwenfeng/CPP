#ifndef __RECORD_PARSER_H__
#define __RECORD_PARSER_H__

#include "comm_define.h"
#include "client.poll_activity.pb.h"

#define MAX_ROLE_BUF_NUM 128
#define MAX_RECORD_BOLB_LEN 65535  // POTENTIAL��󻺴泤�� 

class record_parser
{
public:
    record_parser();

    virtual ~record_parser();
	// 
    int add_record(const client::act_db_info& bi);
	// �������
    int remove_all();

    inline const client::act_db_info& get_act_db_info() const { return _record_info; }

	inline client::act_db_info& mut_db_info(){return _record_info;}
protected:
    int parse_record(const string& in); // ��DB�е����ݣ�������PB
    int serialize_record(string& out);  // ��PB���л���DB����

protected:
    client::act_db_info _record_info;
};



#endif
