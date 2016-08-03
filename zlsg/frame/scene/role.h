#ifndef __ROLE_H__
#define __ROLE_H__

#include "postion.h"
#include "scene_define.h"
#include "role_skill.h"
#include "role_buffer.h"
#include "role_effect.h"
#include "scene_role_attr.h"
#include "effect_result.h"

class role
{
public:
    role();
    role(const role& rhs);
    virtual ~role();

public:
    virtual int init() = 0;
    role& operator=(const role& rhs);

public: // 人物属性相关
    inline const map_postion& get_map_pos()  const { return _pos; }
    inline const map_postion& get_born_pos() const { return _pos; }
    inline void set_born_pos(const map_postion& p) { _pos = p; }

    virtual uint32 get_speed ();
    virtual float  get_speedf();

    inline uint32 get_map_id() const { return _map_id;   }
    inline void   set_map_id(uint32 map_id) { _map_id = map_id; }
    inline uint32 get_scene_id() const { return _scene_id; }
    inline void   set_scene_id(uint32 scene_id) { _scene_id = scene_id; }
    inline uint8  get_role_type() const { return _role_type; }
    inline void   set_role_type(uint8 rt) { _role_type = rt; }
    inline uint32 get_feature() const { return _feature; }
    inline void   set_feature(uint32 val) { _feature = val; }

    inline void   set_attr(ROLE_ATTRIBUTE type, uint32 val) { _attr.set_val(type, val); }
    inline uint32 get_attr(ROLE_ATTRIBUTE type) const { return _attr.get_val(type); }
    inline const scene_role_attr& get_attr    () const { return  _attr; }
    inline       scene_role_attr* mutable_attr()       { return &_attr; }

	virtual const char * get_role_name() const { return _role_name; }
   // inline const char*  get_role_name () const { return _role_name; }
    inline void set_role_name (const char* n, uint32 len) {
        len = min(max(1, (int)len), MAX_NAME_LEN - 1);
        strncpy(_role_name, n, len);
        _role_name[len] = '\0';
    }

    inline uint32 get_role_id   () const     { return _role_id; }
    inline void   set_role_id   (uint32 id)  { _role_id = id; }
    inline uint32 get_level     () const     { return _attr.level(); }
    inline void   set_level     (uint32 lvl) { _attr.set_val(LEVEL, lvl); }
    inline uint32 get_job       () const     { return _attr.profession(); }
    inline uint64 get_experience() const     { return _attr.experience(); }
    inline void   add_experience(uint64 exp) { _attr.set_val_64(EXPERIENCE, _attr.experience() + exp); }
    inline void   sub_experience(uint64 exp) {
        if (_attr.experience() > exp) {
            _attr.set_val_64(EXPERIENCE, _attr.experience() - exp);
        } else {
            _attr.set_val_64(EXPERIENCE, 0);
        }
    }

    inline virtual void reset_original_attr() {}

public: // 人物技能相关
    inline void set_skill(const role_skill& skills) { _rsk = skills; }
    inline       role_skill* mutable_skill()       { return &_rsk; }
    inline const role_skill& get_skill    () const { return  _rsk; }

    virtual uint32 get_attack_range(skill* s) const;

    int  skill_cost(skill* sk);

public: // buff相关
    inline       role_buffer* mutable_buffer()       { return &_buf; }
    inline const role_buffer& get_buffer    () const { return  _buf; }

public: // 数值计算相关
    void operate_attr(uint32 calculate_type, ROLE_ATTRIBUTE type, uint32 val);

protected:
    uint32          _role_id;  
    char            _role_name[MAX_NAME_LEN];
    uint8           _role_type;
    uint32          _feature;                                    // 子类型
    uint32          _scene_id;
    uint32          _map_id;
    map_postion     _pos;    // 出生点位置

    uint32          _session;

    role_skill      _rsk;
    role_buffer     _buf;

    scene_role_attr _attr;
    scene_role_attr _modify_attr;
};

#endif
