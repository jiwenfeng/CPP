#include "cfg_parser.h"
#include "config_gem_act.h"

config_gem_act::config_gem_act(void)
{
}

config_gem_act::~config_gem_act(void)
{
}

config_gem_act_mgr::config_gem_act_mgr(TTableGroup_s* p)
        : _gem_act(p)
{
    init(p);
}


config_gem_act_mgr::~config_gem_act_mgr(void)
{
    clear();
}

int config_gem_act_mgr::init(TTableGroup_s* p)
{
    if (p == NULL) {
        return -1;
    }

    _gem_act = p;
    _table = _gem_act->GetTable("t_gemactivation");
    if (_table == NULL) {
        printf("not found t_gemactivation\n");
        return -2;
    }

    cfg_parser cp;
    for (int i = 1; i <= _table->m_recordCount; ++i) {
        if (_table->GetField      (i, "ActivationID"         ) == 0) {
            continue;
        }

        config_gem_act* p = new config_gem_act;
        p->gem_act_id                         = _table->GetField      (i, "ActivationID"				 );
		p->act_type							  = _table->GetField      (i, "ActivationType"               );
		p->sequence							  = _table->GetField      (i, "Sequence"					 );

		cpf_1 f0;
		f0.set_cfg(&p->act_cond);
		cp.parser<cpf_1>(_table->GetFieldString(i, "ActivationCondition").c_str(), ";", f0);

		cpf_1 f1;
		f1.set_cfg(&p->act_attr);
		cp.parser<cpf_1>(_table->GetFieldString(i, "ActivationAttribute").c_str(), ";", f1);

		_cp.insert(std::pair<int32, config_gem_act*>(p->gem_act_id,p));
    }

    return 0;
}

int config_gem_act_mgr::clear()
{
    for (std::map<uint32, config_gem_act*>::iterator i = _cp.begin(); i != _cp.end(); ++i) {
        if (i->second != NULL) {
            delete i->second;
        }
    }

    _cp.clear();

    return 0;
}

const config_gem_act* config_gem_act_mgr::get_config_gem_act(uint32 gem_act_id) const
{
    std::map<uint32, config_gem_act*>::const_iterator itr = _cp.find(gem_act_id);

	if (itr != _cp.end()){
		return itr->second;
	}

	return NULL;
}

uint32 config_gem_act_mgr::get_config_gem_act(uint32 act_type,uint32 cond_value) const
{
	uint32 hole_count = 0;
	std::map<uint32, config_gem_act*>::const_iterator itr = _cp.begin();

	for (; itr != _cp.end(); ++itr){
		if (itr->second->get_act_type() == act_type){
			uint32 cur_cond = 0;
			const std::map<int, int>& act_con_map_ = itr->second->get_act_cond();
			std::map<int, int>::const_iterator it = act_con_map_.begin();
			for (; it != act_con_map_.end(); ++it){
				cur_cond = it->second;
				break;
			}

			if (cond_value >= cur_cond){
				hole_count = itr->second->get_sequence();
			}

		}
	}

	return hole_count;
}

void config_gem_act_mgr::get_config_gem_act_attr(uint32 act_type,uint32 cond_value,std::multimap<int,int>& act_attr_map_)const
{

	std::map<uint32, config_gem_act*>::const_iterator itr = _cp.begin();


	for (; itr != _cp.end(); ++itr){
		if (itr->second->get_act_type() == act_type){
			uint32 cur_cond = 0;
			const std::map<int, int>& act_con_map_ = itr->second->get_act_cond();
			std::map<int, int>::const_iterator it = act_con_map_.begin();
			for (; it != act_con_map_.end(); ++it){
				cur_cond = it->second;
				break;
			}

			if (cond_value >= cur_cond){
				const std::map<int, int>& attr_map_ = itr->second->get_act_attr();
				std::map<int, int>::const_iterator it_attr = attr_map_.begin();
				for (;it_attr != attr_map_.end();++it_attr){
					act_attr_map_.insert(std::pair<int,int>(it_attr->first,it_attr->second));
				}
			}

		}
	}

}

uint32 config_gem_act_mgr::get_gem_skill_max_count()const
{
	uint32 skill_max_count = 0;

	std::map<uint32, config_gem_act*>::const_iterator itr = _cp.begin();

	for (; itr != _cp.end(); ++itr){
		if (itr->second->get_act_type() == 3){
			++skill_max_count;
		}
	}

	return skill_max_count;
}