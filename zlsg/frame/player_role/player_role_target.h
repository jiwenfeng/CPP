#ifndef __PLAYER_ROLE_TARGET_H__
#define __PLAYER_ROLE_TARGET_H__

#include "comm_define.h"
#include "player_role_statis.h"

class player_role_target
{
public:
    player_role_target();
    ~player_role_target();

    void init(vector<player_role_statis>* prs);
    
public: // once
    void pet_fight();
    uint32 get_pet_fight();
    void use_aoe_skill();
    uint32 get_use_aoe_skill();
    void use_unique_skill();
    uint32 get_use_unique_skill();
    void join_family();
    uint32 get_join_family();
    void equip_temp_wing();
    uint32 get_equip_temp_wing();
    void equip_ring();
    uint32 get_equip_ring();
    void equip_wing();
    uint32 get_equip_wing();
    void enter_family_scene();
    uint32 get_enter_family_scene();
    void enter_team_tower();
    uint32 get_enter_team_tower();
    
public: // half byte
    void add_friend();
    uint32 get_add_friend();
    void kill_monster();
    uint32 get_kill_monster();
    void kill_role();
    uint32 get_kill_role();
    void kill_elite();
    uint32 get_kill_elite();
    void kill_boss(uint32 count);
    uint32 get_kill_boss();
    void enter_pet_raid();
    uint32 get_enter_pet_raid();
    void enter_money_raid();
    uint32 get_enter_money_raid();
    void learn_skill();
    uint32 get_learn_skill();
    void explore(uint32 num);
    uint32 get_explore();
    void wing_level(uint32 lvl);
    uint32 get_wing_level();
    void pet_quality(uint32 quality);
    uint32 get_pet_quality();
    void join_siege();
    uint32 get_join_siege();
    void join_domain();
    uint32 get_join_domain();
    void equip_on(uint32 count);
    uint32 get_equip_on();
    void equip_enhance(uint32 count);
    uint32 get_equip_enhance();
    void equip_reset(uint32 count);
    uint32 get_equip_reset();
    void equip_quality(uint32 count);
    uint32 get_equip_quality();
    void equip_sublime(uint32 count);
    uint32 get_equip_sublime();
    void lnlaid_stone(uint32 count);
    uint32 get_lnlaid_stone();
    void open_pulse();
    uint32 get_open_pulse();
    void high_explore(uint32 num);
    uint32 get_high_explore();
    
public: // byte
    void use_trans();
    uint32 get_use_trans();
    void pet_advanced(uint32 lvl);
    uint32 get_pet_advanced();
    void join_battle();
    uint32 get_join_battle();
    
public: // normal
    void fight_power();
    void level();
    
private:
    bool   target_bit(uint32& src, uint32 index);
    uint32 get_target_bit(uint32 src, uint32 index);
    uint32 target_half_byte(uint32& src, uint32 index, uint32 val = 1);
    uint32 get_target_half_byte(uint32 src, uint32 index);
    uint32 target_byte(uint32& src, uint32 index, uint32 val = 1);
    uint32 get_target_byte(uint32 src, uint32 index);

public:
    static const uint32 ALL_TARGET = 100;
    static const uint16 TARGET_TYPE[ALL_TARGET];

private:
    vector<player_role_statis>* prs;
};

#endif //  __PLAYER_ROLE_TARGET_H__
