#ifndef __GAME_MAP_H__
#define __GAME_MAP_H__

#include "scene_define.h"

class scene;

// ��Ϸ��ͼ�࣬һ����ͼ�а��������������һϵ��ͬһ���͵ĳ������ü���
class game_map  
{
public:
    game_map(const config_map* p);
    virtual ~game_map();

public:
    int init();

public:
    inline const config_map* get_config_map() const { return _mc; }
    inline const config_raid* get_config_raid() const { return _cr; }

    const scene* get_scene(uint32 scene_id) const;

    inline uint32 get_id()       const { return _map_id; }
    inline uint32 get_type()     const { return _map_type; }
    inline uint32 get_sub_type() const { return _sub_type; }
    const std::map<uint32, scene*>& get_scene_map() const { return _scenes; }

    bool is_pre_create() const;

    inline uint32 get_min_lvl() const { return _min_lvl; }

private:
    uint32 get_map_type(uint32 map_id);
    uint32 get_map_sub_type(uint32 map_id);

private:
    const config_map*        _mc;
    const config_raid*       _cr;

    uint32                   _map_id;      // ��ͼID
    uint32                   _map_type;    // ��ͼ����
    uint8                    _sub_type;    // ��ͼ�ķ�����
    uint8                    _min_lvl;     // ��С�ȼ�����
    uint8                    _max_lvl;     // ���ȼ�����

    uint16                   _scene_num;   // �����ĳ���ID����
    std::map<uint32, scene*> _scenes;      // �õ�ͼ�����ĳ���, scene_id--->scene
};

#endif
