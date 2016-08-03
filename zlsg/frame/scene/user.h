#ifndef __USER_H__
#define __USER_H__

class player_role;

namespace miniserver
{
	class user
	{
	public:
		user(player_role* r);
		virtual ~user();

	public:
		inline player_role& get_player_role() { return *_spr; }
		player_role* detach_player_role();

	protected:
		player_role* _spr;
	};

}


#endif
