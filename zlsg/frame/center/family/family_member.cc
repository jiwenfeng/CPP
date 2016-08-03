#include "family_member.h"

family_member::family_member(mysql::tb_family_member* tfm /*= NULL*/)
        : _member(tfm)
        , change(false)
{
    writer_type = WOTE_NULL;
}

family_member::family_member(uint32 family_id, uint32 role_id, uint32 permissions, uint32 time, uint32 add_by)
{
    writer_type = WOTE_INSERT;
    _member = new mysql::tb_family_member;
    set(family_id, role_id, permissions, time, add_by);
}


family_member::~family_member()
{
    if (_member != NULL) {
        delete _member;
        _member = NULL;
    }
}

uint32 family_member::role_id()
{
    if (_member == NULL) {
        return -1;
    }

    return _member->role_id();
}

uint32 family_member::family_id()
{
    if (_member == NULL) {
        return -1;
    }

    return _member->family_id();
}

uint32 family_member::add_by()
{
    if (_member == NULL) {
        return -1;
    }

    return _member->add_by();
}

uint32 family_member::permissions()
{
    if (_member == NULL) {
        return 0;
    }
    
    return _member->permissions();
}

uint32 family_member::time()
{
    if (_member == NULL) {
        return 0;
    }

    return _member->time();
}

int family_member::set(uint32 family_id, uint32 role_id, uint32 permissions, uint32 time, uint32 add_by)
{
    if (_member == NULL) {
        return -1;
    }

    if (writer_type != WOTE_INSERT) {
        writer_type = WOTE_UPDATE;
    }

    _member->set_family_id(family_id);
    _member->set_role_id(role_id);
    _member->set_permissions(permissions);
    _member->set_time(time);
    _member->set_add_by(add_by);
    _member->set_strength(0);
	_member->set_dragon_count(0);
	_member->set_tiger_count(0);
	_member->set_rosefinch_count(0);
	_member->set_tortoise_count(0);
	_member->set_money(0);
	_member->set_total_donate(0);
    change = true;

    return 0;
}

int family_member::set_permissions(uint32 permissions)
{
    if (_member == NULL) {
        return -1;
    }

    if (writer_type != WOTE_INSERT) {
        writer_type = WOTE_UPDATE;
    }

    _member->set_permissions(permissions);
    change = true;

    return 0;
}

int family_member::set_time(uint32 time)
{
    if (_member == NULL) {
        return -1;
    }

    if (writer_type != WOTE_INSERT) {
        writer_type = WOTE_UPDATE;
    }

    _member->set_time(time);
    change = true;

    return 0;
}

int family_member::set_strength(uint32 strength)
{
    if (_member == NULL) {
        return -1;
    }

    if (writer_type != WOTE_INSERT) {
        writer_type = WOTE_UPDATE;
    }

    _member->set_strength(strength);
    change = true;

    return 0;
}

int family_member::set_dragon_count(uint32 dragon_count)
{
	if (_member == NULL) {
		return -1;
	}

	if (writer_type != WOTE_INSERT) {
		writer_type = WOTE_UPDATE;
	}

	_member->set_dragon_count(dragon_count);

	change = true;

	return 0;
}

int family_member::set_tiger_count(uint32 tiger_count)
{
	if (_member == NULL) {
		return -1;
	}

	if (writer_type != WOTE_INSERT) {
		writer_type = WOTE_UPDATE;
	}

	_member->set_tiger_count(tiger_count);

	change = true;

	return 0;
}

int family_member::set_rosefinch_count(uint32 rosefinch_count)
{

	if (_member == NULL) {
		return -1;
	}

	if (writer_type != WOTE_INSERT) {
		writer_type = WOTE_UPDATE;
	}

	_member->set_rosefinch_count(rosefinch_count);

	change = true;

	return 0;
}

int family_member::set_tortoise_count(uint32 tortoise_count)
{
	if (_member == NULL) {
		return -1;
	}

	if (writer_type != WOTE_INSERT) {
		writer_type = WOTE_UPDATE;
	}

	_member->set_tortoise_count(tortoise_count);

	change = true;

	return 0;
}

int family_member::set_money(uint32 money)
{
	if (_member == NULL) {
		return -1;
	}

	if (writer_type != WOTE_INSERT) {
		writer_type = WOTE_UPDATE;
	}

	_member->set_money(money);

	change = true;

	return 0;
}

int family_member::set_total_donate(uint32 total_donate)
{
	if (_member == NULL) {
		return -1;
	}

	if (writer_type != WOTE_INSERT) {
		writer_type = WOTE_UPDATE;
	}

	_member->set_total_donate(total_donate);

	change = true;

	return 0;
}

int family_member::remove()
{
    if (writer_type == WOTE_INSERT) {
        change = false;
        return 0;
    } else {
        writer_type = WOTE_REMOVE;
        change = true;
        return 0;
    }

    return 0;
}

int family_member::save(amtsvr_context* ctx)
{
    if (!change || _member == NULL) {
        return -1;
    }

    mysql::tb_family_member* tfm = new mysql::tb_family_member(*_member);
    CACHE_KEY k(tfm->family_id(), tfm->role_id());
    if (writer_type != WOTE_REMOVE) {
    }

    write(ctx, k, tfm);

    change      = false;
    writer_type = WOTE_NULL;

    return 0;
}
