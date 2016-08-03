#ifndef __FAMILY_DOMAIN_MGR_H__
#define __FAMILY_DOMAIN_MGR_H__

#include "comm_define.h"
#include "family_domain.h"

class family_domain_mgr
{
public:
    family_domain_mgr();
    virtual ~family_domain_mgr();

public:
    int init(struct cmq_t* l);

    int save(amtsvr_context* ctx);

    multimap<uint32, family_domain*>& get() { return _domain; }

public:
	// �������������ľ���
    //int update(client::family_domain_update* fdu);
	// ����
    int update(uint32 family_id, uint32 scene_id);
	// �Ƴ���ǰ���������ľ���
    int remove(uint32 family_id);
	// ��ȡ���ŵ�������
    uint32 get_num(uint32 family_id);
	// ��ȡ��ǰ�����������ľ���ID
    uint32 get_owner(uint32 scene_id);
	// ��ȡ��ǰ������ӵ�е�ȫ����������ID
    void get_scene(uint32 family_id, std::vector<uint32>& sis);

private:
    
    multimap<uint32, family_domain*> _domain;
}; 

#endif /* __FAMILY_DOMAIN_MGR_H__ */
