#ifndef __CONFIG_BOWS_H__
#define __CONFIG_BOWS_H__

#include "cfg_cache_mgr.h"
#include "cfg_parser.h"
#include "config_base.h"

class config_bows_base
{
public:
    inline int                     get_bow_id				 () const { return _bow_id				  ; }
    inline int                     get_max_skill_level       () const { return _max_skill_level       ; }
    inline const std::vector<int>& get_advance_need_item     () const { return _advance_need_item     ; }
    inline int                     get_advance_need_item_num () const { return _advance_need_item_num ; }
    inline int                     get_advance_count_min     () const { return _advance_count_min     ; }
    inline int                     get_advance_count_max     () const { return _advance_count_max     ; }
    inline int                     get_advance_ratio         () const { return _advance_ratio         ; }
    inline int                     get_advance_fail_bless_min() const { return _advance_fail_bless_min; }
    inline int                     get_advance_fail_belss_max() const { return _advance_fail_belss_max; }
    inline int                     get_advance_bless_max     () const { return _advance_bless_max     ; }
    inline int                     get_success_broadcast     () const { return _success_broadcast     ; }
    //inline int                     get_monster_id            () const { return _monster_id            ; }
    inline int                     get_advance_bless_time    () const { return _advance_bless_time    ; }
    //inline const std::vector<int>& get_talent_skill          () const { return _talent_skill          ; }
    inline const std::vector<int>& get_upgrade_replace_item  () const { return _upgrade_replace_item  ; }
    inline int                     get_use_item_record       () const { return _use_item_record       ; }
    inline int                     get_next_bow_id			 () const { return _next_bow_id           ; }
    inline int                     get_have_addition         () const { return _have_addition         ; }
    inline int                     get_record_max_value      () const { return _record_max_value      ; }
    inline int                     get_potential_addtion     () const { return _potential_addtion     ; }
	//inline const std::vector<int>& get_skill_id              () const { return _skill_id              ; }
	inline std::string             get_bow_name				 () const { return _bow_name              ; }
public:
    int              _bow_id                ;// ����ID
    int              _advance               ;// ��������
    int              _max_skill_level       ;// ���׹�����������������
    std::vector<int> _advance_need_item     ;// �����������Ĳ���ID
    int              _advance_need_item_num ;// ÿ�����Ĳ�������
    int              _advance_count_min     ;// ���׳ɹ�������״�����Сֵ
    int              _advance_count_max     ;// ���׳ɹ�������״������ֵ
    int              _advance_ratio         ;// �������˼����ý��׳ɹ����ʣ���ֱȣ�
    int              _advance_fail_bless_min;// ÿ��ʧ������ף��ֵ����min
    int              _advance_fail_belss_max;// ÿ��ʧ������ף��ֵ����max
    int              _advance_bless_max     ;// ף��ֵ����ֵ
    int              _success_broadcast     ;// ���׳ɹ��Ƿ�ȫ�����棨0�����棬1���棩
    //int              _monster_id            ;// ��������ID(ʹ�ù���������Ϣ)
    int              _advance_bless_time    ;// ף��ֵ����ʱ��
    //std::vector<int> _talent_skill          ;// �츳����(����id;����id)
    std::vector<int> _upgrade_replace_item  ;// ������ʹ����������б�(itemid;itemid)
    int              _use_item_record       ;// �û���Ǳ�ܵ���ʹ�ô�������(������1|��Ʒid1_��������1;����2|��Ʒid2_��������2)��0=�û�����1=Ǳ�ܵ�)
    int              _next_bow_id         ;// ��һ�׵�����ID
    int              _have_addition         ;// �Ƿ�ӵ�����Լӳ�
    int              _record_max_value      ;// ���û�ֵ
    int              _potential_addtion     ;// Ǳ�ܼӳ�ֵ
	//std::vector<int> _skill_id              ;// ����ID
	std::string		 _bow_name			;// ������
};

class config_bows_attr
{
public:
    inline int                       get_bow_id      () const { return _bow_id      ; }
    //inline int                       get_player_job    () const { return _player_job    ; }
    inline int                       get_bow_level   () const { return _bow_level   ; }
    inline const std::map<int, int>& get_game_attribute() const { return _game_attribute; }
    inline int                       get_bow_speed () const { return _bow_speed ; }

public:
    int                _bow_id      ;// ����ID
   // int                _player_job    ;// ��ɫְҵ����(ÿһְҵ��Ӧ��ͬ�ȼ����Բ�ͬ)
    int                _bow_level   ;// �����ȼ�
    std::map<int, int> _game_attribute;// ���Լӳ��б�
    int				   _bow_speed	;
};

class config_bows
{
public:
    ~config_bows() {
        if (_base != NULL) {
            delete _base;
            _base = NULL;
        }

		std::multimap<int, config_bows_attr*>::iterator it = _attr.begin();
		for (; it != _attr.end(); ++it){
			delete it->second;
		}

		_attr.clear();
    }
public:
    inline void set_base(config_bows_base* base) { _base = base; }
    inline void insert_attr(config_bows_attr* attr) {
        _attr.insert(std::pair<int, config_bows_attr*>(attr->get_bow_level(), attr));
    }
    
public:
    inline const config_bows_base*                                 get_base() const { return _base; }
    inline const std::multimap<int, config_bows_attr*>&		       get_attr() const { return _attr; }
    
public:
    config_bows_base*                                _base;
    std::multimap<int, config_bows_attr*>            _attr;
};

class config_bows_mgr : public config_base_mgr
{
public:
    config_bows_mgr();
    virtual ~config_bows_mgr();

public:
    virtual int init(TTableGroup_s* p);
    int                 init(TTableGroup_s* group_base, TTableGroup_s* group_attr);
    int                 clear();
    const TTable*       get_table() const { return _table; }

public:
    const config_bows*      get_config_bows(uint32 bow_id) const;
    const config_bows*      get_config_bows_min() const;
	const config_bows*      get_config_bows_max() const;
    const config_bows_base* get_config_bows_base(uint32 bow_id) const;
    const config_bows_attr* get_config_bows_attr(uint32 bow_id, uint32 lvl) const;

	float calculate_bow_power(uint32 bow_id,uint32 bow_level) const;

private:
    TTableGroup_s*                          _group;
    TTable*                               _table;
    std::map<int, config_bows*>         _content;
};

#endif // __CONFIG_BOWS_H__
