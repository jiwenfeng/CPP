#ifndef __config_birth_H__
#define __config_birth_H__

#include "cfg_cache_mgr.h"
#include "config_base.h"

class config_birth
{
public:
    config_birth(void);
    ~config_birth(void);

public:
    inline uint32                    get_job()              const { return _job             ; }
    inline uint32                    get_scene_id()         const { return _scene_id        ; }
    inline uint32                    get_spawn_point_id()   const { return _spawn_point_id  ; }
    inline const std::map<int, int>& get_equip()            const { return _equip           ; }
    inline const std::map<int, int>& get_item_list()        const { return _item_list       ; }
    inline const std::vector<int>&   get_skill_list()       const { return _skill_list      ; }
    inline uint32                    get_default_skill_id() const { return _default_skill_id; }
    inline uint32                    get_quest_id()         const { return _quest_id        ; }

public:
    uint32                   _job;
    uint32                   _scene_id;
    uint32                   _spawn_point_id;
    std::map<int, int>       _equip;
    std::map<int, int>       _item_list;
    std::vector<int>         _skill_list;
    uint32                   _default_skill_id;
    uint32                   _quest_id;
    std::map<int, int>       _shortcut;
    uint32                   _default_clothe_id;
    std::map<int, int>       _view_item_list;
    std::string              _job_describes;
    uint32                   _job_icon;
};

class config_birth_mgr : public config_base_mgr
{
public:
    config_birth_mgr(TTableGroup_s* p = NULL);
    ~config_birth_mgr(void);

public:
    int init(TTableGroup_s* p);
    int clear();

public:
    const TTable* get_table() const { return _table; }
    const config_birth* get_config_birth(uint32 job) const;

private:
    TTableGroup_s* _birth;
    TTable*      _table;
    std::map<uint32, config_birth*> _cb;
};

#endif // __config_birth_H__
