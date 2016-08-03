#include "offline_role.h"

offline_role::offline_role(player_role* r, center_role* c) : _pr(r), _cr(c)
{
    prev[OPTE_CONFLICT] = NULL;
    prev[OPTE_LIST] = NULL;
    next[OPTE_CONFLICT] = NULL;
    next[OPTE_LIST] = NULL;
}

offline_role::~offline_role()
{
    if (_pr != NULL) {
        delete _pr;
    }

    if (_cr != NULL) {
        delete _cr;
    }
}

uint32 offline_role::get_role_id() const
{
    if (_pr == NULL) {
        return 0;
    }

    return _pr->get_role_id();
}

void offline_role::attach(player_role* r)
{
    if (_pr == r) {
        return;
    }

    if (_pr != NULL) {
        if (_pr->get_db_player_role()->level() > r->get_db_player_role()->level()
            ||
            (_pr->get_db_player_role()->level() == r->get_db_player_role()->level()
            &&
            _pr->get_db_player_role()->experience() > r->get_db_player_role()->experience())) {
                amtsvr_log("重复加载用户数据且加载的错误数据");
                r->get_db_player_role()->CopyFrom(*_pr->get_db_player_role());
        }

        delete _pr;
    }

    _pr = r;
}

void offline_role::attach(center_role* c)
{
    if (_cr == c) {
        return;
    }

    if (_cr != NULL) {
        delete _cr;
    }

    _cr = c;
}

void offline_role::detach()
{
    if (_pr != NULL) {
        delete _pr;
        _pr = NULL;
    }

    if (_cr != NULL) {
        delete _cr;
        _cr = NULL;
    }
}

player_role* offline_role::pop()
{
    player_role* tmp = _pr;
    _pr = NULL;
    return tmp;
}

center_role* offline_role::pop_cr()
{
    center_role* tmp = _cr;
    _cr = NULL;
    return tmp;
}

