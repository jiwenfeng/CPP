#ifndef __SCENE_USER_H__
#define __SCENE_USER_H__

#include "client.jump_scene.pb.h"
#include "client.trans_user.pb.h"
#include "client.family.pb.h"
#include "client.pick.pb.h"
#include "client.raid_operate.pb.h"
#include "user.h"
#include "scene_role.h"
#include "scene_role_task.h"
#include "base_time.h"

#ifndef __LIB_MINI_SERVER__
#include "scene_role_target.h"
#endif

class scene_lose;
class scene_pet;
class scene_npc;

class player_role_status;
#define PRACTICE_LIST_INTERAL         2      // 2秒钟请求一次列表
#define MEDITATE_STRATEGY_LEVEL_LIMIT 1      // 打坐获得韬略值等级限制


#define LEVEL_FUNC_ID					13
#define BOX_FUNC_ID						14
#define OFFLINE_FUNC_ID					17
#define CYCLE_FUNC_ID					20
#define DEVIL_FUNC_ID					21

//	标志位设置
#define INSERT_FLAG(value, flag)	{(value) |= (flag);}
#define REMOVE_FLAG(value, flag)	{(value) &= ~(flag);}
#define HAS_FLAG(value, flag)		( ((value) & (flag)) != 0 ? true : false )

// 元宝消费
#define MC_BUY_PHYSIC						1	// 购买体力
#define MC_BUY_MONEY						2	// 购买铜钱
#define MC_RESET_RAID						3	// 重置精英副本
#define MC_RESET_ARENA						4	// 重置竞技场挑战次数
#define MC_BUY_SKILL_POINT					5	// 购买技能点次数
#define MC_RESET_MOUNTS_SKILL_COUNT			6	// 重置坐骑技能
#define MC_RESET_CYCLE						7	// 重置沙场百战
#define MC_RESET_DEVIL						8	// 重置魔化神将
#define MC_CLEAR_ARENA_CD					101	// 清除竞技场CD
#define MC_BUY_BOX							102	// 宝箱自动购买
#define MC_BUY_OFFLINE_EXP					103	// 离线挂机经验
#define MC_BUY_ACTIVITY						104 // 活动购买
#define MC_CREATE_FAMILY					105	// 创建军团
#define MC_BUY_PROP							106	// 商城购买
#define MC_UPGRADE_MOUNTS_SKILL				107	// 坐骑技能升级

#define MC_VIP_SPEND			1	// VIP购买
#define MC_ARENA_SPEND			2	// 竞技场
#define	MC_TREASURE_SPEND		3	// 宝箱
#define MC_OFFLINE_SPEND		4	// 离线挂机
#define MC_ACTIVITY_SPEND		5	// 活动
#define MC_FAMILY_SPEND			6	// 军团
#define MC_STORE_SPEND			7	// 商城
#define MC_MOUNTS_SPEND			8	// 坐骑
// 元宝消费

// 日常/成就
#define ROLE_UPGRADE		1		/* 人物等级升级 */
#define PASS_COPY			2		/* 通关副本 */
#define PET_UPGRADE			4		/* 美人升级 */
#define PET_LINGERING		5		/* 美人缠绵 */
#define PET_NUMBER			6		/* 招募美人数量 */
#define MOUNT_ADVANCE		7		/* 坐骑进阶 */
#define MOUNT_UPGRADE		8		/* 坐骑升级 */
#define POTENTION_ACTIVE	9		/* 激活心法 */
#define EQUIP_UP_STAR		10		/* 装备升星 */
#define EQUIP_XILIAN		11		/* 装备洗练 */
#define ROLE_SKILL_UPGRADE	12		/* 玩家技能升级 */
#define MOUNT_SKILL_UPGRADE	14		/* 坐骑技能升级 */
#define PET_SKILL_UPGRADE	17		/* 美人技能升级 */
#define SMELT_EQUIP			18		/* 熔炼装备 */
#define TREASURE			19		/* 寻宝 */
#define ARENA_CHALLENGE		22		/* 竞技场挑战 */
#define FAMILY_DONATE		23		/* 军团捐献 */
#define BUY_COPPER			24		/* 购买铜钱 */
#define GET_PHYSIC_POWER	25		/* 领取体力 */
#define GET_INGOT			26		/* 领取元宝 */
#define PASS_TEAM_RAID		27		/* 组队副本 */
#define PASS_PET_RAID		28		/* 美人关 */
#define PASS_LEVELS_RAID	29		/* 过关斩将 */
#define ROLE_SKILL_UPGRADE_STAR 30	/* 主角技能升星 */
#define BUY_PHYSICAL_POWER	31		/* 购买体力 */
#define SHOPPING			32		/* 商城购买 */
#define CYCLE_MOPUP			33		/* 沙场百战扫荡 */
#define DEVIL_RESET			34		/* 魔化神将重置 */
#define GEM_UPGRADE			35		/* 宝石升级 */
#define GEM_LEVEL			36		/* 宝石总级 */
#define BOW_LEVEL			37		/* 弓箭升阶 */
// 日常/成就

#ifndef __LIB_MINI_SERVER__
#define CHECK_TARGET_PROGRESS(psu, type, ...) do { \
	 func_ptr f = (func_ptr)scene_role_target::get_func_ptr(type);	\
	 if(NULL != f) { \
		 f(psu, type, ##__VA_ARGS__);	\
	 }	\
}while(0)
#else
#define CHECK_TARGET_PROGRESS(psu, type, ...)
#endif


class scene_user : public miniserver::user, public scene_role
{
public: // 基础方法相关
    scene_user(player_role* p);
    virtual ~scene_user();

public: // 初始化相关
    virtual int init();
    int init(uint32 mid, uint32 sid);
    int init_user_attr();
    int init_user_skill();
    int init_user_buffer();

    inline void   set_agent_id(uint32 id) { _agent_id = id; }
    inline uint32 get_agent_id() const { return _agent_id; }

    inline void   set_client_id(uint32 id) { _client_id = id; }
    inline uint32 get_client_id() const { return _client_id; }

    inline void set_scene_ready(bool v) { _scene_ready = v; }
    inline bool get_scene_ready() const { return _scene_ready; }

    virtual int attach_scene(scene_inst* s);
    virtual int detach_scene();

public: // 保存相关
    void save_user();
    void save_user_attr();
    void save_user_status();
	void reset_user_weapon();

	uint32 get_store_weapon() { return get_attr().get_val(STORE_WEAPON); }
	void set_store_weapon(uint32 id) { mutable_attr()->set_val(STORE_WEAPON, id); }
	uint32 get_king_weapon() { return _king_weapon; }
	void set_king_weapon(uint32 id) { _king_weapon = id; }

public: // 副本操作相关
    inline uint32 get_raid_type() const { return _raid_type; }
    inline void set_raid_type(uint32 val) { _raid_type = val; }

    void on_raid_complete(uint32 raid_id, uint32 type, uint32 star);

    void auto_raid();

    void enter_mini_server(uint32 raid_id, uint32 enter_time);
    bool check_mini_server(uint32 raid_id, uint32 leave_time);

	size_t get_pet_count();

#ifdef __LIB_MINI_SERVER__
    void set_raid_complete_count(uint32 count);
#endif // __LIB_MINI_SERVER__
    uint32 get_raid_complete_count(uint32 id);

public: // 协议处理相关
    int on_pb_buy_goods_npc_shop(client::buy_goods_req* b, client::buy_goods_rsp& rsp);
    int on_pb_black_shop_req(client::black_shop_req* req, client::black_shop_rsp& rsp_msg);
	int on_pb_black_shop_reflesh_req(client::black_shop_reflesh_req* req,client::black_shop_reflesh_rsp& rsp_msg);
	int on_pb_black_shop_buy_req(client::black_shop_buy_req* req,client::black_shop_buy_rsp& rsp_msg);
	int on_pb_sign_list_req(client::sign_list_rsp& rsp_msg);
	int on_pb_sign_in_req(client::sign_in_req* req,client::sign_in_rsp& rsp_msg);
	int on_pb_land_reward_req(client::land_reward_req* req,client::land_reward_rsp& rsp_msg);
	int on_pb_land_reward_list_req(client::land_reward_list_rsp& rsp_msg);
	int on_pb_gem_list_req(client::gem_list_req* req,client::gem_list_rsp& rsp_msg);
	int on_pb_gem_level_up_req(client::gem_level_up_req* req,client::gem_level_up_rsp& rsp_msg);

public: // 邮件数据填相关
    int fill_send_mail_info(client::send_mail_req* smreq, vector<client::props_info>& attach_ment, client::delivery_mail_info& dmi);
    int send_mail_attachment(client::send_mail_req* smreq, vector<client::props_info>& attach_ment);
    int send_mail_cost(client::send_mail_req* smreq);
    int send_mail_attach_attachment(client::mail_info& mi, vector<client::props_info>& attach_ment);
	uint32 get_mail_past_time(uint32 mail_type,bool has_adjunct = false);

public: // 跳转相关
    int  on_jump_scene_by_scene(client::jump_scene* js, client::jump_user& ju);
    int  fill_back_trans_jump(client::jump_user& ju);
    int  fill_default_trans_jump(client::jump_user& ju);
    int  fill_spa_trans_jump(client::jump_user& ju);
    void init_formal_pos();
    void save_formal_pos();
    void save_formal_pos(uint32 scene_id, uint32 x, uint32 y);
    int  trans_back_user();
    int  trans_back_user_at_once();
	void modify_offline_timelong();

public: // 效果相关
    ITEM_OPERATE_TYPE props_use_effect(uint32 grid_index, uint32 props_count, client::role_data rd);
    ITEM_OPERATE_TYPE chang_pos(const config_props* base_info, uint32 count);
    ITEM_OPERATE_TYPE use_gift(const config_props* base_info, uint32 count);
    ITEM_OPERATE_TYPE use_item(const config_props* base_info, uint32 count, client::role_data rd);
    ITEM_OPERATE_TYPE change_role_attr(const config_props* base_info, uint32 count = 1);
    ITEM_OPERATE_TYPE compound_item(const config_props* base_info, uint32 count);
	ITEM_OPERATE_TYPE bow_skill_study(const config_props* base_info, uint32 count);

public: // 更新相关
    virtual bool on_update(uint32 tick, uint32 user_num, bool is_one_sec, bool is_fiv_sec, bool is_ten_sec, bool is_thirty_sec, bool is_one_minute);
    virtual bool on_update_one_second(uint32 tick, uint32 user_num);
    virtual bool on_update_fiv_second(uint32 tick, uint32 user_num);
    virtual bool on_update_ten_second(uint32 tick, uint32 user_num);
    virtual bool on_update_thirty_second(uint32 tick, uint32 user_num);
    virtual bool on_update_one_minute(uint32 tick, uint32 user_nmu);

    bool on_update_move(uint32 tick);
    bool on_update_practice(uint32 tick);
    bool on_update_status(uint32 tick);
    bool on_update_pet_fight_status();
	bool change_pre_pet_attr();
    bool on_update_skill_point();
	bool on_update_black_shop();
	bool on_update_mount_bless_time();
	bool on_update_bow_bless_time();
	bool on_update_props_date();
    virtual int on_killed(scene_role* psr);
    virtual int on_kill(scene_role* psr);
    virtual int on_attacked(scene_role* psr, skill& sk, effect_result& atk_rsp, bool& trigger);

    virtual int on_region_check(const region_data& r);
    virtual int on_leave_region(const region_data& r);
    virtual int on_enter_region(const region_data& r);

    virtual bool on_watch(scene_role* sr);
    virtual void on_dead(uint32 tick);

    int on_picked(scene_npc* n);

    int on_level_up(uint32 old_level, uint32 now_level);
    int on_level_up_skill(uint32 old_level, uint32 now_level);
    int on_level_up_attr(uint32 old_level, uint32 now_level);
    int on_level_up_rank(uint32 old_level, uint32 now_level);
    int on_level_up_vip(mysql::tb_player_role& tpr, uint32 old_level, uint32 now_level);
    int on_level_up_task(mysql::tb_player_role& tpr, uint32 old_level, uint32 now_level);
    int on_level_up_open(uint32 old_level, uint32 now_level);
	int on_level_up_gem(uint32 now_level);

	int check_bow_open();

    void justifiable_defense(scene_user* su);
    bool in_justifiable_defense(scene_role* sr);

    void check_equip_timeout(uint32 sec);

    void on_convoy_task_start(uint32 quality, bool notify);
    void on_convoy_task_end(bool active_end = false, uint32* quality = NULL);
    virtual void on_rob_convoy(uint32 quality);

    void update_mount_info(uint32 mount_level, uint32 mount_power);

    void reset_pack_size(uint32 pack_size, uint32 depot_size);

	bool reflesh_shop();

	void find_replace_bow_skill(uint32& need_replace_id,uint32& rand_ser_index);

public: // 给客户端发送数据相关
    void send_client_change_props_msg();

public: // 填充数据与其它模块接口相关
    void fill_query_attr(client::role_attr& ra);
    void fill_user_msg(client::role_msg& rm);
	// 填充当前日期等等数据
	void fill_cur_date(client::day_info& data_info);

    void fill_props_msg(client::role_props_msg& rpm);
    void fill_raid_info_msg(client::raid_info_msg& rim);

    virtual int send_agent(struct amtsvr_context* ctx, google::protobuf::Message& msg);
    virtual int send_agent(google::protobuf::Message& msg);

    virtual int send_client(struct amtsvr_context* ctx, google::protobuf::Message& msg);
    virtual int send_client(google::protobuf::Message& msg);

public: // 家族相关
    uint32 get_family_id() { return get_attr().family_id(); }
    uint32 get_family_position_id() { return get_attr().family_position(); }
    int modify_family_power(uint32 power);
    int update_power(uint32 fight_power);

#ifndef __LIB_MINI_SERVER__
public:
    void update_online_smy_info(client::UPDATE_TYPE_E type, uint32 val1, uint32 val2 = 0);		// 有需要可以扩充传入参数
#endif // __LIB_MINI_SERVER__

public: // 竞技场相关
	void add_target_id(uint32 id) { _target.push_back(id); }
	const std::vector<uint32> &get_target() { return _target; }
	void clear_target() {_target.clear();}
    uint32 get_arena_used_times();
    uint32 get_arena_left_fresh_time();
	void reset_arena_used_times();
	void set_arena_used_times(uint32 num);
	uint32 get_arena_chall_time();
	void clear_arena_cd_time();
	uint32 get_arena_reset_times();
	uint32 get_challenge_cd();

public: // 属性状态相关
    virtual void stand(bool broadcast = true);
    virtual void relive();
    virtual void reset_original_attr();
    virtual void calculate_attr();
    virtual void calculate_status();

    uint32 calculate_fight_power();
    void calculate_offline_data();
    void calculate_skill_point();
	uint32 calculate_role_bow_speed(uint32 bow_id,uint32 bow_level);
    uint32 get_money(ROLE_ATTRIBUTE type) const { return get_attr().get_val(type); }
    int set_money(ROLE_ATTRIBUTE type, uint32 num);
    int add_money(ROLE_ATTRIBUTE type, uint32 num);
    int sub_money(ROLE_ATTRIBUTE type, uint32 num);

	uint32 add_family_exp(uint32 origin_exp);
    int add_exp(uint64 exp);
    int add_normal_exp(uint64 exp);	// 不受经验丹加成
    uint32 cal_kill_npc_exp(scene_npc* npc);

    void add_anger(uint32 anger);
    void add_physical_power(uint32 num);

    int recover_physical_power(uint32 tick);

    void on_arena_result(bool win);

    void   add_filed_score(uint32 score);
    void   sub_filed_score(uint32 score);
    uint32 get_filed_score();

    void set_offline_time(uint32 t);
    void set_skill_point_time(uint32 t);

    int set_robot(const bool s);
    int set_riding(const client::role_status_riding_status s);
    int set_atk_mode(const client::role_status_attack_mode m);
    bool is_same_map_and_scene(uint32 m_id, uint32 s_id);

    int recharge(int n);
    uint32 get_vip_lv();
	int set_vip_lv(uint32 lv, bool month);

    virtual void fall_from_mounts(uint32 tick);

public:
	bool has_func_open(uint32 func_id);

public: // 装备相关
	int add_drop_props(uint32 drop_id, client::raid_result_info *rri = NULL, uint32 timeout = 0);
	int add_drop_props(uint32 drop_id,vector<client::props_info>& item_vec,uint32 time_out = 0);
	int add_props(const vector<uint32> &drops, client::raid_result_info *rri = NULL);
	int add_props(const vector<int> &drops, client::raid_result_info *rri = NULL);
	// 包含临时背包的道具添加
	void super_add_props(std::vector<client::props_info>& pis,bool is_send = true);

    bool add_props(std::vector<client::props_info>& pis,bool is_send = true);
    bool add_props(const client::props_info& pi, bool notify = true);
    int  add_props(uint32 id, uint32 num, uint32 bind = 1, uint32 quality = 0, bool notify = true);
	int  add_melting_prop(uint32 id, uint32 num);
    int  sub_props(uint32 id, uint32 num);
	bool sub_super_props(const std::vector<int>& sub_item_id,uint32 need_count,std::map<uint32,uint32>& total_expend);// 顺序（该接口调用前要确保物品足够扣）
	bool calc_grindex_sub_count(uint32& need_count,const std::vector<int>& sub_item_id,std::map<uint32 ,uint32>& expend_map_);
	bool sub_props(std::map<uint32 ,uint32>& expend_map_);
    bool check_pack_space(uint32 props_id);
    uint32 get_free_space();
    uint32 get_props_num(uint32 prop_id);
    bool is_can_add_props();
	uint32 cal_equip_reward_power();
	bool is_add_attack_count();
#ifdef __LIB_MINI_SERVER__
    void add_equip(uint32 role_id, std::vector<client::props_info>& pis);
#endif // __LIB_MINI_SERVER__

public: // 打坐与双修
    void   practice();
    bool   can_comm_practice(bool* agree = NULL);
    bool   comm_practice(uint64 user_inst_id);
    bool   can_get_comm_practice_list();
    void   set_auto_agree_comm_practice(bool ret) { _auto_comm_practice = ret; }
    bool   get_auto_agree_comm_practice() { return _auto_comm_practice; }
    uint64 get_comm_practice_user() { return _comm_practice_user; }
    void   stop_practice();

public: // 采集相关
    client::PICK_RET_E pick(uint64 sid);
    client::PICK_RET_E pick_commit();
    void               pick_remove();

public: // 邮件相关
    void add_mail(client::delivery_mail_info& dmi);

public: // 系统消息
    static const uint32 SYS_MSG_TO_FRIEND = 1;
    static const uint32 SYS_MSG_TO_TEAM = 2;
    static const uint32 SYS_MSG_TO_FAMILY = 3;
    static const uint32 SYS_MSG_TO_SCENE = 4;
    void send_sys_msg(uint32 msg_id, char* cmd, uint32 type = 0);
    void sys_msg_enhance(uint32 goods_id, uint32 enhance_lvl);
    void sys_msg_stone(uint32 goods_id, uint32 num);
    void sys_msg_imquality(uint32 material_id, uint32 goods_id);
    void sys_msg_be_kill(scene_role* src_role);
    void sys_msg_kill();
    void sys_msg_explore(uint32 place_id, uint32 goods_id);
    void sys_msg_pick(uint32 npc_id, uint32 goods_id);
    void sys_msg_pet(uint32 pet_id);
    void sys_msg_advance_pet(uint32 pet_id, uint32 lvl);
    void sys_msg_half_year_vip();
    void sys_msg_hour_vip();
    void sys_msg_wing(uint32 wing_id);
    void sys_msg_upgrade_wing(uint32 wing_id);
    void sys_msg_domain();
    void sys_msg_target(uint32 id);
    void sys_msg_label(uint32 raid_id, uint32 label);
    void sys_msg_shop(uint32 num, uint32 goods_id);
    void sys_msg_convoy(scene_role* sr = NULL);
    void sys_msg_aircraft(uint32 id);
    void sys_msg_score(uint32 num);

public:
	void set_old_challenge_count(uint32 count) { _challenge_count = count; }
	uint32 get_old_challenge_count() { return _challenge_count; }

public: // 活跃度
    void   add_activity(uint32 count);

public: // 排行榜相关
    void update_lv_rank();								// 等级榜
    void update_power_rank();							// 战力榜
    void update_mounts_rank(int id, int lv);			// 坐骑榜
    void update_potential_rank(int lv);					// 心法榜
	void update_bows_rank(int id, int lv);			    // 弓箭榜
	void update_gem_rank(int lv);						// 宝石榜
	void update_equip_power_rank(uint32 limit_type,int new_power);	    // 装备排行榜（人物装备+美人装备）
	void update_gem_integral(uint32 limit_type,int add_integral); // 宝石积分排行榜

public: // 温泉相关
    void   spa_action_by_other();
    uint32 get_spa_action_times();
    client::SPA_ACTION_RET_E spa_action_req(uint32& action_times, bool& add_other_exp);

public: // 任务相关
    inline scene_role_task& get_task() { return _task; }
    inline scene_role_task* mutable_task() { return &_task; }

public: // 封禁相关
    void set_ban(uint32 type, uint32 end);

public: // 操作日志
    void sub_money_log(uint32 root_id, uint32 type_id, uint32 old_value, uint32 new_value, uint32 goods_id = 0, uint32 num = 0);
    void save_logs();

public:
	void update_activity_record(uint32 act_limit_type,uint32 value_count);
public: // 美人相关
    bool   call_pet(uint32 pet_id, player_role_pet& prp);
    bool   ectype_call_pet(uint32 pet_id, player_role_pet& prp, client::pet_fight_mode_change_rsp& rsp_msg,bool is_frist = false);
    bool   call_back_pet(uint32 pet_id, player_role_pet& prp, bool is_ectype, client::pet_fight_mode_change_rsp& rsp_msg);
	bool   levels_call_pet(client::pet_fight_mode_change_req* req,player_role_pet& prp,client::pet_fight_mode_change_rsp& rsp_msg,bool is_frist = false);
	bool   pet_levels_set_req(uint32 pet_id, uint32 levels_status,client::pet_levels_set_rsp& rsp_msg);
    uint32 get_current_pet();		// 当前出战的美人
    bool   has_pet(uint32 id);

public:    // 临时背包
    inline const std::vector<client::props_info>& get_props() const { return _props; }
    inline std::vector<client::props_info>* mutalbe_props() { return &_props; }

public:
	bool is_friend_family(uint32 id);
	void add_friend_family(uint32 id);
	void remove_friend_family(uint32 id);
	void clear_friend_family();

private:    // 重置相关
    int  init_day();
    int  init_task(int32 day_ret, uint32 now);
    int  init_status(player_role_status& prs, uint32 mid, uint32 sid);
    int	 init_mount_skill_count();
    int	 init_treasure_data();
    int  init_vip();
    int  reset_potential_share_exp();

private:
    uint32 _agent_id;  // 对应的服务端的agent,主动发送客户端消息时，亦可通过agent服务模块来中转数据
    uint32 _client_id; // 对应的用户客户端需要发送的数据

    uint32      _save_map_id;
    uint32      _save_scene_id;
    map_postion _save_pos; // 保存现在的位置，用于跳转副本前存储原来地图的位置

    bool _scene_ready;  // 用户是否准备好场景，如果没有准备好场景则不给发送其它角色的相关信息和数据, 通过客户端去做数据屏蔽处理

    uint32 _practice_time;          // 开始打坐的时间
    uint32 _refresh_practice_time;  // 刷新双修列表时间
    uint32 _comm_practice_time;     // 双修开始时间
    bool   _auto_comm_practice;     // 自动同意双修
    uint64 _comm_practice_user;     // 双修的对象

    uint32 _physical_tick;

    uint64 _pick_npc_inst_id; // 正在采摘的NPC ID

    uint32                   _sub_infamy_timer; // 在线一小时恶名值减少标记
    std::map<uint32, uint32> _defenses;         // 正当防卫攻击列表

    scene_role_task     _task;

    uint32 _raid_type;
#ifdef __LIB_MINI_SERVER__
    uint32 _raid_count;
#endif // __LIB_MINI_SERVER__

    std::vector<client::props_info> _props; // 临时背包

	std::vector<uint32> _target; // 竞技场挑战对手/组队的队友

	// friend family list;
	std::map<uint32, int> _ffl;

	uint32 _store_weapon;
	uint32 _king_weapon;

    uint32 _raid_id;
    uint32 _raid_time;
	uint32 _challenge_count;
};

#endif
