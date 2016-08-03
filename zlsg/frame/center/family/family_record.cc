#include "family_record.h"

family_record::family_record(mysql::tb_family_record* tfr /* = NULL */)
    : _record(tfr)
    , _change(false)
{
    writer_type = WOTE_NULL;
}

family_record::family_record(uint32 type, uint32 killer_family, uint32 killer, uint32 slain_family, uint32 slain_title, uint32 slain)
    : _record(NULL)
    , _change(false)
{
    set(type, killer_family, killer, slain_family, slain_title, slain);
}

family_record::~family_record()
{
    if (_record != NULL) {
        delete _record;
        _record = NULL;
    }
}

int family_record::save(amtsvr_context* ctx)
{
    if (!_change || _record == NULL || writer_type == WOTE_NULL) {
        return -1;
    }

    mysql::tb_family_record* tfr = new mysql::tb_family_record(*_record);
    CACHE_KEY k(tfr->id(), 0);
    write(ctx, k, tfr);
    writer_type = WOTE_NULL;
    
    return 0;
}

int family_record::set(uint32 type, uint32 killer_family, uint32 killer, uint32 slain_family, uint32 slain_title, uint32 slain)
{
    if (_record == NULL) {
        _record = new mysql::tb_family_record;
        writer_type = WOTE_INSERT;
    } else if (writer_type != WOTE_INSERT) {
        writer_type = WOTE_UPDATE;
    }

    _change = true;
    _record->set_id(0);
    _record->set_type(type);
    _record->set_killer_family(killer_family);
    _record->set_killer(killer);
    _record->set_slain_family(slain_family);
    _record->set_slain_title(slain_title);
    _record->set_slain(slain);
    
    return 0;
}

int family_record::remove()
{
    if (writer_type == WOTE_INSERT) {
        _change = false;
    } else {
        writer_type = WOTE_REMOVE;
        _change = true;
    }
    
    return 0;
}
