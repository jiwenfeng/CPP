#ifndef __CONFIG_MATERRIAL_H__
#define __CONFIG_MATERRIAL_H__

#include "cfg_cache_mgr.h"
#include "cfg_parser.h"
#include "config_base.h"

class config_material
{
public:
    config_material();
    ~config_material();

public:
	// GET
	inline uint32                              get_compound_id()		const { return compound_id		;}
	inline const std::map<int,int>&            get_material_list()		const { return material_list	;}

public:

public:
	// ģ�������
	uint32                              compound_id                     ; // �ϳ���ID
	std::map<int,int>				    material_list				    ; // �ϳ��䷽������ID-1|��������-1������ID-2|��������-2������
};

class config_material_mgr : public config_base_mgr
{
public:
    config_material_mgr(TTableGroup_s* p = NULL);
    ~config_material_mgr();

public:
    int                 init(TTableGroup_s* p);
    int                 clear();

public:
    const TTable*       get_table() const { return _table; }
    const config_material* get_config_material(uint32 compound_id) const;

private:
    TTableGroup_s*                            _material;
    TTable*                                   _table;
    std::map<uint32, config_material*>        _cp;
};

#endif // __CONFIG_MATERRIAL_H__
