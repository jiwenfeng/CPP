#include "inner.bulletin.pb.h"
#include "center_interface.h"

void fill_tail(inner::inner_tail* tail)
{
    tail->set_status(1);
    tail->set_info("");
    tail->set_data("");
}

void* on_center_pb_addNotice_req(inner::addNotice_req* req, void* ud, size_t* rsp_len)
{
    center_service_t* w = (center_service_t*)ud;
    w->cs_mgr.add_msg(req->message(), req->starttime(), req->endtime(), req->intervaltime());
    
    inner::addNotice_rsp rsp;
    fill_tail(rsp.mutable_tail());
    center_send_inner(w, req->head(), rsp);
    
    return NULL;
}

void* on_center_pb_getNoticeList_req(inner::getNoticeList_req* req, void* ud, size_t* rsp_len)
{
    center_service_t* w = (center_service_t*)ud;
    inner::getNoticeList_rsp rsp;
    std::vector<scheduled_msg>& msg = w->cs_mgr.get_msg();
    size_t size = msg.size();
    for (size_t i = 0; i < size; ++i) {
        inner::notice_info* ni = rsp.add_ni();
        ni->set_id(msg[i].id);
        ni->set_message(msg[i].message);
        ni->set_intervaltime(msg[i].interval / 100);
        ni->set_starttime(msg[i].start_time);
        ni->set_endtime(msg[i].end_time);
    }
    
    fill_tail(rsp.mutable_tail());
    center_send_inner(w, req->head(), rsp);
    
    return NULL;
}

void* on_center_pb_updateNotice_req(inner::updateNotice_req* req, void* ud, size_t* rsp_len)
{
    center_service_t* w = (center_service_t*)ud;
    w->cs_mgr.update_msg(req->ni().id(), req->ni().message(), req->ni().starttime(), req->ni().endtime(), req->ni().intervaltime());

    inner::updateNotice_rsp rsp;
    fill_tail(rsp.mutable_tail());
    center_send_inner(w, req->head(), rsp);

    return NULL;
}

void* on_center_pb_delNotice_req(inner::delNotice_req* req, void* ud, size_t* rsp_len)
{
    center_service_t* w = (center_service_t*)ud;
    w->cs_mgr.del_msg(req->id());

    inner::delNotice_rsp rsp;
    fill_tail(rsp.mutable_tail());
    center_send_inner(w, req->head(), rsp);

    return NULL;
}

void* on_pb_center_bulletin_req(inner::bulletin_req* req, void* ud, size_t* rsp_len)
{
    center_service_t* w = (center_service_t*)ud;

    inner::bulletin_rsp rsp;
    fill_tail(rsp.mutable_tail());
    center_send_inner(w, req->head(), rsp);

    return NULL;
}

void* on_pb_center_revolving_req(inner::revolving_req* req, void* ud, size_t* rsp_len)
{
    center_service_t* w = (center_service_t*)ud;

	// ²åÈë¹ã²¥ÐòÁÐ£º
	w->notic_mgr.add_notice_record(req->contents(),req->start_time(),req->end_time(),req->repeat_time());
	
    inner::revolving_rsp rsp;
    fill_tail(rsp.mutable_tail());
    center_send_inner(w, req->head(), rsp);

    return NULL;
}

void* on_pb_center_get_revolving_req(inner::get_revolving_req* req, void* ud, size_t* rsp_len)
{
	center_service_t* w = (center_service_t*)ud;

	inner::get_revolving_rsp rsp_msg;

	std::map<uint32, mysql::tb_center_notice*>& notic_map_ = w->notic_mgr.get_notice_map();
	std::map<uint32, mysql::tb_center_notice*>::iterator it = notic_map_.begin();
	for (; it != notic_map_.end(); ++it){
		inner::revolving* info = rsp_msg.add_rs();
		info->set_msg_id(it->second->msg_id());
		info->set_contents(it->second->content());
		info->set_start_time(it->second->start_time());
		info->set_end_time(it->second->close_time());
		info->set_repeat_time(it->second->interval_time());
		info->set_is_over(it->second->is_over());
	}


	fill_tail(rsp_msg.mutable_tail());
	center_send_inner(w, req->head(), rsp_msg);

	return NULL;
}

void* on_pb_center_del_revolving_req(inner::del_revolving_req* req, void* ud, size_t* rsp_len)
{
	center_service_t* w = (center_service_t*)ud;

	inner::del_revolving_rsp rsp_msg;

	for (int i = 0; i < req->ids_size(); ++i){
		w->notic_mgr.remove_notice(req->ids(i));
	}

	fill_tail(rsp_msg.mutable_tail());
	center_send_inner(w, req->head(), rsp_msg);

	return NULL;
}

void* on_pb_center_stop_revolving_req(inner::stop_revolving_req* req, void* ud, size_t* rsp_len)
{
	center_service_t* w = (center_service_t*)ud;

	inner::stop_revolving_rsp rsp_msg;

	for (int i = 0; i < req->ids_size(); ++i){
		w->notic_mgr.set_over(req->ids(i));
	}

	fill_tail(rsp_msg.mutable_tail());
	center_send_inner(w, req->head(), rsp_msg);

	return NULL;
}