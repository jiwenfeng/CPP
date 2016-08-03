#ifndef __CONFIG_GEM_ACTIVITY_H__
#define __CONFIG_GEM_ACTIVITY_H__

#include "cfg_cache_mgr.h"
#include "cfg_parser.h"
#include "config_base.h"

class config_gem
{
public:
    config_gem();
    ~config_gem();

public:
	// GET
	inline uint32                              get_gem_type()					const { return gem_type					;}
	inline uint32                              get_gem_lv()						const { return gem_lv					;}
	inline const std::map<int, int>&		   get_game_attr()					const { return game_attr				;}
	inline uint32                              get_up_exp()						const { return up_exp					;}
	inline const std::map<int, succinct_info>& get_add_exp_list()				const { return add_exp_list				;}
	inline const std::vector<int>&			   get_upgrade_goods()				const { return upgrade_goods			;}
	inline uint32                              get_upgrade_id()					const { return upgrade_id				;}
	inline uint32                              get_need_count()					const { return need_count				;}
	inline const std::vector<int>&			   get_using_component()			const { return using_component			;}
public:

public:
	// ģ�������
	uint32						 gem_type					; // ��ʯ������
	uint32                       gem_lv                     ; // Ʒ��
	std::map<int,int>			 game_attr					; // ���ӵ���������
	uint32						 up_exp						; // ���������ܾ���
	std::map<int, succinct_info> add_exp_list				; // ����һ�λ�þ���ֵ(��ʽ������_����_�������ͣ�����_����_�������ͣ�����_����_�������ͣ����ݾ����Ӧ��ȡ�ĸ��ʴ���ѡ��һ������ȡ����Ϊ��ֱȣ�1=С������2=�б�����3=�󱩻�
	std::vector<int>				 upgrade_goods				; // �������ĵ��ߣ��������Ĵ˵��ߣ��˵��ߵ�������һ�࣬���ҿ�����������������ģ���ʽ����ƷID;�������_��ƷID
	uint32                       upgrade_id                 ; // ������ʯID
	uint32                       need_count                 ; // ����������������
	std::vector<int>			 using_component		    ; // ����Ƕ�������������ͣ�1=������2=�·���3=������4=ͷ����5=������6=Ь�ӣ����ø�ʽ������;����;����
};

class config_gem_mgr : public config_base_mgr
{
public:
    config_gem_mgr(TTableGroup_s* p = NULL);
    ~config_gem_mgr();

public:
    int                 init(TTableGroup_s* p);
    int                 clear();

public:
    const TTable*       get_table() const { return _table; }
    const config_gem*   get_config_gem(uint32 gem_type,uint32 gem_level) const;

private:
    TTableGroup_s*                          _gem;
    TTable*                                 _table;
    std::multimap<uint32, config_gem*>      _cp;
};

#endif // __CONFIG_MALL_H__
