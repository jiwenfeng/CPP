#ifndef __SCENE_PET_H__
#define __SCENE_PET_H__

#include "pet.h"
#include "scene_npc.h"

class scene_user;
class origin_npc;

class scene_pet : public pet, public scene_npc
{
public:
    scene_pet(const origin_npc& on, client::pet_db_info* prp, uint64 pet_inst_id, scene_role* master ,bool is_full = true);
    virtual ~scene_pet();

public:
    virtual int  init(bool is_full,const origin_npc& on);
    void init_scene_pet_skill();
	void init_scene_pet_skill(const vector<skill>& v);
    virtual bool on_update(uint32 tick, uint32 user_num, bool is_one_sec, bool is_fiv_sec, bool is_ten_sec, bool is_thirty_sec, bool is_one_minute);
    virtual bool on_update_fiv_second(uint32 tick, uint32 user_num);
    virtual bool on_status_stand(uint32 tick);
    virtual bool on_status_dead (uint32 tick);
    virtual int  on_kill(scene_role* psr);
    virtual int  on_killed(scene_role* psr);
    virtual void on_reset(bool reborn = true);
    virtual uint32 sub_hp(uint32& hp);

    virtual uint32 get_camp()      const;
    virtual uint32 get_team_id()   const;
    virtual uint32 get_family_id() const;

    virtual void fill_role_data(client::role_data& rd) const;
    virtual void fill_role_name(client::role_name& rn);

public:
	void set_origin_attr(const std::map<int, int> &m);
    void set_origin_attr(origin_npc* origin_obj,const std::map<int, int>& a);
    void add_equip_attr(origin_npc* origin_obj,uint32 pet_id,const config_magic* magic_info);

public:
    inline uint32 get_kill_count() const { return _kill_count; }
    inline void   set_kill_count(uint32 kill_count) { _kill_count = kill_count; } 
    inline void   add_kill_count() { ++_kill_count; }
    inline uint32 get_pet_status() const { return _pet_status; }

public:
	virtual void add_counter_role(scene_role *sr, uint32 hate);
	virtual void del_counter_role(scene_role *sr);
	virtual void clear_counter_list();
	virtual bool is_counter_role(scene_role *sr);

public:
    // 宠物升级或穿脱装备 重算下属性
    bool on_pet_attr_change(uint32 pet_id,uint32 opt_type);
    // 宠物效果升级 重算下属性
    bool on_skill_effect_up(uint32 pet_id, uint32 skill_id, uint32 upgrade_level);
    // 重新计算美人属性（只针对出战中的美人）继承主人属性计算
    bool recalculation_pet_attr(uint32 pet_id, uint32 pet_level, const origin_npc& on, bool is_full_hp = false);

protected:
    virtual scene_role* get_hatest_role();

private:
    uint32 _kill_count;
    uint32 _pet_status;	// 状态 出战与否 1 出战 2休战
};

#endif
