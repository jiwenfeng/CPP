#ifndef __CELL_H__
#define __CELL_H__

#include "cell_pos.h"

namespace client {
    class terrain_info;
}

class scene_role;

class cell {
public:
    cell();
    ~cell() {}

public:
    inline cell_pos get_pos() const { return _pos; }
    inline void set_pos(const cell_pos& val) { _pos = val; }

    inline uint32 get_region() const { return _region; }
    inline void set_region(uint32 val) { _region = val; }

    inline bool get_block() const { return _block; }
    inline void set_block(bool val) { _block = val; }

public:
    inline const std::map<uint64, scene_role*>& get_roles() const { return _roles; }
    inline const std::map<uint32, client::terrain_info*>& get_terrains() const { return _terrains; }

    void insert_role(scene_role* psr);
    void insert_terrain(client::terrain_info* ti);

    void erase_role(scene_role* psr);
    void erase_terrain(client::terrain_info* ti);

private:
    cell_pos                                _pos;
    uint32                                  _region;
    bool                                    _block;
    std::map<uint64, scene_role*>           _roles;
    std::map<uint32, client::terrain_info*> _terrains;
};

class cell_mgr {
public:
    cell_mgr(const config_map* pcm = NULL);
    ~cell_mgr();

public:
    void init(const config_map* pcm);

public:
    bool enter_cell(scene_role* psr, const cell_pos& pos);
    bool leave_cell(scene_role* psr);

    uint64 get_role_in_cell(const cell_pos& pos);
    bool   get_cell_by_role(uint64 id, cell_pos& pos);
    bool   get_around_role(const cell_pos& pos, uint32 length = 1);
    bool   get_around_empty(const cell_pos& pos, uint32 length, std::vector<cell_pos>& vtr);
    bool   get_around_empty(const cell_pos& pos, std::vector<cell_pos>& vtr, int length = 1);
    bool   get_not_block(const cell_pos& pos, std::vector<cell_pos>& vtr, uint32 count, int length = 1);
	bool   get_not_block(const cell_pos &pos, std::vector<cell_pos> &cp, uint32 count, std::vector<cell_pos> &except, int length = 1);

    bool   is_block(const cell_pos& pos);
    bool   is_empty(const cell_pos& pos);
    bool   get_empty(cell_pos& pos);
	bool get_empty(const cell_pos &pos, cell_pos &cp, int length = 1);
    uint32 get_region(const cell_pos& pos);

    cell_pos step(const cell_pos& s, const cell_pos& e);
    cell_pos adjacent(const cell_pos& s, const cell_pos& e);
    cell_pos path(const cell_pos& s, const cell_pos& e, int& d);
    bool     path_step(const cell_pos& s, const cell_pos& e, std::list<cell_pos>& path);

public:
    void point(const cell_pos& pos, std::vector<uint64>& vr, std::vector<cell_pos>& vc);
    void around(const cell_pos& pos, ROLE_FACING facing, uint32 length, uint32 role_count_limit, std::vector<uint64>& vr, std::vector<cell_pos>& vc);
    void line(const cell_pos& pos, ROLE_FACING facing, uint32 length, uint32 width, uint32 role_count_limit, std::vector<uint64>& vr, std::vector<cell_pos>& vc);
    void triangle(const cell_pos& pos, ROLE_FACING facing, uint32 length, uint32 role_count_limit, std::vector<uint64>& vr, std::vector<cell_pos>& vc);
    void trigeminal(const cell_pos& pos, ROLE_FACING facing, uint32 length, uint32 role_count_limit, std::vector<uint64>& vr, std::vector<cell_pos>& vc);

public:
    inline uint32 get_width() const { return _width; }
    inline uint32 get_height() const { return _height; }

public:
    cell* get_cell(uint32 w, uint32 h);
    cell* get_cell(const cell_pos& cp);
	void get_all_empty_cell(vector<cell_pos> &cp);
private:
    uint64 get_id(uint32 w, uint32 h);
    void   set_id(uint32 w, uint32 h, uint64 id);
    int    step(uint32 a, uint32 b);
    int    check_d(const cell_pos& s, const cell_pos& e, int& d);

public: //test
    void show_all();

public:
    static const uint64 CELL_BLOCK = -1;
    static const uint64 CELL_EMPTY = 0;

private:
    uint32                     _width;
    uint32                     _height;
    cell*                      _table;
    std::map<uint64, cell_pos> _id;
};

#endif // __CELL_H__
