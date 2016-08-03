#include "scene_role.h"
#include "effect_result.h"

effect_result::effect_result()
{
    _atk_effect   = false;
    _effect_times = 1;
    _skill_id     = 0;

    _atk_type     = SAETE_NULL;

    _sk_base_effect_val = 0;
    _sk_grow_effect_val = 0;
    _ef_effect_val      = 0;    
    _effect_total_val   = 0; 

    _crit_ratio    = 0;
    _actual_damage = 0;
}

effect_result::~effect_result()
{
}

void effect_result::add_miss_result(scene_role& effect_role)
{
    client::fight_data* fd = add_fd();
    effect_role.fill_role_data(*fd->mutable_dst_rd());
    fd->set_fight_type(client::fight_data_TYPE_attk_miss);
    fd->set_critical(0);
    fd->set_data(0);
}

void effect_result::add_dodge_result(scene_role& effect_role)
{
    client::fight_data* fd = add_fd();
    effect_role.fill_role_data(*fd->mutable_dst_rd());
    fd->set_fight_type(client::fight_data_TYPE_attk_dodge);
    fd->set_critical(0);
    fd->set_data(0);
}

void effect_result::add_buff_result(scene_role& effect_role)
{
    client::fight_data* fd = add_fd();
    effect_role.fill_role_data(*fd->mutable_dst_rd());
    fd->set_fight_type(client::fight_data_TYPE_attk_buff);
    fd->set_critical(0);
    fd->set_data(0);
}

void effect_result::add_normal_result(scene_role& effect_role, uint32 critical, uint32 data)
{
    client::fight_data* fd = add_fd();
    effect_role.fill_role_data(*fd->mutable_dst_rd());
    fd->set_fight_type(client::fight_data_TYPE_attk_normal);
    fd->set_critical(critical);
    fd->set_data(data);
}

void effect_result::add_metal_result(scene_role& effect_role, uint32 critical, uint32 data)
{
    client::fight_data* fd = add_fd();
    effect_role.fill_role_data(*fd->mutable_dst_rd());
    fd->set_fight_type(client::fight_data_TYPE_attk_metal);
    fd->set_critical(critical);
    fd->set_data(data);
}

uint32 effect_result::get_result_sum()
{
    uint32 sum = 0;
    int count  = fd_size();
    for (int i = 0; i < count; ++i) {
        sum += fd(i).data();
    }

    return sum;
}

void effect_result::set_atk_type(int attr_type)
{
    if (attr_type == ATTR_ATTACK) {
        _atk_type = SAETE_PHYSICAL;
    } else if (attr_type == ATTR_METAL_ATTK) {
        _atk_type = SAETE_FIVEXING;
    } else {
        _atk_type = SAETE_NULL;
    }
}
