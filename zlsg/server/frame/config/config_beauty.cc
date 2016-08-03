#include "cfg_parser.h"
#include "config_beauty.h"

config_beauty::config_beauty(void)
{
}

config_beauty::~config_beauty(void)
{
}

config_beauty_mgr::config_beauty_mgr(TTableGroup_s* p)
        : _beauty(p)
{
    init(p);
}


config_beauty_mgr::~config_beauty_mgr(void)
{
    clear();
}

int config_beauty_mgr::init(TTableGroup_s* p)
{
    if (p == NULL) {
        return -1;
    }

    _beauty = p;
    _table = _beauty->GetTable("t_general");
    if (_table == NULL) {
        printf("not found general\n");
        return -2;
    }

    cfg_parser cp;
    for (int i = 1; i <= _table->m_recordCount; ++i) {
        if (_table->GetField      (i, "general_id"         ) == 0) {
            continue;
        }

        config_beauty* p = new config_beauty;
        p->general_id                           = _table->GetField      (i, "general_id"              );
		p->general_name                         = _table->GetFieldString(i, "general_name"            );
		p->general_level                        = _table->GetField      (i, "general_level"           );
		p->master_level                         = _table->GetField      (i, "master_level"            );
		p->up_need_exp							= _table->GetField      (i, "up_need_exp"             );
		p->fight_capacity						= _table->GetField		(i, "fight_capacity"          );
		p->fight_hppercent						= _table->GetField		(i, "fight_hppercent"		  );
		p->fight_atkpercent						= _table->GetField		(i, "fight_atkpercent"		  );
		p->fight_defpercent						= _table->GetField		(i, "fight_defpercent"		  );
		p->magic_id								= _table->GetField		(i, "GmagicID"				  );
		p->recruit_level						= _table->GetField		(i, "recruit_level"			  );
		p->skill_belong							= _table->GetField		(i, "GeneralSkill"			  );
		p->fit_times							= _table->GetField		(i, "fit_times"				  );
		cpf_0 p1;// map
		p1.set_cfg(&p->fit_need_item);
		cp.parser<cpf_0>(_table->GetFieldString(i, "fit_need_item").c_str(), ";", p1);

		cpf_0 p2;
		p2.set_cfg(&p->fit_add_property);
		cp.parser<cpf_0>(_table->GetFieldString(i, "fit_add_property").c_str(), ";", p2);

		cpf_0 p4;
		p4.set_cfg(&p->recruit_need_item);
		cp.parser<cpf_0>(_table->GetFieldString(i, "recruit_need_item").c_str(), ";", p4);

		cpf p5; // vector
		p5.set_cfg(&p->general_skill);
		cp.parser<cpf>(_table->GetFieldString(i, "general_skill").c_str(), ";", p5);

		cpf_0 p6;
		p6.set_cfg(&p->game_attribute);
		cp.parser<cpf_0>(_table->GetFieldString(i, "Gattribute").c_str(), ";", p6);

		//if (check_is_have(p->general_id,p->general_level)){
		//	_cp.insert(std::pair<uint32, config_beauty*>(p->general_id,p));
		//}else{
		//	delete p;
		//	p = NULL;
		//}

		if(!_cp.insert(std::make_pair(p->general_id, p))->second){
			delete p;
			p = NULL;
		}
    }

    return 0;
}

int config_beauty_mgr::clear()
{
    for (std::multimap<uint32, config_beauty*>::iterator i = _cp.begin(); i != _cp.end(); ++i) {
        if (i->second != NULL) {
            delete i->second;
        }
    }

    _cp.clear();

    return 0;
}

const config_beauty* config_beauty_mgr::get_config_beauty(uint32 general_id,uint32 general_level) const
{
	 std::multimap<uint32, config_beauty*>::const_iterator itr = _cp.find(general_id);
	 for (;itr != _cp.end(); ++itr){
		 if (itr->second->get_general_level() == general_level && itr->first == general_id){
			 return itr->second;
		 }
	 }

	return NULL;
}

const config_beauty* config_beauty_mgr::get_config_beauty_by_lingering_count(uint32 general_id,uint32 lingering_count) const
{

	std::multimap<uint32, config_beauty*>::const_iterator itr = _cp.find(general_id);
	for (;itr != _cp.end(); ++itr){
		if (itr->second->get_fit_times() == lingering_count && itr->first == general_id){
			return itr->second;
		}
	}

	return NULL;
}

bool config_beauty_mgr::check_is_have(uint32 id,uint32 level)
{
	std::multimap<uint32, config_beauty*>::iterator it = _cp.find(id);
	for (; it != _cp.end();++it){
		if (it->second->get_general_level() == level && it->first == id){
			return false;
		}
	}

	return true;
}