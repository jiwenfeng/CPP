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
	// 更新领土所属的军团
    //int update(client::family_domain_update* fdu);
	// 暂无
    int update(uint32 family_id, uint32 scene_id);
	// 移除当前场景所属的军团
    int remove(uint32 family_id);
	// 获取军团的领土数
    uint32 get_num(uint32 family_id);
	// 获取当前场景所归属的军团ID
    uint32 get_owner(uint32 scene_id);
	// 获取当前军团所拥有的全部领土场景ID
    void get_scene(uint32 family_id, std::vector<uint32>& sis);

private:
    
    multimap<uint32, family_domain*> _domain;
}; 

#endif /* __FAMILY_DOMAIN_MGR_H__ */
