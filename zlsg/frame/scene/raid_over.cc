#include "config_raid.h"
#include "scene_define.h"
#include "raid_scene_inst.h"
#include "raid_over.h"
#include "config_npc.h"

#define TO_VAL(STR)     ++i;                                    \
    for (; i < a.size() && a[i] != '_' && a[i] != ';' && a[i] != '|'; ++i) {   \
        STR += a[i];                                            \
    }

genterate_raid_over_rule::genterate_raid_over_rule(server::raid_over_rules* rules)
    : _rules(rules)
{
}

genterate_raid_over_rule::~genterate_raid_over_rule()
{

}

void genterate_raid_over_rule::init(const std::string& success_rules, const std::string& failure_rules)
{
    if (_rules == NULL) {
#ifdef __INNER_DEBUG__PACKAGE_
        amtsvr_log("error server::raid_over_rules == NULL");
        return ;
#endif // __INNER_DEBUG__PACKAGE_
    }

    init_success_rules(success_rules);
    init_failure_rules(failure_rules);
}

void genterate_raid_over_rule::init_success_rules(const std::string& success_rules)
{
    if (success_rules.empty()) {
        return ;
    }

    server::raid_success_rule* p = _rules->add_success();

    for (size_t i = 0; i < success_rules.size(); ++i) {
        switch (success_rules[i]) {
        case '!': {
            i = add_dead_role(success_rules, i, p->add_role());
            break;
        }
        case '@': {
            i = add_get_item(success_rules, i, p->add_item());
            break;
        }
        case '$': {
            i = add_dead_role(success_rules, i, p->add_touch());
            break;
         }
        case '*': {
            i = add_ai_timer(success_rules, i, p->add_timer());
            break;
        }
        case '#': {
            i = add_role_postion(success_rules, i, p->add_pos());
            break;
        }
        case '%': {
            i = add_role_postion(success_rules, i, p->add_block());
            break;
        }
		case '&' : {
			i = add_survive_role(success_rules, i, p->add_survive());
		}
        default: break;
        }

        if (i < success_rules.length() && success_rules[i] == ';') {
            p = _rules->add_success();
        }
    }

    // printf("%s:%d, %s\n", __FILE__, __LINE__, _rules->DebugString().c_str());
}

void genterate_raid_over_rule::init_failure_rules(const std::string& failure_rules)
{
    if (failure_rules.empty()) {
        return ;
    }

    server::raid_failure_rule* p = _rules->add_failure();

    for (size_t i = 0; i < failure_rules.size(); ++i) {
        switch (failure_rules[i]) {
        case '!': {
            i = add_dead_count(failure_rules, i, p->add_role());
            break;
                  }
        case '@': {
            i = add_scene_timeout(failure_rules, i, p->add_timeout());
            break;
                  }
        case '#': {
            i = add_dead_count(failure_rules, i, p->add_count());
            break;
                  }
        case '$': {
            i = add_role_attribute(failure_rules, i, p->add_attr());
            break;
                  }
        case '%': {
            i = add_role_postion(failure_rules, i, p->add_pos());
            break;
                  }
        case ';': {
            p = _rules->add_failure();
                  }
        default: break;
        }

        if (i < failure_rules.length() && failure_rules[i] == ';') {
            p = _rules->add_failure();
        }
    }

    // printf("%s:%d, %s\n", __FILE__, __LINE__, _rules->DebugString().c_str());
}

int genterate_raid_over_rule::add_survive_role(const std::string &a, size_t i, server::survive_role *rule)
{
	++i;
	std::string count, tick;
	TO_VAL(count);
    for (; i < a.length() && a[i] != '_' && a[i] != ';' && a[i] != '|'; ++i)
	{
		count += a[i];
		if (i + 1 == a.length() || a[i + 1] == ';' || a[i + 1] == '|') {
			tick = count;
			count = "0";
			break;
		}
	}
	if (i < a.length() && a[i] == '_' && count != "0") {
		TO_VAL(tick);
	}
    rule->set_count(atoi(count.c_str()));
    rule->set_tick (atoi(tick.c_str() ));
	return 0;
}

int genterate_raid_over_rule::add_dead_role(const std::string& a, size_t i, server::dead_role* rule)
{
    ++i;
    std::string id, count, tick;
    TO_VAL(id);
    if (id != "0") {
        TO_VAL(count);
        if (i < a.length() && a[i] == '_') {
            TO_VAL(tick);
        }
    } else {
        ++i;
        for (; i < a.length() && a[i] != '_' && a[i] != ';' && a[i] != '|'; ++i) {
            count += a[i];
            if (i + 1 == a.length() || a[i + 1] == ';' || a[i + 1] == '|') {
                tick = count;
                count = "0";
                break;
            }
        }

        if (i < a.length() && a[i] == '_' && count != "0") {
            TO_VAL(tick);
        }
    }

    rule->set_id   (atoi(id.c_str()   ));
    rule->set_count(atoi(count.c_str()));
    rule->set_tick (atoi(tick.c_str() ));

    return i;
}

int genterate_raid_over_rule::add_get_item(const std::string& a, size_t i, server::get_item* rule)
{
    ++i;
    std::string id, count, tick;
    TO_VAL(id);
    TO_VAL(count);
    if (a[i] == '_') {
        TO_VAL(tick);
    } else {
        tick = "0";
    }

    rule->set_item_id(atoi(id.c_str()));
    rule->set_count(atoi(count.c_str()));
    rule->set_tick(atoi(tick.c_str()));
    return i;
}

int genterate_raid_over_rule::add_ai_timer(const std::string& a, size_t i, server::ai_timer* rule)
{
    ++i;
    std::string alarm;
    TO_VAL(alarm);
    rule->set_alarm(atoi(alarm.c_str()));
    return i;
}

int genterate_raid_over_rule::add_scene_timeout(const std::string& a, size_t i, server::scene_timeout* rule)
{
    ++i;
    std::string tenms;
    TO_VAL(tenms);
    rule->set_tenms(atoi(tenms.c_str()));
    return i;
}

int genterate_raid_over_rule::add_dead_count(const std::string& a, size_t i, server::dead_count* rule)
{
    ++i;
    std::string count;
    TO_VAL(count);
    rule->set_count(atoi(count.c_str()));
    return i;
}

int genterate_raid_over_rule::add_role_attribute(const std::string& a, size_t i, server::role_attribute* rule)
{
    ++i;
    std::string id, val;
    TO_VAL(id);
    TO_VAL(val);
    rule->set_id(atoi(id.c_str()));
    rule->set_type(CUR_HP);
    rule->set_value(atoi(val.c_str()));
    return i;
}

int genterate_raid_over_rule::add_role_postion(const std::string& a, size_t i, server::role_postion* rule)
{
    ++i;
    std::string id, x, y, distance, number, tick;
    TO_VAL(id);
    TO_VAL(x);
    TO_VAL(y);
    TO_VAL(distance);
    TO_VAL(number);
    if (a[i] == '_') {
        TO_VAL(tick);
    } else {
        tick = "0";
    }
    rule->set_id(atoi(id.c_str()));
    rule->set_x(atoi(x.c_str()));
    rule->set_y(atoi(y.c_str()));
    rule->set_distance(atoi(distance.c_str()));
    rule->set_number(atoi(number.c_str()));
    rule->set_tick(atoi(tick.c_str()));
    return i;
}

raid_over::raid_over(scene_inst* psi)
    : _si(psi), _region_id(0)
{
    _dead_count = 0;
    _tenms = 0;
    init(psi);
}

raid_over::raid_over(scene_inst* psi, const server::raid_over_rules& rules)
    : _si(psi), _over_rules(&rules), _region_id(0)
{
    _dead_count = 0;
    _tenms = 0;
    set_scene_inst(psi);
}

raid_over::~raid_over(void)
{
}

int raid_over::on_update(uint32 tick)
{

    _tenms += tick;

    if (_over_rules == NULL) {
        return 0;
    }

    int ret = 0;
    if (check_failure()) {
		return -1;
//        ret = -1;
    }

    if (check_success()) {
		return 1;
//        ret = 1;
    }

    return ret;
}

void raid_over::fill_success(client::raid_success_info* p)
{
    p->CopyFrom(_success_info);
}

void raid_over::fill_failure(client::raid_failure_info* p)
{
    p->CopyFrom(_failure_info);
}

void raid_over::init(scene_inst* psi)
{
    if (psi == NULL) {
#ifdef __INNER_DEBUG__PACKAGE_
        amtsvr_log("error raid_scene_inst == NULL");
#endif // __INNER_DEBUG__PACKAGE_
        return ;
    }

	uint32 type = psi->get_scene_type();
    if (type == MAP_TYPE_RAID || type == MAP_TYPE_PET || type == MAP_TYPE_TEAM || type == MAP_TYPE_LEVELS || type == MAP_TYPE_ARENA || type == MAP_TYPE_CYCLE || type == MAP_TYPE_DEVIL) {
        raid_scene_inst* prsi = static_cast<raid_scene_inst*>(psi);
        if (prsi->is_elite()) {
            _over_rules = &prsi->get_scene().get_raid_over_rules_elite();
        } else if (prsi->is_first()) {
            _over_rules = &prsi->get_scene().get_raid_over_rules_first();
        } else {
            _over_rules = &prsi->get_scene().get_raid_over_rules();
        }
    }

    set_scene_inst(psi);
}

void raid_over::mark_dead_role(scene_role* psr)
{
    if (psr == NULL) {
        return ;
    }

    const server::raid_over_rules& rules = *_over_rules;
    for (int32 i = 0; i < rules.success_size(); ++i) {
        const server::raid_success_rule& success_rule = rules.success(i);
        for (int32 j = 0; j < success_rule.block_size(); ++j) {
            const server::role_postion& pos = success_rule.block(j);
            if (pos.id() != 0 && pos.id() == psr->get_role_id()) {
                map_postion to = psr->get_scene_pos();
                int32 x0 = max(0, (int)(pos.x() - pos.distance()));
                int32 x1 = pos.x() + pos.distance();
                int32 y0 = max(0, (int)(pos.y() - pos.distance()));
                int32 y1 = pos.y() + pos.distance();
                if (psr->get_scene_pos().x <= x0 && psr->get_scene_pos().x <= x1 && psr->get_scene_pos().y <= y0 && psr->get_scene_pos().y <= y1) {
                } else {
                    ++_pos_dead_role[psr->get_role_id()];
                }
            }
        }
    }

    // switch (psr->get_role_type()) {
    // case RTE_USER: ++_user_deads[psr->get_role_id()]; break;
    // case RTE_NPC:  ++_role_deads[psr->get_role_id()]; ++_dead_count; break;
    // default: break;
    // }
	if(psr->get_role_type() == RTE_USER || (psr->is_robot() && psr->get_camp() == DEFENCE))
	{
		++_user_deads[psr->get_role_id()];
	}
	else if(psr->get_role_type() == RTE_NPC)
	{
		++_role_deads[psr->get_role_id()];
		++_dead_count;
	}
}

void raid_over::mark_items(client::props_info* ppi)
{
    if (ppi == NULL) {
        return ;
    }

    _items[ppi->props_id()] += ppi->count();
}

void raid_over::mark_touch(scene_role* psr)
{
    if (psr == NULL) {
        return ;
    }

    ++_touch[psr->get_role_id()];
}

void raid_over::mark_role_attribute(scene_role* psr)
{
    if (psr == NULL) {
        return ;
    }

    if (std::find(_failure_attributes.begin(), _failure_attributes.end(), psr) != _failure_attributes.end()) {
        return ;
    }

    const server::raid_over_rules& rules = *_over_rules;
//     for (int32 i = 0; i < rules.success_size(); ++i) {
//         const server::raid_success_rule& success_rule = rules.success(i);
//         for (int32 j = 0; j < success_rule.attr)
//         {
//         }
//     }

    for (int32 i = 0; i < rules.failure_size(); ++i) {
        const server::raid_failure_rule& failure_rule = rules.failure(i);
        for (int32 j = 0; j < failure_rule.attr_size(); ++j) {
            const server::role_attribute& attr = failure_rule.attr(j);
            if (attr.id() == psr->get_role_id() && attr.value() >= psr->get_attr().get_val((ROLE_ATTRIBUTE)attr.type())) {
                _failure_attributes.push_back(psr);
                if (_attributes[i][j]._first_id == 0) {
                    _attributes[i][j]._first_id = psr->get_role_id();
                }
                ++_attributes[i][j]._count;
            }
        }
    }
}

void raid_over::mark_role_postion(scene_role* psr, const map_postion& from, const map_postion& to)
{
    if (psr == NULL) {
        return ;
    }

    if (std::find(_success_postions.begin(), _success_postions.end(), psr) == _success_postions.end()) {
        const server::raid_over_rules& rules = *_over_rules;
        for (int32 i = 0; i < rules.success_size(); ++i) {
            const server::raid_success_rule& failure_rule = rules.success(i);
            for (int32 j = 0; j < failure_rule.pos_size(); ++j) {
                const server::role_postion& pos = failure_rule.pos(j);
                //map_postion to = psr->get_scene_pos();
                int32 x0 = max(0, (int)(pos.x() - pos.distance()));
                int32 x1 = pos.x() + pos.distance();
                int32 y0 = max(0, (int)(pos.y() - pos.distance()));
                int32 y1 = pos.y() + pos.distance();
                
                //if (psr->get_role_id() == 7040161) {
                //    amtsvr_log("%d:%d, %d:%d, %d:%d, %d:%d, %d\n", 
                //        from.x, from.y,
                //        to.x, to.y,
                //        x0, y0,
                //        x1, y1,
                //        from.line_rect_intersection(to, postion(x0, y0), postion(x1, y1)) ? 1 : 0);
                //}

                if (from.line_rect_intersection(to, postion(x0, y0), postion(x1, y1)) &&
                    (pos.id() == psr->get_role_id() || (psr->get_role_type() == RTE_USER && pos.id() == 0))) {
                    _success_postions.push_back(psr);
                    if (_success_postions_m[i][j]._first_id == 0) {
                        if (psr->get_role_type() != RTE_USER) {
                            _success_postions_m[i][j]._first_id = psr->get_role_id();
                        } else {
                            _success_postions_m[i][j]._first_id = 0;
                        }
                    }
                    ++_success_postions_m[i][j]._count;
                }
            }
        }
    }

    if (std::find(_failure_postions.begin(), _failure_postions.end(), psr) != _failure_postions.end()) {
        return ;
    }

    const server::raid_over_rules& rules = *_over_rules;

    for (int32 i = 0; i < rules.failure_size(); ++i) {
        const server::raid_failure_rule& failure_rule = rules.failure(i);
        for (int32 j = 0; j < failure_rule.pos_size(); ++j) {
            const server::role_postion& pos = failure_rule.pos(j);
            //map_postion to = psr->get_scene_pos();
            int32 x0 = max(0, (int)(pos.x() - pos.distance()));
            int32 x1 = pos.x() + pos.distance();
            int32 y0 = max(0, (int)(pos.y() - pos.distance()));
            int32 y1 = pos.y() + pos.distance();
            if (from.line_rect_intersection(to, postion(x0, y0), postion(x1, y1))) {
                _failure_postions.push_back(psr);
                if (_postions[i][j]._first_id == 0) {
                    if (psr->get_role_type() != RTE_USER) {
                        _postions[i][j]._first_id = psr->get_role_id();
                    } else {
                        _postions[i][j]._first_id = 0;
                    }
                }
                ++_postions[i][j]._count;
            }
        }
    }
}

bool raid_over::check_success()
{
    const server::raid_over_rules& rules = *_over_rules;
    if (rules.success_size() == 0) {
        return false;
    }

    bool ret = true;
    for (int32 i = 0; i < rules.success_size(); ++i) {
        ret = true;
        const server::raid_success_rule& success_rule = rules.success(i);
        for (int32 j = 0; j < success_rule.role_size(); ++j) {
            const server::dead_role& role = success_rule.role(j);
            if (role.tick() != 0 && _tenms > role.tick()) {
                ret = false;
                break;
            }

            raid_scene_inst* psi = NULL;
			uint32 type = get_scene_inst()->get_scene_type();
            if (type == MAP_TYPE_RAID || type == MAP_TYPE_PET || type == MAP_TYPE_TEAM || type == MAP_TYPE_LEVELS || type == MAP_TYPE_ARENA || type == MAP_TYPE_CYCLE || type == MAP_TYPE_DEVIL) {
                psi = static_cast<raid_scene_inst*>(get_scene_inst());
            }

            bool check = false;
            if (_region_id != 0) {
                check = psi->check_kill_role_region(_region_id);
            } else {
                check = psi->check_kill_role(NULL, NULL);
            }

            if (psi != NULL && role.id() == 0 && role.count() == 0 && !check) {
                ret = false;
                break;
            } else if (role.id() == 0 && role.count() != 0) {
//                 uint32 dead_count = 0;
//                 for (std::map<uint32, uint32>::iterator it = _role_deads.begin(); it != _role_deads.end(); ++it) {
//                     dead_count += it->second;
//                 }

                if (_dead_count < role.count()) {
                    ret = false;
                    break;
                }
            } else if (_role_deads[role.id()] < role.count()) {
                ret = false;
                break;
            } else {
                _success_info.mutable_role()->set_id   (role.id());
                _success_info.mutable_role()->set_count(role.count());
            }
        }

        if (!ret) {
            continue;
        }

        for (int32 j = 0; j < success_rule.item_size(); ++j) {
            const server::get_item& item = success_rule.item(j);
            if (item.tick() != 0 && _tenms > item.tick()) {
                ret = false;
                break;
            }

            if (_items[item.item_id()] < item.count()) {
                ret = false;
                break;
            } else {
                _success_info.mutable_item()->set_id   (item.item_id());
                _success_info.mutable_item()->set_count(item.count  ());
            }
        }

        if (!ret) {
            continue;
        }

        for (int32 j = 0; j < success_rule.touch_size(); ++j) {
            const server::dead_role& touch = success_rule.touch(j);
            if (touch.tick() != 0 && _tenms > touch.tick()) {
                ret = false;
                break;
            }

            if (_touch[touch.id()] < touch.count()) {
                ret = false;
                break;
            } else {
                _success_info.mutable_touch()->set_id   (touch.id   ());
                _success_info.mutable_touch()->set_count(touch.count());
            }
        }

//         for (int32 j = 0; j < success_rule.timer_size(); ++j) {
//         }

        if (!ret) {
            continue;
        }

        for (int32 j = 0; j < success_rule.pos_size(); ++j) {
            const server::role_postion& pos = success_rule.pos(j);
            if (pos.tick() != 0 && _tenms > pos.tick()) {
                ret = false;
                break;
            }

            if (_success_postions_m[i][j]._count == 0) {
                ret = false;
                break;
            }

            if (pos.number() > _success_postions_m[i][j]._count) {
                ret = false;
                break;
            } else {
                _success_info.mutable_pos()->set_id(_success_postions_m[i][j]._first_id);
                _success_info.mutable_pos()->set_count(_success_postions_m[i][j]._count);
            }
        }

        if (!ret) {
            continue;
        }

        for (int32 j = 0; j < success_rule.block_size(); ++j) {
            const server::role_postion& pos = success_rule.block(j);
            if (pos.tick() != 0 && _tenms > pos.tick()) {
                ret = false;
                break;
            }

            if (_pos_dead_role[pos.id()] < pos.number()) {
                ret = false;
                break;
            } else {
                _success_info.mutable_block()->set_id(pos.id());
                _success_info.mutable_block()->set_count(pos.number());
            }
        }
		if(!ret) {
			continue;
		}
		for(int32 k = 0; k < success_rule.survive_size(); ++k)
		{
			const server::survive_role &survive = success_rule.survive(k);
			if(_tenms < survive.tick())
			{
				ret = false;
				break;
			}
            raid_scene_inst* psi = NULL;
			uint32 type = get_scene_inst()->get_scene_type();
            if (type == MAP_TYPE_RAID || type == MAP_TYPE_PET || type == MAP_TYPE_TEAM || type == MAP_TYPE_LEVELS || type == MAP_TYPE_ARENA || type == MAP_TYPE_CYCLE || type == MAP_TYPE_DEVIL)
			{
                psi = static_cast<raid_scene_inst*>(get_scene_inst());
            }
			if(psi == NULL)
			{
				ret = false;
				break;
			}
			if(survive.count() > psi->get_survive_count())
			{
				ret = false;
				break;
			}
		}
        if (ret) {
            break;
        } else {
            _success_info.Clear();
        }
    }

    return ret;
}

bool raid_over::check_failure()
{
    const server::raid_over_rules& rules = *_over_rules;
    if (rules.failure_size() == 0) {
        return false;
    }
    bool ret = true;
    for (int32 i = 0; i < rules.failure_size(); ++i) {
        ret = true;
        const server::raid_failure_rule& failure_rule  = rules.failure(i);
        for (int32 j = 0; j < failure_rule.role_size(); ++j) {
            const server::dead_count& role = failure_rule.role(j);
            if (role.count() > _user_deads.size()) {
                ret = false;
                break;
            } else if (role.count() == 1 && get_scene_inst()->get_user_map().obj_num() == role.count()) {
                _failure_info.set_hero_dead(true);
            } else {
                _failure_info.set_role_dead(role.count());
            }
        }

        if (!ret) {
            continue;
        }

        for (int32 j = 0; j < failure_rule.timeout_size(); ++j) {
            const server::scene_timeout& timeout = failure_rule.timeout(j);
            if (timeout.tenms() > _tenms) {
                ret = false;
                break;
            } else {
                _failure_info.set_timeout(timeout.tenms());
            }
        }

        if (!ret) {
            continue;
        }

        for (int32 j = 0; j < failure_rule.count_size(); ++j) {
            const server::dead_count& count = failure_rule.count(j);
            std::map<uint32, uint32>::iterator k = _user_deads.begin();
            uint32 all = 0;
            while (k != _user_deads.end()) {
                all += k->second;
                ++k;
            }

            if (count.count() > all) {
                ret = false;
                break;
            } else {
                _failure_info.set_dead_count(count.count());
            }
        }

        if (!ret) {
            continue;
        }

        for (int32 j = 0; j < failure_rule.attr_size(); ++j) {
            if (_attributes[i][j]._count <= 0) {
                ret = false;
                break;
            } else {
                _failure_info.set_vip_dead(_attributes[i][j]._first_id);
            }
        }

        if (!ret) {
            continue;
        }

        for (int32 j = 0; j < failure_rule.pos_size(); ++j) {
            const server::role_postion& pos = failure_rule.pos(j);
            if (pos.number() > _postions[i][j]._count) {
                ret = false;
                break;
            } else {
                _failure_info.set_monster(_postions[i][j]._first_id);
            }
        }

        if (ret) {
            break;
        } else {
            _failure_info.Clear();
        }
    }

    return ret;
}
