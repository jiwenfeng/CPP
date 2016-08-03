#include "client.open.pb.h"
#include "mysql.t_newtask.pb.h"
#include "config_raid.h"
#include "config_quest.h"
#include "config_open.h"
#include "player_role.h"
#include "player_role_statis_mgr.h"
#include "operate_log.h"
#include "game_interface.h"
#include "scene_user.h"
#include "scene_inst.h"

void* on_pb_ctrl_scene_req(client::ctrl_scene_req* req, void* ud, size_t* rsp_len)
{
    PB_FUNC_GUARD(req);

    scene_inst* psi = psu->get_scene();
    if (psi == NULL || psi->get_scene_type() != MAP_TYPE_RAID) {
        return NULL; 
    }

    switch (req->type()) {
    case client::ctrl_scene_req_ctrl_type_suspend: {
        uint32 ut = req->has_unixtime() ? req->unixtime() : 30000;
        psi->suspend(ut);
        break;
    }
    case client::ctrl_scene_req_ctrl_type_resume: {
        psi->resume();
        break;
    }
    default: break;
    }

    client::ctrl_scene_rsp rsp;
    rsp.set_ret(0);
    rsp.mutable_req()->CopyFrom(*req);
    rsp.mutable_req()->mutable_svr_head()->Clear();
    psu->send_client(rsp);
    
    return NULL;
}

static void task_log(scene_user* su, client::submit_task_req* req)
{
    server::operate_log log;
    server::operate_task* ot = log.mutable_ot();
    ot->set_sid(req->svr_head().svr_id());
    ot->set_role_id(su->get_role_id());
    ot->set_task_id(req->task_id());
    ot->set_status(1);
    ot->set_quality(0);
    ot->set_create_date((uint32)time(NULL));
    su->get_player_role().get_logs().push(log);
}

void fill_task_msg_accepteds(client::task_msg_type t, const std::map<uint32, client::task_accepted>& accepteds, client::task_msg& msg)
{
    msg.set_t(t);
    std::map<uint32, client::task_accepted>::const_iterator i = accepteds.begin();
    while (i != accepteds.end()) {
        msg.add_accepted()->CopyFrom(i->second);
        ++i;
    }
}

void fill_task_msg_completeds(client::task_msg_type t, const std::map<uint32, client::task_completed>& completeds, client::task_msg& msg)
{
    msg.set_t(t);
    std::map<uint32, client::task_completed>::const_iterator i = completeds.begin();
    while (i != completeds.end()) {
        msg.add_completed()->CopyFrom(i->second);
        ++i;
    } 
}

void fill_task_msg(client::task_msg_type t, const std::map<uint32, client::task_accepted>& accepteds, const std::map<uint32, client::task_completed>& completeds, client::task_msg& msg)
{
    fill_task_msg_accepteds(t, accepteds, msg);
    fill_task_msg_completeds(t, completeds, msg);
}

void fill_task_msg_accepted(client::task_msg_type t, const client::task_accepted& accepted, client::task_msg& msg)
{
    msg.set_t(t);
    msg.add_accepted()->CopyFrom(accepted);
}

void fill_task_msg_completed(client::task_msg_type t, const client::task_completed& completed, client::task_msg& msg)
{
    msg.set_t(t);
    msg.add_completed()->CopyFrom(completed);
}

void* on_pb_accept_task_req(client::accept_task_req* req, void* ud, size_t* rsp_len)
{
    PB_FUNC_GUARD(req);

    if (req->task_id() == 0) {
        client::accept_task_rsp rsp;
        rsp.set_task_id(req->task_id());
        rsp.set_ret(TR_TASK_ERROR);
        game_send_client(ss, req->svr_head(), rsp);
        return NULL;
    }

    player_role_task& tasks = psu->get_task().get_player_role_task();
    std::map<uint32, client::task_accepted>* pat = tasks.mutable_accepted_tasks();
    client::task_accepted& a = (*pat)[req->task_id()];
    a.set_task_id(req->task_id());
    a.mutable_progress();
    client::accept_task_rsp rsp;
    rsp.set_task_id(req->task_id());
    rsp.set_ret(TR_SUCCESS);
    game_send_client(ss, req->svr_head(), rsp);

    psu->get_task().init_task(a);

    amtsvr_billlog(ss->ctx, "%u|%s|%u|%u|%u", psu->get_role_id(), req->GetTypeName().c_str(), req->task_id(), rsp.task_id(), (uint32)rsp.ret());

    return NULL;
}

bool task_is_complete(client::task_accepted& task, const config_quest* config)
{
    return true;
    // TODO: 此函数没有完成 可移至scene_role_task类中
    if (config == NULL) {
        return false;
    }

    if (!task.has_progress()) {
        return false;
    }

    switch (config->get_quest_type())
    {
    case QT_TALK: return true;
    case QT_RAID: return task.progress().raid_size() != 0;
    case QT_PICK: {
        break;
                  }
    case QT_KILL: {
        break;
                  }
    case QT_GUARD: {
        break;
                   }
    case QT_LEVEL: {
        break;
                   }
    case QT_EVENT: {
        break;
                   }

    }

    return true;
}

void on_quest_complete(scene_user* psu, const config_quest* complete_quest)
{
    const config_open_mgr* mgr = GET_CONFIG_MGR(config_open_mgr);
    if (mgr == NULL) {
        return ;
    }

    std::map<uint32, client::open_info>* opens = psu->get_player_role().get_tasks().mutable_opens();
    const std::map<uint32, config_open*>& cfg = mgr->get_cfg();
    std::map<uint32, config_open*>::const_iterator begin = cfg.begin();
    std::map<uint32, config_open*>::const_iterator end = cfg.end();
	client::open_func_req of;
	of.set_role_id(psu->get_role_id());
    for (; begin != end; ++begin) {
        const config_open* pco = begin->second;
        if (pco == NULL || pco->get_open_type() != config_open::OPEN_TYPE_QUEST) {
            continue;
        }

        if (complete_quest->get_quest_id() == (uint32)pco->get_open_condition()) {
            (*opens)[pco->get_func_id()].set_id(pco->get_func_id());
            client::open_msg om;
            om.set_t(client::open_msg_type_add);
            om.add_id()->CopyFrom((*opens)[pco->get_func_id()]);
            psu->send_client(om);
			of.add_id()->CopyFrom((*opens)[pco->get_func_id()]);
            break;
        }
    }

	if(of.id_size() > 0) {
		psu->send_center(of);
	}

    // 记录完成任务日志
    mysql::t_newtask log;
    log.set_id(0);
    log.set_sid(psu->get_player_role().sid());
    log.set_did("");
    log.set_accountname(psu->get_player_role().account());
    log.set_playername(psu->get_role_name());
    log.set_taskid(complete_quest->get_quest_id());
    log.set_status(1);
    log.set_quality(1);
    log.set_createdate((uint32)time(NULL));
    psu->send_log(log);
}

void accept_task(std::map<uint32, client::task_accepted>* pat, uint32 next_task_id, scene_user* psu)
{
    //uint32 next_task_id = req->task_id() + 1;
    const config_quest* quest = CONFMGR->get_config_quest_mgr().get_config_quest(next_task_id);
    if (quest == NULL) {// && quest->_accept_type == 0) {
        return ;
    }

    client::task_accepted& a = (*pat)[next_task_id];
    a.set_task_id(next_task_id);
    a.mutable_progress();
    task_msg msg;
    fill_task_msg_accepted(client::task_msg_type_insert, a, msg);
    psu->send_client(msg);
    psu->get_task().init_task(a);

    // 记录接受任务日志
    mysql::t_newtask log;
    log.set_id(0);
    log.set_sid(psu->get_player_role().sid());
    log.set_did("");
    log.set_accountname(psu->get_player_role().account());
    log.set_playername(psu->get_role_name());
    log.set_taskid(next_task_id);
    log.set_status(0);
    log.set_quality(1);
    log.set_createdate((uint32)time(NULL));
    psu->send_log(log);
}

void complete_task(const config_quest* complete_quest, scene_user* psu)
{
    psu->add_normal_exp(complete_quest->get_reward_exp());
    psu->add_money(MILITARY_STRATEGY, complete_quest->get_reward_taolue());
    psu->add_money(INGOT, complete_quest->get_reward_gold());
    psu->add_money(PHYSICAL_POWER, complete_quest->get_reward_energy());
    psu->add_money(TAEL, complete_quest->get_reward_coin());
    psu->send_client_change_attr();
    std::vector<client::props_info> items;
    complete_quest->get_reward_item(psu->get_attr(PROFESSION), items);
    psu->add_props(items);
    if (!items.empty()) {
        psu->send_client_change_props_msg();
    }

    on_quest_complete(psu, complete_quest);
}

void* on_pb_submit_task_req(client::submit_task_req* req, void* ud, size_t* rsp_len)
{
    PB_FUNC_GUARD(req);

	if (!psu->is_can_add_props()){
		return NULL;
	}

    player_role_task& tasks = psu->get_task().get_player_role_task();
    std::map<uint32, client::task_accepted>* pat = tasks.mutable_accepted_tasks();
    std::map<uint32, client::task_accepted>::iterator i = pat->find(req->task_id());
    if (i == pat->end()) {
        return NULL;
    }

    const config_quest* complete_quest = CONFMGR->get_config_quest_mgr().get_config_quest(req->task_id());
    if (!task_is_complete(i->second, complete_quest)) {
        return NULL;
    }

    if (complete_quest == NULL) {
        return NULL;
    }
	// 发送奖励
    complete_task(complete_quest, psu);

	// 发送旧的任务信息移除
    task_msg msg;
	if (req->has_task_flag()){
		msg.set_task_flag(req->task_flag());
	}
    fill_task_msg_accepted(client::task_msg_type_remove, i->second, msg);
    psu->send_client(msg);

    pat->erase(i);
    psu->get_task().remove_task_condition(req->task_id());

    std::map<uint32, client::task_completed>* pct = tasks.mutable_completed_tasks();
    client::task_completed& completed = (*pct)[req->task_id()];
    completed.set_task_id(req->task_id());
    completed.set_count(completed.count() + 1);
    completed.set_time((uint32)time(NULL));

    msg.Clear();
    if ((*pct)[req->task_id()].count() == 1) {
        fill_task_msg_completed(client::task_msg_type_insert, completed, msg);
    } else {
        fill_task_msg_completed(client::task_msg_type_update, completed, msg);
    }

	if (req->has_task_flag()){
		msg.set_task_flag(req->task_flag());
	}

    psu->send_client(msg);
	// 接下一个任务，发送接新的任务的信息
    accept_task(pat, complete_quest->get_next_quest_id(), psu);

    task_log(psu, req);
    amtsvr_billlog(ss->ctx, "%u|%s|%u", psu->get_role_id(), req->GetTypeName().c_str(), req->task_id());

    return NULL;
}

void* on_pb_talk_task_req(client::talk_task_req* req, void* ud, size_t* rsp_len)
{
    PB_FUNC_GUARD(req);

    player_role_task& tasks = psu->get_task().get_player_role_task();
    std::map<uint32, client::task_accepted>* pat        = tasks.mutable_accepted_tasks();
    std::map<uint32, client::task_accepted>::iterator i = pat->find(req->task_id());
    if (i == pat->end()) {
        return NULL;
    }

    client::task_talk* ptalk = i->second.mutable_progress()->add_talk();
    ptalk->set_npc_id(req->npc_id());

    client::talk_task_rsp rsp;
    rsp.set_ret(TR_SUCCESS);
    rsp.set_task_id(req->task_id());
    psu->send_client(rsp);
    
    client::task_msg msg;
    fill_task_msg_accepted(client::task_msg_type_update, i->second, msg);
    psu->send_client(msg);

    amtsvr_billlog(ss->ctx, "%u|%s|%u|%u", psu->get_role_id(), req->GetTypeName().c_str(), req->task_id(), req->npc_id());

    return NULL;
}

void* on_game_pb_story_end_req(client::story_end_req* req, void* ud, size_t* rsp_len)
{
    PB_FUNC_GUARD(req);

    scene_inst* psi = psu->get_scene();
    if (psi == NULL) {
        return NULL;
    }

    if (psi->get_scene_type() != MAP_TYPE_RAID) {
        return NULL;
    }

    psi->on_story_end(psu, req->id());

    return NULL;
}
