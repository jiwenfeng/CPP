#include "inner.promotions.pb.h"
#include "client.poll_activity.pb.h"
#include "center_interface.h"

void fill_tail(inner::inner_tail* tail);
// 获取后台活动配置的列表
void* on_pb_center_get_promotions_req(inner::get_promotions_req* req, void* ud, size_t* rsp_len)
{
	center_service_t* w = (center_service_t*)ud;

	inner::get_promotions_rsp rsp_msg;

	const std::map<uint32,activity_promotions*>& promotions_map_ = w->poa_mgr->get_promotions_mgr().get_promotions_list();
	std::map<uint32,activity_promotions*>::const_iterator it = promotions_map_.begin();

	for (; it != promotions_map_.end(); ++it){
		inner::promotions* info = rsp_msg.add_ps();
		info->set_id(it->second->get_db_info()->id());
		info->set_name(it->second->get_db_info()->name());
		info->set_type(it->second->get_db_info()->type());
		info->set_start_time(it->second->get_db_info()->start_time());
		info->set_duration_time(it->second->get_db_info()->duration_time());
		info->set_end_time(it->second->get_db_info()->end_time());
		info->set_loop_time(it->second->get_db_info()->loop_time());
		info->set_state(it->second->get_db_info()->state());
	}

	fill_tail(rsp_msg.mutable_tail());
	center_send_inner(w, req->head(), rsp_msg);

	return NULL;
}
// 设置活动开启数据
void* on_pb_center_set_promotions_req(inner::set_promotions_req* req, void* ud, size_t* rsp_len)
{
	center_service_t* w = (center_service_t*)ud;

	inner::set_promotions_rsp rsp_msg;
	for (int i = 0; i < req->ps_size(); ++i){
		w->poa_mgr->reset_activity_open_data(req->ps(i).id(),req->ps(i).type(),req->ps(i).start_time(),req->ps(i).duration_time(),req->ps(i).loop_time());
		w->poa_mgr->get_promotions_mgr().create_new_promotions(req->ps(i).id(),req->ps(i).name(),req->ps(i).type(),req->ps(i).start_time(),
			req->ps(i).duration_time(),req->ps(i).loop_time());
		// 填充返回数据
		inner::promotions* pinfo = rsp_msg.add_ps();
		pinfo->CopyFrom(req->ps(i));
	}

	fill_tail(rsp_msg.mutable_tail());
	center_send_inner(w, req->head(), rsp_msg);

	return NULL;
}
// 停止（启动）活动
void* on_pb_center_del_promotions_req(inner::del_promotions_req* req, void* ud, size_t* rsp_len)
{
	center_service_t* w = (center_service_t*)ud;

	inner::del_promotions_rsp rsp_msg;
	std::map<uint32,activity_promotions*>& promotions_map_ = w->poa_mgr->get_promotions_mgr().mutable_promotions_list();
	// 先设置
	for (int i = 0; i < req->ids_size(); ++i){
		std::map<uint32,activity_promotions*>::iterator it = promotions_map_.find(req->ids(i));
		if (promotions_map_.size() != 0){
			if (it != promotions_map_.end()){
				if (it->second->get_db_info()->state() == BACKGROUND_ACTIVITY_OPEN_TYPE_STOP){
					it->second->set_act_status(BACKGROUND_ACTIVITY_OPEN_TYPE_NONE);
				}else{
					it->second->set_act_status(BACKGROUND_ACTIVITY_OPEN_TYPE_STOP);
				}

				rsp_msg.add_ids(req->ids(i));
			}
		}
	}
	
	fill_tail(rsp_msg.mutable_tail());
	center_send_inner(w, req->head(), rsp_msg);

	return NULL;
}