#if 0
#include "siege_center.h"

siege_center::siege_center(mysql::tb_siege_warfare* tf/* = NULL*/)
    : _siege_warfare(tf)
    , _change(false)
{
    writer_type = WOTE_NULL;
}

siege_center::~siege_center()
{
    if (_siege_warfare != NULL) {
        delete _siege_warfare;
        _siege_warfare = NULL;
    }
}

int siege_center::save(amtsvr_context* ctx)
{
    if (!_change || _siege_warfare == NULL) {
        return -1;
    }

    mysql::tb_siege_warfare* tf = new mysql::tb_siege_warfare(*_siege_warfare);
    CACHE_KEY k(tf->id(), 0);
    write(ctx, k, tf);
    writer_type = WOTE_NULL;
    _change = false;
    
    return 0;
}

int siege_center::update(client::broadcast_siege_official* req)
{
    if (_siege_warfare == NULL) {
        _siege_warfare = new mysql::tb_siege_warfare;
        writer_type = WOTE_INSERT;
        _siege_warfare->set_id(1);
    } else if (writer_type != WOTE_INSERT) {
        writer_type = WOTE_UPDATE;
    }

    _siege_warfare->set_castellan(0);
    _siege_warfare->set_depety_castellan_0(0);
    _siege_warfare->set_depety_castellan_1(0);
    _siege_warfare->set_official_0(0);
    _siege_warfare->set_official_1(0);
    _siege_warfare->set_official_2(0);
    _siege_warfare->set_official_3(0);

    _change = true;
    official_tag = 0;
    depety_castellan_tag = 0;
    for (int32 i = 0; i < req->coi_size(); ++i) {
        switch (req->coi(i).office()) {
        case 4: _siege_warfare->set_castellan(req->coi(i).role_id()); break;
        case 2: set_depety_castellan(req->coi(i).role_id()); break;
        case 1: set_official(req->coi(i).role_id()); break;
        default: break;
        }
    }
        
    return 0;
}

int siege_center::fill(client::query_city_official_rsp& rsp)
{
    if (_siege_warfare == NULL) {
        return -1;
    }
    
    client::city_official_info* coi = NULL;
    coi = rsp.add_coi();
    coi->set_role_id(_siege_warfare->castellan());
    coi->set_office(4);
    coi = rsp.add_coi();
    coi->set_role_id(_siege_warfare->depety_castellan_0());
    coi->set_office(2);
    coi = rsp.add_coi();
    coi->set_role_id(_siege_warfare->depety_castellan_1());
    coi->set_office(2);
    coi = rsp.add_coi();
    coi->set_role_id(_siege_warfare->official_0());
    coi->set_office(1);
    coi = rsp.add_coi();
    coi->set_role_id(_siege_warfare->official_1());
    coi->set_office(1);
    coi = rsp.add_coi();
    coi->set_role_id(_siege_warfare->official_2());
    coi->set_office(1);
    coi = rsp.add_coi();
    coi->set_role_id(_siege_warfare->official_3());
    coi->set_office(1);
    
    return 0;
}

void siege_center::set_official(uint32 role_id)
{
    switch (official_tag) {
    case 0: _siege_warfare->set_official_0(role_id); break;
    case 1: _siege_warfare->set_official_1(role_id); break;
    case 2: _siege_warfare->set_official_2(role_id); break;
    case 3: _siege_warfare->set_official_3(role_id); break;
    default:
        break;
    }
    
    ++official_tag;
}

void siege_center::set_depety_castellan(uint32 role_id)
{
    switch (depety_castellan_tag) {
    case 0: _siege_warfare->set_depety_castellan_0(role_id); break;
    case 1: _siege_warfare->set_depety_castellan_1(role_id); break;
    default:
        break;
    }
    
    ++depety_castellan_tag;
}
#endif
