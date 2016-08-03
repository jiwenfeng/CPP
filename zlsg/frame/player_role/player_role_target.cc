#include "player_role_target.h"

const uint16 player_role_target::TARGET_TYPE[ALL_TARGET] = {
    0, 0
};

player_role_target::player_role_target()
    : prs(NULL)
{
}

player_role_target::~player_role_target()
{
}

void player_role_target::init(vector<player_role_statis>* _prs)
{
    if (_prs != NULL) {
        prs = _prs;
    }
}

void player_role_target::pet_fight()
{
    uint32 src = prs->at(PRSTE_TARGET_ONCE_0).get();
    target_bit(src, 0);
    prs->at(PRSTE_TARGET_ONCE_0).set_val(src);
}

uint32 player_role_target::get_pet_fight()
{
    uint32 src = prs->at(PRSTE_TARGET_ONCE_0).get();
    return get_target_bit(src, 0);
}

void player_role_target::use_aoe_skill()
{
    uint32 src = prs->at(PRSTE_TARGET_ONCE_0).get();
    target_bit(src, 1);
    prs->at(PRSTE_TARGET_ONCE_0).set_val(src);
}

uint32 player_role_target::get_use_aoe_skill()
{
    uint32 src = prs->at(PRSTE_TARGET_ONCE_0).get();
    return get_target_bit(src, 1);
}

void player_role_target::use_unique_skill()
{
    uint32 src = prs->at(PRSTE_TARGET_ONCE_0).get();
    target_bit(src, 2);
    prs->at(PRSTE_TARGET_ONCE_0).set_val(src);
}

uint32 player_role_target::get_use_unique_skill()
{
    uint32 src = prs->at(PRSTE_TARGET_ONCE_0).get();
    return get_target_bit(src, 2);
}

void player_role_target::join_family()
{
    uint32 src = prs->at(PRSTE_TARGET_ONCE_0).get();
    target_bit(src, 3);
    prs->at(PRSTE_TARGET_ONCE_0).set_val(src);
}

uint32 player_role_target::get_join_family()
{
    uint32 src = prs->at(PRSTE_TARGET_ONCE_0).get();
    return get_target_bit(src, 3);
}

void player_role_target::equip_temp_wing()
{
    uint32 src = prs->at(PRSTE_TARGET_ONCE_0).get();
    target_bit(src, 4);
    prs->at(PRSTE_TARGET_ONCE_0).set_val(src);
}

uint32 player_role_target::get_equip_temp_wing()
{
    uint32 src = prs->at(PRSTE_TARGET_ONCE_0).get();
    return get_target_bit(src, 4);
}

void player_role_target::equip_ring()
{
    uint32 src = prs->at(PRSTE_TARGET_ONCE_0).get();
    target_bit(src, 5);
    prs->at(PRSTE_TARGET_ONCE_0).set_val(src);
}

uint32 player_role_target::get_equip_ring()
{
    uint32 src = prs->at(PRSTE_TARGET_ONCE_0).get();
    return get_target_bit(src, 5);
}

void player_role_target::equip_wing()
{
    uint32 src = prs->at(PRSTE_TARGET_ONCE_0).get();
    target_bit(src, 6);
    prs->at(PRSTE_TARGET_ONCE_0).set_val(src);
}

uint32 player_role_target::get_equip_wing()
{
    uint32 src = prs->at(PRSTE_TARGET_ONCE_0).get();
    return get_target_bit(src, 6);
}

void player_role_target::enter_family_scene()
{
    uint32 src = prs->at(PRSTE_TARGET_ONCE_0).get();
    target_bit(src, 7);
    prs->at(PRSTE_TARGET_ONCE_0).set_val(src);
}

uint32 player_role_target::get_enter_family_scene()
{
    uint32 src = prs->at(PRSTE_TARGET_ONCE_0).get();
    return get_target_bit(src, 7);
}

void player_role_target::enter_team_tower()
{
    uint32 src = prs->at(PRSTE_TARGET_ONCE_0).get();
    target_bit(src, 8);
    prs->at(PRSTE_TARGET_ONCE_0).set_val(src);
}

uint32 player_role_target::get_enter_team_tower()
{
    uint32 src = prs->at(PRSTE_TARGET_ONCE_0).get();
    return get_target_bit(src, 8);
}

void player_role_target::add_friend()
{
    uint32 src = prs->at(PRSTE_TARGET_HALF_BYTE_0).get();
    target_half_byte(src, 0);
    prs->at(PRSTE_TARGET_HALF_BYTE_0).set_val(src);
}

uint32 player_role_target::get_add_friend()
{
    uint32 src = prs->at(PRSTE_TARGET_HALF_BYTE_0).get();
    return get_target_half_byte(src, 0);
}

void player_role_target::kill_monster()
{
    uint32 src = prs->at(PRSTE_TARGET_HALF_BYTE_0).get();
    target_half_byte(src, 4);
    prs->at(PRSTE_TARGET_HALF_BYTE_0).set_val(src);
}

uint32 player_role_target::get_kill_monster()
{
    uint32 src = prs->at(PRSTE_TARGET_HALF_BYTE_0).get();
    return get_target_half_byte(src, 4);
}

void player_role_target::kill_role()
{
    uint32 src = prs->at(PRSTE_TARGET_HALF_BYTE_0).get();
    target_half_byte(src, 8);
    prs->at(PRSTE_TARGET_HALF_BYTE_0).set_val(src);
}

uint32 player_role_target::get_kill_role()
{
    uint32 src = prs->at(PRSTE_TARGET_HALF_BYTE_0).get();
    return get_target_half_byte(src, 8);
}

void player_role_target::kill_elite()
{
    uint32 src = prs->at(PRSTE_TARGET_HALF_BYTE_0).get();
    target_half_byte(src, 12);
    prs->at(PRSTE_TARGET_HALF_BYTE_0).set_val(src);
}

uint32 player_role_target::get_kill_elite()
{
    uint32 src = prs->at(PRSTE_TARGET_HALF_BYTE_0).get();
    return get_target_half_byte(src, 12);
}

void player_role_target::kill_boss(uint32 boss)
{
    uint32 src = prs->at(PRSTE_TARGET_HALF_BYTE_0).get();
    uint32 old = (src >> 16) & 0x0000000F;
    uint32 val = (old | boss) < 0x0000000F ? (old | boss) : 0x0000000F;
    src |= (val << 16);
    prs->at(PRSTE_TARGET_HALF_BYTE_0).set_val(src);
}

uint32 player_role_target::get_kill_boss()
{
    uint32 src = prs->at(PRSTE_TARGET_HALF_BYTE_0).get();
    return get_target_half_byte(src, 16);
}

void player_role_target::enter_pet_raid()
{
    uint32 src = prs->at(PRSTE_TARGET_HALF_BYTE_0).get();
    target_half_byte(src, 20);
    prs->at(PRSTE_TARGET_HALF_BYTE_0).set_val(src);
}

uint32 player_role_target::get_enter_pet_raid()
{
    uint32 src = prs->at(PRSTE_TARGET_HALF_BYTE_0).get();
    return get_target_half_byte(src, 20);
}

void player_role_target::enter_money_raid()
{
    uint32 src = prs->at(PRSTE_TARGET_HALF_BYTE_0).get();
    target_half_byte(src, 24);
    prs->at(PRSTE_TARGET_HALF_BYTE_0).set_val(src);
}

uint32 player_role_target::get_enter_money_raid()
{
    uint32 src = prs->at(PRSTE_TARGET_HALF_BYTE_0).get();
    return get_target_half_byte(src, 24);
}

void player_role_target::learn_skill()
{
    uint32 src = prs->at(PRSTE_TARGET_HALF_BYTE_0).get();
    target_half_byte(src, 28);
    prs->at(PRSTE_TARGET_HALF_BYTE_0).set_val(src);
}

uint32 player_role_target::get_learn_skill()
{
    uint32 src = prs->at(PRSTE_TARGET_HALF_BYTE_0).get();
    return get_target_half_byte(src, 28) + 1; // + 1是初始的普攻技能
}

void player_role_target::explore(uint32 num)
{
    uint32 src = prs->at(PRSTE_TARGET_HALF_BYTE_1).get();
    target_half_byte(src, 0, num);
    prs->at(PRSTE_TARGET_HALF_BYTE_1).set_val(src);
}

uint32 player_role_target::get_explore()
{
    uint32 src = prs->at(PRSTE_TARGET_HALF_BYTE_1).get();
    return get_target_half_byte(src, 0);
}

void player_role_target::wing_level(uint32 lvl)
{
    uint32 src = prs->at(PRSTE_TARGET_HALF_BYTE_1).get();
    target_half_byte(src, 4, lvl);
    prs->at(PRSTE_TARGET_HALF_BYTE_1).set_val(src);
}

uint32 player_role_target::get_wing_level()
{
    uint32 src = prs->at(PRSTE_TARGET_HALF_BYTE_1).get();
    return get_target_half_byte(src, 4);
}

void player_role_target::pet_quality(uint32 quality)
{
    uint32 src = prs->at(PRSTE_TARGET_HALF_BYTE_1).get();
    target_half_byte(src, 8, quality);
    prs->at(PRSTE_TARGET_HALF_BYTE_1).set_val(src);
}

uint32 player_role_target::get_pet_quality()
{
    uint32 src = prs->at(PRSTE_TARGET_HALF_BYTE_1).get();
    return get_target_half_byte(src, 8);
 }

void player_role_target::join_siege()
{
    uint32 src = prs->at(PRSTE_TARGET_HALF_BYTE_1).get();
    target_half_byte(src, 12);
    prs->at(PRSTE_TARGET_HALF_BYTE_1).set_val(src);
}

uint32 player_role_target::get_join_siege()
{
    uint32 src = prs->at(PRSTE_TARGET_HALF_BYTE_1).get();
    return get_target_half_byte(src, 12);
}

void player_role_target::join_domain()
{
    uint32 src = prs->at(PRSTE_TARGET_HALF_BYTE_1).get();
    target_half_byte(src, 16);
    prs->at(PRSTE_TARGET_HALF_BYTE_1).set_val(src);
}

uint32 player_role_target::get_join_domain()
{
    uint32 src = prs->at(PRSTE_TARGET_HALF_BYTE_1).get();
    return get_target_half_byte(src, 16);
}

void player_role_target::equip_on(uint32 count)
{
    uint32 src = prs->at(PRSTE_TARGET_HALF_BYTE_1).get();
    target_half_byte(src, 20, count);
    prs->at(PRSTE_TARGET_HALF_BYTE_1).set_val(src);
}

uint32 player_role_target::get_equip_on()
{
    uint32 src = prs->at(PRSTE_TARGET_HALF_BYTE_1).get();
    return get_target_half_byte(src, 20);
}

void player_role_target::equip_enhance(uint32 count)
{
    uint32 src = prs->at(PRSTE_TARGET_HALF_BYTE_1).get();
    target_half_byte(src, 24, count);
    prs->at(PRSTE_TARGET_HALF_BYTE_1).set_val(src);
}

uint32 player_role_target::get_equip_enhance()
{
    uint32 src = prs->at(PRSTE_TARGET_HALF_BYTE_1).get();
    return get_target_half_byte(src, 24);
}

void player_role_target::equip_reset(uint32 count)
{
    uint32 src = prs->at(PRSTE_TARGET_HALF_BYTE_1).get();
    target_half_byte(src, 28, count);
    prs->at(PRSTE_TARGET_HALF_BYTE_1).set_val(src);
}

uint32 player_role_target::get_equip_reset()
{
    uint32 src = prs->at(PRSTE_TARGET_HALF_BYTE_1).get();
    return get_target_half_byte(src, 28);
}

void player_role_target::equip_quality(uint32 count)
{
    uint32 src = prs->at(PRSTE_TARGET_HALF_BYTE_2).get();
    target_half_byte(src, 0, count);
    prs->at(PRSTE_TARGET_HALF_BYTE_2).set_val(src);
}

uint32 player_role_target::get_equip_quality()
{
    uint32 src = prs->at(PRSTE_TARGET_HALF_BYTE_2).get();
    return get_target_half_byte(src, 0);
}

void player_role_target::equip_sublime(uint32 count)
{
    uint32 src = prs->at(PRSTE_TARGET_HALF_BYTE_2).get();
    target_half_byte(src, 4, count);
    prs->at(PRSTE_TARGET_HALF_BYTE_2).set_val(src);
}

uint32 player_role_target::get_equip_sublime()
{
    uint32 src = prs->at(PRSTE_TARGET_HALF_BYTE_2).get();
    return get_target_half_byte(src, 4);
}

void player_role_target::lnlaid_stone(uint32 count)
{
    uint32 src = prs->at(PRSTE_TARGET_HALF_BYTE_2).get();
    target_half_byte(src, 8, count);
    prs->at(PRSTE_TARGET_HALF_BYTE_2).set_val(src);
}

uint32 player_role_target::get_lnlaid_stone()
{
    uint32 src = prs->at(PRSTE_TARGET_HALF_BYTE_2).get();
    return get_target_half_byte(src, 8);
}

void player_role_target::open_pulse()
{
    uint32 src = prs->at(PRSTE_TARGET_HALF_BYTE_2).get();
    target_half_byte(src, 12);
    prs->at(PRSTE_TARGET_HALF_BYTE_2).set_val(src);
}

uint32 player_role_target::get_open_pulse()
{
    uint32 src = prs->at(PRSTE_TARGET_HALF_BYTE_2).get();
    return get_target_half_byte(src, 12);
}

void player_role_target::high_explore(uint32 num)
{
    uint32 src = prs->at(PRSTE_TARGET_HALF_BYTE_2).get();
    target_half_byte(src, 16, num);
    prs->at(PRSTE_TARGET_HALF_BYTE_2).set_val(src);
}

uint32 player_role_target::get_high_explore()
{
    uint32 src = prs->at(PRSTE_TARGET_HALF_BYTE_2).get();
    return get_target_half_byte(src, 16);
}

void player_role_target::join_battle()
{
    uint32 src = prs->at(PRSTE_TARGET_BYTE).get();
    target_byte(src, 8);
    prs->at(PRSTE_TARGET_BYTE).set_val(src);
}

uint32 player_role_target::get_join_battle()
{
    uint32 src = prs->at(PRSTE_TARGET_BYTE).get();
    return get_target_byte(src, 8);
}

void player_role_target::use_trans()
{
    uint32 src = prs->at(PRSTE_TARGET_BYTE).get();
    target_byte(src, 0);
    prs->at(PRSTE_TARGET_BYTE).set_val(src);
}

uint32 player_role_target::get_use_trans()
{
    uint32 src = prs->at(PRSTE_TARGET_BYTE).get();
    return get_target_byte(src, 0);
}

void player_role_target::pet_advanced(uint32 lvl)
{
    uint32 src = prs->at(PRSTE_TARGET_BYTE).get();
    target_byte(src, 16, lvl);
    prs->at(PRSTE_TARGET_BYTE).set_val(src);
}

uint32 player_role_target::get_pet_advanced()
{
    uint32 src = prs->at(PRSTE_TARGET_BYTE).get();
    return get_target_byte(src, 16);
}

bool player_role_target::target_bit(uint32& src, uint32 index)
{
    if ((src & (0x00000001 << index)) == 0) {
        src |= (0x00000001 << index);
        return true;
    }
    
    return false;
}

uint32 player_role_target::get_target_bit(uint32 src, uint32 index)
{
    return (src >> index) & 0x00000001;
}

uint32 player_role_target::target_half_byte(uint32& src, uint32 index, uint32 val)
{
    uint32 old = (src >> index) & 0x0000000F;
    val = (old + val) < 0x0000000F ? (old + val) : 0x0000000F;
    src -= (old << index);
    src |= (val << index);
    
    return val;
}

uint32 player_role_target::get_target_half_byte(uint32 src, uint32 index)
{
    return (src >> index) & 0x0000000F;
}

uint32 player_role_target::target_byte(uint32& src, uint32 index, uint32 val)
{
    uint32 old = (src >> index) & 0x000000FF;
    val = (old + val) < 0x000000FF ? (old + val) : 0x000000FF;
    src -= (old << index);
    src |= (val << index);
    
    return val;
}

uint32 player_role_target::get_target_byte(uint32 src, uint32 index)
{
    return (src >> index) & 0x000000FF;
}
