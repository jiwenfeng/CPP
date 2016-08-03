#include "center_interface.h"
#include "link_lua.h"

#define SYS_MSG_TO_FRIEND        1
#define SYS_MSG_TO_TEAM          2
#define SYS_MSG_TO_FAMILY        3
#define WORLD_CHAT_TIME_INTERVAL 6   // 聊天时间间隔6秒
#define WORLD_CHAT_MSG_LEN       250 // 用户聊天最大输入字符数目80

void update_role_title_msg(center_service_t* w,uint32 role_id)
{
	const online_role* oi = w->oi_mgr.get_online_info(role_id);
	if (oi == NULL){
		return;
	}

	client::update_title_info_rsp rsp_msg;
	rsp_msg.set_role_id(role_id);

	family* family_info = w->cf_mgr->get_family_by_role_id(role_id);

	if (family_info != NULL){
		rsp_msg.set_family_name(family_info->get_family_name());
		rsp_msg.set_family_title(w->cf_mgr->get_family_title(role_id,family_info->get_family_id()));
		if (w->ct_mgr->get_family_id() != family_info->get_family_id()){
			rsp_msg.set_is_siege(false);
		}else{
			rsp_msg.set_is_siege(true);
		}
	}

	center_send_client_by_role_id(w,role_id,rsp_msg);
}

static bool center_chat_time_limit(center_service_t* w, online_role* oi)
{
    if (oi == NULL) {
        return false;
    }

    uint32 now = (uint32)time(NULL);
    if ((int)(now - (oi->last_chat_time)) <= WORLD_CHAT_TIME_INTERVAL) {
        return false;
    }

    oi->last_chat_time = now;

    return true;
}

static void center_broadcast_chat_msg(center_service_t* w, client::client_chat_msg* cm)
{
    const mysql::sp_load_all_user_smy* smy = w->aus_mgr.get_user_smy(cm->svr_head().role_id());
    if (smy == NULL /*|| smy->level() < 10*/) {
        return ;
    }

    online_role* oi = w->oi_mgr.mutable_online_info(cm->svr_head().role_id());
    // 检测CD时间
    if (!center_chat_time_limit(w, oi)) {
        return;
    }

    w->oi_mgr.brodcast(w->ctx, *(cm->mutable_cm()));
}

static void test_command(center_service_t* w, client::client_chat_msg* cm);

static void scene_broadcast_chat_msg(center_service_t* w, client::client_chat_msg* cm)
{
    test_command(w, cm);

    online_role* oi = w->oi_mgr.mutable_online_info(cm->svr_head().role_id());
    if (!center_chat_time_limit(w, oi)){
        return;
    }

    center_send_scene(w, cm->svr_head().role_id(), *cm);
}

void get_family_member_by_role(center_service_t* w, uint32 role_id, std::vector<uint32>& member);

static void family_broadcast_chat_msg(center_service_t* w, client::client_chat_msg* cm)
{
    std::vector<uint32> member;
    get_family_member_by_role(w, cm->svr_head().role_id(), member);
    for (uint32 i = 0; i < member.size(); ++i) {
        center_send_client_by_role_id(w, member[i], *(cm->mutable_cm()));
    }
}

static void team_broadcast_chat_msg(center_service_t* w, client::client_chat_msg* cm)
{
    std::vector<uint32> roles;
	uint32 team_id = 0;
	team_id = w->tm_mgr.get_team_id(cm->svr_head().role_id());
	if (team_id != 0){
		w->tm_mgr.get_team_members(team_id, roles);
		for (uint32 i = 0; i < roles.size(); ++i) {
			center_send_client_by_role_id(w, roles[i], *(cm->mutable_cm()));
		}
	}
}

static void private_chat_msg(center_service_t* w, client::client_chat_msg* cm)
{
    if (!cm->cm().has_dst_role_id()){
        return;
    }

    online_role* src_oi = w->oi_mgr.mutable_online_info(cm->svr_head().role_id());
    online_role* dst_oi = w->oi_mgr.mutable_online_info(cm->cm().dst_role_id());
    if (src_oi == NULL){
        return;
    }

	client::private_friend_rsp rsp;
	if (dst_oi == NULL){
		rsp.set_result_type(0);
		center_send_client(w, *src_oi, rsp);
		return;
	}

    center_send_client(w, *src_oi, *(cm->mutable_cm()));
    center_send_client(w, *dst_oi, *(cm->mutable_cm()));

    const mysql::sp_load_all_user_smy* dst_smy = w->aus_mgr.get_user_smy(cm->cm().dst_role_id());
    if (dst_smy == NULL){
        return;
    }

    const mysql::sp_load_all_user_smy* src_smy = w->aus_mgr.get_user_smy(cm->svr_head().role_id());
    if (src_smy == NULL){
        return;
    }


    client::friend_list_rsp src_add_rsp;
    client::friend_list_rsp src_del_rsp;
    bool has_del = false;
    w->fm_mgr.add_temporary_friend(cm->svr_head().role_id(),cm->cm().dst_role_id(),dst_smy->level(),dst_smy->role_name(),dst_smy->vip_level(),src_add_rsp,has_del,src_del_rsp);
    client::friend_list_rsp dst_add_rsp;
    client::friend_list_rsp dst_del_rsp;
    bool has_del_2 = false;
    w->fm_mgr.add_temporary_friend(cm->cm().dst_role_id(),cm->svr_head().role_id(),src_smy->level(),src_smy->role_name(),src_smy->vip_level(),dst_add_rsp,has_del_2,dst_del_rsp);


    player_role_friends* src_friends = w->fm_mgr.mut(cm->svr_head().role_id());
    player_role_friends* dst_friends = w->fm_mgr.mut(dst_smy->role_id());
    if (src_friends != NULL) { 
        src_friends->set_contact_time(cm->cm().dst_role_id(), (uint32)time(NULL));
    }

    if (dst_friends != NULL){
        dst_friends->set_contact_time(cm->svr_head().role_id(),(uint32)time(NULL));
    }

    center_send_client_by_role_id(w, cm->svr_head().role_id(), src_add_rsp);
    center_send_client_by_role_id(w, cm->cm().dst_role_id(), dst_add_rsp);
    if (has_del){
        center_send_client_by_role_id(w, cm->svr_head().role_id(), src_del_rsp);
    }

    if (has_del_2){
        center_send_client_by_role_id(w, cm->cm().dst_role_id(), dst_del_rsp);
    }
}

static void horn_broadcast_chat_msg(center_service_t* w, client::client_chat_msg* cm)
{
    cm->mutable_cm()->set_channel(client::chat_msg_CHANNEL_HORN);
    // 先支持联调
    w->oi_mgr.brodcast(w->ctx, *(cm->mutable_cm()));
}

static void sub_horn_msg(center_service_t* w, client::client_chat_msg* cm)
{
    center_send_scene(w, cm->svr_head(), *cm);
}

static void friend_chat_msg(center_service_t* w, client::client_chat_msg* cm)
{
    player_role_friends* src_friends = w->fm_mgr.mut(cm->svr_head().role_id());
    if (src_friends == NULL) {
        return ;
    }

    std::map<uint32, client::friend_info>* p = src_friends->get_friend();
    std::map<uint32, client::friend_info>::iterator i = p->begin();
    for (; i != p->end(); ++i) {
        center_send_client_by_role_id(w, i->second.role_id(), *(cm->mutable_cm()));
    }
}

void system_chat_msg(center_service_t* w, client::client_chat_msg* cm)
{
    if (cm->cm().dst_role_id() == 0) {
        w->oi_mgr.brodcast(w->ctx, *(cm->mutable_cm()));
    } else if (cm->cm().dst_role_id() == SYS_MSG_TO_FRIEND) { // 好友
        friend_chat_msg(w, cm);
    } else if (cm->cm().dst_role_id() == SYS_MSG_TO_TEAM) { // 队友
        team_broadcast_chat_msg(w, cm);
    } else if (cm->cm().dst_role_id() == SYS_MSG_TO_FAMILY) { // 家族
        family_broadcast_chat_msg(w, cm);
    } 
}
// 聊天消息检测合法性
static int filter_client_chat_req(center_service_t* w, client::client_chat_msg* cm, bool& is_client_req)
{
    is_client_req = false;
    if (cm->has_svr_head()){
        if (cm->cm().content().length() > WORLD_CHAT_MSG_LEN){
            return -2;
        }

        uint32 role_id = cm->svr_head().role_id();
        const online_role* oi = w->oi_mgr.get_online_info(role_id);
        if (oi == NULL) {
            return -2;
        }

        client::chat_msg* cmi = cm->mutable_cm();
        if (cmi == NULL) {
            return -3;
        }

        // 检测禁言时间
        uint32 ban_time = w->bu_mgr.find_chat(role_id);
        if ((uint32)time(NULL) < ban_time) {
            return -4;
        }

        cmi->set_src_rold_id(role_id);
        cmi->set_src_role_name(oi->role_name);
        is_client_req = true;
    }

    return 0;
}
// 客户端发送消息接收
void* on_pb_center_chat_msg(client::client_chat_msg* cm, void* ud, size_t* rsp_len)
{
    center_service_t* w = (center_service_t*)ud;
    bool is_client_req;
    if (filter_client_chat_req(w, cm, is_client_req) != 0){
        return NULL;
    }

	w->iw_mgr.replace_illegal_word(*cm->mutable_cm()->mutable_content());

    test_command(w, cm);

    // 注意后续追加聊天频道时，要对用户发送的消息频道做合法性过滤
    switch (cm->cm().channel()){
    case client::chat_msg_CHANNEL_WORLD:// 世界
        {
            center_broadcast_chat_msg(w, cm);
        }
        break;
    case client::chat_msg_CHANNEL_SCENE:
        {
            scene_broadcast_chat_msg(w, cm);
        }
        break;
    case client::chat_msg_CHANNEL_FAMILY:// 军团
        {
            family_broadcast_chat_msg(w, cm);
        }
        break;
    case client::chat_msg_CHANNEL_TEAM:// 队伍
        {
            team_broadcast_chat_msg(w, cm);
        }
        break;
    case client::chat_msg_CHANNEL_PRIVATE:// 私聊
        {
            private_chat_msg(w, cm);
        }
        break;
    case client::chat_msg_CHANNEL_HORN:
        {
            sub_horn_msg(w, cm);
        }
        break;
    case client::chat_msg_CHANNEL_FRIEND:
        {
            private_chat_msg(w, cm);
        }
        break;
    case client::chat_msg_CHANNEL_HORN_BROADCAST:
        {
            if (!is_client_req){
                horn_broadcast_chat_msg(w, cm);
            }
        }
        break;
    case client::chat_msg_CHANNEL_SYSTEM:
        {
            if (!is_client_req){
                system_chat_msg(w, cm);
            }
        }
        break;
    default:
        break;
    }

    return NULL;
}

void* on_pb_center_update_title_info_req(client::update_title_info_req* req, void* ud, size_t* rsp_len)
{
	center_service_t* w = (center_service_t*)ud;

	update_role_title_msg(w,req->role_id());

	return NULL;
}

void* on_pb_center_broadcast_area_to_center(client::broadcast_area_to_center* req, void* ud, size_t* rsp_len)
{
	center_service_t* w = (center_service_t*)ud;

	broadcast_area_rsp rsp_msg;
	rsp_msg.CopyFrom(req->broadcast_info());

	send_broadcast_area_msg(w,rsp_msg);

	return NULL;
}
/*void* on_pb_center_svr_chat_msg(client::svr_chat_msg* scm, void* ud, size_t* rsp_len)
{
return NULL;
}*/

#ifdef __INNER_DEBUG__PACKAGE_
static void split(std::string str, std::string pattern, std::vector<std::string>& result)
{
    std::string::size_type pos;
    str += pattern;
    uint32 size = str.size();
    for(uint32 i = 0; i < size; ++i) { 
        pos = str.find(pattern, i);
        if (pos < size) {
            std::string s = str.substr(i, pos - i);
            result.push_back(s);
            i = pos + pattern.size() - 1;
        }
    }
}
#endif

void* on_pb_center_query_req(client::query_req* bir, void* ud, size_t* rsp_len);
int init_cfgs_from_dot_bin_file();

static void test_command(center_service_t* w, client::client_chat_msg* cm)
{
    if (!w->_gm) {
       return ;
    }

#ifdef __INNER_DEBUG__PACKAGE_
    string str = cm->cm().content();
    std::vector<string> command;
    split(str, " ", command);
    if (command.size() < 3 && command[0] != "!") {
        return ;
    }

    client::test t;
    t.mutable_svr_head()->CopyFrom(cm->svr_head());

    if (command[1] == "add_hp") {
        int32 hp = atoi(command[2].c_str());
        t.set_hp(hp);
    } else if (command[1] == "sub_hp") {
        int32 hp = atoi(command[2].c_str());
        t.set_hp(-hp);
    } else if (command[1] == "add_mp") {
        int32 mp = atoi(command[2].c_str());
        t.set_mp(mp);
    } else if (command[1] == "sub_mp") {
        int32 mp = atoi(command[2].c_str());
        t.set_mp(-mp);
    } else if (command[1] == "add_prop") {
        uint32 prop_id = atoi(command[2].c_str());
        int32 prop_num = 1;
        t.set_prop_id(prop_id);
        if (command.size() > 3) {
            prop_num = atoi(command[3].c_str());
        }
        t.set_prop_num(prop_num);
    } else if (command[1] == "set_attr" && command.size() > 3) {
        uint32 attr_id = atoi(command[2].c_str());
        int32 attr_num = atoi(command[3].c_str());
        t.set_attr_id(attr_id);
        t.set_attr_num(attr_num);
    } else if (command[1] == "add_attr" && command.size() > 3) {
        uint32 attr_id = atoi(command[2].c_str());
        int32 attr_num = atoi(command[3].c_str());
        t.set_add_attr_id(attr_id);
        t.set_attr_num(attr_num);
    } else if (command[1] == "sub_attr" && command.size() > 3) {
        uint32 attr_id = atoi(command[2].c_str());
        int32 attr_num = atoi(command[3].c_str());
        t.set_sub_attr_id(attr_id);
        t.set_attr_num(attr_num);
    } else if (command[1] == "clear_attr" && command.size() > 1) {
        t.set_clear_add_attr(true);
    } else if (command[1] == "query") {
        uint32 id = atoi(command[2].c_str());
        client::query_req bir;
        bir.set_type(client::CQTE_SMY);
        bir.add_role_ids(id);
        on_pb_center_query_req(&bir, (void*)w, NULL);
    } else if (command[1] == "cfg") {
        /*system("cd /home/rockpile/7788/server/frame/pb/generate/;./cfg.sh");
        system("./copy.sh");
        FILE* pp = popen("cd /home/abel/6666_LGame/amtsvr/; ./restart.sh", "r");
        if (pp != NULL) {
        pclose(pp);
        }
        pp = popen("cd /home/abel/8888_LGame/amtsvr/; ./restart.sh", "r");
        if (pp != NULL) {
        pclose(pp);
        }*/
        return ;
    } else if (command[1] == "task") {
        uint32 task_id = atoi(command[2].c_str());
        t.set_task_id(task_id);
    } else if (command[1] == "sys_msg") {
        client::chat_msg cm;
        cm.set_channel(client::chat_msg_CHANNEL_SYSTEM);
        cm.set_src_rold_id(atoi(command[2].c_str()));
        if (command.size() > 3) {
            cm.set_content(command[3].c_str());
        }
        w->oi_mgr.brodcast(w->ctx, cm);
    } else if (command[1] == "get_gifts_req") {
        client::get_gifts_req req;
        req.set_reward_id(atoi(command[2].c_str()));
        req.mutable_svr_head()->CopyFrom(cm->svr_head());
        center_send_scene(w, req.svr_head().role_id(), req);
    } else if (command[1] == "abort") {
        GAME_EXIT = true;
        //amtsvr_command(NULL, "ABORT", NULL);
        return ;
    } else if (command[1] == "reload_config") {
        amtsvr_log("reload_config begin\n");
        init_cfgs_from_dot_bin_file();
        CONFMGR->reload();
        amtsvr_log("reload_config end\n");
        return ;
    } else if(command[1] == "add_drop"){
        int32 drop_id = atoi(command[2].c_str());
        t.set_drop_id(drop_id);
    } else if (command[1] == "land_day"){
		int32 land = atoi(command[2].c_str());
		t.set_land_day(land);
    }else if (command[1] == "export_cfg" && command.size() > 4) {
        t.set_cfg_file(command[2]);
        t.set_cfg_table(command[3]);
        t.set_cfg_field(command[4]);
    } else if (command[1] == "export_cfg") {
        t.set_cfg_file("all");
    }else if (command[1] == "reset_mount"){
        int32 mount_skill_count = atoi(command[2].c_str());
        t.set_mount_skill_count(mount_skill_count);
	} else if (command[1] == "send_arena_mail") {
		send_rank_reward(w);
    } else if (command[1] == "open_act") {
        int32 index = atoi(command[2].c_str());
        switch(index)
        {
        case 1:				/* 王都争霸 */
            w->ct_mgr->open_act(10);
            break;
        case 2:				/* 世界boss */
            w->bm_mgr.refresh_world_boss(w->ctx);
        default:
            break;
        }
    } else if (command[1] == "save_log") {
        amtsvr_sendname(w->ctx, SYS_LOG_NAME, PTYPE_SYSTEM, 0, (void*)"write_all", sizeof("write_all"));
        amtsvr_sendname(w->ctx, BILL_LOG_NAME, PTYPE_SYSTEM, 0, (void*)"write_all", sizeof("write_all"));
    } else if (command[1] == "reload_reg") {
        link_lua lua;
        lua.init_reg_pb();
        lua.do_reg();
    } else if (command[1] == "reload_lua") {

    } else if (command[1] == "lua") {
        std::string script;
        for (size_t i = 2; i < command.size(); ++i) {
            script += command[i];
        }

        t.set_lua(script);
    } else if (command[1] == "area_id"){
		int32 area_id = atoi(command[2].c_str());
		t.set_area_id(area_id);
    } else if (command[1] == "pet_id"){
		uint32 pet_id = atoi(command[2].c_str());
		uint32 pet_exp = 1;
		t.set_pet_id(pet_id);
		if (command.size() > 3) {
			pet_exp = atoi(command[3].c_str());
		}

		t.set_pet_exp(pet_exp);
    } else if (command[1] == "recharge") {
        t.set_money(atoi(command[2].c_str()));
    } else if (command[1] == "vip") {
		t.set_vip_lv(atoi(command[2].c_str()));
		if(command.size() == 4)
		{
			t.set_month(atoi(command[3].c_str()));
		}
		else
		{
			t.set_month(0);
		}
	} else if (command[1] == "illegal") {
		for(size_t i = 0; i < command.size() - 2; ++i)
		{
			w->iw_mgr.add_illegal_word(command[i + 2]);
		}
	} else if (command[1] == "raid") {
		t.set_raid_id(atoi(command[2].c_str()));
		if(command.size() == 4)
		{
			t.set_raid_type(atoi(command[3].c_str()));
		}
		else
		{
			t.set_raid_type(0);
		}
		if(command.size() == 5)
		{
			t.set_once(atoi(command[4].c_str()));
		}
		else
		{
			t.set_once(1);
		}
	}

    center_send_scene(w, cm->svr_head(), t);
#endif
}


