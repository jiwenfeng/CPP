#ifndef __INNER_INTERFACE_H__
#define __INNER_INTERFACE_H__

#include "agent_interface.h"

namespace inner {
    class addNotice_req;
    class getNoticeList_req;
    class updateNotice_req;
    class delNotice_req;
};

void* on_pb_addNotice_req(inner::addNotice_req* req, void* ud, size_t* rsp_len);
void* on_pb_getNoticeList_req(inner::getNoticeList_req* req, void* ud, size_t* rsp_len);
void* on_pb_updateNotice_req(inner::updateNotice_req* req, void* ud, size_t* rsp_len);
void* on_pb_delNotice_req(inner::delNotice_req* req, void* ud, size_t* rsp_len);

#endif
