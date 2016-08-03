#ifndef __CONFIG_POTENTIAL_H__
#define __CONFIG_POTENTIAL_H__

#include "cfg_cache_mgr.h"
#include "cfg_parser.h"
#include "config_base.h"

class config_potential
{
public:
    config_potential();
    ~config_potential();

public:
	// GET
	inline uint32                              get_potential_id			    () const { return _potential_id						;}
	inline uint32                              get_oder_id					() const { return _oder_id							;}
	inline uint32                              get_pre_potential			() const { return _pre_potential					;}
	inline uint32                              get_need_level				() const { return _need_level						;}
	inline uint32                              get_need_potential			() const { return _need_potential                   ;}
	inline uint32                              get_need_game_money			() const { return _need_game_money					;}
	inline uint32                              get_need_probability		    () const { return _need_probability					;}
	inline uint32                              get_open_server_relief       () const { return _open_server_relief               ;}
	inline uint32                              get_need_count_min			() const { return _need_count_min					;}
	inline uint32                              get_need_count_max           () const { return _need_count_max					;}
	inline const std::map<int,int>&            get_game_attribute			() const { return _game_attribute					;}
	inline const std::vector<int>&             get_reward_skill_list        () const { return _reward_skill_list                ;}
	inline const std::vector<int>&             get_clear_skill_list			() const { return _clear_skill_list					;}
	inline uint32                              get_fail_skill_id			() const { return _fail_skill_id					;}
	inline uint32                              get_success_skill_id			() const { return _success_skill_id					;}
	inline uint32                              get_is_world_broadcast       () const { return _is_world_broadcast               ;}
	inline const std::map<int,int>&			   get_append_attribute			() const { return _append_attribute					;}
	inline const std::map<int,int>&			   get_append_need_item			() const { return _append_need_item					;}
	inline std::string                         get_number_name              () const { return _number_name                      ;}

public:

public:
	// ģ�������
	uint32								 _potential_id                ; // ���Ʊ��
	uint32								 _oder_id                     ; // ����
	uint32								 _pre_potential				  ; // ǰ�õ��Ʊ��
	uint32								 _need_level				  ; // ������Ҫ�ȼ�
	uint32								 _need_potential			  ; // �����������
	uint32								 _need_game_money			  ; // ����������Ϸ��
	uint32								 _need_probability			  ; // ��������óɹ����ʣ���ֱȱ�������ӣ�
	uint32								 _open_server_relief		  ; // ������������ϵ��
	uint32								 _need_count_min			  ; // ���������������
	uint32								 _need_count_max			  ; // ���������������
	std::map<int,int>					 _game_attribute			  ; // �ӳ��������У���������|�ӳ�ֵ:��Чְҵ;��������|�ӳ�ֵ;��
	std::vector<int>					 _reward_skill_list			  ; // ����������⼼�ܼ��ܱ������
	std::vector<int>					 _clear_skill_list			  ; // �����������ܼ��ܱ������
	uint32								 _fail_skill_id				  ; // ����ʧ�ܳ�������ܱ�ţ���Ӧ�������ݿ⣩
	uint32								 _success_skill_id			  ; // ����ɹ���������ܱ�ţ���Ӧ�������ݿ⣩
	uint32								 _is_world_broadcast		  ; // ����ɹ��Ƿ���ȫ���������棨0�����ͣ�1���ͣ�
	std::map<int,int>					 _append_attribute			  ; // ���������������У���������|�ӳ�ֵ:��Чְҵ;��������|�ӳ�ֵ;��
	std::map<int,int>					 _append_need_item			  ; // ��Ҫ����(����id_����)ֻ֧��һ�ֵ���
	std::string							 _number_name			      ; // ��������
};

class config_potential_mgr : public config_base_mgr
{
public:
    config_potential_mgr(TTableGroup_s* p = NULL);
    ~config_potential_mgr();

public:
    int                 init(TTableGroup_s* p);
    int                 clear();

public:
    const TTable*       get_table() const { return _table; }
    const config_potential* get_config_potential(uint32 potential_id) const;

private:
    TTableGroup_s*                               _potential;
    TTable*                                    _table;
    std::map<uint32, config_potential*>        _cp;
};

#endif // __CONFIG_POTENTIAL_H__
