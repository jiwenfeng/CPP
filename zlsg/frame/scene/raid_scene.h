#ifndef __RAID_SCENE_H__
#define __RAID_SCENE_H__

#include "scene.h"

class config_raid;

// ���������࣬������Ҫ����������ص�����������˸�����Ϣ������ϸ�ڡ���������
class raid_scene : public scene
{
public:
    raid_scene(const config_map* p, const config_raid* cr);
    virtual ~raid_scene();

public:
    virtual int init();

public:
    inline const config_raid*             get_config_raid()     const { return _cr;                      }
    uint32 get_type()          const ;
    uint32 get_raid_id()       const ;
    uint32 get_min_team_num()  const ;
    int    get_raid_type()     const ;
    uint32 get_standard_time() const ;
    uint32 get_battle_ui_id()  const ;

public:
    const uint32 get_evaluation(uint32 tenms) const;
    uint32 cal_npc_lose(const std::vector<const origin_npc*>& npcs, std::vector<client::props_info>& pis) const;
    uint32 get_falling_objects(std::vector<client::props_info>& pis) const;

protected:

};

#endif
