#include <stdlib.h>
#include <math.h>
#include "random.h"
#include "postion.h"

postion::postion()
        : x(0)
        , y(0)
{
}

postion::postion(const postion& rhs)
{
    if (this == &rhs) {
        return;
    }

    *this = rhs;
}

postion::postion(int a, int b)
    : x(a)
    , y(b)
{

}

postion::~postion()
{
}

bool postion::operator==(const postion& rhs) const
{
    return !operator!=(rhs);
}

bool postion::operator!=(const postion& rhs) const
{
    if(this == &rhs) {
        return false;
    }

    return x != rhs.x || y != rhs.y;
}

bool postion::operator<(const postion& rhs) const
{
    if (this == &rhs) {
        return false;
    }

    if (x < rhs.x) {
        return true;
    } else if (rhs.x < x) {
        return false;
    }

    if (y < rhs.y) {
        return true;
    } else if (rhs.y < y) {
        return false;
    }

    return false;
}

postion& postion::operator=(const postion& rhs)
{
    if (this == &rhs) {
        return *this;
    }

    x = rhs.x;
    y = rhs.y;

    return *this;
}

uint32 postion::get_distance(const postion& rhs) const
{
    return (uint32)sqrt((double)((x - rhs.x) * (x - rhs.x) + (y - rhs.y) * (y - rhs.y)));
}

double postion::get_fdistance(const postion& rhs) const
{
    return sqrt((double)((x - rhs.x) * (x - rhs.x) + (y - rhs.y) * (y - rhs.y)));
}

void postion::add(int _x, int _y)
{
    x += _x;
    if (x <= 0) {
        x = 0;
    }

    y += _y;
    if (y <= 0) {
        y = 0;
    }
}

bool postion::line_rect_intersection(const postion& end_p, const postion& rect_top, const postion& rect_bottom) const
{

    int a = this->y - end_p.y;
    int b = end_p.x -this->x;
    int c = this->x * end_p.y - end_p.x * this->y;

    if (a == 0 && b == 0 && c == 0) {
        if (this->x >= rect_top.x  && this->x <= rect_bottom.x && this->y >= rect_top.y && this->y <= rect_bottom.y) {
            return true;
        } else {
            return false;
        }
    }

    int left   = rect_top.x;
    int top    = rect_top.y;
    int right  = rect_bottom.x;
    int bottom = rect_bottom.y;

    // 思路:先看线段所在直线是否与矩形相交，如果不相交则必为false
    // 如果相交，则看线段的两个点是否在矩形的同一边（即两点的 x（y)  坐标都比矩形的小 x(y) 坐标小，或者大）,
    // 若在同一边则为false，否则就是相交的情况。
    if ((a * left + b * top + c >= 0 && a * right + b * bottom +c <= 0) || (a * left + b * top + c <= 0 && a * right + b * bottom +c >= 0) || (a * left + b * bottom + c >= 0 && a * right + b * top + c <= 0) || (a * left + b * bottom + c <= 0 && a * right + b * top + c >= 0)) {
        if (left > right) {
            swap(left, right);
        }

        if (top < bottom) {
            swap(top, bottom);
        }

        if ((this->x < left && end_p.x < left) || (this->x > right && end_p.x > right) || (this->y > top && end_p.y > top) || (this->y < bottom && end_p.y < bottom)) { ///判断线段是否在矩形一侧
            return false;
        } else {
            return true;
        }
    }

    return false;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////

const void map_postion::get_near_pos(int range, map_postion& out) const 
{
    int d = mtrandom::rand_int32() % 360;
    out.x = x + (int)(cos((double)d) * range);
    if (out.x < 0) {
        out.x = 0;
    }

    out.y = y + (int)(sin((double)d) * range);
    if (out.y < 0) {
        out.y = 0;
    }
    
    out.z = z;
}

bool map_postion::is_near_pos(const map_postion &p, int distance) const
{
	return is_near_pos(p.x, p.y, p.z, distance);
}

bool map_postion::is_near_pos(int _x, int _y, int _z, int distance) const
{
    return ((z == _z) && ((int)sqrt((double)((x - _x) * (x - _x) + (y - _y) * (y - _y))) <= distance));
}

const int map_postion::get_quadrant(const map_postion& pos) const
{
    int quadrant = 0;
    if (x < pos.x) {
        if (y < pos.y) {
            quadrant = 2;
        } else {
            quadrant = 3;
        }
    } else {
        if (y < pos.y) {
            quadrant = 1;
        } else {
            quadrant = 4;
        }
    }

    return quadrant;
}

void map_postion::random_adjust_pos(uint32 range, int max_width, int max_high)
{
    int i = (int)(mtrandom::rand_int32() % range);
    int k = (int)(mtrandom::rand_int32() % 2); // 这里要外面保证不会超过地图位置
    if (k == 0) {
        if (x + i < max_width) {
            x += i;
        }

        if (y +i < max_high) {
            y += i;
        }
    } else {
        if (x > i) {
            x -= i;
        }

        if (y > i) {
            y -= i;
        }
    }
}

const bool map_postion::is_req_quadrant(const map_postion& cur_pos, const map_postion& dst_pos, uint32 ar) const
{
    int cur_quadrant = get_quadrant(cur_pos);
    int dst_quadrant = get_quadrant(dst_pos);
    int ret = abs(cur_quadrant - dst_quadrant);
    if (ret == 2) {
        return true;
    }

    if (ret == 0) {
        return false;
    }

    if (dst_quadrant == 1) {
        if (cur_quadrant == 2 && abs(dst_pos.y - y) <= (int)ar) {
            return true;
        }

        if (cur_quadrant == 4 && abs(dst_pos.x - x) <= (int)ar) {
            return true;
        }

    } else if (dst_quadrant == 2) {
        if (cur_quadrant == 1 && abs(dst_pos.y - y) <= (int)ar) {
            return true;
        }

        if (cur_quadrant == 3 && abs(dst_pos.x - x) <= (int)ar) {
            return true;
        }
    } else if (dst_quadrant == 3) {
        if (cur_quadrant == 4 && abs(dst_pos.y - y) <= (int)ar) {
            return true;
        }

        if (cur_quadrant == 2 && abs(dst_pos.x - x) <= (int)ar) {
            return true;
        }
    } else {
        if (cur_quadrant == 3 && abs(dst_pos.y - y) <= (int)ar) {
            return true;
        }

        if (cur_quadrant == 1 && abs(dst_pos.x - x) <= (int)ar) {
            return true;
        }
    }

    return false;
}

const bool map_postion::is_infanshaped(const map_postion& cur_pos, const map_postion& target_pos, const map_postion& dst_pos, int angle) const
{
    return false;
}

void map_postion::direction_point(const map_postion& ref, int lengthen, map_postion& taget, map_postion* fix_pos /* = NULL */) const
{
    taget.x = ref.x - this->x;
    taget.y = ref.y - this->y;
    double length = sqrt((double)(taget.x * taget.x + taget.y * taget.y));
    taget.x = (int)((taget.x / length) * lengthen);
    taget.y = (int)((taget.y / length) * lengthen);
    if (fix_pos == NULL) {
        taget.x += this->x;
        taget.y += this->y;
    } else {
        taget.x += fix_pos->x;
        taget.y += fix_pos->y;
    }
}

