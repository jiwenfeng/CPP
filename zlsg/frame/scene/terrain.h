#ifndef __TERRAIN_H__
#define __TERRAIN_H__

#include "scene_define.h"
#include "client.terrain.pb.h"
#include "scene_role.h"

class terrain_mgr {
public:
    terrain_mgr();
    ~terrain_mgr();
    
public:
    bool has_terrain() { return !_tis.empty(); }
    uint32 get_index() { return _index++; }
    uint32 get_size() { return _size; }
    void   add_terrain(const client::terrain_info& ti);
    void   add_terrain(const config_skill* cs, const config_effect* ce, const scene_role* def, const scene_role* atk, client::terrain_info& ti);
    void   add_terrain(const config_skill* cs, const config_effect* ce, uint32 x, uint32 y, client::terrain_info& ti);
    bool   is_change() { return _change.toi_size() != 0; }

    void get_terrain_by_pos(const map_postion& pos, std::vector<client::terrain_info>& tis);

public:
    bool check_terrain_timeout(uint32 tick);
    bool check_terrain(scene_role* sr);
    void fill_terrain_list(client::terrain_list& tl);
    void fill_change_list(client::terrain_list& tl);
    bool remove_terrain_list(client::terrain_list& remove);
    void stop_add();

    void get_center_point(const client::terrain_info& ti, postion& pos);
    bool get_center_point(uint32 terrain_id, map_postion& mp);

private:
    void change_terrain(client::TERRAIN_LIST_METHOD_E type, const client::terrain_info& ti);

    double multiply(const map_postion& p0, const client::point& p1, const client::point& p2);
    bool is_in(const client::terrain_info& ti, const map_postion& mp);
    
private:
    uint32                          _index;
    uint32                          _size;
    std::list<client::terrain_info> _tis;
    client::terrain_list            _change;
    bool                            _stop_add;
};

#endif // __TERRAIN_H__
