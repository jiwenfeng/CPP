#ifndef __FAMILY_SCENE_INST_H__
#define __FAMILY_SCENE_INST_H__

#include "client.family.pb.h"
#include "raid_scene.h"
#include "raid_scene_inst.h"

// 家族场景实例类
class family_scene_inst : public raid_scene_inst
{
public:
    family_scene_inst(uint32 id, const game_map* p, const raid_scene* s, struct amtsvr_context* c, const int m = DEFAUTL_SCENE_INST_USER_NUM);

    virtual ~family_scene_inst();

public:
    virtual int init();

    virtual int enter_scene(scene_user* u);

    virtual void on_role_leave(scene_role* psu);

    virtual int update(uint32 tick, bool is_one_sec, bool is_fiv_sec, bool is_ten_sec, bool is_thirty_sec, bool is_one_minute);

    virtual int get_raid_type () { return _rsc->get_raid_type(); }

    inline const raid_scene& get_raid_scene() {return *_rsc;};

    virtual int  on_kill_npc(scene_user* atk, scene_npc* npc);
    virtual int on_role_move(scene_role* r, const map_postion& from, const map_postion& to);
    
public:
    void   set_family_id(uint32 id) { _family_id = id; }
    uint32 get_family_id()          { return _family_id; }
    void   set_family_name(string name) { family_name = name; }
    string get_family_name()            { return family_name; }
    void   set_boss_time(uint32 h, uint32 m, uint32 s);
    void   call_boss_msg();
    
public:
    static uint32 family_power_to_boss_id(uint32 family_power);

public:
    //int open(client::inferno_struck_open_via* via);
    //int skill(scene_user* su, client::inferno_struck_skill_req* req, client::inferno_struck_skill_rsp& rsp);

protected:
    void monster_score_msg();
    void monster_countdown_msg();
    void result_msg(scene_user* su);

    uint32 get_space_broken(uint32 role_id);
    void   set_space_broken(uint32 role_id, uint32 num);

    uint32 get_monster_score();
    uint32 get_kill_monster(uint32 role_id = 0);
    uint32 get_kill_boss(uint32 role_id = 0);
    uint32 get_contribution(uint32 kill_monster, uint32 kill_boss);

    void refresh_npc();
    void result(uint32 win = 0);
    void leave_npc();

    void use_space_broken(uint32 type);
    void use_frost_freeze(uint32 type);
    void recover_move_speed();
    void next_round();

    void sys_msg_inferno_struck_round();
    void sys_msg_inferno_struck_win();
    
public:
    static const uint32 MAX_FROST_FREEZE = 1;
    static const uint32 MAX_SPACE_BROKEN = 3;
    
    static const uint32 MAX_MONSTER_SCORE = 30;
    static const uint32 FAMILY_SCENE_ID   = 351;

protected:
    const raid_scene* _rsc; // 副本场景配置
    uint32            _family_id;
    string            family_name;
    uint32            boss_time_h;
    uint32            boss_time_m;
    uint32            boss_time_s;
    uint32            boss_time;
    bool              call_boss;

    bool                     _inferno_struck;
    std::map<uint32, uint32> _space_broken;
    uint32                   _frost_freeze;
    uint32                   _chief_id;
    uint32                   _leave_monster;
    uint32                   _leave_boss;
    uint32                   _cur_monster;
    uint32                   _cur_boss;
    std::map<uint32, uint32> _kill_monster;
    std::map<uint32, uint32> _kill_boss;
    bool                     _is_monster_score_update;
    uint32                   _countdown;
    uint32                   _round;
    uint32                   _round_dead_npc_count;
    std::vector<uint64>      _leave_npc;
    uint32                   _old_move_speed;
    uint32                   _move_speed_recover_time;
};

#endif
