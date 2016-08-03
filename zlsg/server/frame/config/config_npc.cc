#include "random.h"
#include "cfg_parser.h"
#include "client.role_props.pb.h"
#include "config_mgr.h"
#include "config_props.h"
#include "config_fallitem.h"
#include "config_npc.h"
#ifndef __LIB_MINI_SERVER__
#include "config_robot.h"
#endif

void rand_attr(const config_props *cp, client::props_info &pi, const vector<int> &attr)
{
    const std::map<int, cpf_val_3>& attr_map = cp->get_random_attribute();
	if(!attr.empty())
	{
		for(size_t i = 0; i < attr.size(); ++i)
		{
			std::map<int, cpf_val_3>::const_iterator j = attr_map.find(attr[i]);
			if(j == attr_map.end())
			{
				continue;
			}
            client::attribute* pa = pi.add_attr();
            pa->set_type(j->second._type);
            pa->set_value(j->second._min);
		}
		return ;
	}
	uint32 rand_size = cp->get_name_color_index();
	if(rand_size == 0 || cp->get_random_attribute().empty() || rand_size > 4)
	{
		return ;
	}
	uint32 denominator = 0;
	std::map<int, cpf_val_3>::const_iterator j = attr_map.begin();
	for (;j != attr_map.end(); ++j)
	{
		denominator += j->second._ratio;
	}
	for (uint32 i = 0; i < rand_size; ++i) 
	{
		uint32 rand_value = mtrandom::rand_int32(denominator);
		uint32 attenuation = 0;
		for (j = attr_map.begin() ; j != attr_map.end(); ++j) 
		{
			attenuation += j->second._ratio;
			if (rand_value >= attenuation) 
			{
				continue;
			}
			client::attribute* pa = pi.add_attr();
			pa->set_type(j->second._type);
			pa->set_value(j->second._min);
			break;
		}
	}
}

static uint32 fix_timeout(uint32 type, uint32 timeout, uint32 now, uint32 ts)
{
	switch(type)
	{
		case 0:
			timeout = 0;
			break;
		case 1:
			timeout = timeout;
			break;
		case 4:
			timeout = ts + timeout;
			break;
		default:
			timeout = now + timeout;
			break;
	}

	return timeout;
}

static void merger_props(std::vector<client::props_info> &v, client::props_info &pi, bool merger)
{
	const config_props *cp = CONFMGR->get_config_props(pi.props_id());
	if(NULL == cp)
	{
		return ;
	}
	if(!merger)
	{
		v.push_back(pi);
		return ;
	}
	uint32 limit = cp->get_total();
	if(0 == limit)
	{
		v.push_back(pi);
		return ;
	}
	for(size_t i = 0; i < v.size(); ++i)
	{
		if(v[i].props_id() != pi.props_id())
		{
			continue;
		}
		if(v[i].timeout() != pi.timeout())
		{
			continue;
		}
		if(v[i].count() + pi.count() <= limit)
		{
			v[i].set_count(v[i].count() + pi.count());
			return ;
		}
		pi.set_count(v[i].count() + pi.count() - limit);
		v[i].set_count(limit);
	}
	while(pi.count() > 0)
	{
		if(pi.count() <= limit)
		{
			v.push_back(pi);
			return ;
		}
		client::props_info p(pi);
		p.set_count(limit);
		pi.set_count(pi.count() - limit);
		v.push_back(p);
	}
}

int make_props(uint32 drop_id, vector<client::props_info> &vpi, uint32 job, uint32 ts, uint32 diff, bool merger)
{
	const config_drop *cd = CONFMGR->get_config_drop(drop_id);
	if(NULL == cd)
	{
		return -1;
	}
	uint32 now = (uint32)time(NULL);
	const std::vector<drop_content> &dc = cd->get_drop_content();
	for(size_t i = 0; i < dc.size(); ++i)
	{
		if((uint32)dc[i].lv < diff)
		{
			continue;
		}
		if(dc[i].job != 5 && (uint32)dc[i].job != job)
		{
			continue;
		}
		uint32 flag = false;
		for(int j = 0; j < dc[i].count; ++j)
		{
			if(mtrandom::rand_int32(dc[i].ratio_denominator) > (uint32)dc[i].ratio_molecular)
			{
				continue;
			}
			const drop_content *d = NULL;
			uint32 n = 1;
			if(dc[i].type == 1)		// µôÂä×é
			{
				d = CONFMGR->get_config_packet(dc[i].id);
				if(NULL == d)
				{
					continue;
				}
				n = d->count;
			}
			else
			{
				d = &dc[i];
				const config_props *cps = CONFMGR->get_config_props(d->id);
				if(cps == NULL)
				{
					break;
				}
				if(cps->get_item_type() == 0 || d->ratio_molecular == d->ratio_denominator)
				{
					n = d->count;
					flag = true;
				}
			}
			uint32 t = fix_timeout(d->to_type, d->timeout, now, ts);
			client::props_info pi;
			if(0 == make_props(d, pi, n, t))
			{
				merger_props(vpi, pi, merger);
			}
			if(flag)
			{
				break;
			}
		}
	}
	return 0;
}

int make_props(uint32 props_id, uint32 n, vector<client::props_info> &v)
{
	const config_props *cp = CONFMGR->get_config_props(props_id);
	if(NULL == cp)
	{
		return -1;
	}
	client::props_info pi;
	pi.set_pos(0);
	pi.set_grid_index(0);
	pi.set_props_id(props_id);
	pi.set_count(n);
	pi.set_quality(cp->get_quality());
	pi.set_bind(cp->get_bind_type());
	pi.set_level(0);
	rand_attr(cp, pi);
	v.push_back(pi);
	return 0;
}

int make_props(const uint32 props_id, const uint32 n, client::props_info &pi)
{
	const config_props *cp = CONFMGR->get_config_props(props_id);
	if(NULL == cp)
	{
		return -1;
	}
	pi.set_pos(0);
	pi.set_grid_index(0);
	pi.set_props_id(props_id);
	pi.set_count(n);
	pi.set_quality(cp->get_quality());
	pi.set_bind(cp->get_bind_type());
	pi.set_level(0);
	rand_attr(cp, pi);
	return 0;
}

int make_props(const drop_content *dc, client::props_info &pi, uint32 n, uint32 timeout)
{
	const config_props *cp = CONFMGR->get_config_props(dc->id);
	if(NULL == cp)
	{
		return -1;
	}
	pi.set_pos(0);
	pi.set_grid_index(0);
	pi.set_props_id(dc->id);
	pi.set_count(n);
	pi.set_quality(cp->get_quality());
	pi.set_bind(cp->get_bind_type());
	pi.set_level(0);
	if(dc->to_type != 0)
	{
		pi.set_to_type(dc->to_type);
		pi.set_timeout(timeout);
	}
	rand_attr(cp, pi, dc->attr);
	return 0;
}

config_drop::config_drop(void)
{
}

config_drop::~config_drop(void)
{
}

config_npc::config_npc(void)
{
}

config_npc::~config_npc(void)
{
}

config_npc_mgr::config_npc_mgr(TTableGroup_s* p)
    : _npc(p)
{
    init(p);
}


config_npc_mgr::~config_npc_mgr(void)
{
    clear();
}

int config_npc_mgr::init_drop(TTableGroup_s* ptg)
{
    if (ptg == NULL) {
        return -1;
    }

    _drop = ptg;
    _table = _drop->GetTable("npc_drop");
    if (_table == NULL) {
        printf("not found npc_drop\n");
        return -2;
    }

    cfg_parser cp;
    for (int i = 1; i <= _table->m_recordCount; ++i) {
        if (_table->GetField      (i, "DropID"   ) == 0) {
            continue;
        }

        config_drop* p = new config_drop;
        p->_drop_id             = atoi(_table->GetFieldString(i, "DropID"         ).c_str());

        cpf_drop_content p2(&p->_drop_content);
        cp.parser<cpf_drop_content>(_table->GetFieldString(i, "DropContent").c_str(), ";", p2);

//		for(size_t i = 0; i < p->_drop_content.size(); ++i)
//		{
//			p->_drop_content[i].debug();
//		}

        _cd.insert(std::pair<uint32, config_drop*>(p->_drop_id, p));
    }

    return 0;
}

int config_npc_mgr::init(TTableGroup_s* p)
{
    if (p == NULL) {
        return -1;
    }

    _npc = p;
    _table = _npc->GetTable("t_monster");
    if (_table == NULL) {
        printf("not found t_monster\n");
        return -2;
    }

    cfg_parser cp;
    for (int i = 1; i <= _table->m_recordCount; ++i) {
        uint32 id = _table->GetField      (i, "MonsterID"   );
        if (id  == 0) {
            continue;
        }

        if (_cb.find(id) != _cb.end()) {
            printf("%s:%d %s, config_npc repeat id = %d\n", __FILE__, __LINE__, __FUNCTION__, id);
            continue;
        }

        config_npc* p = new config_npc;
        p->_npc_id               = _table->GetField      (i, "MonsterID"         );
        p->_npc_name             = _table->GetFieldString(i, "MonsterName"       );
		p->_npc_name1			 = _table->GetFieldString(i, "MonsterName1");
        p->_npc_type             = _table->GetField      (i, "MonsterType"       );
        p->_monster_level        = _table->GetField      (i, "MonsterLevel"      );
        p->_attack_mode          = _table->GetField      (i, "AttackMode"        );
        p->_attack_cool          = _table->GetField      (i, "AttackCool"        );
        p->_random_move          = _table->GetField      (i, "RandomMove"        );
        p->_move_cool            = _table->GetField      (i, "MoveCool"          );
        p->_experience           = _table->GetField      (i, "Experience"        );
        p->_random_move_range    = _table->GetField      (i, "RandomMoveRange"   );
        p->_visible_range        = _table->GetField      (i, "VisibleRange"      );
        p->_follow_range         = _table->GetField      (i, "FollowRange"       );
        p->_default_skill        = _table->GetField      (i, "DefaultSkill"      );
        p->_monster_ai           = _table->GetFieldString(i, "MonsterAI"         );
        p->_rebirth_time         = _table->GetField      (i, "RebirthTime"       );
        p->_corpse_remain_time   = _table->GetField      (i, "CorpseRemainTime"  );
        p->_monster_difficulty_1 = _table->GetField      (i, "MonsterDifficulty1");
        p->_collect_time_long	 = _table->GetField      (i, "GetAcquisitionTime");
        p->_is_remove_pet        = _table->GetField      (i, "IsRemovePet"       );
        p->_monster_disappear    = _table->GetField      (i, "MonsterDisappear"  );

        cpf p1(&p->_drop_list);
        cp.parser<cpf>(_table->GetFieldString(i, "DropList").c_str(), ";", p1);

        cpf_0 p0(&p->_game_attribute);
        cp.parser<cpf_0>(_table->GetFieldString(i, "GameAttribute").c_str(), ";", p0);

        cpf_2 p2(&p->_random_skill);
        cp.parser<cpf_2>(_table->GetFieldString(i, "RandomSkill").c_str(), ";", p2);

        for (size_t j = 0; j < p->_drop_list.size(); ++j) {
            std::map<uint32, config_drop*>::iterator it = _cd.find(p->_drop_list[j]);
            if (it != _cd.end()) {
                p->_cds.push_back(it->second);
            } 
        }

        char buf[32] = {0};
        for (int j = 1; j <= 9; ++j) {
            sprintf(buf, "Skill%d", j);
            uint32 skill_id = _table->GetField(i, buf);
            if (skill_id != 0) {
                p->_skills.push_back(skill_id);
            }
        }

        p->_skills.push_back(p->_default_skill);

		cpf p3(&p->_zombieDropList);
		cp.parser<cpf>(_table->GetFieldString(i, "ZombieDrop").c_str(), ";", p3);

        cpf immunity_effect(&p->_immunity_effect);
        cp.parser<cpf>(_table->GetFieldString(i, "ImmunityEffect").c_str(), ";", immunity_effect);
        cpf immunity_skill(&p->_immunity_skill);
        cp.parser<cpf>(_table->GetFieldString(i, "ImmunitySkill").c_str(), ";", immunity_skill);

        _cb.insert(std::pair<uint32, config_npc*>(p->_npc_id, p));
    }

    return 0;
}

int config_npc_mgr::clear()
{
    for (std::map<uint32, config_npc*>::iterator i = _cb.begin(); i != _cb.end(); ++i) {
        if (i->second != NULL) {
            delete i->second;
        }
    }

    _cb.clear();

    for (std::map<uint32, config_drop*>::iterator i = _cd.begin(); i != _cd.end(); ++i) {
        if (i->second != NULL) {
            delete i->second;
        }
    }

    _cd.clear();

    return 0;
}

const config_npc* config_npc_mgr::get_config_npc(uint32 id) const
{
    std::map<uint32, config_npc*>::const_iterator i = _cb.find(id);
    if (i == _cb.end()) {
        return NULL;
    }

    return i->second;
}

const config_drop* config_npc_mgr::get_config_drop(uint32 id) const
{
    std::map<uint32, config_drop*>::const_iterator i = _cd.find(id);
    if (i == _cd.end()) {
        return NULL;
    }

    return i->second;
}


config_npc *config_npc_mgr::generate_config_npc(uint32 role_id, uint32 config_id, uint32 level, uint32 type, uint32 disappear) const
{
#ifndef __LIB_MINI_SERVER__
	config_npc *cn = new config_npc;
	cn->_npc_id        = role_id;
	cn->_monster_level = level;
	const config_robot *robot = CONFMGR->get_config_robot(config_id);
	if (robot == NULL) 
	{
		delete cn;
		return NULL;
	}
	cn->_experience         = 0;
	if(type != 0)
	{
		cn->_npc_type       = type;
	}
	else
	{
		cn->_npc_type		= robot->_monster_type;
	}
	cn->_follow_range       = robot->_follow_range;
	cn->_visible_range      = robot->_visible_range;
	cn->_corpse_remain_time = robot->_corpse_remain_time;
	cn->_attack_cool        = robot->_attack_cool;
	cn->_attack_mode		= robot->_attack_mode;
	cn->_default_skill      = robot->_default_skill;
	cn->_monster_ai         = robot->get_robot_ai();
	cn->_skills             = robot->get_skills();
	cn->_rebirth_time		= 0;
	cn->_random_move_range  = robot->_random_move_range;
	cn->_monster_disappear  = disappear;
	return cn;
#else
	return NULL;
#endif
}

