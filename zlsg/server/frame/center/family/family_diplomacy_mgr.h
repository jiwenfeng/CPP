#ifndef __FAMILY_DIPLOMACY_MGR_H__
#define __FAMILY_DIPLOMACY_MGR_H__

#include "comm_define.h"
#include "family_diplomacy.h"

class family_diplomacy_mgr
{
public:
    family_diplomacy_mgr();
    virtual ~family_diplomacy_mgr();

public:
    int init(struct cmq_t* l);

    int save(amtsvr_context* ctx);

    multimap<uint32, family_diplomacy*>& get_diplomacy_obj() { return family_diplomacy_map_; }

public:
	// 移除军团某个特定军团的关系
	int remove(uint32 family_id,uint32 target_id,bool& is_remove_ally);
	// 移除某个军团的所有外交数据（用于解散军团）
	int remove(uint32 family_id);
	// 清理军团关系（用于某个军团解散了，更新其他跟他有关系的军团关系数据）
	int update_diplomacy_relation(uint32 target_id);
	// 外交关系操作
	bool operate_family_foreign(uint32 opt_type,uint32 family_id,uint32 target_id);
	// 申请结盟（验证）
	bool apply_ally(uint32 opt_type,uint32 family_id,uint32 target_id);
	// 添加敌盟
	bool add_hostility(uint32 opt_type,uint32 family_id,uint32 target_id);
	// 同意结盟
	bool agree_ally(uint32 opt_type,uint32 family_id,uint32 target_id);
	// 拒绝结盟
	bool reject_ally(uint32 opt_type,uint32 family_id,uint32 target_id);

	bool clean_up_relation_by_hostility(uint32 family_id,uint32 target_id);

	bool clear_apply_relation();
	// 获取关系的个数
	uint32 get_relation_count(uint32 family_id,uint32 relation_type);
public:
	void fill_diplomacy_info(uint32 family_id,client::ally_family_list_rsp& rsp_msg);

	uint32 get_relation_type(uint32 family_id,uint32 target_id);

	uint32 get_relation_family_id(uint32 family_id,uint32 relation_type,vector<uint32>& vec_family_id);
private:
    // key 军团ID
    multimap<uint32, family_diplomacy*> family_diplomacy_map_;
	multimap<uint32, family_diplomacy*> _remove;
	vector<family_diplomacy*> _del_vec;
}; 

#endif /* __FAMILY_DOMAIN_MGR_H__ */
