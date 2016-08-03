#ifndef __config_npc_H__
#define __config_npc_H__

#include "cfg_cache_mgr.h"
#include "cfg_parser.h"
#include "client.role_props.pb.h"
#include "config_base.h"

class config_props;


void rand_attr(const config_props *cp, client::props_info &pi, const vector<int> &attr = vector<int>());

int make_props(uint32 drop_id, vector<client::props_info> &vpi, uint32 job, uint32 ts = 0, uint32 diff = 0, bool merger = true);

int make_props(uint32 props_id, uint32 n, vector<client::props_info> &v);

int make_props(const uint32 props_id, const uint32 n, client::props_info &pi);

int make_props(const drop_content *dc, client::props_info &pi, uint32 n = 1, uint32 timeout = 0);

class config_drop {
public:
    config_drop(void);
    ~config_drop(void);

public:
    inline const uint32                       get_drop_id()          const { return _drop_id         ; }
    inline const std::vector<drop_content>&   get_drop_content()     const { return _drop_content    ; }
    inline const std::map<int, int>           get_increase_level()   const { return _increase_level  ; }
    inline const std::map<int, int>           get_random_attribute() const { return _random_attribute; }

public:
    uint32                      _drop_id         ;
    std::vector<drop_content>   _drop_content    ;
    std::map<int, int>          _increase_level  ;
    std::map<int, int>          _random_attribute;
};

enum MONSTER_TYPE {
    MT_NORMAL_MONSTER = 0,
    MT_ELITE             ,
    MT_BOSS              ,
    MT_NPC               ,
    MT_GUARD             ,
    MT_PICK              ,
    MT_USER_FRIEND       ,
    MT_TEMP_PET          ,
    MT_TASK_NPC          ,
};

class config_npc {
public:
    config_npc(void);
    ~config_npc(void);
public:
    inline const uint32                    get_npc_id              () const { return _npc_id              ; }
    inline const string&                   get_npc_name            () const { return _npc_name            ; }
    inline const string&                   get_npc_name1            () const { return _npc_name1            ; }
    inline const uint32                    get_npc_type            () const { return _npc_type            ; }
    inline const uint32                    get_monster_level       () const { return _monster_level       ; }
    inline const uint32                    get_attack_mode         () const { return _attack_mode         ; }
    inline const uint32                    get_attack_cool         () const { return _attack_cool         ; }
    inline const uint32                    get_random_move         () const { return _random_move         ; }
    inline const uint32                    get_move_cool           () const { return _move_cool           ; }
    inline const uint32                    get_experience          () const { return _experience          ; }
    inline const uint32                    get_random_move_range   () const { return _random_move_range   ; }
    inline const uint32                    get_visible_range       () const { return _visible_range       ; }
    inline const uint32                    get_follow_range        () const { return _follow_range        ; }
    inline const uint32                    get_default_skill       () const { return _default_skill       ; }
    inline const string&                   get_monster_ai          () const { return _monster_ai          ; }
    inline const uint32                    get_rebirth_time        () const { return _rebirth_time        ; }
    inline const uint32                    get_corpse_remain_time  () const { return _corpse_remain_time  ; }
    inline const std::map<int, int>&       get_game_attribute      () const { return _game_attribute      ; }
    inline const std::map<int, cpf_val_2>& get_random_skill        () const { return _random_skill        ; }
    inline const uint32                    get_monster_difficulty_1() const { return _monster_difficulty_1; }
    inline const uint32                    get_is_remove_pet       () const { return _is_remove_pet       ; }
    inline const uint32                    get_monster_disappear   () const { return _monster_disappear   ; }
    inline const std::vector<int>&         get_immunity_effect     () const { return _immunity_effect     ; }
    inline const std::vector<int>&         get_immunity_skill      () const { return _immunity_skill      ; }

public:
    inline const std::vector<config_drop*>& get_config_drops() const { return _cds; }

	inline const std::vector<int>&		   get_zombie_drop_list() const { return _zombieDropList;}
	inline const uint32					   get_collect_need_time() const { return _collect_time_long; }

public:
    inline const std::vector<uint32>&      get_skills() const { return _skills; }

public:
    uint32                   _npc_id              ;
    string                   _npc_name            ;
    string                   _npc_name1            ;
    uint32                   _npc_type            ;
    uint32                   _monster_level       ;
    uint32                   _attack_mode         ;
    uint32                   _attack_cool         ;
    uint32                   _random_move         ;
    uint32                   _move_cool           ;
    uint32                   _experience          ;
    uint32                   _random_move_range   ;
    uint32                   _visible_range       ;
    uint32                   _follow_range        ;
    uint32                   _default_skill       ;
    string                   _monster_ai          ;
    uint32                   _rebirth_time        ;
    std::vector<int>         _drop_list           ;
    uint32                   _corpse_remain_time  ;
    std::map<int, int>       _game_attribute      ;
    std::map<int, cpf_val_2> _random_skill        ;
    uint32                   _monster_difficulty_1;
	std::vector<int>		 _zombieDropList	  ;
	uint32					 _collect_time_long	  ;
    uint32                   _is_remove_pet       ;
    uint32                   _monster_disappear   ;
    std::vector<int>         _immunity_effect     ;
    std::vector<int>         _immunity_skill      ;

public:
    std::vector<uint32>      _skills;

public:
    std::vector<config_drop*>             _cds;

};

class config_npc_mgr : public config_base_mgr
{
public:
	config_npc_mgr(TTableGroup_s* p = NULL);
	~config_npc_mgr(void);

public:
    int init_drop(TTableGroup_s* ptg);
    int                                  init(TTableGroup_s* p);
    int                                  clear();

public:
    const TTable*                        get_table()   const { return _table; }
    const std::map<uint32, config_npc*>& get_all_npc() const { return _cb; }
    const config_npc*                    get_config_npc(uint32 id) const;
    const config_drop*                   get_config_drop(uint32 id) const;
	config_npc *generate_config_npc(uint32 role_id, uint32 config_id, uint32 level, uint32 type = 0, uint32 disappear = 0) const;

private:
    TTableGroup_s*                 _drop;
    //TTable*                        _drop_table;
    std::map<uint32, config_drop*> _cd;
    
private:
    TTableGroup_s*                _npc;
    TTable*                       _table;
    std::map<uint32, config_npc*> _cb;
};

#endif // __config_npc_H__
