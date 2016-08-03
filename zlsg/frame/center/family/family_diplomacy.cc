#include "family_diplomacy.h"

family_diplomacy::family_diplomacy(mysql::tb_family_diplomacy* family_diplomacy_db/* = NULL*/)
    : _diplomacy(family_diplomacy_db)
    , _change(false)
{
    writer_type = WOTE_NULL;
}

family_diplomacy::family_diplomacy(uint32 family_id, uint32 target_id,uint32 relation_type,uint32 apply_time)
    : _diplomacy(NULL)
    , _change(false)
{
    create_diplomacy_info(family_id, target_id,relation_type,apply_time);
 }

family_diplomacy::~family_diplomacy()
{
    if (_diplomacy != NULL) {
        delete _diplomacy;
        _diplomacy = NULL;
    }
}

int family_diplomacy::save(amtsvr_context* ctx)
{
    if (!_change || _diplomacy == NULL) {
        return -1;
    }

    mysql::tb_family_diplomacy* tfd = new mysql::tb_family_diplomacy(*_diplomacy);
    CACHE_KEY k(tfd->family_id(), tfd->target_id());
    write(ctx, k, tfd);
    _change = false;
    writer_type = WOTE_NULL;
    
    return 0;
}

int family_diplomacy::create_diplomacy_info(uint32 family_id, uint32 target_id,uint32 relation_type,uint32 apply_time)
{
    if (_diplomacy == NULL) {
        _diplomacy = new mysql::tb_family_diplomacy;
        writer_type = WOTE_INSERT;
    } else {
        writer_type = WOTE_UPDATE;
    }

    _diplomacy->set_family_id(family_id);
    _diplomacy->set_target_id(target_id);
    _diplomacy->set_relation_type(relation_type);
	_diplomacy->set_apply_time(apply_time);

    _change = true;

    return 0;
}

int family_diplomacy::change_relaition_type(uint32 relation_type)
{
	if (_diplomacy == NULL) {
		return -1;
	}

	if (writer_type != WOTE_INSERT) {
		writer_type = WOTE_UPDATE;
	}

	_diplomacy->set_relation_type(relation_type);

	_change = true;

	return 0;
}

int family_diplomacy::remove()
{
	if (writer_type == WOTE_INSERT) {
		_change = false;
		return 0;
	} else {
		writer_type = WOTE_REMOVE;
		_change = true;
		return 0;
	}

	return 0;
}