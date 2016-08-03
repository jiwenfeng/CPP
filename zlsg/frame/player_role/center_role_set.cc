#include "center_role_set.h"

center_role_set::center_role_set()
{
    db_role_set = NULL;
}

center_role_set::~center_role_set()
{
    if (db_role_set != NULL) {
        delete db_role_set;
        db_role_set = NULL;
    }
}

int center_role_set::attach(mysql::tb_role_center_set* trs)
{
    if (db_role_set != NULL) {
        return -1;
    }

    if (trs == NULL) {
        return -1;
    }

    db_role_set = trs;

    return 0;
}

void center_role_set::write(amtsvr_context* ctx)
{
    if (db_role_set != NULL) {
        CACHE_KEY k(db_role_set->role_id(), 0);
        mysql::tb_role_center_set* tmp = new mysql::tb_role_center_set(*db_role_set);
        pb_writer::write(ctx, k, tmp);
    }
}

mysql::tb_role_center_set* center_role_set::create()
{
    if (db_role_set != NULL) {
        return db_role_set;
    }

    db_role_set = new mysql::tb_role_center_set;
    db_role_set->set_team_auto_agree(DEFAULT_TEAM_AUTO_AGREE);
    db_role_set->set_team_auto_join(DEFAULT_TEAM_AUTO_JOIN);
    db_role_set->set_team_min_lvl(DEFAULT_TEAM_MIN_LVL);
    db_role_set->set_team_max_lvl(DEFAULT_TEAM_MAX_LVL);
    writer_type = WOTE_INSERT;

    return db_role_set;
}

uint32 center_role_set::get_role_id() const
{
    return db_role_set == NULL ? 0u : db_role_set->role_id();
}

bool center_role_set::is_auto_agree() const
{
    return (db_role_set == NULL ? DEFAULT_TEAM_AUTO_AGREE : db_role_set->team_auto_agree() > 0);
}

bool center_role_set::is_auto_join() const
{
    return (db_role_set == NULL ? DEFAULT_TEAM_AUTO_JOIN : db_role_set->team_auto_join() > 0);
}

uint32 center_role_set::get_min_lvl() const
{
    return (db_role_set == NULL ? DEFAULT_TEAM_MIN_LVL : db_role_set->team_min_lvl());
}

uint32 center_role_set::get_max_lvl() const
{
    return (db_role_set == NULL ? DEFAULT_TEAM_MAX_LVL : db_role_set->team_max_lvl());
}

#if 0
void center_role_set::fill_team_set(client::team_set& ts) const
{
    if (db_role_set == NULL) {
        ts.set_auto_agree(DEFAULT_TEAM_AUTO_AGREE);
        ts.set_auto_join(DEFAULT_TEAM_AUTO_JOIN);
        ts.set_min_lvl(DEFAULT_TEAM_MIN_LVL);
        ts.set_max_lvl(DEFAULT_TEAM_MAX_LVL);
        return;
    }

    ts.set_auto_agree(db_role_set->team_auto_agree() == 1 ? true : false);
    ts.set_auto_join(db_role_set->team_auto_join() == 1 ? true : false);
    ts.set_min_lvl(db_role_set->team_min_lvl());
    ts.set_max_lvl(db_role_set->team_max_lvl());
}
#endif
#if 0
int center_role_set::set(uint32 role_id, const client::team_set& ts)
{
    create();

    db_role_set->set_role_id(role_id);
    db_role_set->set_team_auto_agree(ts.auto_agree() ? 1:0);
    db_role_set->set_team_auto_join(ts.auto_join() ? 1:0);
    db_role_set->set_team_min_lvl(ts.min_lvl());
    db_role_set->set_team_max_lvl(ts.max_lvl());

    return 0;
}
#endif
