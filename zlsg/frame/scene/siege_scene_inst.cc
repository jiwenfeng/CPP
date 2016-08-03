#include "scene_npc.h"
#include "scene_user.h"
#include "config_raid.h"
#include "siege_scene_inst.h"
#include "random.h"
#include "game_map_mgr.h"
#include "config_npc.h"
#include "config_robot_ai.h"

siege_scene_inst::siege_scene_inst(uint32 id, const game_map* p, const battle_scene* s, struct amtsvr_context* c, const int m)
	: battle_scene_inst(id, p, s, c, m), _status(SIEGE_PREPARE)
{
	_cur = 0;
	_atk_robot_num = 0;
	_def_robot_num = 0;
}

siege_scene_inst::~siege_scene_inst()
{
	_role_list.clear();
	_family_list.clear();
	_tu.clear();
	_cur = 0;
	_atk_robot_num = 0;
	_def_robot_num = 0;
}

void siege_scene_inst::set_open_time(uint32 time)
{
	_open_time = time;
}

void siege_scene_inst::set_family_id(uint32 family_id)
{
	_family_id = family_id;
}

uint32 siege_scene_inst::get_family_id()
{
	return _family_id;
}

void siege_scene_inst::set_occupy_time(uint32 time)
{
	_occupy_time = time;
}

uint32 siege_scene_inst::get_occupy_time()
{
	return _occupy_time;
}

void siege_scene_inst::set_close_time(uint32 time)
{
	_close_time = time;
}

uint32 siege_scene_inst::get_close_time()
{
	return _close_time;
}

uint32 siege_scene_inst::get_open_time()
{
	return _open_time;
}

void siege_scene_inst::siege_start(client::siege_start *ss)
{
	set_open_time(ss->open_time());
	set_close_time(ss->close_time());
	set_status(SIEGE_START);
	_family_list.clear();
	for(int i = 0; i < ss->fsi_size(); ++i)
	{
		client::family_simple_info fsi = ss->fsi(i);
		_family_list.insert(make_pair(fsi.family_id(), fsi));
	}
	if(ss->family_id() != 0)
	{
		occupy_flag(ss->family_id());
	}
	else
	{
		refresh_flag();
		reset_role_camp();
	}
	send_random_trans_msg();
	init_scene_user();
}

void siege_scene_inst::occupy_flag(uint32 family_id)
{
	set_occupy_time(time_utils::tenms());
	add_status(SIEGE_OCCUPY);
	set_family_id(family_id);
	refresh_flag();
	reset_role_camp();
	reset_robots_ai();
	count_camp_num();
}

void siege_scene_inst::count_camp_num()
{
	_atk_robot_num = _def_robot_num = 0;
	vector<scene_role *> v;
	get_scene_roles(v);
	for(size_t i = 0; i < v.size(); ++i)
	{
		if(v[i]->is_robot() && !v[i]->get_status().is_dead())
		{
			if(v[i]->get_camp() == ATTACK)
			{
				_atk_robot_num++;
			}
			else
			{
				_def_robot_num++;
			}
		}
	}
}

void siege_scene_inst::reset_robots_ai()
{
	vector<scene_role *> v;
	get_scene_roles(v);
	for(size_t i = 0; i < v.size(); ++i)
	{
		if(v[i]->is_robot() && !v[i]->get_status().is_dead())
		{
			reset_ai(static_cast<scene_npc *>(v[i]));
		}
	}
}

void siege_scene_inst::refresh_flag()
{
	uint32 npc_id = query_family_flag(get_family_id());
    const origin_npc* o = GAME_MAP_MGR->get_npc(npc_id);
	if(o == NULL)
	{
        amtsvr_log("scene:%u npc:%u not found\n", get_scene_id(), npc_id);
		return;
	}
	map_postion p(3864, 1748, 0);
	scene_npc *npc = generate_npc_inst(*o, (uint32)time(NULL), p);
	if(NULL == npc)
	{
		amtsvr_log("flag enter scene failed\n");
		return;
	}
	if(IS_OCCUPIED())
	{
		const string& name = get_family_name(get_family_id());
		npc->set_role_name(name.c_str(), name.length());
	}
	screen_push_npc(npc);
}

void siege_scene_inst::send_siege_info(scene_role *sr)
{
	client::siege_scene_camp ssc;
	ssc.set_camp(0);
	uint32 now = time_utils::tenms();
	ssc.set_open_time((now - get_open_time()) / 100);

	if(!IS_OCCUPIED())
	{
		sr->send_client(ssc);
		return;
	}
	ssc.set_occupy_time((now - get_occupy_time()) / 100);
	map<uint32, family_simple_info>::iterator it = _family_list.find(get_family_id());
	if(it != _family_list.end())
	{
		ssc.set_family_name(it->second.family_name());
		ssc.set_user_name(it->second.user_name());
	}
	sr->send_client(ssc);
}

void siege_scene_inst::update_role_camp(scene_role *sr, bool notify)
{
	sr->set_camp(get_role_camp(sr));
	sr->set_attack_mode(client::role_status_attack_mode_attack_mod_camp);
	sr->broadcast_pet_attr();
	sr->send_client_change_attr();
	if(notify)
	{
		send_siege_info(sr);
	}
}

void siege_scene_inst::sys_combo_msg(uint32 msg_id, const char *w, const char *l /* = NULL */)
{
	if(w == NULL)
	{
		return;
	}
	vector<client::content_data> v;
	client::content_data dc1, dc2;
	dc1.set_s(w);
	v.push_back(dc1);
	if(NULL != l)
	{
		dc2.set_s(l);
		v.push_back(dc2);
	}
	scene_announcement(msg_id, v);
}

const string &siege_scene_inst::get_family_name(uint32 family_id)
{
	map<uint32, client::family_simple_info>::iterator it = _family_list.find(family_id);
	if(it != _family_list.end())
	{
		return it->second.family_name();
	}
	static string ret = "";
	return ret;
}

uint32 siege_scene_inst::query_family_flag(uint32 family_id)
{
	map<uint32, family_simple_info>::iterator it = _family_list.find(family_id);
	uint32 id = it == _family_list.end() ? 1 : it->second.flag_id();
	return CONFMGR->get_config_flag_id(id);
}

void siege_scene_inst::on_role_enter(scene_role* psr)
{
	if(IS_START() && psr->get_role_type() == RTE_USER)
	{
		send_siege_info(psr);
	}
	return battle_scene_inst::on_role_enter(psr);
}

int siege_scene_inst::init_scene_user()
{
	vector<scene_user *> v;
	get_scene_users(v);
	for(size_t i = 0; i < v.size(); ++i)
	{
		add_scene_user(v[i]);
	}
	return 0;
}

int siege_scene_inst::add_scene_user(scene_user *su)
{
	if(su == NULL)
	{
		return 0;
	}
	uint64 inst_id = su->get_inst_id();
	map<uint64, client::siege_personal_info>::iterator it = _role_list.find(inst_id);
	if(it == _role_list.end())
	{
		client::siege_personal_info cpi;
		cpi.set_tick(0);
		cpi.set_cur_combo(0);
		cpi.set_role_id(su->get_role_id());
		cpi.set_user_name(su->get_role_name());
		cpi.set_exp(0);
		cpi.set_timelong(0);
		cpi.set_region(get_role_region(su));
		_role_list.insert(make_pair(inst_id, cpi));
	}
	else
	{
		it->second.set_region(get_role_region(su));
	}
	return 0;
}

int siege_scene_inst::enter_scene(scene_user* su)
{
	if(IS_START())
	{
		su->set_camp(get_role_camp(su));
		reset_role_pos(su);
		su->set_attack_mode(client::role_status_attack_mode_attack_mod_camp);
		add_scene_user(su);
	}
	return battle_scene_inst::enter_scene(su);
}

int siege_scene_inst::enter_scene(scene_npc *sn)
{
	if(sn->is_robot())
	{
		uint32 camp = get_role_camp(sn);
		sn->set_camp(camp);
		if(camp == ATTACK)
		{
			_atk_robot_num++;
		}
		else
		{
			_def_robot_num++;
		}
		sn->set_attack_mode(client::role_status_attack_mode_attack_mod_camp);
		reset_role_pos(sn);
	}
	return scene_inst::enter_scene(sn);
}

int siege_scene_inst::leave_scene(scene_npc *sn)
{
	if(sn->is_robot())
	{
		uint32 camp = get_role_camp(sn);
		if(camp == ATTACK)
		{
			_atk_robot_num --;
			if(_atk_robot_num < 0)
			{
				_atk_robot_num = 0;
			}
		}
		else
		{
			_def_robot_num--;
			if(_def_robot_num < 0)
			{
				_def_robot_num = 0;
			}
		}
	}
	return scene_inst::leave_scene(sn);
}

void siege_scene_inst::reset_role_pos(scene_role *sr)
{
	int camp = get_role_camp(sr);
	const rect &r = CONFMGR->get_config_raid_mgr().get_revive_rect(get_scene_id(), camp);
	int x = r.top_left.x + mtrandom::rand_int32(r.width());
	int y = r.top_left.y + mtrandom::rand_int32(r.height());
	map_postion mp(x, y, 0);
	sr->set_scene_pos(mp);
	sr->set_born_pos(mp);
}

uint32 siege_scene_inst::get_role_region(scene_role *sr)
{
	if(NULL == sr)
	{
		return 0;
	}
	const map_postion &p = sr->get_scene_pos();
	const std::vector<exp_region> &er = CONFMGR->get_config_raid_mgr().get_exp_region(get_scene_id());
	for(size_t i = 0; i < er.size(); ++i)
	{
		for(size_t j = 0; j < er[i]._r.size(); ++j)
		{
			if(er[i]._r[j].is_in_range(p))
			{
				return (uint32)er[i]._rate;
			}
		}
	}
	return 0;
}

int siege_scene_inst::reset_role_region(scene_role *r)
{
	uint32 region = get_role_region(r);
	if(region == 0)
	{
		return 0;
	}
	map<uint64, client::siege_personal_info>::iterator it = _role_list.find(r->get_inst_id());
	if(it == _role_list.end())
	{
		return 0;
	}
	if(it->second.region() != region)
	{
		it->second.set_region(region);
		it->second.set_timelong(0);
	}
	return 0;
}

int siege_scene_inst::on_role_move(scene_role* r, const map_postion& from, const map_postion& to)
{
	if(IS_START() && r->get_role_type() == RTE_USER)
	{
		reset_role_region(r);
	}
	return scene_inst::on_role_move(r, from, to);
}

int siege_scene_inst::leave_scene(scene_user *su)
{
	su->set_camp(0);
	if(IS_OVER() || !IS_START())
	{
		return battle_scene_inst::leave_scene(su);
	}
	map<uint64, client::siege_personal_info>::iterator it = _role_list.find(su->get_inst_id());
	if(it != _role_list.end())
	{
		it->second.set_cur_combo(0);
		it->second.set_timelong(0);
		it->second.set_region(0);
	}
	su->pick_remove();
	vector<scene_user *>::iterator i = find(_tu.begin(), _tu.end(), su);
	if(i != _tu.end())
	{
		_tu.erase(i);
	}
	return battle_scene_inst::leave_scene(su);
}

int siege_scene_inst::on_one_second_update(uint32 tick)
{
	int sec = CONFMGR->get_basic_value(10013);
	if(sec <= 0)
	{
		return 0;
	}
	map<uint64, client::siege_personal_info>::iterator it;
	for(it = _role_list.begin(); it != _role_list.end(); ++it)
	{
		scene_user *su = get_scene_user(it->first);
		if(NULL == su)
		{
			continue;
		}
		it->second.set_tick(it->second.tick() + 1);
		it->second.set_timelong(it->second.timelong() + 1);
		if(it->second.timelong() % sec == 0)
		{
			uint32 lv = su->get_attr().get_val(LEVEL);
			uint32 job = su->get_attr().get_val(PROFESSION);
			uint32 exp = CONFMGR->get_config_city_war_exp(job, lv) * it->second.region();
			if(exp == 0)
			{
				continue;
			}
			su->add_normal_exp(exp);
			it->second.set_exp(it->second.exp() + exp);
		}
	}
	return 0;
}

int siege_scene_inst::update(uint32 tick, bool is_one_sec, bool is_fiv_sec, bool is_ten_sec, bool is_thirty_sec, bool is_one_minute)
{
	if(IS_START())
	{
		if(!IS_TRANS() && ((get_open_time() + 500) <= tick))
		{
			random_trans_users();
			add_status(SIEGE_TRANS);
		}
		if(is_one_sec)
		{
			on_one_second_update(tick);
		}
		if((tick >= get_close_time()) || (IS_OCCUPIED() && tick >= get_occupy_time() + CONFMGR->get_basic_value(10045)))
		{
			close();
		}
		if((tick % (uint32)CONFMGR->get_basic_value(10209)) <= 10)
		{
			load_ghost();
			_cur++;
		}
	}
	if(IS_OVER() && is_scene_empty())
	{
		set_close();
	}
	return battle_scene_inst::update(tick, is_one_sec, is_fiv_sec, is_ten_sec, is_thirty_sec, is_one_minute);
}

void siege_scene_inst::reset_role_camp()
{
	vector<scene_role *> v;
	get_scene_roles(v);
	for(size_t i = 0; i < v.size(); ++i)
	{
		if(v[i]->get_role_type() == RTE_USER || v[i]->is_robot())
		{
			v[i]->clear_counter_list();
			update_role_camp(v[i], !v[i]->is_robot());
		}
	}
}

void siege_scene_inst::close()
{
	clear_npc();
	send_siege_over();
	reset_role_camp();
	set_status(SIEGE_OVER);
	set_close_time(time_utils::tenms());
	_role_list.clear();
	_family_list.clear();
}

uint32 siege_scene_inst::get_msg_id()
{
	if(IS_OCCUPIED())
	{
		if(time_utils::tenms() < get_close_time())
		{
			return 26;
		}
		else
		{
			return 27;
		}
	}
	return 28;
}

void siege_scene_inst::send_siege_over()
{
	client::siege_over so;
	so.set_family_id(get_family_id());
	so.set_msg_id(get_msg_id());
	map<uint64, client::siege_personal_info>::iterator it = _role_list.begin(); // 
	int tl = CONFMGR->get_basic_value(10046) / 100;
	for(; it != _role_list.end(); ++it)
	{
		if((int)it->second.tick() >= tl)
		{
			so.add_role(it->second.role_id());
		}
	}
	send_center(so);
}

void siege_scene_inst::send_dead_msg(scene_role *atk, scene_role *def)
{
	client::siege_dead_msg_rsp rsp;
	if(NULL == atk || atk == def)
	{
		rsp.set_user_name("");
		rsp.set_family_name("");
		def->send_client(rsp);
		return;
	}
	rsp.set_user_name(atk->get_role_name());
	uint32 family_id = atk->get_family_id();
	rsp.set_family_name(get_family_name(family_id));
	def->send_client(rsp);	// 发送死亡提示
}

int siege_scene_inst::on_kill_role(scene_role *r_atk, scene_role *r_def)
{
	if(r_def == NULL)
	{
		return scene_inst::on_kill_role(r_atk, r_def);
	}
	if(!IS_START())
	{
		relive(r_def);
		return scene_inst::on_kill_role(r_atk, r_def);
	}
	if(r_def->get_role_type() == RTE_PET)
	{
		return scene_inst::on_kill_role(r_atk, r_def);
	}
	send_dead_msg(r_atk, r_def);
	if(r_def->get_role_type() == RTE_USER)
	{
		static_cast<scene_user *>(r_def)->pick_remove();
	}
	calc_combo(r_atk, r_def);
	return scene_inst::on_kill_role(r_atk, r_def);
}

int siege_scene_inst::reset_role_combo(scene_role *sr1, scene_role *sr2)
{
	if(NULL == sr2)
	{
		return -1;
	}
	map<uint64, client::siege_personal_info>::iterator i = _role_list.find(sr2->get_inst_id());
	if(i == _role_list.end())
	{
		return -1;
	}
	const char *win_name = NULL;
	if(NULL == sr1)
	{
		win_name = sr1->get_role_name();
	}
	const char *lose_name = sr2->get_role_name();
	switch(i->second.cur_combo())
	{
		case 3:sys_combo_msg(34, win_name, lose_name); break;
		case 10:sys_combo_msg(35, win_name, lose_name); break;
		case 20:sys_combo_msg(36, win_name, lose_name); break;
		case 50:sys_combo_msg(37, win_name, lose_name); break;
		case 100:sys_combo_msg(38, win_name, lose_name); break;
	}
	i->second.set_cur_combo(0);
	return 0;
}

int siege_scene_inst::add_role_combo(scene_role *sr)
{
	if(NULL == sr)
	{
		return -1;
	}
	map<uint64, client::siege_personal_info>::iterator it = _role_list.find(sr->get_inst_id());
	if(it == _role_list.end())
	{
		return -1;
	}
	const char *name = sr->get_role_name();
	it->second.set_cur_combo(it->second.cur_combo() + 1);
	switch(it->second.cur_combo())
	{
		case 3:sys_combo_msg(29, name);break;
		case 10:sys_combo_msg(30, name);break;
		case 20:sys_combo_msg(31, name);break;
		case 50:sys_combo_msg(32, name);break;
		case 100:sys_combo_msg(33, name);break;
		default:break;
	}
	return 0;
}

int siege_scene_inst::calc_combo(scene_role *sr1, scene_role *sr2)
{
	if(NULL == sr2 || (sr2->get_role_type() != RTE_USER && !sr2->is_robot()))
	{
		return 0;
	}
	scene_role *master = sr1->get_master();
	if(NULL == master)
	{
		master = sr1;
	}
	if(master != sr2)
	{
		add_role_combo(master);
	}
	reset_role_combo(master, sr2);
	return 0;
}

bool siege_scene_inst::is_alliance(scene_role *sr0, scene_role *sr1)
{
	if(!is_friend(sr0, sr1))
	{
		return false;
	}
	return get_role_camp(sr0) == get_role_camp(sr1);
}

bool siege_scene_inst::is_friend_family(uint32 f1, uint32 f2)
{
	if(f1 == 0)
	{
		return false;
	}
	if(f1 == f2)
	{
		return true;
	}
	std::map<uint32, client::family_simple_info>::iterator it = _family_list.find(f1);
	if(it == _family_list.end())
	{
		return false;
	}
	for(int i = 0; i < it->second.alliance_size(); ++i)
	{
		if(f2 == it->second.alliance(i))
		{
			return true;
		}
	}
//	for(int i = 0; i < it->second.fl_size(); ++i)
//	{
//		if(f2 == it->second.fl(i).family_id())
//		{
//			return true;
//		}
//	}
	return false;
}

/*
 * 0 中立 1 进攻 2 防守
 */
int siege_scene_inst::get_family_camp(uint32 family_id)
{
	if(0 == family_id)
	{
		return ATTACK;
	}
	uint32 occupy = get_family_id();
	if(0 == occupy)
	{
		return ATTACK;
	}
	if(family_id == occupy)
	{
		return DEFENCE;
	}
	std::map<uint32, client::family_simple_info>::iterator it = _family_list.find(occupy);
	if(it == _family_list.end())
	{
		return ATTACK;
	}
	for(int i = 0; i < it->second.alliance_size(); ++i)
	{
		if(family_id == it->second.alliance(i))
		{
			return DEFENCE;
		}
	}
#if 0
	for(int i = 0; i < it->second.fl_size(); ++i)
	{
		if(family_id == it->second.fl(i).family_id())
		{
			return DEFENCE;
		}
	}
#endif
	return ATTACK;
}

int siege_scene_inst::get_role_camp(scene_role *sr)
{
	return get_family_camp(sr->get_family_id());
}

void siege_scene_inst::send_random_trans_msg()
{
	client::siege_random_trans srt;
	srt.set_unixtime(5);
	scene_broadcast(srt);
	get_scene_users(_tu);
}

void siege_scene_inst::random_trans_users()
{
	vector<cell_pos> cp;
	get_cell().get_all_empty_cell(cp);
	if(cp.empty())
	{
		return;
	}
	int s = cp.size();
	for(size_t i = 0; i < _tu.size(); ++i)
	{
		map_postion mp = to_map_postion(cp[mtrandom::rand_int32(s - 1)]);
		trans_postion(_tu[i], mp);
	}
	_tu.clear();
}

int siege_scene_inst::on_pick_npc(scene_user* psu, scene_npc* psn)
{
	client::siege_flag_occupy req;
	req.set_old_family_id(get_family_id());
	req.set_new_family_id(psu->get_family_id());
	send_center(req);

	vector<client::content_data> v;
	client::content_data dc;
	dc.set_s(get_family_name(psu->get_family_id()));
	v.push_back(dc);
	scene_announcement(45, v);

	occupy_flag(psu->get_family_id());
	return 0;
}

void siege_scene_inst::on_pick(scene_user *psu, uint64 inst_id)
{
	client::pick_rsp rsp;
	rsp.set_role_id(psu->get_role_id());
	rsp.set_inst_id(inst_id);
	rsp.set_action(client::PAE_ADD);
	if(!IS_START())
	{
		rsp.set_ret(PIRE_ACT_CLOSE);
		psu->send_client(rsp);
		return ;
	}
	uint32 family_id = psu->get_family_id();
	if(family_id == 0)
	{
		rsp.set_ret(PIRE_OFFICIAL_LIMIT);
		psu->send_client(rsp);
		return;
	}
	std::map<uint32, family_simple_info>::iterator it = _family_list.find(family_id);
	if(it == _family_list.end() || it->second.role_id() != psu->get_role_id())
	{
		rsp.set_ret(PIRE_OFFICIAL_LIMIT);
		psu->send_client(rsp);
		return ;
	}
	if(get_family_id() == family_id)
	{
		rsp.set_ret(PIRE_TARGET_IS_MINE);
		psu->send_client(rsp);
		return ;
	}
	if(psu->is_friend_family(get_family_id()))
	{
		rsp.set_ret(PIRE_ALLIANCE);
		psu->send_client(rsp);
		return ;
	}
	client::family_money_query_req req;
	req.set_family_id(family_id);
	req.set_role_id(psu->get_role_id());
	req.set_inst_id(inst_id);
	send_center(req);
}

void siege_scene_inst::do_pick(client::family_money_query_rsp *rsp)
{
	scene_user *psu = get_scene_user_by_role_id(rsp->role_id());
	if(NULL == psu)
	{
		return;
	}
	int need = CONFMGR->get_basic_value(10042);
	client::pick_rsp pr;
	pr.set_role_id(rsp->role_id());
	pr.set_inst_id(rsp->inst_id());
	pr.set_action(client::PAE_ADD);
	if(rsp->money() < need)
	{
		pr.set_ret(PIRE_MONEY_LIMIT);
		psu->send_client(pr);
		return;
	}
	client::PICK_RET_E ret = psu->pick(rsp->inst_id());
	if(ret == client::PIRE_SUCCESS)
	{
		client::family_money_mod_req req;
		req.set_family_id(rsp->family_id());
		req.set_money(rsp->money() - need);
		send_center(req);
	}
	pr.set_ret(ret);
	psu->send_client(pr);
}

bool siege_scene_inst::relive(scene_role *sr)
{
	if(sr == NULL)
	{
		return false;
	}
	if(!IS_START())
	{
		if(sr->get_role_type() == RTE_USER)
		{
			client::dead_in_raid d;
			d.set_time(0);
			sr->send_client(d);
		}
		return false;
	}
	int camp = get_role_camp(sr);
	const rect &r = CONFMGR->get_config_raid_mgr().get_revive_rect(get_scene_id(), camp);
	const map_postion& cur = sr->get_scene_pos();
	if(r.is_in_range(cur))
	{
		if(sr->get_role_type() == RTE_USER)
		{
			client::dead_in_raid d;
			d.set_time(0);
			sr->send_client(d);
		}
		return false;
	}
	sr->set_attack_mode(client::role_status_attack_mode_attack_mod_camp);
	int x = r.top_left.x + mtrandom::rand_int32(r.width());
	int y = r.top_left.y + mtrandom::rand_int32(r.height());
	map_postion mp(x, y, 0);
	trans_postion(sr, mp);
	return true;
}

int siege_scene_inst::fill_personal_info(scene_user *su, client::siege_personal_info_rsp &rsp)
{
	std::map<uint64, client::siege_personal_info>::iterator it = _role_list.find(su->get_inst_id());
	if(it == _role_list.end())
	{
		return -1;
	}
	uint32 lv = su->get_attr().get_val(LEVEL);
	uint32 job = su->get_attr().get_val(PROFESSION);
	uint32 exp = CONFMGR->get_config_city_war_exp(job, lv);
	rsp.set_region(it->second.region());
	rsp.set_exp(it->second.exp());
	rsp.set_time(it->second.tick());
	rsp.set_add(exp);
	return 0;
}

void siege_scene_inst::on_role_leave(scene_role *sr)
{
	if(sr->get_role_type() != RTE_USER)
	{
		return scene_inst::on_role_leave(sr);
	}
	scene_user *su = static_cast<scene_user *>(sr);
	if (su->mutable_buffer()->del_raid_buf()) {
		uint32 count = 0;
		su->mutable_buffer()->update(time_utils::tenms(), count);
		client::buffer_list bl;
		su->mutable_buffer()->fill_change_buffer_list(bl);
		su->fill_role_data(bl.mutable_rd());
		screen_broadcast(*su, bl, false);
		su->calculate_attr();
		su->calculate_status();
		su->send_client_change_attr();
	}
	return scene_inst::on_role_leave(sr);
}

// 军团模式的时候同军团和盟军团不可攻击
// 全体模式攻击所有人
// 阵营模式只能攻击非本阵营玩家
// 优先判断是否反击
bool siege_scene_inst::is_friend(scene_role *psr0, scene_role *psr1)
{
	if(psr0 == NULL || psr1 == NULL)
	{
		return scene_inst::is_friend(psr0, psr1);
	}
	if(psr0->same_master(psr1))
	{
		return true;
	}
	if(psr0->get_feature() == MT_PICK || psr1->get_feature() == MT_PICK)
	{
		return true;
	}
	if(psr0->get_status().atk_mode() == client::role_status_attack_mode_attack_mod_all)
	{
		return false;
	}
	if(psr0->is_counter_role(psr1))
	{
		return false;
	}
	if(psr0->get_status().atk_mode() == client::role_status_attack_mode_attack_mod_peace)
	{
		return true;
	}
	if(psr0->get_status().atk_mode() == client::role_status_attack_mode_attack_mod_camp)
	{
		return psr0->get_camp() == psr1->get_camp();
	}
	if(psr0->get_status().atk_mode() == client::role_status_attack_mode_attack_mod_family)
	{
		return is_friend_family(psr0->get_family_id(), psr1->get_family_id());
	}
	return scene_inst::is_friend(psr0, psr1);
}

int siege_scene_inst::push(const client::robot_attr *ra)
{
	map_postion p(0, 0, 0);
	scene_robot *sr = generate_robot(ra, p, 1114, 0, (uint32)CONFMGR->get_basic_value(10207));
	if(NULL == sr)
	{
		return -1;
	}
	reset_ai(sr);
	return 0;
}

void siege_scene_inst::fill_load_info(client::offline_load_image &li)
{
	map<uint32, client::family_simple_info>::iterator i;
	for(i = _family_list.begin(); i != _family_list.end(); ++i)
	{
		li.add_role(i->second.role_id());
	}
	li.set_flag(_cur % 2);
	uint32 f = get_family_id();
	if(0 == f)
	{
		scene_inst::fill_load_info(li);
		return ;
	}
	i = _family_list.find(f);
	if(i == _family_list.end())
	{
		scene_inst::fill_load_info(li);
		return ;
	}
	for(int j = 0; j < i->second.alliance_size(); ++j)
	{
		li.add_family(i->second.alliance(j));
	}
	li.add_family(f);
	return scene_inst::fill_load_info(li);
}

void siege_scene_inst::reset_ai(scene_npc *sn)
{
	if(NULL == sn)
	{
		return ;
	}
	int camp = get_role_camp(sn);
	const config_robot_ai *ai = CONFMGR->random_config_robot_ai(camp);
	if(NULL == ai)
	{
		return ;
	}
	sn->set_ai(ai->get_robot_ai());
	sn->set_ai_expire_tick(ai->get_robot_ai_time());
}

bool siege_scene_inst::ghost_can_load()
{
	if(!IS_START())
	{
		return false;
	}
	if(_cur % 2 == 0)
	{
		return _atk_robot_num < CONFMGR->get_basic_value(10210);
	}
	return _def_robot_num < CONFMGR->get_basic_value(10214);
}
