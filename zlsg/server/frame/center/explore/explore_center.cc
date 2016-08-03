#include "explore_center.h"

explore_center::explore_center(mysql::tb_role_explore_detail* tred/* = NULL*/)
    : _explore(tred)
    , _change(false)
{
    writer_type = WOTE_NULL;
}

explore_center::~explore_center()
{
    if (_explore != NULL) {
        delete _explore;
        _explore = NULL;
    }
}

int explore_center::set(client::explore_update* eu)
{
    if (_explore == NULL) {
        _explore = new mysql::tb_role_explore_detail;
    }

    _explore->set_id(0);
    _explore->set_role_id(eu->role_id());
    _explore->set_time(eu->time());
    _explore->set_place_id(eu->place_id());
    _explore->set_goods_id(eu->goods_id());
    _explore->set_goods_num(eu->goods_num());
    _explore->set_goods_quality(eu->goods_quality());
    _change = true;

    return 0;
}

mysql::tb_role_explore_detail* explore_center::get()
{
    return this->_explore;
}

int explore_center::save(amtsvr_context* ctx)
{
    if (!_change) {
        return 0;
    }


    return 0;
}
