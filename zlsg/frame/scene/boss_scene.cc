#include "boss_scene.h"

boss_scene::boss_scene(const config_map* p, const config_raid* cr) : scene(p, cr)
{
}

boss_scene::~boss_scene()
{
}

int boss_scene::init()
{
    return scene::init();
}
