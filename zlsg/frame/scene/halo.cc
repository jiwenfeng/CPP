#include "config_effect.h"
#include "scene_role.h"
#include "scene_inst.h"
#include "halo.h"

halo::halo() 
    : _master(NULL)
    , _radius(0)
    , _cfg(NULL)
{

}

halo::halo(scene_role* master,  uint32 radius, const config_effect* cfg, uint32 tick)
    : _master(master)
    , _radius(radius)
    , _cfg(cfg)
{
    assert(master != NULL);
    assert(cfg != NULL);
    _start_tick = tick;
    _end_tick   = tick + cfg->get_duration_tick() / 10;

    config_effect_mgr* pcem = MUTABLE_CONFIG_MGR(config_effect_mgr);
    if (pcem == NULL) {
        return ;
    }

    _tirgger = pcem->get_config_effect(_cfg->get_next_trigger_effect_id());
}

halo::~halo()
{
    release();
}

void halo::init(scene_role* master, uint32 radius, const config_effect* cfg, uint32 tick)
{
    assert(master != NULL);
    assert(cfg != NULL);
    _master     = master;
    _radius     = radius;
    _cfg        = cfg   ;
    _start_tick = tick;
    _end_tick   = tick + cfg->get_duration_tick() / 10;

    config_effect_mgr* pcem = MUTABLE_CONFIG_MGR(config_effect_mgr);
    if (pcem == NULL) {
        return ;
    }

    _tirgger = pcem->get_config_effect(_cfg->get_next_trigger_effect_id());
}

void halo::move_update(uint32 tick)
{
    scene_inst* psi = _master->get_scene();
    if (_master->get_scene() == NULL || _tirgger == NULL) {
        return ;
    }

    std::vector<scene_role*> psrs;
    for (size_t index = 0; index < _roles.size(); ++index) {
        scene_role* psr = psi->get_scene_role(_roles[index]);
        if (psr == NULL) {
            continue;
        }

        if (psr->get_scene_pos().get_distance(_master->get_scene_pos()) > _radius) {
            psrs.clear();
            psi->get_screen_role(psr->get_scene_pos(), psrs);
            for (size_t index = 0; index < psrs.size(); ++index) {
                psrs[index]->role_in(psr, tick);
            }
        }
    }

    _roles.clear();

    if (get_start_tick() > tick || get_end_tick() < tick) {
        return ;
    }

    psrs.clear();
    psi->get_screen_role(_master->get_scene_pos(), psrs);
    client::role_data rd;
    _master->fill_role_data(rd);
    for (size_t index = 0; index < psrs.size(); ++index) {
        scene_role* psr = psrs[index];
        if (psr->get_scene_pos().get_distance(_master->get_scene_pos()) > _radius || psr->get_scene() == NULL) {
            psr->mutable_buffer()->del_buf(_tirgger);
            continue;
        }

        add_buff(psr, &rd);
    }
}

void halo::add_buff(scene_role* psr, client::role_data* rd)
{
    if (_tirgger == NULL) {
        return ;
    }

    bool add = false;
    const std::vector<int>& veot = _tirgger->get_effect_object_type();
    for (size_t target_index = 0; target_index < veot.size(); ++target_index) {
		switch(veot[target_index]) {
			case EOT_ENEMY : add = !psr->get_scene()->is_alliance(_master, psr); break;
			case EOT_FRIEND : add = psr->get_scene()->is_alliance(_master, psr); break;
			case EOT_OWN : add = (psr == _master); break;
			default: break;
		}
    }

    if (add) {
        psr->mutable_buffer()->add_buf(_tirgger, rd);
        _roles.push_back(psr->get_inst_id());
    }
}

//void halo::update()
//{
//    scene_inst* psi = _master->get_scene();
//    if (_master->get_scene() == NULL) {
//        return ;
//    }
//
//    for (size_t index = 0; index < _roles.size(); ++index) {
//        scene_role* psr = psi->get_scene_role(_roles[index]);
//        if (psr == NULL) {
//            continue;
//        }
//
//        psr->mutable_buffer()->del_buf(_tirgger);
//    }
//
//    if (_roles.empty()) {
//        return ;
//    }
//
//    client::role_data rd;
//    _master->fill_role_data(rd);
//    for (size_t index = 0; index < _roles.size(); ++index) {
//        scene_role* psr = psi->get_scene_role(_roles[index]);
//        if (psr == NULL) {
//            continue;
//        }
//
//        psr->mutable_buffer()->add_buf(_tirgger, &rd);
//    }
//}

bool halo::find_role(uint64 id)
{
    return std::find(_roles.begin(), _roles.end(), id) != _roles.end();
}

void halo::add_role(scene_role* psr)
{
    if (psr == NULL || psr->get_scene_pos().get_distance(_master->get_scene_pos()) > _radius) {
        return ;
    }

    _roles.push_back(psr->get_inst_id());

    client::role_data rd;
    _master->fill_role_data(rd);
    add_buff(psr, &rd);
}

void halo::remove_role(scene_role* psr)
{
	if(NULL == _tirgger)
	{
		return ;
	}
    psr->mutable_buffer()->del_buf(_tirgger);
}

bool halo::check_distance(scene_role* psr)
{
    if (psr == NULL || psr->get_scene_pos().get_distance(_master->get_scene_pos()) <= _radius) {
        client::role_data rd;
        _master->fill_role_data(rd);
        add_buff(psr, &rd);

        return true;
    }

    remove_role(psr);

    return false;
}

void halo::release()
{
    if (_master == NULL) {
        return ;
    }

    scene_inst* psi = _master->get_scene();
    if (_master->get_scene() == NULL || _tirgger == NULL) {
        return ;
    }

    for (size_t index = 0; index < _roles.size(); ++index) {
        scene_role* psr = psi->get_scene_role(_roles[index]);
        if (psr == NULL) {
            continue;
        }

        psr->mutable_buffer()->del_buf(_tirgger);
    }
}

halo_mgr::halo_mgr()
{

}

halo_mgr::~halo_mgr()
{
    halos.clear();
}

void halo_mgr::insert(scene_role* master, uint32 radius, const config_effect* cfg, uint32 tick)
{
    assert(master != NULL);
    assert(cfg != NULL);
    halos[cfg->get_effect_id()].init(master, radius, cfg, tick);
}

void halo_mgr::move_update(uint32 tick)
{
    std::map<uint32, halo>::iterator begin = halos.begin();
    while (begin != halos.end()) {
        begin->second.move_update(tick);
        if (begin->second.get_end_tick() < tick && begin->second.get_start_tick() < tick) {
            halos.erase(begin++);
        } else {
            ++begin;
        }
    }
}

void halo_mgr::role_in(scene_role* psr, uint32 tick)
{
    std::map<uint32, halo>::iterator begin = halos.begin();
    while (begin != halos.end()) {
        if (!begin->second.check_distance(psr)) {
            ++begin;
            continue;
        }

        //if (begin->second.find_role(psr->get_inst_id())) {
        //    ++begin;
        //    continue;
        //}

        if (begin->second.get_end_tick() < tick && begin->second.get_start_tick() < tick) {
            begin->second.move_update(tick);
            halos.erase(begin++);
        } else {
            begin->second.add_role(psr);
            ++begin;
        }
    }
}

void halo_mgr::role_out(scene_role* psr, uint32 tick)
{
    std::map<uint32, halo>::iterator begin = halos.begin();
    std::map<uint32, halo>::iterator end   = halos.end  ();
    while (begin != end) {
        begin->second.remove_role(psr);
        //if (begin->second.find_role(psr->get_inst_id())) {
        //    begin->second.check_distance(psr);
        //}

        ++begin;
    }
}

bool halo_mgr::has_halo()
{
    return !halos.empty();
}

void halo_mgr::remove_all()
{
    halos.clear();
}

void halo_mgr::erase(const config_effect *cfg)
{
    if (cfg == NULL) {
        return;
    }

    std::map<uint32, halo>::iterator it = halos.find(cfg->get_effect_id());
    if(it == halos.end()) {
        return;
    }

    halos.erase(it);
}
