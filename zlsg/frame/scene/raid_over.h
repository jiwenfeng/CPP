#ifndef __RAID_OVER_H__
#define __RAID_OVER_H__

#include "client.raid_operate.pb.h"

class scene_inst;

class genterate_raid_over_rule
{
public:
    genterate_raid_over_rule(server::raid_over_rules* rules);
    ~genterate_raid_over_rule();

public:
    void init(const std::string& success_rules, const std::string& failure_rules);

    inline void set_rules(server::raid_over_rules* rules) { _rules = rules; }

private:
    void init_success_rules(const std::string& success_rules);
    void init_failure_rules(const std::string& failure_rules);
    int  add_dead_role(const std::string& a, size_t i, server::dead_role* rule);
    int  add_get_item(const std::string& a, size_t i, server::get_item* rule);
    int  add_ai_timer(const std::string& a, size_t i, server::ai_timer* rule);
    int  add_scene_timeout(const std::string& a, size_t i, server::scene_timeout* rule);
    int  add_dead_count(const std::string& a, size_t i, server::dead_count* rule);
    int  add_role_attribute(const std::string& a, size_t i, server::role_attribute* rule );
    int  add_role_postion(const std::string& a, size_t i, server::role_postion* rule);
	int  add_survive_role(const std::string &a, size_t i, server::survive_role *rule);
private:
    server::raid_over_rules* _rules;
};

struct raid_over_role_record {
    uint32 _first_id;
    uint32 _count;

    raid_over_role_record() {
        _first_id = 0;
        _count = 0;
    }
};

class raid_over
{
public:
    raid_over(scene_inst* psi);
    raid_over(scene_inst* psi, const server::raid_over_rules& rules);
    ~raid_over(void);

public:
    inline uint32 get_region_id() const { return _region_id; }
    inline void set_region_id(uint32 val) { _region_id = val; }

public:
    int on_update(uint32 tick);

public:
    void mark_dead_role(scene_role* psr);
    void mark_items(client::props_info* ppi);
    void mark_touch(scene_role* psr);
    void mark_role_attribute(scene_role* psr);
    void mark_role_postion(scene_role* psr, const map_postion& from, const map_postion& to);

public:
    void fill_success(client::raid_success_info* p);
    void fill_failure(client::raid_failure_info* p);

private:
    void init(scene_inst* psi);
    scene_inst* get_scene_inst() const { return _si; }
    void set_scene_inst(scene_inst* val) { _si = val; }

private:
    bool check_success();
    bool check_failure();

private:
    scene_inst*                                              _si;
    const server::raid_over_rules*                           _over_rules;
    std::map<uint32, uint32>                                 _user_deads;
    std::map<uint32, uint32>                                 _role_deads;
    std::map<uint32, uint32>                                 _pos_dead_role;
    uint32                                                   _dead_count;
    std::map<uint32, uint32>                                 _items;
    std::map<uint32, uint32>                                 _touch;
    std::map<uint32, uint32>                                 _alarms;
    std::vector<scene_role*>                                 _success_attributes;
    std::vector<scene_role*>                                 _failure_attributes;
    std::map<int32, std::map<int32, raid_over_role_record> > _attributes;
    std::vector<scene_role*>                                 _success_postions;
    std::vector<scene_role*>                                 _failure_postions;
    std::map<int32, std::map<int32, raid_over_role_record> > _success_postions_m;
    std::map<int32, std::map<int32, raid_over_role_record> > _postions;
    uint32                                                   _tenms;
    client::raid_success_info                                _success_info;
    client::raid_failure_info                                _failure_info;
    uint32                                                   _region_id;
};

#endif // __RAID_OVER_H__
