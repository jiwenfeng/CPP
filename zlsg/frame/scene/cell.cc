#include "config_map.h"
#include "client.terrain.pb.h"
#include "scene_role.h"
#include "cell.h"

cell::cell()
{
    _region = 0;
    _block  = false;
}

void cell::insert_role(scene_role* psr)
{
    _roles.insert(std::pair<uint64, scene_role*>(psr->get_inst_id(), psr));
}

void cell::insert_terrain(client::terrain_info* ti)
{
    _terrains.insert(std::pair<uint32, client::terrain_info*>(ti->terrain_id(), ti));
}

void cell::erase_role(scene_role* psr)
{
    _roles.erase(psr->get_inst_id());
}

void cell::erase_terrain(client::terrain_info* ti)
{
    _terrains.erase(ti->terrain_id());
}

cell_mgr::cell_mgr(const config_map* pcm)
    : _table(NULL)
{
    init(pcm);
}

cell_mgr::~cell_mgr()
{
    _id.clear();
    delete []_table;
}

void cell_mgr::init(const config_map* pcm)
{
    if (pcm == NULL) {
        return ;
    }

    if (_table != NULL) {
        delete []_table;
        _table = NULL;
    }

    _width  = pcm->unitGridWidthNum;
    _height = pcm->unitGridHeightNum;
    _table  = new cell[_width * _height];
//     memset(_table, 0, (_width * _height) * sizeof(uint64));
    for (uint32 i = 0; i < _width; ++i) {
        for (uint32 j = 0; j < _height; ++j) {
            cell* p = get_cell(i, j);
            if (p == NULL) {
                continue;
            }
			cell_pos cp(i, j);
			p->set_pos(cp);
            if (pcm->unit_mask(i, j, MAP_UNIT_BLOCK)) {
                p->set_block(true);
            }
            p->set_region(pcm->get_region(i, j));
        }
    }
}

bool cell_mgr::enter_cell(scene_role* psr, const cell_pos& pos)
{
    if (psr == NULL) {
        return false;
    }

    uint64 id = psr->get_inst_id();
    leave_cell(psr);

    cell* pc = get_cell(pos._x, pos._y);
    if (pc == NULL || pc->get_block()) {
        return false;
    }

    if (!pc->get_roles().empty()) {
        return false;
    }

    pc->insert_role(psr);
    _id.insert(std::pair<uint64, cell_pos>(id, pos));

    return true;
}

bool cell_mgr::leave_cell(scene_role* psr)
{
    if (psr == NULL) {
        return false;
    }

    std::map<uint64, cell_pos>::iterator i = _id.find(psr->get_inst_id());
    if (i == _id.end()) {
        return false;
    }

    cell* pc = get_cell(i->second._x, i->second._y);
    if (pc == NULL) {
        _id.erase(i);
        return false;
    }

    _id.erase(i);
    pc->erase_role(psr);

    return true;
}

uint64 cell_mgr::get_role_in_cell(const cell_pos& pos)
{
    cell* pc = get_cell(pos._x, pos._y);
    if (pc == NULL) {
        return false;
    }

    if (pc->get_roles().empty()) {
        return 0;
    }

    return pc->get_roles().begin()->first;
}

bool cell_mgr::get_cell_by_role(uint64 id, cell_pos& pos)
{
    std::map<uint64, cell_pos>::iterator i = _id.find(id);
    if (i == _id.end()) {
        return false;
    }

    pos = i->second;

    return true;
}

bool cell_mgr::get_around_empty(const cell_pos& pos, uint32 length, std::vector<cell_pos>& vtr)
{
    for (uint32 x = pos._x - length; x < pos._x + length; ++x) {
        for (uint32 y = pos._y - length; y < pos._y + length; ++y) {
            if (get_id(x, y) == 0) {
                vtr.push_back(cell_pos(x, y));
                return true;
            }
        }
    }

    return false;
}

bool cell_mgr::get_around_empty(const cell_pos& pos, std::vector<cell_pos>& vtr, int length/* = 1 */)
{
    if ((uint32)length > _width && (uint32)length > _height) {
        return false;
    }

    int src_x = (int)pos._x;
    int src_y = (int)pos._y;
    for (int32 x = src_x - length; x < src_x + length; ++x) {
        for (int32 y = src_y - length; y < src_y + length; ++y) {
            if (x < 0 || y < 0) { // 超短点
                continue;
            }

            if (_width <= (uint32)x || _height <= (uint32)y) { // 超长点
                continue;
            }

            if (abs(src_x - x) < length && abs(src_y - y) < length) { // 已计算过的点
                continue;
            }

            if (get_id(x, y) == 0) {
                vtr.push_back(cell_pos(x, y));
            }
        }
    }

    if (vtr.empty()) {
        return get_around_empty(pos, vtr, length + 1);
    }

    return true;
}

bool cell_mgr::get_not_block(const cell_pos &pos, std::vector<cell_pos> &cp, uint32 count, std::vector<cell_pos> &except, int length /*= 1 */)
{
    if ((uint32)length > _width && (uint32)length > _height) {
        return false;
    }
    int src_x = (int)pos._x;
    int src_y = (int)pos._y;
    for (int32 x = src_x - length; x <= src_x + length; ++x) {
        for (int32 y = src_y - length; y <= src_y + length; ++y) {
            if (x < 0 || y < 0) { // 超短点
                continue;
            }
            if (_width <= (uint32)x || _height <= (uint32)y) { // 超长点
                continue;
            }

            if (abs(src_x - x) < length && abs(src_y - y) < length) { // 已计算过的点
                continue;
            }

            if (get_id(x, y) == CELL_EMPTY) {
				cell_pos p(x, y);
				if(find(except.begin(), except.end(), p) == except.end())
				{
					cp.push_back(p);
					except.push_back(p);
				}
				if(cp.size() >= count)
				{
					return true;
				}
            }
        }
    }
    if (cp.size() < count) {
		return get_not_block(pos, cp, count, except, length + 1);
    }
    return true;
}

bool cell_mgr::get_not_block(const cell_pos& pos, std::vector<cell_pos>& vtr, uint32 count, int length /* = 1 */)
{
    if ((uint32)length > _width && (uint32)length > _height) {
        return false;
    }

    int src_x = (int)pos._x;
    int src_y = (int)pos._y;
    for (int32 x = src_x - length; x < src_x + length; ++x) {
        for (int32 y = src_y - length; y < src_y + length; ++y) {
            if (x < 0 || y < 0) { // 超短点
                continue;
            }

            if (_width <= (uint32)x || _height <= (uint32)y) { // 超长点
                continue;
            }

            if (abs(src_x - x) < length && abs(src_y - y) < length) { // 已计算过的点
                continue;
            }

            if (get_id(x, y) == CELL_EMPTY) {
                vtr.push_back(cell_pos(x, y));
				if(vtr.size() >= count)
				{
					return true;
				}
            }
        }
    }
    if (vtr.size() < count) {
		return get_not_block(pos, vtr, count, length + 1);
    }
    return true;
}

bool cell_mgr::is_block(const cell_pos& pos)
{
    if (_width <= pos._x || _height <= pos._y) {
        return true;
    }

    return get_id(pos._x, pos._y) == CELL_BLOCK;
}

bool cell_mgr::is_empty(const cell_pos& pos)
{
    if (_width <= pos._x || _height <= pos._y) {
        return false;
    }

    return get_id(pos._x, pos._y) == CELL_EMPTY;
}

bool cell_mgr::get_empty(const cell_pos &pos, cell_pos &cp, int length /* = 1*/)
{
    if ((uint32)length > _width && (uint32)length > _height) {
        return false;
    }
	for(uint32 x = pos._x - length; x < pos._x + length; ++x)
	{
		for(uint32 y = pos._y - length; y < pos._y + length; ++y)
		{
			if(get_id(x, y) == CELL_EMPTY)
			{
				cp.set(x, y);
				return true;
			}
		}
	}
	return get_empty(pos, cp, length + 1);
}

bool cell_mgr::get_empty(cell_pos& pos)
{
    uint32 length = 1;
    for (uint32 x = pos._x - length; x < pos._x + length; ++x) {
        for (uint32 y = pos._y - length; y < pos._y + length; ++y) {
            if (get_id(x, y) == 0) {
                pos.set(x, y);
                return true;
            }
        }
    }

    return false;
}

uint32 cell_mgr::get_region(const cell_pos& pos)
{
    cell* p = get_cell(pos);
    if (p == NULL) {
        return 0;
    }

    return p->get_region();
}

cell_pos cell_mgr::step(const cell_pos& s, const cell_pos& e)
{
    return cell_pos(s._x + step(s._x, e._x), s._y + step(s._y, e._y));
}

cell_pos cell_mgr::adjacent(const cell_pos& s, const cell_pos& e)
{
    cell_pos pos;
    if (s._x == e._x) {
        pos.set(s._x + 1, e._y);
        if (is_empty(pos)) {
            return pos;
        } else {
            pos.set(s._x - 1, e._y);
        }
    } else if (s._y == e._y) {
        pos.set(e._x, e._y + 1);
        if (is_empty(pos)) {
            return pos;
        } else {
            pos.set(e._x, e._y - 1);
        }
    } else {
        pos.set(e._x, s._y);
        if (is_empty(pos)) {
            return pos;
        } else {
            pos.set(s._x, e._y);
        }
    }

    if (is_empty(pos)) {
        return pos;
    } else {
        /*if (s._x == e._x) {
            if (s._x < (_width / 2)) {
                pos.set(s._x + 1, s._y);
            } else {
                pos.set(s._x - 1, s._y);
            }
        } else {
            if (s._y < (_height / 2)) {
                pos.set(s._x, s._y + 1);
            } else {
                pos.set(s._x, s._y - 1);
            }
        }
        return pos;*/
    }

    return s;
}
/*
 1 
4 2
 3 
*/

#define CHECK_RETURN(p) if (is_empty(p)) { if (t != 0) { d = 0; } return p; }
cell_pos cell_mgr::path(const cell_pos& s, const cell_pos& e, int& d)
{
    /*if (s.around(e)) {
        return s;
    }*/

    int t = check_d(s, e, d);
    if (d == 0) {
        d = t;
    }

    cell_pos pos;
    if (d == 2) {
        // l
        CHECK_RETURN(pos.set(s._x - 1, s._y    ));
        CHECK_RETURN(pos.set(s._x - 1, s._y - 1));
        CHECK_RETURN(pos.set(s._x - 1, s._y + 1));
        CHECK_RETURN(pos.set(s._x    , s._y - 1));
        CHECK_RETURN(pos.set(s._x    , s._y + 1));
    } else if (d == 4) {
        CHECK_RETURN(pos.set(s._x + 1, s._y    ));
        CHECK_RETURN(pos.set(s._x + 1, s._y + 1));
        CHECK_RETURN(pos.set(s._x + 1, s._y - 1));
        CHECK_RETURN(pos.set(s._x    , s._y + 1));
        CHECK_RETURN(pos.set(s._x    , s._y - 1));
    } else if (d == 1) {
        // u
        CHECK_RETURN(pos.set(s._x    , s._y - 1));
        CHECK_RETURN(pos.set(s._x + 1, s._y - 1));
        CHECK_RETURN(pos.set(s._x - 1, s._y - 1));
        CHECK_RETURN(pos.set(s._x + 1, s._y    ));
        CHECK_RETURN(pos.set(s._x - 1, s._y    ));
    } else if (d == 3) {
        CHECK_RETURN(pos.set(s._x    , s._y + 1));
        CHECK_RETURN(pos.set(s._x - 1, s._y + 1));
        CHECK_RETURN(pos.set(s._x + 1, s._y + 1));
        CHECK_RETURN(pos.set(s._x - 1, s._y    ));
        CHECK_RETURN(pos.set(s._x + 1, s._y    ));
    }

    if (t != 0) { d = 0; }

    return s;
}

bool cell_mgr::path_step(const cell_pos& s, const cell_pos& e, std::list<cell_pos>& path)
{
    cell_pos pos(e);
    if (!get_empty(pos)) {
        return false;
    }
    
    path.push_back(e);
    path.push_back(pos);

    int i = 0;
    while (pos != s && i < 10) {
        cell_pos p;
        if (pos._x > s._x) {
            p.set(pos._x - 1, pos._y);
        } else if (pos._x < s._x) {
            p.set(pos._x + 1, pos._y);
        }

        if (pos._y > s._y) {
            p.set(p._x, pos._y - 1);
        } else if (pos._y < s._y) {
            p.set(p._x, pos._y + 1);
        }

        if (is_empty(p)) {
            path.push_back(p);
            continue;
        }

        ++i;
    }

    if (i == 10) {
        path.clear();
        return false;
    }

    return true;
}

void cell_mgr::around(const cell_pos& pos, ROLE_FACING facing, uint32 length, uint32 role_count_limit, std::vector<uint64>& vr, std::vector<cell_pos>& vc)
{
    uint64 id = 0;
    cell_pos cp;
    for (uint32 x = max(0, (int)(pos._x - length)); x <= pos._x + length; ++x) {
        for (uint32 y = max(0, (int)(pos._y - length)); y <= pos._y + length; ++y) {
            if (x == pos._x && y == pos._y) {
                continue;
            }

            id = get_role_in_cell(cp.set(x, y));
            if (id != 0) {
                vr.push_back(id);
            } else {
                vc.push_back(cp);
                continue ;
            }

            /*if (role_count_limit < vr.size()) {
                vr.erase(vr.end() - (vr.size() - role_count_limit), vr.end());
                return ;
            } else if (role_count_limit == vr.size()) {
                break;
            }*/
        }
    }
}

void cell_mgr::point(const cell_pos& pos, std::vector<uint64>& vr, std::vector<cell_pos>& vc)
{
    uint64 id = 0;
    id = get_role_in_cell(pos);
    if (id != 0) {
        vr.push_back(id);
    } else {
        vc.push_back(pos);
    }
}

void cell_mgr::line(const cell_pos& pos, ROLE_FACING facing, uint32 length, uint32 width, uint32 role_count_limit, std::vector<uint64>& vr, std::vector<cell_pos>& vc)
{
    return around(pos, facing, length, role_count_limit, vr, vc);

    uint32 x  = 0;
    uint32 y  = 0;
    uint64 id = 0;
    cell_pos cp;
    for (uint32 i = 1; i <= length; ++i) {
        switch (facing) {
        case NORTH:     x = pos._x    , y = pos._y - i; break;
        case NORTHEAST: x = pos._x + i, y = pos._y - i; break;
        case EAST:      x = pos._x + i, y = pos._y    ; break;
        case SOUTHEAST: x = pos._x + i, y = pos._y + i; break;
        case SOUTH:     x = pos._x    , y = pos._y + i; break;
        case SOUTHWEST: x = pos._x - i, y = pos._y + i; break;
        case WEST:      x = pos._x - i, y = pos._y    ; break;
        case NORTHWEST: x = pos._x - i, y = pos._y - i; break;
        default: return ;
        }

        id = get_role_in_cell(cp.set(x, y));
        if (id != 0) {
            vr.push_back(id);
        }

        if (role_count_limit <= vr.size()) {
            return ;
        }
    }
}

void cell_mgr::triangle(const cell_pos& pos, ROLE_FACING facing, uint32 length, uint32 role_count_limit, std::vector<uint64>& vr, std::vector<cell_pos>& vc)
{
    around(pos, facing, length, role_count_limit, vr, vc);

    /*line(pos, facing, length, role_count_limit, vr);
    uint32 x   = 0;
    uint32 y   = 0;
    uint32 len = 0;
    ROLE_FACING l, r;
    cell_pos cp;
    for (uint32 i = 1; i <= length; ++i) {
        switch (facing) {
        case NORTH    : x = pos._x    , y = pos._y - i; l = WEST     , r = EAST     ; break;
        case NORTHEAST: x = pos._x + i, y = pos._y - i; l = SOUTHWEST, r = NORTHEAST; break;
        case EAST     : x = pos._x + i, y = pos._y    ; l = NORTH    , r = SOUTH    ; break;
        case SOUTHEAST: x = pos._x + i, y = pos._y + i; l = NORTHEAST, r = SOUTHWEST; break;
        case SOUTH    : x = pos._x    , y = pos._y + i; l = EAST     , r = WEST     ; break;
        case SOUTHWEST: x = pos._x - i, y = pos._y + i; l = NORTHWEST, r = NORTHEAST; break;
        case WEST     : x = pos._x - i, y = pos._y    ; l = SOUTH    , r = NORTH    ; break;
        case NORTHWEST: x = pos._x - i, y = pos._y - i; l = NORTHEAST, r = SOUTHWEST; break;
        case NORTH    : x = pos._x    , y = pos._y + i; l = WEST     , r = EAST     ; break;
        case NORTHEAST: x = pos._x + i, y = pos._y + i; l = SOUTHWEST, r = NORTHEAST; break;
        case EAST     : x = pos._x + i, y = pos._y    ; l = NORTH    , r = SOUTH    ; break;
        case SOUTHEAST: x = pos._x + i, y = pos._y - i; l = NORTHEAST, r = SOUTHWEST; break;
        case SOUTH    : x = pos._x    , y = pos._y - i; l = EAST     , r = WEST     ; break;
        case SOUTHWEST: x = pos._x - i, y = pos._y - i; l = NORTHWEST, r = NORTHEAST; break;
        case WEST     : x = pos._x - i, y = pos._y    ; l = SOUTH    , r = NORTH    ; break;
        case NORTHWEST: x = pos._x - i, y = pos._y + i; l = NORTHEAST, r = SOUTHWEST; break;
        default: return ;
        }

        len = i + 1;
        cp.set(x, y);
        line(cp, l, len, role_count_limit, vr);
        line(cp, r, len, role_count_limit, vr);

        if (role_count_limit < vr.size()) {
            vr.erase(vr.end() - (vr.size() - role_count_limit), vr.end());
            return ;
        }
    }*/
}

void cell_mgr::trigeminal(const cell_pos& pos, ROLE_FACING facing, uint32 length, uint32 role_count_limit, std::vector<uint64>& vr, std::vector<cell_pos>& vc)
{
    around(pos, facing, length, role_count_limit, vr, vc);
}

cell* cell_mgr::get_cell(uint32 w, uint32 h)
{
    if (_width <= w || _height <= h) {
        return NULL;
    }

    return _table + _width * h + w;
}

cell* cell_mgr::get_cell(const cell_pos& cp)
{
    return get_cell(cp._x, cp._y);
}

uint64 cell_mgr::get_id(uint32 w, uint32 h)
{
    cell* pc = get_cell(w, h);
    if (pc == NULL || pc->get_block()) {
        return -1;
    }

    if (pc->get_roles().empty()) {
        return 0;
    }

    return pc->get_roles().begin()->first;
}

void cell_mgr::set_id(uint32 w, uint32 h, uint64 id)
{
    if (_width <= w || _height <= h) {
        return ;
    }

    //*(_table + _width * h + w).;
}

int cell_mgr::step(uint32 a, uint32 b) 
{
    if (a > b) {
        return -1;
    } else if ( a == b) {
        return 0;
    } else {
        return 1;
    }
}

int cell_mgr::check_d(const cell_pos& s, const cell_pos& e, int& d)
{
    if (s._x == e._x) {
        if (s._y > e._y) {
            d = 1;
        } else {
            d = 3;
        }
    } else if (s._y == e._y) {
        if (s._x > e._x) {
            d = 2;
        } else {
            d = 4;
        }
    }

    if (d != 0) {
        return 0;
    }

    if (abs(int(s._x - e._x)) > abs(int(s._y - e._y))) {
        if (s._y > e._y) {
            return 1;
        } else {
            return 3;
        }
    } else {
        if (s._x > e._x) {
            return 2;
        } else {
            return 4;
        }
    }

    return 0;
}

void cell_mgr::show_all()
{
    std::map<uint64, cell_pos>::iterator i = _id.begin();
    for (; i != _id.end(); ++i) {
        printf("id = %ld, x = %d, y = %d\n", i->first, i->second._x, i->second._y);
    }
}

void cell_mgr::get_all_empty_cell(vector<cell_pos> &cp)
{
	for(uint32 i = 0; i < _width * _height; ++i)
	{
		cell_pos _cp = _table[i].get_pos();
		if(!is_block(_cp))
		{
			cp.push_back(_cp);
		}
	}
}
