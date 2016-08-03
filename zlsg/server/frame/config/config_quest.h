#ifndef __CONFIG_QUEST_H__
#define __CONFIG_QUEST_H__

#include "cfg_cache_mgr.h"
#include "cfg_parser.h"
#include "client.role_props.pb.h"
#include "config_base.h"

enum QUEST_TYPE {
    QT_TALK  = 1,
    QT_RAID  = 2,
    QT_PICK  = 3,
    QT_KILL  = 4,
    QT_GUARD = 5,
    QT_LEVEL = 6,
    QT_EVENT = 7,
};

class config_quest {
public:
    config_quest();
    ~config_quest();

public:
    inline       uint32               get_map_id       () const { return _map_id       ; }
    inline       uint32               get_quest_id     () const { return _quest_id     ; }
    inline       uint32               get_next_quest_id() const { return _next_quest_id; }
    inline       uint32               get_accept_type  () const { return _accept_type  ; }
    inline       uint32               get_quest_type   () const { return _quest_type   ; }
    inline const std::map<int, int>&  get_need_item    () const { return _need_item    ; }
    inline const std::map<int, int>&  get_kill_monster () const { return _kill_monster ; }
    inline const std::map<int, int>&  get_guard_monster() const { return _guard_monster; }
    inline       uint32               get_need_level   () const { return _need_level   ; }
    inline       uint32               get_other        () const { return _other        ; }
    inline       uint32               get_reward_exp   () const { return _reward_exp   ; }
    inline       uint32               get_reward_taolue() const { return _reward_taolue; }
//     inline const std::map<int, int>   get_reward_item  () const { return _reward_item  ; }
    inline       uint32               get_reward_gold  () const { return _reward_gold  ; }
    inline       uint32               get_reward_energy() const { return _reward_energy; }
    inline       uint32               get_reward_coin  () const { return _reward_coin  ; }

public:
    void get_reward_item(uint32 id, std::vector<client::props_info>& items) const;

public:
    uint32                              _map_id       ;
    uint32                              _quest_id     ;
    uint32                              _next_quest_id;
    uint32                              _accept_type  ;
    uint32                              _quest_type   ;
    std::map<int, int>                  _need_item    ;
    std::map<int, int>                  _kill_monster ;
    std::map<int, int>                  _guard_monster;
    uint32                              _need_level   ;
    uint32                              _other        ;
    uint32                              _reward_exp   ;
    uint32                              _reward_taolue;
    std::vector<quest_reward_item>      _reward_item  ;
    uint32                              _reward_gold  ;
    uint32                              _reward_energy;
    uint32                              _reward_coin  ;
};

class config_quest_mgr  : public config_base_mgr
{
public:
    config_quest_mgr();
    ~config_quest_mgr();

public:
    int init(TTableGroup_s* p);
    int clear();

public:
    const config_quest* get_config_quest(uint32 id) const;

private:
    std::map<uint32, config_quest*> _content;
};

#endif // __CONFIG_QUEST_H__
