#ifndef __POTENTIAL_PARSER_H__
#define __POTENTIAL_PARSER_H__

#include "comm_define.h"
#include "client.role_potential.pb.h"

#define MAX_ROLE_BUF_NUM 128
#define MAX_POTENTIAL_BOLB_LEN 65535  // POTENTIAL��󻺴泤�� 

class potential_parser
{
public:
    potential_parser();

    virtual ~potential_parser();
	// �����ķ�����
    int add_potential(const client::acupoint_info& bi);
	// �Ƴ�ĳ������
    int remove_potential(uint32 index);
	// �������
    int remove_all();

	int get_acupoint_info(uint32 index,client::acupoint_info& acupoint_info);

    inline const std::map<uint32, client::acupoint_info>& get_all_potential_info() const { return potential_map_; }

protected:
    int parse_potential(const string& in); // ��DB�е����ݣ�������PB
    int serialize_potential(string& out);  // ��PB���л���DB����

protected:
    std::map<uint32, client::acupoint_info> potential_map_;
};



#endif
