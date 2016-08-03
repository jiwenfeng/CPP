#include "battle_user_score.h"

battle_user_score::battle_user_score()
{
	role_id = 0;
	score = 0;
}

battle_user_score::battle_user_score(uint32 rid)
{
	role_id = rid;
	score = 0;
}

battle_user_score::~battle_user_score()
{

}

void battle_user_score::add_score(uint32 s)
{
	score += s;
}

void battle_user_score::pack_battle_user_score(client::battle_score& bs)
{
	bs.set_role_id(role_id);
	bs.set_score(score);
}
