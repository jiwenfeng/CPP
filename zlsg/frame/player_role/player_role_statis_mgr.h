#ifndef __PLAYER_ROLE_STATIS_MGR_H_
#define __PLAYER_ROLE_STATIS_MGR_H_

#include "player_role_statis.h"
#ifndef __LIB_MINI_SERVER__
#include "player_role_target.h"
#endif

class player_role_statis_mgr
{
public:
    player_role_statis_mgr();
    virtual ~player_role_statis_mgr();

public:
    int init(uint32 role_id);
    int attach_role_statis(int pb_count, cmq_svr_pb_ctx* pb_ctx);
    int push(mysql::tb_role_statis* trs);
    player_role_statis* get(PLAYER_ROLE_STATIS_TYPE_E type);
    void write(amtsvr_context* ctx);

public:
    inline std::vector<player_role_statis>& get_statis() { return _prs; }

public:
    bool   is_day_init();
    void   set_day_init();
    
	void   add_land_day();
    int    init_vip_trans(uint32 num);
    int    reset_vip_trans(uint32 num);
    uint32 get_vip_trans();
    int    sub_vip_trans(int num = 1);
    
    bool   vip_premiums();
    //int    vip_fresh_task();
    
    uint32 get_push_fig();
    int    add_push_fig(int num = 1);
    uint32 get_hide_raid(uint32 pos);
    int    set_hide_raid(uint32 pos);

public: // 战场相关记录
    void add_join_siege_battle(uint32 add_count = 1);
    void add_join_domain_battle(uint32 add_count = 1);

public: // 福利相关记录
    bool   online_day(uint32 day);
    uint32 online_day_count();
    bool   online_day_award(uint32 day);
    
    void   add_week_online_time(uint32 add_time);
    bool   last_week_online_time();
    uint32 week_time();
    
    void   add_online_meditation_time(uint32 add_time);
    void   add_offline_meditation_time();
    uint32 meditation_time();

    bool welfare_gifts(uint32 index);

    bool yellow_vip_every_day();
    bool yellow_vip_year_every_day();
    bool yellow_vip_gifts(uint32 index);

    void init_online_gifts_record();
    void add_online_time(uint32 add_time = 60);
    bool online_gifts_record(uint32 index);

    void add_test_time(uint32 add_time);
    bool test_gifts_record(uint32 index);

public: // 活跃度
    void   add_activity(uint32 count);
    uint32 get_activity();
    bool   activity_gifts(uint32 index);

    bool   family_donate();
    bool   family_welfare();
    bool   family_raid();
    bool   family_boss();
    bool   secrete_elite();
    bool   world_boss();
    bool   add_day_online(uint32 add_time);
    bool   hide_push_fig();
    bool   join_spa();
    bool   join_arena();

//public: // 换铜钱
//    uint32 exchange_tael();
//    void   add_exchange_tael();

public: // 战场相关
    bool honor_return(); // 荣誉返还

public: // 目标
#ifndef __LIB_MINI_SERVER__
    player_role_target& get_target() {return target;}
    bool target_gifts(uint32 index);
    bool target_add_gifts(uint32 index);
#endif

public: // 投资
    void   set_invest_join_time(uint32 t);
    uint32 get_invest_join_time();
    bool   is_join_invest();
    bool   join_invest();
    bool   invest_ret(uint32 index);

public: // 充值
    void add_ingot(uint32 ingot);
    uint32 get_recharge();

public: // 灵脉仙境
    bool enter_land_raid();
    bool get_land_tax_reward();

public: // 中秋活动
    uint32 get_mooncake();
    bool   add_mooncake(uint32 num);

public: // 领取家族领土每日福利
    uint32 get_domain_day_reward();
    bool add_domain_day_reward(uint32 num);

    uint32 get_domain_day_ring();
    bool add_domain_day_ring(uint32 num);

    uint32 get_siege_day_ring();
    bool add_siege_day_ring(uint32 num);

    //uint32 get_domain_worship_num(uint32 id);
   // void add_domain_worship_num(uint32 id);
    
public: // 运镖相关
    uint32 get_convoy_refresh_times();
    void add_convoy_refresh_times(uint32 num=1);

    uint32 get_convoy_rob_times();
    void add_convoy_rob_times(uint32 num=1);

public: // 野外积分
    //uint32 filed_score_exchange();
    //void add_filed_score_exchange();
    bool add_filed_score(uint32 num);
    bool sub_filed_score(uint32 num);
    uint32 get_filed_score();

public: // 家族技能属性
    uint32 get_family_skill_attr(uint32 type);
    void   add_family_skill_attr(uint32 type);
public: // 捡蛋排行奖励
    bool get_joint_reward();

public: // 仙修伙伴
    bool get_mate_reward(uint32 index);
public: //割据相关 
	bool get_saparatism_award(uint32 id);

public:
    void fill_all(client::role_statis_rsp& rsp);
    
    void fill(client::role_statis_req* req, client::role_statis_rsp& rsp);

    void fill_update(client::role_statis_rsp& rsp);

public:
    uint32 get_mate_scene_enter_times();

    void add_mate_scene_enter_times();

    uint32 get_sect_reward_times();
    void add_sect_reward_times();

    uint32 get_joint_raid_times();
    void add_joint_raid_times();

public:
	uint32 get_levels_reset_times();
	void add_levels_reset_times();

public: /* 竞技场相关 */
	void add_challenge_times();	
	void sub_challenge_times(uint32 times);
	uint32 get_challenge_times();
	uint32 get_challenge_time();
	void reset_challenge_times();
	uint32 get_reset_challenge_times();
	void clear_challenge_cd();
	void set_challenge_cd();
	uint32 get_challenge_cd();
	void recover_challenge_times(uint32 n, uint32 timelong);
	uint32 get_last_recover_time();

public:
	uint32 get_offline_timelong();
	void add_offline_timelong(uint32 t);
	void reset_offline_timelong();
	void add_total_offline_tl(uint32 t);
	uint32 get_total_offline_tl();

public:
	uint32 get_cycle_invite_count();
	void add_cycle_invite_count(uint32 t);

private:
    bool index_pos_bit(uint32 index, uint32 pos);
    void index_add(uint32 index, uint32 num);
    void index_set(uint32 index, uint32 num);
    bool is_set_bit(uint32 index, uint32 pos);

private:
    std::vector<player_role_statis> _prs;
#ifndef __LIB_MINI_SERVER__
    player_role_target         target;
#endif
};

#endif
