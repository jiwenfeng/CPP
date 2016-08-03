#include "config_siege.h"
#include "client.city.pb.h"
#include "center_interface.h"
#include "client.family.pb.h"
#include "utf8.h"

static const string &get_role_name(center_service_t *w, uint32 role_id)
{
	const mysql::sp_load_all_user_smy *smy = w->aus_mgr.get_user_smy(role_id);
	if(smy == NULL)
	{
		static string str = "";
		return str;
	}
	return smy->role_name();
}

static void change_chief_skill(center_service_t *w, uint32 family_id, uint32 op)
{
	uint32 role_id = w->cf_mgr->get_chief(family_id);
	const online_role *o = w->oi_mgr.get_online_info(role_id);
	if(o == NULL)
	{
		return ;
	}
	client::siege_skill_modify req;
	req.set_role_id(role_id);
	for(int i = 0; i < 3; ++i)
	{
		uint32 id = (uint32)CONFMGR->get_basic_value(10127 + i);
		if(id == 0)
		{
			continue;
		}
		req.add_id(id);
	}
	req.set_op(op);
	center_send_scene(w, *o, req);
}

static bool packet_role_simple_info(center_service_t *w, const struct manager_info &mi, client::official_info **fmi)
{
	const mysql::sp_load_all_user_smy *smy = w->aus_mgr.get_user_smy(mi.role_id);
	if(NULL == smy)
	{
		return false;
	}
	(*fmi)->set_role_id(mi.role_id);
	(*fmi)->set_role_name(smy->role_name());
	int index = w->cf_mgr->trans_permission_to_index(mi.official_id);
	(*fmi)->set_pos(index);
	(*fmi)->set_sex(smy->profession());
	(*fmi)->set_model_id(CONFMGR->get_config_siege_mgr().get_profession_model(index, smy->profession()));
	return true;
}

static void send_mail(center_service_t *w, uint32 role_id, int flag)
{
	client::mail_info mi;
	client::mail_head *mh = mi.mutable_mh();
	client::mail_body* mb = mi.mutable_mb();
	mh->set_mail_id(0);
	mh->set_mail_type(MAIL_TYPE_SYSTEM);
	mh->set_recv_role_id(role_id);
	mh->set_send_role_id(0);
	mh->set_send_role_name("system");
	if(1 == flag)
	{
		mh->set_title("capital_win_title");
		mb->set_content("capital_win_content");
	}
	else
	{
		mh->set_title("capital_lose_title");
		mb->set_content("capital_lose_content");
	}

	mh->set_send_time((uint32)time(NULL));
	mh->set_past_time(get_mail_past_time(MAIL_TYPE_SYSTEM,true));
	mh->set_attachment_num(1);
	mh->set_opened(false);
	mh->set_is_take(false);
	client::mail_attr* attr = mi.add_attr();
	attr->set_attr_type(EXPERIENCE);
	if(1 == flag)
	{
		attr->set_attr_value(CONFMGR->get_basic_value(10047));
	}
	else
	{
		attr->set_attr_value(CONFMGR->get_basic_value(10048));
	}
    amtsvr_billlog(w->ctx, "%u|%s", role_id, mi.DebugString().c_str());
	send_new_mail(w, role_id, mi);
}

static void announce_siege_over(center_service_t *w, uint32 msg_id, uint32 family_id)
{
	if(family_id != 0)
	{
		vector<client::content_data> v;
		const string &family_name = w->cf_mgr->get_family_name(family_id);
		uint32 role_id = w->cf_mgr->get_chief(family_id);
		const string &role_name = get_role_name(w, role_id);
		client::content_data cd1, cd2;
		cd1.set_s(family_name);
		cd2.set_s(role_name);
		v.push_back(cd1);
		v.push_back(cd2);
		send_broadcast_area_msg(w, msg_id, v);
	}
}

static void send_siege_reward(center_service_t *w, client::siege_over *so)
{
	uint32 now = (uint32)time(NULL);
	for(int i = 0; i < so->role_size(); ++i)
	{
		uint32 role_id = so->role(i);
		uint32 family_id = w->cf_mgr->get_family_id(role_id);
		uint32 time = w->cf_mgr->get_join_time(family_id, role_id);
		if(now < time + (uint32)CONFMGR->get_basic_value(10050))
		{
			continue;
		}
		send_mail(w, role_id, so->family_id() != 0 && so->family_id() == family_id);
	}
}

//王城界面信息
void* on_pb_center_city_info_req(client::city_info_req* req, void* ud, size_t* rsp_len)
{
	center_service_t *w = (center_service_t *)ud;
	uint32 family_id = w->ct_mgr->get_family_id();
	client::city_info_rsp rsp;
	rsp.set_open_time(w->ct_mgr->get_open_time());
	rsp.set_last(CONFMGR->get_basic_value(10049));
	if(0 == family_id)	// 没有占领军团
	{
		rsp.set_modify(false);
		center_send_client(w, req->svr_head(), rsp);
		return NULL;
	}
	uint32 role_id = req->svr_head().role_id();
	uint32 now = (uint32)time(NULL);
	rsp.set_family_name(w->cf_mgr->get_family_name(family_id));
	rsp.set_modify(w->cf_mgr->get_chief(family_id) == role_id);
	rsp.set_title(w->ct_mgr->get_title());
	rsp.set_timelong(now - w->ct_mgr->get_occupy_time());
	std::vector<manager_info> mi;
	w->cf_mgr->get_manager(family_id, mi);
	std::vector<manager_info>::iterator it = mi.begin();
	for(; it != mi.end(); ++it)
	{
		official_info *oi = rsp.add_oi();
		packet_role_simple_info(w, *it, &oi);
	}
	// 要加入配偶
	center_send_client(w, req->svr_head(), rsp);
    amtsvr_billlog(w->ctx, "%u|%s", role_id, req->GetTypeName().c_str());
	return NULL;
}

//修改国号
void *on_pb_center_city_mod_title_req(client::city_mod_title_req* req, void* ud, size_t* rsp_len)
{
	center_service_t *w = (center_service_t *)ud;
	client::city_mod_title_rsp rsp;
	if(w->ct_mgr->is_act_open())
	{
		rsp.set_err_no(SIEGE_PROCEED);
		center_send_client(w, req->svr_head(), rsp);
		return NULL;
	}
	if(time_utils::is_same_day(w->ct_mgr->get_mod_title_time(), (uint32)time(NULL)))
	{
		rsp.set_err_no(TIME_LIMIT);
		center_send_client(w, req->svr_head(), rsp);
		return NULL;
	}
	uint32 role_id = req->svr_head().role_id();
	uint32 fid = w->ct_mgr->get_family_id();
	if(w->cf_mgr->get_chief(fid) != role_id)
	{
		rsp.set_err_no(KING_LIMIT);
		center_send_client(w, req->svr_head(), rsp);
		return NULL;
	}
	bool chinese;
	if(utf_len(req->city_title().c_str(), chinese) > 2)
	{
		rsp.set_err_no(NAME_TOO_LONG);
		center_send_client(w, req->svr_head(), rsp);
		return NULL;
	}
	if(!chinese)
	{
		rsp.set_err_no(NAME_MUST_CHINESE);
		center_send_client(w, req->svr_head(), rsp);
		return NULL;
	}
	if(w->iw_mgr.has_illegal_word(req->city_title()))
	{
		rsp.set_err_no(ILLEGAL_WORD);
		center_send_client(w, req->svr_head(), rsp);
		return NULL;
	}
	w->ct_mgr->set_city_title(req->city_title());
	rsp.set_err_no(SUCCESS);
	center_send_client(w, req->svr_head(), rsp);
	// 发送改名广播
	vector<client::content_data> v;
	const string &role_name = get_role_name(w, role_id);
	client::content_data cd1, cd2;
	cd1.set_s(role_name);
	cd2.set_s(req->city_title());
	v.push_back(cd1);
	v.push_back(cd2);
	send_broadcast_area_msg(w, 46, v);

    amtsvr_billlog(w->ctx, "%u|%s|%s", role_id, req->GetTypeName().c_str(), req->city_title().c_str());

	init_scene_sculpture(w);
	return NULL;
}

//城池福利请求
void *on_pb_center_city_welfare_info_req(client::city_welfare_info_req *req, void *ud, size_t *rsp_len)
{
	center_service_t *w = (center_service_t *)ud;
	uint32 role_id = req->svr_head().role_id();
	uint32 family_id = w->cf_mgr->get_family_id(role_id);
	uint32 fid = w->cwm_mgr->get_family_id();
	client::city_welfare_info_rsp rsp;

	const config_siege_mgr &mgr = CONFMGR->get_config_siege_mgr();
	const map<int, config_siege *> &list = mgr.get_welfare_list();
	map<int, config_siege *>::const_iterator it = list.begin();
	for(; it != list.end(); ++it)
	{
		client::city_pos_reward *cpr = rsp.add_cpr();
		cpr->set_pos(it->first);
		const map<int, int> &item = it->second->get_reward();
		cpr->set_id(item.begin()->first);
		cpr->set_num(item.begin()->second);
	}


	if((0 == fid) || (fid != family_id))
	{
		rsp.set_pos(0);
		center_send_client(w, req->svr_head(), rsp);
		return NULL;
	}

	// 需检查配偶
	int index = w->cf_mgr->get_family_title_index(role_id, family_id);
	rsp.set_pos(index);
	rsp.set_status(w->cwm_mgr->welfare_has_got(index) ? 1 : 0);
	center_send_client(w, req->svr_head(), rsp);
    amtsvr_billlog(w->ctx, "%u|%s", role_id, req->GetTypeName().c_str());
	return NULL;
}

void *on_pb_center_city_welfare_get_confirm(client::city_welfare_get_confirm *req, void *ud, size_t *rsp_len)
{
	center_service_t *w = (center_service_t *)ud;
	uint32 family_id = w->cf_mgr->get_family_id(req->role_id());
	// 需检查配偶
	int pos = w->cf_mgr->get_family_title_index(req->role_id(), family_id);
	w->cwm_mgr->set_time(pos);
	return NULL;
}

//领取城池奖励
void *on_pb_center_city_welfare_get_req(client::city_welfare_get_req *req, void *ud, size_t *rsp_len)
{
	center_service_t *w = (center_service_t *)ud;
	uint32 role_id = req->svr_head().role_id();
	uint32 family_id = w->cf_mgr->get_family_id(role_id);
	int pos = w->cf_mgr->get_family_title_index(role_id, family_id);
	uint32 owner = w->cwm_mgr->get_family_id();
	client::city_welfare_get_rsp rsp;
	if(w->ct_mgr->is_act_open())
	{
		rsp.set_err_no(SIEGE_NOT_CLOSED);
		center_send_client(w, req->svr_head(), rsp);
		return NULL;
	}

	if(owner == 0 || owner != family_id || pos < 1)
	{
		rsp.set_err_no(OFFICIAL_LIMIT);
		center_send_client(w, req->svr_head(), rsp);
		return NULL;
	}
	if(w->cwm_mgr->welfare_has_got(pos))
	{
		rsp.set_err_no(REWARD_GOT);
		center_send_client(w, req->svr_head(), rsp);
		return NULL;
	}
	const config_siege_mgr &mgr = CONFMGR->get_config_siege_mgr();
	const map<int, int> &item = mgr.get_reward(pos);
	client::city_welfare_get_notify notify;
	notify.set_props_id(item.begin()->first);
	notify.set_num(item.begin()->second);
	notify.set_role_id(req->svr_head().role_id());
	center_send_scene_by_service_id(w, req->svr_head().scenesvr_id(), notify);
    amtsvr_billlog(w->ctx, "%u|%s", role_id, req->GetTypeName().c_str());
	return NULL;
}

#if 0
void *on_pb_center_siege_enter_req(client::siege_enter_req *req, void *ud, size_t *rsp_len)
{
	center_service_t *w = (center_service_t *)ud;
	if(!SIEGE_FIELD_OPEN(w->ct_mgr->get_act_status()))
	{
		client::siege_enter_rsp rsp;
		rsp.set_ret(client::siege_enter_rsp_RET_SCENE_CLOSED);
		center_send_client(w, req->svr_head(), rsp);
		return NULL;
	}
	const online_role *oi = w->oi_mgr.get_online_info(req->svr_head().role_id());
	if(!oi)
	{
		return NULL;
	}
	client::siege_enter_field sef;
	sef.set_role_id(req->svr_head().role_id());
	center_send_scene(w, *oi, sef);
	return NULL;
}
#endif

void update_role_title_msg(center_service_t* w,uint32 role_id);
//城池争夺战结束
void *on_pb_center_siege_over(client::siege_over* so, void* ud, size_t* rsp_len)
{
	center_service_t *w = (center_service_t *)ud;
	announce_siege_over(w, so->msg_id(), so->family_id());
	send_siege_reward(w, so);
	w->ct_mgr->on_siege_over(so->family_id());
	w->cwm_mgr->mod_family_id(w->ctx, so->family_id());
	init_scene_sculpture(w);

	std::vector<uint32> vec_id;
	w->cf_mgr->get_manager(so->family_id(),vec_id);

	for (uint32 i = 0; i < vec_id.size(); ++i){
		update_role_title_msg(w,vec_id[i]);
	}
	
	return NULL;
}

void *on_pb_center_siege_status(client::siege_status *req, void *ud, size_t *rsp_len)
{
	center_service_t *w = (center_service_t *)ud;
	client::siege_status_rsp rsp;
	rsp.set_status(w->ct_mgr->get_act_status());
	const online_role *oi = w->oi_mgr.get_online_info(req->role_id());
	center_send_client(w, *oi, rsp);
    amtsvr_billlog(w->ctx, "%u|%s|%u", req->role_id(), req->GetTypeName().c_str(), rsp.status());
	return NULL;
}

void *on_pb_center_siege_flag_occupy(client::siege_flag_occupy *req, void *ud, size_t *rsp_len)
{
	center_service_t *w = (center_service_t *)ud;
	w->ct_mgr->set_temp_family(req->new_family_id());
	change_chief_skill(w, req->old_family_id(), 2);
	change_chief_skill(w, req->new_family_id(), 1);
	return NULL;
}

void *on_pb_siege_call_accept_req(client::siege_call_accept_req *req, void *ud, size_t *rsp_len)
{
	center_service_t *w = (center_service_t *)ud;
	uint32 role_id = req->svr_head().role_id();
	uint32 family_id = w->cf_mgr->get_family_id(role_id);
	client::siege_call_accept_rsp rsp;
	if(0 == family_id)
	{
		rsp.set_ret(client::siege_call_accept_rsp_RET_NO_FAMILY);
		center_send_client(w, req->svr_head(), rsp);
		return NULL;
	}
	uint32 chief = w->cf_mgr->get_chief(family_id);
	if(chief == role_id)
	{
		rsp.set_ret(client::siege_call_accept_rsp_RET_MY_CALL);
		center_send_client(w, req->svr_head(), rsp);
		return NULL;
	}
	const online_role *oi = w->oi_mgr.get_online_info(role_id);
	if(!oi)
	{
		return NULL;
	}
	client::siege_call sc;
	sc.set_role_id(role_id);
	sc.set_chief(chief);
	center_send_scene(w, *oi, sc); 
    amtsvr_billlog(w->ctx, "%u|%s", req->svr_head().role_id(), req->GetTypeName().c_str());
	return NULL;
}

void *on_pb_center_broadcast_siege_status(client::siege_status_rsp *rsp, void *ud, size_t *rsp_len)
{
	center_service_t *w = (center_service_t *)ud;
	const map<uint32, online_role>& all_online = w->oi_mgr.get_all_online_info();
	map<uint32, online_role>::const_iterator it = all_online.begin();
	for(; it != all_online.end(); ++it)
	{
		center_send_client(w, it->second, *rsp);
	}
	return NULL;
}

void *on_pb_center_king_skill_add(client::king_skill_add *ksa, void *ud, size_t *rsp_len)
{
	center_service_t *w = (center_service_t *)ud;
	if(w->cf_mgr->get_chief(ksa->family_id()) != ksa->role_id())
	{
		return NULL;
	}
	if(w->ct_mgr->get_temp_family() != ksa->family_id())
	{
		return NULL;
	}
	change_chief_skill(w, ksa->family_id(), 1);
	return NULL;
}

void *on_pb_center_siege_start(client::siege_start *req, void *ud, size_t *rsp_len)
{
	center_service_t *w = (center_service_t *)ud;
	const map<uint32, family*>& family_list =  w->cf_mgr->get_family_list();
	map<uint32, family *>::const_iterator it = family_list.begin();
	for(; it != family_list.end(); ++it)
	{
		client::family_simple_info *fsi = req->add_fsi();
		fsi->set_family_id(it->first);
		fsi->set_family_name(it->second->get_name());
		fsi->set_flag_id(it->second->get_flag_id());
		fsi->set_role_id(it->second->get_chief());
		const mysql::sp_load_all_user_smy *smy = w->aus_mgr.get_user_smy(fsi->role_id());
        if (smy == NULL) {
            fsi->set_user_name("name_error");
        } else {
			fsi->set_user_name(smy->role_name());
        }
		vector<uint32> v;
		w->cf_mgr->get_diplomacy_relation(it->first, client::OPT_FORIGN_TYPE_ALLY, v);
		for(size_t i = 0; i < v.size(); ++i)
		{
			fsi->add_alliance(v[i]);
		}
	}
	center_send_scene_by_scene_id(w, 101, *req);
	return NULL;
}
