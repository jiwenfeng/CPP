#ifndef __ATTACK_RESULT_H__
#define __ATTACK_RESULT_H__

#include "scene_define.h"
#include "client.attack.pb.h"

class scene_role;
class skill;

enum SKILL_ATTACK_EFFECT_TYPE_E
{
    SAETE_PHYSICAL = 0, // 物理技能
    SAETE_FIVEXING = 1, // 五行技能
    SAETE_NULL     = 2, // 无属性的技能
};

class effect_result : public client::attack_rsp
{
public:
    effect_result();
    virtual ~effect_result();

public:
    inline SKILL_ATTACK_EFFECT_TYPE_E skill_atk_type(){return _atk_type;}
    inline float& skill_base_effect_val(){return _sk_base_effect_val;}
    inline float& skill_grow_effect_val(){return _sk_grow_effect_val;}
    inline float& effect_effect_val()    {return _ef_effect_val;}
    void set_atk_type(int attr_type);
    inline float effect_total_val(){return _effect_total_val;}
   
    inline int&    crit_ratio()       {return _crit_ratio;}
    inline uint32& get_skill_id()     {return _skill_id;}
    inline uint32& get_effect_times() {return _effect_times;}
    inline bool&   get_atk_effect()   {return _atk_effect;}
    inline uint32& get_mulriple()     {return _mulriple;}
    
    void add_miss_result(scene_role& effect_role);
    void add_dodge_result(scene_role& effect_role);
    void add_buff_result(scene_role& effect_role);
    void add_normal_result(scene_role& effect_role, uint32 critical, uint32 data);
    void add_metal_result(scene_role& effect_role, uint32 critical, uint32 data);

    uint32 get_result_sum();

    void   set_actual_damage(uint32 ad) { _actual_damage = ad; }
    uint32 get_actual_damage() { return _actual_damage; }

    inline void cal_effect_total_val() {
        _effect_total_val = (_sk_base_effect_val + _sk_grow_effect_val + _ef_effect_val) * _mulriple;
    }

protected:
    bool   _atk_effect;
    uint32 _effect_times;
    uint32 _skill_id;

    SKILL_ATTACK_EFFECT_TYPE_E _atk_type; // 技能的数值类型

    float _sk_base_effect_val;            // 技能基础数值
    float _sk_grow_effect_val;            // 技能成长数值
    float _ef_effect_val;                 // 效果数值              
    float _effect_total_val;              // 最终技能+效果数值
    uint32 _mulriple;

    int   _crit_ratio;
    uint32 _actual_damage;
};

#endif
