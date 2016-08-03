#include "config_npc.h"
#include "origin_npc.h"
#include "npc.h"

npc::npc(const npc& rhs) : role(rhs)
{
    //origin_self = rhs.origin_self;
}

npc::~npc()
{

}
