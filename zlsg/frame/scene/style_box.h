#ifndef __STYLE_BOX_H__
#define __STYLE_BOX_H__

#include <vector>
#include <map>

using namespace std;

#include "rect.h"
#include "scene_define.h"

#define DEFAULT_SCREEN_WIDTH  1200 // 400 //320 // 
#define DEFAULT_SCREEN_HEIGHT 800  // 300 //210 // 

#define DEFAULT_OTHER_SCREEN_WIDTH  750 // 1200
#define DEFAULT_OTHER_SCREEN_HEIGHT 500 // 800

class scene_role;
class scene_npc;
class scene_pet;
class scene_user;

class style_box
{
public:
    style_box()
    {
        box_x = 0;
        box_y = 0;
    }

    virtual ~style_box()
    {

    }

    inline int get_box_x()
    {
        return box_x;
    }

    inline void set_box_x(int x)
    {
        box_x = x;
    }

    inline int get_box_y()
    {
        return box_y;
    }

    inline void set_box_y(int y)
    {
        box_y = y;
    }

    rect& get_rect()
    {
        return _rt;
    }

    const rect& get_rect() const
    {
        return _rt;
    }

    void set_rect(rect& rhs)
    {
        _rt = rhs;
    }

    void add_scene_user(uint64 inst_id, uint32 role_id);
    void add_scene_npc(uint64 inst_id, uint32 role_id);

    void remove_scene_user(uint64 inst_id);
    void remove_scene_npc(uint64 inst_id);
    void remove_all_scene_npc();

    void get_roles(vector<uint64>& role_list, bool get_user = true, bool get_npc = true);
    void get_users(vector<uint64>& user_list);
    void get_npcs(vector<uint64>& npc_list);

private:
    int  box_x;
    int  box_y;
    rect _rt;
    map<uint64, uint32> scene_role_map[2];
};

class space_style_box
{
public:
    space_style_box();

    virtual ~space_style_box();

public:
    void init(const rect& _rt, int width = DEFAULT_SCREEN_WIDTH, int height = DEFAULT_SCREEN_HEIGHT);

    bool is_neer(const postion& p1, const postion& p2);

    style_box* get_style_box(const postion& p) const;

    void get_style_boxs(const postion& p, vector<style_box*>& boxs) const;

    bool get_style_move_boxs(const postion& from, const postion& to, std::vector<style_box*>& push, std::vector<style_box*>& move, std::vector<style_box*>& left) const;

    bool is_change(const postion& from, const postion& to) const;

    inline int get_screen_width()
    {
        return screen_width;
    }

    inline int get_screen_height()
    {
        return screen_height;
    }

    void remove_scene_role(style_box* box, scene_role* sr);

private:
    int screen_width;
    int screen_height;

    typedef vector<style_box*> seq_box;
    vector<seq_box> _array;
};

#endif
