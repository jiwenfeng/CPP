#ifndef __ROLE_SKILL_H__
#define __ROLE_SKILL_H__

#include "scene_define.h"
#include "skill_effect.h"

#define MAX_SKILL_NUM 32 // 最多拥有32种技能

enum
{
    SK_COST_LIFE = 0,  // 血气
    SK_COST_FORCE,     // 真气
    SK_COST_POWER,     // 仙气
    SK_COST_ANGER,     // 怒气
    SK_COST_MAX,
};

struct skill_cost
{
    skill_cost() : ratio(0) , val(0) {

    }

    uint32 ratio; // 消耗百分比
    uint32 val;   // 消耗数量
};

class skill : public skill_effect
{
public:
    skill();
    skill(const config_skill& cs, uint32 effect_level = 1);
    skill(const mysql::tb_role_skill& tss, const config_skill& cs);
    skill(const client::skill_data& sd, const config_skill& cs);
    skill(const skill& rhs);

public:
    virtual  skill& operator=(const skill& rhs);
    bool operator==(uint32 sk_id);

public:
    int init(const config_skill* pcs = NULL);
    bool cd_pass(uint32 now, uint32 speed = 0);

    inline void set_cd(uint32 now) { cd = now; }

    inline uint32 get_distance  () const { return distance  ; }
    inline uint16 get_max_target() const { return max_target; }
    inline void   set_level(uint32 l) { level = l;}
    inline void   set_effect_id(uint32 new_effect_id) { effect_id = new_effect_id;}
    inline void   set_mas(uint32 mas_value) { mas_val = mas_value;}
    inline uint32 get_cost_anger() const { return cost[SK_COST_ANGER].val; }
    inline uint32 get_effect_id() const { return effect_id; }
	inline uint32 get_ser_index() const { return ser_index ;}
	inline void set_ser_index(uint32 cur_ser_index){ ser_index = cur_ser_index;}
public:
    uint32     id;                    // 技能ID(系统)	
    uint8      level;                 // 效果等级
    uint32     mas_val;               // 熟练度
    uint16     ser_index;             // 序列号
    uint32     cd;                    // 最后使用技能时间
    uint32     effect_id;             // 效果的ID

    uint32     distance;              // 攻击距离
    uint32     range;                 // 攻击有效范围
    uint16     max_target;            // 攻击目标上限
    skill_cost cost[SK_COST_MAX];     // 技能消耗
};

class role_skill
{
public:
    role_skill();
    role_skill(const role_skill& rhs);
    virtual ~role_skill();

public:
    role_skill& operator=(const role_skill& rhs);

public:
    void init_min_atk_range();
    void init_max_anger();
    inline uint32 get_min_atk_range() const { return _min_atk_range; }
    inline uint32 get_max_anger() const { return _max_anger; }
    inline const std::vector<skill>& get_all_skill() const { return _have_sk; }
    inline std::vector<skill>& get_all_skill() { return _have_sk; }
    inline uint16 generate_skill_ser_index() {return ++_max_ser_index;}

public:
    void clear_skill() { _have_sk.clear(); }
    bool add_skill(const skill& sk,uint32 serial_index = 0);
    bool delete_skill(uint32 skill_id,uint32 serial_index = 0);
	bool check_old_skill();
	inline bool is_gem_skill(uint32 skill_belong){ return skill_belong == SB_GEM   ;};
	inline bool is_bow_skill(uint32 skill_belong){ return skill_belong == SB_ARCHER;};
    bool upgrade_skill_effect(uint32 skill_id,uint32 upgrade_level = 1);
	bool set_skill_effect(uint32 skill_id,uint32 effect_id,uint32 level,uint32 ser_index = 0);
    bool upgrade_mount_skill_effect(uint32 skill_id);
    bool remove_all_skill();
    void set_mounts_skill_exp(uint32 skill_id,uint32 exp);
    skill* use_skill(const uint32 sk_id, uint32 attack_speed = 0);
    skill* get_skill(const uint32 sk_id,uint32 ser_index = 0);
    skill* get_skill();

    void get_skill_by_type(SKILL_TYPE st, std::vector<skill*>& vs,uint32 skill_belong = 0);

	void get_skill_by_belong(uint32 skill_belong,std::vector<skill*>& vs);

    skill* use_ai_skill(const uint32 skill_id);
    skill* get_ai_skill(const uint32 skill_id);

    inline void set_attack_speed(uint32 attack_speed) { _attack_speed = attack_speed; }

    void get_pet_skill(uint32 pet_id, uint32 pet_level, std::vector<skill>& pet_skill_id_vec) const;

    bool check_mount_skill(uint32 mount_id, const config_skill* pcs) const;

	uint32 get_gem_skill_count();

	uint32 get_bow_skill_count();

	void get_bow_skill(std::map<uint32,uint32>& bow_skill_map_);
public:
    void fill_skill_data(client::role_skill_msg& rsm);
    void fill_mounts_skill_data(client::role_skill_msg& rsm);
    void fill_pets_skill_data(client::role_skill_msg& rsm,uint32 pet_id,uint32 pet_level);
    void fill_all_pets_skill_data(client::role_skill_msg& rsm);
	void fill_bows_skill_data(client::role_skill_msg& rsm);
	void fill_all_gem_skill_data(client::role_skill_msg& rsm);
protected:
    std::vector<skill>      _have_sk;
    uint8                   _used_index;
    uint32                  _min_atk_range; 
    uint16                  _max_ser_index;
    std::map<uint32, skill> _ai_skill;
    uint32                  _attack_speed;
    uint32                  _cd;
    uint32                  _max_anger;
    uint32                  _order;
};


#endif
