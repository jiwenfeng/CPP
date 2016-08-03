#ifndef __SCENE_LOSE_MGR_H__
#define __SCENE_LOSE_MGR_H__

#include "scene_define.h"

#define DEFAULT_DROP_PROP_NUM 500

class scene_lose : public client::drop_props_info
{
public:
    scene_lose() { }
    scene_lose(const client::drop_props_info& dpi) {
        this->CopyFrom(dpi);
    }

    virtual ~scene_lose() {
    }

public:
    scene_lose& operator=(const scene_lose& rhs) {
        this->CopyFrom(rhs);
        return *this;
    }

    void reset() {
    }

    bool can_pick(uint32 role_id); 

public:

};

class scene_inst;

class scene_lose_mgr
{
public:
    scene_lose_mgr(scene_inst* si);
    virtual ~scene_lose_mgr();

public:
    int on_update(uint32 tick);
	bool can_pick(uint32 index, uint32 role_id);
    
public:
    uint32      generate_index() { return ++_index; }
    scene_lose& generate_lose();
    void        add_lose(const client::drop_props_info* dpi);
    scene_lose* get_lose(uint32 index);
    void        pick_lose(uint32 index);
	inline const std::map<uint32, scene_lose> &get_all_lost() { return _loses; }

    bool fill_loses(client::drop_props_msg& msg);

private:
    uint32                       _index;
    std::map<uint32, scene_lose> _loses;
    scene_inst*                  _si;
};

#endif
