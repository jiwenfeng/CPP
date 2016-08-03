#ifndef __WORLDBOSS_SCENE_H__
#define __WORLDBOSS_SCENE_H__

#include "scene.h"

class boss_scene : public scene
{
public:
    boss_scene(const config_map* p, const config_raid* cr);
    virtual ~boss_scene();

public:
    virtual int init();

public:
};

#endif // __WORLDBOSS_SCENE_H__
