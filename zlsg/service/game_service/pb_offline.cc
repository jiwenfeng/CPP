#include "config_raid.h"
#include "client.offline.pb.h"
#include "mysql.t_offline.pb.h"
#include "player_role.h"
#include "scene_user.h"
#include "scene_inst.h"
#include "game_interface.h"
#include "config_upgrade.h"
#include "player_role_statis_mgr.h"

static void offline_log(scene_user* psu)
{
    mysql::t_offline log;
    log.set_id(0);
    log.set_sid(psu->get_player_role().sid());
    log.set_did("");
    log.set_accountname(psu->get_player_role().account());
    log.set_playername(psu->get_role_name());
    log.set_playerlevel(psu->get_level());
    log.set_ip("");
    if (psu->get_attr(OFFLINE_TIME) > psu->get_attr(ONLINE_TIME)) {
        log.set_onlinetime(psu->get_attr(OFFLINE_TIME) - psu->get_attr(ONLINE_TIME));
    } else {
        log.set_onlinetime(1);
    }
    psu->set_attr(HISTORY_ONLINE, psu->get_attr(HISTORY_ONLINE) + log.onlinetime());
    log.set_historyonline(psu->get_attr(HISTORY_ONLINE));
    log.set_createdate((uint32)time(NULL));
    psu->send_log(log);
}

void* stop_comm_practice(struct game_service_t* s, scene_user* su)
{
    scene_inst* si = su->get_scene();
    if (si == NULL){
        return NULL;
    }
    
    const scene_role_status& status = su->get_status();
    
    if (status.comm_practice()) {
        su->stop_practice();
        scene_user* dst_su = si->get_scene_user(su->get_comm_practice_user());
        if (dst_su == NULL){
            return NULL;
        }
        //dst_su->stop_practice();
        dst_su->practice();
        return NULL;
    }

    return NULL;
}

void game_user_offline(struct game_service_t* s, scene_user* su)
{
    if (su == NULL) {
        return;
    }

    client::offline o;
    uint32 role_id = su->get_role_id();

    o.set_user_id(0);
    o.set_role_id(su->get_role_id());
    o.set_scene_server_id(0);
    client::server_head* head = o.mutable_svr_head();
    head->set_role_id(role_id);
    head->set_agent_id(su->get_agent_id());
    head->set_client_id(su->get_client_id());

    game_user_offline(s, su, su->get_scene(), &o);
    
    su = NULL;
}

void game_user_offline(struct game_service_t* s, scene_user* su, scene_inst* si, client::offline* p)
{
    if (su == NULL || p == NULL) {
        return;
    }

    if (!p->has_active() || !p->active()) {
        game_close_agent(s, su->get_agent_id());
    }
    
    su->set_offline_time((uint32)time(NULL));
    su->set_skill_point_time((uint32)time(NULL));

    offline_log(su);

    if (si != NULL) {
        su->save_user();
        s->sim->logout_scene(su, si);
    } else {
        su->detach_scene();
    }

    su->get_player_role().write_all(s->ctx);
    su->fill_query_attr(su->get_player_role().get_query_attr());
    player_role* pr = su->detach_player_role();
    delete su;
    su = NULL;

    if (pr != NULL) {
        server::record_offline_user ro;
        uintptr_t ptr = (uintptr_t)pr;
        ro.set_ptr((uint64)ptr);
        if (game_send_center(s, ro) < 0) {
            delete pr;
        }
    }

    client::offline o(*p);
    game_send_center(s, o);
}

void* on_pb_offline(client::offline* p, void* ud, size_t* rsp_len)
{
    struct game_service_t* s = (struct game_service_t*)ud;
    scene_user* su = s->sim->get_user(p->svr_head().role_id());
    if (su == NULL || su->get_agent_id() != p->svr_head().agent_id()) { // 要判断是当前客户端连接断开
        su = s->sim->get_space_user(p->svr_head().role_id());
        if (su == NULL || su->get_agent_id() != p->svr_head().agent_id()) {
            return NULL;
        } else {
            s->sim->go_to_scene(su);
        }
    }

#ifdef _WIN32
    char* str = U2G(su->get_role_name());
    amtsvr_log("role_id:%u, role_name:%s\n", su->get_role_id(), str);
    delete str;
#else
    amtsvr_log("role_id:%u, role_name:%s\n", su->get_role_id(), su->get_role_name());
#endif

    scene_inst* si = su->get_scene();
    stop_comm_practice(s, su);
    game_user_offline(s, su, si, p);

    return NULL;
}

void *on_pb_offline_timelong_req(client::offline_timelong_req *req, void *ud, size_t *len)
{
	PB_FUNC_GUARD(req);
	client::offline_timelong_rsp rsp;
	player_role_statis_mgr &mgr = psu->get_player_role().get_role_statis_mgr();
	rsp.set_time(mgr.get_offline_timelong());
	rsp.set_total(mgr.get_total_offline_tl());
	psu->send_client(rsp);
    amtsvr_billlog(ss->ctx, "%u|%s", psu->get_role_id(), req->GetTypeName().c_str());
	return NULL;
}

void *on_pb_offline_reward_req(client::offline_reward_req *req, void *ud, size_t *len)
{
	PB_FUNC_GUARD(req);
	const config_upgrade* pcu = CONFMGR->get_config_upgrade(psu->get_job(), psu->get_level());
	if(pcu == NULL)
	{
		return NULL;
	}
	player_role_statis_mgr &mgr = psu->get_player_role().get_role_statis_mgr();
	client::offline_reward_rsp rsp;
	uint32 sec = mgr.get_offline_timelong();
	if(sec < 60)
	{
		rsp.set_ret(1);
		psu->send_client(rsp);
		return NULL;
	}
	switch(req->type())
	{
		case 1:
			break;
		case 2:
			{
				uint32 n = psu->get_attr().get_val(TAEL);
				uint32 need = (uint32)CONFMGR->get_basic_value(10153);
				if(n < need)
				{
					return NULL;
				}
				psu->mutable_attr()->set_val(TAEL, n - need);
			}
			break;
		case 3:
			{
				uint32 n = psu->get_attr().get_val(INGOT);
				uint32 need = (uint32)CONFMGR->get_basic_value(10154);
				if(n < need)
				{
					return NULL;
				}
				psu->mutable_attr()->set_val(INGOT, n - need);
				uint32 new_money = psu->get_attr().get_val(INGOT);
				psu->sub_money_log(MC_OFFLINE_SPEND, MC_BUY_OFFLINE_EXP, n, new_money);
			}
			break;
		default:
			return NULL;
	}
	uint32 minute = (uint32)floor((float)sec / 60.0);
	uint32 exp = (uint32)floor((float)minute * pcu->get_qa_reward_exp() / 100.0) * req->type();
	uint32 taolve = (uint32)floor((float)minute * pcu->get_qa_reward_strategy() / 100.0) * req->type();
	psu->add_normal_exp(exp);
	psu->mutable_attr()->add_val(MILITARY_STRATEGY, taolve);
	psu->send_client_change_attr();
	mgr.add_total_offline_tl(sec);
	mgr.reset_offline_timelong();
	rsp.set_ret(0);
	psu->send_client(rsp);
    amtsvr_billlog(ss->ctx, "%u|%s|%u|%u|%u|%u", psu->get_role_id(), req->GetTypeName().c_str(), req->type(), sec, exp, taolve);
	return NULL;
}
