#include "player_role_label.h"

player_role_label::player_role_label(mysql::tb_role_label* p)
    : _trl(p)
{
    writer_type = WOTE_UPDATE;
}

player_role_label::~player_role_label()
{
    if (_trl != NULL) {
        delete _trl;
        _trl = NULL;
    }
}

int player_role_label::save(amtsvr_context* ctx)
{
    if (_trl == NULL) {
        return -1;
    }

    mysql::tb_role_label*  trl = new mysql::tb_role_label(*_trl);
    CACHE_KEY k(trl->role_id(), trl->label_code());
    write(ctx, k, trl);
    writer_type = WOTE_UPDATE;
    
    return 0;
}

int player_role_label::create(uint32 role_id, uint32 label_code)
{
    if (_trl == NULL) {
        _trl = new mysql::tb_role_label;
        writer_type = WOTE_INSERT;
    }

    _trl->set_role_id(role_id);
    _trl->set_label_code(label_code);
    _trl->set_use_state(0);
    _trl->set_effective_state(0);
    _trl->set_end_date(0);
    
    return 0;
}

uint32 player_role_label::label_code()
{
    if (_trl == NULL) {
        return 0;
    }

    return _trl->label_code();
}

int player_role_label::set_use_state(uint32 use_state)
{
    if (_trl == NULL) {
        return -1;
    }
    
    _trl->set_use_state(use_state);
    
    return 0;
}
    
int player_role_label::set_effective_state(uint32 effective_state)
{
    if (_trl == NULL) {
        return -1;
    }

    _trl->set_effective_state(effective_state);
    
    return 0;
}

int player_role_label::set_end_date(uint32 end_date)
{
    if (_trl == NULL) {
        return -1;
    }
    
    _trl->set_end_date(end_date);
    
    return 0;
}
    
bool player_role_label::is_end()
{
    if (_trl->end_date() == 0) {
        return false;
    }

    time_t t = time(0);

    return t > _trl->end_date();
}

int player_role_label::fill_label_info(client::label_info* li)
{
    if (this->_trl == NULL) {
        return -1;
    }

    li->set_label_code(_trl->label_code());
    li->set_use_state(_trl->use_state());
    li->set_effective_state(_trl->effective_state());
    li->set_end_date(_trl->end_date());

    return 0;
}
