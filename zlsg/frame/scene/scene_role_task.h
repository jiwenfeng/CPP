#ifndef __SCENE_ROLE_TASK_H__
#define __SCENE_ROLE_TASK_H__

#include "scene_define.h"
#include "player_role_task.h"

class scene_role_task
{
public:
    scene_role_task(player_role_task& task);
    ~scene_role_task();

public:
    player_role_task& get_player_role_task() { return _task; }
    
public:
    void init_task(const client::task_accepted& a);
    void remove_task_condition(uint32 task_id);
    
public:
    bool add_raid(uint32 raid_id);
    void add_pick(uint32 pick);
    void add_kill(uint32 id);
    void add_guard(uint32 id);
    void add_level(uint32 level);
    void add_event(uint32 id);

public:
    client::task_msg& get_update() { return _update; }
    
private:
    std::multimap<uint32, client::task_raid>  _need_raid;
    std::multimap<uint32, client::task_pick>  _need_pick;
    std::multimap<uint32, client::task_kill>  _need_kill;
    std::multimap<uint32, client::task_guard> _need_guard;
    std::multimap<uint32, client::task_level> _need_level;
    std::multimap<uint32, client::task_event> _need_event;

    player_role_task& _task;

    client::task_msg _update;
};

#endif // __SCENE_ROLE_TASK_H__
