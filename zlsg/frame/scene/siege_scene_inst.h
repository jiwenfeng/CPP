#ifndef __SIEGE_SCENE_INST_H__
#define __SIEGE_SCENE_INST_H__

#include "scene_inst.h"
#include "battle_scene_inst.h"
#include "client.city.pb.h"
#include "client.family.pb.h"
#include "client.arena.pb.h"

#define DEFAULT_SIEGE_SCENE_USER_NUM 3000

#define SIEGE_PREPARE	0x00001
#define SIEGE_START		0x00010
#define SIEGE_TRANS		0x00100
#define SIEGE_OCCUPY	0x01000
#define SIEGE_OVER		0x10000

#define IS_START()		(_status & SIEGE_START)
#define IS_OCCUPIED()	(_status & SIEGE_OCCUPY)
#define IS_TRANS()		(_status & SIEGE_TRANS)
#define IS_OVER()		(_status & SIEGE_OVER)



class siege_scene_inst : public battle_scene_inst
{
public:
    siege_scene_inst(uint32 id, const game_map* p, const battle_scene* s, struct amtsvr_context* c, const int m = DEFAULT_SIEGE_SCENE_USER_NUM );
	~siege_scene_inst();

public:
	virtual int enter_scene(scene_user *su);
	virtual int enter_scene(scene_npc *sn);
	virtual int leave_scene(scene_user *su);
	virtual int leave_scene(scene_npc *sn);
	virtual int on_kill_role(scene_role *atk, scene_role *def);
    virtual int on_pick_npc(scene_user* psu, scene_npc* psn);
	virtual int update(uint32 tick, bool is_one_sec, bool is_fiv_sec, bool is_ten_sec, bool is_thirty_sec,bool is_one_minute);
	virtual int get_relive_timelong() { return CONFMGR->get_basic_value(10043); }
    virtual int on_role_move(scene_role* r, const map_postion& from, const map_postion& to);
    virtual bool is_need_broadcast_npc_name() { return true; }
	virtual bool relive(scene_role *sr);
	virtual bool is_friend(scene_role *psr0, scene_role *psr1);
	virtual bool is_alliance(scene_role *sr0, scene_role *sr1);
	virtual void on_pick(scene_user *psu, uint64 inst_id);
    virtual void on_role_enter(scene_role* psr);
	virtual void on_role_leave(scene_role *sr);

private:
	void close();
	void refresh_flag();
	void send_siege_over();
	void send_random_trans_msg();
	void random_trans_users();
	void set_status(uint32 status) {_status = status; }
	void add_status(uint32 status) { _status |= status; }
	void set_family_id(uint32 family_id);
	void set_open_time(uint32 time);
	void set_occupy_time(uint32 time);
	void set_close_time(uint32 time);
	void reset_role_camp();
	uint32 get_family_id();
	uint32 get_occupy_time();
	uint32 get_open_time();
	uint32 get_close_time();
	uint32 get_role_region(scene_role *sr);
	int init_scene_user();
	int add_scene_user(scene_user *su);
	int get_role_camp(scene_role *sr);
	int get_family_camp(uint32 family_id);
	int reset_role_region(scene_role *sr);
	bool is_friend_family(uint32 f1, uint32 f2);

public:
	void sys_combo_msg(uint32 msg_id, const char *w, const char *l = NULL);
	void send_dead_msg(scene_role *atk, scene_role *def);
	void update_role_camp(scene_role *sr, bool notify = true);
	void send_siege_info(scene_role *sr);
	void do_pick(client::family_money_query_rsp *rsp);
	void siege_start(client::siege_start *ss);
	void reset_role_pos(scene_role *sr);
	void occupy_flag(uint32 family_id);
	void reset_robots_ai();
	int calc_combo(scene_role *sr1, scene_role *sr2);
	int add_role_combo(scene_role *sr);
	int reset_role_combo(scene_role *sr1, scene_role *sr2);
	int on_one_second_update(uint32 seconds);
	int fill_personal_info(scene_user *su, client::siege_personal_info_rsp &rsp);
	const string &get_family_name(uint32 family_id);
	uint32 query_family_flag(uint32 family_id);
	uint32 get_msg_id();
	map_postion get_role_pos(uint32 family_id);

public:
	virtual int push(const client::robot_attr *ra);
	virtual bool ghost_can_load();
	virtual uint32 get_ghost_limit_lv() { return CONFMGR->get_basic_value(10213); }
	virtual uint32 get_ghost_limit_power() { return CONFMGR->get_basic_value(10212); }
	virtual void fill_load_info(client::offline_load_image &li);
	virtual void reset_ai(scene_npc *sn);
	void count_camp_num();

private:
	map<uint64, client::siege_personal_info> _role_list;
	map<uint32, client::family_simple_info> _family_list;
	uint32 _status;			// 当前状态
	uint32 _family_id;
	uint32 _close_time;
	uint32 _occupy_time;
	uint32 _open_time;
	vector<scene_user *> _tu; // 待传送人员
	uint32 _cur;
	int _atk_robot_num;
	int _def_robot_num;
};

#endif
