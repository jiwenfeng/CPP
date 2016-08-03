#ifndef __CAMP_MGR_H__
#define __CAMP_MGR_H__

#include "scene_define.h"

#define MAX_STRENGTH_ROLE_NUM 3

class camp_t 
{
public:
	uint32 _id;
	uint32 _max_role;
	uint32 _cur_role;
	uint32 _score;
	uint32 _born_x;
	uint32 _born_y;
	uint32 _born_z;

    uint32 max_strength;
public:
	camp_t()
	{
		_id = 0;
		_max_role = 0;
		_cur_role = 0;
		_score = 0;
		_born_x = 0;
		_born_y = 0;
		_born_z = 0;
        max_strength = 0;
	}

	camp_t(uint32 id, uint32 max_role)
	{
		_id = id;
		_max_role = max_role;
		_cur_role = 0;
		_score = 0;
		_born_x = 0;
		_born_y = 0;
		_born_z = 0;
        max_strength = 0;
	}

	camp_t(uint32 id, uint32 max_role, uint32 x, uint32 y, uint32 z)
	{
		_id = id;
		_max_role = max_role;
		_cur_role = 0;
		_score = 0;
		_born_x = x;
		_born_y = y;
		_born_z = z;
        max_strength = 0;
	}

	~camp_t()
	{
		
	}

	bool operator==(const camp_t& rhs)
	{
		return rhs._id == _id;
	}

	void pack_battle_camp_score(client::battle_camp_score& bcs)
	{
		bcs.set_camp_id(_id);
		bcs.set_score(_score);
	}
};

class camp_mgr
{
public:
	camp_mgr();

	virtual ~camp_mgr();

	int add_camp(const camp_t& c);

	bool has_camp(){return camps.size() > 0;}

    uint32 get_camp_num(uint32 id);

	camp_t* get_camp(uint32 id);

    camp_t* get_min_role_camp(uint32 strength);

    camp_t* get_win();

    void clear();

    void pack_battle_camp_score_list(client::battle_camp_score_list& bcsl);

    camp_t* get_min_strength_camp(uint32 strength);

    void leave(uint32 camp_id);

protected:
	vector<camp_t> camps;
};

#endif
