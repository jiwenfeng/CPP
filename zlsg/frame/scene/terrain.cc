#include <cmath>
#include "config_effect.h"
#include "terrain.h"

terrain_mgr::terrain_mgr()
        : _index(0)
        , _size(0)
{
    _stop_add = false;
}

terrain_mgr::~terrain_mgr()
{
}

void terrain_mgr::add_terrain(const client::terrain_info& ti)
{
    if (_stop_add) {
        return ;
    }

    ++_size;
    _tis.push_back(ti);
    change_terrain(client::TLME_ADD, ti);
}

void terrain_mgr::add_terrain(const config_skill* cs, const config_effect* ce, const scene_role* def, const scene_role* atk, client::terrain_info& ti)
{
    if (cs == NULL || ce == NULL || atk == NULL) {
        return ;
    }

    if (def == NULL) {
        def = atk;
    }

    int32 length = 48 * (cs->get_round_radius() * 2 + 1);
    client::point p;
    p.set_x(max(0, def->get_scene_pos().x - length / 2));
    p.set_y(max(0, def->get_scene_pos().y - length / 2));
    ti.set_terrain_id(get_index());
    ti.set_effect_id(ce->get_effect_id());
    ti.mutable_rect()->mutable_a()->set_x(p.x());
    ti.mutable_rect()->mutable_a()->set_y(p.y());
    ti.mutable_rect()->mutable_b()->set_x(p.x() + length);
    ti.mutable_rect()->mutable_b()->set_y(p.y());
    ti.mutable_rect()->mutable_c()->set_x(p.x());
    ti.mutable_rect()->mutable_c()->set_y(p.y() + length);
    ti.mutable_rect()->mutable_d()->set_x(p.x() + length);
    ti.mutable_rect()->mutable_d()->set_y(p.y() + length);
    ti.set_start_time(time_utils::tenms());
    ti.set_end_time(ti.start_time() + ce->get_duration_tick() / 10);
    atk->fill_role_data(ti.mutable_rd());
    const config_skill* pcs = ce->get_next_trigger_skill();
    if (pcs != NULL) {
        const std::vector<int>& vml = pcs->get_method_list();
        for (size_t j = 0; j < vml.size(); ++j) {
            ti.add_trigger_effects(vml[j]);
        }
    }
    add_terrain(ti);
}

void terrain_mgr::add_terrain(const config_skill* cs, const config_effect* ce, uint32 x, uint32 y, client::terrain_info& ti)
{
    if (cs == NULL || ce == NULL) {
        return ;
    }

    int32 length = 48 * (cs->get_round_radius() * 2 + 1);
    client::point p;
    p.set_x(max(0, (int)(x - length / 2)));
    p.set_y(max(0, (int)(y - length / 2)));
//     p.set_x(x);
//     p.set_y(y);
    ti.set_terrain_id(get_index());
    ti.set_effect_id(ce->get_effect_id());
    ti.mutable_rect()->mutable_a()->set_x(p.x());
    ti.mutable_rect()->mutable_a()->set_y(p.y());
    ti.mutable_rect()->mutable_b()->set_x(p.x() + length);
    ti.mutable_rect()->mutable_b()->set_y(p.y());
    ti.mutable_rect()->mutable_c()->set_x(p.x());
    ti.mutable_rect()->mutable_c()->set_y(p.y() + length);
    ti.mutable_rect()->mutable_d()->set_x(p.x() + length);
    ti.mutable_rect()->mutable_d()->set_y(p.y() + length);
    ti.set_start_time(time_utils::tenms());
    ti.set_end_time(ti.start_time() + ce->get_duration_tick() / 10);
    ti.mutable_rd()->set_role_id(0);
    ti.mutable_rd()->set_inst_id(0);
    ti.mutable_rd()->set_role_typ(client::role_data_TYPE_NPC);
    const config_skill* pcs = ce->get_next_trigger_skill();
    if (pcs != NULL) {
        const std::vector<int>& vml = pcs->get_method_list();
        for (size_t j = 0; j < vml.size(); ++j) {
            ti.add_trigger_effects(vml[j]);
        }
    }
    add_terrain(ti);
}

void terrain_mgr::get_terrain_by_pos(const map_postion& pos, std::vector<client::terrain_info>& tis)
{
    std::list<client::terrain_info>::iterator i = _tis.begin();
    while (i != _tis.end()) {
        if (is_in(*i, pos)) {
            tis.push_back(*i);
        }
        
        ++i;
    }
}

bool terrain_mgr::check_terrain_timeout(uint32 tick)
{
    bool r = false;
    std::list<client::terrain_info>::iterator i = _tis.begin();
    while (i != _tis.end()) {
        if (tick > i->end_time()) {
            --_size;
            change_terrain(client::TLME_DELETE, *i);
            _tis.erase(i++);
            r = true;
            continue;
        }

        ++i;
    }

    return r;
}
        
bool terrain_mgr::check_terrain(scene_role* sr)
{
    std::list<client::terrain_info>::iterator i = _tis.begin();
    while (i != _tis.end()) {
        if (is_in(*i, sr->get_scene_pos())) {
            sr->add_effect(i->effect_id());
        }
        ++i;
    }

    return true;
}

void terrain_mgr::fill_terrain_list(client::terrain_list& tl)
{
    std::list<client::terrain_info>::iterator i = _tis.begin();
    for ( ; i != _tis.end(); ++i) {
        client::terrain_operate_info* toi = tl.add_toi();
        toi->set_method(client::TLME_ADD);
        toi->mutable_ti()->CopyFrom(*i);
    }
}

void terrain_mgr::fill_change_list(client::terrain_list& tl)
{
    tl.CopyFrom(_change);
    _change.Clear();
}

void terrain_mgr::get_center_point(const client::terrain_info& ti, postion& pos)
{
    pos.x = (ti.rect().a().x() + ti.rect().d().x()) / 2;
    pos.y = (ti.rect().a().y() + ti.rect().d().y()) / 2;
}

bool terrain_mgr::get_center_point(uint32 terrain_id, map_postion& mp)
{
    std::list<client::terrain_info>::iterator i = _tis.begin();
    while (i != _tis.end()) {
        if (i->terrain_id() == terrain_id) {
            mp.x = 0;//(i->x() + i->width()) / 2;
            mp.y = 0;//(i->y() + i->height()) / 2;
            return true;
        }

        ++i;
    }

    return false;
}

void terrain_mgr::change_terrain(client::TERRAIN_LIST_METHOD_E type, const client::terrain_info& ti)
{
    client::terrain_operate_info* toi = _change.add_toi();
    toi->set_method(type);
    toi->mutable_ti()->CopyFrom(ti);
}

double terrain_mgr::multiply(const map_postion& p0, const client::point& p1, const client::point& p2)
{
    return (((int)p0.x - (int)p2.x()) * ((int)p1.y() - (int)p2.y()) - ((int)p1.x() - (int)p2.x()) * ((int)p0.y - (int)p2.y()));
}

bool terrain_mgr::is_in(const client::terrain_info& ti, const map_postion& mp)
{
    return multiply(mp, ti.rect().a(), ti.rect().b()) * multiply(mp, ti.rect().c(), ti.rect().d()) <= 0 && multiply(mp, ti.rect().c(), ti.rect().a()) * multiply(mp, ti.rect().d(), ti.rect().b()) <= 0;
}

bool terrain_mgr::remove_terrain_list(client::terrain_list& remove)
{
    std::list<client::terrain_info>::iterator i = _tis.begin();
    while (i != _tis.end()) {
        client::terrain_operate_info* toi = remove.add_toi();
        toi->set_method(client::TLME_DELETE);
        toi->mutable_ti()->CopyFrom(*i);
        ++i;
    }

    return remove.toi_size() > 0;
}

void terrain_mgr::stop_add()
{
    _stop_add = true;
}
