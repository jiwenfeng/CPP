#include "client.region.pb.h"
#include "scene_define.h"
#include "raid_scene_inst.h"
#include "region_over.h"

int genterate_region_over_rule::init(std::string& rules)
{
    if (rules.empty()) {
        return -1;
    }

    _id = 0;
    _rules.Clear();

    string temp, id;
    size_t i = 0;
    for (i = 0; i < rules.length(); ++i) {
        if (rules[i] >= '0' && rules[i] <= '9') {
            id += rules[i];
        } else if (i == 0) {
            return -1;
        } else {
            ++i;
            break;
        }
    }

    _id = atoi(id.c_str());
    rules = rules.substr(i, rules.length() - i);

    for (i = 0; i < rules.length(); ++i) {
         if (rules[i] == ';') {
            ++i;
            break;
        } else {
            temp += rules[i];
        }
    }

    rules = rules.substr(i, rules.length() - i);
    genterate_raid_over_rule::init(temp, "");

    return 0;
}

region_over::region_over(scene_inst* psi)
    : _psi(psi)
    , _cur_id(0)
    , _cur_regions_over(NULL)
{
    const std::map<uint32, server::raid_over_rules>& ros = psi->get_scene().get_region_rules();
    for (std::map<uint32, server::raid_over_rules>::const_iterator it = ros.begin(); it != ros.end(); ++it) {
        raid_over* p = new raid_over(psi, it->second);
        p->set_region_id(it->first);
        _regions.insert(std::pair<uint32, raid_over*>(it->first, p));
    }
}

region_over::~region_over()
{
    for (std::map<uint32, raid_over*>::iterator i = _regions.begin(); i != _regions.end(); ++i) {
        delete i->second;
    }
    _regions.clear();
}

void region_over::activation_region(uint32 region)
{
    if (_completes[region]) {
        return ;
    }

    _cur_id = region;
    std::map<uint32, raid_over*>::iterator i = _regions.find(_cur_id);
    if (i == _regions.end()) {
        _completes[_cur_id] = true;
        client::region_complete_msg msg;
        client::region_info* ri = msg.add_infos();
        ri->set_id(_cur_id);
        _psi->scene_broadcast(msg);
        _cur_regions_over = NULL;
        return ;
    }

    _cur_regions_over = i->second;
}

int region_over::on_update(uint32 tick)
{
    if (_cur_regions_over != NULL) {
        int ret = _cur_regions_over->on_update(tick);
        if (ret > 0) {
            _completes[_cur_id] = true;
            client::region_complete_msg msg;
            client::region_info* ri = msg.add_infos();
            ri->set_id(_cur_id);
            _psi->scene_broadcast(msg);
            _cur_regions_over = NULL;
        }
    }

    return 0;
}

void region_over::mark_dead_role(scene_role* psr)
{
    if (_cur_regions_over != NULL) {
        _cur_regions_over->mark_dead_role(psr);
    }
}

void region_over::mark_items(client::props_info* ppi)
{
    if (_cur_regions_over != NULL) {
        _cur_regions_over->mark_items(ppi);
    }
}

void region_over::mark_touch(scene_role* psr)
{
    if (_cur_regions_over != NULL) {
        _cur_regions_over->mark_touch(psr);
    }
}

void region_over::mark_role_attribute(scene_role* psr)
{
    if (_cur_regions_over != NULL) {
        _cur_regions_over->mark_role_attribute(psr);
    }
}

void region_over::mark_role_postion(scene_role* psr, const map_postion& from, const map_postion& to)
{
    if (_cur_regions_over != NULL) {
        _cur_regions_over->mark_role_postion(psr, from, to);
    }
}
