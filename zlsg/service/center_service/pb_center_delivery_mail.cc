#include "center_interface.h"
#include "config_mail.h"

static void send_to_mail_center(center_service_t* w, uint32 role_id, client::delivery_mail_info* dmi)
{
    w->wg_mcm.push_new_mail(role_id, dmi->mi());
}

// ����ֱ�ӷ���������ֱ�����ʼ�����
void send_new_mail(center_service_t *w,uint32 role_id,const client::mail_info& mi)
{
	const online_role* oi = w->oi_mgr.get_online_info(role_id);
	if (oi == NULL){ 
		w->wg_mcm.push_new_mail(role_id, mi);
		return;
	}

	const mysql::sp_load_all_user_smy* smy = w->aus_mgr.get_user_smy(role_id);
	if (smy == NULL){
		amtsvr_billlog(w->ctx, "send mail failed(user not exist):%u\n", role_id); 
		return;
	}

	client::delivery_mail_info dmi;
	dmi.set_recv_name(smy->role_name());
	client::mail_info* info = dmi.mutable_mi();
	info->CopyFrom(mi);

	msg_handler* handle = MSG_HANDLER_POOL->pop();
	if (handle == NULL){
		amtsvr_billlog(w->ctx, "send mail failed(out of memory):%u\n", role_id); 
		return;
	}

	handle->msg = new client::delivery_mail_info(dmi);
	if (amtsvr_send(w->ctx, 0, oi->scene_svr_id, PTYPE_CLIENT|PTYPE_TAG_DONTCOPY, 0, handle, sizeof(msg_handler)) < 0){
		w->wg_mcm.push_new_mail(role_id, mi);
	}
}
// ��game_server֪ͨcenter_server�ʹ��ʼ�
void* on_pb_delivery_mail(client::delivery_mail_info* dmi, void* ud, size_t* rsp_len)
{
    center_service_t* w = (center_service_t*)ud;

    const mysql::sp_load_all_user_smy* smy = w->aus_mgr.get_user_smy(dmi->recv_name());
    if (smy == NULL){
        return NULL;
    }

    uint32 role_id = smy->role_id();
    dmi->mutable_mi()->mutable_mh()->set_recv_role_id(role_id);
    
    const online_role* oi = w->oi_mgr.get_online_info(role_id);
    if (oi == NULL){ 
        send_to_mail_center(w, role_id, dmi);
        return NULL;
    }

    if (oi->scene_svr_id == 0){
        send_to_mail_center(w, role_id, dmi);
        return NULL;
    }

    msg_handler* mh = MSG_HANDLER_POOL->pop();
    if (mh == NULL){
        return NULL;
    }
    mh->msg = new client::delivery_mail_info(*dmi);
   
    if (amtsvr_send(w->ctx, 0, oi->scene_svr_id, PTYPE_CLIENT|PTYPE_TAG_DONTCOPY, 0, mh, sizeof(msg_handler)) < 0){
        send_to_mail_center(w, role_id, dmi);
        return NULL;
    }
    
    return NULL;
}

// �ʼ���������id������
void* on_center_pb_send_mail_check_role(client::send_mail_check_role* req, void* ud, size_t* rsp_len)
{
    center_service_t* w = (center_service_t*)ud;
    const client::send_mail_req& smr = req->smr();
    const mysql::sp_load_all_user_smy* smy = NULL;
    bool id   = true;
    bool name = true;
    
    // check id
    smy = w->aus_mgr.get_user_smy(smr.info().mh().recv_role_id());
    if (smy == NULL || smr.info().mh().recv_role_id() == 0) {
        id = false;
    }

    // check name
    if (id == false) {
        smy = w->aus_mgr.get_user_smy(smr.recv_name());
        if (smy == NULL) {
            name = false;
        }
    }
	// �����ʼ���������ʼ����Լ�������
    client::send_mail_check_complete rsp;
    rsp.mutable_smr()->CopyFrom(smr);
    rsp.set_pass(id || name);
	if (id || name) {
		rsp.mutable_smr()->mutable_info()->mutable_mh()->set_recv_role_id(smy->role_id());
	}
	// ���ظ��Լ�
    center_send_scene(w, smr.svr_head(), rsp);

    return NULL;
}

void* on_pb_broadcast_msg_req(client::broadcast_msg_req* req, void* ud, size_t* rsp_len)
{
	center_service_t *w = (center_service_t *)ud;
	const config_mail *cm = CONFMGR->get_config_mail_mgr().get_config_mail(req->area_id());
	if(cm == NULL)
	{
		return NULL;
	}
	client::broadcast_area_rsp rsp;
	rsp.set_area_id(req->area_id());
	client::mail_body *mb = rsp.mutable_area_msg();
	mb->set_content(cm->get_content_value());
	for(int i = 0; i < req->data_size(); ++i)
	{
		client::content_data *cd = mb->add_data();
		cd->CopyFrom(req->data(i));
	}

	rsp.set_broadcast_count(cm->get_brod_times());

	send_broadcast_area_msg(w, rsp);
	return NULL;
}
