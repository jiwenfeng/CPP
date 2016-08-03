#ifndef __SCENE_ROLE_H__
#define __SCENE_ROLE_H__

#include "scene_define.h"
#include "client.role_pos_msg.pb.h"
#include "client.role_name_msg.pb.h"
#include "client.role_move_msg.pb.h"
#include "client.role_remove_msg.pb.h"
#include "role.h"
#include "hate_man.h"
#include "scene_role_status.h"
#include "player_role_pet.h"

struct cell_pos;

class  scene_inst;
class  halo_mgr;

class scene_role : public role, public hate_man
{
public:
    scene_role(const uint64 id);
    scene_role(const scene_role& rhs);
    virtual ~scene_role();

public: // 场景相关联方法
    inline const uint64 get_inst_id() const { return _inst_id; }
    inline void  set_inst_id(uint64 id) { _inst_id = id; }

    inline scene_inst* get_scene() { return _in_scene; }
    virtual int attach_scene(scene_inst* s);
    virtual int detach_scene();

    inline const map_postion& get_scene_pos() const { return _scene_pos; }
    void set_scene_pos(const map_postion& p);
    virtual void change_cur_pos(const map_postion& to);

    inline uint32 get_region() const { return _region; }
    inline void set_region(uint32 val) { _region = val; }

    inline  bool is_same_role(const scene_role& sr) const { return (get_role_type() == sr.get_role_type() && get_inst_id() == sr.get_inst_id()); }

    virtual bool on_watch(scene_role* psr) { return false; }

public: // 宠物相关
    inline std::map<uint64, scene_role*>& get_pets() { return _pets; }
    inline void set_master(scene_role* psr) { _master = psr; }
    inline scene_role* get_master() { return _master; }
    inline const scene_role* get_master() const { return _master; }
    void add_pet(scene_role* sr);
    void remove_pet(scene_role* sr);
    void delete_pet(scene_role* sr = NULL);
    ROLE_TYPE_E get_real_role_type();
    bool same_master(scene_role* psr);
	bool cure_pet(uint32 pet_id);
	bool pet_attr_change(uint32 pet_id,uint32 opt_type);
	bool pet_skill_effect_up(uint32 pet_id,uint32 skill_id,uint32 upgrade_level = 1);

public: // 移动相关
    inline uint32 is_keep_move() const { return _move_count > 1; }
    inline uint32 get_move_count() const { return _move_count; }
    inline void set_move_count(uint32 mc) { _move_count = mc; }
    inline const std::vector<move_map_postion>& get_move_node() const { return _move_node; }
    inline std::vector<move_map_postion>* mutable_move_node() { return &_move_node; }
    inline void set_move_node(std::vector<move_map_postion>& mn) { _move_node = mn; }
    inline uint32 get_last_move_tick() const { return _last_move_tick; }
    inline void set_last_move_tick(uint32 tick) { _last_move_tick = tick; }

    bool move_to_pos(const map_postion& p, MOVE_TYPE mt = MT_NORMAL);
    bool get_next_move(uint32 tick, move_map_postion& mmp);
    bool move_by_path(uint32 tick, bool notice = true);
    bool move_by_cell(uint32 tick);
    bool move_ignore_block(uint32 tick);

    virtual void follow(const map_postion& p) {};

    virtual int on_region_change(uint32 old, uint32 now);
    virtual int on_region_check(const region_data& r);
    virtual int on_leave_region(const region_data& r);
    virtual int on_enter_region(const region_data& r);

    virtual int on_move_stop(uint32 tick);
    virtual int on_move_end();

    bool flash_move(const map_postion& mp);
    bool swap_postion(scene_role* sr);

    inline  int  get_force_speed() const { return _force_speed; }
    virtual void set_force_speed(int cs) { _force_speed = cs; }

    // 人物朝向
    inline ROLE_FACING get_facing() const { return _facing; }
    inline void        set_facing(ROLE_FACING rf) { _facing = rf; }
    void               set_facing(float x, float y);
    ROLE_FACING        role_facing(const map_postion& start, const map_postion& end);

public: // 战斗相关
    void   use_skill(skill* ps);
    void   fill_miss(skill* ps, const std::vector<map_postion>& miss, effect_result& rsp);
    void   attack_damage(skill* ps, std::vector<scene_role*>& vsr, effect_result& rsp);
    virtual uint32 attack_damage(scene_role* sr, skill* ps);
    void   attack_effect(const config_skill* pcs, uint32 effect_id, std::vector<scene_role*>& enemies, std::vector<scene_role*>& friends, effect_result& rsp);
    void   attack_effect(skill* ps, std::vector<scene_role*>& enemies, std::vector<scene_role*>& friends, effect_result& rsp);
    void   attack_effect(const config_effect* pce, scene_role* psr);
    scene_role* scene_buff_target(const config_effect* ce, std::vector<scene_role*>& enemies, std::vector<scene_role*>& friends);
    void   move(const config_effect* pce, std::vector<scene_role*>& enemies, effect_result& rsp);
    void   call(const config_effect* pce);
	bool   rest_pet(uint32 pet_id,bool is_ectype);
    void   collision(const config_skill* pcs, std::vector<scene_role*>& vsr, effect_result& rsp);
    void   flash(const config_effect* pce);
    void   swap(std::vector<scene_role*>& enemies);
    void   clear_buffer(const config_effect* pce, std::vector<scene_role*>& enemies, std::vector<scene_role*>& friends);
    void   fall_from_mounts(const config_effect* pce, std::vector<scene_role*>& enemies);
    virtual void fall_from_mounts(uint32 tick);
    bool   is_hit_point (scene_role* sr);
    virtual uint32 sub_hp(uint32& hp);
    void   use_passive_skill();
    void   use_attack_trigger_skill();
	void   bow_attack_skill();
	void   use_bow_skill();
    bool   check_path_block(map_postion& p);
	virtual bool is_robot() { return false; }
    virtual scene_role* get_hatest_role();

public: // 属性状态相关方法
    inline uint32 get_create_time() const { return _create_time; }
    inline uint32 get_dead_time  () const { return _dead_time  ; }
    inline uint32 get_fighttime  () const { return _fight_time ; }
    inline void set_create_time(uint32 t) { _create_time = t; }
    inline void set_dead_time  (uint32 t) { _dead_time   = t; }
    inline void set_fight_time (uint32 t) { _fight_time  = t; }

    virtual void calculate_attr();
    virtual void calculate_status();

    void   set_camp(uint32 id);
    virtual uint32 get_camp() const { return _camp_id; }
    void   set_team_id(uint32 id);
    virtual uint32 get_team_id() const { return _team_id; }
    void   set_family_id(uint32 id);
	void   set_attack_mode(uint32 mode);
    virtual uint32 get_family_id() const { return get_attr(FAMILY_ID); }
	const uint32 get_job() const { return get_attr(PROFESSION); }

    inline       scene_role_status* mutable_status() { return &_status; }
    inline const scene_role_status& get_status() const { return _status; }
    virtual void on_status_change(uint32 index, uint32 val);

    virtual void on_attr_change(ROLE_ATTRIBUTE type, uint64 old_val, uint64 new_val);

    void remove_protect_status();

    bool can_attack();
    bool can_attack(const scene_role* dsrole);
    bool can_move();
    bool can_use_skill(const skill& sk);
    bool can_practice();

    bool is_in_chase_range(const map_postion& p, uint32 range);
    bool is_in_chase_range_cell(const cell_pos& c, uint32 range);
    bool is_in_range(const map_postion& p, uint32 range);
    bool is_in_range(const map_postion& src, const map_postion& dst, uint32 range);
    bool is_in_range_cell(const cell_pos* c, uint32 range);

    inline uint32 get_last_update_tick() const { return _last_update_tick; }
    inline void   set_last_update_tick(uint32 tick) { _last_update_tick = tick; }
    inline uint32 get_last_attack_tick() const { return _last_attack_tick; }
    inline void   set_last_attack_tick(uint32 tick) { _last_attack_tick = tick; }
    inline uint32 get_last_recover_tick() const { return _last_recover_tick; }
    inline void   set_last_recover_tick(uint32 tick) { _last_recover_tick = tick; }
	inline uint32 get_last_skill_point_tick() const { return _last_skill_point_tick; }
	inline void   set_last_skill_point_tick(uint32 tick) { _last_skill_point_tick = tick; }
    inline uint32 get_100ms_tick() const { return _100ms_tick; }
    inline void   set_100ms_tick(uint32 tick) { _100ms_tick = tick; }
    inline uint32 get_last_mount_tick() const { return _last_mount_tick; }
    inline void   set_last_mount_tick(uint32 tick) { _last_mount_tick = tick; }

    inline void    set_attack_count(uint32 val = 0) { _attack_count = val; }
    inline void    add_attack_count() { ++_attack_count; }
    inline void    sub_attack_count() { --_attack_count; }
    inline uint32& get_attack_count() { return _attack_count; }
	inline void    set_bow_attack_count(uint32 val = 0) { get_role_type() == RTE_USER ? _bow_attack_count = val : 0; }
	inline void    add_bow_attack_count(){ get_role_type() == RTE_USER ? ++_bow_attack_count : 0;}
	inline uint32& get_bow_attack_count() { return _bow_attack_count; }
    inline void    set_defense_count(uint32 val = 0) { _defense_count = val; }
    inline void    add_defense_count() { ++_defense_count; }
    inline uint32& get_defense_count() { return _defense_count; }

    inline MOVE_TYPE get_move_type() const { return _mt; }
    inline void set_move_type(MOVE_TYPE val) { _mt = val; }

	void  send_bow_attack_count();
	uint32 get_gem_skill_count();

	uint32 get_bow_skill_count();

	void get_bow_skill(std::map<uint32,uint32>& bow_skill_map_);

public: // 序列化打包相关方法
    virtual void fill_role_data(client::role_data& rd) const;
    virtual void fill_role_data(client::role_data* rd) const;
    virtual void fill_role_move_msg(client::role_move_msg& rmm);
    void fill_role_pos_msg(client::role_pos_msg& rpm);
    void fill_role_name_msg(client::role_name_msg& rnm); 
    virtual void fill_role_name(client::role_name& rn);
    void fill_role_skill_msg(client::role_skill_msg& rsm);
    void fill_mounts_skill_msg(client::role_skill_msg& rsm);
	void fill_bows_skill_msg(client::role_skill_msg& rsm);
	void fill_gems_skill_msg(client::role_skill_msg& rsm);
    void fill_pets_skill_msg(client::role_skill_msg& rsm,uint32 pet_id,uint32 pet_level);
	void fill_all_pets_skill_msg(client::role_skill_msg& rsm);
    void fill_role_broadcast_msg(client::role_msg& rm, bool born = false);
    bool fill_role_modify_msg(client::role_msg& rm, client::role_attr& broadcast_attr,  bool& need_broadcast);
    void fill_role_remove_msg(client::role_remove_msg& rrm);
    void fill_client_pos(client::client_map_pos& cmp);
    void fill_role_attr(client::role_attr& ra);
    void fill_role_attr(client::role_attr* ra);
    void fill_role_status(client::role_status& rs);
    bool fill_modify_status(client::role_status& rs);
    bool fill_buffer(client::buffer_list& bl);

public: // 更新、动作相关方法
    virtual bool on_update(uint32 tick, uint32 user_num, bool is_one_sec, bool is_fiv_sec, bool is_ten_sec, bool is_thirty_sec, bool is_one_minute);
    virtual bool on_update_100ms(uint32 tick, uint32 user_num);
    virtual bool on_update_one_second(uint32 tick, uint32 user_num);
    virtual bool on_update_fiv_second(uint32 tick, uint32 user_num);
    virtual bool on_update_ten_second(uint32 tick, uint32 user_num)    { return false; }
    virtual bool on_update_thirty_second(uint32 tick, uint32 user_num) { return false; }
    virtual bool on_update_one_minute(uint32 tick, uint32 user_nmu)    { return false; }

	virtual bool on_update_disappear(uint32 tick, uint32 user_num) { return true; }

    virtual int on_attacked(scene_role* psr, skill& sk, effect_result& atk_rsp, bool& trigger);
    virtual int on_killed(scene_role* psr);
    virtual int on_kill(scene_role* psr);

    virtual void stand(bool broadcast = true);

    virtual void on_dead(uint32 tick);
    virtual bool update_buffer(uint32 tick, uint32& effect_count);
    virtual void on_rob_convoy(uint32 quality) {};
    virtual void on_reset(bool reborn = true) {}
    virtual void relive() {}

public: // 效果相关
    virtual int add_effect(const config_effect* effect, client::role_data* caster = NULL);
    int  add_effect(uint32 effect_id, uint32 effect_times = 1);
    int  remove_effect(uint32 effect_id);
    bool on_update_effect(uint32 tick, uint32 user_num);
    void screen_broadcast_change_buf();
    virtual bool check_immunity_effect(const config_effect* effect);
    virtual bool check_immunity_skill(const config_skill* skill);

public: // 光环
    bool has_halo();
    halo_mgr* halo_mgr_instance();
    void halo_mgr_release();
    void add_halo(const config_skill* pcs, const config_effect* pce, std::vector<scene_role*>& enemies, std::vector<scene_role*>& friends);
    void add_halo(scene_role* pcaster, const config_skill* pcs, const config_effect* pce);
	void del_halo(scene_role *sr, const config_effect *pce);
    void role_in(scene_role* psr, uint32 tick);
    void role_out(scene_role* psr, uint32 tick);
	bool has_dead() { return _has_dead; }
	void set_role_status(bool status) { _has_dead = status; }
	void set_disappear() { _disappear = true; }
	bool is_disappear() { return _disappear; }

public: // 反击
	virtual void add_counter_role(scene_role *sr, uint32 hate);
	virtual void del_counter_role(scene_role *sr);
	virtual void clear_counter_list();
	virtual bool is_counter_role(scene_role *sr);
	virtual scene_role *get_counter_role();

public:
    void send_client_attr();
    void send_client_change_attr();
	void broadcast_pet_attr();
	void broadcast_pet_attr(scene_role *sr);

    virtual int send_agent(struct amtsvr_context* ctx, google::protobuf::Message& msg) { return 0; }
    virtual int send_agent(google::protobuf::Message& msg) {return 0;}

    virtual int send_client(struct amtsvr_context* ctx, google::protobuf::Message& msg) { return 0; }
    virtual int send_client(google::protobuf::Message& msg) {return 0;}

    int send_center(struct amtsvr_context* ctx, google::protobuf::Message& msg);
    int send_center(google::protobuf::Message& msg);

    int send_log(google::protobuf::Message& msg);

protected:
    void reset_scene_role();

public:
    bool is_change_attr;// 应用于人物属性改变作为宠物属性重算的标记

protected:
    uint64       _inst_id;
    scene_inst*  _in_scene;

    uint32       _create_time;
    uint32       _dead_time;
    uint32       _fight_time;

    uint8                         _move_count;
    std::vector<move_map_postion> _move_node;
    int                           _force_speed;

    map_postion _scene_pos;
    uint32      _region;

    uint32 _last_move_tick;
    uint32 _last_update_tick;
    uint32 _last_attack_tick;
    uint32 _last_recover_tick;
    uint32 _last_skill_point_tick;
    uint32 _100ms_tick;
    uint32 _last_mount_tick;

    uint32 _attack_count;
    uint32 _defense_count;
	uint32 _bow_attack_count;

    scene_role_status             _status;        // 只有场景中的角色才有状态
    scene_role_status             _modify_status; // 修改的角色状态

    uint32                        _camp_id;
    uint32                        _team_id;

    ROLE_FACING                   _facing;
    MOVE_TYPE                     _mt;

    scene_role*                   _master;
    std::map<uint64, scene_role*> _pets;
    halo_mgr*                     _halos;
	bool						  _disappear;
	bool						  _has_dead;

	map<uint64, uint32>			  _counter_list;
};

#endif // __SCENE_ROLE_H__
