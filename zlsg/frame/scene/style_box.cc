#include <stdlib.h>
#ifndef _WIN32
#include <inttypes.h>
#endif // _WIN32
#include "style_box.h"
#include "scene_npc.h"
#include "scene_user.h"

void style_box::add_scene_user(uint64 inst_id, uint32 role_id)
{
    scene_role_map[RTE_USER][inst_id] = role_id;
}

void style_box::add_scene_npc(uint64 inst_id, uint32 role_id)
{
    scene_role_map[RTE_NPC][inst_id] = role_id;
}

void style_box::remove_scene_user(uint64 inst_id)
{
    scene_role_map[RTE_USER].erase(inst_id);
}

void style_box::remove_scene_npc(uint64 inst_id)
{
    scene_role_map[RTE_NPC].erase(inst_id);
}

void style_box::remove_all_scene_npc()
{
    scene_role_map[RTE_NPC].clear();
}

void style_box::get_roles(vector<uint64>& role_list, bool get_user, bool get_npc)
{
    if (get_user) {
        get_users(role_list);
    }

    if (get_npc) {
        get_npcs(role_list);
    }
}

void style_box::get_users(vector<uint64>& user_list)
{
    map<uint64, uint32>::iterator it;
    for (it = scene_role_map[RTE_USER].begin(); it != scene_role_map[RTE_USER].end(); ++it){
        user_list.push_back(it->first);
    }
}

void style_box::get_npcs(vector<uint64>& npc_list)
{
    map<uint64, uint32>::iterator it;
    for (it = scene_role_map[RTE_NPC].begin(); it != scene_role_map[RTE_NPC].end(); ++it){
        npc_list.push_back(it->first);
    }
}

// ------------------------------------------------------------------------------------------------

space_style_box::space_style_box()
{
    screen_width  = 1;
    screen_height = 1;
}

space_style_box::~space_style_box()
{
    int wcount = _array.size();
    for (int x = 0; x < wcount; ++x){
        int hcount = _array[x].size();
        for (int y = 0; y < hcount; ++y){
            if (_array[x][y] != NULL){
                delete _array[x][y];
                _array[x][y] = NULL;
            }
        }
    }
}

void space_style_box::init(const rect& _rt, int width, int height)
{
    int wcount = (_rt.width() + width - 1) / width;
    int hcount = (_rt.height() + height - 1) / height;
    //printf("space_style_box::init screen_width:%d screen_height:%d wcount:%d hcount:%d map_rect:", width, height, wcount, hcount);
    _rt.print();
    //printf("\n");

    _array.resize(wcount);
    for (int x = 0 ; x < wcount; ++x){
        _array[x].resize(hcount);
    }

    for (int x = 0; x < wcount; ++x){
        for (int y = 0; y < hcount; ++y){
            style_box* box = new style_box();
            box->set_box_x(x);
            box->set_box_y(y);
            box->get_rect().set(x * width, x * height, x * width + width -1, y * height + height -1);
            
            box->get_rect().print();
            
            _array[x][y] = box;
        }
        //printf("\n");
    }

    screen_width = width;
    screen_height = height;
}

bool space_style_box::is_neer(const postion& p1, const postion& p2)
{
    int x1 = p1.x / screen_width;
    int y1 = p1.y / screen_height;

    int x2 = p2.x / screen_width;
    int y2 = p2.y / screen_height;

    return abs( x1 - x2 ) <= 1 && abs( y1 - y2 ) <= 1;
}

style_box* space_style_box::get_style_box(const postion& p) const
{
    int x = p.x / screen_width;
    int y = p.y / screen_height;

    if ((uint32)x < _array.size()){
        if ((uint32)y < _array[x].size()){
            return _array[x][y];
        }
    }

    return NULL;
}

void space_style_box::get_style_boxs(const postion& p, vector<style_box*>& boxs) const
{
    boxs.clear();
    int x = p.x / screen_width;
    int y = p.y / screen_height;
    int x_start = x - 1;
    int y_start = y - 1;
    int x_end   = x + 1;
    int y_end   = y + 1;

    if (x_start < 0) {
        x_start = 0;
    }
    
    if (y_start < 0) {
        y_start = 0;
    }
    
    if (_array.size() == 0) {
        return;
    }
    
    if (x_end >= (int)_array.size()) {
        x_end = (int)_array.size() - 1;
    }
    
    if (y_end >= (int)_array[0].size()) {
        y_end = (int)_array[0].size() -1;
    }

    for (x = x_start; x <= x_end; ++x) {
        for (y = y_start; y <= y_end; ++y) {
            boxs.push_back(_array[x][y]);
        }
    }
}

bool space_style_box::get_style_move_boxs(const postion& from, const postion& to, std::vector<style_box*>& push, std::vector<style_box*>& move, std::vector<style_box*>& left) const
{
    // printf("space_style_box::get_style_move_boxs[x:%lu,y:%lu] (%d,%d)->(%d,%d)\n", _array.size(), _array[0].size(), from.x, from.y, to.x, to.y);
    push.clear();
    move.clear();
    left.clear();

    int x_from = from.x / screen_width;
    int y_from = from.y / screen_height;
    int x_to = to.x / screen_width;
    int y_to = to.y / screen_height;
    if (x_from == x_to && y_from == y_to) {
        get_style_boxs(to, move);
        return false;
    }
//     printf("space_style_box::get_style_move_boxs:[%d,%d]------------->[%d,%d]\n", x_from, y_from, x_to, y_to);

    get_style_boxs(to, push);
    std::vector<style_box*>::iterator it;

//     for (it = boxs_to.begin(); it != boxs_to.end(); ++it) {
//         style_box* p = (*it);
//         // 左左
//         if (x_to > x_from && p->get_box_x() > x_to) {
//             push.push_back(*it);
//             continue;
//         }
//         // 右右
//         if (x_to < x_from && p->get_box_x() < x_to) {
//             push.push_back(*it);
//             continue;
//         }
//         // 下下
//         if (y_to > y_from && p->get_box_y() > y_to) {
//             push.push_back(*it);
//             continue;
//         }
//         // 上上
//         if (y_to < y_from && p->get_box_y() < y_to) {
//             push.push_back(*it);
//             continue;
//         }
// 
//         move.push_back(*it);
//     }
// 
//     for (uint32 i = 0; i < push.size(); ++i){
//         printf("[%d,%d]", push[i]->get_box_x(), push[i]->get_box_y());
//     }
//     printf("--------------push_box end-------------\n");

    std::vector<style_box*> boxs_from;
    get_style_boxs(from, boxs_from);

//     for (it = boxs_from.begin(); it != boxs_from.end(); ++it){
//         style_box* p = (*it);
//         // 左右
//         if (x_to > x_from && p->get_box_x() < x_from){
//             left.push_back(*it);
//             continue;
//         }
//         // 右左
//         if (x_to < x_from && p->get_box_x() > x_from){
//             left.push_back(*it);
//             continue;
//         }
//         // 下上
//         if (y_to > y_from && p->get_box_y() < y_from){
//             left.push_back(*it);
//             continue;
//         }
//         // 上下
//         if (y_to < y_from && p->get_box_y() > y_from){
//             left.push_back(*it);
//             continue;
//         }
//     }

    bool same = false;
    for (size_t i = 0; i < boxs_from.size(); ++i) {
        same = false;
        for (it = push.begin(); it != push.end(); ++it) {
            if (boxs_from[i] == *it) {
                move.push_back(*it);
                push.erase(it);
                same = true;
                break;
            }
        }

        if (!same) {
            left.push_back(boxs_from[i]);
        }
    }

//     for (size_t i = 0; i < del.size(); ++i) {
//         printf("[%d,%d] ", del[i]->get_box_x(), del[i]->get_box_y());
//     }
//     printf("--------------del end-------------\n");
//     for (size_t i = 0; i < old.size(); ++i) {
//         printf("[%d,%d] ", old[i]->get_box_x(), old[i]->get_box_y());
//     }
//     printf("--------------old end-------------\n");
//     for (size_t i = 0; i < boxs_to.size(); ++i) {
//         printf("[%d,%d] ", boxs_to[i]->get_box_x(), boxs_to[i]->get_box_y());
//     }
//     printf("--------------new end-------------\n");
// 
//     for (uint32 i = 0; i < left.size(); ++i) {
//         printf("[%d,%d]", left[i]->get_box_x(), left[i]->get_box_y());
//     }
//     printf("--------------left_box end-------------\n");

    return true;
}

bool space_style_box::is_change(const postion& from, const postion& to) const
{
    int x_from = from.x / screen_width;
    int y_from = from.y / screen_height;
    int x_to = to.x / screen_width;
    int y_to = to.y / screen_height;

    return (x_from != x_to || y_from != y_to);
}

void space_style_box::remove_scene_role(style_box* box, scene_role* sr)
{
    if (box == NULL || sr == NULL) {
        return ;
    }

    if (sr->get_role_type() == RTE_USER) {
        box->remove_scene_user(sr->get_inst_id());
    } else {
        box->remove_scene_npc(sr->get_inst_id());
    }
}
