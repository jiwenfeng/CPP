#include "battle_scene.h"

battle_scene::battle_scene(const config_map* p, const config_raid* cr) : scene(p, cr)
{
}

battle_scene::~battle_scene()
{
}

int battle_scene::init()
{
    return scene::init();
}

bool battle_scene::open_time(uint32 ot)
{
    return false;
}
