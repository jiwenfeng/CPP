#ifndef __SCENE_INIT_MGR_H__
#define __SCENE_INIT_MGR_H__

#include "scene.h"
#include "game_map.h"

class scene_inst;
class scene_user;
class raid_scene;

// 场景实例管理，管理多个实例， map是实例id到实例的映射关系
class scene_inst_mgr
{
public:
    scene_inst_mgr(const game_map* p, const uint32 sid, struct amtsvr_context* c);
    virtual ~scene_inst_mgr();

public:
    int init();

    const game_map* get_game_map() const { return gm; }

    scene_inst* get_scene(uint32 scene_inst_id, scene_user* su);
	scene_inst *get_normal_scene();

    void destroy(uint32 scene_inst_id);

    int update(uint32 tick, bool is_one_sec, bool is_fiv_sec, bool is_ten_sec, bool is_thirty_sec, bool is_one_minute); // 500ms为单位的更新

    int get_scene_by_scene_id(uint32 scene_id, std::vector<scene_inst*>& vsi);

    scene_user* get_user(uint32 role_id);
    scene_user* find_user(uint32 role_id);

    int login_scene(scene_user* su, uint32 scene_id, uint32 scene_inst_id);
    int logout_scene(scene_user* su, scene_inst* si);

    void go_to_space(scene_user* su);
    void go_to_scene(scene_user* su);

    scene_user* get_space_user(uint32 role_id);

    int create_randnpc(client::randnpc* rn);

    void on_update_farm_info(client::update_farm_info* ufi);

    scene_inst* create_siege_scene();
    scene_inst* get_siege_scene();
    scene_inst* get_boss_scene();

    void destroy_all();

    void save();

    void release();

protected:
    scene_inst* get_raid_scene(const raid_scene* rs, scene_user* su);

    scene_inst* alloc(scene_user* su);  // 分配一个新的场景实例

    scene_inst* get(uint32 scene_inst_id);

    scene_inst* get_battle_scene();

    scene_inst* get_scene_by_family(uint32 family_id);

    scene_inst* get_scene_by_farmer(uint32 farm_role_id);

    scene_inst* get_scene_by_test(uint32 scene_id);

    scene_inst* create_raid_scene(const scene* s, scene_user* su, bool& bnew);

    scene_inst* create_battle_scene(const scene* s);

    scene_inst* create_world_boss_scene(const scene* s);

    scene_inst* get_scene_by_team(uint32 team_id);

    void destroy(scene_inst* inst);

private:
    const game_map* gm;
    const uint32 scene_id;
    struct amtsvr_context* ctx;
    uint32 alloc_scene_inst_id;
    std::map<uint32, scene_inst*> scene_inst_map;     // scene_inst id-->scene_inst
    std::map<uint32, uint32>      team_ref_scene;     // team_id->scene_inst_id
    std::map<uint32, uint32>      family_ref_scene;   // family_id-->scene_inst_id
    std::map<uint32, uint32>      farm_ref_scene;     // farm role_id-->scene_inst_id
    std::map<uint32, uint32>      test_ref_scene;
    //uint32 session_id;
    std::map<uint32, scene_user*> _space;
};

#endif
