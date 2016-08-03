#ifndef __PET_PARSER_H__
#define __PET_PARSER_H__

#include "comm_define.h"
#include "client.pets.pb.h"

#define MAX_ROLE_PET_NUM 128
#define MAX_PET_BOLB_LEN 65535  // POTENTIAL最大缓存长度 

class pet_parser
{
public:
    pet_parser();
    virtual ~pet_parser();

public:
    // 对数据的处理
    int change_pet_info(const client::pet_db_info& bi);
    // 改变状态
    int change_pet_status(uint32 pet_id,uint32 status);
    // 获取状态
    int get_pet_status(uint32 pet_id,uint32& pet_status);
    // 改变宠物血量
    bool change_pet_cur_hp(uint32 pet_id,uint32 cur_hp);
    // 是否有美人是出战的状态
    bool is_has_pet_fighting();
    // 是否有美人是备战状态
    bool is_has_pet_prepare();

	uint32 get_fighting_pet_id();
    // 移除某个数据
    int remove_pet(uint32 pet_id);
    // 清空数据
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
    int parse_pet(const string& in); // 将DB中的数据，解析成PB
    int serialize_pet(string& out);  // 将PB序列化到DB数据

protected:
    std::map<uint32, client::pet_db_info> pet_db_info_map_;
};

#endif
