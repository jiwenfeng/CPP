#include "config_npc.h"
#include "scene_role_ai.h"

scene_role_ai::scene_role_ai()
{
}

scene_role_ai::~scene_role_ai()
{
}

void scene_role_ai::set_ai(const string &a)
{
	_ai.Clear();
	server::ai_data *ad = _ai.add_ai();

    for (size_t i = 0; i < a.size(); ++i) {
        switch (a[i]) {
        case '#': {
            i = add_method(a, i, ad);
            break;
        }
        case '@': {
            i = add_timer(a, i, ad);
            break;
        }
        case '*': {
            i = add_alarm(a, i, ad);
            break;
        }
        case '$': {
            i = add_attr(CUR_HP, server::ACR_LESS, a, i, ad);
            break;
        }
            /*case '*': {
            i = add_attr(CUR_HP, server::ACR_EQUAL, a, i);
            break;
            }*/
        case '%': {
            i = add_distance(a, i, ad);
            break;
        }
        case '&': {
            i = add_attack(a, i, ad);
            break;
        }
        case '!': {
            i = add_defense(a, i, ad);
            break;
        }
        case '~': {
            i = add_dead(a, i, ad);
            break;
        }
        default: break;
        }

        if (a.size() <= i || a[i] == ';') {
            ad = _ai.add_ai();
        }
    }
}

void scene_role_ai::init(const config_npc* cn)
{
    const string& a = cn->get_monster_ai();
	set_ai(a);
#if 0
    server::ai_data* ad = _ai.add_ai();

    //const uint32  ID0 = 140711;
    //const uint32  ID1 = 140711;
    //if (cn->get_npc_id() == ID0 || cn->get_npc_id() == ID1/* || cn->get_npc_id() == 161011*/) {
    //    bool breakpoint = false;
    //    breakpoint = true;
    //}

    for (size_t i = 0; i < a.size(); ++i) {
        switch (a[i]) {
        case '#': {
            i = add_method(a, i, ad);
            break;
        }
        case '@': {
            i = add_timer(a, i, ad);
            break;
        }
        case '*': {
            i = add_alarm(a, i, ad);
            break;
        }
        case '$': {
            i = add_attr(CUR_HP, server::ACR_LESS, a, i, ad);
            break;
        }
            /*case '*': {
            i = add_attr(CUR_HP, server::ACR_EQUAL, a, i);
            break;
            }*/
        case '%': {
            i = add_distance(a, i, ad);
            break;
        }
        case '&': {
            i = add_attack(a, i, ad);
            break;
        }
        case '!': {
            i = add_defense(a, i, ad);
            break;
        }
        case '~': {
            i = add_dead(a, i, ad);
            break;
        }
        default: break;
        }

        if (a.size() <= i || a[i] == ';') {
            ad = _ai.add_ai();
        }
    }

    //if (cn->get_npc_id() == ID0 || cn->get_npc_id() == ID1/* || cn->get_npc_id() == 161011*/) {
    //    amtsvr_log("\n-------\nnpc %u\n %s\n-------\n", cn->get_npc_id(), _ai.DebugString().c_str());
    //}
#endif
}

bool scene_role_ai::check_timer(const server::ai_data& ad, uint32& timer, uint32& alarm) const
{
    for (int32 i = 0; i < ad.timer_size(); ++i) {
        if (ad.timer(i).has_timer()) {
            if (timer < ad.timer(i).timer()) {
                return false;
            }

            timer = 0;
        }

        if (ad.timer(i).has_alarm()) {
            if (alarm == 0 || alarm < ad.timer(i).alarm()) {
                return false;
            }

            alarm = 0;
        }
    }

    return true;
}

bool scene_role_ai::check_attr(const server::ai_data& ad, const scene_role_attr& attr) const
{
    for (int32 i = 0; i < ad.attr_size(); ++i) {
        if (ad.attr(i).attr() >= GAME_ATTRIBUTE_MAX) {
            continue;
        }

        ROLE_ATTRIBUTE index = (ROLE_ATTRIBUTE)ad.attr(i).attr();

        if (ad.attr(i).has_less() && ad.attr(i).less() < attr.get_val(index)) {
            return false;
        }

        if (ad.attr(i).has_greater() && ad.attr(i).greater() > attr.get_val(index)) {
            return false;
        }

        if (ad.attr(i).has_equal() && ad.attr(i).equal() != attr.get_val(index)) {
            return false;
        }
    }

    return true;
}

bool scene_role_ai::check_distance(const server::ai_data& ad, const map_postion& npc, const map_postion& target) const
{
    for (int32 i = 0; i < ad.distance_size(); ++i) {
        if (ad.distance(i).distance() < npc.get_distance(target) || npc.get_distance(target) < ad.distance(i).limit()) {
            return false;
        }
    }
    
    return true;
}

bool scene_role_ai::check_attack(const server::ai_data& ad, uint32& attack_count) const
{
    for (int32 i = 0; i < ad.attack_size(); ++i) {
        if (attack_count == 0 || (ad.attack(i).attack() != 0 && attack_count % ad.attack(i).attack() != 0)) {
            return false;
        } else {
            ++attack_count;
        }
    }

    return true;
}

bool scene_role_ai::check_defense(const server::ai_data& ad, uint32& defense_count) const
{
    for (int32 i = 0; i < ad.defense_size(); ++i) {
        if (defense_count == 0 || (ad.defense(i).defense() != 0 && defense_count % ad.defense(i).defense() != 0)) {
            return false;
        } else {
            ++defense_count;
        }
    }
    
    return true;
}

#define TO_VAL(STR)     ++i;                                    \
    for (; i < a.size() && a[i] != '_' && a[i] != ';' && a[i] != ','; ++i) {   \
        STR += a[i];                                            \
    }

#define TO_TEXT(STR) if (i < a.size() - 1 && (a[i + 1] == '\"' && a[i] == '_')) {  \
    ++i;                                                                           \
    ++i;                                                                           \
    for ( ; i < a.size() - 1 && (a[i + 1] != '_' || a[i] != '\"'); ++i) {          \
    msg += a[i];                                                                   \
    } ++i; }

size_t scene_role_ai::add_move(const string& a, size_t i, server::ai_data* ad /* = NULL */, uint32 rand /* = 100 */)
{
    std::string t, x, y;
    server::ai_move* am = ad->add_move();
    am->set_rand(rand);
    for (++i; i < a.size(); ++i) {
        if ((i + 1 == a.size()) && a[i] != '_' && a[i] != ';') {
            t += a[i];
        }
        
        if (a[i] == '_' || (i + 1 == a.size()) || a[i] == ';') {
            if (x.empty()) {
                x = t;
            } else if (y.empty()) {
                y = t;
            }

            if (!x.empty() && !y.empty()) {
                am->add_x(atoi(x.c_str()));
                am->add_y(atoi(y.c_str()));
                x.clear();
                y.clear();
            }

            t.clear();

            if (a[i] == ';') {
                break;
            }

            continue;
        }

        t += a[i];
    }
    
    return add_msg(a, i, ad);
}

size_t scene_role_ai::add_fix_move(const string& a, size_t i, server::ai_data* ad /* = NULL */, uint32 rand /* = 100 */)
{
    std::string grid;
    TO_VAL(grid);
    server::ai_fix_move* afm = ad->add_fix_move();
    afm->add_grid(atoi(grid.c_str()));
    afm->set_rand(rand);

    return add_msg(a, i, ad);
}

size_t scene_role_ai::add_force_move(const string& a, size_t i, server::ai_data* ad /* = NULL */)
{
    std::string t, x, y;
    for (++i; i < a.size(); ++i) {
        if ((i + 1 == a.size()) && a[i] != '_' && a[i] != ';') {
            t += a[i];
        }
        
        if (a[i] == '_' || (i + 1 == a.size()) || a[i] == ';') {
            if (x.empty()) {
                x = t;
            } else if (y.empty()) {
                y = t;
            }

            if (!x.empty() && !y.empty()) {
                server::ai_move* am = ad->add_force_move();
                am->add_x(atoi(x.c_str()));
                am->add_y(atoi(y.c_str()));
                x.clear();
                y.clear();
            }

            t.clear();
            continue;
        }

        if (a[i] == ';') {
            break;
        }

        t += a[i];
    }

    return add_msg(a, i, ad);
}

size_t scene_role_ai::add_attack_target(const string& a, size_t i, server::ai_data* ad /*= NULL*/)
{
    std::string id;
    TO_VAL(id);
    server::ai_attack* aa = ad->add_target();
    aa->set_attack(atoi(id.c_str()));

    return add_msg(a, i, ad);
}

size_t scene_role_ai::add_method(const string& a, size_t i, server::ai_data* ad /* = NULL */)
{
    std::string rand, method, skill_id, target, count;
    TO_VAL(rand);
    TO_VAL(method);
    uint32 sid = 0;
    if (method == "dingdianMove") {
        return add_move(a, i, ad, atoi(rand.c_str()));
    } else if (method == "xiuzheng") {
        return add_fix_move(a, i, ad, atoi(rand.c_str()));
    } else if (method == "qiangzhiMove") {
        return add_force_move(a, i, ad);
    } else if (method == "setattackTarget") {
        return add_attack_target(a, i, ad);
    } else if (method == "stop") {
        return add_stop(a, i, ad, atoi(rand.c_str()));
    } else if (method == "hide") {
        return add_hide(a, i, ad, atoi(rand.c_str()));
    } else if (method == "setMaster") {
        return add_master(a, i, ad, atoi(rand.c_str()));
    } else {
        skill_id = method;
        sid = atoi(skill_id.c_str());
        if (sid == 0) {
            return i;
        }
    }

    TO_VAL(target);
    TO_VAL(count);

    if (ad == NULL) {
        ad = _ai.add_ai();
    }

    server::ai_method* am = ad->add_method();

    am->set_rand(atoi(rand.c_str()));
    am->set_skill_id(sid);
    am->set_target(atoi(target.c_str()));
    if (count.empty()) {
        am->set_count(0);
    } else {
        am->set_count(atoi(count.c_str()));
    }

    return add_msg(a, i, ad);
}

size_t scene_role_ai::add_timer(const string& a, size_t i, server::ai_data* ad /* = NULL */)
{
    string timer;
    TO_VAL(timer);
    if (ad == NULL) {
        ad = _ai.add_ai();
    }

    server::ai_timer* at = ad->add_timer();
    at->set_timer(atoi(timer.c_str()));
    return add_method(a, i, ad);
}

size_t scene_role_ai::add_alarm(const string& a, size_t i, server::ai_data* ad /* = NULL */)
{
    string alarm;
    TO_VAL(alarm);
    if (ad == NULL) {
        ad = _ai.add_ai();
    }

    server::ai_timer* at = ad->add_timer();
    at->set_alarm(atoi(alarm.c_str()));
    return add_method(a, i, ad);
}

size_t scene_role_ai::add_attr(uint32 type, server::AI_COMPARE_RULE acr, const string& a, size_t i, server::ai_data* ad /* = NULL */)
{
    int t = 0;
    if ((i + 1) < a.size()) {
        switch (a[i + 1]) {
        case '@': ++i; t = 1; break;
        case '*': ++i; t = 2; break;
        case '&': ++i; t = 3; break;
        case '!': ++i; t = 4; break;
        default: break;
        }
    }

    string val;
    TO_VAL(val);
    if (ad == NULL) {
        ad = _ai.add_ai();
    }

    server::ai_attr* aa = ad->add_attr();
    aa->set_attr(type);
    switch (acr) {
    case server::ACR_LESS: {
        aa->set_less(atoi(val.c_str()));
        break;
    }
    case server::ACR_GREATER: {
        aa->set_greater(atoi(val.c_str()));
        break;
    }
    case server::ACR_EQUAL: {
        aa->set_equal(atoi(val.c_str()));
        break;
    }
    default: break;
    }
    
    switch (t) {
    case 1: return add_timer  (a, i, ad);
    case 2: return add_alarm  (a, i, ad);
    case 3: return add_attack (a, i, ad);
    case 4: return add_defense(a, i, ad);
    default: break;
    }

    return add_method(a, i, ad);
}

size_t scene_role_ai::add_distance(const string& a, size_t i, server::ai_data* ad /* = NULL */)
{
    string val, limit;
    TO_VAL(limit);
    TO_VAL(val);
    if (ad == NULL) {
        ad = _ai.add_ai();
    }

    server::ai_distance* distance = ad->add_distance();
    distance->set_distance(atoi(val.c_str()));
    distance->set_limit   (atoi(limit.c_str()));

    return add_method(a, i, ad);
}

size_t scene_role_ai::add_attack(const string& a, size_t i, server::ai_data* ad /* = NULL */)
{
    string val;
    TO_VAL(val);
    if (ad == NULL) {
        ad = _ai.add_ai();
    }

    server::ai_attack* attack = ad->add_attack();
    attack->set_attack(atoi(val.c_str()));

    return add_method(a, i, ad);
}

size_t scene_role_ai::add_defense(const string& a, size_t i, server::ai_data* ad /* = NULL */)
{
    string val;
    TO_VAL(val);
    if (ad == NULL) {
        ad = _ai.add_ai();
    }

    server::ai_defense* defense = ad->add_defense();
    defense->set_defense(atoi(val.c_str()));

    return add_method(a, i, ad);
}

size_t scene_role_ai::add_dead(const string& a, size_t i, server::ai_data* ad /*= NULL*/)
{
    if (ad == NULL) {
        ad = _ai.add_ai();
    }

    server::ai_dead* dead = ad->mutable_dead();
    dead->set_dead(true);

    return add_method(a, i, ad);
}

size_t scene_role_ai::add_stop(const string& a, size_t i, server::ai_data* ad /*= NULL*/, uint32 rand /* = 100 */)
{
    string val;
    TO_VAL(val);
    if (ad == NULL) {
        ad = _ai.add_ai();
    }

    server::ai_stop* stop = ad->mutable_stop();
    stop->set_time(atoi(val.c_str()));
    stop->set_rand(rand);

    return add_method(a, i, ad);
}

size_t scene_role_ai::add_hide(const string& a, size_t i, server::ai_data* ad /* = NULL */, uint32 rand /* = 100 */)
{
    string val, pet;
    TO_VAL(val);
    TO_VAL(pet);
    if (ad == NULL) {
        ad = _ai.add_ai();
    }

    server::ai_hide* hide = ad->mutable_hide();
    hide->set_time(atoi(val.c_str()));
    hide->set_rand(rand);
    hide->set_pet(atoi(pet.c_str()));

    return add_method(a, i, ad);
}

size_t scene_role_ai::add_master(const string& a, size_t i, server::ai_data* ad /* = NULL */, uint32 rand /* = 100 */)
{
    string type, time;
    TO_VAL(type);
    TO_VAL(time);
    if (ad == NULL) {
        ad = _ai.add_ai();
    }

    server::ai_master* master = ad->mutable_master();
    master->set_type(atoi(type.c_str()));
    master->set_time(atoi(time.c_str()));
    master->set_rand(rand);

    return add_method(a, i, ad);
}

size_t scene_role_ai::add_msg(const string& a, size_t i, server::ai_data* ad /*= NULL*/)
{
    if (ad == NULL) {
        return i;
    }

    std::string msg;
    //TO_TEXT(msg);

    if (i < a.size() - 1 && (a[i + 1] == '\"' && a[i] == '_')) {
        ++i;
        ++i;
        for ( ; i < a.size() - 1 && (a[i + 1] != '_' || a[i] != '\"'); ++i) {
            msg += a[i];
        }

        ++i;
    }

    if (!msg.empty()) {
        std::string show, delay;
        TO_VAL(show)
        TO_VAL(delay);

        server::ai_msg* amsg = ad->mutable_msg();
        amsg->set_text(msg);
        amsg->set_delay(atoi(delay.c_str()));
        amsg->set_show(atoi(show.c_str()));
        amsg->set_send(false);
    }

    return i;
}
