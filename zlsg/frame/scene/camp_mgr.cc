#include "camp_mgr.h"

camp_mgr::camp_mgr()
{

}

camp_mgr::~camp_mgr()
{

}

int camp_mgr::add_camp(const camp_t& c)
{
	uint32 size = camps.size();
	if (size >= MAX_CAMP_NUM) {
		return -1;
	}
	
	for (uint32 i = 0; i < size; ++i) {
		if (camps[i] == c) {
			return -2;
		}
	}

	camps.push_back(c);

	return 0;
}

camp_t* camp_mgr::get_min_role_camp(uint32 strength)
{
	camp_t* c = get_min_strength_camp(strength);
    if (c != NULL) {
        return c;
    }

	uint32 min = DEFAUTL_SCENE_INST_USER_NUM;
	uint32 size = camps.size();
	for (uint32 i = 0; i < size; ++i) {
		if (camps[i]._cur_role < min) {
			c = &camps[i];
			min = camps[i]._cur_role;
		}
	}

	if (c == NULL) {
        if (size > 0) {
            return &camps[0];
        }
		return NULL;
	}

	if (c->_cur_role >= c->_max_role) {
        return c;
		//return NULL;
	}

	return c;
}

camp_t* camp_mgr::get_min_strength_camp(uint32 strength)
{
    uint32 size = camps.size();
    uint32 min_strength = 0;
    uint32 min_index = 0;

    for (uint32 i = 0; i < size; ++i) {
        if (camps[i].max_strength == 0) {
            camps[i].max_strength = strength;
            return &camps[i];
        }

        if (i == 0) {
            min_strength = camps[i].max_strength;
            min_index = i;
        } else {
            if (camps[i].max_strength < min_strength) {
                min_strength = camps[i].max_strength;
                min_index = i;
            }
        }
    }

    if (min_index >= size) {
        return NULL;
    }

    if (camps[min_index].max_strength < strength) {
        camps[min_index].max_strength = strength;
        return &camps[min_index];
    }

    return NULL;
}

camp_t* camp_mgr::get_win()
{
    camp_t* win = NULL;
    uint32 size = camps.size();
	for (uint32 i = 0; i < size; ++i) {
        if (win == NULL) {
            win = &camps[i];
            continue;
        }
        
		if (win->_score < camps[i]._score) {
            win = &camps[i];
		}
	}

    return win;
}

camp_t* camp_mgr::get_camp(uint32 id)
{
	uint32 size = camps.size();
	for (uint32 i = 0; i < size; ++i) {
		if (camps[i]._id == id) {
			return &camps[i];
		}
	}

	return NULL;
}

uint32 camp_mgr::get_camp_num(uint32 id)
{
    uint32 size = camps.size();
	for (uint32 i = 0; i < size; ++i) {
		if (camps[i]._id == id) {
			return camps[i]._cur_role;
		}
	}
    
    return 0;
}

void camp_mgr::pack_battle_camp_score_list(client::battle_camp_score_list& bcsl)
{
	uint32 size = camps.size();
	for (uint32 i = 0; i < size; ++i) {
		camps[i].pack_battle_camp_score(*bcsl.add_bcs());
	}
}

void camp_mgr::clear()
{
    camps.clear();
}

void camp_mgr::leave(uint32 camp_id)
{
    camp_t* ct = get_camp(camp_id);
    --ct->_cur_role;
}
