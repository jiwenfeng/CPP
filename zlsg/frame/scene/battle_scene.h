#ifndef __BATTLE_SCENE_H__
#define __BATTLE_SCENE_H__

#include "scene.h"

class battle_scene : public scene
{
public:
    battle_scene(const config_map* p, const config_raid* cr);
    virtual ~battle_scene();

public:
    virtual int init();
    bool open_time(uint32 ot);

public:
};

#endif // __BATTLE_SCENE_H__
