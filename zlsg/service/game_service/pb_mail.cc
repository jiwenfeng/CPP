#include "game_interface.h"
#include "player_role.h"
#include "player_role_mail.h"
#include "scene_user.h"
#include "scene_inst.h"

// 接受新的邮件
void* on_pb_delivery_mail_info(client::delivery_mail_info* dmi, void* ud, size_t* rsp_len)
{
    struct game_service_t* ss = (struct game_service_t*)ud;
    uint32 role_id = dmi->mi().mh().recv_role_id();
    scene_user* su = ss->sim->find_user(role_id);
    if (su == NULL){
        return NULL;
    }

    uint32 mail_id = su->get_player_role().get_role_mails().add_mail(role_id, dmi->mi());

    client::new_mail_recv new_mail_rsp;
    client::mail_head* mh = new_mail_rsp.add_mh();
    mh->CopyFrom(dmi->mi().mh());
    mh->set_mail_id(mail_id);
    su->send_client(ss->ctx, new_mail_rsp);
	
    amtsvr_billlog(ss->ctx, "%u|%s|%u", su->get_role_id(), dmi->GetTypeName().c_str(),mail_id);

    return NULL;
}

//---------------------------------------------------------------
// 邮件列表请求
void* on_pb_mail_list_req(client::mail_list_req* mlr, void* ud, size_t* rsp_len)
{
    struct game_service_t* ss = (struct game_service_t*)ud;
    scene_user* su = ss->sim->get_user(mlr->svr_head().role_id());
    if (su == NULL){
        return NULL;
    }
    
    client::mail_list_rsp mail_rsp;
    su->get_player_role().get_role_mails().proc_mail_list_req(mail_rsp);
    su->send_client(ss->ctx, mail_rsp);

	amtsvr_billlog(ss->ctx, "%u|%s", su->get_role_id(), mlr->GetTypeName().c_str());

    return NULL;
}
// 写信
void* on_pb_send_mail_req(client::send_mail_req* mlr, void* ud, size_t* rsp_len)
{
    struct game_service_t* ss = (struct game_service_t*)ud;
    scene_user* su = ss->sim->get_user(mlr->svr_head().role_id());
    if (su == NULL){
        return NULL;
    }

    // check name
    client::send_mail_check_role sm_cr;
    sm_cr.mutable_smr()->CopyFrom(*mlr);
    su->send_center(ss->ctx, sm_cr);

    //amtsvr_billlog(ss->ctx, "%d|%s|%s|%d|%s|%s|%d|%d%s", su->get_role_id(), mlr->GetTypeName().c_str(), mlr->recv_name().c_str(), mlr->recv_role_id(), mlr->title().c_str(), mlr->mb().content().c_str(), mlr->tael(), mlr->ingot(), attachment.c_str());
    
    return NULL;
}

int make_send_mail(scene_user* psu, client::send_mail_req* smreq, client::send_mail_rsp& sm_rsp, client::delivery_mail_info& dmi)
{
    const ::client::mail_info& mail_info = smreq->info();
    client::mail_info* info = dmi.mutable_mi();
    uint32 now = (uint32)time(NULL);

    // 填充邮件的头部
    client::mail_head* mh = info->mutable_mh();
    mh->set_mail_id(mail_info.mh().mail_id());
    mh->set_mail_type(mail_info.mh().mail_type());
    mh->set_recv_role_id(mail_info.mh().recv_role_id());
    mh->set_send_role_id(mail_info.mh().send_role_id());
    mh->set_send_role_name(mail_info.mh().send_role_name());
    mh->set_title(mail_info.mh().title());
    mh->set_send_time(now);
	bool has_adjuct = false;
	if (mail_info.mh().attachment_num() != 0){
		has_adjuct = true;
	}
	mh->set_past_time(psu->get_mail_past_time(mail_info.mh().mail_type(),has_adjuct));
    mh->set_attachment_num(mail_info.mh().attachment_num());
    mh->set_opened(mail_info.mh().opened());
    dmi.set_recv_name(smreq->recv_name());

    // 填充邮件内容mail_body
    if (mail_info.has_mb() && mail_info.mb().has_content() && mail_info.mb().content().size() > 0) {
        client::mail_body* mb =	info->mutable_mb();
        mb->set_content(mail_info.mb().content());
    }

    for (int j = 0; j < mail_info.attr_size(); j++) {
        client::mail_attr* attr = info->mutable_attr(j);
        attr->set_attr_type(mail_info.attr(j).attr_type());
        attr->set_attr_value(mail_info.attr(j).attr_value());
    }

    // 填充邮件的附件
    for (int32 i = 0; i < info->ma_size(); i++) {
        client::props_info* item_info = info->add_ma();
        item_info->CopyFrom(mail_info.ma(i));
    }

    sm_rsp.set_ret(send_mail_rsp_SEND_RET_SRET_SUCCESS);

    return 0;
}

// center_server发回检测完成发给收信人to center_server
void* on_game_pb_send_mail_check_complete(client::send_mail_check_complete* req, void* ud, size_t* rsp_len)
{
    client::send_mail_req* mlr = req->mutable_smr();
    struct game_service_t* ss = (struct game_service_t*)ud;
    scene_user* su = ss->sim->get_user(mlr->svr_head().role_id());
    if (su == NULL){
        return NULL;
    }

	client::send_mail_rsp sm_rsp;
    if (req->pass()) {
        client::delivery_mail_info dmi;
        if (make_send_mail(su, mlr, sm_rsp, dmi) == 0){
			// 由center_server 发送邮件出去
            su->send_center(ss->ctx, dmi);
        }
		
        su->send_client(sm_rsp);
    } else {
        client::send_mail_rsp sm_rsp;
        sm_rsp.set_ret(client::send_mail_rsp_SEND_RET_SRET_RECV_ERR);
        su->send_client(sm_rsp);
    }

    return NULL;
}
// 打开邮件
void* on_pb_mail_open_req(client::mail_open_req* mor, void* ud, size_t* rsp_len)
{
    struct game_service_t* ss = (struct game_service_t*)ud;
    scene_user* su = ss->sim->get_user(mor->svr_head().role_id());
    if (su == NULL){
        return NULL;
    }

    client::mail_open_rsp mo_rsp;
    if (su->get_player_role().get_role_mails().proc_mail_open_req(mor->mail_id(), mo_rsp)){
		su->send_client(mo_rsp);
    }

	string ma = "";
	char buf[32] = {0};
	int32 size = mo_rsp.ma_size();
	for (int32 i = 0; i < size; ++i) {
		memset(buf, 0, sizeof(buf));
		snprintf(buf, sizeof(buf), "|%u", mo_rsp.ma(i).props_id());
		ma += buf;
	}
    
    amtsvr_billlog(ss->ctx, "%u|%s|%u|%u|%s|%s", su->get_role_id(), mor->GetTypeName().c_str(), mor->mail_id(), mo_rsp.mail_id(), mo_rsp.mb().content().c_str(), ma.c_str());

    return NULL;
}
// 删除邮件请求
void* on_pb_remove_mail_req(client::remove_mail_req* rmr, void* ud, size_t* rsp_len)
{
    struct game_service_t* ss = (struct game_service_t*)ud;
    scene_user* su = ss->sim->get_user(rmr->svr_head().role_id());
    if (su == NULL){
        return NULL;
    }

    client::remove_mail_rsp rm_rsp;
    su->get_player_role().get_role_mails().proc_mail_remove_req(*rmr, rm_rsp);
    su->send_client(rm_rsp);

    amtsvr_billlog(ss->ctx, "%u|%s", su->get_role_id(), rmr->GetTypeName().c_str());

    return NULL;
}
// 提取附件
void* on_pb_extract_attachment_req(client::extract_attachment_req* ear, void* ud, size_t* rsp_len)
{
    struct game_service_t* ss = (struct game_service_t*)ud;
    scene_user* su = ss->sim->get_user(ear->svr_head().role_id());
    if (su == NULL){
        return NULL;
    }

    std::vector<client::props_info> item_info;
    std::vector<client::mail_attr> mail_attr;
	item_info.clear();
	mail_attr.clear();
    client::extract_attachment_rsp ear_rsp;
    player_role_mail& prm = su->get_player_role().get_role_mails();
    uint32 mail_id = ear->mail_id();
    ear_rsp.set_mail_id(mail_id);

	// 如果附件过期，无法领取
	if (!prm.check_attachment_time_out(mail_id)){
		ear_rsp.set_ret(client::extract_attachment_rsp_EXTRACT_RET_ERET_TIME_OUT);
		su->send_client(ear_rsp);
		return NULL;
	}

	int ret = prm.proc_extract_attachment(mail_id, item_info, mail_attr);
    if (ret != 0 || prm.has_take(mail_id)){
        ear_rsp.set_ret(client::extract_attachment_rsp_EXTRACT_RET_ERET_ATTACH_ERR);
		su->send_client(ear_rsp);
		return NULL;
    }

	if (item_info.size() != 0){
		if (!su->add_props(item_info)){
			ear_rsp.set_ret(client::extract_attachment_rsp_EXTRACT_RET_ERET_PACK_FULL);
			su->send_client(ear_rsp);
			return NULL;
		}
	}

	if (mail_attr.size()!= 0){
		// 给人物增加相关属性
		for (size_t j = 0; j < mail_attr.size(); ++j){
			if (static_cast<ROLE_ATTRIBUTE>(mail_attr[j].attr_type()) == EXPERIENCE){
				su->add_normal_exp(static_cast<uint64>(mail_attr[j].attr_value()));
				continue;
			}
			su->add_money(static_cast<ROLE_ATTRIBUTE>(mail_attr[j].attr_type()),mail_attr[j].attr_value());
		}
		su->send_client_change_attr();
	}

	ear_rsp.set_ret(client::extract_attachment_rsp_EXTRACT_RET_ERET_SUCCESS);
	// 设置提取状态
	prm.set_is_take_status(mail_id);

    su->send_client(ear_rsp);

	string ma = "";
	char buf[64] = {0};
	for (size_t i = 0; i < item_info.size(); ++i) {
		memset(buf, 0, sizeof(buf));
		snprintf(buf, sizeof(buf), "|%d|%d", item_info[i].props_id(), item_info[i].count());
		ma += buf;
	}

	ma += "|A";
	for (size_t i = 0; i < mail_attr.size(); ++i) {
		memset(buf, 0, sizeof(buf));
		snprintf(buf, sizeof(buf), "|%d|%d", mail_attr[i].attr_type(), mail_attr[i].attr_value());
		ma += buf;
	}

    amtsvr_billlog(ss->ctx, "%u|%s", su->get_role_id(), ear->GetTypeName().c_str());

    return NULL;
}

