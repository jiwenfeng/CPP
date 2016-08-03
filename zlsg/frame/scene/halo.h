#ifndef __HALO_H__
#define __HALO_H__

class scene_role;

class halo {
public:
    halo();
    halo(scene_role* master, uint32 radius, const config_effect* cfg, uint32 tick);
    ~halo();

public:
    void init(scene_role* master, uint32 radius, const config_effect* cfg, uint32 tick);
    void move_update(uint32 tick);
    //void update();
    bool find_role(uint64 id);
    void add_role(scene_role* psr);
    void remove_role(scene_role* psr);
    bool check_distance(scene_role* psr);
    void release();

public:
    inline uint32 get_start_tick() const { return _start_tick; }
    inline uint32 get_end_tick() const { return _end_tick; }

private:
    void add_buff(scene_role* psr, client::role_data* rd);

private:
    scene_role*          _master;
    uint32               _radius;
    const config_effect* _cfg;
    const config_effect* _tirgger;
    std::vector<uint64>  _roles;
    uint32               _start_tick;
    uint32               _end_tick;
};

class halo_mgr
{
public:
    halo_mgr();
    ~halo_mgr();

public:
    void insert(scene_role* master, uint32 radius, const config_effect* cfg, uint32 tick);
    void move_update(uint32 tick);
    void role_in(scene_role* psr, uint32 tick);
    void role_out(scene_role* psr, uint32 tick);
    bool has_halo();
    void remove_all();
    void erase(const config_effect *cfg);

private:
    std::map<uint32, halo> halos;
};


#endif // __HALO_H__