#include "game_interface.h"
#include "scene_user.h"
#include "scene_inst.h"

void* on_pb_randnpc(client::randnpc* rn, void* ud, size_t* rsp_len)
{
    struct game_service_t* ss = (struct game_service_t*)ud;
    ss->sim->create_randnpc(rn);
    return NULL;
}



