#include "game_interface.h"
#include "scene_user.h"
#include "scene_inst.h"

void pick_npc(client::pick_req* req, scene_user* psu);

void* on_pb_pick_req(client::pick_req* req, void* ud, size_t* rsp_len)
{
    PB_FUNC_GUARD(req);

    pick_npc(req, psu);

    return NULL;
}

void pick_npc(client::pick_req* req, scene_user* psu)
{
	scene_inst *si = psu->get_scene();
	if(NULL == si)
	{
		return;
	}

    client::pick_rsp rsp;
	rsp.set_role_id(psu->get_role_id());
	rsp.set_action(req->pi().action());
    switch (req->pi().action()) {
    case client::PAE_ADD: {
        if (!req->pi().has_rd()) {
            rsp.set_ret(client::PIRE_PARAMETER_ERROR);
			rsp.set_inst_id(req->pi().rd().inst_id());
            psu->send_client(rsp);
            break;
        }

        si->on_pick(psu, req->pi().rd().inst_id());

        break;
        }
    case client::PAE_COMMIT: {
        rsp.set_ret(psu->pick_commit());
		rsp.set_inst_id(req->pi().rd().inst_id());
        psu->send_client(rsp);
        break;
        }
    default:
        rsp.set_ret(client::PIRE_PARAMETER_ERROR);
		rsp.set_inst_id(req->pi().rd().inst_id());
        psu->send_client(rsp);
        break;
    }
}
