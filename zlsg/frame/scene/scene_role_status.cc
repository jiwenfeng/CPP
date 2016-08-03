#include "scene_role.h"
#include "scene_role_status.h"

scene_role_status::scene_role_status() : _ref(GetReflection()), _desc(GetDescriptor())
{
    _modify_status = NULL;
    _change_status = false;
    _psr           = NULL;
}

scene_role_status::scene_role_status(const scene_role_status& rhs) : client::role_status(rhs)
{
    _ref           = rhs._ref;
    _desc          = rhs._desc;
    _modify_status = rhs._modify_status;
    _change_status = false;
    _psr           = rhs._psr;
}

scene_role_status::~scene_role_status()
{
}

int scene_role_status::init(mysql::tb_role_status* s, scene_role* psr)
{
    _psr = psr;

    if (s == NULL) {
        return -1;
    }

    set_meditate      (false                                          );
    set_autopath      (false                                          );
    set_fight         (false                                          );
    set_robot         (false                                          );
    set_pvp           ((client::role_status_pvp_status)s->pvp_mode()  );
    set_online        (true                                           );
    set_atk_mode      ((client::role_status_attack_mode)s->attk_mode());
    set_alive         ((client::role_status_alive_status)s->alive()   );
    set_riding        ((client::role_status_riding_status)s->riding() );
    set_furious       (s->buf_furious  () > 0                         );
    set_hurted        (s->buf_hurted   () > 0                         );
    set_week          (s->buf_week     () > 0                         );
    set_giddy         (s->buf_giddy    () > 0                         );
    set_faint         (s->buf_faint    () > 0                         );
    set_fired         (s->buf_fired    () > 0                         );
    set_posion        (s->buf_posion   () > 0                         );
    set_blood         (s->buf_blood    () > 0                         );
    set_injury        (s->buf_injury   () > 0                         );
    set_cursed        (s->buf_cursed   () > 0                         );
    set_blind         (s->buf_blind    () > 0                         );
    set_maimed        (s->buf_maimed   () > 0                         );
    set_silent        (s->buf_silent   () > 0                         );
    set_disarm        (s->buf_disarm   () > 0                         );
    set_slowly        (s->buf_slowly   () > 0                         );
    set_dec_harm      (s->buf_dec_harm () > 0                         );
    set_rej_harm      (s->buf_rej_harm () > 0                         );
    set_streanth      (s->strength     () > 0                         );
    set_body          (s->buf_turn_body() > 0                         );
    set_guard         (false                                          );

    return 0;
}

int scene_role_status::init()
{
    set_meditate      (false                                           );
    set_autopath      (false                                           );
    set_fight         (false                                           );
    set_robot         (false                                           );
    set_pvp           (client::role_status_pvp_status_pvp_prote        );
    set_online        (true                                            );
    set_atk_mode      (client::role_status_attack_mode_attack_mod_peace);
    set_alive         (client::role_status_alive_status_status_alive   );
    set_riding        (client::role_status_riding_status_status_walk   );
    set_furious       (false                                           );
    set_hurted        (false                                           );
    set_week          (false                                           );
    set_giddy         (false                                           );
    set_faint         (false                                           );
    set_fired         (false                                           );
    set_posion        (false                                           );
    set_blood         (false                                           );
    set_injury        (false                                           );
    set_cursed        (false                                           );
    set_blind         (false                                           );
    set_maimed        (false                                           );
    set_silent        (false                                           );
    set_disarm        (false                                           );
    set_slowly        (false                                           );
    set_dec_harm      (false                                           );
    set_rej_harm      (false                                           );
    set_streanth      (false                                           );
    set_body          (false                                           );
    set_guard         (false                                           );

    return 0;
}

void scene_role_status::normalize()
{
    uint32 count = _desc->field_count();
    for (uint32 i = client::RSE_STATUS_FURIOUS; i < client::RSE_STATUS_STREANTH && i < count; ++i) {
        if (_ref->GetBool(*this, _desc->field(i))) {
            _change_status = true;
            _ref->SetBool(this, _desc->field(i), false);
            if (_modify_status != NULL) {
                _modify_status->set_val(i, false);
            }
        }
    }
}

void scene_role_status::set_val(uint32 index, uint32 val)
{
    if (index < client::RSE_STATUS_MEDITATE || index >= client::RSE_STATUS_MAX) {
        return;
    }

    _change_status = true;

    switch (index) {
    case client::RSE_STATUS_PVP: {
        if (val < client::role_status_pvp_status_pvp_kill || val > client::role_status_pvp_status_pvp_prote) {
            break;
        }
        set_pvp((client::role_status_pvp_status)val);
        if (_modify_status) _modify_status->set_pvp((client::role_status_pvp_status)val);
        break;
    }
    case client::RSE_STATUS_ATTKMODE: {
        if (val < client::role_status_attack_mode_attack_mod_null || val >= client::role_status_attack_mode_attack_mod_invalid) {
            break;
        }
        set_atk_mode((client::role_status_attack_mode)val);
        if (_modify_status) _modify_status->set_atk_mode((client::role_status_attack_mode)val);
        break;
    }
    case client::RSE_STATUS_ALIVE: {
        if (val < client::role_status_alive_status_status_alive || val > client::role_status_alive_status_status_dead) {
            break;
        }
        set_alive((client::role_status_alive_status)val);
        if (_modify_status) _modify_status->set_alive((client::role_status_alive_status)val);
        break;
    }
    case client::RSE_STATUS_RIDING: {
        if (val < client::role_status_riding_status_status_walk || val > client::role_status_riding_status_status_fly) {
            break;
        }
        set_riding((client::role_status_riding_status)val);
        if (_modify_status) _modify_status->set_riding((client::role_status_riding_status)val);
        break;
    }
    case client::RSE_STATUS_GUARD: {
        set_guard(val);
        if (_modify_status) {
            _modify_status->set_guard(val);
        }
        break;
    }
    default: {
        bool real_val = (val != 0 ? true : false);
        _ref->SetBool(this, _desc->field(index), real_val);
        if (_modify_status != NULL) {
            if (_modify_status->_ref->HasField(*_modify_status, _modify_status->_desc->field(index))) {
                if (real_val != _modify_status->_ref->GetBool(*_modify_status, _modify_status->_desc->field(index))) {
                    _modify_status->_ref->ClearField(_modify_status, _modify_status->_desc->field(index));
                    break;
                }
            }

            _modify_status->set_val(index, val);
        }
        break;
    }
    }

    if (_psr != NULL) {
        _psr->on_status_change(index, val);
    }
}

void scene_role_status::set_attack_mode(uint8 attk_type, uint8 attk_mode)
{
    switch (attk_type) {
    case EATE_FORCE_SET: {
        if ((uint8)atk_mode() != attk_mode 
            && attk_mode > client::role_status_attack_mode_attack_mod_null 
            && attk_mode < client::role_status_attack_mode_attack_mod_invalid) {
            _change_status = true;
            set_attack_mode((const client::role_status_attack_mode)attk_mode);
        }
        break;
    }
    case EATE_FORBID: {
        if ((uint8)atk_mode() == attk_mode) {
            if (attk_mode == client::role_status_attack_mode_attack_mod_peace) {
                _change_status = true;
                set_attack_mode(client::role_status_attack_mode_attack_mod_team);
            } else {
                _change_status = true;
                set_attack_mode(client::role_status_attack_mode_attack_mod_peace);
            }
        }
        break;
    }
    default:
        break;
    }
}

void scene_role_status::set_attack_mode(const client::role_status_attack_mode m)
{
    set_val(client::RSE_STATUS_ATTKMODE, m);
}

bool scene_role_status::client_set_attack_mode(uint8 attk_type, uint8 attk_mode, const client::role_status_attack_mode set_type)
{
    if (atk_mode() == set_type) {
        return false;
    }

    switch (attk_type) {
    case EATE_FORCE_SET: {
        if (set_type != attk_mode) {
            return false;
        }
        break;
    }
    case EATE_FORBID: {
        if (set_type == attk_mode) {
            return false;
        }
        break;
    }
    default: break;
    }

    _change_status = true;
    set_attack_mode((client::role_status_attack_mode)set_type);

    return true;
}

void scene_role_status::set_dead()
{
    set_val(client::RSE_STATUS_ALIVE, client::role_status_alive_status_status_dead);
}

void scene_role_status::set_relive()
{
    set_val(client::RSE_STATUS_ALIVE, client::role_status_alive_status_status_alive);
}

void scene_role_status::set_guard_start(uint32 quality)
{
    set_val(client::RSE_STATUS_GUARD, quality);
}

void scene_role_status::set_guard_end()
{
    set_val(client::RSE_STATUS_GUARD, 0);
}

void scene_role_status::set_raid(bool raid /*= true*/)
{
    set_val(client::RSE_STATUS_STREANTH, raid);
}

void scene_role_status::set_modify_status(scene_role_status* srs)
{
    _modify_status = srs; 
}

bool scene_role_status::fill_modify_status(client::role_status* srs)
{
    if (srs != NULL) {
        return pop_modify_status(*srs);
    }

    return false;
}

bool scene_role_status::pop_modify_status(client::role_status& srs)
{
    if (!_change_status || _modify_status == NULL) {
        return false;
    }

    srs = *_modify_status;
    _modify_status->Clear();
    _change_status = false;

    return true;
}

void scene_role_status::pop_all_status(client::role_status& srs)
{
    srs.CopyFrom(*this);
    if (_modify_status != NULL) {
        _modify_status->Clear();
    }

    _change_status = false;
}

void scene_role_status::reset()
{
    Clear();
    if (_modify_status != NULL) {
        _modify_status->Clear();
    }

    _change_status = false;
    init();
}
