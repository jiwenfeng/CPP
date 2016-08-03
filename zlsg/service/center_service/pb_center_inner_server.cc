#include "inner.server.pb.h"
#include "center_interface.h"

void fill_tail(inner::inner_tail* tail);

void* on_center_pb_close_server(inner::close_server* req, void* ud, size_t* rsp_len)
{
    GAME_EXIT = true;

    return NULL;
}

void* on_pb_center_start_time_req(inner::start_time_req* req, void* ud, size_t* rsp_len)
{
    center_service_t* w = (center_service_t*)ud;

    set_amtsvr_svr_start_time(req->unixtime());

    inner::start_time_rsp rsp;
    rsp.set_unixtime(req->unixtime());
    fill_tail(rsp.mutable_tail());
    center_send_inner(w, req->head(), rsp);

    return NULL;
}

void* on_pb_center_open_gm_req(inner::open_gm_req* req, void* ud, size_t* rsp_len)
{
    center_service_t* w = (center_service_t*)ud;

    if (req->unixtime() == 1) {
        w->_gm = true;
    } else if (req->unixtime() == 0) {
        w->_gm = false;
    }

    inner::open_gm_rsp rsp;
    rsp.set_unixtime(w->_gm ? 1 : 0);
    fill_tail(rsp.mutable_tail());
    center_send_inner(w, req->head(), rsp);

    return NULL;
}