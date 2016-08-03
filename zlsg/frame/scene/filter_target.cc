#include "scene_role.h"
#include "scene_user.h"
#include "filter_target.h"

class filter_target_peace
{
public:
	bool operator()(scene_role* psr) {
		return psr->get_real_role_type() == RTE_USER;
	}
};

class filter_target_team
{
public:
	filter_target_team(uint32 team_id) : _team_id(team_id) {}
	bool operator()(scene_role* psr) {
		return (psr->get_real_role_type() == RTE_USER) && (psr->get_team_id() == _team_id);
	}

private:
	uint32 _team_id;
};

class filter_target_family
{
public:
	filter_target_family(uint32 family_id) : _family_id(family_id) {}
	bool operator()(scene_role* psr) {
		scene_user *su = NULL;
		if(psr->get_role_type() == RTE_USER)
		{
			su = static_cast<scene_user *>(psr);
		}
		else
		{
			su = static_cast<scene_user *>(psr->get_master());
		}
		return (su->get_family_id() == _family_id) || (su->is_friend_family(_family_id));
	}
private:
	uint32 _family_id;
};

class filter_target_all
{
public:
	bool operator()(scene_role* psr) {
		return false;
	}

};

class filter_target_camp
{
public:
	filter_target_camp(uint32 camp) : _camp(camp) {}
	bool operator()(scene_role *psr) {
		return psr->get_camp() == _camp;
	}
private:
	uint32 _camp;
};

filter_target::filter_target()
{}

filter_target::~filter_target()
{}

void filter_target::filter_target_by_atk_mode(scene_role* psr, std::vector<scene_role*>& enemies, std::vector<scene_role*>& friends)
{
	//	std::vector<scene_role*>::iterator remove_begin;
	switch (psr->get_status().atk_mode()) {
		case client::role_status_attack_mode_attack_mod_peace:
			{
				vector<scene_role *>::iterator it;
				filter_target_peace ftp;
				for(it = enemies.begin(); it != enemies.end(); )
				{
					if(ftp(*it))
					{
						friends.push_back(*it);
						it = enemies.erase(it);
					}
					else
					{
						it++;
					}
				}
			}
			break;
		case client::role_status_attack_mode_attack_mod_team:
			//remove_begin = remove_if(enemies.begin(), enemies.end(), filter_target_team(psr->get_team_id()));
			break;
		case client::role_status_attack_mode_attack_mod_family:
			{
				vector<scene_role *>::iterator it;
				filter_target_family ftf(psr->get_family_id());
				for(it = enemies.begin(); it != enemies.end();)
				{
					if(ftf(*it))
					{
						friends.push_back(*it);
						it = enemies.erase(it);
					}
					else
					{
						it++;
					}
				}
			}
			break;
			//case client::role_status_attack_mode_attack_mod_all:
			//    std::vector<scene_role*>::iterator i = remove_if(enemies.begin(), enemies.end(), filter_target_all());
			//    break;
		default:
			//			remove_begin = enemies.end();
			break;
	}

	//	if (remove_begin != enemies.end()) {
	//		enemies.erase(remove_begin, enemies.end());
	//	}
}
