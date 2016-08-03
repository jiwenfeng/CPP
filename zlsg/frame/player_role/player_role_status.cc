#include "config_birth.h"
#include "config_map.h"
#include "player_role_status.h"

player_role_status::player_role_status(mysql::tb_role_status* s /* = NULL*/ ) : db_role_status(s)
{
}

player_role_status::~player_role_status()
{
    if (db_role_status != NULL) {
        delete db_role_status;
        db_role_status = NULL;
    }
}

int player_role_status::create(uint32 role_id, const config_birth* cb, uint32 now)
{
    if (db_role_status == NULL) {
        db_role_status = new mysql::tb_role_status;
        writer_type = WOTE_INSERT;
    }

    db_role_status->set_role_id              (role_id);
    db_role_status->set_pos_map_id           (cb->get_scene_id());
    db_role_status->set_pos_scene_id         (cb->get_scene_id());
    db_role_status->set_pos_x                (0);
    db_role_status->set_pos_y                (0);
    db_role_status->set_pos_z                (0);
    db_role_status->set_tmp_map_id           (0);
    db_role_status->set_tmp_scene_id         (0);
    db_role_status->set_tmp_x                (0);
    db_role_status->set_tmp_y                (0);
    db_role_status->set_tmp_z                (0);
    db_role_status->set_mditation            (0);
    db_role_status->set_riding               (0);
    db_role_status->set_findpath             (0);
    db_role_status->set_fight                (0);
    db_role_status->set_auto_play            (0);
    db_role_status->set_pvp_mode             (0);
    db_role_status->set_attk_mode            (0);
    db_role_status->set_alive                (1);
    db_role_status->set_online               (0);
    db_role_status->set_red_goods_num        (0);
    db_role_status->set_blue_goods_num       (0);
    db_role_status->set_complete_task_num    (0);
    db_role_status->set_abandon_task_num     (0);
    db_role_status->set_receve_task_num      (0);
    db_role_status->set_is_package_full      (0);
    db_role_status->set_buf_furious          (0);
    db_role_status->set_buf_hurted           (0);
    db_role_status->set_buf_week             (0);
    db_role_status->set_buf_giddy            (0);
    db_role_status->set_buf_faint            (0);
    db_role_status->set_buf_fired            (0);
    db_role_status->set_buf_posion           (0);
    db_role_status->set_buf_blood            (0);
    db_role_status->set_buf_injury           (0);
    db_role_status->set_buf_cursed           (0);
    db_role_status->set_buf_blind            (0);
    db_role_status->set_buf_maimed           (0);
    db_role_status->set_buf_silent           (0);
    db_role_status->set_buf_disarm           (0);
    db_role_status->set_buf_slowly           (0);
    db_role_status->set_buf_dec_harm         (0);
    db_role_status->set_buf_rej_harm         (0);
    db_role_status->set_buf_turn_body        (0);
    db_role_status->set_strength             (0);
    db_role_status->set_created_date         (now);
    db_role_status->set_tmp_scene_inst_id    (0); 
    db_role_status->set_tmp_scene_inst_time  (0);
    db_role_status->set_spa_action_times     (0);
    db_role_status->set_spa_last_time        (0);
    db_role_status->set_arena_last_chall_time(0);
    db_role_status->set_arena_has_times      (0);
    db_role_status->set_arena_buy_times      (0);
    db_role_status->set_test_rewards         (0);
    db_role_status->set_unlimited_fly        (0);
    db_role_status->set_status               (1);

    return 0;
}

void player_role_status::attach(mysql::tb_role_status* s)
{
    if (db_role_status == s) {
        return;
    }

    if (db_role_status != NULL) {
        delete db_role_status;
    }

    db_role_status = s;
}

void player_role_status::write_status(amtsvr_context* ctx)
{
    if (db_role_status != NULL) {
        CACHE_KEY k(db_role_status->role_id(), 0);
        mysql::tb_role_status* tmp = new mysql::tb_role_status(*db_role_status);
        write(ctx, k, tmp);
        writer_type = WOTE_UPDATE;
    }
}

void player_role_status::set_pos(uint32 map_id, uint32 scene_id, int x, int y, int z)
{
    if (db_role_status == NULL) {
        return;
    }

    db_role_status->set_pos_map_id(map_id);
    db_role_status->set_pos_scene_id(scene_id);
    db_role_status->set_pos_x(x);
    db_role_status->set_pos_y(y);
    db_role_status->set_pos_z(z);
}

void player_role_status::set_tmp_pos(uint32 map_id, uint32 scene_id, int x, int y, int z, uint32 scene_inst_id)
{
    if (db_role_status == NULL) {
        return;
    }

    db_role_status->set_tmp_map_id(map_id);
    db_role_status->set_tmp_scene_id(scene_id);
    db_role_status->set_tmp_x(x);
    db_role_status->set_tmp_y(y);
    db_role_status->set_tmp_z(z);
    db_role_status->set_tmp_z(z);
    db_role_status->set_tmp_scene_inst_id(scene_inst_id);
    db_role_status->set_tmp_scene_inst_time((uint32)time(NULL));
}

void player_role_status::set_alive(uint32 alive)
{
    if (db_role_status == NULL) {
        return ;
    }

    db_role_status->set_alive(alive);
}

int player_role_status::init_now_pos(uint32& map_id, uint32& scene_id, uint32& scene_inst_id)
{
    if (db_role_status == NULL) {
        map_id        = 1;
        scene_id      = 1;
        scene_inst_id = 0;
        return 0;
    }

    if (db_role_status->tmp_map_id() > 0 && db_role_status->tmp_scene_id() && db_role_status->tmp_scene_inst_id() > 0) {
        uint32 now = (uint32)time(NULL);
        if ((db_role_status->tmp_scene_inst_time() + STATUS_ROLE_SCENE_RESERVE > now) && (db_role_status->tmp_x() > 0 || db_role_status->tmp_y() > 0)) {
            map_id        = db_role_status->tmp_map_id();
            scene_id      = db_role_status->tmp_scene_id();
            scene_inst_id = db_role_status->tmp_scene_inst_id();
            return 0;
        }

        set_tmp_pos(0, 0, 0, 0, 0, 0);
    }

    map_id         = db_role_status->pos_map_id();
    scene_id       = db_role_status->pos_scene_id();
    scene_inst_id  = 0;

    if (map_id > 0) {
        const config_map* cm = CONFMGR->get_config_map(map_id);
        bool default_map = true;
        do {
            if (cm == NULL) {
                break;
            }

            if (db_role_status->pos_x() > cm->get_real_width() || db_role_status->pos_y() > cm->get_real_height()) {
                break;
            }

            if (cm->is_block(db_role_status->pos_x() / cm->get_unit_grid_width(), db_role_status->pos_y() / cm->get_unit_grid_height())) {
                break;
            }

            default_map = false;
        } while (false);
                
        if (!default_map) {
            return 0;
        } else if (cm != NULL) {
            map_id   = cm->id;
            scene_id = cm->id;
            set_pos(cm->id, cm->id, cm->birthPointX, cm->birthPointY, 0);
        }
    }

    const config_map* cm = CONFMGR->get_config_map_default();
    if (cm != NULL) {
        map_id   = cm->id;
        scene_id = cm->id;
        set_pos(cm->id, cm->id, cm->birthPointX, cm->birthPointY, 0);
        set_tmp_pos(0, 0, 0, 0, 0, 0);
    }

    return 0;
}

void player_role_status::clear_tmp_pos(uint32& map_id, uint32& scene_id)
{
    set_tmp_pos(0, 0, 0, 0, 0, 0);
    map_id = db_role_status->pos_map_id();
    scene_id = db_role_status->pos_scene_id();
}

uint32 player_role_status::get_spa_action_times()
{
    if (db_role_status == NULL){
        return 0;
    }

    uint32 now = (uint32)time(NULL);
    if (db_role_status->spa_last_time() + SPA_INTERVAL < now){
        db_role_status->set_spa_action_times(SPA_ACTION_INIT_TIMES);
    }

    return  db_role_status->spa_action_times();
}

void player_role_status::init_action_times(uint32 now)
{
    if (db_role_status == NULL) {
        return;
    }

    if (db_role_status->spa_last_time() + SPA_INTERVAL < now) {
        db_role_status->set_spa_action_times(SPA_ACTION_INIT_TIMES);
        db_role_status->set_spa_last_time(now);
    }
}

void player_role_status::sub_action_times(uint32 val)
{
    if (db_role_status == NULL) {
        return;
    }

    if (db_role_status->spa_action_times() <= val) {
        db_role_status->set_spa_action_times(0);
    } else {
        db_role_status->set_spa_action_times(db_role_status->spa_action_times() - val);
    }
}

#if 0
void player_role_status::fill_arena_info(client::arena_rank_info_rsp& rsp)
{
    if (db_role_status == NULL) {
        return;
    }

    uint32 next_cd = 0;
    rsp.set_times(get_arena_chall_times(next_cd));
    rsp.set_next_chall_cd(next_cd);
}
#endif

uint32 player_role_status::get_arena_chall_times(uint32& next_cd)
{
    if (db_role_status == NULL) {
        return 0;
    }

    uint32 now = (uint32)time(NULL);
    if (!time_utils::is_same_day(db_role_status->arena_last_chall_time(), now)) {
        db_role_status->set_arena_has_times(ARENA_CHALL_TIMES);
        next_cd = 0;
    } else {
        if (db_role_status->arena_last_chall_time() + ARENA_CHALL_CD <= now) {
            next_cd = 0;
        } else {
            next_cd = db_role_status->arena_last_chall_time() + ARENA_CHALL_CD - now;
        }
    }

    return db_role_status->arena_has_times();
}

bool player_role_status::sub_arena_chall_times(uint32 n)
{
    uint32 next_cd = 0;
    if (n <= 0 || n > get_arena_chall_times(next_cd) || next_cd > 0) {
        return false;
    }

    db_role_status->set_arena_has_times(db_role_status->arena_has_times() - 1);
    db_role_status->set_arena_last_chall_time((uint32)time(NULL));

    return true;
}

void player_role_status::add_arena_chall_times(uint32 n)
{
    if (n <= 0) {
        return;
    }

    db_role_status->set_arena_has_times(db_role_status->arena_has_times() + 1);

    return;
}

void player_role_status::set_unlimited_fly()
{
    if (db_role_status != NULL) {
        db_role_status->set_unlimited_fly(time_utils::make_day_end_time());
    }
}
