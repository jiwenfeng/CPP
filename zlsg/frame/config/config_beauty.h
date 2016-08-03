#ifndef __CONFIG_BEAUTY_H__
#define __CONFIG_BEAUTY_H__

#include "cfg_cache_mgr.h"
#include "cfg_parser.h"
#include "config_base.h"


class config_beauty
{
public:
    config_beauty();
    ~config_beauty();

public:
	// GET
	inline uint32								get_general_id				() const { return general_id		    ;}
	inline string								get_general_name			() const { return general_name		    ;}
	inline uint32								get_general_level			() const { return general_level		    ;}
	inline uint32								get_master_level			() const { return master_level		    ;}
	inline uint32								get_up_need_exp				() const { return up_need_exp		    ;}
	inline const std::map<int32,int32>&			get_recruit_need_item       () const { return recruit_need_item		;}
	inline const std::map<int32,int32>&			get_fit_need_item			() const { return fit_need_item         ;}
	inline const std::map<int32,int32>&			get_fit_add_property		() const { return fit_add_property      ;}
	inline uint32								get_fight_capacity			() const { return fight_capacity		;}
	inline uint32								get_fight_hppercent			() const { return fight_hppercent		;}
	inline uint32								get_fight_atkpercent		() const { return fight_atkpercent		;}
	inline uint32								get_fight_defpercent		() const { return fight_defpercent		;}
	inline const std::vector<int>&				get_general_skill			() const { return general_skill			;}
	inline uint32								get_magic_id				() const { return magic_id				;}
	inline uint32								get_recruit_level			() const { return recruit_level			;}
	inline uint32								get_skill_belong			() const { return skill_belong			;}
	inline uint32								get_fit_times				() const { return fit_times				;}
	inline const std::map<int32,int32>&			get_game_attribute		    () const { return game_attribute        ;}
public:

public:
	// ģ�������
	uint32                                      general_id                ;// �佫ID
	string										general_name			  ;// ��������
	uint32										general_level			  ;// �佫�ȼ�
	uint32										master_level			  ;// �����������˵ȼ�
	uint32										up_need_exp				  ;// �������辭��
	std::map<int32,int32>						recruit_need_item		  ;// ��ļ���ĵ��ߣ�����ID|������
	std::map<int32,int32>						fit_need_item			  ;// ����������Ʒ������(��ƷID_����;��ƷID_����;)
	std::map<int32,int32>						fit_add_property		  ;// ���κ���������������(����_����ֵ��ְҵ;����_����ֵ;)
	uint32										fight_capacity            ;// ս������ֵ
	uint32										fight_hppercent           ;// ����ӵ����������������ֱ�
	uint32										fight_atkpercent          ;// ����ӵ�����˹���������ֱ�
	uint32										fight_defpercent          ;// ����ӵ�����˷���������ֱ�
	std::vector<int>							general_skill			  ;// ����Я�����ܡ�����ID������ID��
	uint32										magic_id				  ;// �������ID
	uint32										recruit_level			  ;// ��ļ�����������˵ĵȼ�����
	uint32										skill_belong		      ;// ���˼��ܹ���ID(11=ף�ڣ�12=���ļ���13=����Ӣ��14=�����㣬15=���ǣ�16=С�ǣ�17=�缧��18=����)
	uint32										fit_times				  ;// �Ѳ������������жϲ������ģ�
	std::map<int32,int32>						game_attribute		      ;// ��������
};

class config_beauty_mgr : public config_base_mgr
{
public:
    config_beauty_mgr(TTableGroup_s* p = NULL);
    ~config_beauty_mgr();

public:
    int                 init(TTableGroup_s* p);
    int                 clear();

public:
    const TTable*       get_table() const { return _table; }
	bool  check_is_have(uint32 id,uint32 level);
    const config_beauty* get_config_beauty(uint32 general_id,uint32 general_level) const;
	const config_beauty* get_config_beauty_by_lingering_count(uint32 general_id,uint32 lingering_count) const;
private:
    TTableGroup_s*                            _beauty;
    TTable*                                 _table;
    std::multimap<uint32, config_beauty*>  _cp;
};

#endif // __CONFIG_UPEQUIP_H__
