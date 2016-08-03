#include "player_role_vip.h"
#include "player_role.h"
#include "game_interface.h"
#include "scene_user.h"
#include "scene_inst.h"
#include "client.vip.pb.h"
#include "config_vip.h"
#include "config_pay.h"
#include "config_recharge.h"
#include "player_role_raid_mgr.h"
#include "player_role_mounts.h"
#include "config_upgrade.h"
#include "config_mounts.h"
#include "config_mounts_skill.h"
#include "client.mounts.pb.h"
#include "client.devil.pb.h"
#include "player_role_cycle.h"
#include "player_role_devil.h"

typedef int(*cb)(game_service_t *, scene_user *, const config_pay *, client::vip_op_req *);

static map<uint32, cb> _op_cb;

static int buy_physical(game_service_t *ss, scene_user *su, const config_pay *cp, client::vip_op_req *);
static int buy_copper(game_service_t *ss, scene_user *su, const config_pay *cp, client::vip_op_req *);
static int buy_challenge_count(game_service_t *ss, scene_user *su, const config_pay *cp, client::vip_op_req *);
static int buy_carbon2_times(game_service_t *ss, scene_user *su, const config_pay *c, client::vip_op_req *p);
static int buy_skill_point(game_service_t *ss, scene_user *su, const config_pay *cp, client::vip_op_req *);
static int reset_mount_times(game_service_t *ss, scene_user *su, const config_pay *cp, client::vip_op_req *);
static int reset_cycle_level(game_service_t *ss, scene_user *su, const config_pay *cp, client::vip_op_req *);


static int buy_physical(game_service_t *ss, scene_user *su, const config_pay *cp, client::vip_op_req *req)
{
	su->add_money(PHYSICAL_POWER, cp->_get);
	CHECK_TARGET_PROGRESS(su, BUY_PHYSICAL_POWER);
	su->send_client_change_attr();
	return 0;
}

static int buy_copper(game_service_t *ss, scene_user *su, const config_pay *cp, client::vip_op_req *req)
{

    const config_upgrade* cu = CONFMGR->get_config_upgrade(su->get_attr().get_val(PROFESSION), su->get_attr().get_val(LEVEL));
	if(cu == NULL)
	{
		return -1;
	}
	su->add_money(TAEL, cp->_get * cu->get_buy_coin());
	su->send_client_change_attr();
	CHECK_TARGET_PROGRESS(su, BUY_COPPER);
	return 0;
}

static int buy_challenge_count(game_service_t *ss, scene_user *psu, const config_pay *cp, client::vip_op_req *req)
{
	int old_count = psu->get_arena_used_times();
	int total = CONFMGR->get_basic_value(10019);
	if(old_count != total)
	{
		return -1;
	}
	uint32 old_money = psu->get_attr().get_val(INGOT);
	psu->reset_arena_used_times();
	int new_count = psu->get_arena_used_times();
	amtsvr_billlog(ss->ctx, "%u|client.arena_buy_challenge_count|%u|%u|%u|%u|%u|%u", psu->get_role_id(), total - old_count, total - new_count, cp->_need, old_money, old_money - cp->_need);
	return 0;
}

static int reset_mount_times(game_service_t *ss, scene_user *su, const config_pay *cp, client::vip_op_req *)
{
    player_role_mounts& m = su->get_player_role().get_mounts();
    mysql::tb_role_mounts* pdb = m.mutable_db();
	pdb->set_skill_upgrade_num(CONFMGR->get_basic_value(10051));
	return 0;
}

// 重置精英副本次数
static int buy_carbon2_times(game_service_t *ss, scene_user *su, const config_pay *cp, client::vip_op_req *req)
{
	player_role_raid* prr = su->get_player_role().get_raid_mgr().mutable_raid(req->raid_id());
	if(!prr)
	{
		return -1;
	}
    prr->mutable_raid()->set_elite_enter_count(0);
	client::raid_info_msg msg;
	client::raid_info *ri = msg.add_ri();
	const google::protobuf::Descriptor *raid_des = prr->mutable_raid()->GetDescriptor();
	const google::protobuf::Reflection *raid_ref = prr->mutable_raid()->GetReflection();
	const google::protobuf::Descriptor *ri_des = ri->GetDescriptor();
	const google::protobuf::Reflection *ri_ref = ri->GetReflection();
	for(int i = 1; i < raid_des->field_count(); ++i)
	{
		switch(raid_des->field(i)->type())
		{
			case google::protobuf::FieldDescriptor::TYPE_UINT32:
				{
					uint32 n = raid_ref->GetUInt32(*prr->mutable_raid(), raid_des->field(i));
					ri_ref->SetUInt32(ri, ri_des->field(i - 1), n);
				}
				break;
			default:
				break;
		}
	}
	su->send_client(msg);
	return 0;
}

static int buy_skill_point(game_service_t *ss, scene_user *su, const config_pay *cp, client::vip_op_req *req)
{
	su->mutable_attr()->add_val(ROLE_SKILL_POINT, cp->_get);
	return 0;
}

static int reset_cycle_level(game_service_t *ss, scene_user *su, const config_pay *cp, client::vip_op_req *req)
{
	player_role_cycle &prc = su->get_player_role().get_role_cycle();
	return prc.reset_level();
}

static int reset_devil_level(game_service_t *ss, scene_user *su, const config_pay *cp, client::vip_op_req *req)
{
	player_role_devil &prd = su->get_player_role().get_role_devil();
	if(0 != prd.reset_raid(req->raid_id()))
	{
		return -1;
	}
	client::devil_raid_detail drd;
	if(0 != prd.fill_devil_raid_info(req->raid_id(), drd))
	{
		return -1;
	}
	su->send_client(drd);
	CHECK_TARGET_PROGRESS(su, DEVIL_RESET);
	return 0;
}

void register_op_cb()
{
	_op_cb.insert(make_pair(client::OP_BUY_PHYSICAL, buy_physical));
	_op_cb.insert(make_pair(client::OP_BUY_COPPER, buy_copper));
	_op_cb.insert(make_pair(client::OP_BUY_CHALLANGE_TIMES, buy_challenge_count));
	_op_cb.insert(make_pair(client::OP_BUY_CARBON2_TIMES, buy_carbon2_times));
	_op_cb.insert(make_pair(client::OP_BUY_SKILL_POINT, buy_skill_point));
	_op_cb.insert(make_pair(client::OP_RESET_MOUNT_TIMES, reset_mount_times));
	_op_cb.insert(make_pair(client::OP_RESET_CYCLE_LEVEL, reset_cycle_level));
	_op_cb.insert(make_pair(client::OP_RESET_DEVIL_LEVEL, reset_devil_level));
}

void *on_pb_vip_exp_req(client::vip_exp_req *req, void *ud, size_t *rsp_len)
{
	PB_FUNC_GUARD(req);
	player_role_vip &vip = psu->get_player_role().get_player_role_vip();
	client::vip_exp_rsp rsp;
	rsp.set_exp(vip.get_vip_exp());
	psu->send_client(rsp);
    amtsvr_billlog(ss->ctx, "%u|%s", psu->get_role_id(), req->GetTypeName().c_str());
	return NULL;
}

void *on_pb_vip_op_times_req(client::vip_op_times_req *req, void *ud, size_t *rsp_len)
{
	PB_FUNC_GUARD(req);
	uint32 op = req->op();
	if(op < client::OP_BUY_PHYSICAL || op >= client::OP_END)
	{
		return NULL;
	}
	player_role_vip &vip = psu->get_player_role().get_player_role_vip();
	client::vip_op_times_rsp rsp;
	rsp.set_op(req->op());
	uint32 raid_id = 1;
	if(req->op() == client::OP_BUY_CARBON2_TIMES)
	{
		raid_id = req->raid_id();
	}
	rsp.set_times(vip.get_op_count(req->op(), raid_id));
	psu->send_client(rsp);
    amtsvr_billlog(ss->ctx, "%u|%s|%u|%u", psu->get_role_id(), req->GetTypeName().c_str(), req->op(), raid_id);
	return NULL;
}

void *on_pb_vip_op_req(client::vip_op_req *req, void *ud, size_t *rsp_len)
{
	PB_FUNC_GUARD(req);
	uint32 op = req->op();
	if(op < client::OP_BUY_PHYSICAL || op >= client::OP_END)
	{
		return NULL;
	}
	client::vip_op_rsp rsp;
	rsp.set_op(req->op());
	player_role_vip &v = psu->get_player_role().get_player_role_vip();
	map<uint32, cb>::iterator it = _op_cb.find(req->op());
	if(it == _op_cb.end())
	{
		return NULL;
	}
	uint32 raid_id = 1;
	if(req->op() == client::OP_BUY_CARBON2_TIMES)
	{
		raid_id = req->raid_id();
	}
	if(!v.op_has_left_times(req->op(), raid_id))
	{
		rsp.set_ret(client::vip_op_rsp_op_e_TIMES_LIMIT);
		psu->send_client(rsp);
		return NULL;
	}
	uint32 t = v.get_op_count(req->op(), raid_id);
	const config_pay *pay = CONFMGR->get_config_pay(req->op(), t + 1);
	if(NULL == pay)
	{
		return NULL;
	}
	uint32 old_money = psu->get_attr().get_val(INGOT);
	if(old_money < pay->_need)
	{
		rsp.set_ret(client::vip_op_rsp_op_e_INGOT_LIMIT);
		psu->send_client(rsp);
		return NULL;
	}
	int ret = it->second(ss, psu, pay, req);
	if(0 != ret)
	{
		rsp.set_ret(client::vip_op_rsp_op_e_FAILED);
		psu->send_client(rsp);
		return NULL;
	}
	psu->sub_money(INGOT, pay->_need);
	uint32 new_money = psu->get_attr().get_val(INGOT);
	psu->sub_money_log(MC_VIP_SPEND, req->op(), old_money, new_money);
	rsp.set_ret((client::vip_op_rsp_op_e)ret);
	psu->send_client_change_attr();
	psu->send_client(rsp);
	v.set_op_count(req->op(), raid_id);
    amtsvr_billlog(ss->ctx, "%u|%s", psu->get_role_id(), req->GetTypeName().c_str());
	return NULL;
}

void *on_pb_vip_buy_info_req(client::vip_buy_info_req *req, void *ud, size_t *rsp_len)
{
	PB_FUNC_GUARD(req);
	const std::map<uint32, config_recharge *> l = CONFMGR->get_config_recharge_list();
	std::map<uint32, config_recharge *>::const_iterator i;
	client::vip_buy_info_rsp rsp;
	for(i = l.begin(); i != l.end(); ++i)
	{
		client::recharge_info *bi = rsp.add_ri();
		bi->set_id(i->first);
		bi->set_num(psu->get_player_role().get_player_role_vip().get_recharge_times(i->first));
	}

	uint32 tt = psu->get_player_role().get_player_role_vip().get_month_timeout();
	uint32 now = (uint32)time(NULL);
	if(now < tt)
	{
		rsp.set_tick(tt - now);
	}
	else
	{
		rsp.set_tick(0);
	}
	psu->send_client(rsp);
    amtsvr_billlog(ss->ctx, "%u|%s", psu->get_role_id(), req->GetTypeName().c_str());
	return NULL;
}
