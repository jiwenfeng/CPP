#include "cfg_parser.h"
#include "config_mgr.h"
#include "config_giant.h"

config_giant_mgr::config_giant_mgr(TTableGroup_s* p)
: _npc(p)
{
	init(p);
}

config_giant_mgr::~config_giant_mgr(void)
{
	clear();
}

int config_giant_mgr::init(TTableGroup_s* p)
{
	if (p == NULL) {
		return -1;
	}

	_npc = p;
	_table = _npc->GetTable("t_giant");
	if (_table == NULL) {
		printf("not found t_giant\n");
		return -2;
	}

	cfg_parser cp;
	for (int i = 1; i <= _table->m_recordCount; ++i) {
		uint32 id = _table->GetField      (i, "GiantID");
		if (id  == 0) {
			continue;
		}
		if (_cb.find(id) != _cb.end()) {
			printf("%s:%d %s, config_giant repeat id = %d\n", __FILE__, __LINE__, __FUNCTION__, id);
			continue;
		}
		config_npc* p = new config_npc;

        p->_npc_id               = _table->GetField      (i, "MonsterID");
        p->_npc_name             = _table->GetFieldString(i, "GiantName");
        p->_npc_type             = 2;
        p->_monster_level        = _table->GetField      (i, "GiantLevel");

        p->_attack_mode          = _table->GetField      (i, "GAttackMode"        );
		p->_attack_cool = _table->GetField(i, "GAttackCool");
        p->_random_move = _table->GetField(i, "randomMove");
        p->_move_cool = _table->GetField(i, "MoveCool");
        p->_experience           = 0;
        p->_random_move_range    = _table->GetField      (i, "GRandomMoveRange"   );
        p->_visible_range        = _table->GetField      (i, "GVisibleRange"      );
        p->_follow_range         = _table->GetField      (i, "GFollowRange"       );
        p->_default_skill        = _table->GetField      (i, "GDefaultSkill"      );
        p->_monster_ai           = _table->GetFieldString(i, "GiantAI"         );
        p->_rebirth_time         = 0;
        p->_corpse_remain_time   = _table->GetField      (i, "GCorpseRemainTime"  );
        p->_monster_difficulty_1 = 1;
		p->_collect_time_long	 = 0;

        cpf_0 p0(&p->_game_attribute);
        cp.parser<cpf_0>(_table->GetFieldString(i, "GiantAttribute").c_str(), ";", p0);

        char buf[32] = {0};
        for (int j = 1; j <= 9; ++j) {
            sprintf(buf, "GSkill%d", j);
            uint32 skill_id = _table->GetField(i, buf);
            if (skill_id != 0) {
                p->_skills.push_back(skill_id);
            }
        }
        p->_skills.push_back(p->_default_skill);
		_cb.insert(std::pair<uint32, config_npc *>(id, p));

		config_giant *cg = new config_giant;
		cg->_giant_id = id;
		cg->_giant_group = _table->GetField(i, "GiantGroup");
		cg->_level = _table->GetField(i, "GetGiant");
		_cg.insert(std::pair<uint32, config_giant *>(cg->get_giant_group(), cg));
	}

	return 0;
}

int config_giant_mgr::clear()
{
	for (std::map<uint32, config_npc*>::iterator i = _cb.begin(); i != _cb.end(); ++i) 
	{
		if (i->second != NULL) 
		{
			delete i->second;
		}
	}

	_cb.clear();

	std::multimap<uint32, config_giant *>::iterator j;
	for(j = _cg.begin(); j != _cg.end(); ++j)
	{
		if(j->second != NULL)
		{
			delete j->second;
		}
	}
	_cg.clear();

	return 0;
}

const config_npc* config_giant_mgr::get_config_giant(uint32 id) const
{
	std::map<uint32, config_npc *>::const_iterator i = _cb.find(id);
	if(i == _cb.end())
	{
		return NULL;
	}
	return i->second;
}

void config_giant_mgr::get_giant_unlock_group(uint32 lvl, vector<uint32> &v)
{
	std::multimap<uint32, config_giant *>::iterator i;
	for(i = _cg.begin(); i != _cg.end(); ++i)
	{
		if(i->second->get_level() <= lvl)
		{
			v.push_back(i->first);
		}
	}
}

uint32 config_giant_mgr::get_giant_id(uint32 group, uint32 lvl)
{
	std::multimap<uint32, config_giant *>::iterator i = _cg.find(group);
	for(; i != _cg.end(); ++i)
	{
		if(group != i->first)
		{
			return 0;
		}
		const config_npc *cn = get_config_giant(i->second->get_giant_id());
		if(cn == NULL)
		{
			continue;
		}
		if(cn->get_monster_level() == lvl)
		{
			return i->second->get_giant_id();
		}
	}
	return 0;
}
