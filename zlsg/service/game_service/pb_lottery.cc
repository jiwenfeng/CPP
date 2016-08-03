#include "game_interface.h"
#include "player_role.h"
#include "config_lottery.h"
#include "client.lottery.pb.h"
#include "config_npc.h"
#include "config_mall.h"
#include "config_fallitem.h"

static void send_lottery_time(scene_user *su)
{
	player_role &pr = su->get_player_role();
	client::lottery_time_rsp rsp;
	int now = (int)time(NULL);
	int t1 = CONFMGR->get_basic_value(10180);
	int t2 = CONFMGR->get_basic_value(10181);
	uint32 g = max((int)pr.get_lottery_time(client::GOLD_BOX) + t1 - now, 0);
	uint32 b = max((int)pr.get_lottery_time(client::BRONZE_BOX) + t2 - now, 0);
	rsp.set_gold_time(g);
	rsp.set_bronze_time(b);
	su->send_client(rsp);
}

static uint32 get_treasure_box_id(uint32 type)
{
	switch(type)
	{
		case client::GOLD_BOX:
			return CONFMGR->get_basic_value(10176);
		case client::BRONZE_BOX:
			return CONFMGR->get_basic_value(10178);
		default:
			return 0;
	}
}

static uint32 get_treasure_key_id(uint32 type)
{
	switch(type)
	{
		case client::GOLD_BOX:
			return CONFMGR->get_basic_value(10177);
		case client::BRONZE_BOX:
			return CONFMGR->get_basic_value(10179);
		default:
			return 0;
	}
}

static uint32 get_treasure_cd(uint32 type)
{
	if(type == client::GOLD_BOX)
	{
		return CONFMGR->get_basic_value(10180);
	}
	return CONFMGR->get_basic_value(10181);
}

// 缺少的箱子数量
static int get_treasure_box_lack(scene_user *su, uint32 type, uint32 count)
{
	return max((int)count - (int)su->get_props_num(get_treasure_box_id(type)), 0);
}

// 缺少的钥匙数量
static int get_treasure_key_lack(scene_user *su, uint32 type, uint32 count)
{
	return max((int)count - (int)su->get_props_num(get_treasure_key_id(type)), 0);
}

// 需要的钱的数量
static int get_treasure_money_need(scene_user *su, uint32 type, int box, int key)
{
	int pay = type == client::GOLD_BOX ? 1 : 5;
	uint32 box_id = get_treasure_box_id(type);
	uint32 key_id = get_treasure_key_id(type);
	uint32 need = 0;
	if(box > 0)
	{
		const config_mall *cm = CONFMGR->get_config_mall(pay, box_id);
		if(cm == NULL)
		{
			return -1;
		}
		need += (cm->price_value * box);
	}
	if(key > 0)
	{
		const config_mall *cm = CONFMGR->get_config_mall(pay, key_id);
		if(cm == NULL)
		{
			return -1;
		}
		need += (cm->price_value * key);
	}
	return need;
}

static void deduct_treasure_cost(scene_user *su, uint32 type, uint32 count)
{
	uint32 key_id = get_treasure_key_id(type);
	uint32 box_id = get_treasure_box_id(type);
	uint32 key = su->get_props_num(key_id);
	uint32 box = su->get_props_num(box_id);
	uint32 lack_key = key >= count ? 0 : count - key;
	uint32 lack_box = box >= count ? 0 : count - box;
	if(key != 0)
	{
		su->sub_props(key_id, min(key, count));
	}
	if(box != 0)
	{
		su->sub_props(box_id, min(box, count));
	}
	uint32 need = get_treasure_money_need(su, type, lack_box, lack_key);
	if(need == 0)
	{
		return;
	}
	if(type == client::GOLD_BOX)
	{
		uint32 old_money = su->get_attr().get_val(INGOT);
		su->sub_money(INGOT, need);
		uint32 new_money = su->get_attr().get_val(INGOT);
		su->sub_money_log(MC_TREASURE_SPEND, MC_BUY_BOX, old_money, new_money);
		su->send_client_change_attr();
	}
	else
	{
		su->sub_money(TAEL, need);
		su->send_client_change_attr();
	}
}

static int check_treasure_cond(scene_user *su, uint32 type, uint32 count, uint32 auto_buy)
{
	int box = get_treasure_box_lack(su, type, count);
	int key = get_treasure_key_lack(su, type, count);
	if(box == 0 && key == 0)	// 神马都不缺
	{
		return 0;
	}
	if(auto_buy == 0)		/* 缺了还不买 */
	{
		return 1;
	}
	int pay = type == client::GOLD_BOX ? 1 : 5;
	int need = get_treasure_money_need(su, type, box, key);
	if(-1 == need)
	{
		return 5;
	}
	uint32 num; 
	if(pay == 1)
	{
		num = su->get_money(INGOT);
	}
	else
	{
		num = su->get_money(TAEL);
	}
	if(num < (uint32)need)		/* 想买钱不够 */
	{
		return pay == 1 ? 4 : 3;
	}
	return 0;
}

static int get_props_id(scene_user *su, int type, uint32 &prop_id, uint32 &num, bool spicial)
{
	player_role &pr = su->get_player_role();
	// 第一次开黄金宝箱给固定物品
	if(type == client::GOLD_BOX && pr.get_used_num() == 0)
	{
		switch(su->get_attr().get_val(PROFESSION))
		{
			case 0:
				prop_id = CONFMGR->get_basic_value(10165);
				num = 1;
				return 0;
			case 2:
				prop_id = CONFMGR->get_basic_value(10166);
				num = 1;
				return 0;
			default:
				return -1;
		}
	}
	uint32 lv = su->get_attr().get_val(LEVEL);
	uint32 job = su->get_attr().get_val(PROFESSION);
	const vector<uint32>& list = CONFMGR->get_config_lottery_list(type);
	vector<const config_lottery *> v;
	uint32 r = 0;
	for(size_t i = 0; i < list.size(); ++i)
	{
		const config_lottery *cl = CONFMGR->get_config_lottery(list[i]);
		if(cl == NULL)
		{
			continue;
		}
		if(lv < cl->get_lottery_min_lvl() || lv > cl->get_lottery_max_lvl())
		{
			continue;
		}
		if(pr.get_lottery_prop_num(list[i]) >= cl->get_lottery_max())
		{
			continue;
		}
		if(spicial)
		{
			if(cl->get_job() != job)
			{
				continue;
			}
			if(type == client::GOLD_BOX && cl->get_lottery_item_type() != 2)
			{
				continue;
			}
			if(type == client::BRONZE_BOX && cl->get_lottery_item_type() != 1)
			{
				continue;
			}
		}
		if(cl->get_lottery_probability() == 0)
		{
			continue;
		}
		r += cl->get_lottery_probability();
		v.push_back(cl);
	}
	uint32 rate = mtrandom::rand_int32(r);
	const config_lottery *c = NULL;
	uint32 n = 0;
	for(size_t i = 0; i < v.size(); ++i)
	{
		n += v[i]->get_lottery_probability();
		if(rate <= n)
		{
			c = v[i];
			break;
		}
	}
	if(c == NULL)
	{
		return -1;
	}
	if(c->is_group() == 1)
	{
		const drop_content *v = CONFMGR->get_config_packet(c->get_lottery_prop_id());
		if(v == NULL)
		{
			return -1;
		}
		prop_id = v->id;
		num = v->count;
	}
	else
	{
		prop_id = c->get_lottery_prop_id();
		num = 1;
	}
	return c->get_lottery_id();
}

// spicial 必出橙色/紫色
static int do_open(vector<client::props_info>& pis,scene_user *su, int type, client::lottery_open_rsp &rsp, bool spicial = false)
{
	uint32 prop_id = 0, num = 0;
	int ret = get_props_id(su, type, prop_id, num, spicial);
	if(ret == -1 || prop_id == 0 || num == 0)
	{
		return -1;
	}
	client::props_info *pi = rsp.add_pi();
	make_props(prop_id, num, *pi);
	pis.push_back(*pi);
	return ret;
}

static int do_once_open(scene_user *su, const client::lottery_open_req *req, client::lottery_open_rsp &rsp)
{
	player_role &pr = su->get_player_role();
	uint32 n = pr.get_lottery_time(req->type());
	bool flag = false;
	// 冷却中
	if(n + get_treasure_cd(req->type()) > (uint32)time(NULL))
	{
		int ret = check_treasure_cond(su, req->type(), req->times(), req->auto_buy());
		if(ret != 0)
		{
			rsp.set_ret((client::lottery_open_rsp_ret_e)ret);
			return -1;
		}
		flag = true;
	}
	vector<client::props_info> pis;
	pis.clear();
	int ret = do_open(pis, su, req->type(), rsp);
	if(ret == -1)
	{
		rsp.set_ret(client::lottery_open_rsp_ret_e_FAILURE);
		amtsvr_log("%u:open %d box failed\n", su->get_role_id(), req->type());
		return -1;
	}
	if(ret != 0)
	{
		pr.add_lottery_prop_num((uint32)ret);
	}
	su->add_props(pis);
	if(flag)
	{
		deduct_treasure_cost(su, req->type(), req->times());
	}
	else
	{

		pr.modify_lottery_time(req->type(), su->get_role_id());
		send_lottery_time(su);
	}
	if(req->type() == client::GOLD_BOX)
	{
		pr.modify_used_num(1);
	}
	rsp.set_ret(client::lottery_open_rsp_ret_e_SUCCESS);
	return 0;
}

static int do_repeat_open(scene_user *su, const client::lottery_open_req *req, client::lottery_open_rsp &rsp)
{
	int ret = check_treasure_cond(su, req->type(), req->times(), req->auto_buy());
	if(ret != 0)
	{
		rsp.set_ret((client::lottery_open_rsp_ret_e)ret);
		return -1;
	}
	player_role &pr = su->get_player_role();
	vector<client::props_info> pis;
	pis.clear();
	vector<uint32> v;
	for(uint32 i = 0; i < req->times(); ++i)
	{
		int ret = do_open(pis, su, req->type(), rsp, i == 1);
		if(ret == -1)
		{
			// 失败了要不要把寻宝次数减掉呢？先不要吧
			rsp.set_ret(client::lottery_open_rsp_ret_e_FAILURE);
			amtsvr_log("%u:open %d box failed\n", su->get_role_id(), req->type());
			return -1;
		}
		if(ret > 0)
		{
			v.push_back((uint32)ret);
		}
		if(req->type() == client::GOLD_BOX)
		{
			pr.modify_used_num(1);
		}
	}
	for(size_t i = 0; i < v.size(); ++i)
	{
		pr.add_lottery_prop_num(v[i]);
	}
	deduct_treasure_cost(su, req->type(), req->times());
	su->add_props(pis);
	rsp.set_ret(client::lottery_open_rsp_ret_e_SUCCESS);
	return 0;
}

void *on_pb_lottery_time_req(client::lottery_time_req *req, void *ud, size_t *rsp_len)
{
	PB_FUNC_GUARD(req);
	if(!psu->has_func_open(BOX_FUNC_ID))
	{
		return NULL;
	}
	send_lottery_time(psu);
	return NULL;
}

void *on_pb_lottery_open_req(client::lottery_open_req *req, void *ud, size_t *rsp_len)
{
	PB_FUNC_GUARD(req);
	if(!psu->has_func_open(BOX_FUNC_ID))
	{
		return NULL;
	}
	client::lottery_open_rsp rsp;
	if(!psu->is_can_add_props())
	{
		rsp.set_ret(client::lottery_open_rsp_ret_e_BAG_LIMIT);
		psu->send_client(rsp);
		return NULL;
	}
	if(req->times() == 1)
	{
		do_once_open(psu, req, rsp);
	}
	else
	{
		do_repeat_open(psu, req, rsp);
	}
	psu->send_client(rsp);
	if(rsp.ret() == 0)
	{
		CHECK_TARGET_PROGRESS(psu, TREASURE, req->times());
	}
	return NULL;
}
