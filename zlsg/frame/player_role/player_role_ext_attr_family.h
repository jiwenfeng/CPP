#ifndef __player_role_ext_attr_family_h__
#define __player_role_ext_attr_family_h__

#include "client.family.pb.h"
#include "player_role_ext_attr.h"

class player_role_ext_attr_family : public player_role_ext_attr
{
public:
    //void fill(client::family_role_attr_rsp& rsp);

    uint32 get_lv(uint32 type);

    void add(uint32 type, uint32* attr);
};

#endif // __player_role_ext_attr_family_h__
