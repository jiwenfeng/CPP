#include "player_role_order.h"

player_role_order::player_role_order(mysql::tb_role_order* tro/* = NULL*/)
    : _order(tro)
{
    writer_type = WOTE_UPDATE;
}

int player_role_order::create(uint32 role_id, uint32 store_id, uint32 count, uint32 type)
{
    _order = NULL;
    return create(role_id, store_id, count, type, (uint32)time(NULL));
}

player_role_order::~player_role_order()
{
    release();
}

void player_role_order::save(amtsvr_context* ctx)
{
    if (_order == NULL) {
        return ;
    }

    CACHE_KEY k(_order->role_id(), _order->store_id());
    mysql::tb_role_order* tmp = new mysql::tb_role_order(*_order);
    pb_writer::write(ctx, k, tmp);
    writer_type = WOTE_UPDATE;
}

int player_role_order::create(uint32 role_id, uint32 store_id, uint32 count, uint32 type, uint32 time)
{
    if (_order == NULL) {
        _order = new mysql::tb_role_order;
        writer_type = WOTE_INSERT;
    } else if (writer_type != WOTE_INSERT) {
        writer_type = WOTE_UPDATE;
    }
    
    _order->set_role_id(role_id);
    _order->set_store_id(store_id);
    _order->set_count(count);
    _order->set_type(type);
    _order->set_time(time);
    
    return 0;
}

int player_role_order::add(uint32 count)
{
    if (_order == NULL) {
        return -1;
    }

    check_time();
    _order->set_count(_order->count() + count);
    _order->set_time((uint32)time(NULL));
    
    return 0;
}

uint32 player_role_order::get()
{
    if (_order == NULL) {
        return 0;
    }

    check_time();
    return _order->count();
}

int player_role_order::check_time()
{
    if (_order == NULL) {
        return -1;
    }

    uint32 order_time = _order->time();
    uint32 now        = (uint32)time(NULL);
    bool   same_day   = false;
    if (now > order_time) {
        same_day = time_utils::is_same_day(order_time, now);

        if (!same_day) {
            _order->set_count(0);
        }
    }

    return 0;
}

void player_role_order::fill(client::order_tracking_info* oti)
{
    if (_order == NULL || oti == NULL) {
        return ;
    }

    check_time();
    oti->set_id(_order->store_id());
    oti->set_count(_order->count());
    oti->set_type(_order->type());
}

int player_role_order::remove()
{
    if (_order == NULL) {
        return -1;
    }

    if (writer_type == WOTE_INSERT) {
        writer_type = WOTE_NULL;
    } else if (writer_type != WOTE_NULL) {
        writer_type = WOTE_REMOVE;
    }
    
    return 0;
}
