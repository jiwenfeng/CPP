#ifndef __ROLE_BUFFER_H__
#define __ROLE_BUFFER_H__

#include <map>
#include "config_effect.h"
#include "scene_define.h"
#include "client.effect.pb.h"

class role_buffer_info : public client::buffer_info
{
public:
    role_buffer_info() {
        _operate_flag = client::BLME_ADD;
        _ce           = NULL;
    }

    role_buffer_info(const role_buffer_info& rhs) : client::buffer_info(rhs) {
        _operate_flag = rhs._operate_flag;
        _ce           = rhs._ce;
    }

    role_buffer_info(const client::buffer_info& rhs) : client::buffer_info(rhs) {
        _operate_flag = client::BLME_ADD;
        _ce           = CONFMGR->get_config_effect(rhs.effect_id());
    }

    virtual~ role_buffer_info() {

    }

public:
    inline role_buffer_info& operator=(const role_buffer_info& from) {
        CopyFrom(from);
        _operate_flag = from._operate_flag;
        _ce           = from._ce;
        return *this;
    }

    inline role_buffer_info& operator=(const buffer_info& from) {
        CopyFrom(from);
        _operate_flag = client::BLME_ADD;
        _ce           = CONFMGR->get_config_effect(from.effect_id());
        return *this;
    }

    void set(const client::buffer_info& bi) {
        CopyFrom(bi);
        _ce = CONFMGR->get_config_effect(bi.effect_id());
    }

    inline void set_operate_flag(client::BUFFER_LIST_METHOD_E of) { _operate_flag = of; }
    inline const client::BUFFER_LIST_METHOD_E get_operate_flag() const { return _operate_flag; }

    inline const void set_config_effect(const config_effect* ce) { _ce = ce; }
    inline const config_effect* get_config_effect() const { return _ce; }

protected:
    client::BUFFER_LIST_METHOD_E _operate_flag;
    const config_effect*         _ce;
};

class player_role_buffer;
class scene_role_attr;
class scene_role_status;

class role_buffer
{
public:
    role_buffer(player_role_buffer* b = NULL);
    role_buffer(const role_buffer& rhs);
    virtual ~role_buffer();

public:
    inline void set_prb(player_role_buffer* b) { prb = b; }

    int init_buf(const client::buffer_info& bi);

    int reset_buf();

    int add_buf(const client::buffer_info& bi);
    int add_buf(const config_effect* effect, client::role_data* caster = NULL);
    int add_buf(const config_effect* effect, role_buffer_info& rbi);
    int add_buf_time(const config_effect* effect, role_buffer_info& rbi, bool change = true);
    int add_buf_value(const config_effect* effect, role_buffer_info& rbi, bool change = true);
    int del_buf(const config_effect* effect);
    int del_buf(uint32 effect_id);
    int del_buf(uint32 useful_count, uint32 harmful_count);
    int del_buf(const std::vector<int>& types);
    int rep_buf(const config_effect* effect);
    int update(uint32 tick, uint32& effect_count);

    uint32 fill_all_buffer_list(client::buffer_list& bl);
    uint32 fill_change_buffer_list(client::buffer_list& bl);

    void save_buffer();

    void on_dead();

public:
    bool del_raid_buf();

public:
    inline const std::multimap<uint32, role_buffer_info>& get_buf() const { return mapbuf; }
    inline bool                                    is_change  () const { return _change ; }
    inline bool                                    is_trigger() const { return _trigger; }
    inline void                                    trigger(bool val) { _trigger = val; }

    inline bool                                    is_broadcast() const { return !change_buffer.empty(); }
    inline std::vector<client::buffer_effect_val>& get_attr   () { return _attr   ; }
    inline std::vector<client::buffer_effect_val>& get_status () { return _status ; }
    inline std::vector<client::trigger_skill>&     get_skills () { return _skills ; }
    inline const client::taunt_effect              get_taunt  () const { return _taunt  ; }
    inline const client::buffer_effect_val&        get_seckill() const { return _seckill; }

    //void get_halos(uint32 distance, std::vector<const config_effect*>& halos);

    void calculate_attr(scene_role_attr* p);
    void calculate_status(scene_role_status* p);
    void calculate_shield(scene_role_attr* p, uint32& damage);
    void calculate_rebound(uint32 damage, uint32& rebound);

protected:
    int add_buf_exist(role_buffer_info& src_buf, const config_effect* effect);
    int add_buf_new(const config_effect* effect, client::role_data* caster);
    int set_buf(const config_effect* effect, role_buffer_info& rbi);
    bool check_immune(const config_effect* effect);

    void fill_buffer_info(const role_buffer_info& src, client::buffer_info& bi);

    int on_timer_buffer(uint32 tick, role_buffer_info& rbi);

    int on_timeout(uint32 tick, role_buffer_info& rbi);

    void set_attr(const config_effect* effect, role_buffer_info& rbi);
    void buf_set_attr(const config_effect* effect, role_buffer_info& rbi);
    void buf_set_attr(const std::map<int, cpf_val_2>& al, role_buffer_info& rbi);
    void buf_set_status(const config_effect* effect, role_buffer_info& rbi);
    void buf_set_shiled(const config_effect* effect, role_buffer_info& rbi);
    void buf_set_rebound(const config_effect* effect, role_buffer_info& rbi);
    void buf_set_skills(const config_effect* effect, role_buffer_info& rbi);
    void buf_set_immune(const config_effect* effect, role_buffer_info& rbi);
    void buf_set_taunt(const config_effect* effect, role_buffer_info& rbi);
    void buf_set_seckill(const config_effect* effect, role_buffer_info& rbi);
    int on_buffer_trigger(role_buffer_info& rbi);

    uint32 shield_each_bear_harm(client::each_bear_harm *ebh, uint32 damage);

public:
    static const uint8 ADD_RET_IMMUNE = 1;

protected:
    player_role_buffer*                        prb;
    std::multimap<uint32, role_buffer_info>    mapbuf;
    std::vector<role_buffer_info>              change_buffer;

    bool                                       _change;
    bool                                       _trigger;
    std::vector<client::buffer_effect_val>     _attr;
    std::vector<client::buffer_effect_val>     _status;
    std::vector<client::trigger_skill>         _skills;
    std::vector<client::immune_effect>         _immune;
    client::taunt_effect                       _taunt;
    client::buffer_effect_val                  _seckill;
};


#endif
