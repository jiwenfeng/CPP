#include "role_effect.h"

role_effect::role_effect()
{
}

role_effect::~role_effect()
{
}

bool role_effect::add_effect(const config_effect* _ce)
{
    return true;
}

bool role_effect::is_trigger(uint32 probability)
{
    return mtrandom::rand_mid32(0, 10000) < probability;
}

void role_effect::set_freeze(const std::vector<int>& vfl)
{

}