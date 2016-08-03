#include "skill_effect.h"


skill_effect::skill_effect()
{
    _cs._skill_id = 0;
}

skill_effect::skill_effect(const config_skill& s) 
    : _cs(s)
{
}

skill_effect::~skill_effect()
{
}

void skill_effect::init_effect()
{
}
