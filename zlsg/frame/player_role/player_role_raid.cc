#include "player_role_raid.h"

player_role_raid::player_role_raid(mysql::tb_role_raid* d)
        : _db_role_raid(d)
{
    writer_type = WOTE_UPDATE;
}

player_role_raid::player_role_raid(const player_role_raid& prr)
{
    if (prr._db_role_raid != NULL) {
        _db_role_raid = new mysql::tb_role_raid(*prr._db_role_raid);
    } else {
        _db_role_raid = NULL;
    }
}

player_role_raid::~player_role_raid()
{
    release();
}

int player_role_raid::init(mysql::tb_role_raid* ptb)
{
    if (ptb == NULL || _db_role_raid != NULL) {
        return -1;
    }

    _db_role_raid = ptb;

    return 0;
}

void player_role_raid::save(amtsvr_context* ctx)
{
    if (_db_role_raid == NULL) {
        return ;
    }

    mysql::tb_role_raid* p = new mysql::tb_role_raid(*_db_role_raid);
    CACHE_KEY k(p->role_id(), p->raid_id());
    write(ctx, k, p);
    writer_type = WOTE_UPDATE;
}

void player_role_raid::release()
{
    if (_db_role_raid != NULL) {
        delete _db_role_raid;
        _db_role_raid = NULL;
    }
}

int player_role_raid::create(uint32 role_id, uint32 raid_id, uint32 star_num, uint32 complete_tick, uint32 complete_time, uint32 type, uint32 count /* = 0 */)
{
    if (_db_role_raid != NULL) {
        return -1;
    }

    writer_type = WOTE_INSERT;
    _db_role_raid = new mysql::tb_role_raid;
    _db_role_raid->set_role_id(role_id);
    _db_role_raid->set_raid_id(raid_id);
    uint32 complete_count       = 1;
    uint32 elite_star_num       = 0;
    uint32 elite_complete_tick  = 0;
    uint32 elite_complete_time  = 0;
    uint32 elite_complete_count = 0;
    if (type == 1) {
        elite_star_num       = star_num;
        elite_complete_tick  = complete_tick;
        elite_complete_time  = complete_time;
        elite_complete_count = complete_count;
        star_num       = 0;
        complete_tick  = 0;
        complete_time  = 0;
        complete_count = 0;
    }

    _db_role_raid->set_star_num                (star_num            );
    _db_role_raid->set_fast_tick               (complete_tick       );
    _db_role_raid->set_last_complete_time      (complete_time       );
    _db_role_raid->set_complete_count          (complete_count      );
    _db_role_raid->set_create_time             (complete_time       );
    _db_role_raid->set_auto_time               (0                   );
    _db_role_raid->set_elite_star_num          (elite_star_num      );
    _db_role_raid->set_elite_fast_tick         (elite_complete_tick );
    _db_role_raid->set_elite_last_complete_time(elite_complete_time );
    _db_role_raid->set_elite_complete_count    (elite_complete_count);
    _db_role_raid->set_elite_create_time       (elite_complete_time );
    _db_role_raid->set_elite_auto_time         (0                   );
	_db_role_raid->set_elite_enter_count	   (count				);
//    _db_role_raid->set_elite_enter_count       (0                   );

    return 0;
}

int player_role_raid::set(uint32 star_num, uint32 complete_tick, uint32 complete_time, uint32 type)
{
    if (_db_role_raid == NULL) {
        return -1;
    }

    if (type == 0) {
        uint32 old_star_num = _db_role_raid->star_num();
        if (star_num < old_star_num || old_star_num == 0) {
            _db_role_raid->set_star_num(star_num);
        }

        uint32 old_fast_tick = _db_role_raid->fast_tick();
        if (complete_tick < old_fast_tick) {
            _db_role_raid->set_fast_tick(complete_tick);
        }

        _db_role_raid->set_last_complete_time(complete_time);
        _db_role_raid->set_complete_count(_db_role_raid->complete_count() + 1);
    } else {
        uint32 old_star_num = _db_role_raid->elite_star_num();
        if (star_num < old_star_num || old_star_num == 0) {
            _db_role_raid->set_elite_star_num(star_num);
        }

        uint32 old_fast_tick = _db_role_raid->elite_fast_tick();
        if (complete_tick < old_fast_tick) {
            _db_role_raid->set_elite_fast_tick(complete_tick);
        }

        if (!time_utils::is_same_day(_db_role_raid->elite_last_complete_time(), complete_time)) {
            _db_role_raid->set_elite_enter_count(0);
        }

        _db_role_raid->set_elite_enter_count(_db_role_raid->elite_enter_count() + 1);

        _db_role_raid->set_elite_last_complete_time(complete_time);
        _db_role_raid->set_elite_complete_count(_db_role_raid->elite_complete_count() + 1);
    }

    return 0;
}

int player_role_raid::set_auto(uint32 type)
{
    uint32 now = (uint32)time(NULL);
    if (type == 0) {
        _db_role_raid->set_auto_time(now);
    } else {
        _db_role_raid->set_elite_auto_time(now);
    }

    return 0;
}

// 0表示未通关
void player_role_raid::modify_raid_star(uint32 star)
{
    if (_db_role_raid == NULL) {
		return ;
    }
	if (_db_role_raid->elite_star_num() > star)
	{
		return;
	}
	_db_role_raid->set_elite_star_num(star);
}

const uint32 player_role_raid::get_star_num() const
{
    if (_db_role_raid == NULL) {
        return 0;
    }

    return _db_role_raid->star_num();
}

const uint32 player_role_raid::get_elite_star_num() const
{
	if (_db_role_raid == NULL) {
		return 0;
	}
	return _db_role_raid->elite_star_num();
}

const uint32 player_role_raid::get_complete_count(uint32 type) const
{
    if (_db_role_raid == NULL) {
        return 0;
    }

    if (type == client::enter_raid_req_type_normal) {
        return _db_role_raid->complete_count();
    } else {
        return _db_role_raid->elite_complete_count();
    }
}

const uint32 player_role_raid::get_elite_raid_enter_count() const
{
	if(_db_role_raid == NULL)
	{
		return 0;
	}
	if(!time_utils::is_same_day(_db_role_raid->elite_last_complete_time(), (uint32)time(NULL)))
	{
		_db_role_raid->set_elite_enter_count(0);
	}
	return _db_role_raid->elite_enter_count();
}

void player_role_raid::fill_info(client::raid_info& ri)
{
    if (!time_utils::is_same_day(_db_role_raid->elite_last_complete_time(), (uint32)time(NULL))) {
        _db_role_raid->set_elite_enter_count(0);
    }

    ri.set_raid_id                 (_db_role_raid->raid_id                 ());
    ri.set_star_num                (_db_role_raid->star_num                ());
    ri.set_fast_tick               (_db_role_raid->fast_tick               ());
    ri.set_last_complete_time      (_db_role_raid->last_complete_time      ());
    ri.set_complete_count          (_db_role_raid->complete_count          ());
    ri.set_create_time             (_db_role_raid->create_time             ());
    ri.set_auto_time               (_db_role_raid->auto_time               ());
    ri.set_elite_star_num          (_db_role_raid->elite_star_num          ());
    ri.set_elite_fast_tick         (_db_role_raid->elite_fast_tick         ());
    ri.set_elite_last_complete_time(_db_role_raid->elite_last_complete_time());
    ri.set_elite_complete_count    (_db_role_raid->elite_complete_count    ());
    ri.set_elite_create_time       (_db_role_raid->elite_create_time       ());
    ri.set_elite_auto_time         (_db_role_raid->elite_auto_time         ());
    ri.set_elite_enter_count       (_db_role_raid->elite_enter_count       ());
}
