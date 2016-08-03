#include "cfg_parser.h"
#include "config_props.h"
#include "config_quest.h"
#include "config_mgr.h"
#include "config_npc.h"

config_quest::config_quest()
{

}

config_quest::~config_quest()
{

}


void config_quest::get_reward_item(uint32 id, std::vector<client::props_info>& items) const
{
    std::vector<quest_reward_item>::const_iterator i = _reward_item.begin();
    for ( ; i != _reward_item.end(); ++i) {
        if (id == (uint32)i->profession || ((id == 2 || id == 1) && i->profession == 4) || i->profession == 5) {
			const config_props* pcp = CONFMGR->get_config_props(i->id);
			if(!pcp)
			{
				continue;
			}
            client::props_info pi;
			if(make_props(i->id, i->num, pi) == 0) {
                items.push_back(pi);
            }
        }
    }
}

config_quest_mgr::config_quest_mgr() 
{

}

config_quest_mgr::~config_quest_mgr()
{
    clear();
}

int config_quest_mgr::init(TTableGroup_s* pGroup)
{
    if (pGroup == NULL) {
        return -1;
    }

    TTable* ptable = pGroup->GetTable("t_mainquest");
    if (ptable == NULL) {
        printf("not found t_mainquest\n");
        return -2;
    }

    cfg_parser cp;
    for (int i = 1; i <= ptable->m_recordCount; ++i) {
        if (ptable->GetField(i, "QuestID") == 0) {
            continue;
        }

        config_quest* p = new config_quest;
        p->_map_id         = ptable->GetField      (i, "MapID"              );
        p->_quest_id       = ptable->GetField      (i, "QuestID"            );
        p->_next_quest_id  = ptable->GetField      (i, "NextQuestID"        );
        p->_accept_type    = ptable->GetField      (i, "AcceptType"         );
        p->_quest_type     = ptable->GetField      (i, "QuestType"          );
        p->_need_level     = ptable->GetField      (i, "NeedLevel"          );
        p->_other          = ptable->GetField      (i, "Other"              );
        p->_reward_exp     = ptable->GetField      (i, "RewardExp"          );
        p->_reward_taolue  = ptable->GetField      (i, "RewardTaolue"       );
        p->_reward_gold    = ptable->GetField      (i, "RewardGold"         );
        p->_reward_energy  = ptable->GetField      (i, "RewardEnergy"       );
        p->_reward_coin    = ptable->GetField      (i, "RewardCoin"         );

        cpf_0 f0;
        f0.set_cfg(&p->_need_item);
        cp.parser<cpf_0>(ptable->GetFieldString(i, "NeedItem").c_str(), ";", f0);

        f0.set_cfg(&p->_kill_monster);
        cp.parser<cpf_0>(ptable->GetFieldString(i, "KillMonster").c_str(), ";", f0);

        f0.set_cfg(&p->_guard_monster);
        cp.parser<cpf_0>(ptable->GetFieldString(i, "GuardMonster").c_str(), ";", f0);

        cpf_quest_reward_item f01(&p->_reward_item);
        cp.parser<cpf_quest_reward_item>(ptable->GetFieldString(i, "RewardItem").c_str(), ";", f01);

        _content[p->_quest_id] = p; 
    }

    return 0;
}

int config_quest_mgr::clear()
{
    std::map<uint32, config_quest*>::iterator i = _content.begin();
    while (i != _content.end()) {
        if (i->second != NULL) {
            delete i->second;
        }

        ++i;
    }

    _content.clear();

    return 0;
}

const config_quest* config_quest_mgr::get_config_quest(uint32 id) const
{
    std::map<uint32, config_quest*>::const_iterator i = _content.find(id);
    if (i == _content.end()) {
        return NULL;
    }

    return i->second;
}
