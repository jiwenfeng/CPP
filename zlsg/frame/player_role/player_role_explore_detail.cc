#include "player_role_explore_detail.h"

player_role_explore_detail::player_role_explore_detail(mysql::tb_role_explore_detail* tec/* = NULL*/)
    : _db_role_explore_detail(tec)
    , _change(false)
{
    writer_type = WOTE_NULL;
}

player_role_explore_detail::~player_role_explore_detail()
{
    if (_db_role_explore_detail != NULL) {
        delete _db_role_explore_detail;
        _db_role_explore_detail = NULL;
    }
}

int player_role_explore_detail::set(uint32 role_id, uint32 time, uint32 place_id, uint32 goods_id, uint32 goods_num, uint32 goods_quality)
{
    if (_db_role_explore_detail == NULL) {
        writer_type = WOTE_INSERT;
        _db_role_explore_detail = new mysql::tb_role_explore_detail;
    } else {
        writer_type = WOTE_UPDATE;
    }

    _db_role_explore_detail->set_id(0);
    _db_role_explore_detail->set_role_id(role_id);
    _db_role_explore_detail->set_time(time);
    _db_role_explore_detail->set_place_id(place_id);
    _db_role_explore_detail->set_goods_id(goods_id);
    _db_role_explore_detail->set_goods_num(goods_num);
    _db_role_explore_detail->set_goods_quality(goods_quality);
    _change = true;

    return 0;
}

mysql::tb_role_explore_detail* player_role_explore_detail::get()
{
    return _db_role_explore_detail;
}

int player_role_explore_detail::save(amtsvr_context* ctx)
{
    if (!_change || _db_role_explore_detail == NULL) {
        return -1;
    }

    mysql::tb_role_explore_detail* tred = new mysql::tb_role_explore_detail(*_db_role_explore_detail);
    CACHE_KEY k(tred->id(), tred->role_id());
    if (writer_type != WOTE_REMOVE){
    }
    write(ctx, k, tred);
    _change = false;

    return 0;
}

