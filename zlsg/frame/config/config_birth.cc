#include "cfg_parser.h"
#include "config_birth.h"

config_birth::config_birth(void)
{
}

config_birth::~config_birth(void)
{
}

config_birth_mgr::config_birth_mgr(TTableGroup_s* p)
    : _birth(p)
{
    init(p);
}


config_birth_mgr::~config_birth_mgr(void)
{
    clear();
}

int config_birth_mgr::init(TTableGroup_s* p)
{
    if (p == NULL) {
        return -1;
    }

    _birth = p;
    _table = _birth->GetTable("birth");
    if (_table == NULL) {
        printf("not found birth\n");
        return -2;
    }

    cfg_parser cp;
    for (int i = 1; i <= _table->m_recordCount; ++i) {
        config_birth* p = new config_birth;
        p->_job              = _table->GetField(i, "Job"           );
        p->_scene_id         = _table->GetField(i, "SceneID"       );
        p->_default_skill_id = _table->GetField(i, "DefaultSkillID");
        p->_quest_id         = _table->GetField(i, "QuestID"       );

        cpf f;
        f.set_cfg(&p->_skill_list);
        cp.parser<cpf>(_table->GetFieldString(i, "SkillList").c_str(), ";", f);

        cpf_0 f0;
        f0.set_cfg(&p->_equip);
        cp.parser<cpf_0>(_table->GetFieldString(i, "Equip").c_str(), ";", f0);
        f0.set_cfg(&p->_item_list);
        cp.parser<cpf_0>(_table->GetFieldString(i, "ItemList").c_str(), ";", f0);

        _cb.insert(std::pair<uint32, config_birth*>(p->_job, p));
    }

    return 0;
}

int config_birth_mgr::clear()
{
    for (std::map<uint32, config_birth*>::iterator i = _cb.begin(); i != _cb.end(); ++i) {
        if (i->second != NULL) {
            delete i->second;
        }
    }

    _cb.clear();

    return 0;
}

const config_birth* config_birth_mgr::get_config_birth(uint32 job) const
{
    std::map<uint32, config_birth*>::const_iterator i = _cb.find(job);
    if (i == _cb.end()) {
        return NULL;
    }
    return i->second;
}
