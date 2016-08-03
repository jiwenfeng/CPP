#include "offline_role_mgr.h"
#include "family_mgr.h"
#include "center_interface.h"
#include "config_mgr.h"
#include "config_legion.h"
#include "config_mail.h"

void update_role_title_msg(center_service_t* w,uint32 role_id);

// op_type 0 移除关系 1添加关系
// re_type 1 同盟 2 敌对
void member_update_relationship(center_service_t *w, uint32 f1, uint32 f2, uint32 op_type, uint32 r_type)
{
	vector<uint32> v;
	w->cf_mgr->family_member_list(f1, v);
	for(size_t i = 0; i < v.size(); ++i){
		const online_role* oi = w->oi_mgr.get_online_info(v[i]);
		if(oi == NULL){
			continue;
		}

		if(r_type == 1){
			client::update_friend_family_rsp rsp;
			rsp.set_role_id(v[i]);
			rsp.set_type(op_type);
			rsp.set_family_id(f2);
			center_send_scene(w, *oi, rsp);
		}

		client::update_family_relation ufr;
		ufr.set_op_type(op_type);
		ufr.set_re_type(r_type);
		ufr.set_id(f2);
		center_send_client(w, *oi, ufr);
	}
}

void send_new_mail(center_service_t *w,uint32 role_id,const client::mail_info& mi);
bool fill_area_data(uint32 area_id,client::mail_body* body_info)
{
	const config_mail* mail_info = CONFMGR->get_config_mail_mgr().get_config_mail(area_id);
	if (mail_info == NULL){
		return false;
	}

	body_info->set_content(mail_info->get_content_value());

	return true;
}
void set_player_attr(center_service_t* w, uint32 role_id, uint32 family_id, uint32 title, uint32 family_level,uint32 donnate = 0u,bool is_update_donate  = false)
{
    const online_role* oi = w->oi_mgr.get_online_info(role_id);
    if (oi == NULL) { // 查询离线用户
		return;
    } else { // 直接下发
        client::set_family_req via;
        via.set_family_id(family_id);
        via.set_title(title);
        via.set_role_id(role_id);
		via.set_family_level(family_level);
		if (is_update_donate){
			via.set_donate(donnate);
		}

        center_send_scene(w, *oi, via);
    }
}

void send_family_title_list(center_service_t* w,uint32 role_id,uint32 update_type,uint32 family_id = 0,bool need_broat = false)
{
	client::family_title_list_rsp rsp_msg;
	const online_role* oi = w->oi_mgr.get_online_info(role_id);
	if (oi != NULL) {
		if (update_type == UPTATE_TYPE_ALL){
			w->cf_mgr->family_title_list(rsp_msg);
		}else if(update_type == UPTATE_TYPE_ADD){
			family* family_info = w->cf_mgr->get_family(family_id);
			if (family_info != NULL){
				w->cf_mgr->family_title(family_id,rsp_msg);
			}
		}else if (update_type == UPTATE_TYPE_DEL){
			client::family_title_info* info = rsp_msg.add_info();
			info->set_family_id(family_id);
		}

		rsp_msg.set_type(update_type);
		// 发送
		if (need_broat){
			w->oi_mgr.brodcast(w->ctx, rsp_msg);
		}else{
			center_send_client(w,*oi,rsp_msg);
		}
	}
}

void send_family_mail(center_service_t *w,uint32 role_id,mysql::tb_family* family_db,uint32 event_type)
{
	client::mail_info mi;
	client::mail_head *mh = mi.mutable_mh();
	client::mail_body *mb = mi.mutable_mb();
	mh->set_mail_id(0);
	mh->set_mail_type(MAIL_TYPE_ARMY);
	mh->set_recv_role_id(role_id);
	mh->set_send_role_id(family_db->id());
	mh->set_send_role_name("system");
	switch(event_type)
	{
	case OPT_FORIGN_TYPE_HOSTILITY:
		{
			mh->set_title("legion_title5");
			mb->set_content("legion_content5");
		}
		break;
	case OPT_FORIGN_TYPE_ALLY:
		{
			mh->set_title("legion_title2");
			mb->set_content("legion_content2");
		}
		break;
	case OPT_FORIGN_TYPE_REJECT:
		{
			mh->set_title("legion_title3");
			mb->set_content("legion_content3");
		}
		break;
	case OPT_FORIGN_TYPE_REMOVE_ALLY:// 解除同盟
		{
			mh->set_title("legion_title4");
			mb->set_content("legion_content4");
		}
		break;
	case OPT_FORIGN_TYPE_REMOVE_HOS:// 解除敌盟
		{
			mh->set_title("legion_title6");
			mb->set_content("legion_content6");
		}
		break;
	default:
		return;
		break;
	}
	mh->set_send_time((uint32)time(NULL));
	mh->set_past_time(get_mail_past_time(MAIL_TYPE_ARMY));
	mh->set_opened(false);
	std::vector<client::content_data> data_vec;
	// 军团名字
	client::content_data data_1;
	data_1.set_s(family_db->name());
	data_vec.push_back(data_1);

	for (uint32 i = 0; i < data_vec.size(); ++i){
		client::content_data* data_info = mb->add_data();
		data_info->set_n(data_vec[i].n());
		data_info->set_s(data_vec[i].s());
		data_info->set_f(data_vec[i].f());
	}

	send_new_mail(w,role_id,mi);
}
// 申请
void send_ally_mail(center_service_t *w,uint32 role_id,mysql::tb_family* family_db,uint32 max_num)
{
	client::mail_info mi;
	client::mail_head *mh = mi.mutable_mh();
	client::mail_body *mb = mi.mutable_mb();
	mh->set_mail_id(0);
	mh->set_mail_type(MAIL_TYPE_FAMILY);
	mh->set_recv_role_id(role_id);
	mh->set_send_role_id(family_db->id());
	mh->set_send_role_name("system");
	mh->set_title("legion_title1");
	mb->set_content("legion_content1");
	mh->set_send_time((uint32)time(NULL));
	mh->set_past_time(get_mail_past_time(MAIL_TYPE_FAMILY));
	mh->set_opened(false);

	std::vector<client::content_data> data_vec;
	// 军团名字
	client::content_data data_1;
	data_1.set_s(family_db->name());
	data_vec.push_back(data_1);
	// 军团战旗等级
	client::content_data data_2;
	data_2.set_n(family_db->lvl());
	data_vec.push_back(data_2);
	// 军团长名字
	client::content_data data_3;
	data_3.set_s(family_db->chief_name());
	data_vec.push_back(data_3);
	// 当前成员数
	client::content_data data_4;
	data_4.set_n(family_db->cur_member_num());
	data_vec.push_back(data_4);
	// 最大成员数
	client::content_data data_5;
	data_5.set_n(max_num);
	data_vec.push_back(data_5);
	// 军团战斗力
	client::content_data data_6;
	data_6.set_n(family_db->power());
	data_vec.push_back(data_6);

	for (uint32 i = 0; i < data_vec.size(); ++i){
		client::content_data* data_info = mb->add_data();
		data_info->CopyFrom(data_vec[i]);
	}

	send_new_mail(w,role_id,mi);
}

void update_power_from_game_server(center_service_t* w,uint32 role_id)
{
	client::update_player_power_to_smy update_msg;
	update_msg.set_role_id(role_id);
	center_send_scene(w,role_id,update_msg);
}
void broadcast_all(center_service_t* w, client::client_chat_msg& cm)
{
    cm.mutable_cm()->set_channel(client::chat_msg_CHANNEL_SYSTEM);
    w->oi_mgr.brodcast(w->ctx, *(cm.mutable_cm()));
}

void broadcast_family(center_service_t* w, uint32 role_id, client::client_chat_msg& cm)
{
    std::vector<uint32> member;
    uint32 family_id = w->cf_mgr->get_family_id(role_id);
    w->cf_mgr->get_member(family_id, member);
    cm.mutable_cm()->set_channel(client::chat_msg_CHANNEL_SYSTEM);
    for (uint32 i = 0; i < member.size(); ++i) {
        center_send_client_by_role_id(w, member[i], *(cm.mutable_cm()));
    }
}

void send_to_family(center_service_t* w, uint32 family_id, google::protobuf::Message& msg)
{
    std::vector<uint32> member;
    w->cf_mgr->get_member(family_id, member);
    for (uint32 i = 0; i < member.size(); ++i) {
        center_send_client_by_role_id(w, member[i], msg);
    }
}

void broadcast_family_admin(center_service_t* w, uint32 family_id, client::client_chat_msg& cm)
{
    std::vector<uint32> member;
    w->cf_mgr->get_manager(family_id, member);
    cm.mutable_cm()->set_channel(client::chat_msg_CHANNEL_SYSTEM);
    for (uint32 i = 0; i < member.size(); ++i) {
        center_send_client_by_role_id(w, member[i], *(cm.mutable_cm()));
    }
}

bool get_role_name(center_service_t* w, uint32 role_id, string& name)
{
    const mysql::sp_load_all_user_smy* smy = w->aus_mgr.get_user_smy(role_id);
    if (smy == NULL) {
        return false;
    }

    name = smy->role_name();

    return true;
}

#if 0
void modify_family_rank(center_service_t* w, client::RANK_TYPE type, uint32 id, uint32 val)
{
    // 更新军团等级排行
    client::rank_info_update riu;
    riu.set_type(type);
    client::rank_info* ri = riu.add_ri();
    ri->set_id(id);
    ri->set_val(val);
    w->rm_mgr.update(&riu);
}
#endif

void get_family_member_by_role(center_service_t* w, uint32 role_id, std::vector<uint32>& member)
{
    uint32 family_id = w->cf_mgr->get_family_id(role_id);
    if (family_id != 0) {
        w->cf_mgr->get_member(family_id, member);
    }
}
// 军团列表请求
void* on_center_pb_family_list_req(client::family_list_req* req, void* ud, size_t* rsp_len)
{
    center_service_t* w = (center_service_t*)ud;

    client::family_list_rsp rsp;
	rsp.set_list_type(0);
	uint32 city_family = w->ct_mgr->get_family_id();
    w->cf_mgr->family_list(city_family,req, rsp);
    center_send_client(w, req->svr_head(), rsp);

    return NULL;
}

void* on_center_pb_family_req(client::family_req* req, void* ud, size_t* rsp_len)
{
    center_service_t* w = (center_service_t*)ud;

    client::family_rsp rsp;
    w->cf_mgr->family_info(req, rsp);
    if (req->has_svr_head()) {
        center_send_client(w, req->svr_head(), rsp);
    } else if (req->has_scene_id()) {
        center_send_scene_by_scene_id(w, req->scene_id(), rsp);
    }
    
    return NULL;
}
// 创建军团
void* on_center_pb_create_family_req(client::create_family_req* req, void* ud, size_t* rsp_len)
{
    center_service_t* w = (center_service_t*)ud;

    client::create_family_rsp rsp;
    uint32 family_id = 0;
	// 王城争霸活动开启不能创建军团
	if (w->ct_mgr->is_act_open()){
		client::activity_opt_rsp opt_msg;
		opt_msg.set_result_type(1);
		center_send_client(w, req->svr_head(), opt_msg);
		return NULL;
	}

	const mysql::sp_load_all_user_smy* req_smy = w->aus_mgr.get_user_smy(req->svr_head().role_id());
	if (req_smy == NULL){
		return NULL;
	}

    int ret = w->cf_mgr->create_family(req, req_smy->role_name(),rsp, family_id,req_smy->fighting_power());
    
	// 创建军团成功才扣费
	client::set_create_famliy_consume consume_rsp;
	if (ret == 0){
		consume_rsp.set_is_succeed(set_create_famliy_consume_result_RESULT_SUCCESS);
		set_player_attr(w,req->svr_head().role_id(),rsp.family_id(),CHIEF,1,0);
		send_family_title_list(w,req->svr_head().role_id(),UPTATE_TYPE_ADD,rsp.family_id(),true);
		// 更新下发聊天的头衔数据
		update_role_title_msg(w,req->svr_head().role_id());
	}
	else{
		consume_rsp.set_is_succeed(set_create_famliy_consume_result_RESULT_FAILD);
	}

	consume_rsp.set_type(req->type());
	consume_rsp.set_role_id(req->svr_head().role_id());
	center_send_scene(w, req->svr_head(), consume_rsp);
	center_send_client(w, req->svr_head(), rsp);

	amtsvr_billlog(w->ctx, "%u|%s|%u|%s|%u|%s", req->svr_head().role_id(), req->GetTypeName().c_str(), req->type(),req->name().c_str(),req->flag_id(),req->flag_name().c_str());

    return NULL;
}
// 申请入军团
void* on_center_pb_apply_family_req(client::apply_family_req* req, void* ud, size_t* rsp_len)
{
    center_service_t* w = (center_service_t*)ud;

    // 30级已下不可加入军团
	const mysql::sp_load_all_user_smy* smy = w->aus_mgr.get_user_smy(req->svr_head().role_id());
	if (smy == NULL /*|| smy->level() < 30*/) {
		return NULL;
	}

    client::apply_family_rsp rsp;
	client::approval_family_rsp rsp_msg;
    int ret = w->cf_mgr->apply_family(req, rsp,smy->fighting_power(),rsp_msg);
    center_send_client(w, req->svr_head(), rsp);

	if (rsp_msg.ret() == approval_family_rsp_result_success){
		uint32 family_level = w->cf_mgr->get_family_level(req->family_id());
		// 更新game 人物相关
		set_player_attr(w,req->svr_head().role_id(),req->family_id(),MEMBER,family_level,0);
		update_role_title_msg(w,req->svr_head().role_id());
		center_send_client(w, rsp_msg.role_id(), rsp_msg);

	}

    if (ret != 0) {
        return NULL;
    }
    
    client::apply_family_list_rsp list_rsp;
    list_rsp.set_family_id(req->family_id());
    list_rsp.add_role_id(req->svr_head().role_id());
    vector<uint32> manager;
    w->cf_mgr->get_manager_add(req->family_id(), manager);
    for (uint32 i = 0; i < manager.size(); ++i) {
        center_send_client_by_role_id(w, manager[i], list_rsp);
    }

    return NULL;
}
// 设置是否自动同意入团申请
void* on_center_pb_family_set_req(client::family_set_req* req, void* ud, size_t* rsp_len)
{
	center_service_t* w = (center_service_t*)ud;
	
	client::family_set_rsp rsp_msg;
	w->cf_mgr->family_set(req,rsp_msg);
	center_send_client(w, req->svr_head(), rsp_msg);

	amtsvr_billlog(w->ctx, "%u|%s|%u|%u", req->svr_head().role_id(), req->GetTypeName().c_str(), req->family_id(),req->approval());

	return NULL;
}

// 请求审核列表
void* on_center_pb_apply_family_list_req(client::apply_family_list_req* req, void* ud, size_t* rsp_len)
{
    center_service_t* w = (center_service_t*)ud;

    client::apply_family_list_rsp rsp;
    w->cf_mgr->apply_family_list(req, rsp);
    center_send_client(w, req->svr_head(), rsp);

    return NULL;
}

void* on_center_pb_apply_family_list_self_req(client::apply_family_list_self_req* req, void* ud, size_t* rsp_len)
{
    center_service_t* w = (center_service_t*)ud;

    client::apply_family_list_self_rsp rsp;
    w->cf_mgr->apply_family_list_self(req, rsp);
    center_send_client(w, req->svr_head(), rsp);

    return NULL;
}
// 审批申请请求
void* on_center_pb_approval_family_req(client::approval_family_req* req, void* ud, size_t* rsp_len)
{
    center_service_t* w = (center_service_t*)ud;

	const mysql::sp_load_all_user_smy* smy = w->aus_mgr.get_user_smy(req->role_id());
	if (smy == NULL /*|| smy->level() < 30*/) {
		return NULL;
	}

	client::approval_family_rsp rsp;
	// 允许
	if (req->type() == 0){
		// 审核者
		int ret = w->cf_mgr->approval_family(req, rsp,smy->fighting_power());
		center_send_client(w, req->svr_head(), rsp);

		// 被审核者
		center_send_client_by_role_id(w, rsp.role_id(), rsp);

		// 设置新军团Id
		if (ret == 0 && req->type() == 0 && rsp.family_id() != 0) {
			uint32 family_level = w->cf_mgr->get_family_level(rsp.family_id());
			set_player_attr(w, rsp.role_id(), rsp.family_id(), MEMBER,family_level,0);
			update_role_title_msg(w,rsp.role_id());
		}
	}else{// 拒绝
		 w->cf_mgr->reject_family(req, rsp); 
		 center_send_client(w, req->svr_head(), rsp);
	}

	amtsvr_billlog(w->ctx, "%u|%s|%u|%u|%u", req->svr_head().role_id(), req->GetTypeName().c_str(), req->family_id(),req->role_id(),req->type());

    return NULL;
}
// 邀请
void* on_center_pb_invite_family_req(client::invite_family_req* req, void* ud, size_t* rsp_len)
{
    center_service_t* w = (center_service_t*)ud;

    // 30级已下不可被邀请
    const mysql::sp_load_all_user_smy* smy = w->aus_mgr.get_user_smy(req->role_id());
    if (smy == NULL /*|| smy->level() < 30*/) {
        return NULL;
    }
        
    // 回复给邀请者
    client::invite_family_rsp rsp;
    int ret = w->cf_mgr->invite_family(req, rsp);
    center_send_client(w, req->svr_head(), rsp);

    // 回复给被邀请者
    if (ret == 0) {
        client::invite_family_list_rsp list_rsp;
        client::invite_family_info* ifi = list_rsp.add_ifi();
        ifi->set_family_id(req->family_id());
        ifi->set_role_id(req->svr_head().role_id());
        center_send_client_by_role_id(w, req->role_id(), list_rsp);
    }
    
	amtsvr_billlog(w->ctx, "%u|%s|%u|%u", req->svr_head().role_id(), req->GetTypeName().c_str(), req->family_id(),req->role_id());

    return NULL;
}
// 邀请列表请求
void* on_center_pb_invite_family_list_req(client::invite_family_list_req* req, void* ud, size_t* rsp_len)
{
    center_service_t* w = (center_service_t*)ud;

    client::invite_family_list_rsp rsp;
    w->cf_mgr->invite_family_list(req, rsp);
    center_send_client(w, req->svr_head(), rsp);

    return NULL;
}

void* on_center_pb_agree_family_req(client::agree_family_req* req, void* ud, size_t* rsp_len)
{
    center_service_t* w = (center_service_t*)ud;

    client::agree_family_rsp rsp;
    uint32 referrer = 0;

	const mysql::sp_load_all_user_smy* smy = w->aus_mgr.get_user_smy(req->svr_head().role_id());
	if (smy == NULL /*|| smy->level() < 30*/) {
		return NULL;
	}
    int ret = w->cf_mgr->agree_family(req, rsp, referrer,smy->fighting_power());
    center_send_client(w, req->svr_head(), rsp);
    center_send_client_by_role_id(w, referrer, rsp);

    // 设置新军团Id
    if (ret == 0 && req->type() == 0) {
		uint32 family_level = w->cf_mgr->get_family_level(rsp.family_id());
		set_player_attr(w,req->svr_head().role_id(),rsp.family_id(),MEMBER,family_level,0);
		update_role_title_msg(w,req->svr_head().role_id());
    }

	amtsvr_billlog(w->ctx, "%u|%s|%u|%u", req->svr_head().role_id(), req->GetTypeName().c_str(), req->family_id(),req->type());

    return NULL;
}
// 自动退团
void* on_center_pb_secede_family_to_center(client::secede_family_to_center* req, void* ud, size_t* rsp_len)
{
	center_service_t* w = (center_service_t*)ud;
	client::secede_family_rsp rsp_msg;
	rsp_msg.set_family_id(req->family_id());
	rsp_msg.set_role_id(req->role_id());

	const mysql::sp_load_all_user_smy* smy = w->aus_mgr.get_user_smy(req->role_id());
	if (smy == NULL /*|| smy->level() < 30*/) {
		return NULL;
	}
	// 处理退出军团
	int ret = w->cf_mgr->secede_family(req, rsp_msg,smy->fighting_power());
	if (ret == 0){
		set_player_attr(w,req->role_id(),0,0,0,0);
		update_role_title_msg(w,req->role_id());
		update_role_title_msg(w,req->role_id());
	}

	center_send_client(w, req->role_id(), rsp_msg);

	return NULL;
}
// 更新相关属性给客户端
void* on_center_pb_update_family_data_from_center(client::update_family_data_from_center* req, void* ud, size_t* rsp_len)
{
	center_service_t* w = (center_service_t*)ud;

	// 返回给game server  
	family* role_family = w->cf_mgr->get_family_by_role_id(req->role_id());
	// 
	if (role_family == NULL) {
		set_player_attr(w,req->role_id(),0,0,0,0,true);
	}
	else {
		family_role_value* member_info = w->cf_mgr->get_member_info(role_family->get_id(),req->role_id());
		if (member_info == NULL){
			return NULL;
		}

		set_player_attr(w,req->role_id(),role_family->get_id(),member_info->_permissions,role_family->lvl());
	}

	send_family_title_list(w,req->role_id(),UPTATE_TYPE_ALL);

	amtsvr_billlog(w->ctx, "%u|%s|%u", req->role_id(), req->GetTypeName().c_str(), req->family_id());

	return NULL;
}

// 修改公告请求
void* on_center_pb_modify_family_post_req(client::modify_family_post_req* req, void* ud, size_t* rsp_len)
{
    center_service_t* w = (center_service_t*)ud;

    client::modify_family_post_rsp rsp;
    w->cf_mgr->modify_family_post(req, rsp);
    center_send_client(w, req->svr_head(), rsp);

	amtsvr_billlog(w->ctx, "%u|%s|%u", req->svr_head().role_id(), req->GetTypeName().c_str(), req->family_id());

    return NULL;
}

void* on_center_pb_family_member_list_req(client::family_member_list_req* req, void* ud, size_t* rsp_len)
{
    center_service_t* w = (center_service_t*)ud;

    client::family_member_list_rsp rsp;
    w->cf_mgr->family_member_list(req, rsp);
    center_send_client(w, req->svr_head(), rsp);

    return NULL;
}
// 修改成员权限
void* on_center_pb_modify_family_member_req(client::modify_family_member_req* req, void* ud, size_t* rsp_len)
{
    center_service_t* w = (center_service_t*)ud;

	if (w->ct_mgr->is_act_open()){
		client::activity_opt_rsp opt_msg;
		opt_msg.set_result_type(1);
		center_send_client(w, req->svr_head(), opt_msg);
		return NULL;
	}

    client::modify_family_member_rsp rsp;
	const mysql::sp_load_all_user_smy* smy_info = w->aus_mgr.get_user_smy(req->role_id());
	if (smy_info == NULL){
		return NULL;
	}

	if (req->type() == ELDER){
		if (smy_info->profession() != 0){
			return NULL;
		}
	} else if (req->type() == TIANSHI){
		if (smy_info->profession() != 2){
			return NULL;
		}
	}else if (req->type() == CHIEF){
		uint32 cur_family_id = w->cf_mgr->get_family_id(req->svr_head().role_id());
		if (cur_family_id != 0){
			if (w->ct_mgr->get_family_id() == cur_family_id){
				rsp.set_ret(modify_family_member_rsp_result_king_erro);
				rsp.set_family_id(req->family_id());
				rsp.set_role_id(req->role_id());
				return NULL;
			}
		}
	}
	uint32 be_chang_id = 0;
    int ret = w->cf_mgr->modify_family_member(req, rsp,smy_info->fighting_power(),smy_info->role_name(),be_chang_id);
    center_send_client(w, req->svr_head(), rsp);
	
    if (ret != 0) {
        return NULL;
    }

    if (req->type() == 0) {// 逐出军团
        set_player_attr(w, req->role_id(),0,0,0,0);
		update_role_title_msg(w,req->role_id());
    } else { // 变更职权
        uint32 family_id = w->cf_mgr->get_family_id(req->role_id());
		uint32 family_level = w->cf_mgr->get_family_level(req->family_id());
        set_player_attr(w, req->role_id(), family_id, req->type(),family_level);
        
        if (req->type() == CHIEF) {
            set_player_attr(w, req->svr_head().role_id(), family_id, MEMBER, family_level);
			update_role_title_msg(w,req->svr_head().role_id());
        }

		if (be_chang_id != 0){
			set_player_attr(w, be_chang_id, family_id, MEMBER, family_level);
		}

		update_role_title_msg(w,req->role_id());
    }

    center_send_client_by_role_id(w, req->role_id(), rsp);

	amtsvr_billlog(w->ctx, "%u|%s|%u|%u|%u", req->svr_head().role_id(), req->GetTypeName().c_str(), req->family_id(),req->role_id(),req->type());

    return NULL;
}
// 逐出军团
void* on_center_pb_expel_member_req(client::expel_member_req* req, void* ud, size_t* rsp_len)
{

	center_service_t* w = (center_service_t*)ud;
	client::expel_member_rsp rsp_msg;
	rsp_msg.set_family_id(req->family_id());
	rsp_msg.set_role_id(req->svr_head().role_id());
	rsp_msg.set_evictee_id(req->evictee_id());
	int ret = w->cf_mgr->expel_member(req, rsp_msg);

	// T除成功
	if (ret == 0){
		// 通知军团每一个人
		std::vector<uint32> member;
		w->cf_mgr->get_member(req->family_id(), member);
		for (uint32 i = 0; i < member.size(); ++i) {
			center_send_client_by_role_id(w, member[i], rsp_msg);
		}
		// 检测被T人是否在线，不在线则不管了
		center_send_client_by_role_id(w,req->evictee_id(),rsp_msg);

		set_player_attr(w,req->evictee_id(),0,0,0,0);

		update_role_title_msg(w,req->evictee_id());
	}else{
		center_send_client(w, req->svr_head(), rsp_msg);
	}

	amtsvr_billlog(w->ctx, "%u|%s|%u|%u", req->svr_head().role_id(), req->GetTypeName().c_str(), req->evictee_id(),req->family_id());

	return NULL;
}

void* on_center_pb_disband_family_req(client::disband_family_req* req, void* ud, size_t* rsp_len)
{
    center_service_t* w = (center_service_t*)ud;

	if (w->ct_mgr->is_act_open()){
		client::activity_opt_rsp opt_msg;
		opt_msg.set_result_type(1);
		center_send_client(w, req->svr_head(), opt_msg);
		return NULL;
	}

    uint32 family_id = w->cf_mgr->get_family_id(req->svr_head().role_id());
    uint32 chief_id  = w->cf_mgr->get_chief(family_id);
    if (req->svr_head().role_id() != chief_id) {
        return NULL;
    }
	// 有没有这个军团
	family* family_db = w->cf_mgr->get_family(req->family_id());
	if (family_db == NULL){
		return NULL;
	}

	std::vector<uint32> member;
	w->cf_mgr->get_member(family_id, member);
    client::disband_family_rsp rsp;
	// 王都军团不能解散
	if (w->ct_mgr->get_family_id() == family_db->get_family_id()){
		rsp.set_ret(disband_family_rsp_result_family_has_king);
		rsp.set_family_id(family_db->get_family_id());
		center_send_scene(w, req->svr_head().role_id(), rsp);
		return NULL;
	}

    int ret = w->cf_mgr->disband_family(req, rsp);
	if (ret == 0 && rsp.ret() == disband_family_rsp_result_success){
		for (uint32 i = 0; i < member.size(); ++i) {
			set_player_attr(w, member[i], 0, 0,0, 0);
			update_role_title_msg(w,member[i]);
		}

		send_family_title_list(w,req->svr_head().role_id(),UPTATE_TYPE_DEL,req->family_id(),true);
	}

	center_send_scene(w, req->svr_head().role_id(), rsp);

	amtsvr_billlog(w->ctx, "%u|%s|%u", req->svr_head().role_id(), req->GetTypeName().c_str(), req->family_id());

    return NULL;
}

// 军团贡献
void* on_center_pb_donate_family_req(client::set_family_money* req, void* ud, size_t* rsp_len)
{
    center_service_t* w = (center_service_t*)ud;
	client::set_donate_consume rsp_msg;

    int result = w->cf_mgr->donate_family(req,rsp_msg);

	if (result != 0){
		return NULL;
	}

	rsp_msg.set_is_success(true);
	center_send_scene(w, req->role_id(), rsp_msg);

	amtsvr_billlog(w->ctx, "%u|%s|%u|%u|%u", req->role_id(), req->GetTypeName().c_str(), req->money(),req->type(),req->server_id());

    return NULL;
}

void* on_center_pb_notive_center_donate_sucess(client::notive_center_donate_sucess* req, void* ud, size_t* rsp_len)
{
	center_service_t* w = (center_service_t*)ud;

	client::donate_family_rsp rsp;
	rsp.set_ret(donate_family_rsp_result_success);
	rsp.set_type(req->type());
	rsp.set_total_donate(req->total_donate());
	w->cf_mgr->family_info(w->cf_mgr->get_family_id(req->role_id()), rsp);
	center_send_client_by_role_id(w, req->role_id(), rsp);

	amtsvr_billlog(w->ctx, "%u|%s|%u|%u|%u|%u", req->role_id(), req->GetTypeName().c_str(), req->type(),req->total_donate(),req->ret(),req->money());

	return NULL;
}
// 军团（战旗）升级
void* on_center_pb_upgrade_family_req(client::upgrade_family_req* req, void* ud, size_t* rsp_len)
{
    center_service_t* w = (center_service_t*)ud;

    client::upgrade_family_rsp rsp;
    w->cf_mgr->upgrade_family(req, rsp);

	if (rsp.ret() == 0){
		uint32 family_level = w->cf_mgr->get_family_level(req->family_id());
		std::vector<uint32> member;
		w->cf_mgr->get_member(req->family_id(), member);
		for (uint32 i = 0; i < member.size();++i){
			family_role_value* member_info = w->cf_mgr->get_member_info(req->family_id(),member[i]);
			if (member_info == NULL){
				continue;
			}

			set_player_attr(w,member[i],req->family_id(),member_info->_permissions,family_level);
		}

		family* family_info = w->cf_mgr->get_family(req->family_id());
		if (family_info != NULL){
			// 广播
			client::broadcast_area_rsp area_msg;
			area_msg.set_area_id(43);
			std::vector<client::content_data> data_vec;
			data_vec.clear();
			client::content_data data_1;
			data_1.set_s(family_info->get_name());
			data_vec.push_back(data_1);

			client::content_data data_2;
			data_2.set_n(family_info->lvl());
			data_vec.push_back(data_2);

			client::mail_body* body_info = area_msg.mutable_area_msg();
			for (uint32 i = 0; i < data_vec.size(); ++i){
				client::content_data* data_info = body_info->add_data();
				data_info->CopyFrom(data_vec[i]);
			}

			if (fill_area_data(43,body_info)){
				send_broadcast_area_msg(w,area_msg);
			}
		}

	}

    center_send_client(w, req->svr_head(), rsp);

    return NULL;
}

void* on_center_pb_family_self_req(client::family_self_req* req, void* ud, size_t* rsp_len)
{
    center_service_t* w = (center_service_t*)ud;

    client::family_self_rsp rsp;
    w->cf_mgr->family_self(req, rsp);
    center_send_client(w, req->svr_head(), rsp);

    return NULL;
}

void* on_center_pb_family_log_req(client::family_log_req* req, void* ud, size_t* rsp_len)
{
    center_service_t* w = (center_service_t*)ud;

    client::family_log_rsp rsp;
    w->cf_mgr->family_log(req, rsp);
    center_send_client(w, req->svr_head(), rsp);

    return NULL;
}

void* on_center_pb_family_record_req(client::family_record_req* req, void* ud, size_t* rsp_len)
{
    center_service_t* w = (center_service_t*)ud;
    client::family_record_rsp rsp;
    w->cf_mgr->family_record(req, rsp);
    center_send_client(w, req->svr_head(), rsp);

    return NULL;
}
// 更新军团战斗力
void* on_center_pb_modify_family_power(client::modify_family_power* req, void* ud, size_t* rsp_len)
{
    center_service_t* w = (center_service_t*)ud;
    w->cf_mgr->modify_family_power(req);
    
    return NULL;
}

void* on_center_pb_family_record_update(client::family_record_update* req, void* ud, size_t* rsp_len)
{
    center_service_t* w = (center_service_t*)ud;
    w->cf_mgr->family_record_update(req);

    return NULL;
}

void* on_center_pb_family_log_update(client::family_log_update* req, void* ud, size_t* rsp_len)
{
    center_service_t* w = (center_service_t*)ud;
    w->cf_mgr->family_log_update(req);
    
    return NULL;
}

void *on_center_pb_family_money_mod_req(client::family_money_mod_req *req, void *ud, size_t *rsp_len)
{
	center_service_t *w = (center_service_t *)ud;
	w->cf_mgr->mod_family_money(req->family_id(), req->money());
	return NULL;
}

void* on_center_pb_query_family_money_req(client::family_money_query_req *req, void *ud, size_t *rsp_len)
{
	center_service_t *w = (center_service_t *)ud;
	family *f = w->cf_mgr->get_family(req->family_id());
	if(f == NULL)
	{
		return NULL;
	}
    const online_role* oi = w->oi_mgr.get_online_info(req->role_id());
	if(!oi)
	{
		return NULL;
	}
	client::family_money_query_rsp rsp;
	rsp.set_family_id(req->family_id());
	rsp.set_role_id(req->role_id());
	rsp.set_inst_id(req->inst_id());
	rsp.set_money(f->get_money());
	center_send_scene(w, *oi, rsp);
	return NULL;
}
// 更换战旗形象
void* on_center_pb_change_family_flag_id_req(client::change_family_flag_id_req *req, void *ud, size_t *rsp_len)
{
	center_service_t* w = (center_service_t*)ud;

	if (w->ct_mgr->is_act_open()){
		client::activity_opt_rsp opt_msg;
		opt_msg.set_result_type(1);
		center_send_client(w, req->svr_head(), opt_msg);
		return NULL;
	}

	client::change_family_flag_id_rsp rsp_msg;

	int ret = w->cf_mgr->change_family_flag_id(req,rsp_msg);
	if (ret == 0){
		std::vector<uint32> member;
		w->cf_mgr->get_member(req->family_id(), member);
		for (uint32 i = 0; i < member.size(); ++i) {
			center_send_client_by_role_id(w, member[i], rsp_msg);
		}
	}
	else{
		// 失败了
		center_send_client_by_role_id(w, req->svr_head().role_id(), rsp_msg);
	}

	return NULL;
}
// 更换战旗名字
void* on_center_pb_change_family_flag_name_req(client::change_family_flag_name_req *req, void *ud, size_t *rsp_len)
{
	center_service_t* w = (center_service_t*)ud;

	if (w->ct_mgr->is_act_open()){
		client::activity_opt_rsp opt_msg;
		opt_msg.set_result_type(1);
		center_send_client(w, req->svr_head(), opt_msg);
		return NULL;
	}

	client::change_family_flag_name_rsp rsp_msg;

	int ret = w->cf_mgr->change_family_flag_name(req,rsp_msg);

	if (ret == 0){
		std::vector<uint32> member;
		w->cf_mgr->get_member(req->family_id(), member);
		for (uint32 i = 0; i < member.size(); ++i) {
			center_send_client_by_role_id(w, member[i], rsp_msg);
		}
	}
	else{
		// 失败了
		center_send_client_by_role_id(w, req->svr_head().role_id(), rsp_msg);
	}

	return NULL;
}
// 弹劾军团长
void* on_center_pb_impeach_chief_req(client::impeach_chief_req *req, void *ud, size_t *rsp_len)
{
	center_service_t* w = (center_service_t*)ud;

	// 王城争霸活动开启不能弹劾军团
	if (w->ct_mgr->is_act_open()){
		client::activity_opt_rsp opt_msg;
		opt_msg.set_result_type(1);
		center_send_client(w, req->svr_head(), opt_msg);
		return NULL;
	}

	client::impeach_chief_rsp rsp_msg;
	const mysql::sp_load_all_user_smy* smy_info = w->aus_mgr.get_user_smy(req->svr_head().role_id());
	if (smy_info == NULL){
		return NULL;
	}

	uint32 cur_family_id = w->cf_mgr->get_family_id(req->svr_head().role_id());
	
	if (cur_family_id != 0){
		if (w->ct_mgr->get_family_id()== cur_family_id ){
			rsp_msg.set_result_type(IMPEACH_RESULT_HAS_KING);
			center_send_client(w, req->svr_head(), rsp_msg);
		}
	}

	uint32 cheif_id = w->cf_mgr->get_chief(cur_family_id);
	if (cheif_id == 0){
		return NULL;
	}

	const mysql::sp_load_all_user_smy* cheif_info = w->aus_mgr.get_user_smy(cheif_id);
	if (smy_info == NULL){
		return NULL;
	}
	const online_role* on_info = w->oi_mgr.get_online_info(cheif_id);

	uint32 offline_limit = CONFMGR->get_basic_value(10075);
	uint32 now = (uint32)time(NULL);
	if (now >= cheif_info->offline_time()+ offline_limit && on_info == NULL){
		w->cf_mgr->impeach_chief(req,rsp_msg,smy_info->role_name());
	}else{
		rsp_msg.set_result_type(IMPEACH_RESULT_TIME_ERRO);
	}

	center_send_client_by_role_id(w, req->svr_head().role_id(), rsp_msg);

	return NULL;
}
// 请求捐献列表
void* on_center_pb_donate_list_req(client::donate_list_req *req, void *ud, size_t *rsp_len)
{
	center_service_t* w = (center_service_t*)ud;

	client::donate_list_rsp rsp_msg;

	w->cf_mgr->donate_list(req,rsp_msg);

	center_send_client_by_role_id(w, req->svr_head().role_id(), rsp_msg);

	return NULL;
}
// 结盟 or 敌对 or 验证
void* on_center_pb_ally_family_req(client::ally_family_req *req, void *ud, size_t *rsp_len)
{
	center_service_t* w = (center_service_t*)ud;

	if (w->ct_mgr->is_act_open()){
		client::activity_opt_rsp opt_msg;
		opt_msg.set_result_type(1);
		center_send_client(w, req->svr_head(), opt_msg);
		return NULL;
	}

	client::ally_family_rsp rsp_msg;

	uint32 r1 = w->cf_mgr->get_family_relation_type(req->family_id(), req->target_family_id());
	uint32 r2 = w->cf_mgr->get_family_relation_type(req->target_family_id(), req->family_id());

	int ret = w->cf_mgr->ally_family(req,rsp_msg);

	if (ret == 0){
		uint32 target_family_chief = w->cf_mgr->get_chief(req->target_family_id());
		family* family_info = w->cf_mgr->get_family(req->family_id());
		mysql::tb_family* family_db = family_info->get();
		if (req->opt_type() == OPT_FORIGN_TYPE_CHECKING)
		{// 结盟申请发邮件给目标军团等待同意
			const config_legion* legion_info = CONFMGR->get_config_legion_mgr().get_config_legion(family_info->lvl());
			if (legion_info == NULL){
				return NULL;
			}

			send_ally_mail(w,target_family_chief,family_db,legion_info->get_member_limit());
		}else{
			send_family_mail(w,target_family_chief,family_db,req->opt_type());
		}
		// 同意结盟
		if(req->opt_type() == OPT_FORIGN_TYPE_ALLY) 
		{
			// 你是我的敌盟
			if(r1 == OPT_FORIGN_TYPE_HOSTILITY)
			{
				// 向本军团成员广播删除敌盟关系
				member_update_relationship(w, req->family_id(), req->target_family_id(), 0, 2);
			}
			//  我是你的敌盟
			if(r2 == OPT_FORIGN_TYPE_HOSTILITY)
			{
				// 向你军团成员广播删除敌盟关系
				member_update_relationship(w, req->target_family_id(), req->family_id(), 0, 2);
			}
			// 向双方成员广播添加同盟关系
			member_update_relationship(w, req->family_id(), req->target_family_id(), 1, 1);
			member_update_relationship(w, req->target_family_id(), req->family_id(), 1, 1);
		}
		// 添加敌盟
		else if(req->opt_type() == OPT_FORIGN_TYPE_HOSTILITY)
		{
			// 如果是同盟
			if(r1 == OPT_FORIGN_TYPE_ALLY || r2 == OPT_FORIGN_TYPE_ALLY)
			{
				// 向双方成员广播删除同盟关系
				member_update_relationship(w, req->family_id(), req->target_family_id(), 0, 1);
				member_update_relationship(w, req->target_family_id(), req->family_id(), 0, 1);
			}
			// 向本军团成员广播添加敌盟关系
			member_update_relationship(w, req->family_id(), req->target_family_id(), 1, 2);
		}
	}

	center_send_client_by_role_id(w, req->svr_head().role_id(), rsp_msg);

	return NULL;
}
// 请求解除结盟 or 敌对
void* on_center_pb_remove_foreign_req(client::remove_foreign_req *req, void *ud, size_t *rsp_len)
{
	center_service_t* w = (center_service_t*)ud;

	if (w->ct_mgr->is_act_open()){
		client::activity_opt_rsp opt_msg;
		opt_msg.set_result_type(1);
		center_send_client(w, req->svr_head(), opt_msg);
		return NULL;
	}

	client::remove_foreign_rsp rsp_msg;
	bool is_remove_ally = false;
	int ret = w->cf_mgr->remove_foreign(req,rsp_msg,is_remove_ally);

	uint32 target_family_chief = w->cf_mgr->get_chief(req->target_family_id());
	family* family_info = w->cf_mgr->get_family(req->family_id());
	mysql::tb_family* family_db = family_info->get();

	if (ret == 0 && is_remove_ally){
		// 发解除同盟邮件
		send_family_mail(w,target_family_chief,family_db,OPT_FORIGN_TYPE_REMOVE_ALLY);
		member_update_relationship(w, req->family_id(), req->target_family_id(), 0, 1);
		member_update_relationship(w, req->target_family_id(), req->family_id(), 0, 1);
	}
	else if (ret == 0 && !is_remove_ally){
		// 发解除敌盟邮件
		send_family_mail(w,target_family_chief,family_db,OPT_FORIGN_TYPE_REMOVE_HOS);
		member_update_relationship(w, req->family_id(), req->target_family_id(), 0, 2);
	}

	center_send_client_by_role_id(w, req->svr_head().role_id(), rsp_msg);

	return NULL;
}
// 请求外交列表
void* on_center_pb_ally_family_list_req(client::ally_family_list_req *req, void *ud, size_t *rsp_len)
{
	center_service_t* w = (center_service_t*)ud;

	client::ally_family_list_rsp rsp_msg;

	w->cf_mgr->ally_family_list(req,rsp_msg);

	center_send_client_by_role_id(w, req->svr_head().role_id(), rsp_msg);

	return NULL;
}

void *on_center_pb_load_friend_family_req(client::load_friend_family_req *req, void *ud, size_t *rsp_len)
{
	center_service_t *w = (center_service_t *)ud;
	vector<uint32> v_friend;
	vector<uint32> v_enemy;
	w->cf_mgr->get_diplomacy_relation(req->family_id(), client::OPT_FORIGN_TYPE_ALLY, v_friend);
	w->cf_mgr->get_diplomacy_relation(req->family_id(), client::OPT_FORIGN_TYPE_HOSTILITY, v_enemy);
	client::load_friend_family_rsp rsp;
	client::family_relation_list frl;
	rsp.set_role_id(req->role_id());
	for(size_t i = 0; i < v_friend.size(); ++i){
		rsp.add_id(v_friend[i]);
		frl.add_alliance(v_friend[i]);
	}

	center_send_scene(w, req->role_id(), rsp);

	for(size_t i = 0; i < v_enemy.size(); ++i){
		frl.add_enemy(v_enemy[i]);
	}

	center_send_client_by_role_id(w, req->role_id(), frl);

	return NULL;
}
