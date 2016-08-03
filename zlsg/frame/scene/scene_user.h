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
#define PRACTICE_LIST_INTERAL         2      // 2��������һ���б�
#define MEDITATE_STRATEGY_LEVEL_LIMIT 1      // ����������ֵ�ȼ�����


#define LEVEL_FUNC_ID					13
#define BOX_FUNC_ID						14
#define OFFLINE_FUNC_ID					17
#define CYCLE_FUNC_ID					20
#define DEVIL_FUNC_ID					21

//	��־λ����
#define INSERT_FLAG(value, flag)	{(value) |= (flag);}
#define REMOVE_FLAG(value, flag)	{(value) &= ~(flag);}
#define HAS_FLAG(value, flag)		( ((value) & (flag)) != 0 ? true : false )

// Ԫ������
#define MC_BUY_PHYSIC						1	// ��������
#define MC_BUY_MONEY						2	// ����ͭǮ
#define MC_RESET_RAID						3	// ���þ�Ӣ����
#define MC_RESET_ARENA						4	// ���þ�������ս����
#define MC_BUY_SKILL_POINT					5	// �����ܵ����
#define MC_RESET_MOUNTS_SKILL_COUNT			6	// �������＼��
#define MC_RESET_CYCLE						7	// ����ɳ����ս
#define MC_RESET_DEVIL						8	// ����ħ����
#define MC_CLEAR_ARENA_CD					101	// ���������CD
#define MC_BUY_BOX							102	// �����Զ�����
#define MC_BUY_OFFLINE_EXP					103	// ���߹һ�����
#define MC_BUY_ACTIVITY						104 // �����
#define MC_CREATE_FAMILY					105	// ��������
#define MC_BUY_PROP							106	// �̳ǹ���
#define MC_UPGRADE_MOUNTS_SKILL				107	// ���＼������

#define MC_VIP_SPEND			1	// VIP����
#define MC_ARENA_SPEND			2	// ������
#define	MC_TREASURE_SPEND		3	// ����
#define MC_OFFLINE_SPEND		4	// ���߹һ�
#define MC_ACTIVITY_SPEND		5	// �
#define MC_FAMILY_SPEND			6	// ����
#define MC_STORE_SPEND			7	// �̳�
#define MC_MOUNTS_SPEND			8	// ����
// Ԫ������

// �ճ�/�ɾ�
#define ROLE_UPGRADE		1		/* ����ȼ����� */
#define PASS_COPY			2		/* ͨ�ظ��� */
#define PET_UPGRADE			4		/* �������� */
#define PET_LINGERING		5		/* ���˲��� */
#define PET_NUMBER			6		/* ��ļ�������� */
#define MOUNT_ADVANCE		7		/* ������� */
#define MOUNT_UPGRADE		8		/* �������� */
#define POTENTION_ACTIVE	9		/* �����ķ� */
#define EQUIP_UP_STAR		10		/* װ������ */
#define EQUIP_XILIAN		11		/* װ��ϴ�� */
#define ROLE_SKILL_UPGRADE	12		/* ��Ҽ������� */
#define MOUNT_SKILL_UPGRADE	14		/* ���＼������ */
#define PET_SKILL_UPGRADE	17		/* ���˼������� */
#define SMELT_EQUIP			18		/* ����װ�� */
#define TREASURE			19		/* Ѱ�� */
#define ARENA_CHALLENGE		22		/* ��������ս */
#define FAMILY_DONATE		23		/* ���ž��� */
#define BUY_COPPER			24		/* ����ͭǮ */
#define GET_PHYSIC_POWER	25		/* ��ȡ���� */
#define GET_INGOT			26		/* ��ȡԪ�� */
#define PASS_TEAM_RAID		27		/* ��Ӹ��� */
#define PASS_PET_RAID		28		/* ���˹� */
#define PASS_LEVELS_RAID	29		/* ����ն�� */
#define ROLE_SKILL_UPGRADE_STAR 30	/* ���Ǽ������� */
#define BUY_PHYSICAL_POWER	31		/* �������� */
#define SHOPPING			32		/* �̳ǹ��� */
#define CYCLE_MOPUP			33		/* ɳ����սɨ�� */
#define DEVIL_RESET			34		/* ħ�������� */
#define GEM_UPGRADE			35		/* ��ʯ���� */
#define GEM_LEVEL			36		/* ��ʯ�ܼ� */
#define BOW_LEVEL			37		/* �������� */
// �ճ�/�ɾ�

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
public: // �����������
    scene_user(player_role* p);
    virtual ~scene_user();

public: // ��ʼ�����
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

public: // �������
    void save_user();
    void save_user_attr();
    void save_user_status();
	void reset_user_weapon();

	uint32 get_store_weapon() { return get_attr().get_val(STORE_WEAPON); }
	void set_store_weapon(uint32 id) { mutable_attr()->set_val(STORE_WEAPON, id); }
	uint32 get_king_weapon() { return _king_weapon; }
	void set_king_weapon(uint32 id) { _king_weapon = id; }

public: // �����������
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

public: // Э�鴦�����
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

public: // �ʼ����������
    int fill_send_mail_info(client::send_mail_req* smreq, vector<client::props_info>& attach_ment, client::delivery_mail_info& dmi);
    int send_mail_attachment(client::send_mail_req* smreq, vector<client::props_info>& attach_ment);
    int send_mail_cost(client::send_mail_req* smreq);
    int send_mail_attach_attachment(client::mail_info& mi, vector<client::props_info>& attach_ment);
	uint32 get_mail_past_time(uint32 mail_type,bool has_adjunct = false);

public: // ��ת���
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

public: // Ч�����
    ITEM_OPERATE_TYPE props_use_effect(uint32 grid_index, uint32 props_count, client::role_data rd);
    ITEM_OPERATE_TYPE chang_pos(const config_props* base_info, uint32 count);
    ITEM_OPERATE_TYPE use_gift(const config_props* base_info, uint32 count);
    ITEM_OPERATE_TYPE use_item(const config_props* base_info, uint32 count, client::role_data rd);
    ITEM_OPERATE_TYPE change_role_attr(const config_props* base_info, uint32 count = 1);
    ITEM_OPERATE_TYPE compound_item(const config_props* base_info, uint32 count);
	ITEM_OPERATE_TYPE bow_skill_study(const config_props* base_info, uint32 count);

public: // �������
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

public: // ���ͻ��˷����������
    void send_client_change_props_msg();

public: // �������������ģ��ӿ����
    void fill_query_attr(client::role_attr& ra);
    void fill_user_msg(client::role_msg& rm);
	// ��䵱ǰ���ڵȵ�����
	void fill_cur_date(client::day_info& data_info);

    void fill_props_msg(client::role_props_msg& rpm);
    void fill_raid_info_msg(client::raid_info_msg& rim);

    virtual int send_agent(struct amtsvr_context* ctx, google::protobuf::Message& msg);
    virtual int send_agent(google::protobuf::Message& msg);

    virtual int send_client(struct amtsvr_context* ctx, google::protobuf::Message& msg);
    virtual int send_client(google::protobuf::Message& msg);

public: // �������
    uint32 get_family_id() { return get_attr().family_id(); }
    uint32 get_family_position_id() { return get_attr().family_position(); }
    int modify_family_power(uint32 power);
    int update_power(uint32 fight_power);

#ifndef __LIB_MINI_SERVER__
public:
    void update_online_smy_info(client::UPDATE_TYPE_E type, uint32 val1, uint32 val2 = 0);		// ����Ҫ�������䴫�����
#endif // __LIB_MINI_SERVER__

public: // ���������
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

public: // ����״̬���
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
    int add_normal_exp(uint64 exp);	// ���ܾ��鵤�ӳ�
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

public: // װ�����
	int add_drop_props(uint32 drop_id, client::raid_result_info *rri = NULL, uint32 timeout = 0);
	int add_drop_props(uint32 drop_id,vector<client::props_info>& item_vec,uint32 time_out = 0);
	int add_props(const vector<uint32> &drops, client::raid_result_info *rri = NULL);
	int add_props(const vector<int> &drops, client::raid_result_info *rri = NULL);
	// ������ʱ�����ĵ������
	void super_add_props(std::vector<client::props_info>& pis,bool is_send = true);

    bool add_props(std::vector<client::props_info>& pis,bool is_send = true);
    bool add_props(const client::props_info& pi, bool notify = true);
    int  add_props(uint32 id, uint32 num, uint32 bind = 1, uint32 quality = 0, bool notify = true);
	int  add_melting_prop(uint32 id, uint32 num);
    int  sub_props(uint32 id, uint32 num);
	bool sub_super_props(const std::vector<int>& sub_item_id,uint32 need_count,std::map<uint32,uint32>& total_expend);// ˳�򣨸ýӿڵ���ǰҪȷ����Ʒ�㹻�ۣ�
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

public: // ������˫��
    void   practice();
    bool   can_comm_practice(bool* agree = NULL);
    bool   comm_practice(uint64 user_inst_id);
    bool   can_get_comm_practice_list();
    void   set_auto_agree_comm_practice(bool ret) { _auto_comm_practice = ret; }
    bool   get_auto_agree_comm_practice() { return _auto_comm_practice; }
    uint64 get_comm_practice_user() { return _comm_practice_user; }
    void   stop_practice();

public: // �ɼ����
    client::PICK_RET_E pick(uint64 sid);
    client::PICK_RET_E pick_commit();
    void               pick_remove();

public: // �ʼ����
    void add_mail(client::delivery_mail_info& dmi);

public: // ϵͳ��Ϣ
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

public: // ��Ծ��
    void   add_activity(uint32 count);

public: // ���а����
    void update_lv_rank();								// �ȼ���
    void update_power_rank();							// ս����
    void update_mounts_rank(int id, int lv);			// �����
    void update_potential_rank(int lv);					// �ķ���
	void update_bows_rank(int id, int lv);			    // ������
	void update_gem_rank(int lv);						// ��ʯ��
	void update_equip_power_rank(uint32 limit_type,int new_power);	    // װ�����а�����װ��+����װ����
	void update_gem_integral(uint32 limit_type,int add_integral); // ��ʯ�������а�

public: // ��Ȫ���
    void   spa_action_by_other();
    uint32 get_spa_action_times();
    client::SPA_ACTION_RET_E spa_action_req(uint32& action_times, bool& add_other_exp);

public: // �������
    inline scene_role_task& get_task() { return _task; }
    inline scene_role_task* mutable_task() { return &_task; }

public: // ������
    void set_ban(uint32 type, uint32 end);

public: // ������־
    void sub_money_log(uint32 root_id, uint32 type_id, uint32 old_value, uint32 new_value, uint32 goods_id = 0, uint32 num = 0);
    void save_logs();

public:
	void update_activity_record(uint32 act_limit_type,uint32 value_count);
public: // �������
    bool   call_pet(uint32 pet_id, player_role_pet& prp);
    bool   ectype_call_pet(uint32 pet_id, player_role_pet& prp, client::pet_fight_mode_change_rsp& rsp_msg,bool is_frist = false);
    bool   call_back_pet(uint32 pet_id, player_role_pet& prp, bool is_ectype, client::pet_fight_mode_change_rsp& rsp_msg);
	bool   levels_call_pet(client::pet_fight_mode_change_req* req,player_role_pet& prp,client::pet_fight_mode_change_rsp& rsp_msg,bool is_frist = false);
	bool   pet_levels_set_req(uint32 pet_id, uint32 levels_status,client::pet_levels_set_rsp& rsp_msg);
    uint32 get_current_pet();		// ��ǰ��ս������
    bool   has_pet(uint32 id);

public:    // ��ʱ����
    inline const std::vector<client::props_info>& get_props() const { return _props; }
    inline std::vector<client::props_info>* mutalbe_props() { return &_props; }

public:
	bool is_friend_family(uint32 id);
	void add_friend_family(uint32 id);
	void remove_friend_family(uint32 id);
	void clear_friend_family();

private:    // �������
    int  init_day();
    int  init_task(int32 day_ret, uint32 now);
    int  init_status(player_role_status& prs, uint32 mid, uint32 sid);
    int	 init_mount_skill_count();
    int	 init_treasure_data();
    int  init_vip();
    int  reset_potential_share_exp();

private:
    uint32 _agent_id;  // ��Ӧ�ķ���˵�agent,�������Ϳͻ�����Ϣʱ�����ͨ��agent����ģ������ת����
    uint32 _client_id; // ��Ӧ���û��ͻ�����Ҫ���͵�����

    uint32      _save_map_id;
    uint32      _save_scene_id;
    map_postion _save_pos; // �������ڵ�λ�ã�������ת����ǰ�洢ԭ����ͼ��λ��

    bool _scene_ready;  // �û��Ƿ�׼���ó��������û��׼���ó����򲻸�����������ɫ�������Ϣ������, ͨ���ͻ���ȥ���������δ���

    uint32 _practice_time;          // ��ʼ������ʱ��
    uint32 _refresh_practice_time;  // ˢ��˫���б�ʱ��
    uint32 _comm_practice_time;     // ˫�޿�ʼʱ��
    bool   _auto_comm_practice;     // �Զ�ͬ��˫��
    uint64 _comm_practice_user;     // ˫�޵Ķ���

    uint32 _physical_tick;

    uint64 _pick_npc_inst_id; // ���ڲ�ժ��NPC ID

    uint32                   _sub_infamy_timer; // ����һСʱ����ֵ���ٱ��
    std::map<uint32, uint32> _defenses;         // �������������б�

    scene_role_task     _task;

    uint32 _raid_type;
#ifdef __LIB_MINI_SERVER__
    uint32 _raid_count;
#endif // __LIB_MINI_SERVER__

    std::vector<client::props_info> _props; // ��ʱ����

	std::vector<uint32> _target; // ��������ս����/��ӵĶ���

	// friend family list;
	std::map<uint32, int> _ffl;

	uint32 _store_weapon;
	uint32 _king_weapon;

    uint32 _raid_id;
    uint32 _raid_time;
	uint32 _challenge_count;
};

#endif
