#include "trajectory.h"

trajectory::trajectory(uint64 id, uint64 caster_id, const config_skill* skill, const map_postion& pos)
{
    _start_tick = 0;
    init(id, caster_id, skill, pos);
}

trajectory::~trajectory()
{
}

int trajectory::init(uint64 id, uint64 caster_id, const config_skill* skill, const map_postion& pos)
{
    if (id == 0 || caster_id == 0 || skill == NULL) {
        return -1;
    }

    _id        = id;
    _caster_id = caster_id;
    _skill     = skill;
    _pos       = pos;
  
    return 0;
}   

int trajectory::on_update(uint32 tick)
{
    
    return 0;
}
