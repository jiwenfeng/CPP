#ifndef __PLAYER_ROLE_STATUS_H__
#define __PLAYER_ROLE_STATUS_H__

#include "comm_define.h"
#define STATUS_ROLE_SCENE_RESERVE  60 // 保留1分钟场景
#define SPA_INTERVAL 1800  // 温泉开启间隔
#define SPA_ACTION_INIT_TIMES 12  // 温泉互动次数 
#define ARENA_CHALL_TIMES     15  // 一天15次的挑战机会
#define ARENA_CHALL_CD        300 // 300秒一次挑战间隔

class player_role_status : public pb_writer
{
public:
    player_role_status(mysql::tb_role_status* s = NULL);
    virtual ~player_role_status();

public:
    int create(uint32 role_id, const config_birth* cb, uint32 now);
    void attach(mysql::tb_role_status* s);

public:
    mysql::tb_role_status* get_db_role_status(){return db_role_status;};

    void write_status(amtsvr_context* ctx);

    void set_pos(uint32 map_id, uint32 scene_id, int x, int y, int z);
    void set_tmp_pos(uint32 map_id, uint32 scene_id, int x, int y, int z, uint32 scene_inst_id = 0);
    void set_alive(uint32 alive);

    int init_now_pos(uint32& map_id, uint32& scene_id, uint32& scene_inst_id);

    void clear_tmp_pos(uint32& map_id, uint32& scene_id);

    uint32 get_spa_action_times();
    void init_action_times(uint32 now);
    void sub_action_times(uint32 val);

   // void fill_arena_info(client::arena_rank_info_rsp& rsp);
    uint32 get_arena_chall_times(uint32& next_cd);
    bool sub_arena_chall_times(uint32 n = 1);
    void add_arena_chall_times(uint32 n = 1);

    void set_unlimited_fly();
    inline bool is_unlimited_fly()
    {
        if (db_role_status == NULL || db_role_status->unlimited_fly() == 0){
            return false;
        }

        return ((uint32)time(NULL) < db_role_status->unlimited_fly());
    }

protected:
    mysql::tb_role_status* db_role_status;

};

#endif
