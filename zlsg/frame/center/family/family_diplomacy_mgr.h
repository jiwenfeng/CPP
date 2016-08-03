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
	// �Ƴ�����ĳ���ض����ŵĹ�ϵ
	int remove(uint32 family_id,uint32 target_id,bool& is_remove_ally);
	// �Ƴ�ĳ�����ŵ������⽻���ݣ����ڽ�ɢ���ţ�
	int remove(uint32 family_id);
	// ������Ź�ϵ������ĳ�����Ž�ɢ�ˣ��������������й�ϵ�ľ��Ź�ϵ���ݣ�
	int update_diplomacy_relation(uint32 target_id);
	// �⽻��ϵ����
	bool operate_family_foreign(uint32 opt_type,uint32 family_id,uint32 target_id);
	// ������ˣ���֤��
	bool apply_ally(uint32 opt_type,uint32 family_id,uint32 target_id);
	// ��ӵ���
	bool add_hostility(uint32 opt_type,uint32 family_id,uint32 target_id);
	// ͬ�����
	bool agree_ally(uint32 opt_type,uint32 family_id,uint32 target_id);
	// �ܾ�����
	bool reject_ally(uint32 opt_type,uint32 family_id,uint32 target_id);

	bool clean_up_relation_by_hostility(uint32 family_id,uint32 target_id);

	bool clear_apply_relation();
	// ��ȡ��ϵ�ĸ���
	uint32 get_relation_count(uint32 family_id,uint32 relation_type);
public:
	void fill_diplomacy_info(uint32 family_id,client::ally_family_list_rsp& rsp_msg);

	uint32 get_relation_type(uint32 family_id,uint32 target_id);

	uint32 get_relation_family_id(uint32 family_id,uint32 relation_type,vector<uint32>& vec_family_id);
private:
    // key ����ID
    multimap<uint32, family_diplomacy*> family_diplomacy_map_;
	multimap<uint32, family_diplomacy*> _remove;
	vector<family_diplomacy*> _del_vec;
}; 

#endif /* __FAMILY_DOMAIN_MGR_H__ */
