#ifndef __CREATE_NPC_RULE_H__
#define __CREATE_NPC_RULE_H__

#include "scene_define.h"
#include "client.terrain.pb.h"

class genterate_create_npc_rule {
private:
    genterate_create_npc_rule();
    static genterate_create_npc_rule* instance;

public:
    static genterate_create_npc_rule* get_instance() {
        if (instance == NULL) {
            instance = new genterate_create_npc_rule;
        }

        return instance;
    }

    static void release() {
        if (instance != NULL) {
            delete instance;
        }

        instance = NULL;
    }

public:
    int genterate(const std::string& str, server::create_npc_rules& rules);
    int genterate_effect(const std::string& str, server::create_effect_rules& rules);

public:
    static int advance(const std::string& a, size_t i, server::create_npc_rule& rule);
    static int into(const std::string& a, size_t i, server::create_npc_rule& rule);
    static int story(const std::string& a, size_t i, server::create_npc_rule& rule);
    static int kill(const std::string& a, size_t i, server::create_npc_rule& rule);
    static int week(const std::string& a, size_t i, server::create_npc_rule& rule);
    static int day(const std::string& a, size_t i, server::create_npc_rule& rule);
    static int task(const std::string& a, size_t i, server::create_npc_rule& rule);
    static int activity(const std::string& a, size_t i, server::create_npc_rule& rule);
    static int time(const std::string& a, size_t i, server::create_npc_rule& rule);
    static int item(const std::string& a, size_t i, server::create_npc_rule& rule);
    static int object(const std::string& a, size_t i, server::create_npc_rule& rule);
    static int find(const std::string& a, size_t i, server::create_npc_rule& rule);
    static int group(const std::string& a, size_t i, server::create_npc_rule& rule);
    static int appreare_view(const std::string& a, size_t i, server::create_npc_rule& rule);
    static int appear_around(const std::string& a, size_t i, server::create_npc_rule& rule);
    static int refresh_time(const std::string& a, size_t i, server::create_npc_rule& rule);
    static int next(const std::string& a, size_t i, server::create_npc_rule& rule);
    static int Clear(const std::string& a, size_t i, server::create_npc_rule& rule);
    static int Index(const std::string& a, size_t i, server::create_npc_rule& rule);
    static int Msg(const std::string& a, size_t i, server::create_npc_rule& rule);
    static int suspend(const std::string& a, size_t i, server::create_npc_rule& rule);
    typedef int (*func)(const std::string& str, size_t pos, server::create_npc_rule& rule);

public:
    typedef int (*func_effect)(const std::string& str, size_t pos, server::create_effect_rule& rule);
    static int into_effect(const std::string& a, size_t i, server::create_effect_rule& rule);
    static int time_effect(const std::string& a, size_t i, server::create_effect_rule& rule);

private:
    static int appreare(server::appeare_type type, const std::string& a, size_t i, server::create_npc_rule& rule);
    
private:
    std::map<std::string, func>        _rules;
    std::map<std::string, func_effect> _rules_effect;
};

class scene_npc;
class scene_inst;

class tick_npc {
public:
    tick_npc(uint32 tick, const origin_npc* npc, const map_postion& pos) : _tick(tick), _npc(npc), _pos(pos) {

    }

    ~tick_npc() {

    }

public:
    uint32            _tick;
    const origin_npc* _npc;
    map_postion       _pos; 
};

class tick_effect {
public:
    tick_effect(uint32 last_tick, uint32 frequency) : _last_tick(last_tick), _frequency(frequency) {

    }

    tick_effect(const tick_effect& te) {
        _last_tick = te._last_tick;
        _frequency = te._frequency;
        _ti.CopyFrom(te._ti);
    }

public:
    uint32               _last_tick;
    uint32               _frequency;
    client::terrain_info _ti;
};

class check_create_npc_rule {
public:
    check_create_npc_rule(scene_inst* psi = NULL);
    ~check_create_npc_rule();

public:
    void init(scene_inst* psi);
    inline scene_inst* get_scene_inst() { return _psi; }

public:
    bool is_all_create();

public:
    int on_update(uint32 tick);
    int on_update_npc(uint32 tick);
    int on_update_effect(uint32 tick);

public:
    void on_create_npc_into_appeare(const scene_role* psr, const map_postion& from, const map_postion& to);
    void on_create_npc_kill_monster(const scene_role* def);
    void on_create_npc_find_monster(uint32 id);
    void on_after_story(uint32 id);
    void on_update_time();
    void on_get_item(const client::props_info* ppi);
    void on_touch_object(const scene_role* psr);

    bool check_kill();
    bool check_task();
    bool check_activity();
    bool check_time();
    bool check_item();
    bool check_object();
    bool check_find();

    bool check_suspend(const server::create_npc_rule& cnr);
    bool check_suspend_monster_count(uint32 count);

    inline std::list<server::create_msg>* mutable_msg() { return &_msg; }

private:
    void create_npc(const server::create_npc_rule& cnr);
    void create_npc(const server::create_npc_rule& cnr, std::vector<scene_npc*>& sns);
    void create_npc(const server::create_npc_rule& cnr, uint32 index, std::vector<scene_npc*>& sns);
    void create_npc(tick_npc& npc, uint32 t, std::vector<scene_npc*>& sns);
    void create_npc_by_tick(uint32 tick, std::map<uint32, uint32>& ticks, std::vector<scene_npc*>& sns);
    void create_effect(const server::create_effect_rule& cnr, uint32 index, uint32 tick);
    void create_effect_by_tick(uint32 tick, std::map<uint32, std::map<uint32, uint32> >& ticks);

private:
    scene_inst*                                 _psi;
    std::map<uint32, bool>                      _already_create_npc;
    std::map<uint32, std::map<uint32, bool> >   _already_into;
    std::map<uint32, std::map<uint32, bool> >   _already_story;
    std::map<uint32, std::map<uint32, bool> >   _already_item;
    std::map<uint32, std::map<uint32, bool> >   _already_object;
    std::map<uint32, std::map<uint32, bool> >   _already_time;
    std::map<uint32, std::map<uint32, bool> >   _already_kill;
    std::map<uint32, std::map<uint32, bool> >   _already_find;
    std::map<uint32, uint32>                    _kill_monster;
    std::map<uint32, uint32>                    _find_monster;
    std::map<uint32, std::map<uint32, uint32> > _already_into_tick;
    std::map<uint32, uint32>                    _kill_monster_tick;
    std::map<uint32, uint32>                    _find_monster_tick;
    std::map<uint32, uint32>                    _stories;
    std::map<uint32, std::map<uint32, uint32> > _time_counts;
    std::map<uint32, uint32>                    _items;
    std::map<uint32, uint32>                    _touch_objects;

    std::map<uint32, std::map<uint32, bool> >   _already_into_effect;
    std::map<uint32, std::map<uint32, uint32> > _already_into_tick_effect;
    std::map<uint32, std::map<uint32, uint32> > _already_time_tick_effect;

    std::vector<tick_effect>                    _tick_effects;
    std::list<tick_npc>                         _tick_npcs;
    uint32                                      _pass_tick;

    uint32                                      _cur_index;
    std::list<server::create_msg>               _msg;

};

#endif // __CREATE_NPC_RULE_H__
