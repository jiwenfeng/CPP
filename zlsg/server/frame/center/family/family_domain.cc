#include "family_domain.h"

family_domain::family_domain(mysql::tb_family_domain* tfl/* = NULL*/)
    : _domain(tfl)
    , _change(false)
{
    writer_type = WOTE_NULL;
}

family_domain::family_domain(uint32 family_id, uint32 scene_id)
    : _domain(NULL)
    , _change(false)
{
    set(family_id, scene_id);
 }

family_domain::~family_domain()
{
    if (_domain != NULL) {
        delete _domain;
        _domain = NULL;
    }
}

int family_domain::save(amtsvr_context* ctx)
{
    if (!_change || _domain == NULL) {
        return -1;
    }

    mysql::tb_family_domain* tfd = new mysql::tb_family_domain(*_domain);
    CACHE_KEY k(tfd->scene_id(), 0);
    write(ctx, k, tfd);
    _change = false;
    writer_type = WOTE_NULL;
    
    return 0;
}

int family_domain::set(uint32 family_id, uint32 scene_id)
{
    if (_domain == NULL) {
        _domain = new mysql::tb_family_domain;
        writer_type = WOTE_INSERT;
    } else {
        writer_type = WOTE_UPDATE;
    }

    _domain->set_family_id(family_id);
    _domain->set_scene_id(scene_id);
    _domain->set_time((uint32)time(NULL));
    _change = true;

    return 0;
}

void family_domain::set_family_id(uint32 family_id)
{
    if (_domain == NULL) {
        set(family_id, 0);
    } else {
        set(family_id, _domain->scene_id());
    }
}
