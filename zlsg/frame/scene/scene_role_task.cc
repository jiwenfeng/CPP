#include "config_quest.h"
#include "scene_role_task.h"

scene_role_task::scene_role_task(player_role_task& task)
        : _task(task)
{
    std::map<uint32, client::task_accepted>* pa = _task.mutable_accepted_tasks();
    std::map<uint32, client::task_accepted>::iterator i = pa->begin();
    while (i != pa->end()) {
        init_task(i->second);
        ++i;
    }
}

scene_role_task::~scene_role_task()
{

}

void scene_role_task::init_task(const client::task_accepted& a)
{
    const config_quest* quest = CONFMGR->get_config_quest_mgr().get_config_quest(a.task_id());
    if (quest == NULL) {
        return ;
    }

    switch (quest->get_quest_type()) {
    case QT_TALK: {
        break;
    }
    case QT_RAID: {
        client::task_raid raid;
        raid.set_raid_id(quest->get_map_id());
        raid.set_count(1);
        _need_raid.insert(std::pair<uint32, client::task_raid>(quest->get_quest_id(), raid));
        break;
    }
    case QT_PICK: {
        client::task_pick pick;
        const std::map<int, int>& item = quest->get_need_item();
        std::map<int, int>::const_iterator i = item.begin();
        while (i != item.end()) {
            pick.set_id(i->first);
            pick.set_count(i->second);
            _need_pick.insert(std::pair<uint32, client::task_pick>(quest->get_quest_id(), pick));
            ++i;
        }
        break;
    }
    case QT_KILL: {
        client::task_kill kill;
        const std::map<int, int>& monster = quest->get_kill_monster();
        std::map<int, int>::const_iterator i = monster.begin();
        while (i != monster.end()) {
            kill.set_id(i->first);
            kill.set_count(i->second);
            _need_kill.insert(std::pair<uint32, client::task_kill>(quest->get_quest_id(), kill));
            ++i;
        }
        break;
    }
    case QT_GUARD: {
        break;
    }
    case QT_LEVEL: {
        client::task_level level;
        level.set_level(quest->get_need_level());
        _need_level.insert(std::pair<uint32, client::task_level>(quest->get_quest_id(), level));
        break;
    }
    case QT_EVENT: {
        break;
    }
    default: break;
    }
}

void scene_role_task::remove_task_condition(uint32 task_id)
{
    _need_raid.erase(task_id);
    _need_pick.erase(task_id);
    _need_kill.erase(task_id);
    _need_guard.erase(task_id);
    _need_level.erase(task_id);
    _need_event.erase(task_id);
}

bool scene_role_task::add_raid(uint32 raid_id)
{
    std::vector<uint32> task_ids;
    std::multimap<uint32, client::task_raid>::iterator i = _need_raid.begin();
    while (i != _need_raid.end()) {
        if (i->second.raid_id() == raid_id) {
            task_ids.push_back(i->first);
        }
        ++i;
    }

    bool update = false;
    std::map<uint32, client::task_accepted>* pa = _task.mutable_accepted_tasks();
    for (size_t j = 0; j < task_ids.size(); ++j) {
        std::map<uint32, client::task_accepted>::iterator k = pa->find(task_ids[j]);
        if (k == pa->end()) {
            continue;
        }

        bool new_raid = true;
        for (int32 l = 0; l < k->second.progress().raid_size(); ++l) {
            if (k->second.progress().raid(l).raid_id() == raid_id) {
                new_raid = false;
                k->second.mutable_progress()->mutable_raid(l)->set_count(k->second.progress().raid(l).count() + 1);
                break;
            }
        }

        if (new_raid) {
            client::task_raid* p = k->second.mutable_progress()->add_raid();
            p->set_raid_id(raid_id);
            p->set_count(1);
        }

        _update.set_t(client::task_msg_type_update);
        _update.add_accepted()->CopyFrom(k->second);
        update = true;
    }

    return update;
}

