#include "config_effect.h"
#include "scene.h"
#include "game_map_mgr.h"
#include "scene_user.h"
#include "scene_npc.h"
#include "scene_inst.h"
#include "create_npc_rule.h"

genterate_create_npc_rule* genterate_create_npc_rule::instance = NULL;

genterate_create_npc_rule::genterate_create_npc_rule()
{
    _rules.insert(std::pair<std::string, func>("AdvanceAppeare", genterate_create_npc_rule::advance));
    _rules.insert(std::pair<std::string, func>("IntoAppeare", genterate_create_npc_rule::into));
    _rules.insert(std::pair<std::string, func>("AfterStory", genterate_create_npc_rule::story));
    _rules.insert(std::pair<std::string, func>("KillMonster", genterate_create_npc_rule::kill));
    _rules.insert(std::pair<std::string, func>("TouchTask", genterate_create_npc_rule::task));
    _rules.insert(std::pair<std::string, func>("StartActivity", genterate_create_npc_rule::activity));
    _rules.insert(std::pair<std::string, func>("StartTime", genterate_create_npc_rule::time));
    _rules.insert(std::pair<std::string, func>("GetItem", genterate_create_npc_rule::item));
    _rules.insert(std::pair<std::string, func>("TouchScenseObject", genterate_create_npc_rule::object));
    _rules.insert(std::pair<std::string, func>("FindMonster", genterate_create_npc_rule::find));
    _rules.insert(std::pair<std::string, func>("GroupMonster", genterate_create_npc_rule::group));
    _rules.insert(std::pair<std::string, func>("AppeareView", genterate_create_npc_rule::appreare_view));
    _rules.insert(std::pair<std::string, func>("AppeareAround", genterate_create_npc_rule::appear_around));
    _rules.insert(std::pair<std::string, func>("RefreshTime", genterate_create_npc_rule::refresh_time));
    _rules.insert(std::pair<std::string, func>("Next", genterate_create_npc_rule::next));
    _rules.insert(std::pair<std::string, func>("Clear", genterate_create_npc_rule::Clear));
    _rules.insert(std::pair<std::string, func>("Index", genterate_create_npc_rule::Index));
    _rules.insert(std::pair<std::string, func>("Msg", genterate_create_npc_rule::Msg));
    _rules.insert(std::pair<std::string, func>("suspend", genterate_create_npc_rule::suspend));

    _rules_effect.insert(std::pair<std::string, func_effect>("IntoAppeare", genterate_create_npc_rule::into_effect));
    _rules_effect.insert(std::pair<std::string, func_effect>("StartTime"  , genterate_create_npc_rule::time_effect));
}

int genterate_create_npc_rule::genterate(const std::string& str, server::create_npc_rules& rules)
{
    size_t start = 0;
    server::create_npc_rule* pr = rules.add_rules();
    for (size_t i = 0; i < str.length(); ++i) {
        if (str[i] == ';' && i + 1 != str.length()) {
            pr = rules.add_rules();
            start = i + 1;
            continue;
        } else if (str[i] == '_' || str[i] == '\n') {
            start = i + 1;
            continue;
        } else if (str[i] == '(') {
            std::string name = str.substr(start, i - start);
            std::map<std::string, func>::iterator it = _rules.find(name);
            if (it == _rules.end()) {
                continue;
            }

            it->second(str, i, *pr);
        }
    }

    return 0;
}

int genterate_create_npc_rule::genterate_effect(const std::string& str, server::create_effect_rules& rules)
{
    size_t start = 0;
    server::create_effect_rule* pr = rules.add_rules();
    for (size_t i = 0; i < str.length(); ++i) {
        if (str[i] == ';' && i + 1 != str.length()) {
            pr = rules.add_rules();
            start = i + 1;
            continue;
        } else if (str[i] == '_') {
            start = i + 1;
            continue;
        } else if (str[i] == '(') {
            std::string name = str.substr(start, i - start);
            std::map<std::string, func_effect>::iterator it = _rules_effect.find(name);
            if (it == _rules_effect.end()) {
                continue;
            }

            it->second(str, i, *pr);
        }
    }

    return 0;
}

#define TO_VAL(STR)     ++i;                                            \
    for (; i < a.size() && a[i] != '_' && a[i] != ';' && a[i] != ',' && a[i] != ')'; ++i) { \
    if (a[i] == '\\') {     \
    ++i;                    \
    STR += a[i];            \
    continue;               \
    }                      \
    STR += a[i];                                                    \
    }

int genterate_create_npc_rule::advance(const std::string& a, size_t i, server::create_npc_rule& rule)
{
    std::string x, y;
    TO_VAL(x);
    TO_VAL(y);
    server::advance_appeare* p = rule.add_advance();
    p->set_x(atoi(x.c_str()));
    p->set_y(atoi(y.c_str()));
    return 0;
}


int genterate_create_npc_rule::into(const std::string& a, size_t i, server::create_npc_rule& rule)
{
    std::string x, y, distance, seconds, role;
    TO_VAL(x);
    TO_VAL(y);
    TO_VAL(distance);
    TO_VAL(seconds);
    TO_VAL(role);

    server::into_appeare* p = rule.add_into();
    p->set_x(atoi(x.c_str()));
    p->set_y(atoi(y.c_str()));
    p->set_distance(atoi(distance.c_str()));

    if (!seconds.empty()) {
        p->set_seconds(atoi(seconds.c_str()));
    }

    if (!role.empty()) {
        p->set_role(atoi(role.c_str()));
    }

    return 0;
}

int genterate_create_npc_rule::story(const std::string& a, size_t i, server::create_npc_rule& rule)
{
    std::string id, tick;
    TO_VAL(id);
    TO_VAL(tick);

    server::after_story* p = rule.add_story();
    p->set_id(atoi(id.c_str()));
    p->set_tick(atoi(tick.c_str()));

    return 0;
}

int genterate_create_npc_rule::kill(const std::string& a, size_t i, server::create_npc_rule& rule)
{
    std::string monster_id, number, seconds;
    TO_VAL(monster_id);
    TO_VAL(number);
    TO_VAL(seconds);

    server::kill_monster* p = rule.add_kill();
    p->set_monster_id(atoi(monster_id.c_str()));
    p->set_number    (atoi(number.c_str()));
    if (!seconds.empty()) {
        p->set_seconds(atoi(seconds.c_str()));
    }

    return 0;
}

int genterate_create_npc_rule::week(const std::string& a, size_t i, server::create_npc_rule& rule)
{
    std::string d, h, m, s, c;
    TO_VAL(d);
    TO_VAL(h);
    TO_VAL(m);
    TO_VAL(s);
    TO_VAL(c);

    server::start_time_week* p = rule.add_week();
    p->set_d(atoi(d.c_str()));
    p->mutable_t()->set_h(atoi(h.c_str()));
    p->mutable_t()->set_m(atoi(m.c_str()));
    p->mutable_t()->set_s(atoi(s.c_str()));
    p->mutable_t()->set_c(atoi(c.c_str()));

    return 0;
}

int genterate_create_npc_rule::day(const std::string& a, size_t i, server::create_npc_rule& rule)
{
    std::string h, m, s, c;
    TO_VAL(h);
    TO_VAL(m);
    TO_VAL(s);
    TO_VAL(c);

    server::start_time_week* p = rule.add_week();
    p->set_d(0);
    p->mutable_t()->set_h(atoi(h.c_str()));
    p->mutable_t()->set_m(atoi(m.c_str()));
    p->mutable_t()->set_s(atoi(s.c_str()));
    p->mutable_t()->set_c(atoi(c.c_str()));

    return 0;
}

int genterate_create_npc_rule::task(const std::string& a, size_t i, server::create_npc_rule& rule)
{
    return 0;
}

int genterate_create_npc_rule::activity(const std::string& a, size_t i, server::create_npc_rule& rule)
{
    return 0;
}

int genterate_create_npc_rule::time(const std::string& a, size_t i, server::create_npc_rule& rule)
{
    std::string start_time;
    TO_VAL(start_time);

    server::start_time* p = rule.add_time();
    p->set_time(atoi(start_time.c_str()));
    
    return 0;
}

int genterate_create_npc_rule::item(const std::string& a, size_t i, server::create_npc_rule& rule)
{
    std::string id, count, tick;
    TO_VAL(id);
    count = "1";
    TO_VAL(tick);

    server::get_item* p = rule.add_item();
    p->set_item_id(atoi(id.c_str()));
    p->set_count(atoi(count.c_str()));
    p->set_tick(atoi(tick.c_str()));

    return 0;
}

int genterate_create_npc_rule::object(const std::string& a, size_t i, server::create_npc_rule& rule)
{
    std::string id, count, tick;
    TO_VAL(id);
    count = "1";
    TO_VAL(tick);

    server::touch_scense_object* p = rule.add_object();
    p->set_object(atoi(id.c_str()));
    p->set_count(atoi(count.c_str()));
    p->set_tick(atoi(tick.c_str()));

    return 0;
}

int genterate_create_npc_rule::find(const std::string& a, size_t i, server::create_npc_rule& rule)
{
    std::string character_id, distance, seconds;
    TO_VAL(character_id);
    //TO_VAL(distance);
    TO_VAL(seconds);

    server::find_monster* p = rule.add_find();
    p->set_character_id(atoi(character_id.c_str()));
//     if (!distance.empty()) {
//         p->set_distance(atoi(distance.c_str()));
//     }
    if (!seconds.empty()) {
        p->set_seconds(atoi(seconds.c_str()));
    }

    return 0;
}

int genterate_create_npc_rule::group(const std::string& a, size_t i, server::create_npc_rule& rule)
{
    std::string monster_id, number, seconds;
    TO_VAL(monster_id);
    TO_VAL(number);
    TO_VAL(seconds);

    server::group_monster* p = rule.add_monster();
    p->set_monster_id(atoi(monster_id.c_str()));
    p->set_number    (atoi(number.c_str()));
    if (!seconds.empty()) {
        p->set_seconds(atoi(seconds.c_str()));
    }

    return 0;
}

int genterate_create_npc_rule::appreare_view(const std::string& a, size_t i, server::create_npc_rule& rule)
{
    return appreare(server::appeare_type_view, a, i, rule);
}

int genterate_create_npc_rule::appear_around(const std::string& a, size_t i, server::create_npc_rule& rule)
{
    std::string id, distance, tick;
    TO_VAL(id      );
    TO_VAL(distance);
    TO_VAL(tick    );

    server::appear_around* p = rule.add_around();
    p->set_id      (atoi(id      .c_str()));
    p->set_distance(atoi(distance.c_str()));
    p->set_tick    (atoi(tick    .c_str()));
    return 0;
}

int genterate_create_npc_rule::refresh_time(const std::string& a, size_t i, server::create_npc_rule& rule)
{
    std::string seconds;
    TO_VAL(seconds);
    if (rule.monster_size() == 0 || seconds.empty()) {
        return -1;
    }

    rule.mutable_monster(0)->set_seconds(atoi(seconds.c_str()));

    return 0;
}

int genterate_create_npc_rule::next(const std::string& a, size_t i, server::create_npc_rule& rule)
{
    return 0;
}

int genterate_create_npc_rule::Clear(const std::string& a, size_t i, server::create_npc_rule& rule)
{
    std::string clear;
    TO_VAL(clear);
    if (!clear.empty()) {
        rule.set_clear(atoi(clear.c_str()) != 0);
    }

    return 0;
}

int genterate_create_npc_rule::Index(const std::string& a, size_t i, server::create_npc_rule& rule)
{
    std::string index;
    TO_VAL(index);
    if (!index.empty()) {
        rule.set_index(atoi(index.c_str()));
    }

    return 0;
}

int genterate_create_npc_rule::Msg(const std::string& a, size_t i, server::create_npc_rule& rule)
{
    std::string msg, delay, show;
    TO_VAL(msg);
    TO_VAL(delay);
    TO_VAL(show);
    if (!msg.empty()) {
        server::create_msg* cm = rule.add_msg();
        cm->set_content(msg);
        cm->set_delay(atoi(delay.c_str()));
        cm->set_show(atoi(show.c_str()));
    }

    return 0;
}

int genterate_create_npc_rule::suspend(const std::string& a, size_t i, server::create_npc_rule& rule)
{
    std::string type, count;
    TO_VAL(type);
    TO_VAL(count);

    if (!type.empty()) {
        server::suspend_create* sc = rule.add_suspend();
        sc->set_type(atoi(type.c_str()));
        sc->set_count(atoi(count.c_str()));
    }

    return 0;
}

int genterate_create_npc_rule::appreare(server::appeare_type type, const std::string& a, size_t i, server::create_npc_rule& rule)
{
    string str;
    server::appeare* p = rule.add_app();
    p->set_t(type);
//    ++i;
//     for (; i < a.size(); ++i) {
//         if (a[i] == ',' || a[i] == ';' || a[i] == ')') {
//             p->add_effect(atoi(str.c_str()));
//             str.clear();
// 
//             if (a[i] == ';' || a[i] == ')') {
//                 break;
//             } else {
//                 continue;
//             }
//         } 
// 
//         str += a[i];
//     }

    ++i;
    std::string x1, y1, x2, y2, roll;
    for (; i < a.size(); ++i) {
        if (a[i] == '_' || a[i] == ',' || a[i] == ')') {
            if (x1.empty()) {
                x1 = str;
            } else if (y1.empty()) {
                y1 = str;
            } else if (x2.empty()) {
                x2 = str;
            } else if (y2.empty()) {
                y2 = str;
            } else if (roll.empty()) {
                roll = str;
            }

            str.clear();
            if (a[i] == '_') {
                continue;
            }
        }

        if (a[i] == ',' || a[i] == ')') {
            if (!x1.empty()) {
                p->add_x(atoi(x1.c_str()));
                x1.clear();
            }

            if (!y1.empty()) {
                p->add_y(atoi(y1.c_str()));
                y1.clear();
            }

            if (!x2.empty()) {
                p->add_x(atoi(x2.c_str()));
                x2.clear();
            }

            if (!y2.empty()) {
                p->add_y(atoi(y2.c_str()));
                y2.clear();
            }

            if (!roll.empty()) {
                p->add_roll(atoi(roll.c_str()));
                roll.clear();
            }

            if (a[i] == ')') {
                break;
            } else {
                continue;
            }
        }

        str += a[i];
    }

    //printf("\n---------\n%s\n--------------\n", p->DebugString().c_str());

    return 0;
}

int genterate_create_npc_rule::into_effect(const std::string& a, size_t i, server::create_effect_rule& rule)
{
    std::string x, y, distance, seconds;
    TO_VAL(x);
    TO_VAL(y);
    TO_VAL(distance);
    TO_VAL(seconds);
    server::into_appeare* p = rule.add_into();
    p->set_x(atoi(x.c_str()));
    p->set_y(atoi(y.c_str()));
    p->set_distance(atoi(distance.c_str()));
    if (!seconds.empty()) {
        p->set_seconds(atoi(seconds.c_str()));
    }

    ++i;
    std::string id, frequency;
    TO_VAL(id);
    x.clear();
    TO_VAL(x);
    y.clear();
    TO_VAL(y);
    TO_VAL(frequency);
    server::appear_effect* pae = rule.add_app();
    pae->set_id(atoi(id.c_str()));
    pae->set_x(atoi(x.c_str()));
    pae->set_y(atoi(y.c_str()));
    pae->set_frequency(atoi(frequency.c_str()));

    return 0;
}

int genterate_create_npc_rule::time_effect(const std::string& a, size_t i, server::create_effect_rule& rule)
{
    std::string time;
    TO_VAL(time);
    rule.add_time()->set_time(atoi(time.c_str()));

    ++i;
    std::string x, y, id, frequency;
    TO_VAL(id);
    TO_VAL(x);
    TO_VAL(y);
    TO_VAL(frequency);
    server::appear_effect* pae = rule.add_app();
    pae->set_id(atoi(id.c_str()));
    pae->set_x(atoi(x.c_str()));
    pae->set_y(atoi(y.c_str()));
    pae->set_frequency(atoi(frequency.c_str()));

    return 0;
}

check_create_npc_rule::check_create_npc_rule(scene_inst* psi)
    : _psi(psi)
{
    _cur_index = 0;
    _pass_tick = 0;
    init(psi);
}

check_create_npc_rule::~check_create_npc_rule()
{
}

void check_create_npc_rule::init(scene_inst* psi)
{
    _psi = psi;

    const server::create_effect_rules& cer = get_scene_inst()->get_scene().get_create_effect_rules();
    if (cer.rules_size() == 0) {
        return ;
    }

    for (int32 i = 0; i < cer.rules_size(); ++i) {
        for (int32 j = 0; j < cer.rules(i).time_size(); ++j) {
            _already_time_tick_effect[i][j] = cer.rules(i).time(j).time();
        }
    }
}

bool check_create_npc_rule::is_all_create()
{
    if (!_tick_npcs.empty()) {
        return false;
    }

    std::map<uint32, bool>::iterator i = _already_create_npc.begin();
    for ( ; i != _already_create_npc.end(); ++i) {
        if (!i->second) {
            return false;
        }
    }

    return true;
}

int check_create_npc_rule::on_update(uint32 tick)
{
    _pass_tick += tick;
    if (!_already_into_tick_effect.empty()) {
        create_effect_by_tick(tick, _already_into_tick_effect);
    }

    if (!_already_time_tick_effect.empty()) {
        create_effect_by_tick(tick, _already_time_tick_effect);
    }

    if (!_tick_effects.empty()) {
        on_update_effect(tick);
    }

    on_update_time();
    on_update_npc(tick);

    return 0;
}

bool remove_tick_npc(tick_npc& a)
{
    return a._tick == 0;
}

int check_create_npc_rule::on_update_npc(uint32 tick)
{
    if (get_scene_inst() == NULL) {
        return -1;
    }

    bool ret = true;
    const server::create_npc_rules& cnr = get_scene_inst()->get_scene().get_create_npc_rules();
    uint32 min_index = 0;
    std::vector<int32> ex;
    for (int32 i = 0; i < cnr.rules_size(); ++i) {
        ret = true;
        if (_already_create_npc[i]) {
            continue;
        }

        if (check_suspend(cnr.rules(i))) {
            continue;
        }

        if (cnr.rules(i).has_index()) {
            if (min_index > cnr.rules(i).index() || min_index == 0) {
                min_index = cnr.rules(i).index();
            }

            if (cnr.rules(i).index() != _cur_index + 1) {
                continue;
            }
        }

        if (cnr.rules(i).has_clear() && cnr.rules(i).clear()) {
            if (!_tick_npcs.empty() || !get_scene_inst()->is_scene_npc_all_dead()) {
                continue;
            }
        }

        for (int32 j = 0; j < cnr.rules(i).into_size(); ++j) {
            if (!_already_into[i][j]) {
                ret = false;
                break;
            }
        }

        if (!ret) {
            continue;
        }

        for (int32 j = 0; j < cnr.rules(i).story_size(); ++j) {
            if (!_already_story[i][j]) {
                ret = false;
                break;
            }
        }

        if (!ret) {
            continue;
        }

        for (int32 j = 0; j < cnr.rules(i).kill_size(); ++j) {
            if (!_already_kill[i][j]) {
                ret = false;
                break;
            }
        }

        if (cnr.rules(i).week_size() > 0) {
            continue;
        }

        if (!ret) {
            continue;
        }

        for (int32 j = 0; j < cnr.rules(i).time_size(); ++j) {
            if (cnr.rules(i).time(j).time() > _pass_tick) {
                ret = false;
                break;
            }
        }

        if (!ret) {
            continue;
        }

        for (int32 j = 0; j < cnr.rules(i).item_size(); ++j) {
            if (!_already_item[i][j]) {
                ret = false;
                break;
            }
        }

        if (!ret) {
            continue;
        }

        for (int32 j = 0; j < cnr.rules(i).object_size(); ++j) {
            if (!_already_object[i][j]) {
                ret = false;
                break;
            }
        }

        if (!ret) {
            continue;
        }

        for (int32 j = 0; j < cnr.rules(i).find_size(); ++j) {
            if (!_already_find[i][j]) {
                ret = false;
                break;
            }
        }

        if (ret) {
            //if (cnr.rules(i).has_index()) {
            //    _cur_index = cnr.rules(i).index();
            //}
            if (cnr.rules(i).suspend_size() > 0) {
                create_npc(cnr.rules(i));
                for (int32 j = 0; j < cnr.rules(i).msg_size(); ++j) {
                    _msg.push_back(cnr.rules(i).msg(j));
                }
            } else {
                ex.push_back(i);
            }

            _already_create_npc[i] = true;
        }
    }

    if (min_index != 0) {
        _cur_index = min_index - 1;
    }

    for (size_t k = 0; k < ex.size(); ++k) {
        create_npc(cnr.rules(ex[k]));
        for (int32 j = 0; j < cnr.rules(ex[k]).msg_size(); ++j) {
            _msg.push_back(cnr.rules(ex[k]).msg(j));
        }
    }

    if (!_tick_npcs.empty()) {
        std::vector<scene_npc*> sns;
        uint32 t = (uint32)time(NULL);
        std::list<tick_npc>::iterator i = _tick_npcs.begin();
        for (; i != _tick_npcs.end(); ++i) {
            if (i->_tick < tick) {
                i->_tick = 0;
            } else {
                i->_tick -= tick;
            }

            if (i->_tick == 0) {
                create_npc(*i, t, sns);
            }
        }

        i = std::remove_if(_tick_npcs.begin(), _tick_npcs.end(), remove_tick_npc);
        if (i != _tick_npcs.end()) {
            _tick_npcs.erase(i, _tick_npcs.end());
        }

        if (!sns.empty()) {
            get_scene_inst()->screen_push_npcs(sns, true);
            sns.clear();
        }
    }

    return 0;
}

int check_create_npc_rule::on_update_effect(uint32 tick)
{
    for (size_t i = 0; i < _tick_effects.size(); ++i) {
        _tick_effects[i]._last_tick += tick;
        if (_tick_effects[i]._frequency <= _tick_effects[i]._last_tick) {
            _tick_effects[i]._last_tick = 0;
            uint32 end_time = _tick_effects[i]._ti.start_time() - _tick_effects[i]._ti.end_time();
            _tick_effects[i]._ti.set_start_time(time_utils::tenms());
            _tick_effects[i]._ti.set_end_time(_tick_effects[i]._ti.start_time() + end_time);
            _tick_effects[i]._ti.set_terrain_id(get_scene_inst()->get_terrain_mgr().get_index());
            get_scene_inst()->get_terrain_mgr().add_terrain(_tick_effects[i]._ti);
            get_scene_inst()->scene_broadcast_terrain_change();
            get_scene_inst()->role_on_check_terrain(_tick_effects[i]._ti);
        }
    }

    return 0;
}


void check_create_npc_rule::on_create_npc_into_appeare(const scene_role* psr, const map_postion& from, const map_postion& to)
{
    if (get_scene_inst() == NULL) {
        return ;
    }

    const server::create_npc_rules& cnr = get_scene_inst()->get_scene().get_create_npc_rules();
    for (int32 i = 0; i < cnr.rules_size(); ++i) {
        if (cnr.rules(i).into_size() == 0 || cnr.rules(i).monster_size() == 0) {
            continue;
        }

        for (int32 j = 0; j < cnr.rules(i).into_size(); ++j) {
            if (_already_into[i][j]) {
                continue;
            }

            if (cnr.rules(i).into(j).has_role()) {
                if (psr->get_role_type() == RTE_USER && cnr.rules(i).into(j).role() != 0) {
                    continue;
                } else if (psr->get_role_id() != cnr.rules(i).into(j).role()) {
                    continue;
                }
            } else if (psr->get_role_type() != RTE_USER) {
                continue;
            }

            int32 x0 = max(0, (int)(cnr.rules(i).into(j).x() - cnr.rules(i).into(j).distance()));
            int32 x1 = cnr.rules(i).into(j).x() + cnr.rules(i).into(j).distance();
            int32 y0 = max(0, (int)(cnr.rules(i).into(j).y() - cnr.rules(i).into(j).distance()));
            int32 y1 = cnr.rules(i).into(j).y() + cnr.rules(i).into(j).distance();
            if (from.line_rect_intersection(to, postion(x0, y0), postion(x1, y1))) {
                _already_into[i][j] = true;
                if (cnr.rules(i).into(j).has_seconds()) {
                    _already_into_tick[i][j] = cnr.rules(i).into(j).seconds();
                } else {
                    _already_into_tick[i][j] = 0;
                }
            }
        }
    }

    const server::create_effect_rules& cer = get_scene_inst()->get_scene().get_create_effect_rules();
    for (int32 i = 0; i < cer.rules_size(); ++i) {
        if (cer.rules(i).into_size() == 0 || cer.rules(i).app_size() == 0) {
            continue;
        }

        if (cer.rules(i).into_size() > 1) {
        }

        for (int32 j = 0; j < cer.rules(i).into_size(); ++j) {
            if (_already_into_effect[i][j]) {
                continue;
            }

            int32 x0 = max(0, (int)(cer.rules(i).into(j).x() - cer.rules(i).into(j).distance()));
            int32 x1 = cer.rules(i).into(j).x() + cer.rules(i).into(j).distance();
            int32 y0 = max(0, (int)(cer.rules(i).into(j).y() - cer.rules(i).into(j).distance()));
            int32 y1 = cer.rules(i).into(j).y() + cer.rules(i).into(j).distance();
            if (from.line_rect_intersection(to, postion(x0, y0), postion(x1, y1))) {
                _already_into_effect[i][j] = true;
                if (cer.rules(i).into(j).has_seconds()) {
                    _already_into_tick_effect[i][j] = cer.rules(i).into(j).seconds();
                } else {
                    _already_into_tick_effect[i][j] = 0;
                }
            }
        }
    }
}

void check_create_npc_rule::on_create_npc_kill_monster(const scene_role* def)
{
    if (get_scene_inst() == NULL || def == NULL) {
        return ;
    }

    ++_kill_monster[def->get_role_id()];
    const server::create_npc_rules& cnr = get_scene_inst()->get_scene().get_create_npc_rules();
    for (int32 i = 0; i < cnr.rules_size(); ++i) {
        if (cnr.rules(i).kill_size() == 0 || cnr.rules(i).monster_size() == 0) {
            continue;
        }

        for (int32 j = 0; j < cnr.rules(i).kill_size(); ++j) {
            if(_already_kill[i][j]) {
                continue;
            }

            uint32 kill_monster_id = cnr.rules(i).kill(j).monster_id();
            uint32 kill_number     = cnr.rules(i).kill(j).number();
            if (_kill_monster[kill_monster_id] == kill_number) {
                _already_kill[i][j] = true;
                if (cnr.rules(i).kill(j).has_seconds()) {
                    _kill_monster_tick[j] = cnr.rules(i).kill(j).seconds();
                } else {
                    _kill_monster_tick[j] = 0;
                }
            } 
        }
    }
}

void check_create_npc_rule::on_create_npc_find_monster(uint32 id)
{
    if (get_scene_inst() == NULL) {
        return ;
    }

    const server::create_npc_rules& cnr = get_scene_inst()->get_scene().get_create_npc_rules();
    for (int32 i = 0; i < cnr.rules_size(); ++i) {
        if (cnr.rules(i).find_size() == 0 || cnr.rules(i).monster_size() == 0) {
            continue;
        }

        for (int32 j = 0; j < cnr.rules(i).find_size(); ++j) {
            if (_already_find[i][j]) {
                continue;
            }

            uint32 character_id = cnr.rules(i).find(j).character_id();
            if (_find_monster[id] != 0 || id != character_id) {
                break;
            }

            _already_find[i][j] = true;
            ++_find_monster[id];
            if (cnr.rules(i).find(j).has_seconds()) {
                _find_monster_tick[j] = cnr.rules(i).find(j).seconds();
            } else {
                _find_monster_tick[j] = 0;
            }
        }
    }
}

void check_create_npc_rule::create_npc(const server::create_npc_rule& cnr)
{
    uint32 tick = 0;
    for (int32 j = 0; j < cnr.into_size(); ++j) {
        tick += cnr.into(j).seconds();
    }

    for (int32 j = 0; j < cnr.kill_size(); ++j) {
        tick += cnr.kill(j).seconds();
    }

    for (int32 j = 0; j < cnr.find_size(); ++j) {
        tick += cnr.find(j).seconds();
    }

    for (int32 k = 0; k < cnr.monster_size(); ++k) {
        uint32 monster_id = cnr.monster(k).monster_id();
        uint32 number     = cnr.monster(k).number();
        tick             += cnr.monster(k).seconds();
        const origin_npc* p = GAME_MAP_MGR->get_npc(monster_id);
        if (p == NULL) {
            continue;
        }

        for (uint32 m = 0; m < number; ++m) {
            for (int32 l = 0; l < cnr.app_size(); ++l) {
                if (cnr.app(l).x_size() < 2 || cnr.app(l).y_size() < 2) {
                    continue;
                }

                uint32 x = cnr.app(l).x(1) > cnr.app(l).x(0) ? cnr.app(l).x(1) - cnr.app(l).x(0) : cnr.app(l).x(0) - cnr.app(l).x(1);
                uint32 y = cnr.app(l).y(1) > cnr.app(l).y(0) ? cnr.app(l).y(1) - cnr.app(l).y(0) : cnr.app(l).y(0) - cnr.app(l).y(1);
                map_postion mp(cnr.app(l).x(0), cnr.app(l).y(0), 0);
                mp.x += mtrandom::rand_int32(x);
                mp.y += mtrandom::rand_int32(y);
                if (get_scene_inst()->get_cell().is_block(get_scene_inst()->to_cell_pos(mp))) {
                    std::vector<cell_pos> cps;
                    get_scene_inst()->get_cell().get_not_block(get_scene_inst()->to_cell_pos(mp), cps, 1);
                    if (!cps.empty()) {
                        mp = get_scene_inst()->to_map_postion(cps[0]);
                    } else {
                        scene_role* role = get_scene_inst()->get_first_user();
                        if (role != NULL) {
                            mp = role->get_scene_pos();
                        } else {
                            continue;
                        }
                    }
                }

                uint32 add = 0;
                if (cnr.app(l).roll_size() != 0) {
                    add = cnr.app(l).roll(0);
                }

                _tick_npcs.push_back(tick_npc(tick += add, p, mp));
            }

            for (int32 l = 0; l < cnr.around_size(); ++l) {
                scene_role* role = NULL;
                if (cnr.around(l).id() == 1) {
                    role = get_scene_inst()->get_first_user();
                } else {
                    role = get_scene_inst()->get_scene_npc_by_role_id(cnr.around(l).id());
                }

                if (role == NULL) {
                    continue;
                }

                map_postion mp;
                get_scene_inst()->calc_pos_to_pos(role->get_scene_pos(), (ROLE_FACING)mtrandom::rand_int32(NORTHWEST), cnr.around(l).distance(), mp);
                if (get_scene_inst()->get_cell().is_block(get_scene_inst()->to_cell_pos(mp))) {
                    mp = role->get_scene_pos();
                }

                _tick_npcs.push_back(tick_npc(tick += cnr.around(l).tick(), p, mp));
            }
        }
    }
}

void check_create_npc_rule::create_npc(const server::create_npc_rule& cnr, std::vector<scene_npc*>& sns)
{
    for (int32 k = 0; k < cnr.monster_size(); ++k) {
        uint32 monster_id = cnr.monster(k).monster_id();
        uint32 number     = cnr.monster(k).number();
        const origin_npc* p = GAME_MAP_MGR->get_npc(monster_id);
        if (p == NULL) {
            continue;
        }
        for (uint32 m = 0; m < number; ++m) {
            for (int32 l = 0; l < cnr.app_size(); ++l) {
                map_postion mp(cnr.app(l).x(0), cnr.app(l).y(0), 0);
                if (cnr.app(l).roll_size() > 0) {
                    uint32 roll = cnr.app(l).roll(0);
                    mp.x += mtrandom::rand_int32(roll);
                    mp.y += mtrandom::rand_int32(roll);
                }

                scene_npc* sn = get_scene_inst()->generate_npc_inst(*p, (uint32)time(NULL), mp);
                if (sn != NULL) {
                    sns.push_back(sn);
                }
            }
        }
    }
}

void check_create_npc_rule::create_npc(const server::create_npc_rule& cnr, uint32 index, std::vector<scene_npc*>& sns)
{
    uint32 k = index;
    uint32 monster_id = cnr.monster(k).monster_id();
    uint32 number     = cnr.monster(k).number();
    const origin_npc* p = GAME_MAP_MGR->get_npc(monster_id);
    if (p == NULL || number == 0) {
        amtsvr_log("create npc %u no config\n", monster_id);
        return ;
    }

    uint32 t = (uint32)time(NULL);
    for (uint32 m = 0; m < number; ++m) {
        map_postion mp(cnr.app(k).x(0), cnr.app(k).y(0), 0);
        if (cnr.app(k).roll_size() > 0) {
            uint32 roll = cnr.app(k).roll(0);
            mp.x += mtrandom::rand_int32(roll);
            mp.y += mtrandom::rand_int32(roll);
        }

        scene_npc* sn = get_scene_inst()->generate_npc_inst(*p, t, mp);
        if (sn != NULL) {
            sns.push_back(sn);
        }
    }
}

void check_create_npc_rule::create_npc(tick_npc& npc, uint32 t, std::vector<scene_npc*>& sns)
{
    scene_npc* sn = get_scene_inst()->generate_npc_inst(*npc._npc, t, npc._pos);
    if (sn != NULL) {
        sns.push_back(sn);
    }
}

void check_create_npc_rule::create_npc_by_tick(uint32 tick, std::map<uint32, uint32>& ticks, std::vector<scene_npc*>& sns)
{
    const server::create_npc_rules& cnr = get_scene_inst()->get_scene().get_create_npc_rules();
    if (cnr.rules_size() == 0) {
        return ;
    }

    std::map<uint32, uint32>::iterator i = ticks.begin();
    while (i != ticks.end()) {
        if (i->first >= (uint32)cnr.rules(0).monster_size()) {
            break;
        }

        if (i->second < tick) {
            i->second = 0;
        } else {
            i->second -= tick;
        }

        if (i->second == 0) {
            create_npc(cnr.rules(0), i->first, sns);
            ticks.erase(i++);
            continue;
        }

        ++i;
    }
}

void check_create_npc_rule::create_effect(const server::create_effect_rule& cnr, uint32 index, uint32 tick)
{
    uint32 skill_id  = cnr.app(index).id();
    uint32 x         = cnr.app(index).x();
    uint32 y         = cnr.app(index).y();
    uint32 frequency = cnr.app(index).frequency();
    const config_skill*  pcs = CONFMGR->get_config_skill(skill_id);
    if (pcs == NULL) {
        return ;
    }

    const std::vector<int>& vml = pcs->get_method_list();
    for (size_t j = 0; j < vml.size(); ++j) {
        const config_effect* pce = CONFMGR->get_config_effect(vml[j]);
        if (pce == NULL) {
            continue ;
        }

        if (pce->is_scene_buff()) {
            tick_effect te(0, frequency);
            get_scene_inst()->get_terrain_mgr().add_terrain(pcs, pce, x, y, te._ti);
            get_scene_inst()->scene_broadcast_terrain_change();
            get_scene_inst()->role_on_check_terrain(te._ti);
            if (frequency != 0) {
                _tick_effects.push_back(te);
            }

            continue;
        }
    }
}

void check_create_npc_rule::create_effect_by_tick(uint32 tick, std::map<uint32, std::map<uint32, uint32> >& ticks)
{
    const server::create_effect_rules& cer = get_scene_inst()->get_scene().get_create_effect_rules();
    if (cer.rules_size() == 0) {
        return ;
    }

    for (int32 index = 0; index < cer.rules_size(); ++index) {
        std::map<uint32, uint32>::iterator i = ticks[index].begin();
        while (i != ticks[index].end()) {
            if (i->first >= (uint32)cer.rules(index).app_size()) {
                break;
            }

            if (i->second < tick) {
                i->second = 0;
            } else {
                i->second -= tick;
            }

            if (i->second == 0) {
                create_effect(cer.rules(index), i->first, tick);
                ticks[index].erase(i++);
                continue;
            }

            ++i;
        }
    }
}

void check_create_npc_rule::on_after_story(uint32 id)
{
    ++_stories[id];

    const server::create_npc_rules& cnr = get_scene_inst()->get_scene().get_create_npc_rules();
    for (int32 i = 0; i < cnr.rules_size(); ++i) {
        if (cnr.rules(i).story_size() == 0 || cnr.rules(i).monster_size() == 0) {
            continue;
        }

        for (int32 j = 0; j < cnr.rules(i).story_size(); ++j) {
            if (_stories[cnr.rules(i).story(j).id()] > 0) {
                _already_story[i][j] = true;
            }
        }
    }
}

void check_create_npc_rule::on_update_time()
{
    time_t t = time(NULL);
    struct tm src;
    localtime_r(&t, &src);

    const server::create_npc_rules& cnr = get_scene_inst()->get_scene().get_create_npc_rules();
    for (int32 i = 0; i < cnr.rules_size(); ++i) {
        for (int32 j = 0; j < cnr.rules(i).week_size(); ++j) {
            const server::start_time_week& week = cnr.rules(i).week(j);
            if ((int32)week.t().m() != src.tm_min) {
                _already_time[i][j] = false;
                continue;
            }

            if ((int32)week.t().h() != src.tm_hour) {
                _already_time[i][j] = false;
                continue;
            }

            if ((int32)week.d() != 0 && (int32)week.d() - 1 != src.tm_wday) {
                _already_time[i][j] = false;
                continue;
            }

            if (!_already_time[i][j]) {
                create_npc(cnr.rules(i));
            }

            _already_time[i][j] = true;
        }
    }
}

void check_create_npc_rule::on_get_item(const client::props_info* ppi)
{
    _items[ppi->props_id()] += ppi->count();

    const server::create_npc_rules& cnr = get_scene_inst()->get_scene().get_create_npc_rules();
    for (int32 i = 0; i < cnr.rules_size(); ++i) {
        if (cnr.rules(i).item_size() == 0 || cnr.rules(i).monster_size() == 0) {
            continue;
        }

        for (int32 j = 0; j < cnr.rules(i).item_size(); ++j) {
            if (_stories[cnr.rules(i).item(j).item_id()] > cnr.rules(i).item(j).count()) {
                _already_item[i][j] = true;
            }
        }
    }
}

void check_create_npc_rule::on_touch_object(const scene_role* psr)
{
    ++_touch_objects[psr->get_role_id()];

    const server::create_npc_rules& cnr = get_scene_inst()->get_scene().get_create_npc_rules();
    for (int32 i = 0; i < cnr.rules_size(); ++i) {
        if (cnr.rules(i).object_size() == 0 || cnr.rules(i).monster_size() == 0) {
            continue;
        }

        for (int32 j = 0; j < cnr.rules(i).object_size(); ++j) {
            if (_stories[cnr.rules(i).object(j).object()] > cnr.rules(i).object(j).count()) {
                _already_object[i][j] = true;
            }
        }
    }
}

bool check_create_npc_rule::check_suspend(const server::create_npc_rule& cnr)
{
    static const uint32 MONSTER_COUNT = 1;
    for (int32 index = 0; index < cnr.suspend_size(); ++index) {
        switch (cnr.suspend(index).type()) {
        case MONSTER_COUNT: return check_suspend_monster_count(cnr.suspend(index).count()); break;
        default: break;
        }
    }

    return false;
}

bool check_create_npc_rule::check_suspend_monster_count(uint32 count)
{
    if (get_scene_inst() == NULL) {
        return true;
    }

    std::vector<scene_npc*> ns;
    return get_scene_inst()->get_alive_monster(ns) + _tick_npcs.size() > count;
}

// void check_create_npc_rule::on_update_time_effect(uint32 tick)
// {
//     create_effect_by_tick(tick, _already_time_effect);
//     const server::create_effect_rules& cer = get_scene_inst()->get_scene().get_create_effect_rules();
//     if (cer.rules_size() == 0) {
//         return ;
//     }
// 
//     std::map<uint32, std::map<uint32, uint32> >::iterator i = _already_time_effect.begin();
//     for ( ; i != _already_time_effect.end(); ++i) {
//         std::map<uint32, uint32>::iterator j = i->second.begin();
//         for ( ; j != i->second.end(); ++j) {
//             if (j->second > tick) {
//                 j->second -= tick;
//             } else {
//                 if (i->first < cer.rules_size()) {
//                     const server::create_effect_rule& rule = cer.rules(i->first);
//                     if (j->first < rule.time_size()) {
//                         
//                     }
//                 }
//             }
//         }
//     }
// }
