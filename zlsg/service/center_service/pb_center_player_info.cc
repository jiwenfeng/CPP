#include "config_map.h"
#include "client.trans_user.pb.h"
#include "client.role_login.pb.h"
#include "client.node.pb.h"
#include "player_role_status.h"
#include "player_role_mail.h"
#include "player_role.h"
#include "offline_role_mgr.h"
#include "role_key.h"
#include "center_interface.h"
#include "player_role_mounts.h"
#include "player_role_vip.h"

static void proc_user_online_friend(center_service_t* w, player_role* r, uint32 role_id)
{
    client::friend_on_off_notify foon;
    foon.set_role_id(role_id);
    foon.set_online(true);
    player_role_friends* src_friends = w->fm_mgr.mut(role_id);
    if (src_friends == NULL) {
        return ;
    }

    std::map<uint32, client::friend_info>* p = src_friends->get_friend();
    std::map<uint32, client::friend_info>::iterator i = p->begin();
    for (; i != p->end(); ++i) {
        center_send_client_by_role_id(w, i->second.role_id(), foon);
    }
}

static void proc_user_online_team(center_service_t* w, player_role* r, uint32 role_id)
{
#if 0
    client::team_member_on_off_notify tmoon;
    tmoon.set_role_id(role_id);
    tmoon.set_online(1);

    client::team_info* ti = w->tm_mgr.get_my_team(role_id);
    if (ti == NULL || r == NULL) {
        return ;
    }

    r->set_team_id(ti->id());
    std::vector<uint32> roles;
    if (w->tm_mgr.get_team_members(ti->id(), roles) != 0) {
        return;
    }

    for (uint32 i = 0; i < roles.size(); ++i) {
        center_send_client_by_role_id(w, roles[i], tmoon);
    }
#endif
}

static void proc_user_online_mail(center_service_t* w, player_role* r, uint32 role_id)
{
    mail_center* mc = w->wg_mcm.mutable_mail(role_id);
    if (mc == NULL) {
        return;
    }

    player_role_mail& prm = r->get_role_mails();
    std::vector<role_mail_info>& mi = mc->mut_mails();
    uint32 size = mi.size();
    if (size == 0) {
        return;
    }

    mc->set_change();
    for (uint32 i = 0; i < mi.size(); ++i) {
        if (!mi[i].is_normal()) {
            continue;
        }

        prm.add_mail(role_id, mi[i]);
    }

    mi.clear();
}

static void proc_user_online(center_service_t* w, player_role* r, center_role* cr, client::server_head* svr_head)
{
    mysql::tb_player_role* tpr = r->get_db_player_role();
    uint32 role_id = tpr->role_id();
    const string& role_name = tpr->role_name();

    // 更新在线信息
    online_role oi(role_id, 
        svr_head->scenesvr_id(), 
        svr_head->agent_id(), 
        svr_head->client_id(), 
        role_name, 
        r->get_role_status().get_db_role_status()->pos_scene_id(),
        r->get_player_role_vip().get_vip_exp());
    //online_role oi(role_id, svr_head->scenesvr_id(), svr_head->agent_id(), svr_head->client_id(), role_name, tpr->map_id());
    w->oi_mgr.update(oi);

    online_role* new_oi =  w->oi_mgr.mutable_online_info(role_id);
    if (new_oi == NULL) {
        return;
    }

    new_oi->attach(cr);

    // 记录全局摘要信息
    mysql::sp_load_all_user_smy smy;
    smy.set_role_id(role_id);
    smy.set_role_name(role_name);
    smy.set_status(1);
    smy.set_fighting_power(r->get_db_player_role()->fighting_power());
    smy.set_profession(r->get_db_player_role()->profession());
    smy.set_level(r->get_db_player_role()->level());
	smy.set_weapon_id(r->get_db_player_role()->weapon_id());
	smy.set_armor_id(r->get_db_player_role()->armor_id());
	smy.set_mount_level(r->get_mounts().get_db().mount_level());
	smy.set_mount_power(r->get_mounts().get_db().mount_power());
	smy.set_vip_level(r->get_db_player_role()->vip_level());
    w->aus_mgr.record(smy);

	// 添加到竞技场
//    if (r->get_db_player_role()->level() >= ARENA_MIN_LVL &&  w->ar_mgr.is_not_add(role_id))
//	{
//		w->ar_mgr.add_role(role_id, 0);
//  }

    // 删除离线用户数据
    w->ou_mgr->remove(role_id);
    w->_role_key->del_key_time(role_id);

    // 从邮件中心获取离线邮件信息
    proc_user_online_mail(w, r, role_id);

    // 通知好友上线
    proc_user_online_friend(w, r, role_id);
    proc_user_online_team(w, r, role_id);
}

static int role_login_scene(center_service_t* w, player_role* pr, center_role* cr, uint32 agent_id, uint32 client_id)
{
    if (pr == NULL || pr->get_db_player_role() == NULL) {
        return -1;
    }

    uint32 now = (uint32)time(NULL);
    if (pr->get_db_player_role()->status() == player_role::ROLE_BAN_LOGIN
        && 
        now < pr->get_db_player_role()->is_suspended_account()) {
        w->bu_mgr.add_login(pr->get_db_player_role()->role_id(), pr->get_db_player_role()->is_suspended_account());
        return -1;
    }

    if (pr->get_db_player_role()->status() == player_role::ROLE_BAN_CHAT
        && 
        now < pr->get_db_player_role()->is_suspended_account()) {
        w->bu_mgr.add_chat(pr->get_db_player_role()->role_id(), pr->get_db_player_role()->is_suspended_account());
    }

#ifdef _WIN32
    char* str = U2G(pr->get_db_player_role()->role_name().c_str());
    amtsvr_log("role_id:%u, role_name:%s\n", pr->get_role_id(), str);
    delete str;
#else
    amtsvr_log("role_id:%u, role_name:%s\n", pr->get_role_id(), pr->get_db_player_role()->role_name().c_str());
#endif

    //amtsvr_billlog(w->ctx, "%d|%s|%d|%s", pr->get_role_id(), __FILE__, __LINE__, __FUNCTION__);

    pr->set_online_time((uint32)time(NULL));
     
    uint32 map_id        = 0;
    uint32 scene_id      = 0;
    uint32 scene_inst_id = 0;
    int ret = pr->get_role_status().init_now_pos(map_id, scene_id, scene_inst_id);
    if (ret != 0) {
        return -1;
    }

    uint32 scene_service_id = w->wm_mgr.get_scene_service_id(scene_id);
    if (scene_service_id == 0) {
        scene_service_id = w->wm_mgr.get_scene_service_id(CONFMGR->get_config_map_default()->get_id());
    }

    if (scene_service_id == 0) {
        return -2;
    }

    msg_handler* mh = MSG_HANDLER_POOL->pop();
    if (mh == NULL) {
        return -3;
    }

    client::trans_user* wtr = new client::trans_user;
    uintptr_t ptr = (uintptr_t)pr;
    wtr->set_ptr((uint64)ptr);
    wtr->set_map_id(map_id);
    wtr->set_scene_id(scene_id);
    wtr->set_scene_inst_id(scene_inst_id);
    client::server_head* svr_head = wtr->mutable_svr_head();
    svr_head->set_scenesvr_id(scene_service_id);
    svr_head->set_agent_id(agent_id);
    svr_head->set_client_id(client_id);
    mh->msg = wtr;

    proc_user_online(w, pr, cr, svr_head);

    if (amtsvr_send(w->ctx, 0, scene_service_id, PTYPE_CLIENT|PTYPE_TAG_DONTCOPY, 0, mh, sizeof(msg_handler)) < 0) {
        return -3;
    }

    return 0;
}

// 本次登录为用户自创建角色,或者场景中其它用户场景非法跳转回的数据
void* on_pb_center_player_info(client::trans_user* tr, void* ud, size_t* rsp_len)
{
    center_service_t* w   = (center_service_t*)ud;
    uintptr_t         ptr = (uintptr_t)tr->ptr();
    player_role*      pr  = (player_role*)(ptr);
    if (pr == NULL) {
        return NULL;
    }

    center_role* cr = NULL;
    online_role* oi = w->oi_mgr.mutable_online_info(pr->get_role_id());
    if (oi != NULL) {
        cr = oi->pop_center_role();
    } else {
        cr = new center_role;
    }

    if (role_login_scene(w, pr, cr, tr->svr_head().agent_id(), tr->svr_head().client_id()) != 0) {
        delete pr;
        return NULL;
    }

    return NULL;
}

// 之前有的角色登录
void* on_after_role_login(mysql::sp_login_auth* sla, void* ud, size_t* rsp_len)
{
    center_service_t* w       = (center_service_t*)ud;
    uint32            role_id = sla->v_role_id();
    online_role*      oi      = w->oi_mgr.mutable_online_info(role_id);
    if (oi != NULL && oi->agent_id != sla->p_agent_id()) {
        client::repeat_login_msg msg;
        msg.set_type(0);
        center_send_client(w, oi->client_id, msg);

        center_close_agent(w, *oi);

        oi->agent_id  = sla->p_agent_id();
        oi->client_id = sla->p_client_id();
        mysql::sp_login_auth esla(*sla);
        center_send_scene(w, *oi, esla);

        return NULL;
    }

    player_role* pr = NULL;
    center_role* cr = NULL;
    w->ou_mgr->pop(role_id, pr, cr);
    if (pr == NULL) {
        center_load_role_req(w, role_id, *sla);
        return NULL;
    }

    pr->account(sla->v_name());

    /*if (pr->get_level() >= ARENA_MIN_LVL) {
        w->ar_mgr.push_new(pr->get_role_id());
    }*/

    //pr->set_yellow_vip(sla->p_yel_vip());
    if (role_login_scene(w, pr, cr, sla->p_agent_id(), sla->p_client_id()) != 0) {
        delete pr;
        return NULL;
    }

    return NULL;
}

center_role* get_center_role(center_service_t* w, uint32 role_id)
{
    online_role* poi = w->oi_mgr.mutable_online_info(role_id);
    if (poi != NULL) {
        return poi->mutable_center_role();
    } 

    offline_role* pou = w->ou_mgr->mutable_role(role_id);
    if (pou != NULL) {
        return pou->get_cr();
    }

    return NULL;
}

void* on_center_pb_update_auto_login_key(client::update_auto_login_key* via, void* ud, size_t* rsp_len)
{
    center_service_t* w = (center_service_t*)ud;
    w->_role_key->set_key(via->role_id(), via->key());
//     center_role* pcr = get_center_role(w, via->role_id());
//     if (pcr == NULL) {
//         return NULL;
//     }
// 
//     pcr->set_key(via->key());
    return NULL;
}

void* on_center_pb_auto_login_req(client::auto_login_req* req, void* ud, size_t* rsp_len)
{
    center_service_t* w = (center_service_t*)ud;
//     center_role* pcr = get_center_role(w, req->role_id());
//     if (pcr == NULL) {
//         return NULL;
//     }

    client::auto_login_rsp rsp;

//     if (false && pcr->get_key() != req->key()) {
    const std::string& key = w->_role_key->find(req->role_id());
    if (key == "" || key != req->key()) {
        rsp.set_ret(client::auto_login_rsp_result_error);
        center_send_client(w, req->svr_head(), rsp);
        return NULL;
    }

    client::update_agent_role uar;
    uar.set_role_id(req->role_id());
    center_send_agent(w, req->svr_head().agent_id(), uar);

    mysql::sp_login_auth sp;
    sp.set_v_role_id(req->role_id());
    sp.set_p_agent_id(req->svr_head().agent_id());
    sp.set_p_client_id(req->svr_head().client_id());
    on_after_role_login(&sp, ud, rsp_len);

    char str[32] = {0};
    snprintf(str, 32, "%c%d%c%c%c", (rand() % 25 + 65), rand(), (rand() % 25 + 65), (rand() % 25 + 65), (rand() % 25 + 65));
    rsp.set_ret(client::auto_login_rsp_result_success);
    rsp.set_key(str);
    w->_role_key->set_key(req->role_id(), rsp.key());
    center_send_client(w, req->svr_head(), rsp);

    return NULL;
}

void* on_center_pb_yellow_vip_info_req(client::yellow_vip_info_req* req, void* ud, size_t* rsp_len)
{
    center_service_t* w = (center_service_t*)ud;
    yellow_vip_info_rsp rsp;
    int32 size = req->role_id_size();
    for (int32 i = 0; i < size; ++i) {
        client::yellow_vip_info* yvi = rsp.add_yvi();
        yvi->set_role_id(req->role_id(i));
        yvi->set_lvl(5);
        yvi->set_year(true);
    }

    center_send_client(w, req->svr_head(), rsp);

    return NULL;
}

void* on_center_pb_user_data(client::user_data* req, void* ud, size_t* rsp_len)
{
    center_service_t* w       = (center_service_t*)ud;
    online_role*      oi = w->oi_mgr.mutable_online_info(req->svr_head().role_id());
    if (oi != NULL) {
        return NULL;
    }

    mysql::tb_player_role* pr_db = new mysql::tb_player_role;
    pr_db->ParseFromString(req->db());

    player_role* pr = new player_role(pr_db);
    center_role* cr = NULL;

    if (role_login_scene(w, pr, cr, req->svr_head().agent_id(), req->svr_head().client_id()) != 0) {
        delete pr;
        return NULL;
    }

    return NULL;
}