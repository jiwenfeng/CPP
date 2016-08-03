#ifndef __SCENE_ROLE_AI__
#define __SCENE_ROLE_AI__

#include "scene_define.h"
#include "scene_role_attr.h"

class config_npc;

class scene_role_ai {
public:
    scene_role_ai();
    ~scene_role_ai();

public:
    void init(const config_npc* cn);
    inline const server::ai_info& get_ai() const { return _ai; }
	void set_ai(const string &ai);

public:
    bool check_timer(const server::ai_data& ad, uint32& timer, uint32& alarm) const;
    bool check_attr(const server::ai_data& ad, const scene_role_attr& attr) const;
    bool check_distance(const server::ai_data& ad, const map_postion& npc, const map_postion& target) const;
    bool check_attack(const server::ai_data& ad, uint32& attack_count) const;
    bool check_defense(const server::ai_data& ad, uint32& defense_count) const;
    
private:
    size_t add_move(const string& a, size_t i, server::ai_data* ad = NULL, uint32 rand = 100);
    size_t add_fix_move(const string& a, size_t i, server::ai_data* ad = NULL, uint32 rand = 100);
    size_t add_force_move(const string& a, size_t i, server::ai_data* ad = NULL);
    size_t add_attack_target(const string& a, size_t i, server::ai_data* ad = NULL);
    size_t add_method(const string& a, size_t i, server::ai_data* ad = NULL);
    size_t add_timer(const string& a, size_t i, server::ai_data* ad = NULL);
    size_t add_alarm(const string& a, size_t i, server::ai_data* ad = NULL);
    size_t add_attr(uint32 type, server::AI_COMPARE_RULE acr, const string& a, size_t i, server::ai_data* ad = NULL);
    size_t add_distance(const string& a, size_t i, server::ai_data* ad = NULL);
    size_t add_attack(const string& a, size_t i, server::ai_data* ad = NULL);
    size_t add_defense(const string& a, size_t i, server::ai_data* ad = NULL);
    size_t add_dead(const string& a, size_t i, server::ai_data* ad = NULL);
    size_t add_stop(const string& a, size_t i, server::ai_data* ad = NULL, uint32 rand = 100);
    size_t add_hide(const string& a, size_t i, server::ai_data* ad = NULL, uint32 rand = 100);
    size_t add_master(const string& a, size_t i, server::ai_data* ad = NULL, uint32 rand = 100);
    size_t add_msg(const string& a, size_t i, server::ai_data* ad = NULL);

private:
    server::ai_info _ai;
};

#endif //  __SCENE_ROLE_AI__
