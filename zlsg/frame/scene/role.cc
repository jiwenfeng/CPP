#include "role.h"
#include <math.h>

role::role()
{
    _role_id = 0;
    memset(_role_name, 0, sizeof(_role_name));
    set_role_type(0);
    set_feature(0);
    _scene_id  = 0;
    _map_id    = 0;
    _session   = 0;
    _attr.set_modify_attr(&_modify_attr);
}

role::role(const role& rhs)
{
    operator=(rhs);
}

role::~role()
{
}

role& role::operator=(const role& rhs)
{
    set_role_id  (rhs.get_role_id  ());
    set_role_type(rhs.get_role_type());
    set_feature  (rhs.get_feature  ());
    set_scene_id (rhs.get_scene_id ());
    set_map_id   (rhs.get_map_id   ());
    set_born_pos (rhs.get_born_pos ());

    _session = rhs._session;

    _rsk  = rhs._rsk;
    _buf  = rhs._buf;

    _attr = rhs.get_attr();

    return *this;
}

uint32 role::get_speed()
{
    return _attr.move_speed();
}

float role::get_speedf()
{
    return (float)_attr.move_speed();
}

uint32 role::get_attack_range(skill* s) const
{
    if (s == NULL) {
        return 1;
    }

    return s->range;
}

int role::skill_cost(skill* sk)
{
    if (sk == NULL) {
        return -1;
    }

    //if (sk->cost[SK_COST_LIFE].ratio > 0 || sk->cost[SK_COST_LIFE].val > 0) {
    //    int ratio = (sk->cost[SK_COST_LIFE].ratio / 100.0f) * get_attr()->get_val(MAX_HP);
    //    int val   = sk->cost[SK_COST_LIFE].val;
    //    if (get_attr().get_val(CUR_HP) > (ratio + val)) {
    //        mutable_attr()->set_val(CUR_HP, get_attr().get_val(CUR_HP) - (ratio + val));
    //    } else {
    //        mutable_attr()->set_val(CUR_HP, 1);
    //    }
    //}

    if (sk->cost[SK_COST_ANGER].ratio > 0 || sk->cost[SK_COST_ANGER].val > 0) {
        uint32 ratio = (uint32)((sk->cost[SK_COST_ANGER].ratio / 100.0f) * get_attr().get_val(ANGER));
        uint32 val   = sk->cost[SK_COST_ANGER].val;
        if (get_attr().get_val(ANGER) >= (ratio + val)) {
            mutable_attr()->set_val(ANGER, get_attr().get_val(ANGER) - (ratio + val));
        } else {
            return -1;
        }
    }

    return 0;
}

void role::operate_attr(uint32 calculate_type, ROLE_ATTRIBUTE type, uint32 val)
{
    switch (calculate_type)
    {
    case CALCULATION_ADD:
        {
            _attr.add_val(type, val);
        }
        break;
    case CALCULATION_SUB:
        {
            _attr.sub_val(type, val);
        }
        break;
    case CALCULATION_MULTI:
        {
            float fval = ((float)val) / 100.0f;
            _attr.multi_val(type, fval);
        }
        break;
    case CALCULATION_DIVIDE:
        {
            _attr.divide_val(type, val);
        }
        break;
    default :
        break;
    }
}
