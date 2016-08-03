#ifndef __REGION_OVER_H__
#define __REGION_OVER_H__

#include "raid_over.h"

class genterate_region_over_rule : public genterate_raid_over_rule
{
public:
    genterate_region_over_rule() : genterate_raid_over_rule(&_rules) {

    }

    ~genterate_region_over_rule() {

    }

public:
    int init(std::string& rules);

public:
    inline uint32 get_id() const { return _id; }
    inline const server::raid_over_rules& get_rules() const { return _rules; }

private:
    uint32                  _id;
    server::raid_over_rules _rules;
};

class region_over
{
public:
    region_over(scene_inst* psi);
    ~region_over();

public:
    void activation_region(uint32 region);

public:
    int on_update(uint32 tick);

public:
    void mark_dead_role(scene_role* psr);
    void mark_items(client::props_info* ppi);
    void mark_touch(scene_role* psr);
    void mark_role_attribute(scene_role* psr);
    void mark_role_postion(scene_role* psr, const map_postion& from, const map_postion& to);

private:
    scene_inst*                    _psi;
    uint32                         _cur_id;
    raid_over*                     _cur_regions_over;
    std::map<uint32, raid_over*>   _regions;
    std::map<uint32, bool>         _completes;
};

#endif // __REGION_OVER_H__