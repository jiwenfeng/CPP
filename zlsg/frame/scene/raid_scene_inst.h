// 副本场景实例类
#ifndef __RAID_SCENE_INST_H__
#define __RAID_SCENE_INST_H__

#include "client.raid_operate.pb.h"
#include "scene_inst.h"
#include "raid_scene.h"

#define RAID_SCENE_CLOSE_INTEVAL 6500 // 650秒回收没人的副本

class raid_over;

class raid_scene_inst : public scene_inst
{
public:
    raid_scene_inst(uint32 id, const game_map* p, const raid_scene* s, struct amtsvr_context* c, const int m = DEFAUTL_SCENE_INST_USER_NUM);
    virtual ~raid_scene_inst();

public:
    virtual int init();

    virtual int on_pick_props(scene_role* psr, client::props_info* ppi);
    virtual int on_pick_npc(scene_user* psu, scene_npc* psn);
    virtual int on_role_move(scene_role* r, const map_postion& from, const map_postion& to);
    virtual int on_attack_role(scene_role* atk, scene_role* dst_role, skill& sk, effect_result& atk_rsp);
    virtual int on_kill_role(scene_role* atk, scene_role* def);
    virtual int on_role_attr_change(scene_role* psr);
    virtual int on_story_end(scene_user* psu, uint32 id);
    virtual int on_raid_complete(bool failure = false);

    virtual bool is_time_out() const;

	virtual bool relive(scene_role *sr) { return false; }
    virtual int enter_scene(scene_npc* psn);
    virtual int enter_scene(scene_user* psu);
    virtual int leave_scene(scene_user* psu);
    virtual void on_role_leave(scene_role* psu);
    virtual void on_role_enter(scene_role* sr);

    virtual int update(uint32 tick, bool is_one_sec, bool is_fiv_sec, bool is_ten_sec, bool is_thirty_sec, bool is_one_minute);

    virtual int on_raid_timeout();
    virtual int on_time_reach() {return 0;}
	virtual void set_scene_ready(bool ready, scene_user *psu);

public:
    int raid_lose_normal(vector<client::props_info>& vpi);
    int raid_lose_flop(vector<client::props_info>& vpi);
    int raid_lose_box(uint32 index, vector<client::props_info>& vpi);
    void all_npc_dead();
    bool check_kill_role_region(uint32 region_id);
    bool check_kill_role(scene_role* atk, scene_role* sr);
//	uint32 get_survive_count();

public:
    inline uint32 get_raid_id() const { return _rsc->get_raid_id(); }
    inline const raid_scene& get_raid_scene() { return *_rsc; }

    inline uint32 get_family_id() const { return _family_id; }
    inline void set_family_id(uint32 id) { _family_id = id; }

    inline uint32 get_team_id() const { return _team_id; }
    inline void set_team_id(uint32 id) { _team_id = id; }

    inline void set_farmer_id(uint32 id) { _farmer_id = id; }
    inline uint32 get_farmer_id() const { return _farmer_id; }

    inline uint32 get_type() const { return _type; }
    inline void set_type(uint32 val) { _type = val; }

    inline void set_first(bool first) { _first = first; }

public:
    inline bool is_first() const { return _first; }
    inline bool is_elite() const { return get_type() == client::enter_raid_req_type_elite; }

protected:
    int create_cur_batch_npc();
    int create_cur_batch_npc(const vector<const origin_npc*>& von, vector<scene_npc*>& sns);
    int on_kill_batch_npc();

    virtual void settlement(uint32 settle_type);

private:
    void temp_pet_enter(scene_npc* psn);
    void temp_pet_clear(scene_role* psr);

protected:
    inline uint32 get_start_tick() const { return _start_tick; }
    inline void set_start_tick(uint32 st) { _start_tick = st; }
    inline uint32 get_raid_over_tick() const { return _raid_over_tick; }
    inline void set_raid_over_tick(uint32 tick) { _raid_over_tick = tick; }

protected:
    inline raid_over* get_raid_over() { return _ro; }

private: // test
    std::map<uint64, uint32> _dead_times;
    
protected:
    static const uint32 SETTLE_TYPE_LEAVE    = 1;
    static const uint32 SETTLE_TYPE_TIMEOUT  = 2;
    static const uint32 SETTLE_TYPE_COMPLETE = 4;
   
protected:
    const raid_scene*           _rsc;             // 副本场景配置
    uint32                      _type;
    uint32                      _family_id;       // 家族副本ID
    uint32                      _team_id;         // 队伍ID
    uint32                      _farmer_id;       // 仙园副本,所属于主人ID
    raid_over*                  _ro;
    uint32                      _start_tick;
    uint32                      _raid_over_tick;
    uint32                      _dead_npc_count;  // 死亡的NPC数量
    bool                        _complete;        // 副本是否完成
    uint32                      _complete_time;   // 副本完成时间
    uint32                      _close_raid_time; // 关闭副本时间
    bool                        _first;
	bool						_ready;
};

#endif
