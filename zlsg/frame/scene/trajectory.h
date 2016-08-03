#ifndef __TRAJECTORY_H__
#define __TRAJECTORY_H__

#include "scene_define.h"

class trajectory {
public:
    trajectory(uint64 id, uint64 caster_id, const config_skill* skill, const map_postion& pos);
    ~trajectory();

public:
    int init(uint64 id, uint64 caster_id, const config_skill* skill, const map_postion& pos);

public:
    uint64 get_id()        const { return _id       ; }
    uint64 get_caster_id() const { return _caster_id; }
    const config_skill* get_config_skill() const { return _skill; }
    const map_postion& get_pos() const { return _pos; }

public:
    virtual int on_update(uint32 tick);

private:
    uint64              _id;
    uint64              _caster_id;
    const config_skill* _skill;
    map_postion         _pos;

    uint32              _start_tick;
};

#endif // __TRAJECTORY_H__

