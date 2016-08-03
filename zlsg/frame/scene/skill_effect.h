#ifndef __SKILL_EFFECT_H__
#define __SKILL_EFFECT_H__

#include "config_skill.h"
#include "scene_define.h"

class skill_effect
{
public:
    skill_effect();
    skill_effect(const config_skill& s);
    skill_effect(const skill_effect& rhs) { operator=(rhs); }
    virtual ~skill_effect();

public:
    virtual skill_effect& operator=(const skill_effect& rhs) {
        _cs = rhs._cs;
        return *this;
    }

public:
    inline const config_skill* get_config_skill() const { return &_cs; }
    inline uint32 get_skill_id() const { return _cs.get_skill_id(); }
    inline uint32 get_hate_val() const { return _cs.get_hate_value(); }
	inline uint32 get_skill_belong_type() const { return _cs.get_skills_belonging(); }

protected:
    void init_effect();

public:
    config_skill _cs;
};

//class skill_role_effect :public skill_effect
//{
//public:
//	skill_role_effect();
//	skill_role_effect(const config_skill_role& s);
//	skill_role_effect(const skill_role_effect& rhs) { operator=(rhs); }
//	virtual ~skill_role_effect();
//
//public:
//	skill_role_effect& operator=(const skill_role_effect& rhs) {
//		_skill_role = rhs._cs;
//		return *this;
//	}
//
//public:
//	inline const config_skill_role* get_config_skill_role() const { return &_skill_role; }
//	inline uint32 get_skill_role_id() const { return _skill_role.get_skill_id(); }
//
//public:
//	config_skill_role _skill_role;
//};
#endif
