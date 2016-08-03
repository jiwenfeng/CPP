#ifndef __GEM_PARSER_H__
#define __GEM_PARSER_H__

#include "comm_define.h"
#include "client.role_gem.pb.h"

#define MAX_ROLE_BUF_NUM 128
#define MAX_GEM_BOLB_LEN 65535  // GEM最大缓存长度 

class gem_parser
{
public:
    gem_parser();

    virtual ~gem_parser();
	// 新增
    int add_gem(const client::gem_info& bi);
	// 移除某个数据
    int remove_gem(uint32 gem_pos);
	// 清空数据
    int remove_all();

	void create_gem_info(uint32 gem_pos,uint32 open_hole);

	bool find_gem_upgrade_id(uint32 gem_pos,uint32& hole_id);

	const uint32 get_hole_gem_level(uint32 hole_id);

	bool add_gem_exp(uint32 gem_pos,uint32& last_exp,bool& level_is_change);

	bool add_hole_exp(uint32 hole_id,uint32& exp,bool& level_is_change);

	bool gem_level_is_max(uint32 gem_type,uint32 gem_level);

	uint32 get_gem_type(uint32 gem_pos);

	uint32 get_gem_total_level();

    inline const std::map<uint32, client::gem_info>& get_all_gem_info() const { return gem_map_; }

	void get_pos_gem_list(uint32 gem_pos,std::map<uint32, client::gem_info>& pos_gem_list_);
protected:
    int parse_gem(const string& in); // 将DB中的数据，解析成PB
    int serialize_gem(string& out);  // 将PB序列化到DB数据

protected:
    std::map<uint32, client::gem_info> gem_map_;
};



#endif
