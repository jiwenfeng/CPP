#ifndef __BUFFER_PARSER_H__
#define __BUFFER_PARSER_H__

#include "comm_define.h"
#include "client.effect.pb.h"

#define MAX_ROLE_BUF_NUM 128
#define MAX_BUFFER_BOLB_LEN 65535  // BUFFER��󻺴泤�� 

class buffer_parser
{
public:
    buffer_parser();

    virtual ~buffer_parser();

    int add_buffer(const client::buffer_info& bi);

    int remove_buffer(uint32 priority_lv);

    int remove_all();

    inline const multimap<uint32, client::buffer_info>& get_all_buffers() const { return buffer_map; }

protected:
    int parse_buffer(const string& in); // ��DB�е����ݣ�������PB
    int serialize_buffer(string& out);  // ��PB���л���DB����

protected:
    multimap<uint32, client::buffer_info> buffer_map;
};



#endif
