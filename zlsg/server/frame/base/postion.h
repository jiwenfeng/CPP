#ifndef __POSTION_H__
#define __POSTION_H__

#include <cmath>
#include "define.h"

class postion
{
public:
    postion();
    postion(int a, int b);
    postion(const postion& rhs);
    virtual ~postion();

public:
    bool     operator==(const postion& rhs) const;
    bool     operator!=(const postion& rhs) const;
    bool     operator<(const postion& rhs) const;
    postion& operator=(const postion& rhs);

public:
    uint32 get_distance(const postion& rhs) const;
    double get_fdistance(const postion& rhs) const;
    void add(int _x, int _y);
    const postion& get_point() const{return *this;};

    bool line_rect_intersection(const postion& end_p, const postion& rect_top, const postion& rect_bottom) const;

public:
    int x;
    int y;
};

class map_postion : public postion
{
public:
    map_postion() {
        z = 0;
    }

    map_postion(int _x, int _y, int _z) {
        x = _x;
        y = _y;
        z = _z;
    }

    map_postion(float _x, float _y, float _z) {
        x = (int)_x;
        y = (int)_y;
        z = (int)_z;
    }

    map_postion(uint32 _x, uint32 _y, uint32 _z) {
        x = (int)_x;
        y = (int)_y;
        z = (int)_z;
    }

    map_postion(char* buf) {
        uint16* tmp = (uint16*)buf;
        x = *tmp++;
        y = *tmp++;
        z = *tmp;
    }

    virtual ~map_postion() {
    }
    
public:
    map_postion& operator=(const map_postion& rhs) {
        postion::operator = (rhs);
        z = rhs.z;
        return *this;
    }

    bool operator==(const map_postion& rhs) const {
        return (z == rhs.z && postion::operator ==(rhs));
    }

    bool operator!=(const map_postion& rhs) const {
        return (z != rhs.z || postion::operator !=(rhs));
    }

public:
    bool is_in_same_flat(const map_postion& rhs) const {
        return z == rhs.z;
    }

    const void get_near_pos(int range, map_postion& out) const;

	bool is_near_pos(const map_postion &p, int distance = 200) const;

    bool is_near_pos(int _x, int _y, int _z, int distance = 200) const;

    inline void reset(int _x = 0, int _y = 0, int _z = 0) {
        x = _x;
        y = _y;
        z = _z;
    }

    const bool is_infanshaped(const map_postion& cur_pos, const map_postion& target_pos, const map_postion& dst_pos, int angle = 60) const;

    const bool is_req_quadrant(const map_postion& cur_pos, const map_postion& dst_pos, uint32 ar) const;

    const int get_quadrant(const map_postion& pos) const;

    void random_adjust_pos(uint32 range, int max_width, int max_high);

    void direction_point(const map_postion& ref, int lengthen, map_postion& taget, map_postion* fix_pos = NULL) const;

public:
    static bool is_point_in_circular_sector(float cx, float cy, float ux, float uy, float r, float theta, float px, float py) {
        float dx = px - cx;
        float dy = py - cy;
        float length = sqrt(dx * dx + dy * dy);

        // |D| > r
        if (length > r)
            return false;
 
        // Normalize D
        dx /= length;
        dy /= length;

        ux = px - ux;
        uy = py - uy;
        float l = sqrt(ux * ux + uy * uy);
        ux /= l;
        uy /= l;

        static const float PI = 3.1415926535f;
        float a = acos(dx * ux + dy * uy) / PI * 180.0f;
        return a < theta;
    }

    static bool is_point_in_rectangle(const map_postion& mp1, const map_postion& mp2, const map_postion& mp3, const map_postion& mp4, const map_postion& mp) {
        return multiply(mp, mp1, mp2) * multiply(mp, mp4, mp3) <= 0 && multiply(mp, mp4, mp1) * multiply(mp, mp3, mp2) <= 0;
    }
    static int multiply(const map_postion& p1, const map_postion& p2, const map_postion& p0) {
        return ((p1.x - p0.x) * (p2.y - p0.y) - (p2.x - p0.x) * (p1.y - p0.y));
    }

public:
    int z;
};

class move_map_postion : public map_postion
{
public:
    move_map_postion() { t = 0; };
    ~move_map_postion() {};

public:
     move_map_postion& operator=(const map_postion& rhs) {
        map_postion::operator=(rhs);
        t = 0;
        return *this;
    }

    move_map_postion& operator=(const move_map_postion& rhs) {
        map_postion::operator=(rhs);
        t = rhs.t;
        return *this;
    }

    bool operator==(const map_postion& rsh) {
        return map_postion::operator==(rsh);
    }

    bool operator==(const move_map_postion& rsh) {
        return map_postion::operator==(rsh);
    }

    bool operator!=(const map_postion& rhs) const {
        return map_postion::operator!=(rhs);
    }

    bool operator!=(const move_map_postion& rhs) const {
        return map_postion::operator!=(rhs);
    }

public:
    inline void reset(int _x = 0, int _y = 0, int _z = 0, int _t = 0) {
        map_postion::reset(_x, _y, _z);
        t = _t;
    }
    
public:
    int t;
};

class mov_map_pos : public map_postion
{
public:
    mov_map_pos() {
        distance = 0;
        need_time = 0;
        start_time = 0;
    }

    virtual ~mov_map_pos() {
    }

    mov_map_pos& operator=(const mov_map_pos& rhs) {
        map_postion::operator = (rhs);
        distance = rhs.distance;
        need_time = rhs.need_time;
        start_time = rhs.start_time;
        return *this;
    }

    void init(const map_postion& cur, const map_postion& next, float pixe_sec, uint32& next_start_time) {
        double next_distance = cur.get_fdistance(next);
        double next_need_time = (next_distance * 100) / pixe_sec;
        
        x  = next.x;
        x -= cur.x;
        y  = next.y;
        y -= cur.y;

        distance   = (uint32)next_distance;
        need_time  = (uint32)next_need_time;
        start_time = next_start_time;

        next_start_time += need_time;
    }

public:
    uint32 distance;    // 从这一节点到下一节点的距离
    uint32 need_time;   // 从这一节点走向下一节点所需要的时间
    uint32 start_time;  // 从这一节点开始走的时间
};

#endif
