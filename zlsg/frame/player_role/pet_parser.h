#ifndef __PET_PARSER_H__
#define __PET_PARSER_H__

#include "comm_define.h"
#include "client.pets.pb.h"

#define MAX_ROLE_PET_NUM 128
#define MAX_PET_BOLB_LEN 65535  // POTENTIAL��󻺴泤�� 

class pet_parser
{
public:
    pet_parser();
    virtual ~pet_parser();

public:
    // �����ݵĴ���
    int change_pet_info(const client::pet_db_info& bi);
    // �ı�״̬
    int change_pet_status(uint32 pet_id,uint32 status);
    // ��ȡ״̬
    int get_pet_status(uint32 pet_id,uint32& pet_status);
    // �ı����Ѫ��
    bool change_pet_cur_hp(uint32 pet_id,uint32 cur_hp);
    // �Ƿ��������ǳ�ս��״̬
    bool is_has_pet_fighting();
    // �Ƿ��������Ǳ�ս״̬
    bool is_has_pet_prepare();

	uint32 get_fighting_pet_id();
    // �Ƴ�ĳ������
    int remove_pet(uint32 pet_id);
    // �������
    int remove_all();

    bool get_pet_info(uint32 pet_id, client::pet_db_info& pet_db_info);

	bool get_query_fighting_pet(client::pet_db_info& pet_info);

    client::pet_db_info* mutable_pet_info(uint32 pet_id);

    bool create_pet(uint32 pet_id,client::pet_db_info& pet_db_info,uint32 master_hp);

    inline const std::map<uint32, client::pet_db_info>& get_all_pet_db_info() const { return pet_db_info_map_; }

#ifdef  __LIB_MINI_SERVER__
public:
    void set_pets(std::map<uint32, client::pet_db_info>& pets) { pet_db_info_map_ = pets; }
#endif // __LIB_MINI_SERVER__

protected:
    int parse_pet(const string& in); // ��DB�е����ݣ�������PB
    int serialize_pet(string& out);  // ��PB���л���DB����

protected:
    std::map<uint32, client::pet_db_info> pet_db_info_map_;
};

#endif
