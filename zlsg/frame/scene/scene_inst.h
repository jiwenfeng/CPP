#ifndef __SCENE_INST_H__
#define __SCENE_INST_H__

#include "shmap.h"
#include "style_box.h"
#include "cell.h"
#include "terrain.h"
#include "client.operate_mail.pb.h"
#include "client.city.pb.h"
#include "scene_robot.h"
#include "client.offline.pb.h"

enum GENERATE_NPC_TYPE_E {
    GNTE_NORMAL = 0, // 产生正常的NPC
    GNTE_AWARD  = 1, // 产生抽奖励NPC
    GNTE_FARM   = 2, // 产生的是农场NPC
    GNTE_SPACE  = 3, // 空间裂隙NPC
	GNTE_ROBOT	= 4, // 竞技场机器人
};

class scene;
class game_map;
class origin_npc;
class scene_lose_mgr;
class check_create_npc_rule;
class region_over;


#define	NEUTRALITY 0 // 中立
#define ATTACK	   1 // 攻击
#define DEFENCE    2 // 防守

// 场景实例，就是指游戏内实际场景
class scene_inst : public space_style_box
{
public:
    scene_inst(uint32 id, const game_map* p, const scene* s, struct amtsvr_context* c, const int m = DEFAUTL_SCENE_INST_USER_NUM);
    virtual ~scene_inst();

public: // 场景、地图、人物初始化相关方法
    virtual int init();

    inline const game_map& get_map()      const { return *_gm; }
    inline const scene&    get_scene()    const { return *_sc; }
    inline amtsvr_context* get_ctx()            { return _ctx; }
    uint32 get_map_id    () const;
    uint32 get_scene_id  () const;
    uint32 get_scene_type() const;

	int get_around_role(const map_postion& p, std::vector<scene_role *>& vsr, uint32 distance, bool get_user = true, bool get_npc = true);

    int get_screen_role(const postion& p, std::vector<scene_role*>& vsr, bool get_user = true, bool get_npc = true);
    int get_screen_role(std::vector<style_box*>& box, std::vector<scene_role*>& vsr);
    int get_screen_user(std::vector<style_box*>& box, std::vector<scene_user*>& vsr);
    int get_screen_npc(std::vector<style_box*>& box, std::vector<scene_npc*>& vsr);

    scene_role* get_scene_role(uint64 id);
    scene_role* get_scene_role(const ::client::role_data& rd);
    scene_user* get_scene_user(uint64 id);
    scene_user* get_scene_user_by_role_id(uint32 id);
    scene_npc*  get_scene_npc(uint64 id);
    scene_npc*  get_scene_npc_by_role_id(uint32 id);
    scene_npc*  get_scene_pick(uint64 id);
    void get_scene_users(std::vector<scene_user*>& us);
    void get_scene_users(std::vector<uint32>& us);
	void get_scene_roles(std::vector<scene_role *>& v);
    void get_alive_scene_npcs(std::vector<scene_npc*>& ns);
    virtual bool is_scene_npc_all_dead();
    virtual uint32 get_alive_monster(std::vector<scene_npc*>& ns);
    inline const shmap& get_user_map() const { return _user_map; }

    scene_user* get_first_user();
    inline const uint32 get_inst_id() const { return _inst_id; }
	uint64 generate_npc_inst_id();
    inline scene_lose_mgr* get_scene_lose_mgr() { return _slm; }

    int add_tick_role(scene_role* sr);
    int remove_tick_role(scene_role* sr);

    int add_tick_npc(scene_npc* n);
    int remove_tick_npc(scene_npc* n);

    int add_tick_user(scene_user* u);
    int remove_tick_user(scene_user* u);

    // 清除人物在九宫格上的位置
    int remove_style_box_pos(scene_role& r);

    int remove_role(scene_role* r);

    uint64 generate_pet_id();

    int find_path(const map_postion& from, const map_postion& to, std::vector<move_map_postion>& path);

    inline uint32 get_game_handle() { return strtoul(amtsvr_command(_ctx, "REG", NULL) + 1 , NULL, 16); }

    virtual bool is_need_broadcast_npc_name() { return true; }
	virtual bool relive(scene_role *sr) { return true; } // true 当前场景复活

    virtual int on_pick_props(scene_role* psr, client::props_info* ppi) { return 0; }
    virtual int on_pick_npc(scene_user* psu, scene_npc* psn) { return 0; }
    virtual void on_pick(scene_user *psu, uint64 inst_id);

    virtual int on_submit_task(scene_user* su, uint32 task_id) { return 0; }
    virtual int on_story_end(scene_user* psu, uint32 story_id) { return 0; }

    void destroy_user(scene_user* su);
    void destory_user_all();
    void save();
    void release();

    virtual int get_relive_timelong() { return CONFMGR->get_basic_value(10140); }

	uint32 get_survive_count();

public: // 副本相关
    inline void set_close(bool close = true) { _close_inst = close; } 
    inline bool is_close() const { return _close_inst; }

public: // 定时更新、人物动作相关方法
    int create_randnpc(client::randnpc* rn); 

	scene_npc* generate_npc_inst(uint32 npc_id, uint32 x, uint32 y);
    scene_npc* generate_npc_inst(uint32 npc_id);
    scene_npc* generate_npc_inst(const origin_npc& p, uint32 now);
    scene_npc* generate_npc_inst(const origin_npc& p, uint32 now, const map_postion& mp, GENERATE_NPC_TYPE_E gntype = GNTE_NORMAL, bool enter = true);

    virtual int update(uint32 tick, bool is_one_sec, bool is_fiv_sec, bool is_ten_sec, bool is_thirty_sec, bool is_one_minute);
    virtual int update_thirty_seconds(uint32 tick);
	virtual int update_role_dispear(uint32 tick, uint32 user_num);

    int enter_scene(scene_role* psr);
    int leave_scene(scene_role* psr);
    virtual int enter_scene(scene_npc* n);
    virtual int leave_scene(scene_npc* n);
    virtual int enter_scene(scene_user* u);
    virtual int leave_scene(scene_user* u);
    virtual void on_role_enter(scene_role* psr);
    virtual void on_role_leave(scene_role* psr);
	void del_scene_buffer(scene_role *psr);
    int trans_back_all_user();
    int trans_back_all_user_by_msg();

    int  trans_pos(scene_role* sr, const map_postion& to);
    int  trans_postion(scene_role* sr, const map_postion& to);
    void screen_role_leave(scene_role* sr);

    bool is_scene_empty();

    virtual int on_role_move(scene_role* r, const map_postion& from, const map_postion& to);
    void role_move_affect(scene_role* r, std::vector<style_box*>& push_box, std::vector<style_box*>& move_box, std::vector<style_box*>& left_box);
    int  role_move_stylebox(scene_role* r, const map_postion& from, const map_postion& to);
    void move_style_test(const map_postion& from, const map_postion& to, std::vector<style_box*>& push_box, std::vector<style_box*>& move_box, std::vector<style_box*>& left_box);

    // 主角移动，NPC被动看到
    void npc_watched_user(scene_user* su);
    void npc_watched_user(scene_user* su, std::vector<scene_npc*>& push_npcs);

    // NPC主动移动去看到主角
    void npc_watch_user(scene_role* sr, std::vector<style_box*>& move_box);
    void npc_watch_user(scene_role* sr, std::vector<scene_user*>& push_users);
    void npc_watch_user(scene_npc& n, std::vector<scene_user*>& push_users);

    // pet移动看到npc
    void pet_watch_npc(scene_role* sr, std::vector<scene_npc*>& push_npcs);

    virtual int on_role_attr_change(scene_role* psr);

public: // 单元格子相关
    inline cell_mgr& get_cell() { return _cell; }
    cell_pos     to_cell_pos     (const map_postion& pos);
    map_postion  to_map_postion  (const cell_pos& pos);
    bool         cell_empty      (const cell_pos& pos);
    scene_role*  get_role_in_cell(const cell_pos& pos);
    void         role_move_cell  (scene_role* sr, const map_postion& from, const map_postion& to);

    void         calc_role_cell_to_map_postion(scene_role* sr, int length, int& ux, int& uy);
    void         calc_pos_to_pos(const map_postion& pos, ROLE_FACING rf, int length, map_postion& p);
    bool         get_normalize_d(ROLE_FACING rf, float& x, float& y);
    int          calc_cell_length(int l);
    void         fix_pos(map_postion& p);
    void         fix_role_pos(map_postion& p);

public: // 地形相关
    inline terrain_mgr& get_terrain_mgr() { return _tm; }
    void role_move_check_terrain(scene_role* sr, const map_postion& from, const map_postion& to);
    void role_on_check_terrain(const client::terrain_info& ti);
    void scene_broadcast_terrain_change();
    void fill_terrain_all();

public: // 战斗相关
    bool get_rectangle(scene_role* psr, const map_postion& p1, const map_postion& p2, uint32 length, map_postion& mp1, map_postion& mp2, map_postion& mp3, map_postion& mp4);
    void sort_by_distance(const map_postion& pos, std::vector<scene_role*>& vsr);
    int select_target(scene_role* atk, skill* ps, std::vector<scene_role*>& enemies, std::vector<scene_role*>& friends, std::vector<map_postion>& miss);
    int on_attack(scene_role* src_role, std::vector<scene_role*>& dst_role_list, skill& sk);

    virtual int on_attack_role(scene_role* atk, scene_role* dst_role, skill& sk, effect_result& atk_rsp) { return 0; }
    virtual int on_kill_role(scene_role* atk, scene_role* def);
	virtual void on_role_disappear(scene_role *sr) {}

    void team_add_exp(scene_user* su, uint32 exp);
    void add_activity(uint64 id, uint32 count);

    virtual bool is_friend(scene_role* psr0, scene_role* psr1);
	virtual bool is_alliance(scene_role *psr0, scene_role *psr1) { return is_friend(psr0, psr1); }

public:		/* 竞技场相关 */
// 	virtual int push(client::load_target_data_rsp *rsp);

public:
    // 场景中增加一个角色的接口
    int screen_push_role(scene_role* sr);
    int screen_push_user(scene_user* su);
    int screen_push_npc(scene_npc* sn);
    int screen_push_npcs(std::vector<scene_npc*>& sns, bool bron = false); // 注意此接口都是同场景广播，调用时仅适合应用在副本批次刷怪等用户少的场合
    int screen_push_pet(scene_pet* sp);

    // 场景中改变一个角色的接口
    void multi_broadcast_modify_role_msg(scene_role& r, std::vector<scene_role*>& roles);
    void screen_broadcast_modify_role_msg(scene_role& r, bool except_me = true);

    // 场景中移动一个角色的接口
    int screen_broadcast_move_pos(scene_role& r);

    // 场景其它消息通知接口
	int scene_announcement(uint32 id, const vector<client::content_data> &data);
    int scene_broadcast(google::protobuf::Message& msg);
    int scene_broadcast(std::vector<google::protobuf::Message*>& msgs);
    int scene_broadcast(scene_role& r, google::protobuf::Message& msg, bool except_me = true);

    int screen_broadcast(scene_role& sr, google::protobuf::Message& msg, bool except_me = true);
    int screen_broadcast(scene_role& sr, std::vector<google::protobuf::Message*>& msgs, bool except_me = true);

    int multicast(scene_role& r, std::vector<scene_user*>& users, google::protobuf::Message& msg, bool except_me = true);
    int multicast(scene_role& r, std::vector<scene_user*>& users, std::vector<google::protobuf::Message*>& msgs, bool except_me = true);

    int send_center(google::protobuf::Message& msg);
	int send_client(uint32 role_id, google::protobuf::Message& msg);

	void sys_mail(scene_user* su, string& title, string& content,std::vector<client::mail_attr>& attr, std::vector<props_info>& ma,std::vector<client::content_data>& info);

public: // 挂起场景相关
    bool suspend(uint32 ut);
    bool resume();
    inline uint32 get_suspend_tick() const { return _suspend_tick; }
    inline void set_suspend_tick(uint32 st) { _suspend_tick = st; }
    inline uint32 get_last_tick() const { return _last_tick; }
    inline void set_last_tick(uint32 lt) { _last_tick = lt; }
    inline uint32 get_space_tick() const { return _space_tick; }
    inline void set_space_tick(uint32 val) { _space_tick = val; }

protected: // 广播内部接口
    virtual int scene_push_init_info(scene_user* su) { return 0; }

    int screen_fill_move_role_msg(scene_role& r, client::role_move_msg& rmm, client::multi_role_msg& mrm, client::role_name_msg& rnm, client::buffer_list& bl);
    int multi_fill_move_role_msg(scene_role& r, std::vector<scene_user*>& users, std::vector<scene_npc*>& npcs, client::role_move_msg& rmm, client::multi_role_msg& mrm, client::role_name_msg& rnm, client::buffer_list& bl);

    // A.将特定角色发送给同屏或选定用户们 B.或者将同屏或指定角色发送给指定用户
    int push_role_to_users(scene_role& sr, std::vector<scene_user*>* push_user = NULL, bool born = false);
    int push_roles_to_user(scene_user& su, std::vector<scene_user*>* push_user = NULL, std::vector<scene_npc*>* push_npc = NULL);

    int screen_broadcast_remove(scene_role& r, bool except_me = true);
    int screen_fill_role_remove_msg(scene_role& r, client::role_remove_msg& rrm);
    int multicast_role_remove(std::vector<scene_user*>& users, scene_role& r);
    int multi_fill_role_remove_msg(scene_role& r, std::vector<scene_user*>& users, std::vector<scene_npc*>& npcs, client::role_remove_msg& rrm);
	int broadcast_scene_area_msg(amtsvr_context* ctx,uint32 svr_scene_id,client::broadcast_area_rsp& area_msg);
	int broadcast_scene_msg(amtsvr_context* ctx,uint32 svr_scene_id, google::protobuf::Message &msg);

protected:
    int clear_all();
    int clear_npc(bool notify_remove = false);
    int clear_npc(std::vector<uint64>& tmns);
    int clear_monster();
    int clear_dead_npc();

protected: // 生成NPC相关
    int generate_npc_inst();    // 实例化本场景所有NPC
    inline check_create_npc_rule* get_check_create_npc_rule() { return _ccnr; }

protected: // 区域相关
    void enter_region(scene_role* psr, const cell_pos& from_cp, const cell_pos& to_cp);
    inline region_over* get_region_over() { return _region_over; }

protected:
    void send_sys_msg(uint32 msg_id, const char* cmd = "", uint32 type = 0);
    void sys_msg_filed_score(scene_user* atk, scene_user* def, uint32 score);

public: // 雕像相关
	void update_sculpture(uint32 npc_id, const string &king, const string &title, const string &family_name);
	void send_scene_sculpture(scene_user *su);
	void broadcast_sculpture();

public:
	virtual int push(const client::robot_attr *ra);
	scene_robot *generate_robot(const client::robot_attr *ra, const map_postion &p, uint32 config_id, uint32 type, uint32 disappear);


public:
	virtual void set_scene_ready(bool ready, scene_user *psu) { _ready = ready;}
	virtual bool is_scene_ready() { return _ready; }
public: // 加载镜像相关
	void load_ghost();
	void erase_ghost(uint32 role_id);
	virtual void fill_load_info(client::offline_load_image &li);
	virtual bool ghost_can_load();
	virtual uint32 get_ghost_limit_lv() { return 0; }
	virtual uint32 get_ghost_limit_power() { return 0; }
	virtual uint32 get_ghost_limit_tl() { return CONFMGR->get_basic_value(10205); }
	virtual void reset_ai(scene_npc *sn) {}

protected:
    const  uint32           _inst_id; // 实例id
    const  game_map*        _gm;
    const  scene*           _sc;
    struct amtsvr_context*  _ctx;
    const  int              _max_user;
    int                     _cur_user;
    bool                    _close_inst;
    uint32                  _start_time;

    uint32 _alloc_npc_inst_id;
    uint32 _alloc_pet_inst_id;

    shmap  _user_map;
    shmap  _npc_map;

    std::map<uint64, scene_role*> _remove_role;

    scene_lose_mgr*         _slm; // 场景掉落管理
    cell_mgr                _cell;
    terrain_mgr             _tm;

    uint32 _space_tick;
    uint32 _last_tick;
    uint32 _suspend_tick;

    check_create_npc_rule*         _ccnr;
    region_over*                   _region_over;

	map<uint32, client::kingdom_sculpture_info> _sculpture;
	int _robot_num;
	bool _ready;
};

#endif

