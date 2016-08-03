#include "player_role.h"
#include "user.h"

namespace miniserver
{
	user::user(player_role* r)
		: _spr(r)
	{

	}

	user::~user()
	{
		if (_spr != NULL) {
			delete _spr;
			_spr = NULL;
		}
	}

	player_role* user::detach_player_role()
	{
		player_role* tmp = _spr;
		_spr = NULL;

		return tmp;
	}

}
