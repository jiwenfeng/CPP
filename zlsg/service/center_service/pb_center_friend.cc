#include "center_interface.h"

// 请求关注列表
void* on_pb_friend_list_req(client::friend_list_req* flr, void* ud, size_t* rsp_len)
{
	center_service_t* w = (center_service_t*)ud;
	client::friend_list_rsp rsp;
	rsp.set_list_type(client::FLRE_LIST);
	const player_role_friends* prf = w->fm_mgr.get(flr->svr_head().role_id());
	if (prf != NULL){
		const map<uint32, client::friend_info> friend_map_ = prf->get_friend_list();
		map<uint32, client::friend_info>::const_iterator it = friend_map_.begin();
		for (; it != friend_map_.end(); ++it){
			client::friend_info* fi = rsp.add_fi();
			fi->CopyFrom(it->second);
			const mysql::sp_load_all_user_smy* smy_info = w->aus_mgr.get_user_smy(it->second.role_id());
			if (smy_info != NULL){
				fi->set_vip_level(smy_info->vip_level());
				fi->set_friend_level(smy_info->level());
				player_role_friends* fu = w->fm_mgr.mut(flr->svr_head().role_id());
				if (fu != NULL){
					fu->update_friend_data(it->second.role_id(),smy_info->level(),smy_info->vip_level());
				}
			}
		}
		/*prf->fill_friend_list_rsp(rsp);*/
	}

	center_send_client(w, flr->svr_head(), rsp);

	return NULL;
}
// 请求临时好友列表
void* on_pb_temporary_friend_list_req(client::temporary_friend_list_req* flr, void* ud, size_t* rsp_len)
{
	center_service_t* w = (center_service_t*)ud;
	client::friend_list_rsp rsp;
	rsp.set_list_type(client::FLRE_LIST);
	const player_role_friends* prf = w->fm_mgr.get(flr->svr_head().role_id());
	if (prf != NULL){
		const std::map<uint32, client::friend_info> friend_map_ = prf->get_friend_list();
		std::map<uint32, client::friend_info>::const_iterator it = friend_map_.begin();
		for (; it != friend_map_.end(); ++it){
			if (it->second.relation_type() == friend_info_friend_type_real_friend){
				continue;
			}

			client::friend_info* fi = rsp.add_fi();
			fi->CopyFrom(it->second);
			const mysql::sp_load_all_user_smy* smy_info = w->aus_mgr.get_user_smy(it->second.role_id());
			if (smy_info != NULL){
				fi->set_vip_level(smy_info->vip_level());
				fi->set_friend_level(smy_info->level());
			}
		}
	}

	center_send_client(w, flr->svr_head(), rsp);

	return NULL;
}
// 关注某个玩家
void* on_pb_friend_add_req(client::friend_add_req* req, void* ud, size_t* rsp_len)
{
	client::friend_add_rsp rsp;
	center_service_t* w = (center_service_t*)ud;
	do {
		uint32 src_role_id = req->svr_head().role_id();
		uint32 dst_role_id = req->role_id();
		if (src_role_id == 0 || dst_role_id == 0){
			rsp.set_ret(client::CFORE_SYS_ERR);
			break;
		}

		if (!w->fm_mgr.is_aready_friend(src_role_id,dst_role_id)){
			rsp.set_ret(client::CRORE_ALREAD_FRIEND);
			break;
		}

		if (!w->fm_mgr.can_add_friend(src_role_id, dst_role_id)){
			rsp.set_ret(client::CRORE_FRIEND_NUM_LIMIT);
			break;
		}

		const online_role* oi = w->oi_mgr.get_online_info(dst_role_id);
		if (oi == NULL){ 
			rsp.set_ret(client::CRORE_FRIEDN_OFFLINE);
			break;
		}

		const mysql::sp_load_all_user_smy* smy = w->aus_mgr.get_user_smy(dst_role_id);
		if (smy == NULL){
			break;
		}
		if (w->fm_mgr.add_friend(src_role_id, dst_role_id,smy->level(),smy->role_name(),smy->vip_level()) != 0){
			rsp.set_ret(client::CFORE_SYS_ERR);
			break;
		}

		player_role_friends* src_friends = w->fm_mgr.mut(src_role_id);
		if (src_friends != NULL){
			client::friend_list_rsp src_add_rsp;
			src_add_rsp.set_list_type(FLRE_ADD);
			src_friends->fill_add_friend_info(src_add_rsp);
			center_send_client(w, req->svr_head(), src_add_rsp);
		}

		rsp.set_ret(CFORE_SUCCESS);
	} while (false);
	rsp.set_role_id(req->role_id());
	//center_send_scene(w, req->svr_head(), *req);
	center_send_client(w, req->svr_head(), rsp);

	return NULL;
}

void* on_pb_friend_add_ret(client::friend_add_ret* req, void* ud, size_t* rsp_len)
{
	center_service_t* w = (center_service_t*)ud;
	uint32 src_role_id = req->svr_head().role_id();
	uint32 dst_role_id = req->role_id();
	if (src_role_id == 0 || dst_role_id == 0){
		return NULL;
	}

	client::friend_add_rsp rsp;
	do{
		const online_role* oi = w->oi_mgr.get_online_info(dst_role_id);
		if (oi == NULL){ 
			rsp.set_ret(client::CRORE_FRIEDN_OFFLINE);
			break;
		}

		if (!req->allow()){
			rsp.set_ret(client::CFORE_DENY_ADD);
			rsp.set_role_id(src_role_id);
			center_send_client(w, oi->client_id, rsp);
			return NULL;
		}

		if (!w->fm_mgr.can_add_friend(src_role_id, dst_role_id)){
			rsp.set_ret(client::CRORE_FRIEND_NUM_LIMIT);
			break;
		}

		//if (w->fm_mgr.add_friend(src_role_id, dst_role_id) != 0){
		//	rsp.set_ret(client::CFORE_SYS_ERR);
		//	break;
		//}

		player_role_friends* src_friends = w->fm_mgr.mut(src_role_id);
		if (src_friends != NULL){
			client::friend_list_rsp src_add_rsp;
			src_friends->fill_add_friend_info(src_add_rsp);
			center_send_client(w, req->svr_head(), src_add_rsp);
		}

		player_role_friends* dst_friends = w->fm_mgr.mut(dst_role_id);
		if (dst_friends != NULL){
			client::friend_list_rsp dst_add_rsp;
			dst_friends->fill_add_friend_info(dst_add_rsp);
			center_send_client(w, oi->client_id, dst_add_rsp);
		}
		return NULL;
	} while(false);

	center_send_client(w, req->svr_head(), rsp);
	return NULL;
}

static void del_friends_ex(center_service_t* w, uint32 src_role_id, uint32 dst_role_id)
{
	if (src_role_id == 0 || dst_role_id == 0){
		return;
	}

	w->fm_mgr.del_friend(src_role_id, dst_role_id);
	
	//const online_role* oi = w->oi_mgr.get_online_info(dst_role_id);
	//if (oi == NULL){ 
	//	return;
	//}

	//player_role_friends* dst_friends = w->fm_mgr.mut(dst_role_id);
	//if (dst_friends != NULL){
	//	client::friend_list_rsp dst_add_rsp;
	//	dst_friends->fill_del_friend_info(dst_add_rsp);
	//	center_send_client(w, oi->client_id, dst_add_rsp);
	//}
}

void* on_pb_friend_del_req(client::friend_del_req* fdr, void* ud, size_t* rsp_len)
{
	center_service_t* w = (center_service_t*)ud;
	uint32 src_role_id = fdr->svr_head().role_id();
	for (int i = 0; i < fdr->role_id_size(); ++i){
		del_friends_ex(w, src_role_id, fdr->role_id(i));
	}

	player_role_friends* src_friends = w->fm_mgr.mut(src_role_id);
	if (src_friends != NULL){
		if (src_friends->get_vec_del_friends_size() != 0){
			client::friend_list_rsp src_add_rsp;
			src_friends->fill_del_friend_info(src_add_rsp);
			center_send_client(w, fdr->svr_head(), src_add_rsp);
		}

		if (src_friends->get_vec_vec_tem_del_friends_size() != 0){
			client::friend_list_rsp src_add_rsp;
			src_friends->fill_del_tem_friend_info(src_add_rsp);
			center_send_client(w, fdr->svr_head(), src_add_rsp);
		}
	}


	return NULL;
}

void* on_pb_friend_blk_req(client::friend_blk_req* fdr, void* ud, size_t* rsp_len)
{
	center_service_t* w = (center_service_t*)ud;
	uint32 src_role_id = fdr->svr_head().role_id();
    player_role_friends* src_friends = w->fm_mgr.mut(src_role_id);
    if (src_friends == NULL) {
        for (int i = 0; i < fdr->role_id_size(); ++i){
            w->fm_mgr.blk_friend(src_role_id, fdr->role_id(i));
        }

        src_friends = w->fm_mgr.mut(src_role_id);
        if (src_friends == NULL) {
            return NULL;
        }

        client::friend_list_rsp src_add_rsp;
		src_friends->fill_blk_friend_info(src_add_rsp);
		center_send_client(w, fdr->svr_head(), src_add_rsp);

        return NULL;
    }
    
	for (int i = 0; i < fdr->role_id_size(); ++i){
        src_friends->blk_friend(fdr->role_id(i));
    }

	if (src_friends != NULL){
		client::friend_list_rsp src_add_rsp;
		src_friends->fill_blk_friend_info(src_add_rsp);
		center_send_client(w, fdr->svr_head(), src_add_rsp);
	}

	return NULL;
}

void* on_pb_friend_status_req(client::friend_status_req* req, void* ud, size_t* rsp_len)
{
    center_service_t* w = (center_service_t*)ud;

    client::friend_status_rsp rsp;
    int32 size = req->role_id_size();
    for (int32 i = 0; i < size; ++i) {
        const online_role* oi = w->oi_mgr.get_online_info(req->role_id(i));
        client::friend_status_info* fsi = rsp.add_fsi();
        fsi->set_role_id(req->role_id(i));
        if (oi == NULL){ 
            fsi->set_info(0);
        } else {
            fsi->set_info(1);
        }
    }
    
    center_send_client(w, req->svr_head(), rsp);

    return NULL;
}

void* on_pb_friend_degree_add(client::friend_degree_add* req, void* ud, size_t* rsp_len)
{
    center_service_t* w = (center_service_t*)ud;

    center_add_friends_degree(w, req->role_id(), req->friend_role_id(), req->degree_val());

    return NULL;
}
