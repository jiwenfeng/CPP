#ifndef __CONFIG_CAST_EQUIP_H__
#define __CONFIG_CAST_EQUIP_H__

#include "cfg_cache_mgr.h"
#include "cfg_parser.h"
#include "config_base.h"

// ����ģ������
class config_cast_equip
{
public:
    config_cast_equip();
    ~config_cast_equip();

public:
	inline int                                             get_order_id      () const { return _order_id           ;}
	inline const std::map<int ,int>&                       get_show_level    () const { return _show_level         ;}
	inline int                                             get_need_melting  () const { return _need_melting       ;}
	inline const std::multimap<int,cast_equip>&            get_item_data     () const { return _item_data          ;}
public:

public:
	// ģ�������
	int                                _order_id         ; // װ��ID
	std::map<int ,int>				   _show_level       ; // ��Ҫ�ȼ�(min_max)�����ڵ�����С��С�ڵ������
	int                                _need_melting     ; // ��Ҫ����ֵ
	std::multimap<int,cast_equip>      _item_data        ; // ����ģ������

};

class config_cast_equip_mgr : public config_base_mgr
{
public:
    config_cast_equip_mgr(TTableGroup_s* p = NULL);
    ~config_cast_equip_mgr();

public:
    int                 init(TTableGroup_s* p);
    int                 clear();

public:
    const TTable*       get_table() const { return _table; }
    const config_cast_equip* get_config_cast_equip(uint32 equip_id) const;

private:
    TTableGroup_s*                            _cast_equip;
    TTable*                                 _table;
    std::map<uint32, config_cast_equip*>    _cp;
};

#endif // __CONFIG_UPEQUIP_H__
