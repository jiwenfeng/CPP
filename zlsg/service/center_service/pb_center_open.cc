#include "center_interface.h"
#include "arena_mgr.h"
#include "client.open.pb.h"

void * on_pb_center_open_func_req(client::open_func_req *req, void *ud, size_t *rsp_len)
{
	center_service_t *w = (center_service_t *)ud;
	for(int i = 0; i < req->id_size(); ++i)
	{
		switch(req->id(i).id())
		{
			case 7:
				w->ar_mgr.add_role(req->role_id(), 0);
			default:
				break;
		}
	}
	return NULL;
}
