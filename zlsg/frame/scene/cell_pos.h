#ifndef __CELL_POS_H__
#define __CELL_POS_H__

#include "scene_define.h"

struct cell_pos {
    cell_pos(uint32 x = 0, uint32 y = 0) : _x(x), _y(y) {
    }

    cell_pos(const cell_pos& cp) : _x(cp._x), _y(cp._y) {
    }

    const cell_pos& operator=(const cell_pos& pos) {
        this->_x = pos._x;
        this->_y = pos._y;
        return *this;
    }

    bool operator==(const cell_pos& pos) {
        return this->_x == pos._x && this->_y == pos._y;
    }

    bool operator!=(const cell_pos& pos) {
        return this->_x != pos._x || this->_y != pos._y;
    }


    bool like(const cell_pos& pos) {
        return pos._x - LIKE_LENGTH <= this->_x && this->_x <= pos._x + LIKE_LENGTH && pos._y - LIKE_LENGTH <= this->_y && this->_y <= pos._y + LIKE_LENGTH;
    }

    bool around(const cell_pos& pos, uint32 length = 1) const {
        for (uint32 x = pos._x - length; x <= pos._x + length; ++x) {
            for (uint32 y = pos._y - length; y <= pos._y + length; ++y) {
                if (this->_x == x && this->_y == y) {
                    return true;
                }
            }
        }

        return false;
    }

    const cell_pos& set(uint32 x = 0, uint32 y = 0) {
        _x = x;
        _y = y;
        return *this;
    }

    uint32 get_distance(const cell_pos& cp) const {
        // floor or ceil
        return (uint32)sqrt((double)(((int32)this->_x - (int32)cp._x) * ((int32)this->_x - (int32)cp._x) + ((int32)this->_y - (int32)cp._y) * ((int32)this->_y - (int32)cp._y)));
    }

    cell_pos step(const cell_pos& s, const cell_pos& e) {
        return cell_pos(s._x + step(s._x, e._x), s._y + step(s._y, e._y));
    }

    int step(uint32 a, uint32 b) {
        if (a > b) {
            return -1;
        } else if ( a == b) {
            return 0;
        } else {
            return 1;
        }
    }

    uint32 _x;
    uint32 _y;
    static const uint32 LIKE_LENGTH = 1;
};


#endif // __CELL_POS_H__