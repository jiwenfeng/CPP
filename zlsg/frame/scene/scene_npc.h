#ifndef __SCENE_NPC_H__
#define __SCENE_NPC_H__

#include "client.pick.pb.h"
#include "cell_pos.h"
#include "origin_npc.h"
#include "scene_role.h"

class scene_lose;
class scene_user;

class scene_npc : public scene_role
{
public:
    scene_npc(const origin_npc& n, const uint64 id);
    virtual ~scene_npc();

public:
    scene_npc& operator=(const origin_npc& rhs);

public:
    virtual int init();

    virtual bool on_update(uint32 tick, uint32 user_num, bool is_one_sec, bool is_fiv_sec, bool is_ten_sec, bool is_thirty_sec, bool is_one_minute);
    virtual bool on_update_100ms(uint32 tick, uint32 user_num);
    virtual bool on_update_one_second(uint32 tick, uint32 user_num);
    virtual bool on_watch(scene_role* psr);
    virtual int  on_attacked(scene_role* psr, skill& sk, effect_result& atk_rsp, bool& trigger);
    virtual int  on_killed(scene_role* psr);
    virtual void on_dead(uint32 tick);
    virtual void on_reset(bool reborn = true);
    virtual int  on_move_end();
    virtual void stand(bool broadcast = true);

    virtual void set_force_speed(int speed);

    virtual void reset_npc();
    virtual void reset_original_attr();
    virtual void calculate_attr();

    virtual void fill_role_move_msg(client::role_move_msg& rmm);

	virtual const char *get_role_name() const;

public:
    virtual bool on_status_stand(uint32 tick);
    virtual bool on_status_move(uint32 tick);
    virtual bool on_status_fight(uint32 tick);
    virtual bool on_status_dead(uint32 tick);
    virtual bool on_status_force_move(uint32 tick);
    virtual bool on_status_rand_move(uint32 tick);
    virtual bool on_status_back_move(uint32 tick);
    virtual bool on_status_path_move(uint32 tick);
    virtual bool on_status_idle(uint32 tick);
    virtual bool on_status_in_block(uint32 tick);
    virtual bool on_status_hide(uint32 tick);

    virtual void send_whip_reward();
//    virtual int  on_whip_drop_down(std::vector<uint32>& belong);
    virtual int  on_whip_drop_down(scene_role *sr);
	virtual int	 on_normal_drop_down(scene_role *sr);
//    virtual int  on_normal_drop_down(std::vector<scene_role *>& belong, std::vector<uint32>& profession);

    virtual bool remove_body(uint32 tick);
    virtual void relive();

    virtual void init_to_update();

    void add_to_update();
    void remove_to_update();

    client::PICK_RET_E pick(scene_user* su);
    client::PICK_RET_E pick_commit(scene_user* psu);
    void pick_remove(uint32 role_id);
    void pick_complete();

    virtual bool move_to_cell(const cell_pos& p, bool force = false);
    virtual bool move_to_pos(const map_postion& p, MOVE_TYPE mt = MT_NORMAL);
    virtual void follow(const map_postion& p);

    bool   can_watch();

    inline bool is_must_leave(uint32 now) { return (_leave_time > 0) && (now > _leave_time); }
    inline void set_leave_time(uint32 t)  { _leave_time = t; }

    inline bool get_reative_atk() const { return _reative_atk; }
    inline void set_reative_atk(bool a = true) { _reative_atk = a; }

    uint64 get_max_damage_role();

    void change_mode(client::PET_MODE type);

    void multi_attr();

    int add_lose_props(client::drop_props_msg &dpm);
    int broadcast_drop_props(client::drop_props_msg &dpm);

    void set_npc_status(NPC_STS_E nse);
    inline NPC_STS_E get_npc_status() const { return _nse; }

    void stop();
    void show();
    void hide(uint32 pet);
    void ai_master(uint32 tick);
	void ai_expire(uint32 tick);

	void set_disappear_tick(uint32 tick);
	void set_ai_expire_tick(uint32 tick);
	uint32 get_ai_expire_tick() { return _ai_expire; }
	void set_ai(const string &ai) { origin_self.set_ai(ai); }
	virtual bool on_update_disappear(uint32 tick, uint32 user_num);

    virtual int add_effect(const config_effect* effect, client::role_data* caster = NULL);
    virtual bool check_immunity_effect(const config_effect* effect);
    virtual bool check_immunity_skill(const config_skill* skill);

protected:
    virtual scene_role* get_hatest_role();
	virtual scene_role *get_hatest_role(int limit_level);
    bool chase_role_by_cell(uint32 tick);
    bool go_rand();
    bool go_back();
    bool go_path();
    bool out_block();

protected:
    bool attack(skill* sk, scene_role* dst_role, uint32 tick);
    bool is_pet_all_dead();
    void use_attack_tirgger_skill(uint32 skill_id, scene_role* dst_role, uint32 tick);
    void broadcast_hate_list(scene_role* dst_role);

protected: // AI
    void run_ai(uint32 tick);
    bool run_ai(const server::ai_method&   method  , uint32 tick);
    bool run_ai(const server::ai_move&     move    , uint32 tick);
    bool run_ai(const server::ai_fix_move& fix_move, uint32 tick);
    bool run_ai(const server::ai_attack&   target  , uint32 tick);
    bool run_ai(const server::ai_stop&     target  , uint32 tick);
    bool run_ai(const server::ai_hide&     target  , uint32 tick);
    bool run_ai(const server::ai_master&   target  , uint32 tick);
    void do_ai(uint32 tick);
    void run_ai_dead(uint32 tick);
    bool get_rand_cell(const cell_pos& source, uint32 range, cell_pos& target);

protected:
    inline void      set_move_cell(const cell_pos& cp) { _move_cell = cp; } 
    inline cell_pos& get_move_cell() { return _move_cell; }

    void trans_to_master_pos();

private:
    void get_drop_down_postion(const map_postion& src, uint32 num, map_postion& dst);

public: // 原型相关
    inline origin_npc& get_origin_npc() { return origin_self; }

public:
    static const uint32 BACK_LIMIT;

protected:
    origin_npc origin_self; // 自己的原型

protected:
    NPC_STS_E _nse;

    std::map<uint32, uint32> _pick_roles; // 采集的角色

    uint32 _killer;
    uint32 _leave_time;    // NPC离开时间

    bool   _reative_atk;  // 怪物后期是否变异为主动攻击

    uint32 _atk_id;       // 攻击顺序索引
    int    _d;            // 寻路时的固定方向
    bool   _attacked;     // 是否被攻击

    cell_pos              _move_cell; // 移动目标格
    std::list<cell_pos>   _path_cell; // 移动路径

    uint32                _rand_move_tick;

    std::map<uint32, uint32>                            _ai_timers;
    std::map<uint32, uint32>                            _ai_alarm;
    std::map<uint32, std::map<uint32, uint32> >         _ai_count;
    std::map<uint32, std::map<uint32, server::ai_run> > _ai_run;
    std::map<uint32, std::map<uint32, bool> >           _ai_do_method;
    std::map<uint32, std::map<uint32, bool> >           _ai_do_move;
    std::map<uint32, std::map<uint32, bool> >           _ai_do_fix_move;
    std::map<uint32, std::map<uint32, bool> >           _ai_do_target;
    std::map<uint32, std::map<uint32, bool> >           _ai_do_stop;
    std::map<uint32, std::map<uint32, bool> >           _ai_do_hide;
    std::map<uint32, std::map<uint32, bool> >           _ai_do_master;
    uint32                                              _last_ai_tick;
    uint32                                              _disappear_tick;
    uint32                                              _stop_tick;
    uint32                                              _hide_tick;
    uint32                                              _master_tick;
	uint32												_ai_expire;// ai结束时间
    bool _force_move_notify;

    bool _elite;

	vector<cell_pos> _cp;

    uint32 _target_inst_id; // 上次攻击的角色实例ID
    bool   _is_fiv_sec;
};

#endif
