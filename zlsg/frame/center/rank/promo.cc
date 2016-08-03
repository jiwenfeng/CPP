#include "promo.h"
#if 0
promo::promo(mysql::tb_promo_list_of_names* tb)
{
    _change = false;
    writer_type = WOTE_NULL;
    init(tb->type(), tb->role_id(), tb->val(), tb->reward());
}

promo::promo(uint32 type, uint32 role_id, uint32 val, uint32 reward)
{
    _change = true;
    writer_type = WOTE_INSERT;
    init(type, role_id, val, reward);
}

promo::~promo()
{
    if (_promo_list_of_names != NULL) {
        delete _promo_list_of_names;
        _promo_list_of_names = NULL;
    }
}

void promo::init(uint32 type, uint32 role_id, uint32 val, uint32 reward)
{
    _promo_list_of_names = new mysql::tb_promo_list_of_names;
    _promo_list_of_names->set_type(type);
    _promo_list_of_names->set_role_id(role_id);
    _promo_list_of_names->set_val(val);
    _promo_list_of_names->set_reward(reward);
}

int promo::save(amtsvr_context* ctx)
{
    if (!_change || _promo_list_of_names == NULL) {
        return -1;
    }

    mysql::tb_promo_list_of_names* trc = new mysql::tb_promo_list_of_names(*_promo_list_of_names);
    CACHE_KEY k(trc->type(), trc->role_id());
    if (writer_type != WOTE_REMOVE) {
    }
    
    write(ctx, k, trc);
    _change = false;
    writer_type = WOTE_NULL;

    return 0;
}

mysql::tb_promo_list_of_names* promo::get()
{
    return _promo_list_of_names;
}

void promo::set_reward(uint32 reward)
{
    if (_promo_list_of_names == NULL) {
        return ;
    }

    if (writer_type != WOTE_INSERT) {
        writer_type = WOTE_UPDATE;
    }
    
    _change = true;
    _promo_list_of_names->set_reward(reward);
}
#endif
