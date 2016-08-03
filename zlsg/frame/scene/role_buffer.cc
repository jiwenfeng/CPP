#include "config_effect.h"
#include "player_role_buffer.h"
#include "scene_role_attr.h"
#include "scene_role_status.h"
#include "role_buffer.h"

role_buffer::role_buffer(player_role_buffer* b) : prb(b)
{
    _change  = false;
    _trigger = false;
}

role_buffer::role_buffer(const role_buffer& rhs)
{
    change_buffer = rhs.change_buffer;
    mapbuf        = rhs.mapbuf;
    prb           = rhs.prb;
    _change       = rhs._change;
    _trigger      = rhs._trigger;
}

role_buffer::~role_buffer()
{
}

int role_buffer::init_buf(const client::buffer_info& bi)
{
    return 0;
}

int role_buffer::reset_buf() 
{
    return 0;
}

int role_buffer::add_buf(const client::buffer_info& bi)
{
    role_buffer_info rbi(bi);
    mapbuf.insert(std::pair<uint32, role_buffer_info>(bi.effect_id(), rbi));
    return 0;
}

int role_buffer::add_buf(const config_effect* effect, client::role_data* caster /* = NULL */)
{
    if (effect == NULL) {
        return 1;
    }

    if (check_immune(effect)) {
        return ADD_RET_IMMUNE;
    }

    multimap<uint32, role_buffer_info>::iterator it = mapbuf.begin();
    for ( ; it != mapbuf.end(); ++it) {
        bool same_type_id = false;
        const std::vector<int>& type_ids = effect->get_effect_type_id();
        for (size_t i = 0; i  < type_ids.size(); ++i) {
            if (it->second.get_config_effect() == NULL) {
                break;
            }

            const std::vector<int>& type_ids_t = it->second.get_config_effect()->get_effect_type_id();
            for (size_t j = 0; j < type_ids_t.size(); ++j) {
                if (type_ids[i] == type_ids_t[j]) {
                    same_type_id = true;
                    break;
                }
            }

            if (same_type_id) {
                break;
            }
        }

        if (!same_type_id) {
            continue;
        }

        if (caster != NULL) {
            it->second.mutable_caster()->CopyFrom(*caster);
        }

        return add_buf_exist(it->second, effect);
    }

    return add_buf_new(effect, caster);
}

int role_buffer::add_buf(const config_effect* effect, role_buffer_info& rbi)
{
    if (effect == NULL/* || effect->get_*/) {
        return -1;
    }

    add_buf_time(effect, rbi, false);
    add_buf_value(effect, rbi, false);

    if (effect->get_repeat_max() > rbi.cur_stacks()) {
        rbi.set_cur_stacks(rbi.cur_stacks() + 1);
    }

    rbi.set_operate_flag(client::BLME_UPDATE);
    change_buffer.push_back(rbi);

    return 0;
}

int role_buffer::add_buf_time(const config_effect* effect, role_buffer_info& rbi, bool change /* = true */)
{
    if (effect == NULL) {
        return -1;
    }

    rbi.set_max_effect_times(rbi.max_effect_times() + rbi.duration() / (rbi.interval() > 0 ? rbi.interval() : 1));

    if (effect->get_duration_tick() == (uint32)-1) {
        rbi.set_remain_tick(effect->get_duration_tick());
    } else {
        rbi.set_remain_tick(effect->get_duration_tick() / 10 + rbi.remain_tick());
    }

    if (change) {
        if (effect->get_repeat_max() > rbi.cur_stacks()) {
            rbi.set_cur_stacks(rbi.cur_stacks() + 1);
        }

        rbi.set_operate_flag(client::BLME_UPDATE);
        set_buf(effect, rbi);
        change_buffer.push_back(rbi);
    }

    return 0;
}

int role_buffer::add_buf_value(const config_effect* effect, role_buffer_info& rbi, bool change /* = true */)
{
    if (effect == NULL) {
        return -1;
    }

    if (effect->get_repeat_max() <= rbi.cur_stacks()) {
        return 0;
    }

    buf_set_attr(effect, rbi);
    buf_set_status(effect, rbi);
    buf_set_shiled(effect, rbi);
    buf_set_rebound(effect, rbi);
    buf_set_skills(effect, rbi);

    if (change) {
        rbi.set_cur_stacks(rbi.cur_stacks() + 1);
        rbi.set_operate_flag(client::BLME_UPDATE);
        set_buf(effect, rbi);
        change_buffer.push_back(rbi);
    }

    return 0;
}

int role_buffer::del_buf(const config_effect* effect)
{
    if (effect == NULL) {
        return -1;
    }

    multimap<uint32, role_buffer_info>::iterator it = mapbuf.find(effect->get_effect_id());
    while (it != mapbuf.end()) {
        if (effect->get_effect_id() != it->first) {
            return -1;
        }

        if (it->second.repeat_levels() != effect->get_repeat_levels()) {
            ++it;
            continue;
        }

        on_timeout(0, it->second);
        change_buffer.push_back(it->second);
        mapbuf.erase(it++);
        break;
    }

    return 0;
}

int role_buffer::del_buf(uint32 effect_id)
{
    const config_effect* ce = CONFMGR->get_config_effect(effect_id);
    if (ce == NULL) {
        return -1;
    }

    multimap<uint32, role_buffer_info>::iterator it = mapbuf.find(effect_id);
    while (it != mapbuf.end()) {
        if (effect_id != it->first) {
            return -1;
        }

        if (it->second.repeat_levels() != ce->get_repeat_levels()) {
            ++it;
            continue;
        }

        return on_timeout(0, it->second);
    }

    return -1;
}

int role_buffer::del_buf(uint32 useful_count, uint32 harmful_count)
{
    if (useful_count == 0 && harmful_count == 0) {
        return 0;
    }

    bool del = false;
    multimap<uint32, role_buffer_info>::iterator it = mapbuf.begin();
    int ret = 0;
    while (it != mapbuf.end()) {
        if (it->second.get_config_effect()->get_buff_useful_type() == EUT_USEFUL && useful_count > 0) {
            --useful_count;
            del = true;
        } else if (it->second.get_config_effect()->get_buff_useful_type() == EUT_HARMFUL && harmful_count > 0) {
            --harmful_count;
            del = true;
        } else if (useful_count == 0 && harmful_count == 0) {
            break;
        }

        if (del) {
            del = false;
            on_timeout(0, it->second);
            change_buffer.push_back(it->second);
            mapbuf.erase(it++);
        } else {
            ++it;
        }

        ++ret;
    }

    return ret;
}

int role_buffer::del_buf(const std::vector<int>& types)
{
    if (types.empty()) {
        return 0;
    }

    int ret = 0;
    std::vector<int>::const_iterator begin = types.begin();
    std::vector<int>::const_iterator end   = types.end();
    for ( ; begin != end; ++begin) {
        multimap<uint32, role_buffer_info>::iterator it = mapbuf.begin();
        while (it != mapbuf.end()) {
            const std::vector<int>& cur_types = it->second.get_config_effect()->get_effect_type_id();
            if (std::find(cur_types.begin(), cur_types.end(), *begin) != cur_types.end()) {
                on_timeout(0, it->second);
                change_buffer.push_back(it->second);
                mapbuf.erase(it++);
            } else {
                ++it;
            }

            ++ret;
        }
    }

    return ret;
}

int role_buffer::rep_buf(const config_effect* effect)
{
    if (effect == NULL) {
        return -1;
    }

    multimap<uint32, role_buffer_info>::iterator it = mapbuf.find(effect->get_effect_id());
    while (it != mapbuf.end() && it->second.repeat_levels() == effect->get_repeat_levels()) {
        if (effect->get_effect_id() != it->first) {
            return -1;
        }

        it->second.Clear();
        it->second.set_operate_flag(client::BLME_UPDATE);
        set_buf(effect, it->second);
        change_buffer.push_back(it->second);
        break;
    }

    return 0;
}

void role_buffer::save_buffer()
{
    if (prb == NULL) {
        return;
    }

    prb->remove_all();
    std::multimap<uint32, role_buffer_info>::const_iterator it = mapbuf.begin();
    for ( ; it != mapbuf.end(); ++it) {
        prb->add_buffer(it->second);
    }
}

void role_buffer::on_dead()
{
    std::multimap<uint32, role_buffer_info>::iterator it = mapbuf.begin();
    while (it != mapbuf.end()) {
        role_buffer_info&    rbi = it->second;
        const config_effect* ce  = rbi.get_config_effect();
        if (rbi.get_operate_flag() == client::BLME_DELETE || ce == NULL || ce->is_death_remove()) {
            rbi.set_operate_flag(client::BLME_DELETE);
            change_buffer.push_back(rbi);
            mapbuf.erase(it++);
        } else {
            ++it;
        }
    }
}

bool role_buffer::del_raid_buf()
{
    bool is_del = false;
    multimap<uint32, role_buffer_info>::iterator it = mapbuf.begin();
    while (it != mapbuf.end()) {
        role_buffer_info&    rbi = it->second;
        const config_effect* ce  = rbi.get_config_effect();
        if (rbi.get_operate_flag() == client::BLME_DELETE || ce == NULL || ce->get_out_raid_clear_buff() == 1) {
            rbi.set_operate_flag(client::BLME_DELETE);
            change_buffer.push_back(rbi);
            mapbuf.erase(it++);
            is_del = true;
        } else {
            ++it;
        }
    }

	uint32 count;
	update(time_utils::tenms(), count);

    return is_del;
}

void role_buffer::calculate_attr(scene_role_attr* p)
{
    if (p == NULL) {
        return ;
    }

    for (size_t i = 0; i < _attr.size(); ++i) {
        if (_attr[i].operate() == client::buffer_effect_val_OP_ADD) {
            int32 add_val = _attr[i].val();
            if (add_val < 0) {
                p->sub_val((ROLE_ATTRIBUTE)_attr[i].type(), (uint32)abs(add_val));
            } else {
                p->add_val((ROLE_ATTRIBUTE)_attr[i].type(), add_val);
            }
        } else {
            int32 add_val = (int32)(p->get_val((ROLE_ATTRIBUTE)_attr[i].type()) * (_attr[i].val() / 10000.0f));
            bool sub = add_val < 0;
            if (_attr[i].min_val() != 0 || _attr[i].max_val() != 0) {
                add_val = min(abs(add_val), _attr[i].max_val());
                //add_val = max(_attr[i].min_val(), add_val);
            }

            if (sub) {
                p->sub_val((ROLE_ATTRIBUTE)_attr[i].type(), (uint32)abs(add_val));
            } else {
                p->add_val((ROLE_ATTRIBUTE)_attr[i].type(), add_val);
            }
        }
    }
}

void role_buffer::calculate_status(scene_role_status* p)
{
    if (p == NULL) {
        return ;
    }

    for (size_t i = 0; i < _status.size(); ++i) {
        p->set_val(_status[i].type(), _status[i].val());
    }

    if (_taunt.inst_id() != 0) {
        p->set_val(client::RSE_STATUS_REJ_HARM, 1);
    }
}

void role_buffer::calculate_shield(scene_role_attr* p, uint32& damage)
{
    if (p == NULL) {
        return ;
    }

    int32 fix_damage = 0;
    std::multimap<uint32, role_buffer_info>::iterator i = mapbuf.begin();
    for ( ; i != mapbuf.end() && damage != 0; ++i) {
        if (!i->second.has_shield() || i->second.get_operate_flag() == client::BLME_DELETE) {
            continue;
        }

        fix_damage = shield_each_bear_harm(i->second.mutable_shield()->mutable_ebh(), damage);
        if (i->second.mutable_shield()->durable_max() > fix_damage) {
            i->second.mutable_shield()->set_durable_max(i->second.mutable_shield()->durable_max() - damage);
        } else {
            fix_damage = i->second.mutable_shield()->durable_max();
            fix_damage = min(damage, (uint32)fix_damage);
            i->second.mutable_shield()->set_durable_max(0);

            uint32 hp = min((int32)(i->second.mutable_shield()->over_back_proportion() * p->get_val(MAX_HP)), i->second.mutable_shield()->over_back_max());
            p->add_val(CUR_HP, hp);

            del_buf(i->second.mutable_shield()->effect_id());
        }

        damage -= fix_damage;
    }
}

void role_buffer::calculate_rebound(uint32 damage, uint32& rebound)
{
    rebound = 0;
    std::multimap<uint32, role_buffer_info>::iterator i = mapbuf.begin();
    for ( ; i != mapbuf.end(); ++i) {
        if (!i->second.has_rebound() || i->second.rebound().rebound_count() == 0) {
            continue;
        }

        i->second.mutable_rebound()->set_rebound_count(i->second.rebound().rebound_count() - 1);
        rebound += (uint32)(damage * (i->second.rebound().rebound_proportion() / 10000.0f));
    }
}

int role_buffer::update(uint32 tick, uint32& effect_count)
{
    effect_count = 0;
    std::multimap<uint32, role_buffer_info>::iterator it = mapbuf.begin();
    _attr.clear();
    _status.clear();
    _skills.clear();
    _immune.clear();
    _taunt.Clear();
    _seckill.Clear();
    _change = false;
    trigger(false);
    while (it != mapbuf.end()) {
        ++effect_count;
        role_buffer_info& rbi = it->second;
        if (rbi.get_operate_flag() == client::BLME_DELETE) {
            rbi.set_operate_flag(client::BLME_DELETE);
            change_buffer.push_back(rbi);
            mapbuf.erase(it++);
            trigger(true);
            continue;
        }

        on_timer_buffer(tick, rbi);
        ++it;
    }

    _change = (effect_count != 0);

    return 0;
}

int role_buffer::on_timer_buffer(uint32 tick, role_buffer_info& rbi)
{
    if (rbi.remain_tick() == 0) {
        return on_timeout(tick, rbi);
    }

    uint32 time_interval = rbi.interval();
    if (rbi.effect_tick() > tick ) {
        rbi.set_effect_tick(tick);
    }

    if (tick - rbi.effect_tick() <= time_interval) {
        return 0;
    }

    if (rbi.effect_tick() == 0) {
        rbi.set_effect_tick(tick);
    }

    time_interval = tick - rbi.effect_tick();
    if (rbi.remain_tick() > time_interval) {
        rbi.set_remain_tick(rbi.remain_tick() - time_interval);
    } else {
        rbi.set_remain_tick(0);
    }

    rbi.set_effect_tick(tick);

    // 已经到达生效次数上限
    if (rbi.effected_times() >= rbi.max_effect_times()) {
        return 0;
    }

    rbi.set_effected_times(rbi.effected_times() + 1);
    rbi.set_operate_flag(client::BLME_UPDATE);
    //change_buffer.push_back(rbi);

    _change = true;
    on_buffer_trigger(rbi);
    trigger(true);

    return 0;
}

int role_buffer::on_timeout(uint32 tick, role_buffer_info& rbi)
{
    rbi.set_operate_flag(client::BLME_DELETE);
    return 0;
}

int role_buffer::add_buf_exist(role_buffer_info& src_buf, const config_effect* effect)
{
    if (effect == NULL) {
        return -1;
    }

    switch (effect->get_repeat_type()) {
    case ERT_ADD: add_buf(effect, src_buf); break;
    case ERT_REP: rep_buf(effect); break;
    case ERT_NAN: return 0; break;
    case ERT_CLS: del_buf(effect); break;
    case ERT_ADD_T: add_buf_time(effect, src_buf); break;
    case ERT_ADD_V: add_buf_value(effect, src_buf); break;
    }

    return 0;
}

int role_buffer::add_buf_new(const config_effect* effect, client::role_data* caster)
{
    if (mapbuf.size() >= MAX_ROLE_BUF_NUM) {
        return -1;
    }

    role_buffer_info rbi;
    if (caster != NULL) {
        rbi.mutable_caster()->CopyFrom(*caster);
    }

    set_buf(effect, rbi);
    change_buffer.push_back(rbi);
    mapbuf.insert(std::pair<uint32, role_buffer_info>(effect->get_effect_id(), rbi));

    return 0;
}

int role_buffer::set_buf(const config_effect* effect, role_buffer_info& rbi)
{
    if (effect == NULL) {
        return -1;
    }

    rbi.set_config_effect    (effect);
    rbi.set_effect_id        (effect->get_effect_id());
    rbi.set_repeat_levels    (effect->get_repeat_levels());
    rbi.set_cur_stacks       (1);
    rbi.set_duration         (effect->get_duration_tick());
    rbi.set_duration         (rbi.duration() == (uint32)-1 ? rbi.duration() : rbi.duration() / 10);
    rbi.set_interval         (effect->get_interval_tick() / 10);
    rbi.set_effect_tick      (0);
    rbi.set_remain_tick      (rbi.duration());
    rbi.set_max_effect_times (rbi.duration() / (rbi.interval() > 0 ? rbi.interval() : 1));
    rbi.set_effected_times   (0);

    set_attr(effect, rbi);

    return 0;
}

bool role_buffer::check_immune(const config_effect* effect)
{
    if (effect == NULL) {
        return false;
    }

    for (size_t i = 0; i < _immune.size(); ++i) {
        const std::vector<int>& ids = effect->get_effect_type_id();
        if (std::find(ids.begin(), ids.end(), _immune[i].type()) != ids.end()) {
            if (_immune[i].proportion() < effect->get_buff_probability()) {
                return false;
            }

            if ((_immune[i].proportion() - effect->get_buff_probability()) > mtrandom::rand_int32(TEN_THOUSAND)) {
                return true;
            }
        }
    }

    return false;
}

void role_buffer::set_attr(const config_effect* effect, role_buffer_info& rbi)
{
    buf_set_attr(effect, rbi);
    buf_set_status(effect, rbi);
    buf_set_shiled(effect, rbi);
    buf_set_rebound(effect, rbi);
    buf_set_skills(effect, rbi);
    buf_set_immune(effect, rbi);
    buf_set_taunt(effect, rbi);
    buf_set_seckill(effect, rbi);
}

void role_buffer::buf_set_attr(const config_effect* effect, role_buffer_info& rbi)
{
    if (effect->get_affect_hp().is_valid()) {
        client::buffer_effect_val* bev = rbi.add_attr_val();
        bev->set_type(effect->get_affect_hp().type);
        bev->set_val((uint32)((float)effect->get_affect_hp().molecular / effect->get_affect_hp().denominator * 10000));
        bev->set_operate(client::buffer_effect_val_OP_MULTI);
        bev->set_max_val(effect->get_affect_hp().threshold);
        bev->set_min_val(0);
    }

    if (effect->get_affect_mp().is_valid()) {
        client::buffer_effect_val* bev = rbi.add_attr_val();
        bev->set_type(effect->get_affect_mp().type);
        bev->set_val((uint32)((float)effect->get_affect_mp().molecular / effect->get_affect_mp().denominator * 10000));
        bev->set_operate(client::buffer_effect_val_OP_MULTI);
        bev->set_max_val(effect->get_affect_mp().threshold);
        bev->set_min_val(0);
    }

    const std::map<int, cpf_val_2>& al = effect->get_affect_list();
    buf_set_attr(al, rbi);
}

void role_buffer::buf_set_attr(const std::map<int, cpf_val_2>& al, role_buffer_info& rbi)
{
    std::map<int, cpf_val_2>::const_iterator i = al.begin();
    while (i != al.end()) {
        client::buffer_effect_val* bev = rbi.add_attr_val();
        bev->set_type(ATTR_CFG_TO_CODE[i->second.type]);
        if (i->second.denominator != 0) {
            bev->set_val((int32)(((float)i->second.molecular / (float)i->second.denominator * 10000.0f)));
            bev->set_operate(client::buffer_effect_val_OP_MULTI);
            bev->set_max_val(i->second.threshold);
            bev->set_min_val(0);
        } else {
            bev->set_val(i->second.molecular);
            bev->set_operate(client::buffer_effect_val_OP_ADD);
        }
        ++i;
    }
}

#define BUFFER_ADD_STATUS(X, T) if (status_type & X) {          \
    client::buffer_effect_val* bev = rbi.add_status_val();  \
    bev->set_type(T);                                       \
    bev->set_val(1);                                        \
}

void role_buffer::buf_set_status(const config_effect* effect, role_buffer_info& rbi)
{
    uint32 status_type = effect->get_fl();
    if (status_type & FT_MOVE && status_type & FT_SKILL) {
        client::buffer_effect_val* bev = rbi.add_status_val();
        bev->set_type(client::RSE_STATUS_FAINT);
        bev->set_val(1);
    }

    BUFFER_ADD_STATUS(FT_MOVE     , client::RSE_STATUS_GIDDY   );
    BUFFER_ADD_STATUS(FT_SKILL    , client::RSE_STATUS_SILENT  );
    BUFFER_ADD_STATUS(FT_PK       , client::RSE_STATUS_FIGHT   );
    BUFFER_ADD_STATUS(FT_EQUIP_ATK, client::RSE_STATUS_DISARM  );
    BUFFER_ADD_STATUS(FT_EQUIP_DEF, client::RSE_STATUS_SLOWLY  );
    BUFFER_ADD_STATUS(FT_BLIND    , client::RSE_STATUS_BLIND   );
    BUFFER_ADD_STATUS(FT_ATK      , client::RSE_STATUS_MAIMED  );
    BUFFER_ADD_STATUS(FT_DEF      , client::RSE_STATUS_INJURY  );
    BUFFER_ADD_STATUS(FT_MP       , client::RSE_STATUS_CURSED  );
    BUFFER_ADD_STATUS(FT_RAND_MOVE, client::RSE_STATUS_FIRED   );
    BUFFER_ADD_STATUS(FT_WEEK     , client::RSE_STATUS_WEEK    );
    BUFFER_ADD_STATUS(FT_HURTED   , client::RSE_STATUS_HURTED  );
    BUFFER_ADD_STATUS(FT_DEC_HURM , client::RSE_STATUS_DEC_HARM);
}

void role_buffer::buf_set_shiled(const config_effect* effect, role_buffer_info& rbi)
{
    if (effect->get_durable_max() <= 0) {
        return ;
    }

    client::buffer_effect_shield* bes = rbi.mutable_shield();
    bes->set_effect_id(effect->get_effect_id());
    bes->set_durable_max(effect->get_durable_max());
    if (effect->get_each_bear_harm().denominator != 0) {
        bes->mutable_ebh()->set_molecular(effect->get_each_bear_harm().molecular);
        bes->mutable_ebh()->set_denominator(effect->get_each_bear_harm().denominator);
    } else {
        bes->mutable_ebh()->set_val(effect->get_each_bear_harm().molecular);
    }

    bes->set_over_back_proportion(effect->get_over_back_proportion() / 10000.0f);
    bes->set_over_back_max(effect->get_over_back_max());
}

void role_buffer::buf_set_rebound(const config_effect* effect, role_buffer_info& rbi)
{
    if (effect->get_rebound_count() <= 0) {
        return ;
    }

    client::rebound_damage* rd = rbi.mutable_rebound();
    rd->set_rebound_count(rd->rebound_count() + effect->get_rebound_count());
    rd->set_rebound_proportion(effect->get_rebound_proportion());
}

void role_buffer::buf_set_skills(const config_effect* effect, role_buffer_info& rbi)
{
    if (effect->get_next_trigger_skill_id() == 0) {
        return ;
    }

    client::trigger_skill* ts = rbi.add_skills();
    ts->set_skill_id(effect->get_next_trigger_skill_id());
}

void role_buffer::buf_set_immune(const config_effect* effect, role_buffer_info& rbi)
{
    const std::vector<int>& immune_types = effect->get_immunity_buffer();
    for (size_t i = 0; i < immune_types.size(); ++i) {
        client::immune_effect* pie = rbi.add_immune();                     
        pie->set_type(immune_types[i]);
        pie->set_proportion(effect->get_immunity_probability());
    }
}

void role_buffer::buf_set_taunt(const config_effect* effect, role_buffer_info& rbi)
{
    if (!effect->is_taunt()) {
        return ;
    }

    if (rbi.has_caster()) {
        rbi.mutable_taunt()->set_inst_id(rbi.caster().inst_id());
        rbi.mutable_taunt()->set_ms(effect->get_taunt_time());
    }
}

void role_buffer::buf_set_seckill(const config_effect* effect, role_buffer_info& rbi)
{
    if (!effect->is_seckill()) {
        return ;
    }

    const std::map<int, cpf_val_2>& al = effect->get_seckill_hp();
    std::map<int, cpf_val_2>::const_iterator i = al.begin();
    while (i != al.end()) {
        client::buffer_effect_val* bev = rbi.mutable_seckill();
        bev->set_type(ATTR_CFG_TO_CODE[i->second.type]);
        if (i->second.denominator != 0) {
            bev->set_operate(client::buffer_effect_val_OP_MULTI);
            int32 val = (int32)((float)i->second.molecular / i->second.denominator * 10000);
            if (val > bev->val()) {
                bev->set_val(val);
            }

            bev->set_min_val(0);
            if (i->second.threshold > bev->max_val()) {
                bev->set_max_val(i->second.threshold);
            }
        } else {
            bev->set_val(i->second.molecular);
            bev->set_operate(client::buffer_effect_val_OP_ADD);
        }
        ++i;
    }
}

uint32 role_buffer::fill_all_buffer_list(client::buffer_list& bl)
{
    change_buffer.clear();

    std::multimap<uint32, role_buffer_info>::const_iterator it;
    for (it = mapbuf.begin(); it != mapbuf.end(); ++it) {
        client::buffer_operate_info* poi = bl.add_boi();
        poi->set_method(it->second.get_operate_flag());
        fill_buffer_info(it->second, *poi->mutable_bi());
    }

    return mapbuf.size();
}

uint32 role_buffer::fill_change_buffer_list(client::buffer_list& bl)
{
    uint32 size = change_buffer.size();
    if (size == 0) {
        return 0;
    }

    for (uint32 i = 0; i < size; ++i) {
        client::buffer_operate_info* poi = bl.add_boi();
        poi->set_method(change_buffer[i].get_operate_flag());
        fill_buffer_info(change_buffer[i], *poi->mutable_bi());
    }

    change_buffer.clear();
    return size;
}

void role_buffer::fill_buffer_info(const role_buffer_info& src, client::buffer_info& bi)
{
    bi.CopyFrom(src);
}

int role_buffer::on_buffer_trigger(role_buffer_info& rbi)
{
    for (int32 i = 0; i < rbi.status_val_size(); ++i) {
        _status.push_back(rbi.status_val(i));
    }

    if (rbi.get_config_effect() != NULL) {
        const std::map<int, cpf_val_2>& al = rbi.get_config_effect()->get_effect_correction_i();
        buf_set_attr(al, rbi);
    }

    for (int32 i = 0; i < rbi.attr_val_size(); ++i) {
        _attr.push_back(rbi.attr_val(i));
    }

    for (int32 i = 0; i < rbi.skills_size(); ++i) {
        _skills.push_back(rbi.skills(i));
    }

    for (int32 i = 0; i < rbi.immune_size(); ++i) {
        _immune.push_back(rbi.immune(i));
    }

    while (rbi.has_taunt()) {
        //if (rbi.taunt().ms() > _taunt.ms()) {
        _taunt.CopyFrom(rbi.taunt());
        //}
        break;
    }

    while (rbi.has_seckill()) {
        if (_seckill.val() == 0) {
            _seckill.CopyFrom(rbi.seckill());
            break;
        }

        if (rbi.seckill().val() > _seckill.val()) {
            _seckill.set_val(rbi.seckill().val());
        }

        if (rbi.seckill().max_val() > _seckill.max_val()) {
            _seckill.set_max_val(rbi.seckill().max_val());
        }

        break;
    }

    return 0;
}

uint32 role_buffer::shield_each_bear_harm(client::each_bear_harm *ebh, uint32 damage)
{
    if (ebh->has_molecular() && ebh->has_denominator()) {
        float r = (float)ebh->molecular() / ebh->denominator();
        if (r >= 1.0f) {
            return damage;
        }

        return (uint32)(damage * r);
    } else if (ebh->has_val()) {
        return min(damage, (uint32)ebh->val());
    }

    return damage;
}

//void role_buffer::get_hal os(uint32 distance, std::vector<const config_effect*>& halos)
//{
//    multimap<uint32, role_buffer_info>::iterator begin = mapbuf.begin();
//    multimap<uint32, role_buffer_info>::iterator end   = mapbuf.end();
//    for ( ; begin != end; ++begin) {
//        if (!begin->second.has_halo() || begin->second.halo().radius() < distance) {
//            continue;
//        }
//
//        halos.insert(
//            halos.end(), 
//            begin->second.get_config_effect()->get_halo_effect().begin(),
//            begin->second.get_config_effect()->get_halo_effect().end());
//    }
//}

