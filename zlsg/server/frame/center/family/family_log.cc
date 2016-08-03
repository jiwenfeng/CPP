#include "family_log.h"

family_log::family_log(mysql::tb_family_log* tfl/* = NULL*/)
        : _log(tfl)
        , change (false)
{
    writer_type = WOTE_NULL;
}

family_log::family_log(uint32 family_id, uint32 time, uint32 type, uint32 key, uint32 value,string family_name,uint32 smy_id)
{
    writer_type = WOTE_INSERT;
    _log = new mysql::tb_family_log;
    set(family_id, time, type, key, value,family_name,smy_id);
}

family_log::~family_log()
{
    if (_log != NULL) {
        delete _log;
        _log = NULL;
    }
}

int family_log::set(uint32 family_id, uint32 time, uint32 type, uint32 key, uint32 value,string family_name,uint32 smy_id)
{
    if (_log == NULL) {
        return -1;
    }

    if (writer_type != WOTE_INSERT) {
        writer_type = WOTE_UPDATE;
    }

    _log->set_id(smy_id);
    _log->set_family_id(family_id);
    _log->set_time(time);
    _log->set_type(type);
    _log->set_key(key);
    _log->set_value(value);
	_log->set_family_name(family_name);
    change = true;

    return 0;
}

int family_log::remove()
{
    if (writer_type == WOTE_INSERT) {
        change = false;
    } else {
        writer_type = WOTE_REMOVE;
        change = true;
    }

    return 0;
}

int family_log::fill(client::family_log_info* fli)
{
    if (_log == NULL) {
        return -1;
    }
    
    fli->set_time(_log->time());
    fli->set_type(_log->type());
    fli->set_key(_log->key());
    fli->set_value(_log->value());
	fli->set_family_name(_log->family_name());
    return 0;
}

int family_log::save(amtsvr_context* ctx)
{
    if (!change || _log == NULL) {
        return -1;
    }

    mysql::tb_family_log* tfl = new mysql::tb_family_log(*_log);
    CACHE_KEY k(tfl->id(), tfl->family_id());
    if (writer_type != WOTE_REMOVE){
    }
    write(ctx, k, tfl);
    change = false;
    
	writer_type = WOTE_UPDATE;
    return 0;
}
