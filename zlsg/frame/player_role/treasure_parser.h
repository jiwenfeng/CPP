#ifndef __TREASURE_PARSER_H__
#define __TREASURE_PARSER_H__

#include "comm_define.h"

#define MAX_ROLE_TREASURE_NUM 128
#define MAX_TREASURE_BOLB_LEN 65535  // POTENTIAL最大缓存长度 

class treasure_parser
{
public:
    treasure_parser();

    virtual ~treasure_parser();
	// 对数据的处理
    int change_treasure_info(const client::treasure_db_info& bi,uint32 shop_id = 0);
	// 移除某个数据
    int remove_treasure(uint32 item_id);
	// 清空数据
    int remove_all();

	bool get_treasure_info(uint32 item_id,client::treasure_db_info& pet_db_info,uint32 shop_id = 0);

	bool create_treasure_info(uint32 item_id,client::treasure_db_info& treasure_info,uint32 shop_id = 0);

    inline const std::multimap<uint32, client::treasure_db_info>& get_all_treasure_db_info() const { return tresure_db_info_map_; }

	void get_shop_treasure_db_info(uint32 shop_id,std::map<uint32, client::treasure_db_info>& shop_limit_map);

protected:
    int parse_treasure(const string& in); // 将DB中的数据，解析成PB
    int serialize_treasure(string& out);  // 将PB序列化到DB数据

protected:
    std::multimap<uint32, client::treasure_db_info> tresure_db_info_map_;
};



#endif
