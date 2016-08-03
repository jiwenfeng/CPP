#ifndef __CONFIG_MOUNT_H__
#define __CONFIG_MOUNT_H__

#include "cfg_cache_mgr.h"
#include "cfg_parser.h"
#include "config_base.h"

class config_mount_exp
{
public:
    config_mount_exp();
    ~config_mount_exp();

public:
	// GET
	inline int                              get_mount_level()	  const {return mount_level;}
	inline int                              get_upgrate_exp()	  const {return upgrate_exp;}
	inline int                              get_mount_idx_limit() const {return mount_idx_limit;}
public:

public:
	// ģ�������
	int                       mount_level                     ; // ����ȼ�
	int						  upgrate_exp					  ; // ������һ�����辭��
	int						  mount_idx_limit				  ; // ����׼��޶�
};

class config_mount_exp_mgr : public config_base_mgr
{
public:
    config_mount_exp_mgr(TTableGroup_s* p = NULL);
    ~config_mount_exp_mgr();

public:
    int                 init(TTableGroup_s* p);
    int                 clear();

public:
    const TTable*       get_table() const { return _table; }
    const config_mount_exp* get_config_mount_exp(int mount_level) const;

private:
    TTableGroup_s*                            _mountexp;
    TTable*                                 _table;
    std::map<uint32, config_mount_exp*>  _cp;
};

#endif // __CONFIG_MOUNT_H__
