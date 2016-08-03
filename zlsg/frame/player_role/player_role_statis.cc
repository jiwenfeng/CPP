#include "player_role_statis.h"

player_role_statis::player_role_statis()
{
    db_role_statis = NULL;
}

player_role_statis::~player_role_statis()
{
    if (db_role_statis != NULL) {
        delete db_role_statis;
        db_role_statis = NULL;
    }
}

int player_role_statis::attach(mysql::tb_role_statis* trs)
{
    if (trs == NULL || db_role_statis == trs) {
        return 0;
    }

    if (db_role_statis != NULL) {
        delete db_role_statis;
    }

    writer_type = WOTE_UPDATE;
    db_role_statis = trs;

    return 0;
}

int player_role_statis::set_role_id(uint32 role_id)
{
    if (db_role_statis == NULL) {
        return -1;
    }
    
    db_role_statis->set_role_id(role_id);
    
    return 0;
}

int player_role_statis::add(uint32 role_id, PLAYER_ROLE_STATIS_TYPE_E type, PLAYER_ROLE_STATIS_UPDATE_TYPE_E update_type, uint32 val)
{
    uint32 now = (uint32)time(NULL);
    if (db_role_statis == NULL) {
        db_role_statis = new mysql::tb_role_statis;
        writer_type = WOTE_INSERT;
        db_role_statis->set_role_id(role_id);
        db_role_statis->set_statis_type(type);
        db_role_statis->set_val(val);
        db_role_statis->set_last_time(now);
        
        return 0;
    }

    if (is_need_update(update_type, now)) {
        db_role_statis->set_val(val);
    } else {
        db_role_statis->set_val(db_role_statis->val() + val);
    }

    db_role_statis->set_last_time(now);

    return 0;
}

bool player_role_statis::is_need_update(PLAYER_ROLE_STATIS_UPDATE_TYPE_E update_type, uint32 now)
{
    if (db_role_statis == NULL) {
        return false;
    }

    bool ret = false;
    switch (update_type) {
    case PRSUTE_DAY_UPDATE: {
            if (now > db_role_statis->last_time()) {
                ret = !time_utils::is_same_day(db_role_statis->last_time(), now);
            }
        break;
        }
    case PRSUTE_WEEK_UPDATE: {
            if (now > db_role_statis->last_time()) {
                ret = !time_utils::is_same_week(db_role_statis->last_time(), now);
            }
        break;
        }
    case PRSUTE_MONTH_UPDATE: {
            if (now > db_role_statis->last_time()) {
                ret = !time_utils::is_same_month(db_role_statis->last_time(), now);
            }
            break;
        }
    default: break;
    }

    return ret;
}

uint32 player_role_statis::get()
{
    if (db_role_statis == NULL) {
        return -1;
    }

    return db_role_statis->val();
}

uint32 player_role_statis::get_last_time()
{
    if (db_role_statis == NULL) {
        return 0;
    }

    return db_role_statis->last_time();
}

void player_role_statis::set_last_time(uint32 t)
{
	if(db_role_statis == NULL) {
		return ;
	}
	db_role_statis->set_last_time(t);
}

int player_role_statis::get_val(uint32& val)
{
    if (db_role_statis == NULL) {
        return -1;
    }

    uint32 now = (uint32)time(NULL);
    if (is_need_update((PLAYER_ROLE_STATIS_UPDATE_TYPE_E)db_role_statis->update_type(), now)) {
        db_role_statis->set_last_time(now);
        db_role_statis->set_val(0);
    }

    val = db_role_statis->val();

    return 0;
}

int player_role_statis::set_val(uint32 val)
{
    if (db_role_statis == NULL) {
        return -1;
    }

    db_role_statis->set_last_time((uint32)time(NULL));
    db_role_statis->set_val(val);

    return 0;
}

int player_role_statis::only_set_val(uint32 val)
{
    if (db_role_statis == NULL) {
        return -1;
    }

    db_role_statis->set_val(val);
	return 0;
}

void player_role_statis::write(amtsvr_context* ctx)
{
    if (db_role_statis == NULL || db_role_statis->role_id() == 0) {
        return;
    }

    CACHE_KEY k(db_role_statis->role_id(), db_role_statis->statis_type());
    mysql::tb_role_statis* tmp = new mysql::tb_role_statis(*db_role_statis);
    pb_writer::write(ctx, k, tmp);
    writer_type = WOTE_UPDATE;
}

void player_role_statis::set_type(PLAYER_ROLE_STATIS_TYPE_E type, PLAYER_ROLE_STATIS_UPDATE_TYPE_E e)
{
    if (db_role_statis == NULL) {
        db_role_statis = new mysql::tb_role_statis;
        writer_type = WOTE_INSERT;
        db_role_statis->set_role_id(0);
    }

    db_role_statis->set_statis_type((uint32)type);
    db_role_statis->set_update_type((uint32)e);
    db_role_statis->set_val(0);
    db_role_statis->set_last_time(0);
}

PLAYER_ROLE_STATIS_UPDATE_TYPE_E player_role_statis::get_update_type()
{
    if (db_role_statis == NULL) {
        return PRSUTE_NOT_UPDATE;
    }

    return (PLAYER_ROLE_STATIS_UPDATE_TYPE_E)db_role_statis->update_type();
}

void player_role_statis::add_val(uint32 val)
{
    if (db_role_statis == NULL) {
        db_role_statis = new mysql::tb_role_statis;
        db_role_statis->set_role_id(0);
    }

    db_role_statis->set_val(db_role_statis->val() + val);
    db_role_statis->set_last_time((uint32)time(NULL));
}

void player_role_statis::sub_val(uint32 val)
{
    if (db_role_statis == NULL) {
        db_role_statis = new mysql::tb_role_statis;
        db_role_statis->set_role_id(0);
    }

    db_role_statis->set_val(db_role_statis->val() - val);
}

bool player_role_statis::is_same_day(uint32 now)
{
    if (db_role_statis == NULL) {
        return false;
    }
    
    return time_utils::is_same_day(db_role_statis->last_time(), now);
}

bool player_role_statis::is_same_week(uint32 now)
{
    if (db_role_statis == NULL) {
        return false;
    }
    
    return time_utils::is_same_week(db_role_statis->last_time(), now);
}

bool player_role_statis::is_same_month(uint32 now)
{
    if (db_role_statis == NULL) {
        return false;
    }
    
    return time_utils::is_same_month(db_role_statis->last_time(), now);
}

uint32 player_role_statis::get_team_exp_raid(uint32 src)
{
    return (src & 0x0F000000) >> 24;
}

uint32 player_role_statis::get_exp_raid(uint32 src)
{
    return (src & 0xF0000000) >> 28;
}

uint32 player_role_statis::add_team_exp_raid(uint32 src, uint32 num/* = 1*/)
{
    return src += (num << 24);
}

uint32 player_role_statis::add_exp_raid(uint32 src, uint32 num/* = 1*/)
{
    return src += (num << 28);
}

uint32 player_role_statis::get_pet_raid(uint32 src)
{
    return (src & 0x00FF0000) >> 16;
}

uint32 player_role_statis::add_pet_raid(uint32 src, uint32 num/* = 1*/)
{
    return src += (num << 16);
}

uint32 player_role_statis::get_stone_raid(uint32 src)
{
    return (src & 0x0000FF00) >> 8;
}

uint32 player_role_statis::add_stone_raid(uint32 src, uint32 num/* = 1*/)
{
    return src += (num << 8);
}

uint32 player_role_statis::add_family_raid(uint32 src, uint32 num/* = 1*/)
{
    return src += num;
}

uint32 player_role_statis::get_team_layer_raid(uint32 src)
{
    return src & 0x0000FFFF;
}

uint32 player_role_statis::add_team_layer_raid(uint32 src, uint32 num/* = 1*/)
{
    return src += num;
}

uint32 player_role_statis::get_layer_raid(uint32 src)
{
    return (src & 0xFFFF0000) >> 16;
}

uint32 player_role_statis::add_layer_raid(uint32 src, uint32 num/* = 1*/)
{
    return src += (num << 16);
}

uint32 player_role_statis::get_money_raid(uint32 src)
{
    return (src & 0xFFFF0000) >> 16;
}

uint32 player_role_statis::add_money_raid(uint32 src, uint32 num/* = 1*/)
{
    return src += (num << 16);
}

uint32 player_role_statis::set_family_welfare_day_once_limit(uint32 src)
{
    return src |= 0x80000000;
}

uint32 player_role_statis::set_join_battle_day_once_limit(uint32 src)
{
    return src |= 0x40000000;
}

bool player_role_statis::is_family_welfare_day_once_limit(uint32 src)
{
    return (src & 0x80000000) != 0;
}

bool player_role_statis::is_join_battle_day_once_limit(uint32 src)
{
    return (src & 0x40000000) != 0;
}

void player_role_statis::fill(client::role_statis_rsp& rsp)
{
    if (db_role_statis == NULL) {
        return ;
    }

	client::role_statis_info* rsi = rsp.add_rsi();

    uint32 val = 0;
    get_val(val);
    rsi->set_type       (db_role_statis->statis_type());
    rsi->set_update_type(db_role_statis->update_type());
    rsi->set_val        (val);
    rsi->set_last_time  (db_role_statis->last_time());
}

