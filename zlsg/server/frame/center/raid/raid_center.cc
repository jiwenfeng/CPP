#include "raid_center.h"

raid_center::raid_center(mysql::tb_raid_center* trc/* = NULL */)
    : _db_raid_center(trc)
    , _change(false)
{
    writer_type = WOTE_NULL;
}

raid_center::~raid_center()
{
    if (_db_raid_center != NULL) {
        delete _db_raid_center;
        _db_raid_center = NULL;
    }
}

int raid_center::set(uint32 raid_id, uint32 role_id, uint32 time)
{
    if (_db_raid_center == NULL) {
        _db_raid_center = new mysql::tb_raid_center;
        writer_type = WOTE_INSERT;
    }

    if (writer_type != WOTE_INSERT) {
        writer_type = WOTE_UPDATE;
    }

    _db_raid_center->set_raid_id(raid_id);
    _db_raid_center->set_clearance_sec(time);
    _db_raid_center->set_clearance_role_id(role_id);
    _change = true;
    
    return 0;
}

mysql::tb_raid_center* raid_center::get()
{
    return _db_raid_center;
}

int raid_center::save(amtsvr_context* ctx)
{
    if (!_change || _db_raid_center == NULL) {
        return -1;
    }

    mysql::tb_raid_center* trc = new mysql::tb_raid_center(*_db_raid_center);
    CACHE_KEY k(trc->raid_id(), 0);
    if (writer_type != WOTE_REMOVE){
    }
    write(ctx, k, trc);
    _change = false;
    
    return 0;
}
