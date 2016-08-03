#ifndef __CD_COOLDOWN_PARSER_H__
#define __CD_COOLDOWN_PARSER_H__

#include "comm_define.h"
#include "client.role_cd_cooldown.pb.h"

#define MAX_ROLE_CD_NUM 128
#define MAX_CD_BOLB_LEN 65535  // CD��󻺴泤�� 

class cd_cooldown_parser
{
public:
    cd_cooldown_parser();

    virtual ~cd_cooldown_parser();
	// ����CD����
    int add_cd(const client::cd_info& bi);
	// �Ƴ�ĳ������
    int remove_cd(uint32 index);
	// �������
    int remove_all();

	int get_cd_info(uint32 type_id,client::cd_info& cd_info);

    inline const std::map<uint32, client::cd_info>& get_all_cd_info() const { return cd_info_map_; }
	
	bool add_cooldown_time(uint32 type_id);

	uint32 get_cooldown_time(uint32 type_id) const;

	bool is_can_operate_by_cd(uint32 type_id);

protected:
    int parse_cd(const string& in); // ��DB�е����ݣ�������PB
    int serialize_cd(string& out);  // ��PB���л���DB����

protected:
    std::map<uint32, client::cd_info> cd_info_map_;
};



#endif
