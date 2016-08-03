#ifndef __ROLE_EFFECT_H__
#define __ROLE_EFFECT_H__

#include "scene_define.h"

enum ROLE_EFFECT_TYPE {
    MOVE_POSITION = 0,
};

class role_effect {
public:
    role_effect();
    ~role_effect();

public:
    bool add_effect(const config_effect* _ce);

private:
    bool is_trigger(uint32 probability);
    void set_freeze(const std::vector<int>& vfl);

};

#endif // __ROLE_EFFECT_H__