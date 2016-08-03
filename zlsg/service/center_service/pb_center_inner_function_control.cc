#include "center_interface.h"

void* on_center_pb_activity_control_req(inner::activity_control_req* req, void* ud, size_t* rsp_len)
{
	center_service_t* w = (center_service_t*)ud;

	inner::activity_control_rsp rsp_msg;

	uint32 result_type = 0;

	result_type = w->poa_mgr->set_activity_open_type(req->activity_id(),req->open_type());


	rsp_msg.set_result_type(result_type);

	rsp_msg.set_open_type(req->open_type());

	center_send_inner(w, req->head(), rsp_msg);

	return NULL;
}