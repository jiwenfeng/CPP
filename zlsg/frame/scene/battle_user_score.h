#ifndef __BATTLE_USER_SCORE_H__
#define __BATTLE_USER_SCORE_H__

#include "scene_define.h"

class battle_user_score
{
public:
	battle_user_score();

	battle_user_score(uint32 rid);

	virtual ~battle_user_score();

	uint32 get_score(){return score;}

	void add_score(uint32 s);

	void pack_battle_user_score(client::battle_score& bs);

private:
	uint32 role_id;
	uint32 score;
};

#endif
