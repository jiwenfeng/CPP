#ifndef __ORIGIN_NPC_H__
#define __ORIGIN_NPC_H__

#include "npc.h"
#include "scene_role_ai.h"

class origin_npc : virtual public role
{
public:
    origin_npc(const origin_npc& rhs);
    origin_npc(const config_npc* p);
    virtual ~origin_npc();

public:
    origin_npc& operator=(const origin_npc& rhs);

public:
    virtual int  init();
    virtual void init_original_attr(client::role_attr& a) const;
	virtual int  init_attr(scene_role_attr& a) const;

    virtual uint32 get_attack_range(skill* s = NULL) const;

public:
    inline const config_npc* get_config_npc() const { return _cn; }
    inline const scene_role_ai& get_ai() const { return _ai; }

    inline uint32 get_chase_range() const { return _chase_range; }
    inline uint32 get_attk_type() const { return _attk_type; }
    inline uint32 get_attack_cool() const { return _attack_cool; }
    inline uint32 get_revi_time() const { return _revi_time; }

    inline bool is_rand_move() const { return _rand_move; }
    inline uint32 get_rand_move_space() const { return _rand_move_space; }
    inline uint32 get_monster_type() const { return _monster_type; }
    inline uint32 get_disappear() const { return _disappear; }
    inline void   set_disappear(uint32 disappear) { _disappear = disappear; }

    bool can_create(uint32 now) const;
    bool can_relive() const;
    bool is_active_attack() const;
    bool is_monster() const;
    bool is_boss() const;
    bool is_world_boss() const;
    bool is_picker() const;
    bool is_initiative_attack() const;
	void set_ai(const string &ai) { _ai.set_ai(ai); }

protected:
    virtual int init_skill();

protected:
    const config_npc*        _cn;
    scene_role_ai            _ai;

    uint32                   _chase_range;                                // ×·»÷·¶Î§
    uint32                   _visible_range;                              // ÊÓÒ°·¶Î§
    uint32                   _attk_type;                                  // ¹¥»÷ÀàÐÍ
    uint32                   _attack_cool;                                // ¹¥»÷¼ä¸ô
    uint32                   _revi_code;                                  // ¸´»î¹æÔò
    uint32                   _revi_time;                                  // ¸´»î¼ä¸ô
    bool                     _rand_move;
    uint32                   _rand_move_space;                            // Ñ²Âß¼ä¸ô

    uint32                   _monster_type;

    uint32                   _disappear;
};

#endif
