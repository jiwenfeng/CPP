#include "sa_camp_mgr.h"

sa_camp_mgr::sa_camp_mgr()
{

}

sa_camp_mgr::~sa_camp_mgr()
{

}

int sa_camp_mgr::add_sa_camp(const sa_camp_t& c)
{
	uint32 size = sa_camps.size();
	if (size >= MAX_CAMP_NUM){
		return -1;
	}
	
	for (uint32 i = 0; i < size; ++i){
		if (sa_camps[i] == c){
			return -2;
		}
	}

	sa_camps.push_back(c);

	return 0;
}

sa_camp_t* sa_camp_mgr::get_min_role_sa_camp(uint32 strength)
{
	sa_camp_t* c = get_min_strength_sa_camp(strength);
    if (c != NULL) {
        return c;
    }

	uint32 min = DEFAUTL_SCENE_INST_USER_NUM;
	uint32 size = sa_camps.size();
	for (uint32 i = 0; i < size; ++i){
		if (sa_camps[i]._cur_role < min){
			c = &sa_camps[i];
			min = sa_camps[i]._cur_role;
		}
	}

	if (c == NULL) {
        if (size > 0) {
            return &sa_camps[0];
        }
		return NULL;
	}

	if (c->_cur_role >= c->_max_role) {
        return c;
		//return NULL;
	}

	return c;
}

sa_camp_t* sa_camp_mgr::get_min_strength_sa_camp(uint32 strength)
{
    uint32 size = sa_camps.size();
    uint32 min_strength = 0;
    uint32 min_index = 0;

    for (uint32 i = 0; i < size; ++i){
        if (sa_camps[i].max_strength == 0){
            sa_camps[i].max_strength = strength;
            return &sa_camps[i];
        }

        if (i == 0){
            min_strength = sa_camps[i].max_strength;
            min_index = i;
        } else {
            if (sa_camps[i].max_strength < min_strength){
                min_strength = sa_camps[i].max_strength;
                min_index = i;
            }
        }
    }

    if (min_index >= size){
        return NULL;
    }

    if (sa_camps[min_index].max_strength < strength){
        sa_camps[min_index].max_strength = strength;
        return &sa_camps[min_index];
    }

    return NULL;
}

sa_camp_t* sa_camp_mgr::get_win()
{
    sa_camp_t* win = NULL;
    uint32 size = sa_camps.size();
	for (uint32 i = 0; i < size; ++i) {
        if (win == NULL) {
            win = &sa_camps[i];
            continue;
        }
        
		if (win->_score < sa_camps[i]._score) {
            win = &sa_camps[i];
		}
	}

    return win;
}

sa_camp_t* sa_camp_mgr::get_sa_camp(uint32 id)
{
	uint32 size = sa_camps.size();
	for (uint32 i = 0; i < size; ++i){
		if (sa_camps[i]._id == id){
			return &sa_camps[i];
		}
	}

	return NULL;
}

uint32 sa_camp_mgr::get_sa_camp_num(uint32 id)
{
    uint32 size = sa_camps.size();
	for (uint32 i = 0; i < size; ++i){
		if (sa_camps[i]._id == id){
			return sa_camps[i]._cur_role;
		}
	}
    
    return 0;
}

void sa_camp_mgr::pack_battle_camp_score_list(client::battle_camp_score_list& bcsl)
{
	uint32 size = sa_camps.size();
	for (uint32 i = 0; i < size; ++i){
		sa_camps[i].pack_battle_camp_score(*bcsl.add_bcs());
	}
}

void sa_camp_mgr::clear()
{
    sa_camps.clear();
}

void sa_camp_mgr::leave(uint32 sa_camp_id)
{
    sa_camp_t* ct = get_sa_camp(sa_camp_id);
    --ct->_cur_role;
}
