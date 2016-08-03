#ifndef __BLACK_SHOP_PARSER_H__
#define __BLACK_SHOP_PARSER_H__

#include "comm_define.h"
#include "client.role_props.pb.h"

#define MAX_ROLE_BUF_NUM 128
#define MAX_POTENTIAL_BOLB_LEN 65535  // POTENTIAL最大缓存长度 

class black_shop_parser
{
public:
    black_shop_parser();

    virtual ~black_shop_parser();
	// 新增黑市商店数据数据
    int add_blacl_shop_data(const client::black_shop_db_info& bi);
	// 移除某个数据
    int remove_black_shop_data(uint32 index);
	// 清空数据
    int remove_all();

	int get_black_shop_data_info(uint32 item_slot,client::black_shop_db_info& slot_info);

    inline const std::map<uint32, client::black_shop_db_info>& get_black_shop_item_list() const { return black_shop_map_; }

protected:
    int parse_black_shop(const string& in); // 将DB中的数据，解析成PB
    int serialize_black_shop(string& out);  // 将PB序列化到DB数据

protected:
    std::map<uint32, client::black_shop_db_info> black_shop_map_;
};



#endif
