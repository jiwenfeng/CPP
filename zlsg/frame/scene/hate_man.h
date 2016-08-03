#ifndef __HATE_MAN_H__
#define __HATE_MAN_H__

#include "define.h"

class hate_man;

class hate {
public:
    hate(uint64 id = 0, uint32 first = 0, uint32 last = 0, uint32 v = 0, uint32 real_damage = 0, uint32 distance = 0);

public:
    bool operator>(const hate& rhs);
    bool operator>=(const hate& rhs);

public:
    inline uint64 get_inst_id() const { return _inst_id; }
    inline void set_inst_id(uint64 val) { _inst_id = val; }
    inline uint32 get_val() const { return _val; }

public:
    friend class hate_man;

private:
    uint64 _inst_id;     // 仇恨人的ID
    uint32 _first;	     // 第一次增加仇恨值的时间
    uint32 _last;	     // 最后一次增加仇恨值的时间
    int32  _val;         // 对角色的仇恨值
    uint32 _distance;    // 距离
    uint32 _real_damage; // 实际伤害
    uint32 _last_damage; // 上次伤害
    uint32 _high_damage; // 单次最高
};

class hate_man {
public:
    hate_man();
    virtual ~hate_man();

public:
    uint64 get_hate_role();

    inline bool has_hate_role() { return !_hate_map.empty(); }

    void add_hate_role(uint64 id, int val, uint32 real_damge, uint32 distance);

    void del_hate_role(uint64 id);

    void clean_hate_map();

    void get_hate_rank_list(std::vector<uint64>& v, int num);

    void get_real_damage_rank_list(std::map<uint32, uint64>& rank);

protected:
    std::map<uint64, hate> _hate_map;
};

#endif

