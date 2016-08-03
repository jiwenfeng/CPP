#include "game_interface.h"
#include "boss_scene_inst.h"
#ifndef _WIN32
#include <unistd.h>
#endif


void *on_pb_boss_refresh_req(client::boss_refresh_req *req, void *ud, size_t *req_len)
{
	game_service_t *ss = (game_service_t *)ud;
	scene_inst *si = ss->sim->get_boss_scene();
	if(NULL == si)
	{
		return NULL;
	}
	boss_scene_inst *bsi = static_cast<boss_scene_inst *>(si);
	if(NULL == bsi)
	{
		return NULL;
	}

    //int ret   = 1;
    //do {
    //    ret = __sync_lock_test_and_set(bsi->get_lock(), 1);
    //    if (ret != 0) {
    //        usleep(10);
    //    }
    //} while (ret != 0);

	bsi->refresh_boss();

    //__sync_lock_release(bsi->get_lock());

	return NULL;
}

